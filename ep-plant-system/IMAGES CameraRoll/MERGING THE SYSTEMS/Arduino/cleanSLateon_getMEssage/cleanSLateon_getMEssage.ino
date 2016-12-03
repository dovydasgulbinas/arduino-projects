struct ledMatrix {
  int ena; // enable the scrolling of text if ena is > 1 that it will display message that many times
  int s; //speed. how fast text is scrolling
  // int r; // repeat. how many times message will be repeated after enablig it; ena will become 0 after r times
  char * m;// message. the message that will be deliverd
  int f; // flash. flash function how many times display will be flashed before sending data

} ledM {5, 100, "Pep_Chilly says: HELLO!", 0};


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


void getMessage(char mt = '`') { //mt message terminaor.
  // haltSerial();
  delay(10);
getMsgPar:
  if (Serial.available()) {
    char param = Serial.read(); // gets the parameter id

    if (Serial.available()) {
      char param = Serial.read(); // gets the parameter id

      switch (param) {
        case 'e': // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove
          ledM.ena = Serial.parseInt();
          Serial.print("e: "); //----
          Serial.println(ledM.ena);//----
          Serial.read(); // remove space
          char peeker = Serial.peek();
          if ((peeker != '\n') && (peeker != '\r') && (peeker != '\0') ) { // means that we have a message!
            Serial.readBytesUntil(mt, ledM.m, 144); //assuming that no CR EL are sent
          }
          goto getMsgPar;
        case 's':
          ledM.s = Serial.parseInt();
          Serial.print("s: ");//----
          Serial.println(ledM.s);//----
          Serial.read(); // remove space
          goto getMsgPar;
        case 'f':
          ledM.n = Serial.parseInt();
          Serial.print("f: ");//----
          Serial.println(ledM.f);//----
          Serial.read(); // remove space
          goto getMsgPar;
        default:
          Serial.println("Invalid param"); //---~
          clearSerial();
      }//switch
    }//
  } //if ava
} //()

