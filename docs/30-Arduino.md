# UnaMKR Arduino Mode

This document describes how to use UnaMKR in Arduino mode.

The following tutorials will guide you through how to setup the software development environment and everything you need to start with your IoT projects.


# Prerequisites

* UnaMKR set to Arduino mode (check [UnaMKR Operating Modes](20-Modes?id=switching-mode))
* Arduino Board [MKRZero](https://store.arduino.cc/usa/arduino-mkrzero)
* [Arduino IDE](https://www.arduino.cc/en/Main/Software)
* Micro-USB cable

# Setup Arduino environment

## Arduino IDE

* Download and install the latest [Arduino IDE](https://www.arduino.cc/en/Main/Software)

* Attach physically UnaMKR on top of MZRZero board
![](img/unamkr-mkrzero.png ":size=500")


## Dependent Libraries

* Download and install the libraries required.
* Click `Sketch` → `Include Library` → `Manage Libraries...`
* Install the following libraries
  * Adafruit_Sensor ([https://github.com/adafruit/Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor))
  * Adafruit_BME680 ([https://github.com/adafruit/Adafruit_BME680](https://github.com/adafruit/Adafruit_BME680))
  * Adafruit_ZeroTimer ([https://github.com/adafruit/Adafruit_ZeroTimer](https://github.com/adafruit/Adafruit_ZeroTimer))
  * STM32duino_LSM303AGR ([https://github.com/stm32duino/LSM303AGR](https://github.com/stm32duino/LSM303AGR))

## UnaMKR Library

  * Browse to [https://github.com/UnaBiz/unamkr](https://github.com/UnaBiz/unamkr) and click on `Clone or download` then click on “Download ZIP” button to download the example code.

  * Launch the Arduino integrated development environment.
  * Click on menu `Sketch` → `Include Library` → `Add .ZIP Library...`
  * Select the downloaded zip file.
  * After the installation you should see `UnaMKR` in the menu `File` → `Examples`.

# Run Example code

This part of the documentation describes how to send sensor data to UnaConnect and UnaMe app data visualization via Sigfox network.



*   Browse to [https://www.arduino.cc/en/Guide/ArduinoMKRZero](https://www.arduino.cc/en/Guide/ArduinoMKRZero) and install board support package for MKRZero.
*   Select the proper COM port the Arduino MKR board is on.



<p id="gdcalert4" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-30-AT3.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert5">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-30-AT3.png "image_tooltip")




*   Loading the example code by clicking “File” → “Examples” → “UnaMKR” → “Demo_Sigfox-AllSensors”
*   Click the Upload button on Arduino IDE and wait a few seconds, the message "Done uploading." will appear in the status bar.
*   The information read from all built-in sensors will be sent to Sigfox network every 10 minute and can be triggered manually by the following way as well :
    *   Press the button on the UmaMKR
    *   Putting a magnet up to the reed switch
*    Log onto Sigfox backend to verify the messages.



<p id="gdcalert5" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-30-AT4.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert6">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-30-AT4.png "image_tooltip")




*   To use of the UnaConnect and UnaMe app data visualization, please refer to UnaConnect page:

<p id="gdcalert6" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: undefined internal link (link text: "UnaConnect"). Did you generate a TOC? </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert7">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>

[UnaConnect](#heading=h.9p42w91hb6zc)


# UnaMKR Arduino Pins Definition

|Arduino Pins|UnaMKR|Description|
|:---:|:---:|--- |
|AREF|NC|Voltage Reference System|
|A0|NC|Analog Pin-0|
|A1|NC|Analog Pin-1|
|A2|NC|Analog Pin-2|
|A3|NC|Analog Pin-3|
|A4|NC|Analog Pin-4|
|A5|NC|Analog Pin-5|
|A6|NC|Analog Pin-6|
|0|SDN|S2LP SDN|
|1|Push Button|Push button；Push to low signal level|
|~2|Light Sensor Interrupt|Light Sensor Interrupt|
|~3|Magnetometer Sensor Interrupt|Magnetometer Sensor Interrupt|
|~4|Accelerometer Sensor Interrupt|Accelerometer Sensor Interrupt|
|~5|NC|Digital Pin-5|
|6|Reed Sensor|Reed Sensor|
|7|NC|Digital Pin-7|
|8 MOSI|NC|Digital Pin-8|
|9 SCK|NC|Digital Pin-9|
|10 MISO|NC|Digital Pin-10|
|11 SDA|SDA|I2C-SDA|
|12 SCL|SCL|I2C-SCL|
|13 RX|RX|Arduino MKRZERO UART receiving pin; UnaMKR transmitting pin|
|14 TX|TX|Arduino MKRZERO UART transmitting pin; UnaMKR receiving pin|
|RESET|RESET|Reset Arduino MCU & Sigfox Module；Low active|
|GND|GND|Grounding & power/signal reference level|
|VCC|NC|3V3 Power Source from Arduino platform|
|VIN|NC|Extra power source input of Arduino platform|
|5V|5V|5V Power Source from Arduino platform|

# UnaMKR Arduino Library Reference

This part of the documentation describes each method included in the UnaMKR library

```
UnaMKR initialization
    bool  begin(void);
bool  begin(bool public_key_usage);
        This method is utilized to initialize this component in Arduino. If the 'public_key_usage' is specific, it will enable/disable SigFox public key.
        SigFox SDR requires to enable Public Key mode.
        (pUnaMKR)->begin(true);
        <Example>
            // initial UnaMKR in setup sketch
(pUnaMKR)->begin();

    void  logEnable(bool enable);
        This method is utilized to enable/disable monitor of Arduino to displaying all commands and response on serial port. The default is disable.
        <Example>
            // Enable to display all communication data
(pUnaMKR)->logEnable(true);

UnaMKR read response
    bool  checkData_Available(void);
        This method is utilized to check response data is available from UnaMKR.
        <Example>
            // Wait until the response is available
while (!(pUnaMKR)->checkData_Available()) delay(10);
    bool  getData_try(char *response, int *resp_len);
        This method is utilized to read response data from UnaMKR without blocking.
        <Example>
            // read the response without blocking
Char response[16];
int  length;
(pUnaMKR)->getData_try(response, &length);

    bool  getData(char *response, int *resp_len, unsigned int timeout);
        This method is utilized to get response data from UnaMKR with specific timeout. The unit of timeout is one milli-second. When timeout is zero, it is equivalent to getData_try().
        <Example>
            char response[16];
int  length;
// Get data with timeout 1000 ms
(pUnaMKR)->echo();
(pUnaMKR)->getData(response, &length, 1000);

    bool  getData_CheckOk(unsigned int timeout);
        This method is utilized to get response data from UnaMKR with specific timeout, and it returns true if the response data is "OK\r". The unit of timeout is one milli-second.
        <Example>
            // Check echo is OK with timeout 1000 ms
(pUnaMKR)->echo();
(pUnaMKR)->getData_CheckOK(1000);

    bool  getData_ScanResult(UnaScanResult *result, unsigned int timeout);
        This method is utilized to get one BLE scanning result from UnaMKR with specific timeout, and it returns true if the scanned result is not empty. The unit of timeout is one milli-second.
        <Example>
            // Start BLE scanning 3 seconds without any filters
(pUnaMKR)->btscan(3000, 0, 0);
(pUnaMKR)->getData_CheckOk(4000);
// Read all scanned results
UnaScanResult my_result;
(pUnaMKR)->btGetScanResult();
while ((pUnaMKR)->getData_ScanResult(&my_result, 1000))
{
  ...
}

UnaMKR status/information request
    int   echo(void);
        This method is utilized to check and ping between UnaMKR and Arduino.
        <Example>
            // Ping UnaMKR
(pUnaMKR)->echo();
(pUnaMKR)->getData_CheckOK(1000);

    int   reset(void);
        This method is utilized to soft-reset UnaMKR.
        <Example>
            // Soft-reset UnaMKR
(pUnaMKR)->reset();
(pUnaMKR)->getData_CheckOK(1000);

    int   sleep(void);
int   sleep(int sleep_time);
        This method is utilized to change status of UnaMKR to deep-sleep. If sleep time is specific, the unit of sleep time is one milli-second;
        <Example>
            // Change Status to deep-sleep
(pUnaMKR)->sleep();
(pUnaMKR)->getData_CheckOK(1000);

    int   wakeup(void);
        This method is utilized to change status of UnaMKR from deep-sleep to ready.
        <Example>
            // Wake up UnaMKR
(pUnaMKR)->wakeup();

    int   getVersion(void);
        This method is utilized to get firmware version of UnaMKR.
        <Example>
            char response[16];
int  length;
// Get firmware version of UnaMKR
(pUnaMKR)->getVersion();
(pUnaMKR)->getData(response, &length, 1000);

UnaMKR SigFox functions request
    int   getId(void);
        This method is utilized to get SigFox device ID of UnaMKR.
        <Example>
            char response[32];
int  length;
// Get SigFox device ID
(pUnaMKR)->getId();
(pUnaMKR)->getData(response, &length, 1000);

    int   getPac(void);
        This method is utilized to get SigFox device PAC of UnaMKR.
        <Example>
            char response[32];
int  length;
// Get SigFox device PAC
(pUnaMKR)->getPac();
(pUnaMKR)->getData(response, &length, 1000);

    int   getZone(void);
        This method is utilized to get current SigFox operation zone (RCZ) of UnaMKR.
        <Example>
            char response[32]
int  length;
// Get current SigFox RCZ
(pUnaMKR)->getZone();
(pUnaMKR)->getData(response, &length, 1000);

    int   setZone(int rcz);
        This method is utilized to configure specific SigFox operation zone (RCZ) of UnaMKR. This value will be saved in UnaMKR and applied automatically when the module powers on.
        <Example>
            // Set SigFox RCZ to ZONE-1
(pUnaMKR)->setZone(1);
(pUnaMKR)->getData_CheckOk(1000);

    int   monarch(int search_time);
        This method is utilized to change status of UnaMKR to monarch scanning with specific time. The unit of monarch time is one second. If the response is OK (success), the detected RCZ will be saved in UnaMKR and applied automatically when it powers on. If the response is FAIL, it will go back to previous RCZ.
        <Example>
            // Start monarch scanning (time = 310 seconds)
(pUnaMKR)->monarch(310);
(pUnaMKR)->getData_CheckOk(310*1000);

    int   publicKey(bool enable);
        This method is utilized to enable/disable public key of UnaMKR. The default value is disable.
        <Example>
            // Enable public key due to Emulator
(pUnaMKR)->publicKey(true);
(pUnaMKR)->getData_CheckOk(1000);

    int   uplink(const String data_str);
int   uplink(const char *data_str);
int   uplink(const byte *hex_data, int data_len);
int   uplink(UnaMkrMessage *pMsg);
int   uplink(UnaShieldMessage *pMsg);
        These methods are utilized to transmit SigFox uplink with specific data.
        <Example>
            // Uplink ASCII string "UnaMKR"
(pUnaMKR)->uplink(String("UnaMKR"));
(pUnaMKR)->getData_CheckOk(1000);
            // Uplink numeric ASCII HEX data (0-9, A-F, a-f)
(pUnaMKR)->uplink("0102030405060708090A0B0C");
(pUnaMKR)->getData_CheckOk(1000);
            // Uplink HEX data
byte hex_data[] = {0x01,0x02,0x03,0x04,0x05,0x06};
int  data_length = sizeof(hex_data);
(pUnaMKR)->uplink(hex_data, data_length);
(pUnaMKR)->getData_CheckOk(1000);
            // Uplink a message type for UnaMKR
// The number uplink payload is determined by data field
UnaMkrMessage msg;
msg.clear();
msg.addField_Temperature(28.62);
msg.addField_Humidity(37.10);
msg.addField_Pressure(101362.5);
msg.addField_IndoorAirQuality(10024);
msg.print_msg();
(pUnaMKR)->uplink(&msg);
(pUnaMKR)->getData_CheckOk(1000);
            // Uplink a message type (equal to UnaShield)
UnaShieldMessage msg;
msg.clear();
msg.addField("tmp", 30.00);
msg.addField("hmd", 45.10);
(pUnaMKR)->uplink(&msg);
(pUnaMKR)->getData_CheckOk(1000);

    int   downlink(const String data_str);
int   downlink(const char *data_str);
int   downlink(const byte *hex_data, int data_len);
int   downlink(UnaShieldMessage *pMsg);
        These methods are utilized to transmit SigFox downlink request with specific data.
            char response[32];
int  length;
            // Send downlink request with ASCII string "UnaMKR"
(pUnaMKR)->downlink(Sting("UnaMKR"));
(pUnaMKR)->getData(response, &length, 30000);
            // Send downlink request with numeric ASCII HEX data
// (1-9, A-F, a-f)
(pUnaMKR)->downlink("0102030405060708090A0B0C");
(pUnaMKR)->getData(response, &length, 30000);
            // Send downlink request with HEX data
byte hex_data[] = {0x01,0x02,0x03,0x04,0x05,0x06};
int  data_length = sizeof(hex_data);
(pUnaMKR)->downlink(hex_data, data_length);
(pUnaMKR)->getData(response, &length, 30000);
            // Send downlink request with a message type
// (equal to UnaShield)
UnaShieldMessage msg;
msg.clear();
msg.addField("tmp", 30.00);
msg.addField("hmd", 45.10);
(pUnaMKR)->downlink(&msg);
(pUnaMKR)->getData(response, &length, 30000);

UnaMKR BLE functions request
    int   setBtName(const char *pName);
        This method is utilized to set BLE device name to UnaMKR.
        <Example>
            // Set BLE device name
(pUnaMKR)->setBtName("MyMKR");
(pUnaMKR)->getData_CheckOk(1000);

    int   setBtDeviceAddress(const char *pBdAddr);
        This method is utilized to set public BLE device address (BD address) to UnaMKR.
        <Example>
            // Set BD address
(pUnaMKR)->setBtDeviceAddress("F1E2D3C4B5A6");
(pUnaMKR)->getData_CheckOk(1000);

    int   getBtDeviceAddress(void);
        This method is utilized to get public BLE device address (BD address).
        <Example>
            char response[32];
int  length;
// Get BD address
(pUnaMKR)->getBtDeviceAddress();
(pUnaMKR)->getData(response, &length, 1000);

    int   btReset(void);
        This method is utilized to reset BLE state and GAP role to ready state.
        <Example>
            // Reset BLE
(pUnaMKR)->btReset("MyMKR");
(pUnaMKR)->getData_CheckOk(1000);

    int   btScan(int scan_time, int RSSI_filter, int Adv_filter);
int   btScan(int scan_time,
	ScanRssiFilter RSSI_filter,
	ScanAdvFilter Adv_filter);
        This method is utilized to change BLE to scan state. There are some filters which UnaMKR supported.
        <RSSI filter>
            NO_RSSI_FILTER, no RSSI filter applied.
RSSI_LARGER_THAN_m70DB, ignore RSSI < -70 db.
RSSI_LARGER_THAN_m60DB, ignore RSSI < -60 db.
RSSI_LARGER_THAN_m50DB, ignore RSSI < -50 db.
RSSI_LARGER_THAN_m40DB, ignore RSSI < -40 db.
RSSI_LARGER_THAN_m30DB, ignore RSSI < -30 db.
        <Advertisement filter>
            NO_ADV_FILTER, no advertisement filter applied.
ADV_IBEACON, ignore advertisements except iBeacon .
ADV_IPS, ignore advertisements except indoor-positioning.
        <Example>
            // Start BLE scan. Time: 3000 ms. RSSI filter: RSSI > -50db
(pUnaMKR)->btScan(3000,
	RSSI_LARGER_THAN_m50DB,
	NO_ADV_FILTER);
(pUnaMKR)->getData_CheckOk(4000);

    int   btGetScanResult(void);
        This method is utilized to request all scan results.
        <Example>
            // Get all BLE scan results
UnaScanResult my_result;
(pUnaMKR)->btGetScanResult();
while ((pUnaMKR)->getData_ScanResult(&my_result, 1000))
{
  ...
}

    int   btAdvertising(UnaAdvertiser* adv);
        This method is utilized to change to GAP peripheral role and BLE advertising state.
        <UnaAdvertiser>
            void clear(void);
            Clear all specific advertisements and configuration.
            bool config(ScanAdvType connectable,
	ScanAddressType address_type);
            Configure advertising connectable/non-connectable, public address/random address.
            Adv_t  transfer_localName(const char* name);
            Transfer a short name to advertisement format.
            Adv_t  transfer_UUID16(const uint16_t *uuid16,
	int list_number);
            Transfer an array of UUID-16 list to advertisement format.
            Adv_t  transfer_raw(AdvType_t type,
	const byte *data,
	byte length);
            Transfer AdvType, raw data to advertisement format.
            bool add(Adv_t advertisement);
            Add specific Advertisement to this component.
            bool remove(AdvType_t remove_type);
            Remove specific AdvType in this component.
            Please refer example code Ble_Beacon.ino to learn more setup of Advertiser.
        <Example>
            // Start a connectable BLE advertising
UnaAdvertiser my_adv;
my_adv.clear();
my_adv.config(CONNECTABLE, RANDOM_ADDRESS);
(pUnaMKR)->btAdvertising(&my_adv);
(pUnaMKR)->getData_CheckOk(1000);

    int   btConnect(const char *pBdAddress);
        This method is utilized to create a BLE connection with scanned peripheral's BD address. The GAP role is going to CENTRAL (MASTER).
There is no GATT client for UnaMKR currently. This command is reserved for specific GATT client feature in the future.
        <Example>
            // Create connection
(pUnaMKR)->btConnect("MyMKR");
(pUnaMKR)->getData_CheckOk(1000);

    int   btDisconnect(void);
        This method is utilized to disconnect with current BLE connection. For peripheral role, UnaMKR is going to advertise after disconnected.
        <Example>
            // Disconnect BLE
(pUnaMKR)->btDisconnect();
(pUnaMKR)->getData_CheckOk(1000);

    int   btWrite(const char *data, int data_len);
        This method is utilized to send an ASCII text data to client which connected with GATT chat service.
        This command is only valid for peripheral role.
        <Example>
            // BLE TX
(pUnaMKR)->btWrite("Text from UnMKR");
(pUnaMKR)->getData_CheckOk(1000);

    int   btRead(int read_len);
        This method is utilized to receive an ASCII text data from client which connected with GATT chat service and buffering in UnaMKR.
        This command is only valid for peripheral role.
        <Example>
            // BLE RX (maximum 16 bytes)
char response[16];
int  length;
// Get data with timeout 1000 ms
(pUnaMKR)->btRead(16);
(pUnaMKR)->getData(response, &length, 1000);
```
