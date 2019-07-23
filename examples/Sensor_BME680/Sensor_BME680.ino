/*
 *  Sample code for sensor BME680
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
Adafruit_BME680 bme;

/* Definitions */
#define  SEALEVELPRESSURE_HPA   (1013.25)       // constant for BME680

/* Variables */

/* Arduino Setup */
void setup() {

    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // BME608 Sensor
    bme.begin();
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

    // Delay 5 seconds before loop entry
    delay(5000);
}

void loop() {

    // Check BME680 device and read all sensors
    if (! bme.performReading()) {
        Serial.println("Failed to perform reading :(");
        return;
    }

    // Print values
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.pressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme.humidity);
    Serial.println(" %");

    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.println();

    // Delay 1 second
    delay(1000);

    // Flush serial port
    Serial.flush();
}

/* Expected Output: (example)

Temperature = 24.92 *C
Pressure = 1006.75 hPa
Humidity = 50.64 %
Gas = 75.92 KOhms
Approx. Altitude = 54.26 m

*/
