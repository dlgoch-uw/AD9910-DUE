/*
  List of functions (Table of contents)
   ------------------------------------
   unsigned long ATOL(int startNum)
   unsigned long Power(int base, int exponent)
   unsigned long CalculateFTW(unsigned long tempFreq, unsigned long sysclk)
   void UpdateRegister_A(byte reg_X[], byte numBytes)
   void UpdateRegister_B(byte reg_X[], byte numBytes)
   void UpdateRegister_C(byte reg_X[], byte numBytes)
*/

unsigned long ATOL(int startNum)  //Modified version of atol(). Returns unsigned long integer from only part of a character array
{
  char bufferAtol[75];
  for(int i=0; i<75; i++){  //Loop clears buffer
    bufferAtol[i]='\0';
  }
  for(int i=startNum; i<75; i++){  //Loop moves supposed number characters into buffer
    if(getInstructCicero[i]==';')
      break;
    bufferAtol[i-startNum]=getInstructCicero[i];
  }
  return atol(bufferAtol);  //Numeric characters in buffer change to unsigned long integer
}


/*
unsigned long Power(int base, int exponent)  //Simple function for evaluating exponents
{
  unsigned long tempVar = base;
  for(int i = 1; i < exponent; i++){
    tempVar = tempVar * base;
  }
  return tempVar;
}
*/


unsigned long CalculateFTW(unsigned long tempFreq, unsigned long sysclk)  //Function to calculate the FTW (also used for DRG freq step)
{
  int binPlace[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //Defines binary decimals (binimals?)
  unsigned long tempVar = 0xFFFFFFFF;  //This is (2^32 - 1), should be (2^32). Will add back 1 later.
  unsigned long tempVar2 = tempFreq;
  int moduloCounter = 0;  //Keeps track of halves lost, during Multiplication loop.
  
  if(tempFreq > sysclk && sysclk != 0){  //Checks to make sure chosen output frequency isn't too large
    return 0;
  }
  
  while(true)
  {
    if(tempVar >= sysclk)  //Division. Component left of decimal point
    {
      binPlace[0]++;
      tempVar -= sysclk;
    }
    else{
      break;
    }
  }
  tempVar += 1;  //Remainder. Adding back the 1 here
  for(int i=1; i<(sizeof(binPlace)/sizeof(int)); i++)
  {
    tempVar *= 2;
    if(tempVar >= sysclk)  //Division. Components right of decimal point
    {
      binPlace[i] = 1;
      tempVar -= sysclk;
    }
    else{
      binPlace[i] = 0;
    }
  }
  
  tempVar = 0;  //Reset tempVar for new use as FTW tracker
  for(int i=0; i<(sizeof(binPlace)/sizeof(int)); i++)
  {
    tempVar += (tempVar2 * binPlace[i]);  //Multiplication, for each binary decimal place
    moduloCounter = moduloCounter + (tempVar2 % 2);
    tempVar2 = tempVar2 / 2;
  }
  tempVar += moduloCounter / 2;
  
  return tempVar;  //Returns tempVar to define the calculated FTW
}



void UpdateRegister_A(byte reg_X[], int numBytes)
{
  delayMicroseconds(100); //1000
  digitalWrite(kPinCSB_A, LOW);
  delayMicroseconds(100); //1000
  
  for(int i=0; i<numBytes; i++)
  {
    SPI.transfer(reg_X[i], SPI_CONTINUE);
    
    if((i+1) % 4 == 0)
      {delayMicroseconds(50);} //50
    else
      {delayMicroseconds(15);} //15
  }
  SPI.transfer(reg_X[numBytes]);
  
  delayMicroseconds(100); //1000
  digitalWrite(kPinCSB_A, HIGH);
  delayMicroseconds(100); //1000
  
  digitalWrite(kPinIO_UPDATE_A, HIGH);
  delayMicroseconds(40); //50
  digitalWrite(kPinIO_UPDATE_A, LOW);
  delayMicroseconds(40); //50
}


void UpdateRegister_B(byte reg_X[], unsigned int numBytes)
{
  delayMicroseconds(100); //1000
  digitalWrite(kPinCSB_B, LOW);
  delayMicroseconds(100); //1000
  
  for(byte i=0; i<numBytes; i++)
  {
    SPI.transfer(reg_X[i], SPI_CONTINUE);
    
    if((i+1) % 4 == 0)
      {delayMicroseconds(50);} //50
    else
      {delayMicroseconds(15);} //15
  }
  SPI.transfer(reg_X[numBytes]);
  
  delayMicroseconds(100); //1000
  digitalWrite(kPinCSB_B, HIGH);
  delayMicroseconds(100); //1000
  
  digitalWrite(kPinIO_UPDATE_B, HIGH);
  delayMicroseconds(40); //50
  digitalWrite(kPinIO_UPDATE_B, LOW);
  delayMicroseconds(40); //50
}


void UpdateRegister_C(byte reg_X[], unsigned int numBytes)
{
  delayMicroseconds(100); //1000
  digitalWrite(kPinCSB_C, LOW);
  delayMicroseconds(100); //1000
  
  for(byte i=0; i<numBytes; i++)
  {
    SPI.transfer(reg_X[i], SPI_CONTINUE);
    
    if((i+1) % 4 == 0)
      {delayMicroseconds(50);} //50
    else
      {delayMicroseconds(15);} //15
  }
  SPI.transfer(reg_X[numBytes]);
  
  delayMicroseconds(100); //1000
  digitalWrite(kPinCSB_C, HIGH);
  delayMicroseconds(100); //1000
  
  digitalWrite(kPinIO_UPDATE_C, HIGH);
  delayMicroseconds(40); //50
  digitalWrite(kPinIO_UPDATE_C, LOW);
  delayMicroseconds(40); //50
}

