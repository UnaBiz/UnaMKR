# UnaMKR Hardware

This document contains information about UnaMKR hardware.


# Schematic

This part of the documentation is a representation of the UnaMKR onboard components.


![alt_text](img/unamkr-hw-schematics.png)



# Antenna Connectors

This is how to connect antennas to UnaMKR.


## Sigfox Antenna

* Firstly connect the IPEX to SMA cable to the UnaMKR
* Then screw on the antenna to the SMA connector


![alt_text](img/unamkr-antenna-sigfox.png ":size=500")



## Bluetooth Antenna



*   Connect the IPEX of the bluetooth antenna to the UnaMKR


![alt_text](img/unamkr-antenna-ble.png ":size=500")



# Jumpers Definition


| Jumper J11 | Select Power Source |
:---:|:---:
| From Arduino | From USB |
| ![](img/unamkr-jumpers-j11-left.png ":size=150") | ![](img/unamkr-jumpers-j11-right.png ":size=150")

| Jumper J7 | Select UART RX Line |
:---:|:---:
| From Arduino | From USB |
| ![](img/unamkr-jumpers-j7-left.png ":size=150") | ![](img/unamkr-jumpers-j7-right.png ":size=150")

| Jumper J6 | Select UART TX Line |
:---:|:---:
| From Arduino | From USB |
| ![](img/unamkr-jumpers-j6-left.png ":size=150") | ![](img/unamkr-jumpers-j6-right.png ":size=150")

| Jumper J10 | Flash mode |
:---:|:---:
| Enable | Disable |
| ![](img/unamkr-jumpers-j10-off.png ":size=50") | ![](img/unamkr-jumpers-j10-on.png ":size=50")


| Jumper J14 | ST JTAG |
:---:|:---:
| Pin 1 | 3V3 |
| Pin 2 | SWCLK |
| Pin 3 | GND |
| Pin 4 | SWDIO |
| Pin 5 | NRST |
| Pin 6 | NC |

![](img/unamkr-jumpers-j4.png)





# Sensors I2C Addresses

| Sensor | I2C Address |
---|---:
| Accelerometer Sensor(LSM303AGR)	| 0x19
| Magnetometer Sensor(LSM303AGR)	| 0x1E
| Gas Sensor(BME280) |	0x77
| Temperature Sensor(BME280) |	0x77
| Humidity Sensor(BME280) |	0x77
| Pressure Sensor(BME280) |	0x77
| Light Sensor(TSL2540)	| 0x39
