/*
  InterpretCicero() commands
   ----------------           {X = A,B,C}
   PLL X, N;                  {0 <= N <= 100, max}
   DRG X, N;                  {N = 0,1}
   freq XP, N;                {0 <= P <= 7} & {0 <= N <= 400000000} Hz
   freq high X, N;            {freqLow_X <= N <= 400000000} Hz
   freq low X, N;             {0 <= N <= freqHigh_X} Hz
   freq ramp pos time X, N;   {4 <= N <= 262140} ns
   freq ramp pos step X, N;   {0 <= N <= 400000000} Hz
   freq ramp neg time X, N;   {4 <= N <= 262140} ns
   freq ramp neg step X, N;   {0 <= N <= 400000000} Hz
   freq ramps time X, N;      {4 <= N <= 262140} ns
   freq ramps step X, N;      {0 <= N <= 400000000} Hz
   freq ramps time N;         {4 <= N <= 262140} ns
   freq ramps step N;         {0 <= N <= 400000000} Hz
   OSK X, N;                  {N = 0,1}
   amp X, N;                  {0 <= N <= 16383, max}
   LLR X, N;                  {N = 0,1}
   reset X;

*/

void InterpretCicero()
{
  //Intermediary/calculated variables used here
  unsigned long multPLL;       //Temp multPLL value
  unsigned long sourceFreq;    //Temp sourceFreq value
  unsigned long varFTW;        //Frequency tuning word
  unsigned long freqLimitFTW;  //Ramp frequency limits, FTW
  unsigned long freqStepFTW;   //Ramp frequency steps (for both frequency and time steps), FTW / M
  unsigned long amplitude;     //Amplitude ramp, DDS output amplitude. (units?)
  int varVCO;                  //Set DDS internal VCO depending on PLL multiplier
  
  if((getInstructCicero[0]=='P' || getInstructCicero[0]=='p') && 
    (getInstructCicero[1]=='L' || getInstructCicero[1]=='l') && 
    (getInstructCicero[2]=='L' || getInstructCicero[2]=='l') && 
    getInstructCicero[3]==' ' &&
    (getInstructCicero[4]=='A' || getInstructCicero[4]=='B' || getInstructCicero[4]=='C') && 
    getInstructCicero[5]==',' &&
    getInstructCicero[6]==' ')  //Set PLL multiplier
    {
      if(getInstructCicero[4]=='A')
        {sourceFreq = sourceFreq_A;}
      else if(getInstructCicero[4]=='B')
        {sourceFreq = sourceFreq_B;}
      else if(getInstructCicero[4]=='C')
        {sourceFreq = sourceFreq_C;}
      
      multPLL = ATOL(7);
      if(getInstructCicero[7]=='m' && 
        getInstructCicero[8]=='a' && 
        getInstructCicero[9]=='x')
        {multPLL = 1000000000/sourceFreq;}  //Max system clock frequency allowed
      
      if(multPLL*sourceFreq >= 920000000 && multPLL*sourceFreq <= 1000000000){  //Sets correct internal VCO given the input multiplier
        varVCO = B101;}
      else if(multPLL*sourceFreq >= 832000000 && multPLL*sourceFreq < 920000000){
        varVCO = B100;}
      else if(multPLL*sourceFreq >= 656000000 && multPLL*sourceFreq < 832000000){
        varVCO = B011;}
      else if(multPLL*sourceFreq >= 562000000 && multPLL*sourceFreq < 656000000){
        varVCO = B010;}
      else if(multPLL*sourceFreq >= 482000000 && multPLL*sourceFreq < 562000000){
        varVCO = B001;}
      else if(multPLL*sourceFreq >= 420000000 && multPLL*sourceFreq < 482000000){
        varVCO = B000;}
      else{
        multPLL = 1;  //Not in VCO ranges, sets multPLL=1 to ensure other calculations don't fail
        varVCO = B111;
      }
      
      if(getInstructCicero[4]=='A')
      {
        multPLL_A = multPLL;
        if(varVCO != B111){
          reg_02_A[3] = reg_02_A[3] | 0b00000001;  //PLL enable bit high
          reg_02_A[1] = (reg_02_A[1] & 0b11111000) | varVCO;  //set VCO bits
          reg_02_A[4] = (reg_02_A[4] & 0b00000000) | (multPLL_A << 1);  //set multiplier value bits
        }
        else{
          reg_02_A[3] = reg_02_A[3] & 0b11111110;  //PLL enable bit low
          reg_02_A[1] = (reg_02_A[1] & 0b11111000) | varVCO;  //set VCO bits
        }
        UpdateRegister_A(reg_02_A, 4);
      }
      else if(getInstructCicero[4]=='B')
      {
        multPLL_B = multPLL;
        if(varVCO != B111){
          reg_02_B[3] = reg_02_B[3] | 0b00000001;  //PLL enable bit high
          reg_02_B[1] = (reg_02_B[1] & 0b11111000) | varVCO;  //set VCO bits
          reg_02_B[4] = (reg_02_B[4] & 0b00000000) | (multPLL_B << 1);  //set multiplier value bits
        }
        else{
          reg_02_B[3] = reg_02_B[3] & 0b11111110;  //PLL enable bit low
          reg_02_B[1] = (reg_02_B[1] & 0b11111000) | varVCO;  //set VCO bits
        }
        UpdateRegister_B(reg_02_B, 4);
      }
      else if(getInstructCicero[4]=='C')
      {
        multPLL_C = multPLL;
        if(varVCO != B111){
          reg_02_C[3] = reg_02_C[3] | 0b00000001;  //PLL enable bit high
          reg_02_C[1] = (reg_02_C[1] & 0b11111000) | varVCO;  //set VCO bits
          reg_02_C[4] = (reg_02_C[4] & 0b00000000) | (multPLL_C << 1);  //set multiplier value bits
        }
        else{
          reg_02_C[3] = reg_02_C[3] & 0b11111110;  //PLL enable bit low
          reg_02_C[1] = (reg_02_C[1] & 0b11111000) | varVCO;  //set VCO bits
        }
        UpdateRegister_C(reg_02_C, 4);
      }
    
    } //End PLL code block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if((getInstructCicero[0]=='D' || getInstructCicero[0]=='d') &&
    (getInstructCicero[1]=='R' || getInstructCicero[1]=='r') && 
    (getInstructCicero[2]=='G' || getInstructCicero[2]=='g') && 
    getInstructCicero[3]==' ' &&
    (getInstructCicero[4]=='A' || getInstructCicero[4]=='B' || getInstructCicero[4]=='C') && 
    getInstructCicero[5]==',' &&
    getInstructCicero[6]==' ')  //Enable or disable DRG
    {
      if(getInstructCicero[4]=='A')
      {
        if(getInstructCicero[7] == '1')
          {reg_01_A[2] = reg_01_A[2] | 0b00001000;}
        else if(getInstructCicero[7] == '0')
          {reg_01_A[2] = reg_01_A[2] & 0b11110111;}
        UpdateRegister_A(reg_01_A, 4);
      }
      else if(getInstructCicero[4]=='B')
      {
        if(getInstructCicero[7] == '1')
          {reg_01_B[2] = reg_01_B[2] | 0b00001000;}
        else if(getInstructCicero[7] == '0')
          {reg_01_B[2] = reg_01_B[2] & 0b11110111;}
        UpdateRegister_B(reg_01_B, 4);
      }
      else if(getInstructCicero[4]=='C')
      {
        if(getInstructCicero[7] == '1')
          {reg_01_C[2] = reg_01_C[2] | 0b00001000;}
        else if(getInstructCicero[7] == '0')
          {reg_01_C[2] = reg_01_C[2] & 0b11110111;}
        UpdateRegister_C(reg_01_C, 4);
      }
    
    } //End DRG code block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if(getInstructCicero[0]=='f' &&
    getInstructCicero[1]=='r' && 
    getInstructCicero[2]=='e' && 
    getInstructCicero[3]=='q' && 
    getInstructCicero[4]==' ')  //Set main output frequency for DDS
    {
      if(getInstructCicero[7] == ',' && 
        getInstructCicero[8] == ' ')
        {
          UploadProfile();  //////////////GOTO PROFILE UPLOAD
        }
      else if(getInstructCicero[5]=='h' && 
        getInstructCicero[6]=='i' && 
        getInstructCicero[7]=='g' && 
        getInstructCicero[8]=='h' && 
        getInstructCicero[9]==' ' &&
        getInstructCicero[11]==',' &&
        getInstructCicero[12]==' ')  //Set maximum output frequency for the DRG
        {
          if(getInstructCicero[10]=='A')
          {
            freqHigh_A = ATOL(13);
            if(freqHigh_A > freqLow_A)  //Makes sure high and low limits are okay
            {
              freqLimitFTW = CalculateFTW(freqHigh_A, (multPLL_A*sourceFreq_A));
              reg_0B_A[1] = (freqLimitFTW >> 24) & 0b11111111;
              reg_0B_A[2] = (freqLimitFTW >> 16) & 0b11111111;
              reg_0B_A[3] = (freqLimitFTW >> 8) & 0b11111111;
              reg_0B_A[4] = freqLimitFTW & 0b11111111;
              UpdateRegister_A(reg_0B_A, 8);
            }
          }
          else if(getInstructCicero[10]=='B')
          {
            freqHigh_B = ATOL(13);
            if(freqHigh_B > freqLow_B)  //Makes sure high and low limits are okay
            {
              freqLimitFTW = CalculateFTW(freqHigh_B, (multPLL_B*sourceFreq_B));
              reg_0B_B[1] = (freqLimitFTW >> 24) & 0b11111111;
              reg_0B_B[2] = (freqLimitFTW >> 16) & 0b11111111;
              reg_0B_B[3] = (freqLimitFTW >> 8) & 0b11111111;
              reg_0B_B[4] = freqLimitFTW & 0b11111111;
              UpdateRegister_B(reg_0B_B, 8);
            }
          }
          else if(getInstructCicero[10]=='C')
          {
            freqHigh_C = ATOL(13);
            if(freqHigh_C > freqLow_C)  //Makes sure high and low limits are okay
            {
              freqLimitFTW = CalculateFTW(freqHigh_C, (multPLL_C*sourceFreq_C));
              reg_0B_C[1] = (freqLimitFTW >> 24) & 0b11111111;
              reg_0B_C[2] = (freqLimitFTW >> 16) & 0b11111111;
              reg_0B_C[3] = (freqLimitFTW >> 8) & 0b11111111;
              reg_0B_C[4] = freqLimitFTW & 0b11111111;
              UpdateRegister_C(reg_0B_C, 8);
            }
          }
        } //End 'freq high' code block
      
      else if(getInstructCicero[5]=='l' && 
        getInstructCicero[6]=='o' && 
        getInstructCicero[7]=='w' && 
        getInstructCicero[8]==' ' && 
        getInstructCicero[10]==',' && 
        getInstructCicero[11]==' ')  //Set minimum output frequency for the DRG
        {
          if(getInstructCicero[9]=='A')
          {
            freqLow_A = ATOL(12);
            if(freqHigh_A > freqLow_A)  //Makes sure high and low limits are okay
            {
              freqLimitFTW = CalculateFTW(freqLow_A, (multPLL_A*sourceFreq_A));
              reg_0B_A[5] = (freqLimitFTW >> 24) & 0b11111111;
              reg_0B_A[6] = (freqLimitFTW >> 16) & 0b11111111;
              reg_0B_A[7] = (freqLimitFTW >> 8) & 0b11111111;
              reg_0B_A[8] = freqLimitFTW & 0b11111111;
              UpdateRegister_A(reg_0B_A, 8);
            }
          }
          else if(getInstructCicero[9]=='B')
          {
            freqLow_B = ATOL(12);
            if(freqHigh_B > freqLow_B)  //Makes sure high and low limits are okay
            {
              freqLimitFTW = CalculateFTW(freqLow_B, (multPLL_B*sourceFreq_B));
              reg_0B_B[5] = (freqLimitFTW >> 24) & 0b11111111;
              reg_0B_B[6] = (freqLimitFTW >> 16) & 0b11111111;
              reg_0B_B[7] = (freqLimitFTW >> 8) & 0b11111111;
              reg_0B_B[8] = freqLimitFTW & 0b11111111;
              UpdateRegister_B(reg_0B_B, 8);
            }
          }
          else if(getInstructCicero[9]=='C')
          {
            freqLow_C = ATOL(12);
            if(freqHigh_C > freqLow_C)  //Makes sure high and low limits are okay
            {
              freqLimitFTW = CalculateFTW(freqLow_C, (multPLL_C*sourceFreq_C));
              reg_0B_C[5] = (freqLimitFTW >> 24) & 0b11111111;
              reg_0B_C[6] = (freqLimitFTW >> 16) & 0b11111111;
              reg_0B_C[7] = (freqLimitFTW >> 8) & 0b11111111;
              reg_0B_C[8] = freqLimitFTW & 0b11111111;
              UpdateRegister_C(reg_0B_C, 8);
            }
          }
        } //End 'freq low' code block
      
/////////////////////////////////////////////////////////////////////////////////////////////
      
      else if(getInstructCicero[5]=='r' && 
        getInstructCicero[6]=='a' && 
        getInstructCicero[7]=='m' && 
        getInstructCicero[8]=='p' && 
        getInstructCicero[9]==' '){
          if(getInstructCicero[10]=='p' && 
            getInstructCicero[11]=='o' && 
            getInstructCicero[12]=='s' && 
            getInstructCicero[13]==' '){
              if(getInstructCicero[14]=='t' && 
                getInstructCicero[15]=='i' && 
                getInstructCicero[16]=='m' && 
                getInstructCicero[17]=='e' && 
                getInstructCicero[18]==' ' &&
                getInstructCicero[20]==',' && 
                getInstructCicero[21]==' ')  //Set positive time step for the DRG
                {
                  if(getInstructCicero[19]=='A')
                  {
                    freqRampPosTime_A = ATOL(22);
                    freqStepFTW = (multPLL_A*freqRampPosTime_A*(sourceFreq_A/1000000))/(4*1000);
                    reg_0D_A[3] = (freqStepFTW >> 8) & 0b11111111;
                    reg_0D_A[4] = freqStepFTW & 0b11111111;
                    UpdateRegister_A(reg_0D_A, 4);
                  }
                  else if(getInstructCicero[19]=='B')
                  {
                    freqRampPosTime_B = ATOL(22);
                    freqStepFTW = (multPLL_B*freqRampPosTime_B*(sourceFreq_B/1000000))/(4*1000);
                    reg_0D_B[3] = (freqStepFTW >> 8) & 0b11111111;
                    reg_0D_B[4] = freqStepFTW & 0b11111111;
                    UpdateRegister_B(reg_0D_B, 4);
                  }
                  else if(getInstructCicero[19]=='C')
                  {
                    freqRampPosTime_C = ATOL(22);
                    freqStepFTW = (multPLL_C*freqRampPosTime_C*(sourceFreq_C/1000000))/(4*1000);
                    reg_0D_C[3] = (freqStepFTW >> 8) & 0b11111111;
                    reg_0D_C[4] = freqStepFTW & 0b11111111;
                    UpdateRegister_C(reg_0D_C, 4);
                  }
                }
              else if(getInstructCicero[14]=='s' && 
                getInstructCicero[15]=='t' && 
                getInstructCicero[16]=='e' && 
                getInstructCicero[17]=='p' && 
                getInstructCicero[18]==' ' &&
                getInstructCicero[20]==',' &&
                getInstructCicero[21]==' ')  //Set positive frequency step for the DRG
                {
                  if(getInstructCicero[19]=='A')
                  {
                    freqRampPosStep_A = ATOL(22);
                      freqLimitFTW = CalculateFTW(freqRampPosStep_A, (multPLL_A*sourceFreq_A));
                      reg_0C_A[5] = (freqLimitFTW >> 24) & 0b11111111;
                      reg_0C_A[6] = (freqLimitFTW >> 16) & 0b11111111;
                      reg_0C_A[7] = (freqLimitFTW >> 8) & 0b11111111;
                      reg_0C_A[8] = freqLimitFTW & 0b11111111;
                    UpdateRegister_A(reg_0C_A, 8);
                  }
                  else if(getInstructCicero[19]=='B')
                  {
                    freqRampPosStep_B = ATOL(22);
                      freqLimitFTW = CalculateFTW(freqRampPosStep_B, (multPLL_B*sourceFreq_B));
                      reg_0C_B[5] = (freqLimitFTW >> 24) & 0b11111111;
                      reg_0C_B[6] = (freqLimitFTW >> 16) & 0b11111111;
                      reg_0C_B[7] = (freqLimitFTW >> 8) & 0b11111111;
                      reg_0C_B[8] = freqLimitFTW & 0b11111111;
                    UpdateRegister_B(reg_0C_B, 8);
                  }
                  else if(getInstructCicero[19]=='C')
                  {
                    freqRampPosStep_C = ATOL(22);
                      freqLimitFTW = CalculateFTW(freqRampPosStep_C, (multPLL_C*sourceFreq_C));
                      reg_0C_C[5] = (freqLimitFTW >> 24) & 0b11111111;
                      reg_0C_C[6] = (freqLimitFTW >> 16) & 0b11111111;
                      reg_0C_C[7] = (freqLimitFTW >> 8) & 0b11111111;
                      reg_0C_C[8] = freqLimitFTW & 0b11111111;
                    UpdateRegister_C(reg_0C_C, 8);
                  }
                }
            } //End 'freq ramp pos' code block
          
          else if(getInstructCicero[10]=='n' && 
            getInstructCicero[11]=='e' && 
            getInstructCicero[12]=='g' && 
            getInstructCicero[13]==' '){
              if(getInstructCicero[14]=='t' && 
                getInstructCicero[15]=='i' && 
                getInstructCicero[16]=='m' && 
                getInstructCicero[17]=='e' && 
                getInstructCicero[18]==' ' &&
                getInstructCicero[20]==',' && 
                getInstructCicero[21]==' ')  //Set negative time step for the DRG
                {
                  if(getInstructCicero[19]=='A')
                  {
                    freqRampNegTime_A = ATOL(22);
                    freqStepFTW = (multPLL_A*freqRampNegTime_A*(sourceFreq_A/1000000))/(4*1000);
                    reg_0D_A[1] = (freqStepFTW >> 8) & 0b11111111;
                    reg_0D_A[2] = freqStepFTW & 0b11111111;
                    UpdateRegister_A(reg_0D_A, 4);
                  }
                  else if(getInstructCicero[19]=='B')
                  {
                    freqRampNegTime_B = ATOL(22);
                    freqStepFTW = (multPLL_B*freqRampNegTime_B*(sourceFreq_B/1000000))/(4*1000);
                    reg_0D_B[1] = (freqStepFTW >> 8) & 0b11111111;
                    reg_0D_B[2] = freqStepFTW & 0b11111111;
                    UpdateRegister_B(reg_0D_B, 4);
                  }
                  else if(getInstructCicero[19]=='C')
                  {
                    freqRampNegTime_C = ATOL(22);
                    freqStepFTW = (multPLL_C*freqRampNegTime_C*(sourceFreq_C/1000000))/(4*1000);
                    reg_0D_C[1] = (freqStepFTW >> 8) & 0b11111111;
                    reg_0D_C[2] = freqStepFTW & 0b11111111;
                    UpdateRegister_C(reg_0D_C, 4);
                  }
                }
              else if(getInstructCicero[14]=='s' && 
                getInstructCicero[15]=='t' && 
                getInstructCicero[16]=='e' && 
                getInstructCicero[17]=='p' && 
                getInstructCicero[18]==' ' &&
                getInstructCicero[20]==',' &&
                getInstructCicero[21]==' ')  //Set negative frequency step for the DRG
                {
                  if(getInstructCicero[19]=='A')
                  {
                    freqRampNegStep_A = ATOL(22);
                      freqLimitFTW = CalculateFTW(freqRampNegStep_A, (multPLL_A*sourceFreq_A));
                      reg_0C_A[1] = (freqLimitFTW >> 24) & 0b11111111;
                      reg_0C_A[2] = (freqLimitFTW >> 16) & 0b11111111;
                      reg_0C_A[3] = (freqLimitFTW >> 8) & 0b11111111;
                      reg_0C_A[4] = freqLimitFTW & 0b11111111;
                    UpdateRegister_A(reg_0C_A, 8);
                  }
                  else if(getInstructCicero[19]=='B')
                  {
                    freqRampNegStep_B = ATOL(22);
                      freqLimitFTW = CalculateFTW(freqRampNegStep_B, (multPLL_B*sourceFreq_B));
                      reg_0C_B[1] = (freqLimitFTW >> 24) & 0b11111111;
                      reg_0C_B[2] = (freqLimitFTW >> 16) & 0b11111111;
                      reg_0C_B[3] = (freqLimitFTW >> 8) & 0b11111111;
                      reg_0C_B[4] = freqLimitFTW & 0b11111111;
                    UpdateRegister_B(reg_0C_B, 8);
                  }
                  else if(getInstructCicero[19]=='C')
                  {
                    freqRampNegStep_C = ATOL(22);
                      freqLimitFTW = CalculateFTW(freqRampNegStep_C, (multPLL_C*sourceFreq_C));
                      reg_0C_C[1] = (freqLimitFTW >> 24) & 0b11111111;
                      reg_0C_C[2] = (freqLimitFTW >> 16) & 0b11111111;
                      reg_0C_C[3] = (freqLimitFTW >> 8) & 0b11111111;
                      reg_0C_C[4] = freqLimitFTW & 0b11111111;
                    UpdateRegister_C(reg_0C_C, 8);
                  }
                }
            }  //End 'freq ramp neg' code block

        }  //End 'freq ramp' code block
      
/////////////////////////////////////////////////////////////////////////////////////////////
      
      else if(getInstructCicero[5]=='r' &&  //sets AB (not C) bidirectional ramp parameters
        getInstructCicero[6]=='a' && 
        getInstructCicero[7]=='m' && 
        getInstructCicero[8]=='p' && 
        getInstructCicero[9]=='s' && 
        getInstructCicero[10]==' '){
              if(getInstructCicero[11]=='t' && 
                getInstructCicero[12]=='i' && 
                getInstructCicero[13]=='m' && 
                getInstructCicero[14]=='e' && 
                getInstructCicero[15]==' ')  //Set universal time step for the DRG
                {
                    if(getInstructCicero[16]=='C'){
                      //C, pos and neg
                      freqRampPosTime_C = ATOL(19);
                      freqStepFTW = (multPLL_C*freqRampPosTime_C*(sourceFreq_C/1000000))/(4*1000);
                      reg_0D_C[3] = (freqStepFTW >> 8) & 0b11111111;
                      reg_0D_C[4] = freqStepFTW & 0b11111111;
                      freqRampNegTime_C = ATOL(19);
                      freqStepFTW = (multPLL_C*freqRampNegTime_C*(sourceFreq_C/1000000))/(4*1000);
                      reg_0D_C[1] = (freqStepFTW >> 8) & 0b11111111;
                      reg_0D_C[2] = freqStepFTW & 0b11111111;
                      UpdateRegister_C(reg_0D_C, 4);
                    }
                    else{
                      //A, pos and neg
                      freqRampPosTime_A = ATOL(16);
                      freqStepFTW = (multPLL_A*freqRampPosTime_A*(sourceFreq_A/1000000))/(4*1000);
                      reg_0D_A[3] = (freqStepFTW >> 8) & 0b11111111;
                      reg_0D_A[4] = freqStepFTW & 0b11111111;
                      freqRampNegTime_A = ATOL(16);
                      freqStepFTW = (multPLL_A*freqRampNegTime_A*(sourceFreq_A/1000000))/(4*1000);
                      reg_0D_A[1] = (freqStepFTW >> 8) & 0b11111111;
                      reg_0D_A[2] = freqStepFTW & 0b11111111;
                      UpdateRegister_A(reg_0D_A, 4);
                      //B, pos and neg
                      freqRampPosTime_B = ATOL(16);
                      freqStepFTW = (multPLL_B*freqRampPosTime_B*(sourceFreq_B/1000000))/(4*1000);
                      reg_0D_B[3] = (freqStepFTW >> 8) & 0b11111111;
                      reg_0D_B[4] = freqStepFTW & 0b11111111;
                      freqRampNegTime_B = ATOL(16);
                      freqStepFTW = (multPLL_B*freqRampNegTime_B*(sourceFreq_B/1000000))/(4*1000);
                      reg_0D_B[1] = (freqStepFTW >> 8) & 0b11111111;
                      reg_0D_B[2] = freqStepFTW & 0b11111111;
                      UpdateRegister_B(reg_0D_B, 4);
                    }
                }
              else if(getInstructCicero[11]=='s' && 
                getInstructCicero[12]=='t' && 
                getInstructCicero[13]=='e' && 
                getInstructCicero[14]=='p' && 
                getInstructCicero[15]==' ')  //Set universal frequency step for the DRG
                {
                    if(getInstructCicero[16]=='C'){
                      //C, pos and neg
                      freqRampPosStep_C = ATOL(19);
                        freqLimitFTW = CalculateFTW(freqRampPosStep_C, (multPLL_C*sourceFreq_C));
                        reg_0C_C[5] = (freqLimitFTW >> 24) & 0b11111111;
                        reg_0C_C[6] = (freqLimitFTW >> 16) & 0b11111111;
                        reg_0C_C[7] = (freqLimitFTW >> 8) & 0b11111111;
                        reg_0C_C[8] = freqLimitFTW & 0b11111111;
                      freqRampNegStep_C = ATOL(19);
                        freqLimitFTW = CalculateFTW(freqRampNegStep_C, (multPLL_C*sourceFreq_C));
                        reg_0C_C[1] = (freqLimitFTW >> 24) & 0b11111111;
                        reg_0C_C[2] = (freqLimitFTW >> 16) & 0b11111111;
                        reg_0C_C[3] = (freqLimitFTW >> 8) & 0b11111111;
                        reg_0C_C[4] = freqLimitFTW & 0b11111111;
                      UpdateRegister_C(reg_0C_C, 8);
                    }
                    else{
                      //A, pos and neg
                      freqRampPosStep_A = ATOL(16);
                        freqLimitFTW = CalculateFTW(freqRampPosStep_A, (multPLL_A*sourceFreq_A));
                        reg_0C_A[5] = (freqLimitFTW >> 24) & 0b11111111;
                        reg_0C_A[6] = (freqLimitFTW >> 16) & 0b11111111;
                        reg_0C_A[7] = (freqLimitFTW >> 8) & 0b11111111;
                        reg_0C_A[8] = freqLimitFTW & 0b11111111;
                      freqRampNegStep_A = ATOL(16);
                        freqLimitFTW = CalculateFTW(freqRampNegStep_A, (multPLL_A*sourceFreq_A));
                        reg_0C_A[1] = (freqLimitFTW >> 24) & 0b11111111;
                        reg_0C_A[2] = (freqLimitFTW >> 16) & 0b11111111;
                        reg_0C_A[3] = (freqLimitFTW >> 8) & 0b11111111;
                        reg_0C_A[4] = freqLimitFTW & 0b11111111;
                      UpdateRegister_A(reg_0C_A, 8);
                      //B, pos and neg
                      freqRampPosStep_B = ATOL(16);
                        freqLimitFTW = CalculateFTW(freqRampPosStep_B, (multPLL_B*sourceFreq_B));
                        reg_0C_B[5] = (freqLimitFTW >> 24) & 0b11111111;
                        reg_0C_B[6] = (freqLimitFTW >> 16) & 0b11111111;
                        reg_0C_B[7] = (freqLimitFTW >> 8) & 0b11111111;
                        reg_0C_B[8] = freqLimitFTW & 0b11111111;
                      freqRampNegStep_B = ATOL(16);
                        freqLimitFTW = CalculateFTW(freqRampNegStep_B, (multPLL_B*sourceFreq_B));
                        reg_0C_B[1] = (freqLimitFTW >> 24) & 0b11111111;
                        reg_0C_B[2] = (freqLimitFTW >> 16) & 0b11111111;
                        reg_0C_B[3] = (freqLimitFTW >> 8) & 0b11111111;
                        reg_0C_B[4] = freqLimitFTW & 0b11111111;
                      UpdateRegister_B(reg_0C_B, 8);
                    }
                }
        }  //End 'freq ramps' code block

    }  //End entire 'freq' code block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if((getInstructCicero[0]=='O' || getInstructCicero[0]=='o') &&
    (getInstructCicero[1]=='S' || getInstructCicero[1]=='s') && 
    (getInstructCicero[2]=='K' || getInstructCicero[2]=='k') && 
    getInstructCicero[3]==' ' &&
    (getInstructCicero[4]=='A' || getInstructCicero[4]=='B' || getInstructCicero[4]=='C') && 
    getInstructCicero[5]==',' &&
    getInstructCicero[6]==' ')  //Enable or disable OSK
    {
      if(getInstructCicero[4]=='A')
      {
        if(getInstructCicero[7] == '1'){
            reg_00_A[2] = reg_00_A[2] | 0b10000000;
            reg_00_A[3] = (reg_00_A[3] | 0b00000010) & 0b11111110;
          }
        else if(getInstructCicero[7] == '0'){
            reg_00_A[2] = reg_00_A[2] & 0b01111111;
            reg_00_A[3] = reg_00_A[3] & 0b11111100;
          }
        UpdateRegister_A(reg_00_A, 4);
      }
      
      else if(getInstructCicero[4]=='B')
      {
        if(getInstructCicero[7] == '1'){
            reg_00_B[2] = reg_00_B[2] | 0b10000000;
            reg_00_B[3] = (reg_00_B[3] | 0b00000010) & 0b11111110;
          }
        else if(getInstructCicero[7] == '0'){
            reg_00_B[2] = reg_00_B[2] & 0b01111111;
            reg_00_B[3] = reg_00_B[3] & 0b11111100;
          }
        UpdateRegister_B(reg_00_B, 4);
      }
      else if(getInstructCicero[4]=='C')
      {
        if(getInstructCicero[7] == '1'){
            reg_00_C[2] = reg_00_C[2] | 0b10000000;
            reg_00_C[3] = (reg_00_C[3] | 0b00000010) & 0b11111110;
          }
        else if(getInstructCicero[7] == '0'){
            reg_00_C[2] = reg_00_C[2] & 0b01111111;
            reg_00_C[3] = reg_00_C[3] & 0b11111100;
          }
        UpdateRegister_C(reg_00_C, 4);
      }
    
    } //End OSK code block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if(getInstructCicero[0]=='a' &&
    getInstructCicero[1]=='m' && 
    getInstructCicero[2]=='p' && 
    getInstructCicero[3]==' ' && 
    (getInstructCicero[4]=='A' || getInstructCicero[4]=='B' || getInstructCicero[4]=='C') && 
    getInstructCicero[5]==',' &&
    getInstructCicero[6]==' ')  //Set amplitude for DDS (or ramp specs)
    {
      amplitude = ATOL(7) << 2;
      if((getInstructCicero[7]=='M' || getInstructCicero[7]=='m') &&
        (getInstructCicero[8]=='A' || getInstructCicero[8]=='a') &&
        (getInstructCicero[9]=='X' || getInstructCicero[9]=='x'))
        {amplitude = 16380 << 2;} //Max apmlitude, 14-bits high
      
      if(getInstructCicero[4] == 'A')
        {
          reg_09_A[3] = (amplitude >> 8) & 0b11111111;
          reg_09_A[4] = (reg_09_A[4] & 0b00000011) | (amplitude & 0b11111100);
          UpdateRegister_A(reg_09_A, 4);
        }
      else if(getInstructCicero[4] == 'B')
        {
          reg_09_B[3] = (amplitude >> 8) & 0b11111111;
          reg_09_B[4] = (reg_09_B[4] & 0b00000011) | (amplitude & 0b11111100);
          UpdateRegister_B(reg_09_B, 4);
        }
      else if(getInstructCicero[4] == 'C')
        {
          reg_09_C[3] = (amplitude >> 8) & 0b11111111;
          reg_09_C[4] = (reg_09_C[4] & 0b00000011) | (amplitude & 0b11111100);
          UpdateRegister_C(reg_09_C, 4);
        }
      
    } //End amp code block
    
