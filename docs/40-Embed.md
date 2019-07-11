<!----- Conversion time: 1.069 seconds.


Using this Markdown file:

1. Cut and paste this output into your source file.
2. See the notes and action items below regarding this conversion run.
3. Check the rendered output (headings, lists, code blocks, tables) for proper
   formatting and use a linkchecker before you publish this page.

Conversion notes:

* Docs to Markdown version 1.0β17
* Wed Jul 10 2019 02:56:38 GMT-0700 (PDT)
* Source doc: https://docs.google.com/a/unabiz.com/open?id=1ZG2dBnyxLGYOP0dFAadX1LpNpQc_CZLrprost_mkP_Q

WARNING:
Inline drawings not supported: look for ">>>>>  gd2md-html alert:  inline drawings..." in output.


WARNING:
You have 3 H1 headings. You may want to use the "H1 -> H2" option to demote all headings by one level.

----->


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 2; ALERTS: 10.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p><a href="#gdcalert1">alert1</a>
<a href="#gdcalert2">alert2</a>
<a href="#gdcalert3">alert3</a>
<a href="#gdcalert4">alert4</a>
<a href="#gdcalert5">alert5</a>
<a href="#gdcalert6">alert6</a>
<a href="#gdcalert7">alert7</a>
<a href="#gdcalert8">alert8</a>
<a href="#gdcalert9">alert9</a>
<a href="#gdcalert10">alert10</a>

<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>



# UnaMKR Documentation


## Your access to the 0G World


# Introduction

This document describes how to use UnaMKR in Embed mode.

This mode allows to run code directly into the module, accessing lowest level features, enabling key features such as ultra low power, precise sensor control, radio access (Sigfox and BLE).


# Prerequisites



*   Make sure UnaMKR is set to Embed mode (J6, J7, J11 to the right)
*   Check documentation "UnaMKR - Modes" for more details.


# Getting Started with Embed Mode


## Development environment

Development tool: armKEIL (includes IDE, C/C++ compiler, debugger… ect.)

Path configuration:



*   Library
    *   BlueNRG1_Periph_Driver
    *   Bluetooth_LE
        *   library
    *   CMSIS
    *   cryptolib
    *   hal
    *   S2LP_Library
    *   SDK_Eval_BlueNRG1
    *   SDK_Eval_S2LP
    *   Sigfox
        *   CM0
        *   rf_lib
        *   sigfox_lib_v.2.6.0
*   Projects
    *   SigFox_Applications
        *   common
        *   Standalone_SigFox_Sensors_Sleep_Demo

The FLASH memory of UnaMKR contains application code, SigFox and BlueNRG parameters. The address of these parameters are fixed and they should not be erased. Therefore, the limitation of application code (output binary file) size is 154kB which starts from 0x10040000 to 0x100667FF.


## Project configuration


### [Device] Tab

Select device  BlueNRG-2 Series → STBlueNRG-2 → BlueNRG-2



<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)


### [Target] Tab


    Crystals (Xtal) 12.0 MHz, on-chip IROM (FLASH) starts from 0x10040000 with size 0x40000 and on-chip IRAM starts from 0x20000000 with size 0x6000.



<p id="gdcalert2" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert3">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)


### [Output] Tab


    The debug information and HEX file are optional.



<p id="gdcalert3" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert4">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)


### [Listing] Tab (optional)



<p id="gdcalert4" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert5">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)


### [User] Tab


    Select the checkbox in “After Build/Rebuild” and write user command (if the development tool is installed at path C:\Keil_v5\) “C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o ./Release/Objects/bin/UnaMKR.bin ./Release/Objects/UnaMKR_ALL.axf”.



<p id="gdcalert5" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert6">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)

C/C++ configuration



*   Define

    HS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ LS_SOURCE=LS_SOURCE_INTERNAL_RO BLUENRG2_DEVICE  \
SMPS_INDUCTOR=SMPS_INDUCTOR_10uH FOR_ALL USE_FLASH ERASE_VALUE_IS_FF MONARCH_FEATURE_ENABLED USER_DEFINED_PLATFORM=MONARCH_REF_DESIGN

*   Include paths

    ..\inc;..\..\..\..\Library\hal\inc;..\..\..\..\Library\BlueNRG1_Periph_Driver\inc;..\..\..\..\Library\CMSIS\Device\ST\BlueNRG1\Include;..\..\..\..\Library\CMSIS\Include;..\..\..\..\Library\Bluetooth_LE\inc;..\..\..\..\Library\SDK_Eval_BlueNRG1\inc;..\..\..\..\Library\SDK_Eval_S2LP\inc;..\..\common\id_key_retriever;..\..\..\..\Library\Sigfox\sigfox_lib_v.2.6.0;..\..\common\st_lowlevel;..\..\..\..\Library\BLE_Application\layers_inc;..\..\..\..\Library\BLE_Application\OTA\inc;..\..\..\..\Library\S2LP_Library\inc;..\..\..\..\Library\Sigfox\rf_lib;..\..\common\id_key_retriever\CM0

*   Misc controls

    --diag_suppress=951 --wchar32




<p id="gdcalert6" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert7">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)

ASM configuration



*   Misc controls: --cpreproc



<p id="gdcalert7" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert8">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)

Linker configuration



*   Scatter file: .\BlueNRG2.sct
*   Misc controls: --no_strict_wchar_size



<p id="gdcalert8" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert9">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)

For example, use ST-Link Debugger



<p id="gdcalert9" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert10">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)



<p id="gdcalert10" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert11">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)


<!-- Docs to Markdown version 1.0β17 -->
