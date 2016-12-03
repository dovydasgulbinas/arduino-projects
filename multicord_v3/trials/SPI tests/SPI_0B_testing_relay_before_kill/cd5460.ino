
void sync_buffers() {
  SPI.beginTransaction(SPIset);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFE);
  SPI.endTransaction();
}

void start_conversions() {
  SPI.beginTransaction(SPIset);
  SPI.transfer(0xE8);
  SPI.endTransaction();
}

long get_voltage() {
  byte MSB = 0xFF, MMB = 0xFF, LSB = 0xFF; // if no transfer occurs we will 2^24 - 1 out 16,777,215
  SPI.beginTransaction(SPIset);
  SPI.transfer(0x08);
  MSB = SPI.transfer(0xFF);
  MMB = SPI.transfer(0xFF);
  LSB = SPI.transfer(0xFE);
  SPI.endTransaction();
  return (MSB << 16) + (MMB << 8) + LSB;
}