/////////////////////////////////////////////////////////////////////////////////////////////

  else if((getInstructCicero[0]=='L' || getInstructCicero[0]=='l') &&
    (getInstructCicero[1]=='R' || getInstructCicero[1]=='r') && 
    (getInstructCicero[2]=='R' || getInstructCicero[2]=='r') && 
    getInstructCicero[3]==' ' &&
    (getInstructCicero[4]=='A' || getInstructCicero[4]=='B' || getInstructCicero[4]=='C') && 
    getInstructCicero[5]==',' &&
    getInstructCicero[6]==' ')  //Enable or disable loading LRR @ IO Update (for DRG)
    {
      if(getInstructCicero[4]=='A')
      {
        if(getInstructCicero[7] == '1'){
            reg_00_A[3] = reg_00_A[3] | 0b10000000;
          }
        else if(getInstructCicero[7] == '0'){
            reg_00_A[3] = reg_00_A[3] & 0b01111111;
          }
        UpdateRegister_A(reg_00_A, 4);
      }
      
      else if(getInstructCicero[4]=='B')
      {
        if(getInstructCicero[7] == '1'){
            reg_00_B[3] = reg_00_B[3] | 0b10000000;
          }
        else if(getInstructCicero[7] == '0'){
            reg_00_B[3] = reg_00_B[3] & 0b01111111;
          }
        UpdateRegister_B(reg_00_B, 4);
      }
      else if(getInstructCicero[4]=='C')
      {
        if(getInstructCicero[7] == '1'){
            reg_00_C[3] = reg_00_C[3] | 0b10000000;
          }
        else if(getInstructCicero[7] == '0'){
            reg_00_C[3] = reg_00_C[3] & 0b01111111;
          }
        UpdateRegister_C(reg_00_C, 4);
      }
    
    } //End LRR code block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if((getInstructCicero[0]=='R' || getInstructCicero[0]=='r') &&
    getInstructCicero[1]=='e' && 
    getInstructCicero[2]=='s' && 
    getInstructCicero[3]=='e' && 
    getInstructCicero[4]=='t' && 
    getInstructCicero[5]==' ' && 
    getInstructCicero[7]==';')  //Reset to default DDS settings
    {
      if(getInstructCicero[6]=='A')
        {InitializeReset_A();}
      else if(getInstructCicero[6]=='B')
        {InitializeReset_B();}
      else if(getInstructCicero[6]=='C')
        {InitializeReset_C();}
    }
  
  
  else{
    BlinkLED(1);  //Error. LED blinks 1 times
  }
  
  //Loop clears getInstructCicero[]
  ClearGIC();
}

