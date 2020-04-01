/*
  Register breakouts for all 24 freq profiles
  
  List of functions (Table of contents)
   ------------------------------------
   void UploadProfile()
*/

void UploadProfile()
{
  //Intermediary/calculated variables used here
  unsigned long varFTW;    //Frequency tuning word
  
  if(getInstructCicero[5]=='A')
  {
    if(getInstructCicero[6]=='0'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_0E_A[5] = (varFTW >> 24) & 0b11111111;
        reg_0E_A[6] = (varFTW >> 16) & 0b11111111;
        reg_0E_A[7] = (varFTW >> 8) & 0b11111111;
        reg_0E_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_0E_A, 8);
    }
    else if(getInstructCicero[6]=='1'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_0F_A[5] = (varFTW >> 24) & 0b11111111;
        reg_0F_A[6] = (varFTW >> 16) & 0b11111111;
        reg_0F_A[7] = (varFTW >> 8) & 0b11111111;
        reg_0F_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_0F_A, 8);
    }
    else if(getInstructCicero[6]=='2'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_10_A[5] = (varFTW >> 24) & 0b11111111;
        reg_10_A[6] = (varFTW >> 16) & 0b11111111;
        reg_10_A[7] = (varFTW >> 8) & 0b11111111;
        reg_10_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_10_A, 8);
    }
    else if(getInstructCicero[6]=='3'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_11_A[5] = (varFTW >> 24) & 0b11111111;
        reg_11_A[6] = (varFTW >> 16) & 0b11111111;
        reg_11_A[7] = (varFTW >> 8) & 0b11111111;
        reg_11_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_11_A, 8);
    }
    else if(getInstructCicero[6]=='4'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_12_A[5] = (varFTW >> 24) & 0b11111111;
        reg_12_A[6] = (varFTW >> 16) & 0b11111111;
        reg_12_A[7] = (varFTW >> 8) & 0b11111111;
        reg_12_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_12_A, 8);
    }
    else if(getInstructCicero[6]=='5'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_13_A[5] = (varFTW >> 24) & 0b11111111;
        reg_13_A[6] = (varFTW >> 16) & 0b11111111;
        reg_13_A[7] = (varFTW >> 8) & 0b11111111;
        reg_13_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_13_A, 8);
    }
    else if(getInstructCicero[6]=='6'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_14_A[5] = (varFTW >> 24) & 0b11111111;
        reg_14_A[6] = (varFTW >> 16) & 0b11111111;
        reg_14_A[7] = (varFTW >> 8) & 0b11111111;
        reg_14_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_14_A, 8);
    }
    else if(getInstructCicero[6]=='7'){
      frequency_A = ATOL(9);
        varFTW = CalculateFTW(frequency_A, (multPLL_A*sourceFreq_A));
        reg_15_A[5] = (varFTW >> 24) & 0b11111111;
        reg_15_A[6] = (varFTW >> 16) & 0b11111111;
        reg_15_A[7] = (varFTW >> 8) & 0b11111111;
        reg_15_A[8] = varFTW & 0b11111111;
      UpdateRegister_A(reg_15_A, 8);
    }
  } //End A profile block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if(getInstructCicero[5]=='B')
  {
    if(getInstructCicero[6]=='0'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_0E_B[5] = (varFTW >> 24) & 0b11111111;
        reg_0E_B[6] = (varFTW >> 16) & 0b11111111;
        reg_0E_B[7] = (varFTW >> 8) & 0b11111111;
        reg_0E_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_0E_B, 8);
    }
    else if(getInstructCicero[6]=='1'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_0F_B[5] = (varFTW >> 24) & 0b11111111;
        reg_0F_B[6] = (varFTW >> 16) & 0b11111111;
        reg_0F_B[7] = (varFTW >> 8) & 0b11111111;
        reg_0F_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_0F_B, 8);
    }
    else if(getInstructCicero[6]=='2'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_10_B[5] = (varFTW >> 24) & 0b11111111;
        reg_10_B[6] = (varFTW >> 16) & 0b11111111;
        reg_10_B[7] = (varFTW >> 8) & 0b11111111;
        reg_10_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_10_B, 8);
    }
    else if(getInstructCicero[6]=='3'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_11_B[5] = (varFTW >> 24) & 0b11111111;
        reg_11_B[6] = (varFTW >> 16) & 0b11111111;
        reg_11_B[7] = (varFTW >> 8) & 0b11111111;
        reg_11_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_11_B, 8);
    }
    else if(getInstructCicero[6]=='4'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_12_B[5] = (varFTW >> 24) & 0b11111111;
        reg_12_B[6] = (varFTW >> 16) & 0b11111111;
        reg_12_B[7] = (varFTW >> 8) & 0b11111111;
        reg_12_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_12_B, 8);
    }
    else if(getInstructCicero[6]=='5'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_13_B[5] = (varFTW >> 24) & 0b11111111;
        reg_13_B[6] = (varFTW >> 16) & 0b11111111;
        reg_13_B[7] = (varFTW >> 8) & 0b11111111;
        reg_13_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_13_B, 8);
    }
    else if(getInstructCicero[6]=='6'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_14_B[5] = (varFTW >> 24) & 0b11111111;
        reg_14_B[6] = (varFTW >> 16) & 0b11111111;
        reg_14_B[7] = (varFTW >> 8) & 0b11111111;
        reg_14_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_14_B, 8);
    }
    else if(getInstructCicero[6]=='7'){
      frequency_B = ATOL(9);
        varFTW = CalculateFTW(frequency_B, (multPLL_B*sourceFreq_B));
        reg_15_B[5] = (varFTW >> 24) & 0b11111111;
        reg_15_B[6] = (varFTW >> 16) & 0b11111111;
        reg_15_B[7] = (varFTW >> 8) & 0b11111111;
        reg_15_B[8] = varFTW & 0b11111111;
      UpdateRegister_B(reg_15_B, 8);
    }
  } //End B profile block
  
