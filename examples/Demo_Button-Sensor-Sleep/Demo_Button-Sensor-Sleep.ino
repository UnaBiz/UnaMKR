/*
 *  Sample code for DIO & reading BME680 sensor
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

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
UnaMKR  my_mkr;
Adafruit_BME680 my_bme;

/* Definitions */
#define  TIME_WAIT_RESPONSE     (1000)          // timeout 1 seconds
#define  SEALEVELPRESSURE_HPA   (1013.25)       // constant for BME680

/* Variables */

/* Arduino Setup */
void setup() {

    // Button on MKR
    pinMode(PIN_BUTTON, INPUT);

    // BME608 Sensor
    my_bme.begin();
    my_bme.setTemperatureOversampling(BME680_OS_8X);
    my_bme.setHumidityOversampling(BME680_OS_2X);
    my_bme.setPressureOversampling(BME680_OS_4X);
    my_bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    my_bme.setGasHeater(320, 150); // 320*C for 150 ms

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
  
    // Wait button status (pressed)
    if (digitalRead(PIN_BUTTON) == HIGH)
        return;

    Serial.println("DIO button is pressed.");

    // Wake up
    my_mkr.wakeup();
    
    // Reading informations of sensor BME680
    if (!my_bme.performReading()) {
        Serial.println("Failed to perform reading :(");
    } else {
        Serial.print("Temperature = ");
        Serial.print(my_bme.temperature);
        Serial.println(" *C");
      
        Serial.print("Pressure = ");
        Serial.print(my_bme.pressure / 100.0);
        Serial.println(" hPa");
      
        Serial.print("Humidity = ");
        Serial.print(my_bme.humidity);
        Serial.println(" %");
      
        Serial.print("Gas = ");
        Serial.print(my_bme.gas_resistance / 1000.0);
        Serial.println(" KOhms");
     
        Serial.print("Approx. Altitude = ");
        Serial.print(my_bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial.println(" m");
    }
    
    // Sleep mode
    Serial.print("Sleep mode\r\n");
    my_mkr.sleep();
    my_mkr.getData_CheckOk(TIME_WAIT_RESPONSE);

    // Delay 500 ms
    Serial.println("Delay 500 milli-seconds...");
    delay(500);

    // Flush serial port
    Serial.flush();
}


/* Expected Output:

DIO button is pressed
Temperature = 25.42 *C
Pressure = 1004.43 hPa
Humidity = 54.32 %
Gas = 0.00 KOhms
Approx. Altitude = 73.86 m
Sleep mode
AT$SLEEP
OK
Delay 500 milli-seconds...

*/
