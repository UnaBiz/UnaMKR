/*
 *  Library of communication between MKRZERO and UnaMKR 
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 */

#ifdef ARDUINO
  #if (ARDUINO >= 100)
    #include <Arduino.h>
  #else  //  ARDUINO >= 100
    #include <WProgram.h>
  #endif  //  ARDUINO  >= 100
#endif  //  ARDUINO

#include "UnaMkrMessage.h"

// new payload format
#define UNAMKR_MSG_HEADER   1

static uint8_t calc_crc8(uint8_t *buf, int length) {
  uint8_t crc = 0x00;
  uint8_t MSB;
  int i,j;

  for (i = 0; i < length; i++) {
    crc ^= buf[i];
    for (j = 0; j < 8; j++) {
      MSB = crc & 0x80;
      crc = crc << 1;
      if (MSB) crc ^= 0x07;
    }
  }
  return crc;
}

void UnaMkrMessage :: clear(void)
{
    pl_num = 0;
    memset(pl_bcd_offset, 0, sizeof(pl_bcd_offset));
    memset(payload, 0, sizeof(payload));
}

bool UnaMkrMessage :: setScale(mkr_snr_t snr, float scale)
{
    bool status = true;

    switch (snr)
    {
        case Temperature:
            scale_temperature = scale;
            break;

        case Humidity:
            scale_humidity = scale;
            break;

        case Pressure:
            scale_pressure = scale;
            break;

        case IndoorAirQuality:
            scale_gas = scale;
            break;

        case Acceletometer:
            scale_acceletometer = scale;
            break;

        case Magnetometer:
            scale_magnetometer = scale;
            break;

        case LightSensor:
            scale_visible = scale;
            break;

        case InfraredSensor:
            scale_infrared= scale;
            break;

        case ReedSwitch:
        case RCZone:
        default:
            status = false;
            break;
    }

    return status;
}

bool UnaMkrMessage :: setScale(usr_snr_t snr, float scale)
{
    bool status = true;
    switch (snr)
    {
        case UserDefined_1:
            scale_user[0] = scale;
            break;

        case UserDefined_2:
            scale_user[1] = scale;
            break;

        case UserDefined_3:
            scale_user[2] = scale;
            break;

        case UserDefined_4:
            scale_user[3] = scale;
            break;

        case UserDefined_5:
            scale_user[4] = scale;
            break;

        default:
            status = false;
            break;
    }

    return status;
}

bool UnaMkrMessage :: addField_RCZone(uint8_t zone)
{
    uint8_t data_be = (zone << 4) & 0xF0;
    return add_snr_field(RCZone, BitLength_4_Bits, &data_be);
}

bool UnaMkrMessage :: addField_Temperature(float value)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit: *C, 
    // 
    // data unit: *C
    // default scale: x100, 
    // ex: 21.25 *C

    data = (int16_t)(value * scale_temperature);

    data_be[0] = (((uint16_t)data) >> 8) & 0xFF;
    data_be[1] = (((uint16_t)data)     ) & 0xFF;
    
    return add_snr_field(Temperature, BitLength_16_Bits, data_be);
}

/* Add Humidity Field */
bool UnaMkrMessage :: addField_Humidity(float value)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit: %, 
    // 
    // data unit: %
    // default scale: x100, 
    // ex: 32.92 %

    data = (int16_t)(value * scale_humidity);

    data_be[0] = (((uint16_t)data) >> 8) & 0xFF;
    data_be[1] = (((uint16_t)data)     ) & 0xFF;
    
    return add_snr_field(Humidity, BitLength_16_Bits, data_be);
}

/* Add Pressure Field */
bool UnaMkrMessage :: addField_Pressure(float value)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit: Pa, 
    // 
    // data unit: hPa
    // default scale: x10, 
    // ex: 1002.2 hPa

    data = (int16_t)(value * scale_pressure);

    data_be[0] = (((uint16_t)data) >> 8) & 0xFF;
    data_be[1] = (((uint16_t)data)     ) & 0xFF;
    
    return add_snr_field(Pressure, BitLength_16_Bits, data_be);
}

/* Add IAQ Field */
bool UnaMkrMessage :: addField_IndoorAirQuality(uint32_t value)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit: Ohms, 
    // 
    // data unit: kOhms
    // default scale: x10 

    data = (int16_t)(value * scale_gas);

    data_be[0] = (((uint16_t)data) >> 8) & 0xFF;
    data_be[1] = (((uint16_t)data)     ) & 0xFF;
    
    return add_snr_field(IndoorAirQuality, BitLength_16_Bits, data_be);
}

