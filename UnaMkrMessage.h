/*
 *  Library of communication between MKRZERO and UnaMKR 
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 */
#ifndef  __UNAMKR_MESSAGE__
#define  __UNAMKR_MESSAGE__

typedef enum e_snr_type
{
    Temperature      = 1,
    Humidity         = 2,
    Pressure         = 3,
    IndoorAirQuality = 4,
    Acceletometer    = 5,
    Magnetometer     = 6,
    ReedSwitch       = 7,
    LightSensor      = 8,
    InfraredSensor   = 9,
    RCZone           = 10,

} mkr_snr_t;

typedef enum e_user_snr_type
{
    UserDefined_1    = 11,
    UserDefined_2    = 12,
    UserDefined_3    = 13,
    UserDefined_4    = 14,
    UserDefined_5    = 15,
    
} usr_snr_t;

typedef enum e_bit_length
{
    BitLength_4_Bits  = 0x00,
    BitLength_8_Bits  = 0x01,
    BitLength_12_Bits = 0x02,
    BitLength_16_Bits = 0x03,
    BitLength_20_Bits = 0x04,
    BitLength_24_Bits = 0x05,
    BitLength_28_Bits = 0x06,
    BitLength_32_Bits = 0x07,
    BitLength_36_Bits = 0x08,
    BitLength_40_Bits = 0x09,
    BitLength_44_Bits = 0x0A,
    BitLength_48_Bits = 0x0B,

} bit_len_t;

typedef struct s_bcd_u8
{
    uint8_t   bcd_l: 4;   
    uint8_t   bcd_h: 4;
    
} bcd_u8_t;

typedef union u_complex_data
{
    uint8_t  raw[8];
    
    int8_t   s8;
    uint8_t  u8;
    
    int16_t  s16;
    uint16_t u16;
    
    int32_t  s32;
    uint32_t u32;
  
    int64_t  s64;
    uint64_t u64;
    
} complex_data_t;

#define max_payload_num  5

class UnaMkrMessage
{
public:
    /* constructor & destructor */
    UnaMkrMessage() 
    {
        clear();
        setScale(Temperature, 100.0);
        setScale(Humidity, 100.0);
        setScale(Pressure, 0.1);
        setScale(IndoorAirQuality, 0.01);
        setScale(Acceletometer, 1);
        setScale(Magnetometer, 0.1);
        setScale(LightSensor, 1);
        setScale(InfraredSensor, 1);
        timeout_SendFrame = 10000;     // default 10 seconds
    }
    ~UnaMkrMessage()
    {
        clear();
    }

    /* clear payload */
    void clear(void);

    /* Config Scale for Soecific Sensor */
    bool setScale(mkr_snr_t snr, float scale);
    bool setScale(usr_snr_t snr, float scale);

    /* Add RC Field */
    bool addField_RCZone(uint8_t zone);

    /* Add Temperature Field */
    bool addField_Temperature(float value);
    
    /* Add Humidity Field */
    bool addField_Humidity(float value);

    /* Add Pressure Field */
    bool addField_Pressure(float value);

    /* Add IAQ Field */
    bool addField_IndoorAirQuality(uint32_t value);

    /* Add Acceletometer Field */
    bool addField_Acceletometer(int32_t *axes_xyz);

    /* Add Magnetometer Field */
    bool addField_Magnetometer(int32_t *axes_xyz);
    
    /* Add Ambient Light Field */
    bool addField_LightSensor(uint16_t visiable_value);

    /* Add Ambient Light Field (visible & infrared) */
    bool addField_InfraredSensor(uint16_t ir_value);

    /* Add Reed Switch Field */
    bool addField_ReedSwitch(bool detected);

    /* Add User=Defined Field */
    bool addField_User(usr_snr_t sensor, bit_len_t bit_len, double value);
    bool addField_User(usr_snr_t sensor, bit_len_t bit_len, complex_data_t value);

    /* Print Payload Message */
    bool print_msg(void);

    /* Get Payload information */
    int  getPayloadNum(void){return pl_num;}
    bool getHexData(int payload_index, byte *output, int *length);
    
