/*
  AD9910 Function Generator code, for versatile triple DDS control
  This was written in Arduino 1.6.5,
  for an Arduino Due (programming port), 3.3V, 84MHz
  Daniel Gochnauer, 03/14/17
  
  List of functions (Table of contents)
   ------------------------------------
   void setup()
   void loop()
   void InitializeReset_A()
   void InitializeReset_B()
   void InitializeReset_C()
   void BlinkLED(int blinkCode)
   void ClearGIC()
   
  BlinkLED() codes
   ----------------
   Off = nothing
   Sustained = experiment running
   2x = data transfer
   3x = error
   
   As a rule, self made funtions are first letter capitalized,
   while self made variables are first letter lower case.
*/

#include <SPI.h>

//Designate "kPin" prefix for assigning Ardunio pins
//Suffix "_A" or "_B" or "_C" to distinguish between DDSs.

//DDS_A pins
const byte kPinSYNC_SMP_ERR_A = 23;
const byte kPinRESET_A = 25;
const byte kPinEXT_PWR_DWN_A = 27;
const byte kPinPLL_LOCK_A = 29;
const byte kPinRAM_SWP_OVR_A = 31;
const byte kPinIO_UPDATE_A = 35;
const byte kPinCSB_A = 37;
const byte kPinIO_RESET_A = 39;

//DDS_B pins
const byte kPinSYNC_SMP_ERR_B = 22;
const byte kPinRESET_B = 24;
const byte kPinEXT_PWR_DWN_B = 26;
const byte kPinPLL_LOCK_B = 28;
const byte kPinRAM_SWP_OVR_B = 30;
const byte kPinIO_UPDATE_B = 34;
const byte kPinCSB_B = 36;
const byte kPinIO_RESET_B = 38;

//DDS_C pins
const byte kPinSYNC_SMP_ERR_C = 42;
const byte kPinRESET_C = 43;
const byte kPinEXT_PWR_DWN_C = 45;
const byte kPinPLL_LOCK_C = 47;
const byte kPinRAM_SWP_OVR_C = 49;
const byte kPinIO_UPDATE_C = 44;
const byte kPinCSB_C = 46;
const byte kPinIO_RESET_C = 48;

//General purpose pins
const byte kPinPowerSwitch = 53;  //Power down both DDSs from front panel
const byte kPinLED = 13;  //Default LED pin. Used in function BlinkLED()


