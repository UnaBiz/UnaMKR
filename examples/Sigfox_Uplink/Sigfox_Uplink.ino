/*
 *  Sample code for sending SigFox upklink data frame
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
UnaMKR  my_mkr;

/* Definitions */
#define  TIME_WAIT_UPLINK     (15000)         // timeout 15 seconds

/* Variables */

/* Arduino Setup */
void setup() {

    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // Initialize component
    my_mkr.begin(PUBLIC_KEY_USAGE);

    // Print response data
    my_mkr.logEnable(LOG_MESSAGE_DISPLAY);

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {
  
    char my_data[28];
    int  resp_len;

    // Prepare uplink payload
    strcpy(my_data, "112233445566778899AABBCC");
    
    // Send uplink request to UnaMKR
    Serial.print("Send SigFox frame: ");
    Serial.println((const char*)my_data);
    my_mkr.uplink((const char*)my_data);

    // Wait response
    my_mkr.getData_CheckOk(TIME_WAIT_UPLINK);
    
    // Delay
    Serial.println("Delay 10 seconds...");
    delay(10000);

    // Flush serial port
    Serial.flush();
}


/* Expected Output:

Send SigFox frame: 112233445566778899AABBCC
AT$SF=112233445566778899AABBCC,0
OK
Delay 10 seconds...

*/
