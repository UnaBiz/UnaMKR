/*
 *  Demo code:
 *      1. Ping UnaMKR
 *      2. Start LE scanning
 *      3. Parse advertisement (for IPS)
 *      4. SigFox uplink (beacon number and local coordinates)
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */
#include "UnaMKR.h"

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port
#define  PRINT_PARSER_LOG       0

/* Components */
UnaMKR my_mkr;

/* Definitions */
#define  TIME_BLE_SCAN          (3000)          // timeout for beacon detection, unit: milli-second
#define  TIME_WAIT_RESPONSE     (2500)          // timeout for response, unit: milli-second
#define  TINE_WAIT_UPLINK       (15000)         // timeout for uplink, unit: milli-second

typedef struct  __IPS__ {
    byte ips_Address[6];
    signed char ips_Rssi;
    byte ips_Flag;
    byte ips_Xaxis[4];
    byte ips_Yaxis[4];
    byte ips_Floor;
    byte ips_Tx;
    
} IndoorPosition_t;

/* Variables */
IndoorPosition_t my_ips[6];

/* IPS parser */
static void indoorPos_parser(UnaScanResult *sr);

/* Arduino Setup */
void setup() {
  
    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // MKR initialize
    my_mkr.begin(PUBLIC_KEY_USAGE);

    // Print response data
    my_mkr.logEnable(LOG_MESSAGE_DISPLAY);

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {
  
    char response[128];
    int  resp_len;

    // Check echo
    Serial.println("Ping UnaMKR");
    my_mkr.echo();
    my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

    // BLE scan
    Serial.print("Start BLE Scan, time = ");
    Serial.print(TIME_BLE_SCAN/1000);
    Serial.println(" second(s). Please wait a while...");
    my_mkr.btScan(TIME_BLE_SCAN, NO_RSSI_FILTER, ADV_IPS);
    
    // Wait response
    my_mkr.getData_CheckOk(TIME_BLE_SCAN + TIME_WAIT_RESPONSE);

    // Get scan result
    Serial.println("Get Scan Result");
    my_mkr.btGetScanResult();

    // Wait response
    int device_number = 0;
    UnaScanResult my_result;

    // Clear stack of ips structure
    memset(&my_ips[0], 0, sizeof(my_ips));
    
    // Read & parse result
    while (my_mkr.getData_ScanResult(&my_result, TIME_WAIT_RESPONSE)) {
        indoorPos_parser(&my_result);
    }


    // Sort result (according to RSSI)
    IndoorPosition_t (*ips1), (*ips2), tmp;
    int i, j;

    for (i=0; i<6; i++) {
        ips1 = &my_ips[i];

        if (!(ips1->ips_Flag & 0x01))
            continue;
            
        for (j=i+1; j<6; j++) {
            ips2 = &my_ips[j];
            
            if (!(ips2->ips_Flag & 0x01))
                continue;

            // move the larger one to the head of array
            if (ips1->ips_Rssi < ips2->ips_Rssi) {
                tmp   = *ips2;
                *ips2 = *ips1;
                *ips1 =  tmp;
            }
        }
    }

    // Uplink the scanned result
    char payload[12] = {0};
    char *pNum = &payload[0];
    char *pData= &payload[1];
    
    // Setup SigFox payload. 
    // The maximum beacon number is 3
    for (i=0; i<3; i++) {
        ips1 = &my_ips[i];
        
        if (!(ips1->ips_Flag & 0x01))
            break;
            
        (*pNum)++;
        *(pData++) = (char) ips1->ips_Rssi;
        *(pData++) = (char) ips1->ips_Xaxis[0];
        *(pData++) = (char) ips1->ips_Yaxis[0];

        // Beacon info
        char *pdb, ipslog[64];
        pdb = ipslog;
        pdb += sprintf(pdb, "[Beacon %d]\r\n\tRSSI = %d", i+1, (int) ips1->ips_Rssi);
        pdb += sprintf(pdb, "\r\n\tX-asis = %d", ips1->ips_Xaxis[0]);
        pdb += sprintf(pdb, "\r\n\tY-axis = %d", ips1->ips_Yaxis[0]);
        pdb += sprintf(pdb, "\r\n\r\n");
        Serial.print((const char*) ipslog);
    }
    Serial.println("Uplink IPS detection");
    my_mkr.uplink((const byte*)payload, 1 + (*pNum)*3);

    // Wait uplink response
    my_mkr.getData_CheckOk(TINE_WAIT_UPLINK);
    

    // Delay 10 seconds
    Serial.println("Delay 10 seconds for next scan...");
    delay(10*1000);
}

/* IPS packet parser */
static void indoorPos_parser(UnaScanResult *sr) {
    IndoorPosition_t *ips;
    int i;

    // Save to IPS structure
    for (i=0; i<6; i++) {
        // Check IPS adv containing coordinate information
        ips = &my_ips[i];
        if (!(ips->ips_Flag & 0x01))
            break;
    }

    if (i >= 6) {
        Serial.println("There are too many IPS beacons... ignore this advertisement.");
        return;
    }

    // BD addr
#if PRINT_PARSER_LOG
    Serial.println("Parser: Get BD address");
#endif
    memcpy(&ips->ips_Address[0], &sr->HexAddress[0], 6);


    // RSSI
#if PRINT_PARSER_LOG
    Serial.println("Parser: Get RSSI");
#endif
    ips->ips_Rssi = sr->RSSI;

    // X & Y axis
    byte *ptr = sr->HexData;
    int   adv_len = sr->HexDataLength;
    unsigned int value;

    while (adv_len > 0) {
        byte arg_len, arg_type, flag;
        
        // Read one byte
        arg_len  = *(ptr++);
        arg_type = *(ptr++);

#if PRINT_PARSER_LOG
        Serial.print("Parser: arg len = ");
        Serial.println((int)arg_len, DEC);
        Serial.print("Parser: arg type = ");
        Serial.println((int)arg_type, DEC);
#endif

        // Check argument length
        if (!arg_len)
            break;
        
        // Check argument type
        switch (arg_type) {
            // Indoor positioning
            case 0x25: {
                // Get flag
                flag = *(ptr);
                ips->ips_Flag = flag;

#if PRINT_PARSER_LOG
                Serial.println("Parser: Get flag in advertising data.");
#endif
                // Coordinary
                if (flag & (1<<0)) {
#if PRINT_PARSER_LOG
                    Serial.println("Parser: flag bit 0");
#endif
                    // 4-bytes
                    for (int i=0; i<4; i++)
                    {
                        ips->ips_Xaxis[i] = *(ptr+1+i);
                    }
                    // 4-bytes
                    for (int i=0; i<4; i++)
                    {
                        ips->ips_Yaxis[i] = *(ptr+5+i);
                    }
                }

                // Power
                if (flag & (1<<2)) {
#if PRINT_PARSER_LOG
                    Serial.println("Parser: flag bit 2");
#endif
                    ips->ips_Tx = *(ptr+9);
                }

                // Floor number
                if (flag & (1<<4)) {
#if PRINT_PARSER_LOG
                    Serial.println("Parser: flag bit 4");
#endif
                    ips->ips_Floor = *(ptr+10) - 20;
                }

                // Altitude
                if (flag & (1<<3)) {
                    // ignore altitude
                }

                // Uncertainty
                if (flag & (1<<5)) {
                    // ignore uncertainty
                }

                // Local name in gatt
                if (flag & (1<<6)) {
                    // ignore bit[6]
                }
                
                // reserved bit [7]
                if (flag & (1<<7)) {
                    // ignore bit[7]
                }
                // -------------------
#if PRINT_PARSER_LOG
                char str[64];
                sprintf(str, "\t BD Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n",
                        ips->ips_Address[0],ips->ips_Address[1],
                        ips->ips_Address[2],ips->ips_Address[3],
                        ips->ips_Address[4],ips->ips_Address[5]);
                Serial.print(str);
                Serial.flush();
                
                sprintf(str, "\t RSSI       = %d\r\n", (int) ips->ips_Rssi);
                Serial.print(str);
                Serial.flush();
                
                if (ips->ips_Flag & 0x02) {
                    // local coordinary
                    sprintf(str, "\t X-axis Pos = %u\r\n", (unsigned int*)ips->ips_Xaxis[0]);
                    Serial.print(str);
                    Serial.flush();
                    sprintf(str, "\t Y-axis Pos = %u\r\n", (unsigned int*)ips->ips_Yaxis[0]);
                    Serial.print(str);
                    Serial.flush();
                } else {
                    // WGS84 coordinary
                    sprintf(str, "\t Latitute   = %u\r\n", (unsigned int)ips->ips_Xaxis[0]);
                    Serial.print(str);
                    Serial.flush();
                    sprintf(str, "\t Longitute  = %u\r\n", (unsigned int)ips->ips_Yaxis[0]);
                    Serial.print(str);
                    Serial.flush();
                }
                
                sprintf(str, "\t Floor      = %d\r\n", ips->ips_Floor);
                Serial.print(str);
                Serial.flush();

                sprintf(str, "\t TX Power   = %d\r\n", (signed char)ips->ips_Tx);
                Serial.print(str);
                Serial.flush();
                // -------------------
#endif
                break;
            }

            case 0x02:
            case 0x03: {
                // get UUID
                byte uuid[2];
                byte *puuid = ptr;

                while (puuid < ptr + arg_len - 1) {
                    uuid[0] = puuid[0];
                    uuid[1] = puuid[1];
#if PRINT_PARSER_LOG
                    if (uuid[0] == 0x21 && uuid[1] == 0x18) {
                        Serial.println(" UUID: Indoor Positioning");
                    } else {
                        char str[32];
                        sprintf(str, "[Unknown 16-bit UUID] 0x%02X%02X \r\n", uuid[1], uuid[0]);
                        Serial.print((const char*)str);
                    }
#endif
                    puuid += 2;
                }
                break;
            }
           
            default: {
#if PRINT_PARSER_LOG
                Serial.println("Parser: unknown ADV type");
#endif
                break;
            }
        }

        // Next argument
        ptr += arg_len - 1;
        adv_len -= arg_len + 1;
    }

    return;
}

/* Expected Output: (example)

Ping UnaMKR
AT?
OK
Start BLE Scan, time = 3 second(s). Please wait a while...
AT$BTSCAN=3000,0,2
OK
Get Scan Result
AT$BTRESULT?
0,0,400E21BF211B,-55,29,0D25370100000002000000C41E01030321180201060708556E614D4B52;
0,0,F8A280303055,-60,29,0D25370200000003000000C41E01030321180201060708556E614D4B52
[Beacon 1]
  RSSI = -55
  X-asis = 1
  Y-axis = 2

[Beacon 2]
  RSSI = -60
  X-asis = 2
  Y-axis = 3

Uplink IPS detection
AT$SF=02C90102C40203,0
OK
Delay 10 seconds for next scan...


*/
