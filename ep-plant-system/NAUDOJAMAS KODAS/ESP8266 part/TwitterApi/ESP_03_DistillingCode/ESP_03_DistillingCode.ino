#include <ESP8266WiFi.h>

const char* apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";

WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(10);
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

 tweetToTwitter(server,80,apiKey,"Bullshit!");
}
 
void loop() {
 


}



int tweetToTwitter(const char* server, int port, const char* tsTwitterKey, String message){
if (client.connect(server,port)) {  // Opens a TCP socket and checks 
   
 String postStr;
 postStr +="api_key=";
 postStr += apiKey; //Prepate the postString
 postStr+="&status=";
 postStr+="I+just+pushed+this+from+my+thing\r\n\r\n";
 
     client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     Serial.println(postStr); //Return the Message
     client.stop();
     return 1;
}

else {
Serial.print("Failed to relay to Twitter");
return 0;
}
}

//String formatTweet


