// SERIAL MUST HAVE AT LEAST 2 symbols in order to work properly
#include <ESP8266WiFi.h>
#define FTL 120
#define RTL 120
#define RTM 120
//GET LAST VALUE ON BOOT BLT!

String tsKey = "SE4PYB5942AXS6CT"; //dashboard api key of Thingspeak
const char* twKey = "YS5OTWDY58JTJ4FR"; // Twitter api key issued by thingSpeak
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";
const int tsPort = 80; //standard port of thingspeak
boolean newData = false;
int serialData = 0;
int t = 0; //temperature
int h = 0; //humidity
int l = 0; //luminosity
int m = 0; //moisture
int pl = 0; // previous luminosity
int pm = 0; //previous moisture


WiFiClient client;

int parseChars(char terminator = '+') //Will return a char array of bytes from serial port
{
  int buff = Serial.available();
  if (buff > 1 ) { // serial port must have at least two symbols in the buffer something related to interrupt triggring
    char serialIn;
    char charArr[32] = "";
    int inc = 32;
    if (buff < 32)
      inc = buff;
    char nextChar; //gets the first char from serial buffer
    for (int i = 0; i < inc; i++) {
      nextChar = Serial.peek();
      if ((nextChar != terminator) && (nextChar != '\n') && (nextChar != '\r')) {
        charArr[i] = Serial.read();
      }
      else { //remove bad symbols
        Serial.read();
        if (Serial.peek() == '\r')
          Serial.read();
        if (Serial.peek() == '\n')
          Serial.read();
        break; // parses just the firstword matching the query
      }
    }
    Serial.println(charArr);

    newData = true;
    return atoi(charArr);
  }
  else
    newData = false;
  return -1;

}


void setup() {
  delay(1000);
  Serial.begin(19200);
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
  Serial.println("\t text input for Loop!");
  Serial.flush();
  while (!Serial.available() && !newData) {
  }
  serialData = parseChars(':');
  if (newData) {
    if (serialData == 1) {
      t = parseChars('|');
      h = parseChars('|');
      l = parseChars('|');
      m = parseChars('|');
      pushToTS(server, tsPort, tsKey, t, h, l, m);
      //tweet(server, tsPort, twKey, "This tweet was sent from: COM>ESP8266>ThingSpeak");
      dataResponse(t, h, l, m, pl, pm);
      newData = false;
    }//if of switch###############################################################################
    else if (serialData == 2) {
      Serial.print("justTweet \t");
      Serial.println(parseChars(';'));
      tweet(server, tsPort, twKey, "TestingCommand2");

      newData = false;
    }//elseif
    else {
      Serial.println("Invalid input");
      newData = false;
    }//else
  }


  delay(10);
}//#####LOOP####

int pushToTS(const char* server, const int port, String APIKey, int field1, int field2, int field3, int field4) {

  if (client.connect(server, port)) { //   "184.106.153.149" or api.thingspeak.com
    String postStr = APIKey;
    postStr += "&field1=";
    postStr += String(field1);
    postStr += "&field2=";
    postStr += String(field2);
    postStr += "&field3=";
    postStr += String(field3);
    postStr += "&field4=";
    postStr += String(field4);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    client.stop();

    Serial.print("field1:\t");
    Serial.println(field1);
    Serial.print("field2:\t");
    Serial.println(field2);
    Serial.print("field3:\t");
    Serial.println(field3);
    Serial.print("field4:\t");
    Serial.println(field4);


    return 1;
  }
  else {
    Serial.println("Failed to connect to TS");
    client.stop();
    return 0;
  }//else


}

int tweet(const char* server, int port, const char* tsTwitterKey, String message) {
  if (client.connect(server, port)) { // Opens a TCP socket and checks

    String postStr;
    postStr += "api_key=";
    postStr += tsTwitterKey; //Prepate the postString
    postStr += "&status=";
    postStr += message;

    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr + "\r\n\r\n");
    client.stop();
    Serial.println(message);

    return 1;
  }

  else {
    client.stop();
    Serial.print("Failed to relay to Twitter");
    return -1;
  }
}


void dataResponse(int t, int h, int l, int m, int pl, int pm ) {

  //##### Temperature response ####
  if ((l - pl) > RTL) { //+++ sudden increase in  luminosity | RTL - rising triger luminosity it defines the change in luminosity needed to trigger this statement |defined as RTL > d(l)/dt
    Serial.println("\t Shine bright!");
  }
  else if ((pl - l) > FTL) { //--- sudden drop in  luminosity | FTL - falling triger luminosity it defines the drop in luminosity needed to trigger this statement |defined as FTL > d(l)/dt
    Serial.println("\t No Light!");
  }
  else if ((m - pm) > RTM) { //sudden positive change in moisture | RTM - rising triger moisture it defines the change in moisture needed to trigger this statement |defined as RTM > d(m)/dt
    Serial.println("\t Drowning!");
  }
  else { // static string responses that are based based on arbitrary data input
    int selector = (int)random(0, 5); //# of cases -1
    selector = 0;  // for temporary testing purposes comment out later
    switch (selector) {
      case 0: //temperature case
        if ((t < 25) && (t > 14)) { // normal temperature
          selector = (int)random(0, 6);
          Serial.println(selector);//
          if (selector == 0)
            tweet(server, tsPort, twKey, "Not%20to%20warm%20and%20not%20to%20cold%20-%20EPIC!");
          else if (selector == 1)
            tweet(server, tsPort, twKey, "Chillaxing%20at%20the%20best%20temperature!");
          else if (selector == 2)
            tweet(server, tsPort, twKey, "Chillaxing%20at%20the%20best%20temperature!");
          else if (selector == 3)
            tweet(server, tsPort, twKey, "Chillaxing%20at%20the%20best%20temperature!");
          else if (selector == 4)
            tweet(server, tsPort, twKey, "Chillaxing%20at%20the%20best%20temperature!");

          else
            tweet(server, tsPort, twKey, "Enjoying%20the%20perfect%20temperature!");
          Serial.println("\t PERFECT TEMP");
        }
        else if (t > 24) { // to hot
          selector = (int)random(0, 4);
          Serial.println(selector);//
          if (selector == 0)
            tweet(server, tsPort, twKey, "");
          else if (selector == 1)
            tweet(server, tsPort, twKey, "");
          else if (selector == 2)
            tweet(server, tsPort, twKey, "");
          else
            tweet(server, tsPort, twKey, "");
          Serial.println("\t ");
        }
        else { // to cold

        }
        break; //case0

      case 1: //Humidity case
        break; //case1

      case 2: // Luminosity case
        break; //case2

      case 3: // Daytime case
        break; //case3

      case 4: // random facts
        break; //case4


    }//switch
  }//else
}//dataResponse


