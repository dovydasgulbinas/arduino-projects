/*
 * REFRENCES:
 * FollowerHandShake_02_Distilled // HandshakeMethods
 * integrationWithESP_03_legit //sensor data and processing
 * FollowerHandShake_01_br_CS04 // Switch for changing function parameters // from WEB PROJECTTTTTT
 *
 */

#include <Wire.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2
#define SH A0

struct strToEsp {
  int ena ; //enabled. indicator of whether or not function is
  int a; //averaging 0/1. should it be enabled or not
  int p; //measurment period. A time mcu waits to take in new measurment
  int n; //number of measurment user wants to take
  int ln; // last number. of measurment taken
  long lc; // last call. last time function was called
  float avgTemp; // average temperature
  float avgHum; // average humidity
  float avgLum; //average luminosity
  float avgMoi;// average moisture
  String pData;// last piece of data sent
  String cData;// current piece of data aggregated
  int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut

} streamToEsp{0, 1, 2000, 8, 0, 0, 0, 0, 0, 0, "", "", 0, 0};

int h, t; //humidity and temperature variables
int sh; // soil humidity
int lum;//luminodity

int BH1750_address = 0x23; // i2c address of luxMeter
byte buff[2];
DHT dht(DHTPIN, DHTTYPE);

void handShakeFl(char HET = '#', char HAT = '$');



void setup() {
  Serial.begin(19200);
  pinMode(SH, INPUT);
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
  handShakeFl();
  Serial.println("FOLLOWING!"); //----
}

void loop() {
  processData();
  /////
  /*
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
  delay(10000);
  //////
  */
}

void serialEvent() { //1 0 | 1 a 1 | 1 d 100 p 99|
  while (Serial.available()) {
    // Serial.println("AVALABLET!"); //----
    int in = Serial.parseInt();
    //++??++Serial.readBytesUntil(' ', in, 1); // will read the command untill i // MAKE SURE wether terminator values are removed

    //  Serial.print("Igot: ");//----
    // Serial.println(in); //----
    Serial.read(); // remove space
    if (in == 1) { //ParseParameters| Do one of 3 things: enable, disable, edit parameters //Method here is already selected

      haltSerial();

      char enable = Serial.peek(); // will get 1 or 0 or - paramters
      if (enable == '1') {
        streamToEsp.ena = 1; //enable the StreamToESP
        Serial.read(); //remove the command call
        Serial.println("ENABLIOD!");//----
      }
      else if (enable == '0') {
        streamToEsp.ena = 0; // //Disable the StreamToESP
        Serial.read(); //remove the command call
        Serial.println("DISABLIOD!");//----;

      }
      else { // edit  parameters of the method order does not matter can't be called w/ enabling mode
        haltSerial();
        delay(10);
getParam:
        if (Serial.available()) {
          char param = Serial.read(); // gets the parameter id

          switch (param) {
            case 'a': // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove
              streamToEsp.a = Serial.parseInt();
              Serial.print("a: "); //----
              Serial.println(streamToEsp.a);//----
              Serial.read(); // remove space
              goto getParam;
            case 'p':
              streamToEsp.p = Serial.parseInt();
              Serial.print("p: ");//----
              Serial.println(streamToEsp.p);//----
              Serial.read(); // remove space
              goto getParam;
            case 'n':
              streamToEsp.n = Serial.parseInt();
              Serial.print("n: ");//----
              Serial.println(streamToEsp.n);//----
              Serial.read(); // remove space
              goto getParam;
            case 'v':
              streamToEsp.v = Serial.parseInt();
              Serial.print("v: ");//----
              Serial.println(streamToEsp.v);//----
              Serial.read(); // remove space
              goto getParam;
            case 'r':
              streamToEsp.r = Serial.parseInt();
              Serial.print("r: ");//----
              Serial.println(streamToEsp.r);//----
              Serial.read(); // remove space
              goto getParam;
            default:
              Serial.println("Invalid param"); //---~
              clearSerial();
          }

        }
      }//ELSE cmd 1
    } //parseParams
    else {
      Serial.println("NO COMMAND BLYEAT!"); //----
      clearSerial();
    }//else
  }//while Serial available
}//Serial event
///#################SERIAL EVENT END
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
//// SERIAL COMMANDS
void clearSerial() {
  while (Serial.available()) { // clears the serial buffer if wrong command was called
    Serial.read();
  }
}
void haltSerial() {
  while (!Serial.available()) { // will wait intill there will be bytes in the buffer
  }
}

void handShakeFl(char HET, char HAT) { // followers handshake function
  while (true) { //
    clearSerial();
    haltSerial();
    if (Serial.read() == HET) {
      clearSerial();
      Serial.write(HAT);
      break;
    }
  }//while
}


void processData() {
  if (streamToEsp.ena == 1) {
    if (((millis() - streamToEsp.lc) >= streamToEsp.p)) { // this means that p ms has passed since the last measurment and not enought messuremnt were taken
      if ((streamToEsp.ln < streamToEsp.n)) { // grouping parameters by quickest receive time fastest at the top
    
        streamToEsp.avgMoi += analogRead(SH) / streamToEsp.n;
        streamToEsp.avgLum += outputLux(BH1750_address) / streamToEsp.n; //get luminosity //++++
        streamToEsp.avgHum += dht.readHumidity() / streamToEsp.n;//++++
        streamToEsp.avgTemp += dht.readTemperature() / streamToEsp.n;//++++
        //...
        streamToEsp.ln ++;
        streamToEsp.lc = millis();
      } // if ln<n
      else { //.ln == 8 pushData, reset ln =0,cast to int,parseString, reset all values
        streamToEsp.pData = "1:";
        streamToEsp.pData += String((int)streamToEsp.avgTemp);
        streamToEsp.pData += "|";
        streamToEsp.pData += String((int)streamToEsp.avgHum);
        streamToEsp.pData += "|";
        streamToEsp.pData += String((int)streamToEsp.avgLum);
        streamToEsp.pData += "|";
        streamToEsp.pData += String((int)streamToEsp.avgMoi);
        streamToEsp.pData += "|";
        Serial.println(streamToEsp.pData);
        streamToEsp.avgMoi = 0.0;
        streamToEsp.avgLum = 0.0;
        streamToEsp.avgHum = 0.0;
        streamToEsp.avgTemp = 0.0;
        streamToEsp.ln = 0;
      } // else ln<n
    } // if millis ....
  }// if ena
  else {
  }
} //processData
