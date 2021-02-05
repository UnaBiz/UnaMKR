/*
 *  Sample code for SigFox monarch scanning
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */

#include <UnaMKR.h>

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
UnaMKR my_mkr;

/* Definitions */
#define  TIME_MONARCH_SCAN      (310)           // timeout 310 seconds
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
    UnaMonarch result;

    // Start Monarch Scan.
    //  The SigFox base-station sends a monarch signal with periodic 5 minutes.
    //  Therefore we configure scanning time as 310 seconds.
    //
    Serial.print("Start SigFox monarch, timeout = ");
    Serial.print(TIME_MONARCH_SCAN);
    Serial.println(" second(s)...");
    my_mkr.monarch(TIME_MONARCH_SCAN);
    
    // Wait response, unit = ms
    my_mkr.getData_CheckOk(TIME_MONARCH_SCAN*1000+TIME_WAIT_RESPONSE);
    
    // Get monarch result
    Serial.println("Get monarch result");
    my_mkr.getMonarch();
    if (my_mkr.getData_Monarch(&result, TIME_WAIT_RESPONSE) == true)
    {
        if (result.checkAvailable() && result.checkSuccessful())
        {
            Serial.print("Detect RCZ = ");
            Serial.print(result.getRc(), DEC);
            Serial.print(", RSSI = ");
            Serial.print(result.getRssi(), DEC);
            Serial.println(" dBm.");
        }
        else
        {
            Serial.println("Monarch failed.");
        }
    }
    else 
    {
        Serial.println("Command unsupported, please make sure the firmware version is v0100 or later.");
    }
    
    // Get current RC zone
    Serial.println("Get RCZ");
    my_mkr.getZone();

    // Wait response
    my_mkr.getData(response, &resp_len, TIME_WAIT_RESPONSE);
    Serial.print("Current Zone is ");
    Serial.println(atoi(response), DEC);
    
    // Delay
    Serial.println("Delay 10 seconds...");
    delay(10000);

    // Flush serial port
    Serial.flush();
}

/* Expected Output: (example)

Start SigFox monarch, timeout = 310 second(s)...
AT$MONARCH=310
OK
AT$MONARCH?
1,-51
Get monarch result
Detect RCZ = 1, RSSI = -51 dBm.
Get RCZ
AT$ZONE?
1
Current Zone is 1
Delay 10 seconds...

 */
