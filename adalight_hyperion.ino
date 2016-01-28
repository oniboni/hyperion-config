#include "FastLED.h"
#include <elapsedMillis.h>

#define NUM_LEDS 136
#define EVERY_NTH 4

#define DIMMER 0.2 // 20% light -- save some amps

#define PIN 7
#define serialRate 115200

CRGB leds[NUM_LEDS];

uint8_t prefix[] = {0x41, 0x64, 0x61, 0x00, 0x21, 0x74};

elapsedMillis timeElapsedPrefix;


void setup()
{
  delay(2000);
  LEDS.addLeds<WS2811, PIN>(leds, NUM_LEDS);

  Serial.begin(serialRate);

  // testStrand();
  
  resetStrand(); // init zero
  
  timeElapsedPrefix = 0;
  
  Serial.write("Ada\n");

}

void resetStrand()
{
   LEDS.showColor(CRGB::Black);
   FastLED.show();
}

void testStrand()
{
   //start animation
  for(int x = 0; x < 255 * DIMMER; x++) { 

    LEDS.showColor(x * CRGB::Pink);

   }
  for(int x = 255 * DIMMER; x >= 0; x--) { 
    LEDS.showColor(x * CRGB::Pink);

  }
}

void waitForPrefix()
{
    for(byte i = 0; i < sizeof prefix; i++) 
    {
      waitLoop: while (!Serial.available()) 
      {
        if( timeElapsedPrefix > 5000 ) 
        {
          timeElapsedPrefix = 0;
          resetStrand();
          
          // Serial.write("Ada\n");
        }
      }
      // look for the next byte in the sequence if we see the one we want
      if(prefix[i] == Serial.read()) continue;
      if(i > 2) 
      { //ignore checksum stuff
        Serial.read();
        continue;
      }
      // start over
      i = 0;
      goto waitLoop;
    }
    timeElapsedPrefix = 0;
}

void loop()
{
    // wait for header
    waitForPrefix();

    // read all led data
    // Serial.write("prefix found");

    for(int i = 0; i < NUM_LEDS / EVERY_NTH; ++i)
    {
        // wait till there is enough data
        while (Serial.available() < 3);

        // read data
        Serial.readBytes((char*)leds[i * EVERY_NTH].raw, 3);
    }

    LEDS.show();
}
