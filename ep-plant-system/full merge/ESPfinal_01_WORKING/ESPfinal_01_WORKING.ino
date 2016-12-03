#include <ESP8266WiFi.h>

WiFiClient client;

void handShakeLd(char HET = '#', char HAT = '$', long SLP = 350);
int parseChars(char terminator = '|');
void readSerial(char terminator = '|');


//############ VARIABLES #############
String tsKey = "SE4PYB5942AXS6CT"; //dashboard api key of Thingspeak
const char* twKey = "YS5OTWDY58JTJ4FR"; // Twitter api key issued by thingSpeak
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";
const int tsPort = 80; //standard port of thingspeak

char * stream;
boolean newString = false;
boolean newInt = false;
boolean newData = false;

int serialData = 0;
int t = 0; //temperature
int h = 0; //humidity
int l = 0; //luminosity
int m = 0; //moisture
int pl = 0; // previous luminosity
int pm = 0; //previous moisture


void setup() {
  pinMode(2, OUTPUT); //connected to GPIO2 pin2
  digitalWrite(2, LOW);

  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  delay(10);
  handShakeLd();
  digitalWrite(2, HIGH);
}



void loop() { // someHow writing to serial helps for no fucking reason
 // Serial.println("\t text input for Loop!");//--
  delay(150);
  //Serial.flush();//---
  while (!Serial.available()) { ////Will read code below only if there is some data in serial buffer and if new data flag is false
    yield();
  }
  serialData = parseChars(':'); // dataRed is suitable for processing
  if (newData) {
    if (serialData == 1) {
      t = parseChars('|');
      h = parseChars('|');
      l = parseChars('|');
      m = parseChars('|');
      pushToTS(server, tsPort, tsKey, t, h, l, m);
      newData = false;
      digitalWrite(2, HIGH);
    }//if of switch###############################################################################
    else if (serialData == 2) {
      newData = false;
    }//elseif
    else {
      digitalWrite(2, LOW);
      clearSerial();
      
      //Serial.println("Invalid+input");
      //WRITE REPEAT DATA REQUEST!
      newData = false;
    }//else
  }
yield();
}// loop








