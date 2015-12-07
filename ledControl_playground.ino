#include "LedControl.h"
int DELAY = 50;
int DELAY_FADE = 70;

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
LedControl lc = LedControl(dataIn, clock, load, numMaxim7219);

void setColorWithFade(int ornament, byte color, bool isFade){
  if(isFade){
    for(int intensity =0 ; intensity <16; intensity++){
      lc.setIntensity(0, intensity);
      setColor(ornament, color);
      delay(DELAY_FADE);
    }
  } else {
    setColor(ornament, color);
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
void setColor(int ornament, byte color){
  //  note to self: 
  // setLed(0, SEG, DIG, state);
  // DIG = 0 => DP, 1 => A, 2 =>B, etc.

//  blue color
  byte flag = B0001;
  lc.setLed(0, 0, ornament, (color & flag) );

// green color
  lc.setLed(0, 1, ornament, (color >>1 & flag) );
  
//  red color
  lc.setLed(0, 2, ornament, (color >>2 & flag) );

  return;
}

// LedControl playground
void setup(){
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
}

void loop(){
//  blue, green, red 
  byte list[]= {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE};
  for(int color = 0; color< 7; color++){
    setColorWithFade(0, list[color], false);
    delay(500);
    setColorWithFade(1, list[color], false);
  }
}
