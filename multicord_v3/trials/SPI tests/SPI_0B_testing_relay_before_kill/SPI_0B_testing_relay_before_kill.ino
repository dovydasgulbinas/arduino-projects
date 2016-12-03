//taken from https://github.com/esp8266/Arduino/issues/1429


#include <ESP8266WiFi.h>
#include <SPI.h>             //SPI

SPISettings SPIset(130000, MSBFIRST, SPI_MODE0);

long _voltage = 0;



void sync_buffers();
void start_conversions();
long get_voltage();

boolean relay_on = false;
const byte RELAY = 4;

void setup() {
  pinMode(RELAY, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello will initialize SPI in 5s...");
  delay(5000);
  Serial.println("Sending via SPI...");
  SPI.begin();
  SPI.setHwCs(true);
  SPI.beginTransaction(SPIset); //130k baud rate
  sync_buffers();
  start_conversions();
}

void loop() {
  _voltage = get_voltage();
  Serial.print("Voltage:");
  Serial.println(_voltage);
  if (relay_on) {
    digitalWrite(RELAY, LOW);
    Serial.println("RELAY: OFF");
  }
  else {
    digitalWrite(RELAY, HIGH);
    Serial.println("RELAY: ON");
  }
  delay (1000);
  relay_on = !relay_on;
}



