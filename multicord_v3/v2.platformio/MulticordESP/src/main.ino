#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//#include <ArduinoOTA.h>
#define DEBUG 0

/************************* ThingsSpeak setup *********************************/

String TS_KEY = "SE4PYB5942AXS6CT"; //dashboard api key of Thingspeak
const char* TS_SERVER = "api.thingspeak.com";
const int TS_PORT = 80; //standard port of thingspeak

/************************* MQTT IO setup *********************************/

#define AIO_SERVER      "m11.cloudmqtt.com"
#define AIO_SERVERPORT  10215
#define AIO_USERNAME    "wtyeiqgw"
#define AIO_KEY         "FmDMH1mOQ52J"

/************ Global State (you don't need to change this!) ******************/

WiFiClient client, client1;


// Store the MQTT server, username, and password in flash memory.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client1, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

const char ACK_FEED[] PROGMEM = AIO_USERNAME "/feeds/mcack";
Adafruit_MQTT_Publish ackfeed = Adafruit_MQTT_Publish(&mqtt, ACK_FEED);

const char ONOFF_FEED[] PROGMEM = AIO_USERNAME "/feeds/mc";
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, ONOFF_FEED);

void MQTT_connect();
void ena_sockets(boolean state);
int update_ts_status(const char* server, const int port, String APIKey, String statusUp);
float get_power();
int push_field_to_ts(const char* server, const int port, String APIKey, float field1);
boolean time_to_trigger( long now, long *lastNow, long callPeriod = 16000);

boolean _received_command = false; //it lets the Acknowligment method know when something was received via mqtt topic
char * _last_message; //used for storing and manipulating the last mqqt message sent to the device
float _power = 0;
long _last_call = 0;


void configModeCallback (WiFiManager *myWiFiManager) { // could be used for debbuging purposes, non essiantial
}

void setup_wifi_manager(){
  WiFiManager wifiManager;  //Local intialization. Once its business is done, there is no need to keep it around
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect("MULTICORD 192.168.4.1");
  wifiManager.setAPCallback(configModeCallback);   //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  if(!wifiManager.autoConnect()) {
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi_manager();
  //setup_ota();
  update_ts_status(TS_SERVER, TS_PORT, TS_KEY, "BOOTED HELLO????");
  // Setup MQTT subscription for onoff feed.
 mqtt.subscribe(&onoffbutton); //----
}

void loop() {
  MQTT_connect();


    //########---SUBSCRIPTION BLOCK---###########
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(1000))) {
      if (subscription == &onoffbutton) {
        _received_command = true;
        _last_message = (char *)onoffbutton.lastread;
        String compare_string = String(_last_message); //temporery cast to a string used for comparing in if statements below
        if (compare_string.equals("turn_on")) {
          ena_sockets(true);
        }
        if (compare_string.equals("turn_off")) {
          ena_sockets(false);
        }
      }
    }
    //########################################
    //get power from arduino
    // Now we can publish stuff!
    if (_received_command) {
      if (! ackfeed.publish(_last_message)) {
        //Serial.println(F("Failed"));//----
      } else {
        //Serial.println(F("OK!"));//----
      }
      _received_command = false;
    }

    // ping the server to keep the mqtt connection alive
    if (!mqtt.ping()) {
      mqtt.disconnect();
    }
  if(time_to_trigger( millis(),&_last_call))
  {
    _power = get_power();
    push_field_to_ts(TS_SERVER, TS_PORT, TS_KEY, _power);
  }


}
