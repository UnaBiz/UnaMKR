/*
 *  Sample code for Reed switch
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

/* Definitions */

/* Variables */
volatile bool reedInterruptFlag = false;

/* Interrupt callback */
void reedInterrupt() {
    reedInterruptFlag = true;
}

/* Arduino Setup */
void setup() {

    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // Congih interrupt of reed switch 
    pinMode(PIN_REED_INT, INPUT_PULLUP);
    attachInterrupt(PIN_REED_INT, reedInterrupt, FALLING);

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {
    
    // Check interrupt occurred
    if (reedInterruptFlag) {
        Serial.println("REED INTERRUPT");
        reedInterruptFlag = false;

        // Delay 500 milli-seconds after interrupt occurred
        Serial.println("Delay 500 milli-seconds...");
        delay(500);

        // Flush serial port
        Serial.flush();
    }
    
}

/* Expected Output:

REED INTERRUPT
Delay 500 milli-seconds...

*/
