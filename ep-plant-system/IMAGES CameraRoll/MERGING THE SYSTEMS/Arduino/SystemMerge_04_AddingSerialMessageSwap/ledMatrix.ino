volatile int tc = 0; //tick Counter!
volatile int bi = 0; //buffer index
volatile char c = 0;

void printMsg() {
  if (ledM.ena > 0) {
    if (ledM.f > 0) {
    }// if flash
    for (int i = 0; i < ledM.ena; i++) {
      Serial.println(ledM.m); //----
      printStringWithShift(ledM.m, ledM.s);
    }
    ledM.ena = 0;
    Serial.println("Message has been sent x r times!"); //----
  }//if enabled
}//printMsg

void printStringWithShift(char* s, int shift_speed)
{
  while (*s != 0)
  {
   // c =*s;
    //printCharWTI();
    printCharWithShift(*s, shift_speed); //++++
    s++;
  }
}

void printCharWithShift(char c, int shift_speed)
{
  if (c < 32) return; // exit function if first 31 characters of ascii table are found
  c -= 32;
  memcpy_P(buffer, CH + (7 * c), 7); // first ROW index CH + (7 * C) second is length index how many collumns you have to take to the buffer!
  m.writeSprite(bSize, 0, buffer); // writes the sprite not in the display
  m.setColumn(bSize + buffer[0], 255); //Adds a gap to the end of the  SPRITE, buffer[0] holds sprites width

  for (int i = 0; i < buffer[0] + 1; i++)
  {
    delay(shift_speed);
    m.shiftLeft(false, false);
  }
}


void printCharWTI() {
  if (ledM.ena > 0) {
   //Serial.println("@ENA!");//----
    if (bi == 0) {
   //  Serial.println("@BI!"); //----
      if (c < 32){ 
        tc = 0; //????
        bi = 0; //????
      return; // exit function if first 31 characters of ascii table are found
      }
      c -= 32;
      memcpy_P(buffer, CH + (7 * c), 7); // first ROW index CH + (7 * C) second is length index how many collumns you have to take to the buffer!
      m.writeSprite(bSize, 0, buffer); // writes the sprite not int the display
      m.setColumn(bSize + buffer[0], 0);
      bi = buffer[0] + 1;
      tc = 0;
    }
    if (tc < ledM.s) { // if tc is less that speed given do nothing!
    //  Serial.println("@tc<"); //----
      tc++;
    }//if2

    else { // fixed amout of time has passed and we can run the code
      
      if (bi > 0) { // will repeat the action untill all data is shifted!
        Serial.println("bi>0");
        
        m.shiftLeft(false, false);
        bi--;
        tc = 0; // tc becomes 0
      }// if bi

    }//else b
  }//iff
  else {
    tc = 0;
    bi = 0;
    return;
  }//else
}//()




