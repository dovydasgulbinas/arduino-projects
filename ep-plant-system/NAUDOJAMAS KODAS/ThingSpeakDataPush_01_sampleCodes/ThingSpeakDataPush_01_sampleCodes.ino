#include <ESP8266WiFi.h>

#define ssid       "hermes-wlan"
#define pass       "Olimpietis"
#define userName   "Dovydas_Gulbinas"
#define APIKey     "2e3fr3jqgl"

WiFiClient client;

/* -------------- Setup ------------------ */
void setup() {
  // Setup Serial
  Serial.begin(9600);
  delay(2000);
  Serial.println();
  Serial.println();
  
  ESP8266_Init();
  ESP8266_Connect("http://api.thingspeak.com", 80);
  Serial.println("Initializing plot with thingspeak server...");
  // ESP _INIT GOES HERE
  Serial.println("Making sure disconnected...");
  client.stop();
  Serial.println("Done!");
  connectToServer("http://api.thingspeak.com", 80);
  initTS();
}

/* ------------- Loop -------------------- */
void loop() {
  int val = (int)random(0, 1023);
  
}

/* --------------- Functions -------------------- */

void ESP8266_Init(){
  Serial.println("-------------------------------------");
  Serial.println("           INITIALIZING...");
  Serial.println("-------------------------------------\r\n");
  
  // Put WiFi into Station mode
  Serial.println("1. Putting WiFi into station mode...");
  WiFi.mode(WIFI_STA);
  Serial.println("  Done!\r\n");
  
  // Connect to WiFi
  Serial.print("2. Connecting to: \"");
  Serial.print(ssid);
  Serial.print("\",\"");
  Serial.print(pass);
  Serial.println("\"...");
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("  WiFi connected!");  
  Serial.print("  IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  Serial.println("-------------------------------------");
  Serial.println("      INITIALIZATION COMPLETE!");
  Serial.println("-------------------------------------\r\n");
}


void ESP8266_Connect(char* url, int port){
  Serial.print("Connecting to: \"");
  Serial.print(url);
  Serial.println("\" server...");
  
  if (!client.connect(url, port)) {
    Serial.println("*Connection Failed!*\r\n");
    while(1){};
  }
  else{Serial.println("Connected!");}
}



void postToTS(int f1, int f2, int f3, int f4 char *APIkey){
  String xString = String(x);
  String yString = String(y);
  
  
  
  //Serial.print("Data sending... ");
  client.print(
    String("POST / HTTP/1.1\r\n") +
    "Host: api.thingspeak.com\r\n" +
    "Connection: close\r\n" +
    "X-THINGSPEAKAPIKEY:" + APIKey + "\r\n"
    "Connection: close\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n"
    "Content-Length: (number of characters in message)\r\n"
  
    "\r\n{\"x\": " + xString + ", \"y\": " + yString + ", \"streamtoken\": \"" + token + "\"}\n\r\n" + "0\r\n\r\n");
 Serial.println("Data Sent!"); 
}
