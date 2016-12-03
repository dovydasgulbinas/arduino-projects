/*
 * REFRENCES:
 * FollowerHandShake_02_Distilled // HandshakeMethods
 * integrationWithESP_03_legit //sensor data and processing
 * FollowerHandShake_01_br_CS04 // Switch for changing function parameters // from WEB PROJECTTTTTT
 * maxMatrix_02_ScrollingTextLib //text display library
 */

#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include "DHT.h"
#include <TimerOne.h>



const  char CH[] PROGMEM = {
  3, 8, B0000000, B0000000, B0000000, B0000000, B0000000, // space // first digits is the actual width of the sprite, and second digit is the height of the sprite ir other word first digit tell how many bytes from the array should be updated
  1, 8, B1011111, B0000000, B0000000, B0000000, B0000000, // !
  3, 8, B0000011, B0000000, B0000011, B0000000, B0000000, // "
  5, 8, B0010100, B0111110, B0010100, B0111110, B0010100, // #
  4, 8, B0100100, B1101010, B0101011, B0010010, B0000000, // $
  5, 8, B1100011, B0010011, B0001000, B1100100, B1100011, // %
  5, 8, B0110110, B1001001, B1010110, B0100000, B1010000, // &
  1, 8, B0000011, B0000000, B0000000, B0000000, B0000000, // '
  3, 8, B0011100, B0100010, B1000001, B0000000, B0000000, // (
  3, 8, B1000001, B0100010, B0011100, B0000000, B0000000, // )
  5, 8, B0101000, B0011000, B0001110, B0011000, B0101000, // *
  5, 8, B0001000, B0001000, B0111110, B0001000, B0001000, // +
  2, 8, B10110000, B1110000, B0000000, B0000000, B0000000, // ,
  4, 8, B0001000, B0001000, B0001000, B0001000, B0000000, // -
  2, 8, B1100000, B1100000, B0000000, B0000000, B0000000, // .
  4, 8, B1100000, B0011000, B0000110, B0000001, B0000000, // /
  4, 8, B0111110, B1000001, B1000001, B0111110, B0000000, // 0
  3, 8, B1000010, B1111111, B1000000, B0000000, B0000000, // 1
  4, 8, B1100010, B1010001, B1001001, B1000110, B0000000, // 2
  4, 8, B0100010, B1000001, B1001001, B0110110, B0000000, // 3
  4, 8, B0011000, B0010100, B0010010, B1111111, B0000000, // 4
  4, 8, B0100111, B1000101, B1000101, B0111001, B0000000, // 5
  4, 8, B0111110, B1001001, B1001001, B0110000, B0000000, // 6
  4, 8, B1100001, B0010001, B0001001, B0000111, B0000000, // 7
  4, 8, B0110110, B1001001, B1001001, B0110110, B0000000, // 8
  4, 8, B0000110, B1001001, B1001001, B0111110, B0000000, // 9
  2, 8, B01010000, B0000000, B0000000, B0000000, B0000000, // :
  2, 8, B10000000, B01010000, B0000000, B0000000, B0000000, // ;
  3, 8, B0010000, B0101000, B1000100, B0000000, B0000000, // <
  3, 8, B0010100, B0010100, B0010100, B0000000, B0000000, // =
  3, 8, B1000100, B0101000, B0010000, B0000000, B0000000, // >
  4, 8, B0000010, B1011001, B0001001, B0000110, B0000000, // ?
  5, 8, B0111110, B1001001, B1010101, B1011101, B0001110, // @
  4, 8, B1111110, B0010001, B0010001, B1111110, B0000000, // A
  4, 8, B1111111, B1001001, B1001001, B0110110, B0000000, // B
  4, 8, B0111110, B1000001, B1000001, B0100010, B0000000, // C
  4, 8, B1111111, B1000001, B1000001, B0111110, B0000000, // D
  4, 8, B1111111, B1001001, B1001001, B1000001, B0000000, // E
  4, 8, B1111111, B0001001, B0001001, B0000001, B0000000, // F
  4, 8, B0111110, B1000001, B1001001, B1111010, B0000000, // G
  4, 8, B1111111, B0001000, B0001000, B1111111, B0000000, // H
  3, 8, B1000001, B1111111, B1000001, B0000000, B0000000, // I
  4, 8, B0110000, B1000000, B1000001, B0111111, B0000000, // J
  4, 8, B1111111, B0001000, B0010100, B1100011, B0000000, // K
  4, 8, B1111111, B1000000, B1000000, B1000000, B0000000, // L
  5, 8, B1111111, B0000010, B0001100, B0000010, B1111111, // M
  5, 8, B1111111, B0000100, B0001000, B0010000, B1111111, // N
  4, 8, B0111110, B1000001, B1000001, B0111110, B0000000, // O
  4, 8, B1111111, B0001001, B0001001, B0000110, B0000000, // P
  4, 8, B0111110, B1000001, B1000001, B10111110, B0000000, // Q
  4, 8, B1111111, B0001001, B0001001, B1110110, B0000000, // R
  4, 8, B1000110, B1001001, B1001001, B0110010, B0000000, // S
  5, 8, B0000001, B0000001, B1111111, B0000001, B0000001, // T
  4, 8, B0111111, B1000000, B1000000, B0111111, B0000000, // U
  5, 8, B0001111, B0110000, B1000000, B0110000, B0001111, // V
  5, 8, B0111111, B1000000, B0111000, B1000000, B0111111, // W
  5, 8, B1100011, B0010100, B0001000, B0010100, B1100011, // X
  5, 8, B0000111, B0001000, B1110000, B0001000, B0000111, // Y
  4, 8, B1100001, B1010001, B1001001, B1000111, B0000000, // Z
  2, 8, B1111111, B1000001, B0000000, B0000000, B0000000, // [
  4, 8, B0000001, B0000110, B0011000, B1100000, B0000000, // backslash
  2, 8, B1000001, B1111111, B0000000, B0000000, B0000000, // ]
  3, 8, B0000010, B0000001, B0000010, B0000000, B0000000, // hat
  4, 8, B1000000, B1000000, B1000000, B1000000, B0000000, // _
  2, 8, B0000001, B0000010, B0000000, B0000000, B0000000, // `
  4, 8, B0100000, B1010100, B1010100, B1111000, B0000000, // a
  4, 8, B1111111, B1000100, B1000100, B0111000, B0000000, // b
  4, 8, B0111000, B1000100, B1000100, B0101000, B0000000, // c
  4, 8, B0111000, B1000100, B1000100, B1111111, B0000000, // d
  4, 8, B0111000, B1010100, B1010100, B0011000, B0000000, // e
  3, 8, B0000100, B1111110, B0000101, B0000000, B0000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B0000000, // g
  4, 8, B1111111, B0000100, B0000100, B1111000, B0000000, // h
  3, 8, B1000100, B1111101, B1000000, B0000000, B0000000, // i
  4, 8, B1000000, B10000000, B10000100, B1111101, B0000000, // j
  4, 8, B1111111, B0010000, B0101000, B1000100, B0000000, // k
  3, 8, B1000001, B1111111, B1000000, B0000000, B0000000, // l
  5, 8, B1111100, B0000100, B1111100, B0000100, B1111000, // m
  4, 8, B1111100, B0000100, B0000100, B1111000, B0000000, // n
  4, 8, B0111000, B1000100, B1000100, B0111000, B0000000, // o
  4, 8, B11111100, B0100100, B0100100, B0011000, B0000000, // p
  4, 8, B0011000, B0100100, B0100100, B11111100, B0000000, // q
  4, 8, B1111100, B0001000, B0000100, B0000100, B0000000, // r
  4, 8, B1001000, B1010100, B1010100, B0100100, B0000000, // s
  3, 8, B0000100, B0111111, B1000100, B0000000, B0000000, // t
  4, 8, B0111100, B1000000, B1000000, B1111100, B0000000, // u
  5, 8, B0011100, B0100000, B1000000, B0100000, B0011100, // v
  5, 8, B0111100, B1000000, B0111100, B1000000, B0111100, // w
  5, 8, B1000100, B0101000, B0010000, B0101000, B1000100, // x
  4, 8, B10011100, B10100000, B10100000, B1111100, B0000000, // y
  3, 8, B1100100, B1010100, B1001100, B0000000, B0000000, // z
  3, 8, B0001000, B0110110, B1000001, B0000000, B0000000, // {
  1, 8, B1111111, B0000000, B0000000, B0000000, B0000000, // |
  3, 8, B1000001, B0110110, B0001000, B0000000, B0000000, // }
  4, 8, B0001000, B0000100, B0001000, B0000100, B0000000, // ~
};

