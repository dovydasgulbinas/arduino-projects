#include <ESP8266WiFi.h>

WiFiClient client;

void handShakeLd(char HET = '#', char HAT = '$', long SLP = 350);
int pushToTSF1(const char* server, const int port, String APIKey, float field1);
int pushStatusToTS(const char* server, const int port, String APIKey, String statusUp);
void clearSerial();

int parseInts(char terminator = ':');
//void readSerial(char terminator = '|');
float parseFloats(char terminator = '|');


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
float power = 0.0;


void setup() {
  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  delay(10);
  pushStatusToTS(server, tsPort, tsKey, "connected");
  handShakeLd();
}



void loop() { // someHow writing to serial helps for no fucking reason
  delay(150); // DELAY FIXES EVERYTHING!!
  while (!Serial.available()) { ////Will read code below only if there is some data in serial buffer and if new data flag is false
    yield();
  }
  int serialData = parseInts(); // dataRed is suitable for processing
  if (newData) {
    if (serialData == 1) {
      Serial.read(); // remove the comma separator
      power = parseFloats();
      pushToTSF1(server, tsPort, tsKey, power);
      newData = false;
    }
    else if (serialData == 2) {
      newData = false;
    }//elseif
    else {
      clearSerial();
      newData = false;
    }//else
  }
  yield();
}// loop








