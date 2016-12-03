/************************* Communication with arduino *********************************/
void clear_serial() {
  while (Serial.available()) { // clears the serial buffer if wrong command was called
    Serial.read();
  }
}
void halt_serial() {
  while (!Serial.available()) { // will wait intill there will be bytes in the buffer
  }
}

float get_power() {
  clear_serial();
  Serial.println(F("1,0;")); //request for power
  delay(1);
  char charArr[8] = {0};
  int i = 0;
  while (Serial.available()) {
    charArr[i] = Serial.read(); // read all the bytes in serial buffer
    i++;
  }
  return atof(charArr);
}

void ena_sockets(boolean state) {
  if (state) {
    Serial.println(F("2,1;")); // enable the socket
  }
  else {
    Serial.println(F("2,0;")); //disable the socket
  }
}
