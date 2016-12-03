#include <ESP8266WiFi.h>

const char* apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";
const char terminator = ':';

WiFiClient client;

void setup() {
  delay(1000);
  Serial.begin(115200);
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
  tweetToTwitter(server, 80, apiKey, "get prost M8???" + (const char*)String(random(10,20)));
}

void loop() {





  
if(Serial.available()){
//While(Serial)

}//while
}//loop

int tweetToTwitter(const char* server, int port, const char* tsTwitterKey, const char* message) {
  if (client.connect(server, port)) { // Opens a TCP socket and checks

    String postStr;
    postStr += "api_key=";
    postStr += apiKey; //Prepate the postString
    postStr += "&status=";
    postStr += URLEncode(message);

    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr + "\r\n\r\n");
    
    Serial.println(URLEncode(message));
    client.stop();
    return 1;
  }

  else {
    Serial.print("Failed to relay to Twitter");
    return 0;
  }
}

String URLEncode(const char* msg)
{
    const char *hex = "0123456789abcdef";
    String encodedMsg = "";

    while (*msg!='\0'){
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') ) {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}



