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
