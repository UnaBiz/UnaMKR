:: # ######################################################### #
:: #   BlueNRG-2 application firmware updater                  #
:: #                                                           #
:: #    * Tool: ST-BlueNRG Flasher launcher utility            #
:: # ######################################################### #
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe -h
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe flash -h
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe read -h
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe mass_erase -h
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe verify_momory -h
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe port -h
:: flasher\BlueNRG-1_2_Flasher_Launcher.exe stLink -h
::
:: # ######################################################### #
:: #   File Selector                                           #
:: # ######################################################### #
@echo off
echo  * Please select firmware file you would like to update:
set dialog="about:<input type=file id=FILE><script>FILE.click();new ActiveXObject
set dialog=%dialog%('Scripting.FileSystemObject').GetStandardStream(1).WriteLine(FILE.value);
set dialog=%dialog%close();resizeTo(0,0);</script>"

FOR /f "tokens=* delims=" %%p in ('mshta.exe %dialog%') do set "file=%%p"

IF "%file%"=="" (
  echo on
  echo Exit flasher...
  exit 0
)

echo    Selected file is : "%file%"
:: # ######################################################### #
:: #   Copy Firmware to Current Folder                         #
:: # ######################################################### #
:: # ######################################################### #
:: #   Start to Program Application Firmware                   #
:: #                                                           #
:: #   <Input Arguments>                                       #
:: #    * Start Address: 0x10040000                            #
:: #    * Binary File  : UnaMKR.bin                            #
:: #    * Port         : detect automatically                  #
:: #    * Interface    : UART over USB                         #
:: #    * Options      : Verify enable                         #
:: #                     Log enable                            #
:: #                                                           #
:: # ######################################################### #
IF EXIST "BlueNRG-1_2_Flasher_Launcher.exe" (

  copy "%file%" UnaMKR.bin
  echo on
  BlueNRG-1_2_Flasher_Launcher.exe flash -address 0x10040000 -f "UnaMKR.bin" -auto -UART -verify -l

) ELSE IF EXIST ".\flasher\BlueNRG-1_2_Flasher_Launcher.exe" (
  copy "%file%" UnaMKR.bin
  echo on
  .\flasher\BlueNRG-1_2_Flasher_Launcher.exe flash -address 0x10040000 -f "UnaMKR.bin" -auto -UART -verify -l

) ELSE IF EXIST "%userprofile%\ST\BlueNRG-1_2 Flasher Utility 3.1.0\Application\BlueNRG-1_2_Flasher_Launcher.exe" (
  cd "%userprofile%\ST\BlueNRG-1_2 Flasher Utility 3.1.0\Application"
  copy "%file%" UnaMKR.bin
  echo on
  BlueNRG-1_2_Flasher_Launcher.exe flash -address 0x10040000 -f "UnaMKR.bin" -auto -UART -verify -l

)

@pause
:: # ######################################################### #
:: #   Delete Temporary File                                   #
:: # ######################################################### #
@del UnaMKR.bin
