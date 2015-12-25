#include "LedControl.h"
int DELAY = 40;
int DELAY_FADE = 40;

//configuration notes:
//DIG pins- RGB legs
//SEG pins- common ANODE leg

// number ofleds attached to 8 x 8 driver
int NUM_LEDS = 7;

// number of seg pins used- this is hooked up to the commo
int NUM_MAX_SEG = 6;

//colors list
//primary...
byte BLUE = B0001;
byte GREEN = B0010;
byte RED = B0100;

//secondary...
byte YELLOW = RED | GREEN;
byte MAGENTA = RED | BLUE;
byte CYAN = GREEN | BLUE;

//tertiary..
byte WHITE = RED | GREEN | BLUE;

//pin configuration
int dataIn = 12;
int clock = 11;
int load = 10;
int numMaxim7219 = 1;
int resetPin = A0;
LedControl lc = LedControl(dataIn, clock, load, numMaxim7219);

void setColorWithFade(int ornament, byte color, bool isFade){
  //RGB_0 = [0, 1, 2]; RGB_1 = [3, 4, 5]
  int whichRGB = ornament/NUM_MAX_SEG;
  int whichDIG = ornament % (NUM_LEDS-1);  
  
  if(isFade){
    for(int intensity =4 ; intensity <16; intensity++){
      lc.setIntensity(0, intensity);
      setColor(whichDIG, color, whichRGB*3);
      delay(DELAY_FADE);
    }
  } else {
    setColor(whichDIG, color, whichRGB*3);
  }
}

void reset(int ornament){
    
//  turn everything off
  lc.setLed(0, 0, ornament, false );
  lc.setLed(0, 1, ornament, false );
  lc.setLed(0, 2, ornament, false );
  
  delay( DELAY );
  
  return;
}
void setColor(int digPin, byte color, int offset){
  //  note to self: 
  // setLed(0, SEG, DIG, state);
  // DIG = 0 => DP, 1 => A, 2 =>B, etc.

//  blue color
  byte flag = B0001;
  lc.setLed(0, 0+offset, digPin, (color & flag) );

// green color
  lc.setLed(0, 1+offset, digPin, (color >>1 & flag) );
  
//  red color
  lc.setLed(0, 2+offset, digPin, (color >>2 & flag) );

  return;
}

// LedControl playground
void setup(){
  pinMode(resetPin, INPUT_PULLUP);
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
}

void loop(){
//  blue, green, red 
  byte list[]= {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE, RED};
  byte list2[]= {WHITE, MAGENTA, BLUE, CYAN, GREEN, YELLOW, RED};

  for(int color = 0; color< 7; color++){
    setColorWithFade(0, list[color], false);
    delay(DELAY);  
    setColorWithFade(1, list[color+1], false);
    delay(DELAY);
    setColorWithFade(2, list[color+2], false);
    delay(DELAY);
    setColorWithFade(3, list[color+3], false);
    delay(DELAY);
    setColorWithFade(4, list[color+4], false);
    delay(DELAY);
    setColorWithFade(5, list[color+5], false);
    delay(DELAY);
    setColorWithFade(6, list[color], false);
    delay(500);
  }
}
