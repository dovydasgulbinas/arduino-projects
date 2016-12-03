/*
 * REFERENCES
 * http://blog.simtronyx.de/en/measurement-of-illuminance-with-a-bh1750fvi-breakout-board-gy-30-and-an-arduino-uno/
 * Adafruit sckecth example
 */

#include <Wire.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

float h, t; //humidity and temperature variables

int BH1750_address = 0x23; // i2c address of luxMeter
byte buff[2];

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  BH1750_Init(BH1750_address); //set units
  
 float tlux = -1;
  if (BH1750_Read(BH1750_address) == 2) {
    tlux = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  
  delay(200);
  dht.begin();
  delay(200);
  float hum = dht.readHumidity(); // initial temperature & humidity tests for  verifiyng that the data is being received
  float temp = dht.readTemperature();

if ((isnan(temp) || isnan(hum))&&(tlux!=-1)) {
    Serial.println("Failed to read from one of the sensors");
}
}



void loop() {
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C \t");
  Serial.print("Illuminance: ");
  Serial.print((int)outputLux(BH1750_address), DEC);
  Serial.print(" lx\n");

}
void BH1750_Init(int address) {

  Wire.beginTransmission(address);
  Wire.write(0x10); // 1 [lux] set units to lux
  Wire.endTransmission();
}

byte BH1750_Read(int address) {

  byte i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available()) {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
}

float outputLux(int address) {
  float lux;
  if (BH1750_Read(address) == 2) {
    lux = ((buff[0] << 8) | buff[1]) / 1.2;

    return lux;
  }
  else return lux;
}