/* Add Acceletometer Field */
bool UnaMkrMessage :: addField_Acceletometer(int32_t *axes_xyz)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit    : mg, 
    // input interval: +-2048
    //
    // data unit: mg

    data = (int16_t) (axes_xyz[0] * scale_acceletometer);    // 12 bits
    data_be[0] = (((uint16_t)data) >> 4) & 0xFF;    // bit 11 ~ 4
    data_be[1] = (((uint16_t)data) << 4) & 0xF0;    // bit  3 ~ 0 at high

    data = (int16_t) (axes_xyz[1] * scale_acceletometer);    // 12 bits
    data_be[1]|= (((uint16_t)data) >> 8) & 0x0F;    // bit 11 ~ 8 at low
    data_be[2] = (((uint16_t)data)     ) & 0xFF;    // bit 7 ~ 0

    data = (int16_t) (axes_xyz[2] * scale_acceletometer);    // 12 bits
    data_be[3] = (((uint16_t)data) >> 4) & 0xFF;    // bit 11 ~ 4
    data_be[4] = (((uint16_t)data) << 4) & 0xF0;    // bit  3 ~ 0 at high

    return add_snr_field(Acceletometer, BitLength_36_Bits, data_be);
}

/* Add Magnetometer Field */
bool UnaMkrMessage :: addField_Magnetometer(int32_t *axes_xyz)
{
    uint8_t data_be[12];
    int16_t data;

    // input  unit: mGauss
    // data unit: Gause
    
    data = (int16_t) (axes_xyz[0] * scale_magnetometer);    // 12 bits
    data_be[0] = (((uint16_t)data) >> 4) & 0xFF;    // bit 11 ~ 4
    data_be[1] = (((uint16_t)data) << 4) & 0xF0;    // bit  3 ~ 0 at high

    data = (int16_t) (axes_xyz[1] * scale_magnetometer);    // 12 bits
    data_be[1]|= (((uint16_t)data) >> 8) & 0x0F;    // bit 11 ~ 8 at low
    data_be[2] = (((uint16_t)data)     ) & 0xFF;    // bit 7 ~ 0

    data = (int16_t) (axes_xyz[2] * scale_magnetometer);    // 12 bits
    data_be[3] = (((uint16_t)data) >> 4) & 0xFF;    // bit 11 ~ 4
    data_be[4] = (((uint16_t)data) << 4) & 0xF0;    // bit  3 ~ 0 at high

    return add_snr_field(Magnetometer, BitLength_36_Bits, data_be);
}
    
/* Add Ambient Light Field */
bool UnaMkrMessage :: addField_LightSensor(uint16_t visiable_value)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit:  
    // 
    // data unit:
    // default scale: x1, 
    // ex: 1002
    data = (uint16_t) (visiable_value * scale_visible);
    data_be[0] = (data >> 8) & 0xFF;
    data_be[1] = (data     ) & 0xFF;
    
    return add_snr_field(LightSensor, BitLength_16_Bits, data_be);
}

/* Add Ambient Infrared Field */
bool UnaMkrMessage :: addField_InfraredSensor(uint16_t ir_value)
{
    uint8_t data_be[12];
    int16_t data;

    // input unit: 
    // 
    // data unit: 
    // default scale: x1, 
    // ex: 1002
    data = (uint16_t) (ir_value * scale_infrared);
    data_be[0] = (data >> 8) & 0xFF;
    data_be[1] = (data     ) & 0xFF;
    
    return add_snr_field(LightSensor, BitLength_16_Bits, data_be);
}

bool UnaMkrMessage :: addField_ReedSwitch(bool detected)
{
    uint8_t data_be;
    // input unit: NAN, 
    // 
    // data unit: NAN
    // scale    : NAN 
    // ex: 1

    data_be = (detected) ? (0x10) : (0x00);
    return add_snr_field(ReedSwitch, BitLength_4_Bits, &data_be);
}

