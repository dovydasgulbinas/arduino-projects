const byte rxPin = 3;
const byte txPin = 4;

#include <Wire.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2
#define SH A0

int h, t; //humidity and temperature variables
int sh; // soil humidity
int lum;//luminodity

int BH1750_address = 0x23; // i2c address of luxMeter
byte buff[2];
#include <SoftwareSerial.h>
SoftwareSerial mySerial (rxPin, txPin);
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  char r;
  pinMode(SH, INPUT);
  
//mySerial.begin(19200);
  Serial.begin(19200);
  Wire.begin();
  BH1750_Init(BH1750_address); //set units

  lum = -1;
  if (BH1750_Read(BH1750_address) == 2) {
    lum = ((buff[0] << 8) | buff[1]) / 1.2;
  }

  delay(200);
  dht.begin();
  delay(200);
  h = dht.readHumidity(); // initial temperature & humidity tests for  verifiyng that the data is being received
  t = dht.readTemperature();
  sh = analogRead(SH);

  if ((isnan(t) || isnan(h)) && (lum != -1) && sh > 1005) {
    Serial.println("Failed to read from one of the sensors \n Going to loop{}");
  }

while(true){ //loop until break
 while(Serial.available()) {
  Serial.read();
 }//clear
Serial.println('H');
Serial.flush();
while(!Serial.available()){
} //wait until some data is in the buffer
if(Serial.read()=='G')
break; // exit the loop handShake was initialized!
//delay(250);
}//main while

}



void loop() {
 
  h = (int)dht.readHumidity();
  t = (int)dht.readTemperature();
  lum = (int)outputLux(BH1750_address); //get luminosity
  sh = analogRead(SH);
  Serial.print('1');
  Serial.print(':');
  Serial.print(t);
  Serial.print('|');
  Serial.print(h);
  Serial.print('|');
  Serial.print(lum);
  Serial.print('|');
  Serial.print(sh);
  Serial.println('|');
 delay(3000);
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


