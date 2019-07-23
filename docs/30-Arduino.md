# UnaMKR Arduino Mode

This document describes UnaMKR Arduino mode specific technical information.

# Pins Definition

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

# Arduino Library Reference

This part of the documentation describes each method included in the UnaMKR library

## Class `UnaMKR`

Instantiate the class in the usual Arduino way (declaration in global scope, initialization in setup() and referencing in loop() or interrupts).

```
UnaMKR  my_mkr;
```

## Initialization

### begin()

Initializes UnaMKR instance.

`bool begin(bool use_public_key)`

* `use_public_key`: tells UnaMKR should use Sigfox public key. Default: `false`.

```
// initialize UnaMKR to use in production with actual Sigfox network
my_mkr.begin();

// initialize UnaMKR to use in test mode with Sigfox SDR
my_mkr.begin(true);
```

### logEnable()

Defines if all commands and replies should be logged to Arduino Serial port.

`void logEnable(bool enable)`

* `enable`: To enable or disable logging though serial port. Default: `false`

```
// Enable logging
my_mkr.logEnable(true);

// Disable logging
my_mkr.logEnable();
my_mkr.logEnable(false);
```

## Data reading

### checkData_Available()

This method is utilized to check response data is available from UnaMKR.

`bool  checkData_Available(void)`

```
// Wait until the response is available
while (!my_mkr.checkData_Available()) delay(10);
```

### getData_try()

This method is utilized to read response data from UnaMKR without blocking.

`bool  getData_try(char *response, int *resp_len)`

* `response`:
* `resp_len`:

```
// read the response without blocking
char response[16];
int length;
my_mkr.getData_try(response, &length);
```

### getData()

This method is utilized to get response data from UnaMKR with specific timeout. The unit of timeout is one milli-second. When timeout is zero, it is equivalent to getData_try().

`bool  getData(char *response, int *resp_len, unsigned int timeout)`
* `response`:
* `resp_len`:
* `timeout`:

```
char response[16];
int length;
// Get data with timeout 1000 ms
my_mkr.echo();
my_mkr.getData(response, &length, 1000);
```

### getData_CheckOk()

This method is utilized to get response data from UnaMKR with specific timeout, and it returns true if the response data is "OK\r\n". The unit of timeout is one milli-second.

`bool  getData_CheckOk(unsigned int timeout)`
* `timeout`:

```
// Check echo is OK with timeout 1000 ms
my_mkr.echo();
my_mkr.getData_CheckOK(1000);
```

### getData_ScanResult()

This method is utilized to get one BLE scanning result from UnaMKR with specific timeout, and it returns true if the scanned result is not empty. The unit of timeout is one milli-second.

`bool  getData_ScanResult(UnaScanResult *result, unsigned int timeout)`
* `result`:
* `timeout`:

```
// Start BLE scanning 3 seconds without any filters
my_mkr.btscan(3000, 0, 0);
my_mkr.getData_CheckOk(4000);
// Read all scanned results
UnaScanResult my_result;
my_mkr.btGetScanResult();
while (my_mkr.getData_ScanResult(&my_result, 1000))
{
...
}
```

## Internals

### echo()

This method is utilized to check and ping between UnaMKR and Arduino.

`int echo(void);`

```
// Ping UnaMKR
my_mkr.echo();
my_mkr.getData_CheckOK(1000);
```

### reset()

This method is utilized to soft-reset UnaMKR.

`int reset(void);`

```
// Soft-reset UnaMKR
my_mkr.reset();
my_mkr.getData_CheckOK(1000);
```

### sleep()

This method is utilized to change status of UnaMKR to deep-sleep.

`int sleep(void);` `int sleep(int sleep_time);`
* `sleep_time`: If specified, milliseconds to sleep

```
// Change Status to deep-sleep
my_mkr.sleep();
my_mkr.getData_CheckOk(1000);
```

### wakeup()

This method is utilized to change status of UnaMKR from deep-sleep to ready.

`int wakeup(void)`

```
// Wake up UnaMKR
my_mkr.wakeup();
```

### getVersion()

This method is utilized to get firmware version of UnaMKR.

`int getVersion(void)`

```
char response[16];
int length;
// Get firmware version of UnaMKR
my_mkr.getVersion();
my_mkr.getData(response, &length, 1000);
```

## Sigfox functions

### getId()

This method is utilized to get SigFox device ID of UnaMKR.

`int getId(void)`

```
char response[32];
int length;
// Get SigFox device ID
my_mkr.getId();
my_mkr.getData(response, &length, 1000);
```

### getPac()

This method is utilized to get SigFox device PAC of UnaMKR.

`int getPac(void)`

```
char response[32];
int length;
// Get SigFox device PAC
my_mkr.getPac();
my_mkr.getData(response, &length, 1000);
```

### getZone()

