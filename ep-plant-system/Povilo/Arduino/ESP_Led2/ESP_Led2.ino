/*-------------------------------------------------------------------------------------
   Access Point Version
   http://192.168.4.1
 
   Works with Huzzah; other modules not tested yet.
 
   v01:   webserver extended to 7 on and off switches
   v02:   added demo reel. Had to declare FastLED before
          the webserver. Then put the complete webserver
          into a void webserver() and leave in the loop
          just the case statements and the FastLEDshow.
          Added power control
   v02.1  changed to a drop down menu    
 
  Usage:  after upload open the Serial Monitor in Arduino and see what
          IP address is returned. In my case it is 192.168.1.252
          Open this IP address in a browser (PC or phone)
 
 Gyro Gearloose, Feb 2016
 
/*-------------------------------------------------------------------------------------
HTTP 1.1 Webserver for ESP8266 adapted to Arduino IDE
 
From Stefan Thesen 04/2015
https://blog.thesen.eu/http-1-1-webserver-fuer-esp8266-als-accesspoint/
https://blog.thesen.eu/stabiler-http-1-1-wlan-webserver-mit-dem-esp8266-microcontroller/
 
Running stable for days
(in difference to all samples I tried)
 
Does HTTP 1.1 with defined connection closing.
Reconnects in case of lost WiFi.
Handles empty requests in a defined manner.
Handle requests for non-exisiting pages correctly.
 
This demo allows to switch two functions:
Function 1 creates serial output and toggels GPIO2
Function 2 just creates serial output.
 
Serial output can e.g. be used to steer an attached
Arduino, Raspberry etc.
---------------------------------------------------------------------------------------*/
 
// FastLED setup -----FastLED has to be declared BEFORE the Webserver-----
#include "FastLED.h"
//FASTLED_USING_NAMESPACE
 
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

 
#define DATA_PIN      13     // for Huzzah: Pins w/o special function:  #4, #5, #12, #13, #14; // #16 does not work :( //6
//#define CLK_PIN     12
//#define LED_TYPE    APA102
//#define COLOR_ORDER BGR
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS      60    
CRGB leds[NUM_LEDS];
 
#define BRIGHTNESS          96
//#define MILLI_AMPERE      1000
#define FRAMES_PER_SECOND  120
 
int ledMode = 2;            // this is the starting animation
 
// Websever setup -----------------------------------------------------
#include <ESP8266WiFi.h>

const char* ssid = "Pretty Fly For A Wifi";
const char* password = "nenusipelneidar";
 
unsigned long ulReqcount;
 
 
// Create an instance of the server on Port 80
WiFiServer server(80);
 
void setup()
{
  // setup globals
  ulReqcount=0;
 
  // prepare GPIO2
  //pinMode(2, OUTPUT);
 // digitalWrite(2, 0);
 
  // start serial
  Serial.begin(115200);
  delay(1);
 
  // AP mode
  WiFi.mode(WIFI_STA);
   WiFiStart();

   delay(1000);
  //WiFi.softAP(ssid, password);
  //server.begin();
 
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(DirectSunlight);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(DirectSunlight);
  FastLED.setBrightness(BRIGHTNESS);
  //set_max_power_in_volts_and_milliamps(5, MILLI_AMPERE);
}
 
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 
void WiFiStart()
{
 
  // Connect to WiFi network
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.println(WiFi.localIP());
}
 
void loop() {
  webserver();
 
 // if (ledMode != 999) {
 
  //   switch (ledMode) {
  //    case  1: all_off(); break;
   //   case  2: rainbow(); break;
   //   case  3: rainbowWithGlitter(); break;
   //   case  4: confetti(); break;
   //   case  5: sinelon(); break;
   //  case  6: juggle(); break;
   //   case  7: bpm(); break;
    //  }
    //  }
  //show_at_max_brightness_for_power();
 // delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
//  FastLED.show();  
//  FastLED.delay(1000/FRAMES_PER_SECOND);
 // EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
 
} // end of loop *************************************************************************************
 
