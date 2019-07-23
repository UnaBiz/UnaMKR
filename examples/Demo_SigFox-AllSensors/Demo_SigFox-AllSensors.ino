/*
 *  Demo code:
 *      Read all sensors on UnaMKR
 *      SigFox uplink
 *      Trigger methods: Button, Reed switch, Timer
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */
#include <UnaMKR.h>
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>
#include <LSM303AGR_ACC_Sensor.h>
#include <LSM303AGR_MAG_Sensor.h>
#include <TSL2540.h>
#include <Adafruit_ZeroTimer.h>

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
UnaMKR  my_mkr;
Adafruit_BME680 my_bme;
LSM303AGR_ACC_Sensor *Acc;
LSM303AGR_MAG_Sensor *Mag;
TSL2540  tsl;
Adafruit_ZeroTimer *zt4;

/* Definitions */
#define  TIMER_PERIODIC         (600)           // timer trigger per 10 minutes (600 seconds)
#define  TIME_WAIT_RESPONSE     (1000)          // timeout for response, unit: milli-second
#define  TIME_WAIT_UPLINK       (15000)         // // timeout for uplink, unit: milli-second 

/* Variables */
volatile bool button_pressed = false;
volatile bool reed_detect    = false;
volatile bool timer_detect   = false;
volatile uint32_t timer_count;

/* Interrupt callback */
void reedInterrupt() {
  reed_detect = true;
}

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
  reed_detect    = false;
  timer_detect   = false;
}

/* Timer handler, please do not remove it */
void TC4_Handler()
{
  Adafruit_ZeroTimer::timerHandler(4);
}
  
