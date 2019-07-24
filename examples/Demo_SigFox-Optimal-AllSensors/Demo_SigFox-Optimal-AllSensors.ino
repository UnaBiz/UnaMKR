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
#define  TIMER_PERIODIC         (1200)          // timer trigger uplink, unit: second
#define  TIMER_SAMPLING         (30)            // timer trigger asynchronous sampling, unit: second
#define  TIME_WAIT_RESPONSE     (1000)          // timeout for response, unit: milli-second
#define  TIME_WAIT_UPLINK       (15000)         // timeout for uplink, unit: milli-second

/* Variables */
volatile bool button_pressed = false;
volatile bool reed_detect    = false;
volatile bool timer_detect   = false;
volatile bool timer_sampling = false;
volatile uint32_t timer_count;

/* Interrupt callback */
void reedInterrupt() {
    reed_detect = true;
}

void buttonInterrupt() {
    button_pressed = true;
}

void timerInterrupt() {
    ++timer_count;
    if ((timer_count % (1000*TIMER_SAMPLING)) == 0) {
        timer_sampling = true;
    }
    if (timer_count >= (1000*TIMER_PERIODIC)) {
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
void TC4_Handler() {
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
    
    // LSM303AGR Sensor
    Acc = new LSM303AGR_ACC_Sensor(&Wire);
    Mag = new LSM303AGR_MAG_Sensor(&Wire);
    Acc->SetFS(2.0);    // 2g
    Acc->SetODR(25.0);  // 25Hz
    Mag->SetFS(2.0);    // 2g
    Mag->SetODR(10.0);  // 10Hz
    delay(500);
    Acc->Enable();
    Mag->Enable();

    // TSL2540 Sensor
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
    zt4->setCallback(true, TC_CALLBACK_CC_CHANNEL0, timerInterrupt);
    zt4->enable(true);

    // Delay 5 seconds before loop entry
    delay(5000);
}

/* Arduino Main Loop */
void loop() {

    static int loop_count = 0;
    static int read_count = 0;
    char response[12];
    char str[128];
    int  resp_len;
    float tmp, hmd, prs, gas;
    uint8_t zone;
    int32_t acc[3], mag[3];
    uint16_t vsb, ir;
    bool  rs;

    // Asynchronous sampling for BME680
    if (timer_sampling) {
      my_bme.performReading();
      timer_sampling = false;
    }
    
    // Check interrupt status
    if (!button_pressed && !reed_detect && !timer_detect) {
        return;
    }
    
    Serial.print("Interrupt detected: ");
    if (button_pressed)
      Serial.println("Button ");
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
    UnaMkrMessage msg(true);
    msg.clear();
    msg.setTimeout(TIME_WAIT_UPLINK);
    msg.addField_RCZone(zone);
    msg.addField_ReedSwitch(rs);
    msg.addField_Acceletometer(acc);
    msg.addField_Magnetometer(mag);
    msg.addField_Temperature(tmp);
    msg.addField_Humidity(hmd);
    msg.addField_Pressure(prs);
    msg.addField_IndoorAirQuality(gas);
    msg.addField_LightSensor(vsb);
    msg.addField_InfraredSensor(ir);    

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
    Serial.println(vsb, DEC);
    
    Serial.print("  * Infrared: ");
    Serial.println(ir, DEC);
    
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

Interrupt detected: Button 
AT$ZONE?
1
  * RC = 1
  * Temperature = 25.40 *C
  * Humidity = 47.72 %
  * Pressure = 1003.17 hPa
  * Air Quality = 61.88 kOhms
  * Acc (X,Y,Z)[mg] = (   35,   98,  839)
  * Mag (X,Y,Z)[mGauss] = ( -327,  505, -568)
  * Visible: 704
  * Infrared: 210
  * Reed Switch: 0

[Payload 0] 1202306234727B12A39CBC3C
[Payload 1] 2FE0032FC8182C02C000D236

SigFox uplink... 
AT$SF=1202306234727B12A39CBC3C,0
OK
AT$SF=2FE0032FC8182C02C000D236,0
OK
AT$SLEEP
OK

Done

*/

/* Expected Output: (triggered by reed switch)

Interrupt detected: Reed Switch 
AT$ZONE?
1
  * RC = 1
  * Temperature = 25.47 *C
  * Humidity = 47.70 %
  * Pressure = 1003.19 hPa
  * Air Quality = 61.20 kOhms
  * Acc (X,Y,Z)[mg] = (  -15,   12, 1014)
  * Mag (X,Y,Z)[mGauss] = ( 2524, 2326, -496)
  * Visible: 748
  * Infrared: 218
  * Reed Switch: 1

[Payload 0] 13FF100C3F627CD2A19CBCD4
[Payload 1] 20FC0E8FCF17E802EC00DAB7

SigFox uplink... 
AT$SF=13FF100C3F627CD2A19CBCD4,0
OK
AT$SF=20FC0E8FCF17E802EC00DAB7,0
OK
AT$SLEEP
OK

Done

*/

/* Expected Output: (triggered by timer)

Interrupt detected: Timer 
AT$ZONE?
1
  * RC = 1
  * Temperature = 25.31 *C
  * Humidity = 48.09 %
  * Pressure = 1003.02 hPa
  * Air Quality = 66.57 kOhms
  * Acc (X,Y,Z)[mg] = (   23,   20, 1014)
  * Mag (X,Y,Z)[mGauss] = ( -343,  442, -613)
  * Visible: 733
  * Infrared: 182
  * Reed Switch: 0

[Payload 0] 120170143F6278D2C99CB831
[Payload 1] 2FDE02CFC31A0102DD00B68B

SigFox uplink... 
AT$SF=120170143F6278D2C99CB831,0
OK
AT$SF=2FDE02CFC31A0102DD00B68B,0
OK
AT$SLEEP
OK

Done

*/
