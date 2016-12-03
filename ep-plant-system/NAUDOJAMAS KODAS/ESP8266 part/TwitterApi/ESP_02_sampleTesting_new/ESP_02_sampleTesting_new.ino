// www.arduinesp.com
//
// Plot DTH11 data on thingspeak.com using an ESP8266
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com
 
#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your channel's thingspeak API key,
String apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
 
const char* server = "api.thingspeak.com";
#define DHTPIN 2 // what pin we're connected to
 
DHT dht(DHTPIN, DHT11,15);
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
 
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
}
 
void loop() {
 
 
 
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = "api_key="+apiKey+"&status="+postStr + "I+just+posted+this+from+my+thing+fourth!\r\n\r\n";
 
     client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
      client.print(postStr);
        
 Serial.println(postStr);
  
  }
  client.stop();
 
  Serial.println("Waiting...");
  // thingspeak needs minimum 15 sec delay between updates
  delay(5000);
}

