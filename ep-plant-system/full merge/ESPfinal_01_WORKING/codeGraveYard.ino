
void readSerial(char terminator)
{
  static uint8_t  putIndex = 0;

  while (Serial.available())
  { 
    yield();
    stream[putIndex] = (char)Serial.read();
    if ((stream[putIndex] == '\n') || stream[putIndex] == terminator) // end of message character or full buffer
    {
      // put in a message separator and end the string
      stream[putIndex] = '\0';
      // restart the index for next filling spree and flag we have a message waiting
      putIndex = 0;
      newString = true;
    }
    else
      // Just save the next char in next location
      stream[putIndex++];
  }
}

/* 
  int parseInterger(char terminator)
  {
    static uint8_t  putIndex = 0;
    static char * number;
if(Serial.available()){
    while (Serial.available())
    {
      number[putIndex] = (char)Serial.read();
      if (number[putIndex] == terminator) // end of message character or full buffer
      {
        // put in a message separator and end the string
        number[putIndex] = '\0';
        // restart the index for next filling spree and flag we have a message waiting
        putIndex = 0;
        newInt = true;
        return atoi(number); //exit the function return int
      }
      else{
        // Just save the next char in next location
        number[putIndex++];
        //return -1; // ESP cant handle functions that return nothing
      }
    }
    
  }
  else{
    return -1;
  }
  }
*/