void webserver() {   /// complete web server /////////////////////////////////////////////////////////
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  unsigned long ultimeout = millis()+250;
  while(!client.available() && (millis()<ultimeout) )
  {
    delay(1);
  }
  if(millis()>ultimeout)
  {
    Serial.println("client connection time-out!");
    return;
  }
 
  // Read the first line of the request
  String sRequest = client.readStringUntil('\r');
  //Serial.println(sRequest);
  client.flush();
 
  // stop client, if request is empty
  if(sRequest=="")
  {
    Serial.println("empty request! - stopping client");
    client.stop();
    return;
  }
 
  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  String sPath="",sParam="", sCmd="";
  String sGetstart="GET ";
  int iStart,iEndSpace,iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart>=0)
  {
    iStart+=+sGetstart.length();
    iEndSpace = sRequest.indexOf(" ",iStart);
    iEndQuest = sRequest.indexOf("?",iStart);
   
    // are there parameters?
    if(iEndSpace>0)
    {
      if(iEndQuest>0)
      {
        // there are parameters
        sPath  = sRequest.substring(iStart,iEndQuest);
        sParam = sRequest.substring(iEndQuest,iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = sRequest.substring(iStart,iEndSpace);
      }
    }
  }
 
  ///////////////////////////////////////////////////////////////////////////////
  // output parameters to serial, you may connect e.g. an Arduino and react on it
  ///////////////////////////////////////////////////////////////////////////////
  if(sParam.length()>0)
  {
    int iEqu=sParam.indexOf("=");
    if(iEqu>=0)
    {
      sCmd = sParam.substring(iEqu+1,sParam.length());
      Serial.println(sCmd);
    }
  }
 
 
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse,sHeader;
 
  ////////////////////////////
  // 404 for non-matching path
  ////////////////////////////
  if(sPath!="/")
  {
    sResponse="<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
   
    sHeader  = "HTTP/1.1 404 Not found\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
  ///////////////////////
  // format the html page
  ///////////////////////
  else
  {
    ulReqcount++;
    sResponse  = "<html><head><title>ESP8266 access point for DemoReel100</title></head><body>";
    sResponse += "<font color=\"#FFFFF0\"><body bgcolor=\"#000000\">";  // first is background, second is font color
    sResponse += "<FONT SIZE=-1>";
    sResponse += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">";
    sResponse += "<h1>ESP8266 access point<br>";
    sResponse += " for DemoReel100</h1>";
 
/*  this creates a list with ON / OFF buttons
    // </a>&nbsp is a non-breaking space; moves next character over
    sResponse += "<p>Rainbow &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION1ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION1OFF\"><button>--OFF--</button></a><br>";
    sResponse += "<p>Rainbow Glitter<a href=\"?pin=FUNCTION2ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION2OFF\"><button>--OFF--</button></a><br>";
    sResponse += "<p>Confetti &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION3ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION3OFF\"><button>--OFF--</button></a><br>";
    sResponse += "<p>Sinelon &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION4ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION4OFF\"><button>--OFF--</button></a><br>";
    sResponse += "<p>Juggle&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION5ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION5OFF\"><button>--OFF--</button></a></p>";
    sResponse += "<p>BPM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION6ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION6OFF\"><button>--OFF--</button></a></p>";
    sResponse += "<p>Function 7&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION7ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION7OFF\"><button>--OFF--</button></a></p><br>";
*/
//  This is a nice drop down menue
    sResponse += "<FONT SIZE=+1>";
    sResponse += "<form>";
    sResponse += "Select Animation<br>";
    sResponse += "<select name=\"sCmd\" size=\"7\" >";
    sResponse += "<option value=\"FUNCTION1OFF\"selected>All OFF</option>";
    sResponse += "<option value=\"FUNCTION1ON\">Rainbow</option>";
    sResponse += "<option value=\"FUNCTION2ON\">Rainbow Glitter</option>";
    sResponse += "<option value=\"FUNCTION3ON\">Confetti</option>";
    sResponse += "<option value=\"FUNCTION4ON\">Sinelon</option>";
    sResponse += "<option value=\"FUNCTION5ON\">Juggle</option>";
    sResponse += "<option value=\"FUNCTION6ON\">BPM</option><br>";
    sResponse += "</select>";
    sResponse += "<br><br>";
    sResponse += "<input type= submit>";
    sResponse += "</form>";
    sResponse += "<FONT SIZE=-1>";
 
    //////////////////////
    // react on parameters
    //////////////////////
    if (sCmd.length()>0)
    {
      // write received command to html page
      sResponse += "Command: " + sCmd + "<BR>";
     
      // switch GPIO
      if(sCmd.indexOf("FUNCTION1ON")>=0)
      {
        Serial.println("1 ON");
        ledMode = 2;
      }
      else if(sCmd.indexOf("FUNCTION1OFF")>=0)
      {
        Serial.println("1 OFF");
        ledMode = 1;
      }
 
      if(sCmd.indexOf("FUNCTION2ON")>=0)
      {
         Serial.println("2 ON");
        ledMode = 3;
      }
      else if(sCmd.indexOf("FUNCTION2OFF")>=0)
      {
        Serial.println("2 OFF");
        ledMode = 1;
      }
 
      if(sCmd.indexOf("FUNCTION3ON")>=0)
      {
         Serial.println("3 ON");
        ledMode = 4;
 
      }
      else if(sCmd.indexOf("FUNCTION3OFF")>=0)
      {
        Serial.println("3 OFF");
        ledMode = 1;
 
      }
      if(sCmd.indexOf("FUNCTION4ON")>=0)
      {
        Serial.println("4 ON");
        ledMode = 5;
 
      }
      else if(sCmd.indexOf("FUNCTION4OFF")>=0)
      {
        Serial.println("4 OFF");
        ledMode = 1;
 
      }
      if(sCmd.indexOf("FUNCTION5ON")>=0)
      {
         Serial.println("5 ON");
        ledMode = 6;
 
      }
      else if(sCmd.indexOf("FUNCTION5OFF")>=0)
      {
        Serial.println("5 OFF");
        ledMode = 1;
 
      }
 
      if(sCmd.indexOf("FUNCTION6ON")>=0)
      {
         Serial.println("6 ON");
        ledMode = 7;
 
      }
      else if(sCmd.indexOf("FUNCTION6OFF")>=0)
      {
        Serial.println("6 OFF");
        ledMode = 1;
 
      }
      if(sCmd.indexOf("FUNCTION7ON")>=0)
      {
        Serial.println("7 ON");
        ledMode = 8;
 
      }
      else if(sCmd.indexOf("FUNCTION7OFF")>=0)
      {
         Serial.println("7 OFF");
        ledMode = 1;
 
      }
 
    } // end sCmd.length()>0
   
    sResponse += "<FONT SIZE=-2>";
    sResponse += "<BR>clicks on page =";
    sResponse += ulReqcount;
    sResponse += "<BR>";
    sResponse += "Gyro Gearloose 02/2016<BR><BR>";
    sResponse += "<font color=\"#FF0000\">";
    sResponse += "DemoReel 100 by Mark Kriegsman<BR>";
    sResponse += "Webserver by Stefan Thesen<BR>";
    sResponse += "</body></html>";
    sHeader  = "HTTP/1.1 200 OK\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
 
  // Send the response to the client
  client.print(sHeader);
  client.print(sResponse);
 
 
  // and stop the client
  client.stop();
  Serial.println("Client disonnected");  
  }
/// END of complete web server //////////////////////////////////////////////////////////////////
 
// LED animations ###############################################################################
void all_off() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);  
  //FastLED.show();
  //FastLED.delay(1000/FRAMES_PER_SECOND);
}
 
