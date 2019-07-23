/*
 *  Sample code for bluetooth low energy advertisement
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

/* Components */
UnaMKR my_mkr;
UnaAdvertiser my_beacon;

/* Definitions */
#define  TIME_WAIT_RESPONSE     (2500)          // timeout 2500 ms
#define  MY_LOCAL_NAME          ("myMKR")       // device local name
#define  MY_BEACON_COORDINATE_X (5)             // local coordinate of X-axis
#define  MY_BEACON_COORDINATE_Y (3)             // local coordinate of Y-axis
#define  MY_BEACON_FLOOR        (1)             // first floor

/* Variables */
bool  in_advertising = false;
const char      beacon_sname[] = MY_LOCAL_NAME;   // short name
const uint16_t  beacon_uuid[]  = {0x1821};        // UUID-16 of indoor positioning
const byte      beacon_data[]  =                  // IPS data
{
    /* Indoor Positiooning Advertising */
    0x37,                                     // flag 
    ( MY_BEACON_COORDINATE_X       & 0xFF),   // coordinate-1 (low-byte)
    ((MY_BEACON_COORDINATE_X >> 8) & 0xFF),
    ((MY_BEACON_COORDINATE_X >>16) & 0xFF),
    ((MY_BEACON_COORDINATE_X >>24) & 0xFF),
    ( MY_BEACON_COORDINATE_Y       & 0xFF),   // coordinate-2 (low-byte)
    ((MY_BEACON_COORDINATE_Y >> 8) & 0xFF),
    ((MY_BEACON_COORDINATE_Y >>16) & 0xFF),
    ((MY_BEACON_COORDINATE_Y >>24) & 0xFF),
    0xE0,                                     // referece TX power
    MY_BEACON_FLOOR+20,                       // encoded floor number
    0x01,                                     // uncertainty
};

/* Arduino Setup */
void setup() {
  
    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // Initialize 
    my_mkr.begin(PUBLIC_KEY_USAGE);

    // Print response data
    my_mkr.logEnable(LOG_MESSAGE_DISPLAY);

    // Config beacon packet
    my_beacon.clear();
    my_beacon.config(NON_CONNECTABLE, RANDOM_ADDRESS);
    my_beacon.add(my_beacon.transfer_localName(beacon_sname));
    my_beacon.add(my_beacon.transfer_raw(ADV_TYPE_IPS, beacon_data, sizeof(beacon_data)));
    my_beacon.add(my_beacon.transfer_UUID16(&beacon_uuid[0], sizeof(beacon_uuid)/sizeof(uint16_t)));
    
    // flag
    in_advertising = false;

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {

    // Check advertising flag
    if (!in_advertising) {
        // Set local name
        Serial.println("Configure device name");
        my_mkr.setBtName(beacon_sname);
        
        // Wait response
        my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);
        
        // Start BLE advertisement
        Serial.println("Start BLE Beacon (Indoor Positioning)");
        my_mkr.btAdvertising(&my_beacon);
    
        // Wait response
        my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

        // Done
        Serial.println("Done");
        in_advertising = true;
    }

    // Delay
    delay(1000);

    // Flush serial port
    Serial.flush();
}

/* Expected Output:

Configure device name
AT$NAME=myMKR
OK
Start BLE Beacon (Indoor Positioning)
AT$BTADV=2,1
OK
AT$BTSETADV=10,8,6D794D4B52
OK
AT$BTSETADV=24,37,370500000003000000E01501
OK
AT$BTSETADV=4,3,2118
OK
Done

 */
