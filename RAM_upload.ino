/*
  Instructions for writing to the AD9910 RAM
 
  1)Power up device and apply REF CLK source.
  2)Issue master reset to place the device in known state.
    See Register Map section in data sheet for default values, if desired.
    Note, the RAM enable bit is disabled by default.
  3)Send instruction byte to register 0x02h.
    Next, configure internal system clock as desired.
  4)Send IO_UPDATE.
  5)Send instruction byte to RAM profile 0 register 0x0Eh.
    Use RAM Profile 0 to setup all memory locations (0 up to 1023) to be used
    in the application via the start address and end address values.
  6)Set external profile pins to logic low and send IO_UPDATE.
  7)Send instruction byte to RAM register 0x016h.
    Next, send 32-bits per each memory location as defined by the start
    and end addresses in step 5. For example, if 1000 memory locations are used,
    send 32K bits (32*1000) of data at this time. Note, phase or amplitude data must
    be padded with zeros since there’s only 16 bits of phase offset and 14 bits of amplitude.
  8)Send IO_UPDATE.
  9)RAM is loaded.
  10)Send instruction byte and desired configuration for each
    RAM profile register (0x0Eh to 0x15h) to be used.
  11)Send instruction byte to register 0x00h.
    Configure RAM destination bits as desired and enable the RAM enable bit.
    Set any other desired bits in register at this time.
  12)Send IO_UPDATE.
  13)Use the profile pins to drive RAM.
    Note, profile pins changes must meet setup and hold times to the SYNC_CLK rising edge.
*/

void UploadRAM()  //just for DDS_A, for testing
{
  int startAddress1_A = 0;
  int endAddress1_A = 1023;
  unsigned int timeStep1_A = 1000;   //Frequency ramp, time step (ns)
  unsigned int timeStepFTW;          //Temp/intermediate variable
  int sweepMode = B001;              //RAM sweep mode ()
  
  //step 1, done in InitializeReset_A()
  //step 2, done in InitializeReset_A()
  //step 3:
    reg_00_A[1] = reg_00_A[1] & 0b00011111;
    UpdateRegister_A(reg_00_A, 4);
  //step 4, done in InitializeReset_A()
  //step 5:
    reg_0E_A[1] = 0b00000000;
    timeStepFTW = (multPLL_A*timeStep1_A*(sourceFreq_A/1000000))/(4*1000);
    reg_0E_A[2] = (timeStepFTW >> 8) & 0b11111111;
    reg_0E_A[3] = timeStepFTW & 0b11111111;
    reg_0E_A[4] = (endAddress1_A >> 2) & 0b11111111;
    reg_0E_A[5] = (endAddress1_A << 8) & 0b11111111;
    reg_0E_A[6] = (startAddress1_A >> 2) & 0b11111111;
    reg_0E_A[7] = (startAddress1_A << 8) & 0b11111111;
    reg_0E_A[8] = 0b00000000 | sweepMode;
    UpdateRegister_A(reg_0E_A, 8);
  //step 6, done externally and in UpdateRegister_A()
  //step 7:
    //somehow get all the freq info into byte reg_16_A
    //then only send the correct number of entries
    UpdateRegister_A(reg_16_A, 1+endAddress1_A-startAddress1_A);
  //step 8, done in UpdateRegister_A()
  //step 9, success??? we shall see.
  //step 10, done in step 5
  //step 11:
    reg_00_A[1] = reg_00_A[1] | 0b10000000;
    UpdateRegister_A(reg_00_A, 4);
  //step 12, done in UpdateRegister_A
  //step 13, done externally
}

