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

void print_int_Sector(char label[], byte secId, int segment[]) {
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
