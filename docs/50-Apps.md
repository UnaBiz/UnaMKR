<!----- Conversion time: 6.184 seconds.


Using this Markdown file:

1. Cut and paste this output into your source file.
2. See the notes and action items below regarding this conversion run.
3. Check the rendered output (headings, lists, code blocks, tables) for proper
   formatting and use a linkchecker before you publish this page.

Conversion notes:

* Docs to Markdown version 1.0β17
* Wed Jul 10 2019 02:57:11 GMT-0700 (PDT)
* Source doc: https://docs.google.com/a/unabiz.com/open?id=1BDA0GnCIlFSzy9bXXrTGKM5148DQUawMGK6yoerZDGA
* This document has images: check for >>>>>  gd2md-html alert:  inline image link in generated source and store images to your server.

WARNING:
You have 2 H1 headings. You may want to use the "H1 -> H2" option to demote all headings by one level.

----->


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 1; ALERTS: 8.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p><a href="#gdcalert1">alert1</a>
<a href="#gdcalert2">alert2</a>
<a href="#gdcalert3">alert3</a>
<a href="#gdcalert4">alert4</a>
<a href="#gdcalert5">alert5</a>
<a href="#gdcalert6">alert6</a>
<a href="#gdcalert7">alert7</a>
<a href="#gdcalert8">alert8</a>

<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>



# UnaMKR Documentation


## Your access to the 0G World


# Introduction

UnaMKR is the First Sigfox Monarch Development Kit. It combines connectivity, industrial-grade sensors, hardware and platform integration capabilities in a single easy-to-program development kit.


# UnaConnect


## Introduction

UnaConnect is a web platform to connect IoT devices to the Internet. It allows to decode the payload of the device sending through Sigfox to visualize the data or to send the decoded data to another service.



There are several steps needed to use UnaConnect properly. The first one is to connect the sigfox backend to UnaConnect. Then, in order to use the data on another service or to visualize them, the device needs to be registered on UnaMe.



Warning: To use UnaConnect you will have to use the example code (Demo_Sigfox-AllSensors sketch).


## Setup on Sigfox Backend-

To be available to fully use UnaConnect two callbacks are needed. To add a callback, a new button is available on the Callback’s main page on your device type’s sigfox backend.



When clicking on you, you will go to a screen where you have to select a platform for your callback.



On top of the options, a custom callback is considered as default and allows to connect custom services to the Sigfox Backend in order to send and receive data.



<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps0.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps0.png "image_tooltip")




Then, you will get a new screen to configure the callback:



<p id="gdcalert2" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps1.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert3">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps1.png "image_tooltip")




On the top you can select the type of the callback. The first one to set is DATA type with a BIDIR option. DATA means that the device will send data to the service; the BIDIR option allows the service to send an answer to the device when needed.



The CHANNEL must be a simple URL which allows to send json data format.



The URL pattern is:[ https://api.unabiz.io/v1/unasensors/events-sigfox](https://api.unabiz.io/v1/unasensors/events-sigfox). It is the url of UnaConnect to receive sigfox’s data.



Then, the HTTP Method to use is a post in application/json. With this option, sigfox will send data through a json format which is often used in API.



Finally, the first callback ends with the body:



{

  "device": "{device}",

  "data": "{data}",

  "time": "{time}",

  "duplicate": "{duplicate}",

  "snr": "{snr}",

  "station": "{station}",

  "avgSnr": "{avgSnr}",

  "lat": "{lat}",

  "lng": "{lng}",

  "rssi": "{rssi}",

  "seqNumber": "{seqNumber}",

  "ack": "{ack}",

  "longPolling": "{longPolling}",

  "device_type": "unamkr"

}



In this body, some informations of sigfox are sent. For example, the device means the device id; the data means the payload sent by the device; the time is the timestamp when the device sent the payload. And importantly, the device_type “unamkr” helps UnaConnect to know that you are using a UnaMKR to properly decode the payload.



Once the callback is added, click on the “OK” button and save it.



The next callback to add is a SERVICE callback with a GEOLOC option.



<p id="gdcalert3" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps2.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert4">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps2.png "image_tooltip")




This callback allows you to receive the location of your device.

To set the callback it’s the same process than before. You need to add a custom callback and set it to SERVICE type with a GEOLOC option.



The CHANNEL must be a URL channel and the link is the same than the data callback:[ https://api.unabiz.io/v1/unasensors/events-sigfox](https://api.unabiz.io/v1/unasensors/events-sigfox).



The HTTP Method and Content-Type are on POST method and application/json to send the information thanks to a json format.



The body is the following one:



{

  	"lng": "{lng}",

  	"time": "{time}",

  	"seqNumber": "{seqNumber}",

  	"avgSnr": "{avgSnr}",

  	"station": "{station}",

  	"rssi": "{rssi}",

  	"device": "{device}",

  	"snr": "{snr}",

  	"lat": "{lat}",

  	"duplicate": "{duplicate}",

  	"radius": "{radius}",

  	"geolocSource": "{geolocSource}"

}

The body of the callback is specific to send location messages and allows Sigfox to locate your device when a message is sent.



With these two callbacks, UnaConnect is almost ready. The next step will be to add the device on UnaMe App or Web.


## Installing the UnaMe app -

UnaMe App is a mobile application which allows you to visualize the data of your device. The application is available on the iOS AppStore or Android Play Store for free.



The first page you will see on UnaMe App is a blank page with a “+” button on the top right of the screen.



By clicking on the “+” Button you will have to add your device.

Adding the device allows you to visualize the data and to decode the message of your device.



<p id="gdcalert4" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps3.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert5">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps3.png "image_tooltip")




Once the “Add Sensor” page displayed, you must write the sensor sigfox ID. The sensor ID is the one you entered in **backend.sigfox.com** to register your device.



<p id="gdcalert5" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps4.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert6">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps4.png "image_tooltip")


Finally, click on next and enter a pin code for more security. You can add a name to your sensor and press “Save” to save it.

Once the device is added you will see it on the main page. This page allows you to see all of your sensors.



<p id="gdcalert6" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps5.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert7">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps5.png "image_tooltip")




If you press one sensor you will see a new screen with the name of your sensor, its type and a button “Data History”.



<p id="gdcalert7" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps6.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert8">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps6.png "image_tooltip")




For UnaMKR the data history will display a graph for each sensor.

This page allows you to visualize every sensor and change the period of display on the top.



<p id="gdcalert8" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/UnaMKR-50-Apps7.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert9">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/UnaMKR-50-Apps7.png "image_tooltip")



























<!-- Docs to Markdown version 1.0β17 -->