void rainbow()
{
  // FastLED's built-in rainbow generator
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND);
}
 
void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}
 
void addGlitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
 
void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
//  FastLED.show();  
//  FastLED.delay(1000/FRAMES_PER_SECOND);
}
 
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
//  FastLED.show();  
//  FastLED.delay(1000/FRAMES_PER_SECOND);
}
 
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
//  FastLED.show();  
//  FastLED.delay(1000/FRAMES_PER_SECOND);
}
 
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
//  FastLED.show();  
//  FastLED.delay(1000/FRAMES_PER_SECOND);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*  HTML code to try changes on w3schools.com  http://www.w3schools.com/tags/tryit.asp?filename=tryhtml_a_href
 
<!DOCTYPE html>
<html>
<font color =   #fffff0><body bgcolor=\"#000000\">   //FFFFFF0 has to be written in fffff0
<FONT SIZE=-1>
<h1>ESP8266 control <br>
for DemoReel100</h1>
<body>
 
<p>Rainbow &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION1ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION1OFF\"><button>--OFF--</button></a><br>
 
<p>Rainbow Glitter<a href=\"?pin=FUNCTION2ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION2OFF\"><button>--OFF--</button></a><br>
 
<p>Confetti &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION3ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION3OFF\"><button>--OFF--</button></a><br>
 
<p>Sinelon &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION4ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION4OFF\"><button>--OFF--</button></a><br>
 
<p>Juggle&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION5ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION5OFF\"><button>--OFF--</button></a></p>
 
<p>BPM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION6ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION6OFF\"><button>--OFF--</button></a></p>
 
<p>Function 7&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"?pin=FUNCTION7ON\"><button>--ON--</button></a>&nbsp;<a href=\"?pin=FUNCTION7OFF\"><button>--OFF--</button></a></p><br>
 
<FONT SIZE=+1>
<form>
<p>Select Animation</p>
<select name=\"sCmd\" size=\"7\" >
<option value=\"FUNCTION1OFF\"selected>All OFF</option>
<option value=\"FUNCTION1ON\">Rainbow</option>
<option value=\"FUNCTION2ON\">Rainbow Glitter</option>
<option value=\"FUNCTION3ON\">Confetti</option>
<option value=\"FUNCTION4ON\">Sinelon</option>
<option value=\"FUNCTION5ON\">Juggle</option>
<option value=\"FUNCTION6ON\">BPM</option><br>
</select>
<br><br>
<input type= submit>
</form>
<FONT SIZE=-1>
 
<FONT SIZE=-2>
<BR>clicks on page =
 - connections to page =
<BR>
Gyro Gearloose 02/2016<BR><BR>
<font color= #ff0000>
DemoReel 100 by Mark Kriegsman<BR>
Webserver by Stefan Thesen<BR>
 
</body>
</html>
 
 
*/
