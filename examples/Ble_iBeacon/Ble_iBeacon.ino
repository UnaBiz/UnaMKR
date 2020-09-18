/*
 *  Sample code for BLE advertisement (iBeacon)
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
#define  TIME_WAIT_RESPONSE     (2500)        // timeout 2500 ms
#define  IBEACON_MAJOR          (0x0001)      // Majot dats
#define  IBEACON_MINOR          (0x0002)      // Minor data
#define  IBEACON_TX_POWER       (-60)         // -60 dBm at 1 meter
#define  IBEACON_UUID           0x00,0x00,0x00,0x00,0x00,0x00,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00

/* Variables */
bool  in_advertising = false;
const byte ibeacon_data[]  =  {
    /* iBeacon Advertising */
    0x4C, 0x00,                               // Apple, Inc.
    0x02,                                     // iBeacon 
    21,                                       // Length of following data
    IBEACON_UUID,                             // 16 bytes UUID
    (IBEACON_MAJOR >> 8) & 0xFF,
    (IBEACON_MAJOR     ) & 0xFF,              // 2 bytes major
    (IBEACON_MINOR >> 8) & 0xFF,
    (IBEACON_MINOR     ) & 0xFF,              // 2 bytes minor
    (byte) IBEACON_TX_POWER,                  // 1 byte TX power
    //0x00,                                     // dummy
};
const byte general_data[] = {0x06};

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
    my_beacon.add(my_beacon.transfer_raw(ADV_TYPE_MANUFACTURER, &ibeacon_data[0], sizeof(ibeacon_data)));
    my_beacon.add(my_beacon.transfer_raw(ADV_TYPE_BT_FLAG, &general_data[0], sizeof(general_data)));
     
    // flag
    in_advertising = false;

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {

    // Check advertising flag
    if (!in_advertising) {
        // Start BLE advertisement
        Serial.println("Start BLE Advertisement (iBeacon)");
        my_mkr.btAdvertising(&my_beacon);
    
        // Wait response
        my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

        // Done
        Serial.println("Done");
        in_advertising = true;
    }

    // Delay
    delay(100);

    // Flush serial port
    Serial.flush(); 
}

/* Expected Output:
Start BLE Advertisement (iBeacon)
AT$BTADV=2,1
OK
AT$BTSETADV=50,255,4C0002150000000000009988776655443322110000010002C4
OK
AT$BTSETADV=2,1,06
OK
Done
*/