  #include <ESP8266WiFi.h>
  #include <SPI.h>             //SPI

void setup() {
  Serial.begin(115200);

  SPI.begin();
    SPI.setHwCs(true);
    //SPI.setDataMode(SPI_MODE3);
    //SPI.setClockDivider(SPI_CLOCK_DIV128);
    //SPI.setFrequency(1000000);
    //SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));  
    //SPI.setFrequency(1000000);
}

void loop() {
  SPI.transfer(B01010101);
  delay(500);

  SPI.transfer(B10101010);
  delay(500);
}
