//// SERIAL COMMANDS
void clearSerial() {
  while (Serial.available()) { // clears the serial buffer if wrong command was called
    Serial.read();
  }
}
void haltSerial() {
  while (!Serial.available()) { // will wait intill there will be bytes in the buffer
  }
}

void handShakeFl(char HET, char HAT) { // followers handshake function
  while (true) { //
    clearSerial();
    haltSerial();
    if (Serial.read() == HET) {
      clearSerial();
      Serial.write(HAT);
      break;
    }
  }//while
}
