#include <ESP8266WiFi.h>

WiFiClient client;

int parseChars(char terminator = ':');
//void readSerial(char terminator = '|');
float parseFloats(char terminator = '|');


//############ VARIABLES #############
//String tsKey = "SE4PYB5942AXS6CT"; //dashboard api key of Thingspeak
//const char* twKey = "YS5OTWDY58JTJ4FR"; // Twitter api key issued by thingSpeak
const char* ssid = "Pretty Fly For A Wifi";
const char* password = "nenusipelneidar";
//const char* server = "api.thingspeak.com";
//const int tsPort = 80; //standard port of thingspeak

//char * stream;

boolean newData = false;

int serialData = 0;
int t = 0; //temperature
int h = 0; //humidity
int l = 0; //luminosity
int m = 0; //moisture

void setup() {
  //pinMode(2, OUTPUT); //connected to GPIO2 pin2
  //digitalWrite(2, LOW);

  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println("\t Connecting to W-LAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  delay(10);
  //pushStatusToTS(server, tsPort, tsKey, "connected"); // Writes a message to status field in TS
  //digitalWrite(2, HIGH);
  Serial.println("\t Going to loop()");
}


void loop() { // someHow writing to serial helps for no fucking reason
  Serial.println("\t HIT ENTER TO SEND RANDOM DATA TO 4 FIELDS");

  delay(1000); // DELAY FIXES EVERYTHING!!
 // digitalWrite(2, LOW);
//  while (!Serial.available()) { ////Will read code below only if there is some data in serial buffer and if new data flag is false
  //  yield();
//  }

  t = random(0,99) ;
  h = random(100,199) ;
  l = random(55,66) ;
  m = random(77,300) ;
  //pushToTS(server, tsPort, tsKey, t, h, l, m);
  //digitalWrite(2, HIGH);

  //yield();
}// loop