/* Arduino Setup */
void setup() {

    // BME608 Sensor
    my_bme.begin();
    my_bme.setTemperatureOversampling(BME680_OS_8X);
    my_bme.setHumidityOversampling(BME680_OS_2X);
    my_bme.setPressureOversampling(BME680_OS_4X);
    my_bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    my_bme.setGasHeater(320, 150); // 320*C for 150 ms
    my_bme.performReading();
    
    // LSM303AGR
    Acc = new LSM303AGR_ACC_Sensor(&Wire);
    Mag = new LSM303AGR_MAG_Sensor(&Wire);
    Acc->SetFS(2.0);    // 2g
    Acc->SetODR(25.0);  // 25Hz
    Mag->SetFS(2.0);    // 2g
    Mag->SetODR(10.0);  // 10Hz
    delay(500);
    Acc->Enable();
    Mag->Enable();

    // TSL2540
    tsl.begin();

    // Reed switch
    pinMode(PIN_REED_INT, INPUT_PULLUP);
    attachInterrupt(PIN_REED_INT, reedInterrupt, FALLING);
    
    // Button
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    attachInterrupt(PIN_BUTTON, buttonInterrupt, FALLING);
    
    // Variable
    clearInterrupt();
    timer_count = 0;
    
    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // Initialize component
    my_mkr.begin(PUBLIC_KEY_USAGE);

    // Print response data
    my_mkr.logEnable(LOG_MESSAGE_DISPLAY);

    // Timer
    zt4 = new Adafruit_ZeroTimer(4);
    zt4->configure(TC_CLOCK_PRESCALER_DIV1,   // prescaler
                TC_COUNTER_SIZE_16BIT,        // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM  // match style
                );
    zt4->setPeriodMatch(48000, 24000, 0);     // 1 match, channel 0, unit: ms
    zt4->setCallback(true, TC_CALLBACK_CC_CHANNEL0, timerInterrupt);  // set DAC in the callback
    zt4->enable(true);

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {

    static int loop_count = 0;
    bool bme680_state = true;
    char response[12];
    char str[128];
    int  resp_len;
    float tmp, hmd, prs, gas;
    uint8_t zone;
    int32_t acc[3], mag[3];
    uint16_t vsb, ir;
    bool  rs;

    // Check interrupt status
    if (!button_pressed && !reed_detect && !timer_detect) {
        delay(50);
        return;
    }
    
    Serial.print("Interrupt detected: ");
    if (button_pressed)
      Serial.println("Botton ");
    if (reed_detect)
      Serial.println("Reed Switch ");
    if (timer_detect)
      Serial.println("Timer ");
    
    // Wake-up UnaMKR
    my_mkr.wakeup();

    // Get RC zone
    my_mkr.getZone();
    my_mkr.getData(response, &resp_len, TIME_WAIT_RESPONSE);
    zone = atoi(response);
    if (zone < RCZ_1 || zone > RCZ_6) {
        Serial.println("Invalid RC");
        zone = 0xFF;
    }
    
    // Update sensors
    if (! my_bme.performReading()) {
        /* Reading informations of sensor BME680 */
        Serial.println("Failed to perform reading :(");
        bme680_state = false;
    }
    
    tmp = my_bme.temperature;
    hmd = my_bme.humidity;
    prs = my_bme.pressure;
    gas = my_bme.gas_resistance;
    vsb = tsl.readVisible();
    ir  = tsl.readIR();
    rs  = (digitalRead(PIN_REED_INT) == HIGH) ? false : true;

    rs |= reed_detect;
    Acc->GetAxes(acc);
    Mag->GetAxes(mag);

    // Setup message (payload)
    UnaMkrMessage msg;
    msg.clear();
    msg.setTimeout(TIME_WAIT_UPLINK);
    
    msg.addField_Acceletometer(acc);
    msg.addField_Magnetometer(mag);
 
    if (bme680_state) {
      msg.addField_Temperature(tmp);
      msg.addField_Humidity(hmd);
      msg.addField_Pressure(prs);
      msg.addField_IndoorAirQuality(gas);
    }
    
    msg.addField_LightSensor(vsb);
    msg.addField_InfraredSensor(ir);
    msg.addField_ReedSwitch(rs);
    msg.addField_RCZone(zone);

    // Print all status of sensors
    Serial.print("  * RC = ");
    Serial.println(zone, DEC);
        
    Serial.print("  * Temperature = ");
    Serial.print(tmp);
    Serial.println(" *C");

    Serial.print("  * Humidity = ");
    Serial.print(hmd);
    Serial.println(" %");

    Serial.print("  * Pressure = ");
    Serial.print(prs/100);
    Serial.println(" hPa");

    Serial.print("  * Air Quality = ");
    Serial.print(gas/1000);
    Serial.println(" kOhms");
    
    sprintf(str, "  * Acc (X,Y,Z)[mg] = (%5d,%5d,%5d)", acc[0],acc[1],acc[2]);
    Serial.println((const char*)str);
    
    sprintf(str, "  * Mag (X,Y,Z)[mGauss] = (%5d,%5d,%5d)",mag[0],mag[1],mag[2]);
    Serial.println((const char*)str);

    Serial.print("  * Visible: ");
    Serial.println((int)vsb);
    
    Serial.print("  * IR: ");
    Serial.println((int)ir);
    
    Serial.print("  * Reed Switch: ");
    Serial.println(rs, BIN);

    // Print payloads
    Serial.print("\r\n");
    msg.print_msg();
    
    // Uplink with message type of payload
    Serial.println("SigFox uplink... ");
    my_mkr.uplink(&msg);

    // Wait response
    my_mkr.getData_CheckOk(TIME_WAIT_UPLINK);

    // Increase counter
    loop_count++;
    
    // Sleep mode
    my_mkr.sleep();
    my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

    // Done
    Serial.println("\r\nDone");
    clearInterrupt();
    
    // Flush serial port
    Serial.flush();
}


/* Expected Output: (triggered by button)

Interrupt detected: Botton 
AT$ZONE?
2
  * RC = 2
  * Temperature = 34.49 *C
  * Humidity = 50.93 %
  * Pressure = 1002.11 hPa
  * Air Quality = 4.95 kOhms
  * Acc (X,Y,Z)[mg] = (  -30, -947,  211)
  * Mag (X,Y,Z)[mGauss] = (  -37,  457,   60)
  * Visible: 48
  * IR: 62
  * Reed Switch: 0

[Payload 0] 058FE2C4D0D3130D797000B1
[Payload 1] 068FFD02D0062313E5A020C0
[Payload 2] 0332725430031830030000B1
[Payload 3] 083003E000000000000000CF

SigFox uplink... 
AT$SF=058FE2C4D0D3130D797000B1,0
OK
AT$SF=068FFD02D0062313E5A020C0,0
OK
AT$SF=0332725430031830030000B1,0
OK
AT$SF=083003E000000000000000CF,0
OK
AT$SLEEP
OK

Done

*/

/* Expected Output: (triggered by reed switch)

Interrupt detected: Reed Switch 
AT$ZONE?
2
  * RC = 2
  * Temperature = 34.51 *C
  * Humidity = 50.83 %
  * Pressure = 1002.11 hPa
  * Air Quality = 6.40 kOhms
  * Acc (X,Y,Z)[mg] = (  -19, -970,  195)
  * Mag (X,Y,Z)[mGauss] = (  354,  114,  340)
  * Visible: 49
  * IR: 65
  * Reed Switch: 1

[Payload 0] 058FEDC360C3130D7A7010CA
[Payload 1] 06802300B0222313DAA02016
[Payload 2] 03327254300408300310000B
[Payload 3] 08300410000000000000009D

SigFox uplink... 
AT$SF=058FEDC360C3130D7A7010CA,0
OK
AT$SF=06802300B0222313DAA02016,0
OK
AT$SF=03327254300408300310000B,0
OK
AT$SF=08300410000000000000009D,0
OK
AT$SLEEP
OK

Done

*/

/* Expected Output: (triggered by timer)

Interrupt detected: Timer 
AT$ZONE?
2
  * RC = 2
  * Temperature = 34.43 *C
  * Humidity = 51.43 %
  * Pressure = 1002.11 hPa
  * Air Quality = 6.07 kOhms
  * Acc (X,Y,Z)[mg] = (  -19, -958,  203)
  * Mag (X,Y,Z)[mGauss] = (  -15,  456,   81)
  * Visible: 65
  * IR: 85
  * Reed Switch: 0

[Payload 0] 058FEDC420CB130D737000F3
[Payload 1] 068FFF02D008231416A02038
[Payload 2] 033272543003C8300410009D
[Payload 3] 083005500000000000000038

SigFox uplink... 
AT$SF=058FEDC420CB130D737000F3,0
OK
AT$SF=068FFF02D008231416A02038,0
OK
AT$SF=033272543003C8300410009D,0
OK
AT$SF=083005500000000000000038,0
OK
AT$SLEEP
OK

Done

*/