bool UnaMkrMessage :: addField_User(usr_snr_t snr, bit_len_t bit_len, double value)
{
    uint8_t data_be[12];
    uint16_t u16_v;
    uint32_t u32_v;
    uint64_t u64_v;
    double data, scale;

    switch (snr)
    {
        case UserDefined_1:
            scale = scale_user[0];
            break;

        case UserDefined_2:
            scale = scale_user[1];
            break;

        case UserDefined_3:
            scale = scale_user[2];
            break;

        case UserDefined_4:
            scale = scale_user[3];
            break;

        case UserDefined_5:
            scale = scale_user[4];
            break;

        default:
            scale = 1;
            break;
    }

    // float to unsigned int with big endian
    data = scale * value;

    switch (bit_len)
    {
        case BitLength_4_Bits :
            data_be[0] = (((uint8_t) data) & 0x0F) << 4;
            break;

        case BitLength_8_Bits :
            data_be[0] = (((uint8_t) data) & 0xFF);
            break;

        case BitLength_12_Bits:
            u16_v = (uint16_t) data;
            data_be[0] = (u16_v >> 4) & 0xFF;
            data_be[1] = (u16_v << 4) & 0xF0;
            break;

        case BitLength_16_Bits:
            u16_v = (uint16_t) data;
            data_be[0] = (u16_v >> 8) & 0xFF;
            data_be[1] = (u16_v     ) & 0xFF;
            break;

        case BitLength_20_Bits:
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>12) & 0xFF;
            data_be[1] = (u32_v >> 4) & 0xFF;
            data_be[2] = (u32_v << 4) & 0xF0;
            break;

        case BitLength_24_Bits:
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>16) & 0xFF;
            data_be[1] = (u32_v >> 8) & 0xFF;
            data_be[2] = (u32_v     ) & 0xFF;
            break;

        case BitLength_28_Bits:
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>20) & 0xFF;
            data_be[1] = (u32_v >>12) & 0xFF;
            data_be[2] = (u32_v >> 4) & 0xFF;
            data_be[3] = (u32_v << 4) & 0xF0;
            break;

        case BitLength_32_Bits:
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>24) & 0xFF;
            data_be[1] = (u32_v >>16) & 0xFF;
            data_be[2] = (u32_v >> 8) & 0xFF;
            data_be[3] = (u32_v     ) & 0xFF;
            break;

        case BitLength_36_Bits:
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>28) & 0xFF;
            data_be[1] = (u64_v >>20) & 0xFF;
            data_be[2] = (u64_v >>12) & 0xFF;
            data_be[3] = (u64_v >> 4) & 0xFF;
            data_be[4] = (u64_v << 4) & 0xFF;
            break;

        case BitLength_40_Bits:
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>32) & 0xFF;
            data_be[1] = (u64_v >>24) & 0xFF;
            data_be[2] = (u64_v >>16) & 0xFF;
            data_be[3] = (u64_v >> 8) & 0xFF;
            data_be[4] = (u64_v     ) & 0xFF;
            break;

        case BitLength_44_Bits:
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>36) & 0xFF;
            data_be[1] = (u64_v >>28) & 0xFF;
            data_be[2] = (u64_v >>20) & 0xFF;
            data_be[3] = (u64_v >>12) & 0xFF;
            data_be[4] = (u64_v >> 4) & 0xFF;
            data_be[5] = (u64_v << 4) & 0xFF;
            break;

        case BitLength_48_Bits:
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>40) & 0xFF;
            data_be[1] = (u64_v >>32) & 0xFF;
            data_be[2] = (u64_v >>24) & 0xFF;
            data_be[3] = (u64_v >>16) & 0xFF;
            data_be[4] = (u64_v >> 8) & 0xFF;
            data_be[5] = (u64_v     ) & 0xFF;
            break;

        default:
            return false;

    }

    return add_snr_field(snr, bit_len, data_be);
}

