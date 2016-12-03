// SERIAL MUST HAVE AT LEAST 2 symbols in order to work properly
#include <ESP8266WiFi.h>

String tsKey = "SE4PYB5942AXS6CT"; //dashboard api key of Thingspeak
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
    return -1; //returns null array
  newData = false;
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

      newData = false;
    }//if of switch###############################################################################
    else if (serialData == 2) {
      Serial.print("h case \t");
      Serial.println(parseChars(';'));
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

    Serial.print("field1:\t");
    Serial.println(field1);
    Serial.print("field2:\t");
    Serial.println(field2);
    Serial.print("field3:\t");
    Serial.println(field3);
    Serial.print("field4:\t");
    Serial.println(field4);

    client.stop();
    return 1;
  }
  else {
    Serial.println("Failed to connect to TS");
    client.stop();
    return 0;
  }//else


}



