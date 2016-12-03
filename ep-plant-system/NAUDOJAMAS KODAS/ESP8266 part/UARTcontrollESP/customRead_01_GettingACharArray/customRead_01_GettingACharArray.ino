#include <ESP8266WiFi.h>

const char* apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";

WiFiClient client;

char* parseChars(char terminator = '+') //Will return a char array of bytes from serial port
{
  // && (nextChar != '\n') && (nextChar != '\r')
char serialIn;
char*charArr;
char nextChar = Serial.peek();
Serial.print("nextChar=\t");
Serial.println(nextChar);

while((Serial.available())&& (nextChar != '+')&& (nextChar != '\n')){
serialIn = (char)Serial.read();
*charArr +=serialIn;
nextChar = Serial.peek();
Serial.println(charArr);
}
return charArr;
}


void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  delay(10);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
delay(10);
while(true){ //looping until the output will acceptible!
Serial.println("Input some text and hit enter!");
while(!Serial.available()){
}
Serial.println("----OUTPUT----");
Serial.println(parseChars());

Serial.println("----EXIT SETUP----"); 
delay(250); 
}
}
void loop() {
 
}//loo[




