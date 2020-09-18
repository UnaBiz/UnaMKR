/*
 *  Demo code:
 *      1. Monarch scan (triggered by button or timer)
 *      2. Get RC zone
 *      3. SigFox uplink (uplink data is 0xFF if the detection is failed)
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */
#include <UnaMKR.h>
#include <Adafruit_ZeroTimer.h>

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
UnaMKR  my_mkr;
Adafruit_ZeroTimer *zt4;

/* Definitions */
#define  TIMER_PERIODIC         (600)           // timer trigger per 10 minutes (600 seconds)
#define  TIME_MONARCH_SCAN      (310)           // timeout for monarch detection, unit: second
#define  TIME_WAIT_RESPONSE     (1000)          // timeout for response, unit: milli-second
#define  TIME_WAIT_UPLINK       (15000)         // timeout for uplink, unit: milli-second

/* Variables */
static int zone;
volatile uint32_t timer_count;
volatile bool button_pressed = false;
volatile bool timer_detect   = false;

/* Interrupt callback */
void buttonInterrupt() {
  button_pressed = true;
}

void timerInterrupt() {
  if (++timer_count > 1000*TIMER_PERIODIC)
  {
      timer_detect = true;
      timer_count  = 0;
  }
}

void clearInterrupt() {
  button_pressed = false;
  timer_detect   = false;
}

/* Timer handler, please do not remove it */
void TC4_Handler() {
  Adafruit_ZeroTimer::timerHandler(4);
}

/* Arduino Setup */
void setup() {

    // Variable
    zone = 0;

    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // Button
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    attachInterrupt(PIN_BUTTON, buttonInterrupt, FALLING);

    // Timer
    zt4 = new Adafruit_ZeroTimer(4);
    zt4->configure(TC_CLOCK_PRESCALER_DIV1,   // prescaler
                TC_COUNTER_SIZE_16BIT,        // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM  // match style
                );
    zt4->setPeriodMatch(48000, 24000, 0);     // 1 match, channel 0, unit: ms
    zt4->setCallback(true, TC_CALLBACK_CC_CHANNEL0, timerInterrupt);  // set DAC in the callback
    zt4->enable(true);

    // Initialize component
    my_mkr.begin(PUBLIC_KEY_USAGE);

    // Print response data
    my_mkr.logEnable(LOG_MESSAGE_DISPLAY);

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {
    static int loop_count = 0;
    char response[128];
    byte payload;
    int  resp_len;
    UnaMonarch result;

    // Check interrupt status
    if (!button_pressed && !timer_detect) {
        delay(50);
        return;
    }
    Serial.print("Interrupt detected: ");
    if (button_pressed)
      Serial.println("Botton ");
    if (timer_detect)
      Serial.println("Timer ");

    // Wake-up UnaMKR
    my_mkr.wakeup();

    // Start SigFox monarch scan
    Serial.print("Start SigFox monarch, timeout = ");
    Serial.print(TIME_MONARCH_SCAN);
    Serial.println(" second(s)...");
    my_mkr.monarch(TIME_MONARCH_SCAN);
    
    // Wait response
    if (my_mkr.getData_CheckOk(TIME_MONARCH_SCAN*1000 + TIME_WAIT_RESPONSE)) {
        Serial.println("Success! Get result");
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
                Serial.println("Monarch detection is failed.");
            }
        }
      
        // Get current RC zone
        my_mkr.getZone();
        
        // Wait response
        my_mkr.getData(response, &resp_len, TIME_WAIT_RESPONSE);
        zone = atoi(response);
        Serial.print("Current RC-zone is ");
        Serial.println(zone, DEC);
    } else {
        Serial.println("Monarch detection is failed.");
        zone = -1;
    }

    // Uplink with detected RC zone
    payload = (byte) zone;
    my_mkr.uplink(&payload, 1);

    // Wait response
    my_mkr.getData_CheckOk(TIME_WAIT_UPLINK);

    // Increase counter
    loop_count++;
    clearInterrupt();

    // Sleep mode
    my_mkr.sleep();
    my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

    // Flush serial port
    Serial.flush();
}


/* Expected Output: (example)

Interrupt detected: Botton 
Start SigFox monarch, timeout = 310 second(s)...
AT$MONARCH=310
OK
Success! Get result
AT$MONARCH?
1,-50
Detect RCZ = 1, RSSI = -50 dBm.
AT$ZONE?
1
Current RC-zone is 1
AT$SF=01,0
OK
AT$SLEEP
OK

*/