This method is utilized to get current SigFox operation zone (RCZ) of UnaMKR.

`int getZone(void);`

```
char response[32];
int length;
// Get current SigFox RCZ
my_mkr.getZone();
my_mkr.getData(response, &length, 1000);
```

### setZone()

This method is utilized to configure specific SigFox operation zone (RCZ) of UnaMKR. This value will be saved in UnaMKR and applied automatically when the module powers on.

`int setZone(int rcz);`
* `rcz`: Sigfox Radio Configuration to set


```
// Set SigFox RCZ to Zone1
my_mkr.setZone(1);
my_mkr.getData_CheckOk(1000);
```

### monarch()

This method is utilized to change status of UnaMKR to monarch scanning with specific time. The unit of monarch time is one second. If the response is OK (success), the detected RCZ will be saved in UnaMKR and applied automatically when it powers on. If the response is FAIL, it will go back to previous RCZ.

`int monarch(int search_time);`


```
// Start monarch scanning (time = 310 seconds)
my_mkr.monarch(310);
my_mkr.getData_CheckOk(310*1000);
```

### publicKey()

This method is utilized to enable/disable public key of UnaMKR. The default value is disable.

`int publicKey(bool enable);`

```         
// Enable public key due to Emulator
my_mkr.publicKey(true);
my_mkr.getData_CheckOk(1000);
```

### uplink()

These methods are utilized to send Sigfox uplink message.

`int uplink(const String data_str);`
`int uplink(const char *data_str);`
`int uplink(const byte *hex_data, int data_len);`
`int uplink(UnaMkrMessage *pMsg);`
`int uplink(UnaShieldMessage *pMsg);`

```
// Uplink ASCII string "UnaMKR"
my_mkr.uplink(String("UnaMKR"));
my_mkr.getData_CheckOk(1000);

// Uplink numeric ASCII HEX data (0-9, A-F, a-f)
my_mkr.uplink("0102030405060708090A0B0C");
my_mkr.getData_CheckOk(1000);

// Uplink HEX data
byte hex_data[] = {0x01,0x02,0x03,0x04,0x05,0x06};
int  data_length = sizeof(hex_data);
my_mkr.uplink(hex_data, data_length);
my_mkr.getData_CheckOk(1000);

// Uplink a message type for UnaMKR
// The number uplink payload is determined by data field
UnaMkrMessage msg;
msg.clear();
msg.addField_Temperature(28.62);
msg.addField_Humidity(37.10);
msg.addField_Pressure(101362.5);
msg.addField_IndoorAirQuality(10024);
msg.print_msg();
my_mkr.uplink(&msg);
my_mkr.getData_CheckOk(1000);

// Uplink a message type (equal to UnaShield)
UnaShieldMessage msg;
msg.clear();
msg.addField("tmp", 30.00);
msg.addField("hmd", 45.10);
my_mkr.uplink(&msg);
my_mkr.getData_CheckOk(1000);
```

### downlink()

These methods are utilized to transmit SigFox downlink request with specific data.

`int   downlink(const String data_str);`
`int   downlink(const char *data_str);`
`int   downlink(const byte *hex_data, int data_len);`
`int   downlink(UnaShieldMessage *pMsg);`

```
char response[32];
int length;

// Send downlink request with ASCII string "UnaMKR"
my_mkr.downlink(Sting("UnaMKR"));
my_mkr.getData(response, &length, 30000);

// Send downlink request with numeric ASCII HEX data
// (1-9, A-F, a-f)
my_mkr.downlink("0102030405060708090A0B0C");
my_mkr.getData(response, &length, 30000);

// Send downlink request with HEX data
byte hex_data[] = {0x01,0x02,0x03,0x04,0x05,0x06};
int data_length = sizeof(hex_data);
my_mkr.downlink(hex_data, data_length);
my_mkr.getData(response, &length, 30000);

// Send downlink request with a message type
// (equal to UnaShield)
UnaShieldMessage msg;
msg.clear();
msg.addField("tmp", 30.00);
msg.addField("hmd", 45.10);
my_mkr.downlink(&msg);
my_mkr.getData(response, &length, 30000);
```

## Bluetooth functions

### setBtName()

This method is utilized to set BLE device name to UnaMKR.

`int setBtName(const char *pName);`

```
// Set BLE device name
my_mkr.setBtName("MyMKR");
my_mkr.getData_CheckOk(1000);
```

### setBtDeviceAddress()

This method is utilized to set public BLE device address (BD address) to UnaMKR.

`int setBtDeviceAddress(const char *pBdAddr);`

```
// Set BD address
my_mkr.setBtDeviceAddress("F1E2D3C4B5A6");
my_mkr.getData_CheckOk(1000);
```

### getBtDeviceAddress()

This method is utilized to get public BLE device address (BD address).

`int getBtDeviceAddress(void);`

