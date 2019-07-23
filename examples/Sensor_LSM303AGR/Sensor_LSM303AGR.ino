/*
 *  Sample code for LSM303AGR sensors
 *  
 *  Development kit: UnaMKR
 *
 *  Author: UnaBiz 
 *  *******************************************************
 *  Arduino Boards Manager -> Search 'MKRZERO' -> Install
 */
/**
 ******************************************************************************
 * @file    X_NUCLEO_IKS01A2_LSM303AGR_DataLog_Terminal.ino
 * @author  AST
 * @version V1.0.0
 * @date    7 September 2017
 * @brief   Arduino test application for the STMicrolectronics X-NUCLEO-IKS01A2
 *          MEMS Inertial and Environmental sensor expansion board.
 *          This application makes use of C++ classes obtained from the C
 *          components' drivers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes */
#include <LSM303AGR_ACC_Sensor.h>
#include <LSM303AGR_MAG_Sensor.h>

/* Definitions */
#if 0
# define DEV_I2C Wire1   //Define which I2C bus is used. Wire1 for the Arduino Due
# define SerialPort Serial
#else
# define DEV_I2C    Wire    //Or Wire
# define SerialPort Serial
#endif

/* Components */
LSM303AGR_ACC_Sensor *Acc;
LSM303AGR_MAG_Sensor *Mag;

/* Arduino Setup */
void setup() {

  // Initialize serial for output.
  SerialPort.begin(115200);
  
  // Initialize I2C bus.
  DEV_I2C.begin();

  // Initlialize components.
  Acc = new LSM303AGR_ACC_Sensor(&DEV_I2C);
  Mag = new LSM303AGR_MAG_Sensor(&DEV_I2C);
  
  // configuration of lsm303agr
  Acc->SetFS(2.0);    // 2g
  Acc->SetODR(10.0);  // 10Hz
  Mag->SetFS(2.0);    // 2g
  Mag->SetODR(10.0);  // 10Hz
  delay(500);
  
  Acc->Enable();
  Mag->Enable();

  // Delay 5 seconds before loop entry
  delay(5000);
}

/* Arduino Main Loop */
void loop() {
  char    str[128];
  int32_t accelerometer[3];
  int32_t magnetometer[3];

  // Read accelerometer LSM303AGR.
  Acc->GetAxes(accelerometer);
  
  // Output data.
  sprintf(str, " Acc (X,Y,Z)[mg]: (%5d,%5d,%5d) |",
          accelerometer[0],
          accelerometer[1],
          accelerometer[2]);
  Serial.print((const char*)str);

  // Read magnetometer LSM303AGR.
  Mag->GetAxes(magnetometer);
  
  sprintf(str, " Mag (X,Y,Z)[mGauss]: (%5d,%5d,%5d)",
          magnetometer[0],
          magnetometer[1],
          magnetometer[2]);
  Serial.println((const char*)str);
  
  // Delay 2 second
  delay(2000);

  // Flush serial port
  SerialPort.flush();
}

/* Expected Output: (example)

 Acc (X,Y,Z)[mg]: (   35,   27,  963) | Mag (X,Y,Z)[mGauss]: ( -211,  135, -496)

*/
