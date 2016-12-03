
void getMessage(char mt, char ct) { //mt message terminaor. ct command terminator
  char peeker;
  haltSerial();
  delay(10);
getMsgPar:

  if (Serial.available()) {
    char param = Serial.read(); // gets the parameter id

    switch (param) {
      case 'e': // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove
        Serial.read(); // remove the space that follows the parameter
        ledM.ena = Serial.parseInt();
        Serial.print("e: "); //----
        Serial.println(ledM.ena);//----
        peeker = Serial.peek();
        if (peeker != ct && (peeker == ' ')) { // means that we have a message!
          Serial.read(); // remove the space!
          byte r = Serial.readBytesUntil(mt, ledM.m, 144); //assuming that no CR EL are sent
          ledM.m[r] = '\0'; // terminates the sequence manually since readytes until cant do that
        }
        else
        {
          Serial.read(); // remove ct
        }
        Serial.println(ledM.m); //----
        goto getMsgPar;
      case 's':
        ledM.s = Serial.parseInt();
        Serial.print("s: ");//----
        Serial.println(ledM.s);//----
        Serial.read(); // remove space
        goto getMsgPar;
      case 'f':
        ledM.f = Serial.parseInt();
        Serial.print("f: ");//----
        Serial.println(ledM.f);//----
        Serial.read(); // remove space
        goto getMsgPar;
      case 'b':
        ledM.b = Serial.parseInt();
        Serial.print("b: ");//----
        Serial.println(ledM.b);//----
        Serial.read(); // remove space
        m.setIntensity(ledM.b); //~~~~~~
        goto getMsgPar;
      case 'g':
        ledM.g = Serial.parseInt(); //2 b 11 g 22 f 33
        Serial.print("g: ");//----
        Serial.println(ledM.g);//----
        Serial.read(); // remove space
        goto getMsgPar;

      default:
        Serial.println("Invalid param"); //---~
        clearSerial();
    }//switch
  }//

} //()

