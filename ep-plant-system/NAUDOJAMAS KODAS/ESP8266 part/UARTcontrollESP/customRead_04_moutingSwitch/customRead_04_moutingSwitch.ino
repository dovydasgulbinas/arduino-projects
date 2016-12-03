// SERIAL MUST HAVE AT LEAST 2 symbols in order to work properly
#include <ESP8266WiFi.h>

const char* apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";
boolean newData = false;
char* serialData = "";
char* t = " "; //temperature
char* h = " "; //humidity
char* l = " "; //luminosity
char* m = " "; //moisture

WiFiClient client;

char* parseChars(char terminator = '+') //Will return a char array of bytes from serial port
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
        if(Serial.peek()=='\r')
        Serial.read();
        if(Serial.peek()=='\n')
        Serial.read();
        break; // parses just the firstword matching the query
      }
    }
    Serial.println(charArr);
     newData = true;
    return charArr;
  }
  else
    return ""; //returns null array
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
  Serial.println("text input for Loop!");
  Serial.flush();
  while (!Serial.available()&& !newData) {
  }
  serialData = parseChars(':');
  if (newData) {
    if (serialData[0] == 'd') {
      t = parseChars('|');
      h = parseChars('|');
      l = parseChars('|');
      m = parseChars('|');
      newData = false;
      Serial.println("d case");
      Serial.print("t= \t");
      Serial.println(t);
      Serial.print("h= \t");
      Serial.println(h);
      Serial.print("l= \t");
      Serial.println(l);
      Serial.print("m= \t");
      Serial.println(m);
      Serial.flush();
      
    }//if of switch###############################################################################
    else if (serialData[0] == 'h') {
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