bool UnaMkrMessage :: addField_User(usr_snr_t sensor, bit_len_t bit_len, complex_data_t value)
{
    uint8_t data_be[12];
    uint16_t u16_v;
    uint32_t u32_v;
    uint64_t u64_v;
    double data;
    float  scale;

    switch (sensor)
    {
        case UserDefined_1:
            scale = scale_user[0];
            break;

        case UserDefined_2:
            scale = scale_user[1];
            break;

        case UserDefined_3:
            scale = scale_user[2];
            break;

        case UserDefined_4:
            scale = scale_user[3];
            break;

        case UserDefined_5:
            scale = scale_user[4];
            break;
            
        default:
            scale = 1;
            break;
    }

    // float to unsigned int with big endian
    switch (bit_len)
    {
        case BitLength_4_Bits :
            data = scale * value.s8;
            data_be[0] = (((uint8_t) data) & 0x0F) << 4;
            break;

        case BitLength_8_Bits :
            data = scale * value.s8;
            data_be[0] = (((uint8_t) data) & 0xFF);
            break;

        case BitLength_12_Bits:
            data = scale * value.s16;
            u16_v = (uint16_t) data;
            data_be[0] = (u16_v >> 4) & 0xFF;
            data_be[1] = (u16_v << 4) & 0xF0;
            break;

        case BitLength_16_Bits:
            data = scale * value.s16;
            u16_v = (uint16_t) data;
            data_be[0] = (u16_v >> 8) & 0xFF;
            data_be[1] = (u16_v     ) & 0xFF;
            break;

        case BitLength_20_Bits:
            data = scale * (int32_t)value.s32;
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>12) & 0xFF;
            data_be[1] = (u32_v >> 4) & 0xFF;
            data_be[2] = (u32_v << 4) & 0xF0;
            break;

        case BitLength_24_Bits:
            data = scale * (int32_t)value.s32;
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>16) & 0xFF;
            data_be[1] = (u32_v >> 8) & 0xFF;
            data_be[2] = (u32_v     ) & 0xFF;
            break;

        case BitLength_28_Bits:
            data = scale * (int32_t)value.s32;
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>20) & 0xFF;
            data_be[1] = (u32_v >>12) & 0xFF;
            data_be[2] = (u32_v >> 4) & 0xFF;
            data_be[3] = (u32_v << 4) & 0xF0;
            break;

        case BitLength_32_Bits:
            data = scale * (int32_t)value.s32;
            u32_v = (uint32_t) data;
            data_be[0] = (u32_v >>24) & 0xFF;
            data_be[1] = (u32_v >>16) & 0xFF;
            data_be[2] = (u32_v >> 8) & 0xFF;
            data_be[3] = (u32_v     ) & 0xFF;
            break;

        case BitLength_36_Bits:
            data = scale * (double)value.s64;
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>28) & 0xFF;
            data_be[1] = (u64_v >>20) & 0xFF;
            data_be[2] = (u64_v >>12) & 0xFF;
            data_be[3] = (u64_v >> 4) & 0xFF;
            data_be[4] = (u64_v << 4) & 0xFF;
            break;

        case BitLength_40_Bits:
            data = scale * (double)value.s64;
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>32) & 0xFF;
            data_be[1] = (u64_v >>24) & 0xFF;
            data_be[2] = (u64_v >>16) & 0xFF;
            data_be[3] = (u64_v >> 8) & 0xFF;
            data_be[4] = (u64_v     ) & 0xFF;
            break;

        case BitLength_44_Bits:
            data = scale * (double)value.s64;
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>36) & 0xFF;
            data_be[1] = (u64_v >>28) & 0xFF;
            data_be[2] = (u64_v >>20) & 0xFF;
            data_be[3] = (u64_v >>12) & 0xFF;
            data_be[4] = (u64_v >> 4) & 0xFF;
            data_be[5] = (u64_v << 4) & 0xFF;
            break;

        case BitLength_48_Bits:
            data = scale * (double)value.s64;
            u64_v = (uint64_t) data;
            data_be[0] = (u64_v >>40) & 0xFF;
            data_be[1] = (u64_v >>32) & 0xFF;
            data_be[2] = (u64_v >>24) & 0xFF;
            data_be[3] = (u64_v >>16) & 0xFF;
            data_be[4] = (u64_v >> 8) & 0xFF;
            data_be[5] = (u64_v     ) & 0xFF;
            break;

        default:
            return 2;

    }

    return add_snr_field((uint8_t)sensor, bit_len, data_be);
}



bool UnaMkrMessage :: print_msg(void)
{
    char *ptr, str[64];

    ptr = str;

    if (pl_num == 0)
    {
        Serial.print("Payload is empty\r\n");
        return true;
    }

    for (int i=0; i<pl_num; i++)
    {
        ptr = str + sprintf(str, "[Payload %d] ", i);
        int j;
#if UNAMKR_MSG_HEADER
        for (j=0; j<24; j+=2)   
#else
        for (j=0; j<pl_bcd_offset[i]; j+=2)
#endif
        {
            ptr += sprintf(ptr, "%02X", payload[i].raw[j/2]);
        }
        ptr += sprintf(ptr, "\r\n", i);
        Serial.print(str);
    }

    Serial.print("\r\n");
    return true;
}