//DDS_A default registers
byte reg_00_A[5] = {0x00, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_01_A[5] = {0x01, 0b00000000, 0b01000000, 0b00001000, 0b00100000};
byte reg_02_A[5] = {0x02, 0b00011111, 0b00111111, 0b01000000, 0b00000000};
byte reg_03_A[5] = {0x03, 0b00000000, 0b00000000, 0b01111111, 0b01111111};
byte reg_04_A[5] = {0x04, 0b11111111, 0b11111111, 0b11111111, 0b11111111};
byte reg_05_A[7] = {0x05, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_06_A[7] = {0x06, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_07_A[5] = {0x07, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_08_A[3] = {0x08, 0b00000000, 0b00000000};
byte reg_09_A[5] = {0x09, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0A_A[5] = {0x0A, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0B_A[9] = {0x0B, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0C_A[9] = {0x0C, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0D_A[5] = {0x0D, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0E_A[9] = {0x0E, 0b00001000, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0F_A[9] = {0x0F, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_10_A[9] = {0x10, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_11_A[9] = {0x11, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_12_A[9] = {0x12, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_13_A[9] = {0x13, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_14_A[9] = {0x14, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_15_A[9] = {0x15, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_16_A[1025] = {0x16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//DDS_B default registers
byte reg_00_B[5] = {0x00, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_01_B[5] = {0x01, 0b00000000, 0b01000000, 0b00001000, 0b00100000};
byte reg_02_B[5] = {0x02, 0b00011111, 0b00111111, 0b01000000, 0b00000000};
byte reg_03_B[5] = {0x03, 0b00000000, 0b00000000, 0b01111111, 0b01111111};
byte reg_04_B[5] = {0x04, 0b11111111, 0b11111111, 0b11111111, 0b11111111};
byte reg_05_B[7] = {0x05, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_06_B[7] = {0x06, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_07_B[5] = {0x07, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_08_B[3] = {0x08, 0b00000000, 0b00000000};
byte reg_09_B[5] = {0x09, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0A_B[5] = {0x0A, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0B_B[9] = {0x0B, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0C_B[9] = {0x0C, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0D_B[5] = {0x0D, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0E_B[9] = {0x0E, 0b00001000, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0F_B[9] = {0x0F, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_10_B[9] = {0x10, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_11_B[9] = {0x11, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_12_B[9] = {0x12, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_13_B[9] = {0x13, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_14_B[9] = {0x14, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_15_B[9] = {0x15, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_16_B[1025] = {0x16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//DDS_C default registers
byte reg_00_C[5] = {0x00, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_01_C[5] = {0x01, 0b00000000, 0b01000000, 0b00001000, 0b00100000};
byte reg_02_C[5] = {0x02, 0b00011111, 0b00111111, 0b01000000, 0b00000000};
byte reg_03_C[5] = {0x03, 0b00000000, 0b00000000, 0b01111111, 0b01111111};
byte reg_04_C[5] = {0x04, 0b11111111, 0b11111111, 0b11111111, 0b11111111};
byte reg_05_C[7] = {0x05, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_06_C[7] = {0x06, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_07_C[5] = {0x07, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_08_C[3] = {0x08, 0b00000000, 0b00000000};
byte reg_09_C[5] = {0x09, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0A_C[5] = {0x0A, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0B_C[9] = {0x0B, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0C_C[9] = {0x0C, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0D_C[5] = {0x0D, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0E_C[9] = {0x0E, 0b00001000, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_0F_C[9] = {0x0F, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_10_C[9] = {0x10, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_11_C[9] = {0x11, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_12_C[9] = {0x12, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_13_C[9] = {0x13, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_14_C[9] = {0x14, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_15_C[9] = {0x15, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
byte reg_16_C[1025] = {0x16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


//Serial input array
char getInstructCicero[76];  //Input ASCII text commands from Cicero to change DDS settings
  int gIC = 0;  //Counter for getInstructCicero[]
unsigned long timeoutGIC = 0;  //Timeout parameter for taking Cicero input


//DDS_A parameters
unsigned long multPLL_A = 100;          //Reference clock multiplier (x10MHz)
unsigned long sourceFreq_A = 10000000;  //Reference clock frequency (Hz)
unsigned long frequency_A = 0;          //Profile_0 output frequency (Hz)
unsigned long freqHigh_A = 400000000;   //Frequency ramp, upper limit (Hz)
unsigned long freqLow_A = 0;            //Frequency ramp, lower limit (Hz)
unsigned long freqRampPosTime_A = 1000;      //Frequency ramp, positive time step (ns)
unsigned long freqRampPosStep_A = 10000000;  //Frequency ramp, positive freq step (Hz)
unsigned long freqRampNegTime_A = 1000;      //Frequency ramp, negative time step (ns)
unsigned long freqRampNegStep_A = 10000000;  //Frequency ramp, negative freq step (Hz)

//DDS_B parameters
unsigned long multPLL_B = 100;          //Reference clock multiplier (x10MHz)
unsigned long sourceFreq_B = 10000000;  //Reference clock frequency (Hz)
unsigned long frequency_B = 0;          //Profile_0 output frequency (Hz)
unsigned long freqHigh_B = 400000000;   //Frequency ramp, upper limit (Hz)
unsigned long freqLow_B = 0;            //Frequency ramp, lower limit (Hz)
unsigned long freqRampPosTime_B = 1000;      //Frequency ramp, positive time step (ns)
unsigned long freqRampPosStep_B = 10000000;  //Frequency ramp, positive freq step (Hz)
unsigned long freqRampNegTime_B = 1000;      //Frequency ramp, negative time step (ns)
unsigned long freqRampNegStep_B = 10000000;  //Frequency ramp, negative freq step (Hz)

//DDS_C parameters
unsigned long multPLL_C = 100;          //Reference clock multiplier (x10MHz)
unsigned long sourceFreq_C = 10000000;  //Reference clock frequency (Hz)
unsigned long frequency_C = 0;          //Profile_0 output frequency (Hz)
unsigned long freqHigh_C = 400000000;   //Frequency ramp, upper limit (Hz)
unsigned long freqLow_C = 0;            //Frequency ramp, lower limit (Hz)
unsigned long freqRampPosTime_C = 1000;      //Frequency ramp, positive time step (ns)
unsigned long freqRampPosStep_C = 10000000;  //Frequency ramp, positive freq step (Hz)
unsigned long freqRampNegTime_C = 1000;      //Frequency ramp, negative time step (ns)
unsigned long freqRampNegStep_C = 10000000;  //Frequency ramp, negative freq step (Hz)




void setup()
{
  //Initialize pin settings
  
  //DDS_A pin states
  pinMode(kPinSYNC_SMP_ERR_A, INPUT);
  pinMode(kPinRESET_A, OUTPUT);
  digitalWrite(kPinRESET_A, LOW);
  pinMode(kPinEXT_PWR_DWN_A, OUTPUT);
  digitalWrite(kPinEXT_PWR_DWN_A, LOW);
  pinMode(kPinPLL_LOCK_A, INPUT);
  pinMode(kPinRAM_SWP_OVR_A, INPUT);
  pinMode(kPinIO_UPDATE_A, OUTPUT);
  digitalWrite(kPinIO_UPDATE_A, LOW);
  pinMode(kPinCSB_A, OUTPUT);
  digitalWrite(kPinCSB_A, LOW);
  pinMode(kPinIO_RESET_A, OUTPUT);
  digitalWrite(kPinIO_RESET_A, LOW);
  
  //DDS_B pin states
  pinMode(kPinSYNC_SMP_ERR_B, INPUT);
  pinMode(kPinRESET_B, OUTPUT);
  digitalWrite(kPinRESET_B, LOW);
  pinMode(kPinEXT_PWR_DWN_B, OUTPUT);
  digitalWrite(kPinEXT_PWR_DWN_B, LOW);
  pinMode(kPinPLL_LOCK_B, INPUT);
  pinMode(kPinRAM_SWP_OVR_B, INPUT);
  pinMode(kPinIO_UPDATE_B, OUTPUT);
  digitalWrite(kPinIO_UPDATE_B, LOW);
  pinMode(kPinCSB_B, OUTPUT);
  digitalWrite(kPinCSB_B, LOW);
  pinMode(kPinIO_RESET_B, OUTPUT);
  digitalWrite(kPinIO_RESET_B, LOW);
  
  //DDS_C pin states
  pinMode(kPinSYNC_SMP_ERR_C, INPUT);
  pinMode(kPinRESET_C, OUTPUT);
  digitalWrite(kPinRESET_C, LOW);
  pinMode(kPinEXT_PWR_DWN_C, OUTPUT);
  digitalWrite(kPinEXT_PWR_DWN_C, LOW);
  pinMode(kPinPLL_LOCK_C, INPUT);
  pinMode(kPinRAM_SWP_OVR_C, INPUT);
  pinMode(kPinIO_UPDATE_C, OUTPUT);
  digitalWrite(kPinIO_UPDATE_C, LOW);
  pinMode(kPinCSB_C, OUTPUT);
  digitalWrite(kPinCSB_C, LOW);
  pinMode(kPinIO_RESET_C, OUTPUT);
  digitalWrite(kPinIO_RESET_C, LOW);
  
  //General purpose pin states
  pinMode(kPinPowerSwitch, INPUT_PULLUP);
  pinMode(kPinLED, OUTPUT);
  digitalWrite(kPinLED, LOW);
  
  //Open serial communications
  Serial.begin(9600);   //Serial monitor. Use for debugging
  Serial1.begin(9600);  //Cicero serial. Typical baud rate to interface with computers
  
  //SPI setup, for communicating with the DDSs
  SPI.begin();
  SPI.setClockDivider(255);
  SPI.setDataMode(SPI_MODE0);
  
  
  InitializeReset_A();
  InitializeReset_B();
  InitializeReset_C();
  
BlinkLED(2);
delay(2000);
}



void loop()
{
  while (digitalRead(kPinPowerSwitch) == HIGH) //or LOW
  {
    if(Serial1.available()>0){
      getInstructCicero[gIC] = Serial1.read();  //Stores characters from Cicero in an array
      gIC++;
      timeoutGIC = millis();
    }
    if(gIC>=1 && millis()-timeoutGIC>500){  //Times out 0.5 seconds after last byte of Cicero input
      ClearGIC();
    }
    if(gIC>=1 && getInstructCicero[gIC-1]==';'){
      InterpretCicero();  //Interprets Cicero serial data, then gives values to sendInstructDDS and sendPacketsDDS[]
      delay(1000);
      ClearGIC();
    }
  }
  
  digitalWrite(kPinEXT_PWR_DWN_A, HIGH);
  digitalWrite(kPinEXT_PWR_DWN_B, HIGH);
  digitalWrite(kPinEXT_PWR_DWN_C, HIGH);
  while (digitalRead(kPinPowerSwitch) == LOW) //or HIGH
    {} //Do nothing while in power down mode. Wait for switch to switch.
  InitializeReset_A();
  InitializeReset_B();
  InitializeReset_C();
}



void InitializeReset_A()
{
  //Reset settings
  delay(500);
  digitalWrite(kPinEXT_PWR_DWN_A, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinIO_RESET_A, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinRESET_A, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinRESET_A, HIGH);
  delayMicroseconds(4);
  digitalWrite(kPinRESET_A, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinIO_RESET_A, HIGH);
  delayMicroseconds(4);
  digitalWrite(kPinIO_RESET_A, LOW);
  delayMicroseconds(40);
  
  //System clock "divide by 2" disable
  reg_02_A[3] = reg_02_A[3] | 0b10000000;
  //Enable PLL multiplier
  reg_02_A[3] = reg_02_A[3] | 0b00000001;
  //Set system clock to 1000MHz
  reg_02_A[4] = 0b11001000;
  reg_02_A[1] = (reg_02_A[1] & 0b11111000) | 0B101;
  //Update register
  UpdateRegister_A(reg_02_A, 4);
  
  //DRG freq high limit (400MHz)
  reg_0B_A[1] = 0b01100110;
  reg_0B_A[2] = 0b01100110;
  reg_0B_A[3] = 0b01100110;
  reg_0B_A[4] = 0b01100110;
  //Update register
  UpdateRegister_A(reg_0B_A, 8);
  
  //DRG fast ramp up (10MHz/1us)
  reg_0C_A[5] = 0b00000010;
  reg_0C_A[6] = 0b10001111;
  reg_0C_A[7] = 0b01011100;
  reg_0C_A[8] = 0b00101001;
  reg_0D_A[3] = 0b00000000;
  reg_0D_A[4] = 0b11111010;
  //DRG fast ramp down (10MHz/1us)
  reg_0C_A[1] = 0b00000010;
  reg_0C_A[2] = 0b10001111;
  reg_0C_A[3] = 0b01011100;
  reg_0C_A[4] = 0b00101001;
  reg_0D_A[1] = 0b00000000;
  reg_0D_A[2] = 0b11111010;
  //Update registers
  UpdateRegister_A(reg_0C_A, 8);
  UpdateRegister_A(reg_0D_A, 4);
  
  //OSK max amplitude for switching
  reg_09_A[3] = 0b11111111;
  reg_09_A[4] = 0b11111100;
  //Update register
  UpdateRegister_A(reg_09_A, 4);
  
  //DRG freq low limit (200MHz)
  reg_0B_A[5] = 0b00110011;
  reg_0B_A[6] = 0b00110011;
  reg_0B_A[7] = 0b00110011;
  reg_0B_A[8] = 0b00110011;
  //Update register
  UpdateRegister_A(reg_01_A, 4);
  
  //DRG enable
  reg_01_A[2] = reg_01_A[2] | 0b00001000;
  //Update register
  UpdateRegister_A(reg_01_A, 4);
  
  //Enable OSK TTL control
  reg_00_A[2] = reg_00_A[2] | 0b10000000;
  reg_00_A[3] = (reg_00_A[3] | 0b00000010) & 0b11111110;
  UpdateRegister_A(reg_00_A, 4);
}


void InitializeReset_B()
{
  //Reset settings
  delay(500);
  digitalWrite(kPinEXT_PWR_DWN_B, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinIO_RESET_B, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinRESET_B, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinRESET_B, HIGH);
  delayMicroseconds(4);
  digitalWrite(kPinRESET_B, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinIO_RESET_B, HIGH);
  delayMicroseconds(4);
  digitalWrite(kPinIO_RESET_B, LOW);
  delayMicroseconds(40);
  
  //System clock "divide by 2" disable
  reg_02_B[3] = reg_02_B[3] | 0b10000000;
  //Enable PLL multiplier
  reg_02_B[3] = reg_02_B[3] | 0b00000001;
  //Set system clock to 1000MHz
  reg_02_B[4] = 0b11001000;
  reg_02_B[1] = (reg_02_B[1] & 0b11111000) | 0B101;
  //Update register
  UpdateRegister_B(reg_02_B, 4);
  
  //DRG freq high limit (400MHz)
  reg_0B_B[1] = 0b01100110;
  reg_0B_B[2] = 0b01100110;
  reg_0B_B[3] = 0b01100110;
  reg_0B_B[4] = 0b01100110;
  //Update register
  UpdateRegister_B(reg_0B_B, 8);
  
  //DRG fast ramp up (10MHz/1us)
  reg_0C_B[5] = 0b00000010;
  reg_0C_B[6] = 0b10001111;
  reg_0C_B[7] = 0b01011100;
  reg_0C_B[8] = 0b00101001;
  reg_0D_B[3] = 0b00000000;
  reg_0D_B[4] = 0b11111010;
  //DRG fast ramp down (10MHz/1us)
  reg_0C_B[1] = 0b00000010;
  reg_0C_B[2] = 0b10001111;
  reg_0C_B[3] = 0b01011100;
  reg_0C_B[4] = 0b00101001;
  reg_0D_B[1] = 0b00000000;
  reg_0D_B[2] = 0b11111010;
  //Update registers
  UpdateRegister_B(reg_0C_B, 8);
  UpdateRegister_B(reg_0D_B, 4);
  
  //OSK max amplitude for switching
  reg_09_B[3] = 0b11111111;
  reg_09_B[4] = 0b11111100;
  //Update register
  UpdateRegister_B(reg_09_B, 4);
  
  //DRG freq low limit (200MHz)
  reg_0B_B[5] = 0b00110011;
  reg_0B_B[6] = 0b00110011;
  reg_0B_B[7] = 0b00110011;
  reg_0B_B[8] = 0b00110011;
  //Update register
  UpdateRegister_B(reg_01_B, 4);
  
  //DRG enable
  reg_01_B[2] = reg_01_B[2] | 0b00001000;
  //Update register
  UpdateRegister_B(reg_01_B, 4);
  
  //Enable OSK TTL control
  reg_00_B[2] = reg_00_B[2] | 0b10000000;
  reg_00_B[3] = (reg_00_B[3] | 0b00000010) & 0b11111110;
  UpdateRegister_B(reg_00_B, 4);
}


void InitializeReset_C()
{
  //Reset settings
  delay(500);
  digitalWrite(kPinEXT_PWR_DWN_C, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinIO_RESET_C, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinRESET_C, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinRESET_C, HIGH);
  delayMicroseconds(4);
  digitalWrite(kPinRESET_C, LOW);
  delayMicroseconds(40);
  digitalWrite(kPinIO_RESET_C, HIGH);
  delayMicroseconds(4);
  digitalWrite(kPinIO_RESET_C, LOW);
  delayMicroseconds(40);
  
  //System clock "divide by 2" disable
  reg_02_C[3] = reg_02_C[3] | 0b10000000;
  //Enable PLL multiplier
  reg_02_C[3] = reg_02_C[3] | 0b00000001;
  //Set system clock to 1000MHz
  reg_02_C[4] = 0b11001000;
  reg_02_C[1] = (reg_02_C[1] & 0b11111000) | 0B101;
  //Update register
  UpdateRegister_C(reg_02_C, 4);
  
  //DRG freq high limit (400MHz)
  reg_0B_C[1] = 0b01100110;
  reg_0B_C[2] = 0b01100110;
  reg_0B_C[3] = 0b01100110;
  reg_0B_C[4] = 0b01100110;
  //Update register
  UpdateRegister_C(reg_0B_C, 8);
  
  //DRG fast ramp up (10MHz/1us)
  reg_0C_C[5] = 0b00000010;
  reg_0C_C[6] = 0b10001111;
  reg_0C_C[7] = 0b01011100;
  reg_0C_C[8] = 0b00101001;
  reg_0D_C[3] = 0b00000000;
  reg_0D_C[4] = 0b11111010;
  //DRG fast ramp down (10MHz/1us)
  reg_0C_C[1] = 0b00000010;
  reg_0C_C[2] = 0b10001111;
  reg_0C_C[3] = 0b01011100;
  reg_0C_C[4] = 0b00101001;
  reg_0D_C[1] = 0b00000000;
  reg_0D_C[2] = 0b11111010;
  //Update registers
  UpdateRegister_C(reg_0C_C, 8);
  UpdateRegister_C(reg_0D_C, 4);
  
  //OSK max amplitude for switching
  reg_09_C[3] = 0b11111111;
  reg_09_C[4] = 0b11111100;
  //Update register
  UpdateRegister_C(reg_09_C, 4);
  
  //DRG freq low limit (200MHz)
  reg_0B_C[5] = 0b00110011;
  reg_0B_C[6] = 0b00110011;
  reg_0B_C[7] = 0b00110011;
  reg_0B_C[8] = 0b00110011;
  //Update register
  UpdateRegister_C(reg_01_C, 4);
  
  //DRG enable
  reg_01_C[2] = reg_01_C[2] | 0b00001000;
  //Update register
  UpdateRegister_C(reg_01_C, 4);
  
  //Enable OSK TTL control
  reg_00_C[2] = reg_00_C[2] | 0b10000000;
  reg_00_C[3] = (reg_00_C[3] | 0b00000010) & 0b11111110;
  UpdateRegister_C(reg_00_C, 4);
}


void BlinkLED(int blinkCode)  //Makes LED blink a number of times, to signal some process or error
{
  if(blinkCode==0){  //Steady "on" state
    digitalWrite(kPinLED, HIGH);
    delay(10000);  //LED steady state 'on' for 10 seconds (change to length of experiment)
    digitalWrite(kPinLED, LOW);
  }
  else{
    for(int i = 0; i<blinkCode; i++){
      digitalWrite(kPinLED, HIGH);
      delay(300);
      digitalWrite(kPinLED, LOW);
      delay(300);
    }
  }
}


void ClearGIC(){
  for(int i=0; i<76; i++){
    getInstructCicero[i]='\0';
  }
  gIC = 0;
}

