#include <ESP8266WiFi.h>

const char* apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";

WiFiClient client;

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

  
}

void loop() {
  Serial.println("At loop hit enter to continue!");
  while(!Serial.available()){
  }
  Serial.read();
  delay(1);
if(Serial.available()){
  Serial.println("@ Serial available!");
  int j = Serial.read();
  Serial.println(j);
  char *cmd;
  Serial.readBytesUntil('+',cmd,16);
  Serial.read(); //removes the + char

if(cmd == "data"){
  Serial.println("@data");
int t,h,l,m;
t = Serial.parseInt();
h = Serial.parseInt();
l = Serial.parseInt();
m = Serial.parseInt();

Serial.print("t =\t");
Serial.println(t);


Serial.print("h =\t");
Serial.println(h);

Serial.print("l =\t");
Serial.println(l);

Serial.print("m =\t");
Serial.println(m);

}
else if (cmd == "tweet"){
Serial.println("Will we crash?");
}

else{
Serial.println("Invalid CMD");
}
}//if
}//loo[
