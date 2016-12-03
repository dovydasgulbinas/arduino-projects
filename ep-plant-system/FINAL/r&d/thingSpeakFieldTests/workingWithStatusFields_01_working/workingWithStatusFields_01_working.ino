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

long timeDelay = 30000;


void setup() {
  pinMode(2, OUTPUT); //connected to GPIO2 pin2
  digitalWrite(2, LOW);

  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print(ssid);
  Serial.print("\t ON");
  // Serial.println();
  delay(10);
}



void loop() { // someHow writing to serial helps for no fucking reason


  pushStatusToTS(server, tsPort, tsKey, "ggwp");

  digitalWrite(2, HIGH);
  delay(timeDelay);
  digitalWrite(2, LOW);
  yield();
}// loop

