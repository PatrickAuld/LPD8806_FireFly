#include "LPD8806.h"
#include "SPI.h"

// Firefly like blink based on Photinus Macdermotti Firefly. 
// Blink duration is a little longer than real life but overall timing is close.
// https://legacy.mos.org/fireflywatch/images/MOS_FFW_Firefly_Flash_Chart.pdf
// Code is based on the Ada Fruit LED belt example.
// https://learn.adafruit.com/digital-led-belt
//
// Patrick Auld, patrick@patrickauld.com
//
// NOTE: WILL NOT WORK ON TRINKET OR GEMMA due to floating-point math
/*****************************************************************************/

#if defined(USB_SERIAL) || defined(USB_SERIAL_ADAFRUIT)
// this is for teensyduino support
int dataPin = 2;
int clockPin = 1;
#else 
// these are the pins we use for the LED belt kit using
// the Leonardo pinouts
int dataPin = 16;
int clockPin = 15;
#endif

LPD8806 strip = LPD8806(30, dataPin, clockPin);

void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}

void toColor(uint32_t c, uint8_t wait);
int fireFlyBlink(int endLevel);
void firefly();

void loop() {
  firefly();
}

int fireFlyBlink(int endLevel){
  int maxR = 50;
  int maxG = 127;
  int maxB = 1;
  int inInterval = 127;
  int rStep = inInterval/maxR;
  int gStep = inInterval/maxG;
  int bStep = inInterval/maxB;
  int inDelay = 1;
  for( int i = endLevel; i < inInterval; i++) {
    uint32_t color = strip.Color(i/rStep, i/gStep,i/bStep);
    int colored = inInterval - i;
    toColor(color, inDelay);
  }
  
  int outInterval = 127;
  int outDelay = 7;
  for( int i = outInterval; i > endLevel; i--) {
    uint32_t color = strip.Color(i/rStep, i/gStep,i/bStep);
    int colored = outInterval - i;
    toColor(color, outDelay);
  }
  return (outInterval-endLevel)*outDelay + (inInterval-endLevel)*inDelay;
}

void firefly(){
  int t = fireFlyBlink(10);
  delay(2000-t);
  t = fireFlyBlink(10);
  delay(4000-t);
}

void toColor(uint32_t color, uint8_t wait) {
  for (int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(wait);
}
