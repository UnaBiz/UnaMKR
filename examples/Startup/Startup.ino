/*
 *  Sample code for getting start UnaMKR
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
#define  SPECIFIC_ZONE          (RCZ_1)         // configure specific RC zone to UnaMKR
                                                // RC Zone: https://build.sigfox.com/sigfox-radio-configurations-rc

/* Components */
UnaMKR  my_mkr;

/* Definitions */
#define  TIME_WAIT_RESPONSE     (1000)         // timeout 1 seconds

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
  
    char id[12], pac[20], addr[13], fw_ver[12], zone[4];
    int  resp_len;

    // Get SigFox device ID
    my_mkr.getId();
    my_mkr.getData(id, &resp_len, TIME_WAIT_RESPONSE);
    
    // Get SigFox PAC 
    my_mkr.getPac();
    my_mkr.getData(pac, &resp_len, TIME_WAIT_RESPONSE);
    
    // Get BD address
    my_mkr.getBtDeviceAddress();
    my_mkr.getData(addr, &resp_len, TIME_WAIT_RESPONSE);

    // Get firmware version
    my_mkr.getVersion();
    my_mkr.getData(fw_ver, &resp_len, TIME_WAIT_RESPONSE);

    // Get SigFox current RC-Zone
    my_mkr.getZone();
    my_mkr.getData(zone, &resp_len, TIME_WAIT_RESPONSE);
    
    // Configure RC Zone to UnaMKR
    my_mkr.setZone(SPECIFIC_ZONE);
    my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

    // Print informations
    Serial.print("\r\n");
    Serial.print(" Device ID  : ");
    Serial.println((const char*) id);
    Serial.print(" PAC        : ");
    Serial.println((const char*) pac);
    Serial.print(" BD Address : ");
    Serial.println((const char*) addr);
    Serial.print(" F/W version: ");
    Serial.println((const char*) fw_ver);
    Serial.print(" RC Zone    : ");
    Serial.println((const char*) zone);
    Serial.print(" Set Zone   : ");
    Serial.println(SPECIFIC_ZONE, DEC);

    // flush serial port
    Serial.flush();

    // delay 10 seconds
    delay(10000);
}


/* Expected Output: (example)

AT$ID?
0FFFFFFF
AT$PAC?
FFFFFFFFFFFFFFFF
AT$BTADDR?
F1E2D3C4B5A6
AT$FW?
F/W:V0003
AT$ZONE?
4
AT$SETZONE=1
OK

 Device ID  : 0FFFFFFF
 PAC        : FFFFFFFFFFFFFFFF
 BD Address : F1E2D3C4B5A6
 F/W version: F/W:V0003
 RC Zone    : 4
 Set Zone   : 1

*/