```            
char response[32];
int length;
// Get BD address
my_mkr.getBtDeviceAddress();
my_mkr.getData(response, &length, 1000);
```

### btReset()

This method is utilized to reset BLE state and GAP role to ready state.

`int btReset(void);`

```            
// Reset BLE
my_mkr.btReset();
my_mkr.getData_CheckOk(1000);
```

### btScan()

This method is utilized to change BLE to scan state. There are some filters which UnaMKR supported.

`int btScan(int scan_time, int RSSI_filter, int Adv_filter);`
* `scan_time`:
* `RSSI_filter`:
* `Adv_filter`:

`int btScan(int scan_time, ScanRssiFilter RSSI_filter, ScanAdvFilter Adv_filter);`
* `scan_time`:
* `RSSI_filter`:
  * NO_RSSI_FILTER, no RSSI filter applied.
  * RSSI_LARGER_THAN_m70DB, ignore RSSI < -70 db.
  * RSSI_LARGER_THAN_m60DB, ignore RSSI < -60 db.
  * RSSI_LARGER_THAN_m50DB, ignore RSSI < -50 db.
  * RSSI_LARGER_THAN_m40DB, ignore RSSI < -40 db.
  * RSSI_LARGER_THAN_m30DB, ignore RSSI < -30 db.
* `Adv_filter`: Advertisement filter
  * NO_ADV_FILTER, no advertisement filter applied.
  * ADV_IBEACON, ignore advertisements except iBeacon .
  * ADV_IPS, ignore advertisements except indoor-positioning.

```            
// Start BLE scan. Time: 3000 ms. RSSI filter: RSSI > -50db
my_mkr.btScan(3000, RSSI_LARGER_THAN_m50DB, NO_ADV_FILTER);
my_mkr.getData_CheckOk(4000);
```

### btGetScanResult()

This method is utilized to request all scan results.

`int btGetScanResult(void);`

```            
// Get all BLE scan results
UnaScanResult my_result;
my_mkr.btGetScanResult();
while (my_mkr.getData_ScanResult(&my_result, 1000))
{
  ...
}
```

### btAdvertising()

This method is utilized to change to GAP peripheral role and BLE advertising state.

`int btAdvertising(UnaAdvertiser* adv);`

```        
<UnaAdvertiser>
            // Clear all specific advertisements and configuration.
            void clear(void);

            // Configure advertising connectable/non-connectable, public address/random address.
            bool config(ScanAdvType connectable, ScanAddressType address_type);

            // Transfer a short name to advertisement format.
            Adv_t  transfer_localName(const char* name);

            // Transfer an array of UUID-16 list to advertisement format.
            Adv_t  transfer_UUID16(const uint16_t *uuid16, int list_number);

            // Transfer AdvType, raw data to advertisement format.
            Adv_t  transfer_raw(AdvType_t type, const byte *data,	byte length);

            // Add specific Advertisement to this component.
            bool add(Adv_t advertisement);

            //Remove specific AdvType in this component.
            bool remove(AdvType_t remove_type);

            //Please refer example code Ble_Beacon.ino to learn more setup of Advertiser.
```

```            
// Start a connectable BLE advertising
UnaAdvertiser my_adv;
my_adv.clear();
my_adv.config(CONNECTABLE, RANDOM_ADDRESS);
my_mkr.btAdvertising(&my_adv);
my_mkr.getData_CheckOk(1000);
```

### btConnect()

This method is utilized to create a BLE connection with scanned peripheral's BD address. The GAP role is going to CENTRAL (MASTER).
There is no GATT client for UnaMKR currently. This command is reserved for specific GATT client feature in the future.

`int btConnect(const char *pBdAddress);`

```            
// Create connection with device FB:A2:D6:00:00:01
my_mkr.btConnect("FBA2D6000001");
my_mkr.getData_CheckOk(1000);
```

### btDisconnect()

This method is utilized to disconnect with current BLE connection. For peripheral role, UnaMKR is going to advertise after disconnected.

`int btDisconnect(void);`

```            
// Disconnect BLE
my_mkr.btDisconnect();
my_mkr.getData_CheckOk(1000);
```

### btWrite()

This method is utilized to send an ASCII text data to client which connected with GATT chat service.
This command is only valid for peripheral role.

`int btWrite(const char *data, int data_len);`

```            
// BLE TX
my_mkr.btWrite("Text from UnaMKR");
my_mkr.getData_CheckOk(1000);
```

### btRead()

This method is utilized to receive an ASCII text data from client which connected with GATT chat service and buffering in UnaMKR.
This command is only valid for peripheral role.

`int btRead(int read_len);`
* `read_len`:

```            
// BLE RX (maximum 16 bytes)
char response[16];
int length;
// Get data with timeout 1000 ms
my_mkr.btRead(16);
my_mkr.getData(response, &length, 1000);
```
