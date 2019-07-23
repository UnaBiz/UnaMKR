/*
 *  Sample code for light sensor (TSL25403)
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */
#include "TSL2540.h"

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
TSL2540  tsl;

/* Definitions */

/* Variables */

/* Arduino Setup */
void setup() {

    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // enable sensor
    tsl.begin();

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {

    // Read sensor
    Serial.print("Visible : ");
    Serial.println((int)tsl.readVisible(), DEC);
    Serial.print("Infrared: ");
    Serial.println((int)tsl.readIR(), DEC);

    // delay
    delay(1000);
    
    /* flush serial port */
    Serial.flush();
}

/* Expected Output: (example)

Visible : 526
Infrared: 86

*/
