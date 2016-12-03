/*
 * REFRENCES:
 * FollowerHandShake_02_Distilled // HandshakeMethods
 * integrationWithESP_03_legit //sensor data and processing
 * FollowerHandShake_01_br_CS04 // Switch for changing function parameters // from WEB PROJECTTTTTT
 * REFER TO SS_01 for oblolite not tested code
 * Decided not to use quick time event that are rendered in arduino itself due to not enought knowlede of how changes in data should be transfered to esp. Doing everything on sever side using matlab aanalysisi
   Cutting down the structure removing pavgHum because all of that can be done as effeciently on server side because the only parameter to compare with is on the server already
 */

#include <Wire.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2
#define SH A0
#define LED 3
#define errLED 5


int BH1750_address = 0x23; // i2c address of luxMeter
byte buff[2];

DHT dht(DHTPIN, DHTTYPE);
void handShakeFl(char HET = '#', char HAT = '$');

struct strToEsp {
  int ena ; //enabled. indicator of whether or not function i
  int p; //measurment period. A time mcu waits to take in new measurment in MS
  int n; //number of measurment user wants to take
  int ln; // last number. of measurment taken
  long lc; // last call. last time function was called
  float avgTemp; // average temperature
  float avgHum; // average humidity
  float avgLum; //average luminosity
  float avgMoi;// average moisture
  String cData;// current piece of data aggregated
} streamToEsp{1, 1800, 200, 0, 0, 0.0, 0.0, 0.0, 0.0, ""}; // 200 measurments every 1.8s == total period of 6min

void setup() {

  float h, t; //humidity and temperature variables
  float sh; // soil humidity
  float lum;//luminodity

  pinMode(LED, OUTPUT);
  pinMode(errLED, OUTPUT);
  digitalWrite(errLED, LOW);
  analogWrite(LED, 0);
  Serial.begin(9600);
  pinMode(SH, INPUT);
  Wire.begin();
  delay(200);
  dht.begin();
  BH1750_Init(BH1750_address); //set units
  lum = -1.0;
  if (BH1750_Read(BH1750_address) == 2) {
    lum = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  delay(200);
  h = dht.readHumidity(); // initial temperature & humidity tests for  verifiyng that the data is being received
  t = dht.readTemperature();
  sh = analogRead(SH);

  if ((isnan(t) || isnan(h)) && (lum < 0.0) && sh > 1005) { //-------
    digitalWrite(errLED, HIGH);
  }
  handShakeFl();
  analogWrite(LED, 255);
  delay(300);
}

void loop() {
  processData();
}
void serialEvent() { //1 0 | 1 a 1 | 1 d 100 p 99|
  while (Serial.available()) {
    int in = Serial.parseInt();
    //++??++Serial.readBytesUntil(' ', in, 1); // will read the command untill i // MAKE SURE wether terminator values are removed

    Serial.read(); // remove space
    if (in == 1) { //ParseParameters| Do one of 3 things: enable, disable, edit parameters //Method here is already selected
      haltSerial();

      char enable = Serial.peek(); // will get 1 or 0 or - paramters
      if (enable == '1') {
        streamToEsp.ena = 1; //enable the StreamToESP
        Serial.read(); //remove the command call
      }
      else if (enable == '0') {
        streamToEsp.ena = 0; // //Disable the StreamToESP
        Serial.read(); //remove the command call
      }
      else { // edit  parameters of the method order does not matter can't be called w/ enabling mode
        haltSerial();
        delay(10);
getParam:
        if (Serial.available()) {
          char param = Serial.read(); // gets the parameter id

          switch (param) {
            case 'p': // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove
              streamToEsp.p = Serial.parseInt();
              Serial.read(); // remove space
              goto getParam;
            case 'n':
              streamToEsp.n = Serial.parseInt();
              Serial.read(); // remove space
              goto getParam;
            case 's':
              Serial.print(".ena:\t");
              Serial.println(streamToEsp.ena);
              Serial.print(".p:\t");
              Serial.println(streamToEsp.p);
              Serial.print(".n:\t");
              Serial.println(streamToEsp.n);
              Serial.print(".ln:\t");
              Serial.println(streamToEsp.ln);
              Serial.read(); // remove space
              goto getParam;
            default:// non existent parameter value was used!
              clearSerial();
          }
        }
      }//ELSE cmd 1
    } //parseParams
    else {
      // non existing command number was called
      clearSerial();
    }//else
  }//while Serial available
}//Serial event
///#################SERIAL EVENT END





