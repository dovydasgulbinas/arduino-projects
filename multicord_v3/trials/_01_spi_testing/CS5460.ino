

void hand_shake(){ //initializes a handshake routine with cs5460
  
SPI.beginTransaction(SPISettings(130000, MSBFIRST, SPI_MODE0));



SPI.endTransaction();
}