/////////////////////////////////////////////////////////////////////////////////////////////
  
  else if(getInstructCicero[5]=='C')
  {
    if(getInstructCicero[6]=='0'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_0E_C[5] = (varFTW >> 24) & 0b11111111;
        reg_0E_C[6] = (varFTW >> 16) & 0b11111111;
        reg_0E_C[7] = (varFTW >> 8) & 0b11111111;
        reg_0E_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_0E_C, 8);
    }
    else if(getInstructCicero[6]=='1'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_0F_C[5] = (varFTW >> 24) & 0b11111111;
        reg_0F_C[6] = (varFTW >> 16) & 0b11111111;
        reg_0F_C[7] = (varFTW >> 8) & 0b11111111;
        reg_0F_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_0F_C, 8);
    }
    else if(getInstructCicero[6]=='2'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_10_C[5] = (varFTW >> 24) & 0b11111111;
        reg_10_C[6] = (varFTW >> 16) & 0b11111111;
        reg_10_C[7] = (varFTW >> 8) & 0b11111111;
        reg_10_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_10_C, 8);
    }
    else if(getInstructCicero[6]=='3'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_11_C[5] = (varFTW >> 24) & 0b11111111;
        reg_11_C[6] = (varFTW >> 16) & 0b11111111;
        reg_11_C[7] = (varFTW >> 8) & 0b11111111;
        reg_11_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_11_C, 8);
    }
    else if(getInstructCicero[6]=='4'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_12_C[5] = (varFTW >> 24) & 0b11111111;
        reg_12_C[6] = (varFTW >> 16) & 0b11111111;
        reg_12_C[7] = (varFTW >> 8) & 0b11111111;
        reg_12_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_12_C, 8);
    }
    else if(getInstructCicero[6]=='5'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_13_C[5] = (varFTW >> 24) & 0b11111111;
        reg_13_C[6] = (varFTW >> 16) & 0b11111111;
        reg_13_C[7] = (varFTW >> 8) & 0b11111111;
        reg_13_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_13_C, 8);
    }
    else if(getInstructCicero[6]=='6'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_14_C[5] = (varFTW >> 24) & 0b11111111;
        reg_14_C[6] = (varFTW >> 16) & 0b11111111;
        reg_14_C[7] = (varFTW >> 8) & 0b11111111;
        reg_14_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_14_C, 8);
    }
    else if(getInstructCicero[6]=='7'){
      frequency_C = ATOL(9);
        varFTW = CalculateFTW(frequency_C, (multPLL_C*sourceFreq_C));
        reg_15_C[5] = (varFTW >> 24) & 0b11111111;
        reg_15_C[6] = (varFTW >> 16) & 0b11111111;
        reg_15_C[7] = (varFTW >> 8) & 0b11111111;
        reg_15_C[8] = varFTW & 0b11111111;
      UpdateRegister_C(reg_15_C, 8);
    }
  } //End C profile block
}

