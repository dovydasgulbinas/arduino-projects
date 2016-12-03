

#include "FastLED.h"

#define LED_PIN     6 // Data pin

// Information about the LED strip itself
#define NUM_LEDS    60 //Number of LED in the whole spripe
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define BRIGHTNESS  50


#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20
// How many seconds to show black between switches
#define BLACKTIME   3

void loop()
{
    //    for(int dot = 0; dot < NUM_LEDS; dot++) { 
     //       leds[dot] = CRGB::White;
      //      FastLED.show();
            // clear this led for the next time around the loop
            //leds[dot] = CRGB::Black;
       //     delay(30);
     //   }
   //     for(int dot = 0; dot < NUM_LEDS; dot++) { 
   //       leds[dot] = CRGB::Black;
   //       }
 // FastLED.show();
 // FastLED.delay(8);
}

void setup() {
  delay( 3000 ); // power-up safety delay
  // It's important to set the color correction for your LED strip here,
  // so that colors can be more accurately rendered through the 'temperature' profiles
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );
          for(int dot = 0; dot < NUM_LEDS; dot++) { 
            leds[dot] = CRGB::White;
            FastLED.show();
            // clear this led for the next time around the loop
            //leds[dot] = CRGB::Black;
            delay(10);
        }
}