bool UnaMkrMessage :: getHexData(int payload_index, byte *output, int *length)
{
    if (!output || !length)
        return false;

#if UNAMKR_MSG_HEADER
    if (payload_index < pl_num)
        *length = 12;
    else
        *length = 0;
#else
    if (payload_index < pl_num)
        *length = (pl_bcd_offset[payload_index]+1)/2;
    else
        *length = 0;
#endif

    if (*length)
    {
        memcpy(output, payload[payload_index].raw, *length);
        return true;
    }
    else
    {
        return false;
    }
}

bool UnaMkrMessage :: add_snr_field(uint8_t snr_type, bit_len_t b_len, uint8_t *data)
{
    uint8_t  pl_idx = 0;
    int8_t  bit_len = (((int8_t) b_len) + 1)*4;
    union u_payload *ptr;
    int  *offset, doff;
    uint8_t bit_offset;

    /* bit-length must be larger than 4 */
    if (bit_len < 4)
        return false;
    
    /* check remaining payload size */
#if UNAMKR_MSG_HEADER
    while (pl_bcd_offset[pl_idx] + bit_len/4 + 2 > 22)
#else
    while (pl_bcd_offset[pl_idx] + bit_len/4 + 2 > 24)
#endif
    {
        /* 
         * This payload does not have enough space to add this field. 
         * Check next payload
         */
        if (++pl_idx >= max_payload_num)
            return false;
    }

    /* update pointers and variables */
    if (pl_num < pl_idx + 1)            // update usage payload number
        pl_num = pl_idx + 1;
    ptr    = &payload[pl_idx];          // point to payload array
    offset = &pl_bcd_offset[pl_idx];    // paylaod offset (BCD)
    doff   = 0;                         // input data offset (BCD)

#if UNAMKR_MSG_HEADER
    // header
    if ((*offset) == 0)
    {
        ptr->bcd_raw[0].bcd_h = 0x0;
        (*offset)++;
    }
#endif

    /* check high-nibble or low-nibble */
    if ( (*offset)%2 == 0 )
    {
        ptr->bcd_raw[(*offset)/2].bcd_h = snr_type;
        (*offset)++;
        ptr->bcd_raw[(*offset)/2].bcd_l = (uint8_t)b_len;
        (*offset)++;
    }
    else
    {
        ptr->bcd_raw[(*offset)/2].bcd_l = snr_type;
        (*offset)++;
        ptr->bcd_raw[(*offset)/2].bcd_h = (uint8_t)b_len;
        (*offset)++;

        if ((doff%2) == 0)
            bit_offset = 4;
        else
            bit_offset = 0;

        ptr->bcd_raw[(*offset)/2].bcd_l = ((*(data + doff/2)) >> bit_offset) & 0x0F;
        (*offset)++;
        doff++;
        bit_len -= 4;
    }

    while (bit_len > 0)
    {
        //Serial.print(data[doff/2], HEX);

        /* check data offset (high-nibble) */
        if ((doff%2) == 0)
            bit_offset = 4;
        else
            bit_offset = 0;

        /* set high-nibble of bcd_raw */
        ptr->bcd_raw[(*offset)/2].bcd_h = ((*(data + doff/2)) >> bit_offset) & 0x0F;
        (*offset)++;
        doff++;
        bit_len -= 4;

        /* check remaing length of bits */ 
        if (bit_len <= 0)
            break;

        /* check again */
        if ((doff%2) == 0)
            bit_offset = 4;
        else
            bit_offset = 0;

        /* set low-nibble of bcd_raw */
        ptr->bcd_raw[(*offset)/2].bcd_l = ((*(data + doff/2)) >> bit_offset) & 0x0F;
        (*offset)++;
        doff++;
        bit_len -= 4;
    }

#if UNAMKR_MSG_HEADER
    // tailer
    payload[pl_idx].raw[11] = calc_crc8(payload[pl_idx].raw, 11);
#endif

    return true;
}


bool UnaMkrMessage :: setTimeout(int timeout)
{
    if (!timeout)
        return false;

    timeout_SendFrame = timeout;
    return true;
}

int UnaMkrMessage :: getTimeout(void)
{
    return timeout_SendFrame;
}