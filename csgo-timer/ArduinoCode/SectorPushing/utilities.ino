void printSector(char label[], byte secId, byte segment[]) {
  Serial.println();
  Serial.print("\t");
  Serial.print(label);
  Serial.println(secId);
  for (byte i = 0; i < 5; i++) {
    Serial.print(segment[secId * 5 + i]);
    Serial.print('\t');
  }
  Serial.println();
}

boolean timed_trigger( long now, long *lastNow, long callPeriod){ // need a pointer variable to get the address of a value
  if ((now - *lastNow) >= callPeriod) { // if scan period is reached
    *lastNow = now;
    return true;
  }
  else{
   return false;
  }
}


