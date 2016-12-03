// Code is degined to work on arduino pro mini
// 1:1.11|2.22|3.33|4.44|
void handShakeLd(char HET = '#', char HAT = '$', long SLP = 350);
float parseFloats(char terminator = '|');
int parseInts(char terminator = ':');

boolean newData = false;
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  handShakeLd();
  Serial.println("LEADING!");//-----
}

// the loop routine runs over and over again forever:
void loop() {
 delay(150); //// DELAY FUCKING FIXES EVERYTHING!

  while (!Serial.available()) { ////Will read code below only if there is some data in serial buffer and if new data flag is false
  }
  char serialData = parseInts(); // dataRed is suitable for processing
  if (newData) {
    if (serialData == 1) {
      Serial.println("@1");//-------------
      float t = parseFloats();
      float h = parseFloats();
      float  l = parseFloats();
      float m = parseFloats();
      /*
            Serial.print(t);
            Serial.print('\n');

            Serial.print(h);
            Serial.print('\n');

            Serial.print(l);
            Serial.print('\n');

            Serial.print(m);
            Serial.print('\n');
      */

      //  pushToTS(server, tsPort, tsKey, t, h, l, m);//+++++++
      newData = false;
      digitalWrite(2, HIGH);
    }//if of switch###############################################################################
else{
  clearSerial();
  Serial.println("NO CMD!");
  newData =false;
}
  }
}


/*
void serialEvent() {
  while (Serial.available()) {


  }//while Serial available
}//Serial event
*/

void clearSerial() {
  while (Serial.available()) { // clears the serial buffer if wrong command was called
    Serial.read();
  }
}
void haltSerial() {
  while (!Serial.available()) { // will wait intill there will be bytes in the buffer
  }
}

void handShakeLd(char HET, char HAT, long SLP) { // leader handshake function, SLP send listen period in milliseconds
  clearSerial();
  long sendListenPeriod;
label:
  while (true) {
    Serial.write(HET);
    Serial.flush();
    sendListenPeriod = millis();
    while ((millis() - sendListenPeriod) < SLP) { //Will listen for a fixed amout of time
      // Serial.println(millis() - sendListenPeriod);  //----
    }
    if (Serial.available()) { //its either time out or new data is in the serial buffer
      if (Serial.read() == HAT) {
        // Serial.println("GotaHat!"); //----
        clearSerial();
        break;
      }//if2
      else {
        clearSerial();
        //Serial.println("CrappyInput!"); //----
        //Serial.write(HET);//----
        //Serial.flush();//----
        goto label;
      }//else2

    }//If1
    else {
      //sendListenPeriod = millis(); // prepares for the new timeOut
      // Serial.println("TimedOut!"); //----
      // Serial.write(HET);//----
      //Serial.flush(); //----
      goto label;
    }//else
  } //while
}



float parseFloats(char terminator) //If more data is sent to the buffer code wont work//Will return a char array of bytes from serial port //x param+1
{
  int buff = Serial.available();//*****
  if (buff > 1 ) { // serial port must have at least two symbols in the buffer something related to interrupt triggring
    char serialIn;
    char charArr[64] = "";
    int inc = 64;
    if (buff < 64)
      inc = buff;
    char nextChar; //gets the first char from serial buffer
    for (int i = 0; i < inc; i++) {
      nextChar = Serial.peek();
      if (nextChar != terminator) {
        charArr[i] = Serial.read();
      }
      else { //remove bad symbols
        Serial.read(); //remove terminator
        break; // parses just the firstword matching the query
      }
    }
    Serial.println(charArr);
    Serial.println(atof(charArr));

    newData = true;
    return atof(charArr);
  }
  else
    newData = false;
  return -1;
}


int parseInts(char terminator) //If more data is sent to the buffer code wont work//Will return a char array of bytes from serial port //x param+1
{
  int buff = Serial.available();
  if (buff > 1 ) { // serial port must have at least two symbols in the buffer something related to interrupt triggring
    char serialIn;
    char charArr[64] = "";
    int inc = 64;
    if (buff < 64)
      inc = buff;
    char nextChar; //gets the first char from serial buffer
    for (int i = 0; i < inc; i++) {
      nextChar = Serial.peek();
      if (nextChar != terminator) {
        charArr[i] = Serial.read();
      }
      else { //remove bad symbols
        Serial.read(); //remove terminator
        break; // parses just the firstword matching the query
      }
    }
  //  Serial.println(charArr);

    newData = true;
    return atoi(charArr);
  }
  else
    newData = false;
  return -1;
}
