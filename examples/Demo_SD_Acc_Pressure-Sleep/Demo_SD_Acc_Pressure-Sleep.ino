/*
 *  Demo code:
 *      Read sensors of accelerometer and pressure
 *      Write log
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
#include <SD.h>

/* Options */
#define  PUBLIC_KEY_USAGE       (false)         // set 'true' to enable public key (Emulator)
#define  LOG_MESSAGE_DISPLAY    (true)          // set 'true' to enable displaying all message on serial port

/* Components */
UnaMKR  my_mkr;
Adafruit_BME680 my_bme;
LSM303AGR_ACC_Sensor *Acc;

/* Definitions */
#define  SNR_SAMPLING_TIME     (500)          // 500 milli-seconds

/* Variables */
volatile bool button_pressed = false;
static uint64_t loop_count = 0;
bool deep_sleep = false;
char file_name[64];
int  file_idx;

/* Interrupt callback */
void buttonInterrupt() {
    button_pressed = true;
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
    Acc->SetFS(2.0);    // 2g / 4g / 8g / 16g
    Acc->SetODR(50.0);  // 50 Hz
    delay(500);
    Acc->Enable();
    
    // Serial port for Arduino UART over USB
    Serial.begin(115200);

    // UnaMKR
    my_mkr.begin(PUBLIC_KEY_USAGE);
    my_mkr.wakeup();

    // Button
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    attachInterrupt(PIN_BUTTON, buttonInterrupt, FALLING);
    

    // SD init
    SD.begin(SS1);
    file_idx = 0;

    // Delay 5 seconds before loop entry
    delay(5000);

    // Variable
    deep_sleep = false;
}

/* Arduino Main Loop */
void loop() {
    
    bool bme680_state = true;
    int16_t  acc[3];
    float  pressure;
    char  fname, buff[128];

    unsigned long s_time = millis();
    
    if (!deep_sleep) {
        deep_sleep = true;
        my_mkr.sleep();
        my_mkr.getData_CheckOk(1500);
    }
    
    // Update sensors
    if (!my_bme.performReading()) {
        /* Reading informations of sensor BME680 */
        bme680_state = false;
        pressure = -1.0;
    } else {
        pressure = my_bme.readPressure()/100.0;
    }

    Acc->GetAxesRaw(acc);

    sprintf(buff, 
            "0x%04X,0x%04X,0x%04X,%4.2f\r\n",
            (uint16_t)acc[0],
            (uint16_t)acc[1],
            (uint16_t)acc[2],
            pressure);

    File dataFile = SD.open("acclog.txt", FILE_WRITE);
    if (dataFile) {
        if (button_pressed) {
            button_pressed = false;
            dataFile.print("==========\r\n");
        }
        
        dataFile.print(buff);
        dataFile.close();
    } else {
        Serial.println("[error] opening acclog.txt");
    }


    // Debug on serial port
    if (Serial) {
        Serial.print("Acceleration (X,Y,Z) = ");
        sprintf(buff, 
                "(0x%04X, 0x%04X, 0x%04X), ",
                (uint16_t)acc[0],
                (uint16_t)acc[1],
                (uint16_t)acc[2]);
        Serial.print(buff);
        Serial.print("Pressure = ");
        if (pressure < 0)
            sprintf(buff, "#NAN");
        else
            sprintf(buff, "%4.2f", pressure);
        Serial.print(buff);
        Serial.println(" hPa");

        // Flush serial port
        Serial.flush();
    }

    // Increase counter
    loop_count++;
    
    // Delay 500 ms
    while (millis() - s_time < SNR_SAMPLING_TIME) delay(10);
    
}
