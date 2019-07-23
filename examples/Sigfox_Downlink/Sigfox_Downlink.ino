/*
 *  Sample code for sending SigFox downklink data frame and
 *   wait downlink response data.
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
#define  TIME_WAIT_DOWNLINK     (60000)         // downlink timeout 60 seconds

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
  
    char response[128];
    char my_data[28];
    int  resp_len;

    // Prepare uplink payload
    strcpy(my_data, "CCBBAA998877665544332211");
    
    // Send downlink request to UnaMKR
    Serial.print("Send SigFox frame ");
    Serial.println((const char*)my_data);
    my_mkr.downlink((const char*)my_data);

    // Wait response
    if (!my_mkr.getData(response, &resp_len, TIME_WAIT_DOWNLINK))
    {
        Serial.println("Read response data timeout.");
        delay(10000);
        return;
    }

    if (!resp_len)
    {
        Serial.print("Downlink failed\r\n");
    }
    else
    {
        Serial.print("Recieve downlink data: ");
        Serial.println((const char*) response);
        Serial.print("Length: ");
        Serial.println(resp_len);
    }
    
    // Delay
    Serial.println("Delay 10 seconds...");
    delay(10000);

    // Flush serial port
    Serial.flush();
}


/* Expected Output: (example)

Send SigFox frame CCBBAA998877665544332211
AT$SF=CCBBAA998877665544332211,1
8877665544332211
Recieve downlink data: 8877665544332211
Length: 16
Delay 10 seconds...

*/
