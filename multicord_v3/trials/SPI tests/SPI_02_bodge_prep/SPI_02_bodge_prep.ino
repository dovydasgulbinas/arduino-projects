//taken from https://github.com/esp8266/Arduino/issues/1429


#include <ESP8266WiFi.h>
#include <SPI.h>             //SPI

SPISettings SPIset(1300000, MSBFIRST, SPI_MODE0);

long _voltage = 0;


void sync_buffers(byte CS);
void start_conversions(byte CS);
long get_voltage(byte CS);
const byte CS = 5;


void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  Serial.begin(115200);
  Serial.println("Hello will initialize SPI in 5s...");
  delay(5000);
  Serial.println("Sending via SPI...");
  SPI.begin();
  // SPI.setHwCs(true);
  //SPI.beginTransaction(SPIset); //130k baud rate
  sync_buffers(CS);
  start_conversions(CS);
}

void loop() {
  _voltage = get_voltage(CS);
  Serial.print("Voltage:");
  Serial.println(_voltage);
  delay (1000);
}



