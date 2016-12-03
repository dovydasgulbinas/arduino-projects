// SERIAL MUST HAVE AT LEAST 2 symbols in order to work properly
#include <ESP8266WiFi.h>

const char* apiKey = "YS5OTWDY58JTJ4FR";
const char* ssid = "hermes-wlan";
const char* password = "Olimpietis";
const char* server = "api.thingspeak.com";

WiFiClient client;

char* parseChars(char terminator = '+') //Will return a char array of bytes from serial port
{
  int buff = Serial.available();
 if(buff > 1 ){ // serial port must have at least two symbols in the buffer something related to interrupt triggring
char serialIn;
char charArr[16]="";
int inc = 16;
if(buff<16)
inc = buff;
char nextChar; //gets the first char from serial buffer
for(int i = 0; i < inc; i++){
 nextChar = Serial.peek();
  if((nextChar != terminator) && (nextChar != '\n') && (nextChar != '\r')){
  charArr[i] = Serial.read();   
} 
  else{ //remove bad symbols
    Serial.read();
  }  
}
Serial.println(charArr);
return charArr;
}
else
return "";
}


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
 //looping until the output will acceptible!
Serial.println("Test Serial!");
while(!Serial.available()){
}
Serial.println("----SERIAL OUTPUT----");
parseChars();
Serial.println("----EXIT SETUP----"); 
delay(10); 
}
void loop() {
  
}//loo[




