# UnaMKR Modem Tutorial

This tutorial will guide you through getting your UnaMKR up and running in a minimum of time!


# Prerequisites

*   Data-enabled MicroUSB cable
*   Computer with at least 1 available USB port
*   One of the supported Operating Systems: Windows, Mac OS, Linux, Android... ([USB Serial Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers))
*   And … UnaMKR :-)


# Getting Started


## Read Sigfox ID + PAC

First of all, we need to get the information specific to your UnaMKR, especially its unique Sigfox ID and one-time Product Activation Code.



*   Scan the QR code on your UnaMKR
*   Write down the Sigfox ID and PAC
*   Note that the QR code also contains the Bluetooth MAC address (not used in this guide)


## Activate Sigfox service

Before your UnaMKR is able to use Sigfox service, it is mandatory to activate it:



1. Browse to [https://buy.sigfox.com/activate](https://buy.sigfox.com/activate)
2. Pick your country
3. Click on Next
4. Enter UnaMKR Sigfox ID and PAC and click on Next
5. Enter account details and click on Subscribe
6. Your subscription is active!


## Install Sigfox Antenna

UnaMKR requires an external antenna to be connected to properly use Sigfox services.



*   Connect the IPEX-end (small connector) to SMA cable to the UnaMKR
*   Screw on the antenna to the SMA connector
*   Verify that the IPEX-end is sturdely connected to the communication module.

![alt_text](img/unamkr-antenna-sigfox.png ':size=500')



## Enable External-Power (jumpers)

By default, UnaMKR circuit board is getting powered using the Arduino Pins.

To use external power source (such as Micro USB, like in this tutorial), make sure that Jumpers J11, J6, J7 are shorting the pins on their right side.

See "UnaMKR Hardware" for more details.

You can now safely connect your UnaMKR to your computer using a MicroUSB cable.

If UnaMKR is successfully powered up, a green LED will light up on the circuit board.


## Install USB Serial Driver

When used for the first time, UnaMKR behaves like a Sigfox modem, accessible via its MicroUSB port using Serial protocol.

In order to access UnaMKR, you need to install drivers to access the Serial port function. Those can be found here: [https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)


## Connect to UnaMKR

Now it's time to interact with UnaMKR using your computer!

The connection settings are the following:



*   Baud Rate: 115200
*   Data Bits: 8
*   Parity: None
*   Stop Bits: 1
*   Flow Control: None
*   Note: although not mandatory, we strongly recommend enabling "Local Echo"

Use these settings using a Serial terminal application of your own choice.

Below are some examples of applications:



*   Mac OS, `minicom` or `Serial`
*   Windows, `putty`
*   Linux, `minicom`

When successfully connected:



*   Send the test command `AT?`
*   If UnaMKR replies `OK`, Congratulations: you can proceed to the next step!
*   If not, go back and check you have properly followed every previous step.


## Detecting Sigfox RC

Because of specific countries regulations regarding radio frequencies usage, Sigfox uses different sets of Radio Configurations ([https://build.sigfox.com/sigfox-radio-configurations-rc](https://build.sigfox.com/sigfox-radio-configurations-rc)).

[Sigfox Monarch](https://build.sigfox.com/monarch) enables compatible-devices to automatically detect Radio Configurations that are usable at current location.

So, next, we will use UnaMKR Monarch function to autodetect Sigfox Radio Configuration.

*( of course, the Radio Configuration can be set manually but where is the fun in that? )*

!> Note: UnaMKR Radio Configuration default value is 4.



*   Tell UnaMKR to scan for Monarch frame for 5 minutes
    *   Send `AT$MONARCH=300`
*   As soon as a compatible Radio Configuration is detected, UnaMKR returns `OK` and stores the Radio Configuration Number in memory.
*   If no Radio Configuration have been received, UnaMKR returns `FAILED`.
*   The current Radio Configuration can be read by sending `AT$ZONE?`.

Note: UnaMKR keeps memory of the last set or last detected Radio Configuration, even after power reset.


## Sending first message

If UnaMKR successfully set its Sigfox Radio Configuration, you can now properly use Sigfox and start sending messages.



*   Log on to Sigfox Backend ([https://backend.sigfox.com](https://backend.sigfox.com/))
*   Click on top tab "DEVICES"
*   Input your UnaMKR Sigfox ID
*   Click on button "Filter"
*   Click on the Sigfox ID value
*   Click on the sidebar "MESSAGES"

Switch back to your Serial terminal application:



*   Send the following command `AT$SF=42,0`

Back to your web browser with Sigfox Backend and watch "42" appearing in the Messages list.

Congratulations, you just sent your first message with UnaMKR through Sigfox 0G Network.


# What's Next?

*   Take a look at all the other supported [AT commands](39-Modem).
*   Get some sensors data with [Arduino](30-Arduino)

<!-- Docs to Markdown version 1.0β17 -->
