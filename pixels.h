
//###########################################################
// included libraries
//###########################################################
#include <Adafruit_NeoPixel.h>
#include "i2c.h"

//##########################################################
// LED Pixels
//##########################################################
int PIXEL_LEVEL_FLOOR = 20,
    PIXEL_LEVEL_F = 0, // current accel setting for forward
    PIXEL_LEVEL_B = 0, // current accel setting for back
    PIXEL_LEVEL_L = 0, // current accel setting for left
    PIXEL_LEVEL_R = 0; // current accel setting for right

int PIXEL_LEVEL_RED    = 0,
    PIXEL_LEVEL_BLUE   = 0, // current accel setting for forward
    PIXEL_LEVEL_GREEN  = 0, // current accel setting for back
    PIXEL_LEVEL_YELLOW = 0;

// pin-outs for pixel led strips
#define STRIP_L_PIN 9
#define STRIP_R_PIN 10
#define STRIP_F_PIN 11
#define STRIP_B_PIN 15

// number of pixels per strips
#define NUM_PIXELS 10
// default brightness of led strips
#define LED_BRIGHTNESS 75

// initialize each strip variable
Adafruit_NeoPixel STRIP_F = Adafruit_NeoPixel(NUM_PIXELS, STRIP_F_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel STRIP_B = Adafruit_NeoPixel(NUM_PIXELS, STRIP_B_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel STRIP_L = Adafruit_NeoPixel(NUM_PIXELS, STRIP_L_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel STRIP_R = Adafruit_NeoPixel(NUM_PIXELS, STRIP_R_PIN, NEO_GRB + NEO_KHZ800);


void showStrips() {
  STRIP_F.show();
  STRIP_B.show();
  STRIP_L.show();
  STRIP_R.show();
}
void resetStrips(int ACTIVE_PIXEL, int ACTIVE_PIXEL_L) {

      for(int i=2; i < NUM_PIXELS; i++) {
        STRIP_F.setPixelColor(i,STRIP_F.Color(0,255,0));  
        STRIP_B.setPixelColor(i,STRIP_B.Color(0,255,0));  
        STRIP_L.setPixelColor(i,STRIP_L.Color(0,255,0));  
        STRIP_R.setPixelColor(i,STRIP_R.Color(0,255,0));  
      }
      showStrips();
}
// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return STRIP_F.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return STRIP_F.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return STRIP_F.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void rainbowCycle() {
  uint16_t i, j;

  for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_PIXELS; i++) {
      STRIP_F.setPixelColor(i, Wheel(((i * 256 / NUM_PIXELS) + j) & 255));
      STRIP_B.setPixelColor(i, Wheel(((i * 256 / NUM_PIXELS) + j) & 255));
      STRIP_L.setPixelColor(i, Wheel(((i * 256 / NUM_PIXELS) + j) & 255));
      STRIP_R.setPixelColor(i, Wheel(((i * 256 / NUM_PIXELS) + j) & 255));
    }
    showStrips();
  }
}

void initStrips() {
	  STRIP_F.begin();
	  STRIP_B.begin();
	  STRIP_L.begin();
	  STRIP_R.begin();

      STRIP_F.setBrightness(LED_BRIGHTNESS);
      STRIP_B.setBrightness(LED_BRIGHTNESS);
      STRIP_L.setBrightness(LED_BRIGHTNESS);
      STRIP_R.setBrightness(LED_BRIGHTNESS);
      rainbowCycle();
}


