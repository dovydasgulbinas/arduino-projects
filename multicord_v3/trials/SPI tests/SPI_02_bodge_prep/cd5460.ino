
void sync_buffers(byte CS) {
  SPI.beginTransaction(SPIset);
  digitalWrite(CS, LOW);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFE);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
}

void start_conversions(byte CS) {
  SPI.beginTransaction(SPIset);
  digitalWrite(CS, LOW);
  SPI.transfer(0xE8);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
}

long get_voltage(byte CS) {
  byte MSB = 0xFF, MMB = 0xFF, LSB = 0xFF; // if no transfer occurs we will 2^24 - 1 out 16,777,215
  SPI.beginTransaction(SPIset);
  digitalWrite(CS, LOW);
  SPI.transfer(0x08);
  MSB = SPI.transfer(0xFF);
  MMB = SPI.transfer(0xFF);
  LSB = SPI.transfer(0xFF);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
  return (MSB << 16) + (MMB << 8) + LSB;
}
