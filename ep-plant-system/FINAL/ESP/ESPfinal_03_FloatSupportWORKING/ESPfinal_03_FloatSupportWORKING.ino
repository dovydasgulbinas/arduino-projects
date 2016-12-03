#include <ESP8266WiFi.h>

WiFiClient client;

void handShakeLd(char HET = '#', char HAT = '$', long SLP = 350);
int parseInts(char terminator = ':');
//void readSerial(char terminator = '|');
float parseFloats(char terminator = '|');


//############ VARIABLES #############
String tsKey = "SE4PYB5942AXS6CT"; //dashboard api key of Thingspeak
////const char* twKey = "YS5OTWDY58JTJ4FR"; // Twitter api key issued by thingSpeak
const char* ssid = "Pretty Fly For A Wifi";
const char* password = "nenusipelneidar";
const char* server = "api.thingspeak.com";
const int tsPort = 80; //standard port of thingspeak

char * stream;
boolean newString = false;
boolean newInt = false;
boolean newData = false;
float t = 0.0; //temperature
float h = 0.0; //humidity
float l = 0.0; //luminosity
float m = 0.0; //moisture


int pushStatusToTS(const char* server, const int port, String APIKey, String statusUp);
int pushToTS(const char* server, const int port, String APIKey, float field1, float field2, float field3, float field4);
void clearSerial();

void setup() {
  pinMode(2, OUTPUT); //connected to GPIO2 pin2
  digitalWrite(2, LOW);

  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, password);
   Serial.println("Im connected");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  delay(10);
  pushStatusToTS(server, tsPort, tsKey, "connected");
  //handShakeLd();
    clearSerial();
  digitalWrite(2, HIGH);
}



void loop() { // someHow writing to serial helps for no fucking reason
  delay(150); // DELAY FIXES EVERYTHING!!
  Serial.println("Im allive bitch");
  while (!Serial.available()) { ////Will read code below only if there is some data in serial buffer and if new data flag is false
    yield();
  }
  digitalWrite(2, LOW);
  int serialData = parseInts(); // dataRed is suitable for processing
  if (newData) {
    if (serialData == 1) {
      t = parseFloats();
      h = parseFloats();
      l = parseFloats();
      m = parseFloats();
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
      newData = false;
    }//else
  }
  yield();
}// loop