    /* Setup SF timeout */
    bool setTimeout(int timeout);
    int  getTimeout(void);

private:
    bool add_snr_field(uint8_t snr_type, bit_len_t bit_len, uint8_t *data);

    /* payload stack */
    int  pl_num;
    int  pl_bcd_offset[max_payload_num];

    union u_payload
    {
        uint8_t  raw[12];
        bcd_u8_t bcd_raw[12];

    } payload[max_payload_num];
    
    /* Scale for Each Sensors */
    float  scale_temperature;
    float  scale_humidity;
    float  scale_pressure;
    float  scale_gas;
    float  scale_acceletometer;
    float  scale_magnetometer;
    float  scale_visible;
    float  scale_infrared;
    float  scale_user[5];

    /* timeout for uplink */
    int   timeout_SendFrame;
};

/* UnaShield Message */
class UnaShieldMessage
{
public:
    UnaShieldMessage() {clear();}
    ~UnaShieldMessage(){}
    
    /* clear payload */
    void clear(void)
    {
        Data_num=0;
        payload_length=0;
        memset(Data.payload, 0, 12*sizeof(byte));
    }

    /* add int field, imitation message in UnaShieldV2S2 */
    bool addField(const char* name, int field)
    {
        return setField(name, 10*field);
    }
    /* add float field, imitation message in UnaShieldV2S2 */
    bool addField(const char* name, float field)
    {
        return setField(name, (int)(10.0*field));
    }
    /* add double field, imitation message in UnaShieldV2S2 */
    bool addField(const char* name, double field)
    {
        return setField(name, (int)(10.0*field));
    }

    /* encoded hex data */
    bool getHexData(byte *output, int *length)
    {
        if (!output || !length)
            return false;

        memcpy(output, Data.payload, payload_length);
        *length = payload_length;
        return true;
    }

private:
    bool setField(const char *in, int value)
    {
        char c[3];
        short v;

        if (!in || Data_num >= 3 )
            return false;

        v = (short) value;

        for (int i=0; i<3; i++)
        {
            c[i] = encodeName(in[i]);
        }

        if (c[0] && c[1] && c[2])
        {
            // 5-bit values
            Data.s_payload[Data_num].str1  = c[0] << 2;
            Data.s_payload[Data_num].str1 |= ((c[1] & 0x18) >> 3);
            Data.s_payload[Data_num].str2  = ((c[1] & 0x07) << 5);
            Data.s_payload[Data_num].str2 |= ((c[2] & 0x1F));
            Data.s_payload[Data_num].value = v;
            
            Data_num++;
            payload_length += 4;
            
            // Serial.println("Message: decode success: ");
            // Serial.println((int)c[0]);
            // Serial.println((int)c[1]);
            // Serial.println((int)c[2]);
            // Serial.println((int)v);
            // Serial.println("  ");
            
            return true;
        }
        else
        {
            Serial.println("Message: decode failed");
            return false;
        }
    }

    /*  */
    bool decode(void){return false;}

    /* Convert character ch to the 5-bit equivalent */
    char encodeName(const char in)
    {
        char c = in;
        
        //  Convert to lowercase.
        if (c >= 'A' && c <='Z')
            c = c -'A' + 'a';

        if (c >= 'a' && c <='z')
            c = c - 'a' + firstLetter;

        //  For 1, 2, ... return the digit
        else if (c >= '0' && c <= '9')
            c = c - '0' + firstDigit;

        //  Failed
        else
            c = 0;

        return c;
    }

    /* payload stack */
    union __DATA__
    {
        byte payload[12];

        struct __s_payload__
        {
            unsigned char  str1;
            unsigned char  str2;
            unsigned short value;

        } s_payload[3];

    } Data;

    /* number of field */
    int  Data_num;

    /* length of hex payload */
    int  payload_length;

    /* const for encode/decode */
    const char firstLetter = 1;
    const char firstDigit  = 27;

};

#endif //__UNAMKR_MESSAGE__
