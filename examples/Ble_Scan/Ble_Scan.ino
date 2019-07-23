/*
 *  Sample code for bluetooth low energy scan
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

/* Definitions */
#define  TIME_BLE_SCAN          (3000)          // scan time 3 seconds
#define  TIME_WAIT_RESPONSE     (2500)          // timeout 2500 ms

/* Variables */

/* Arduino Setup */
void setup() {
  
    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // Initialize 
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

    // Start BLE Scan
    Serial.print("Start BLE Scan, time = ");
    Serial.print(TIME_BLE_SCAN/1000);
    Serial.println(" second(s). Please wait a while...");
    
    // The method btScan provides two independents filters:
    // 
    // [ScanRssiFilter]
    //   'NO_RSSI_FILTER', no filter applied.
    //   'RSSI_LARGER_THAN_m70DB' allows advertisement with RSSI > -70db.
    //   'RSSI_LARGER_THAN_m60DB' allows advertisement with RSSI > -60db.
    //   'RSSI_LARGER_THAN_m50DB' allows advertisement with RSSI > -50db.    
    //   'RSSI_LARGER_THAN_m40DB' allows advertisement with RSSI > -40db.    
    //   'RSSI_LARGER_THAN_m30DB' allows advertisement with RSSI > -30db.
    // 
    // [ScanAdvFilter]
    //   'NO_ADV_FILTER' no filter applied.
    //   'ADV_IBEACON' allows iBeacon advertisement.
    //   'ADV_IPS' allows IPS (indoor positioning system) advertisement.
    //
    my_mkr.btScan(TIME_BLE_SCAN, NO_RSSI_FILTER, NO_ADV_FILTER);
    
    // Wait response
    my_mkr.getData_CheckOk(TIME_BLE_SCAN + TIME_WAIT_RESPONSE);

    // Get results
    Serial.println("Get Scan Result");
    my_mkr.btGetScanResult();

    // Wait response
    int device_number = 0;
    UnaScanResult my_result;
    while (my_mkr.getData_ScanResult(&my_result, TIME_WAIT_RESPONSE)) {
        // Increase device number
        device_number++;

        // Print device's informations
        Serial.print("[Device ");
        Serial.print(device_number);
        Serial.print("]\r\n");
        Serial.print("\tConnectable : ");
        Serial.println((my_result.AdvType == CONNECTABLE) ? "YES" : "NO");
        Serial.print("\tAddress Type: ");
        switch (my_result.AddressType) {
            case PUBLIC_ADDRESS:
                Serial.print("Public Address\r\n");
                break;
            case RANDOM_ADDRESS:
                Serial.print("Random Address\r\n");
                break;
            case PUBLIC_IDENTITY_ADDRESS:
                Serial.print("Public Identity Address\r\n");
                break;
            case RANDOM_IDENTITY_ADDRESS:
                Serial.print("Random Identity Address\r\n");
                break;
            default:
                Serial.print("Unknown Address Type\r\n");
                break;
        }
        Serial.print("\tAddress     : ");
        Serial.println((const char*) my_result.Address);
        Serial.print("\tRSSI (db)   : ");
        Serial.println((int) my_result.RSSI);
        Serial.print("\tAdv. Data   : ");
        Serial.println((const char*) my_result.Data);

        // Next result
        Serial.println("");
        Serial.flush();
    }

    // Delay 10000 ms
    Serial.println("Done, delay 10 seconds...");
    delay(10000);

    // Flush serial port
    Serial.flush();
}

/* Expected Output:

Start BLE Scan, time = 3 second(s). Please wait a while...
AT$BTSCAN=3000,0,0
OK
Get Scan Result
AT$BTRESULT?
1,0,20FABB09EB54,-88,30,0201021106F028E36862D634905143EFAAC64C2FBC08085048414E544F4D;
[Device 1]
  Connectable : YES
  Address Type: Public Address
  Address     : 20FABB09EB54
  RSSI (db)   : -88
  Adv. Data   : 0201021106F028E36862D634905143EFAAC64C2FBC08085048414E544F4D

1,1,4C8E4C99F9B9,-79,14,02011A0AFF4C0010050A1C76C6DE
[Device 2]
  Connectable : YES
  Address Type: Random Address
  Address     : 4C8E4C99F9B9
  RSSI (db)   : -79
  Adv. Data   : 02011A0AFF4C0010050A1C76C6DE

Done, delay 10 seconds...

 */