#define DHTTYPE DHT11
#define DHTPIN 7
#define SH A0
char* r = "ready 4 ur luv!";//----
////######### LED MATRIX #########
int  DATA = 4; //data pin of arduino of led matrix
int  CLK = 3; // clock of led matrix
int CS = 2 ;// enable pin of led matix
int nDisplay = 2 ;//number of displays hooked up to a system
int bSize = 8 * nDisplay; //board size calculated bu  nDisplay * 8
MaxMatrix m(DATA, CS, CLK, nDisplay);
byte buffer[8];
///##############
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
  // String cData;// current piece of data aggregated
  int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut

} streamToEsp{1, 1, 2000, 8, 0, 0, 0, 0, 0, 0, "", 0, 0};

struct ledMatrix {
  int ena; // enable the scrolling of text if ena is > 1 that it will display message that many times
  int s; //speed. how fast text is scrolling
  // int r; // repeat. how many times message will be repeated after enablig it; ena will become 0 after r times
  char  m[144];// message. the message that will be deliverd
  int f; // flash. flash function how many times display will be flashed before sending data
  int b; // brightness of the led matrix
  int g; // space betwwen consecutive messages
} ledM {0, 100, "Pep_Chilly says: HELLO!", 0, 4, 0};



int h, t; //humidity and temperature variables
int sh; // soil humidity
int lum;//luminodity

int BH1750_address = 0x23; // i2c address of luxMeter
byte buff[2];
DHT dht(DHTPIN, DHTTYPE);

void handShakeFl(char HET = '#', char HAT = '$');
void getMessage(char mt = '`', char ct = '~'); // command termination works with every symbol?


void setup() {
  // Serial.begin(19200); //++++???
  Timer1.initialize(1000);                      //uS in which timmer is called
  Timer1.attachInterrupt(shiftChar, 1000);
  Serial.begin(9600);
  pinMode(SH, INPUT);
  //######### LED MATRIX #########
  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);
  m.init();
  m.setIntensity(4);
  digitalWrite(CS, LOW);

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
  // Serial.println("#");
  printMsg(); //++++
  //printStringWithShift(r, 80);
  //processData();//++++
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
    else if (in == 2) { //get message!
      getMessage();
    }
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
      return;
    }
  }//while
}

