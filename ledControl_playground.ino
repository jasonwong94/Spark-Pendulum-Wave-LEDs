#include "LedControl.h"
int DELAY = 40;
int DELAY_FADE = 5;

bool TEST= true;

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

//list..
byte LIST[]= {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE, RED};

//pin configuration
int dataIn = 12;
int clock = 11;
int load = 10;
int numMaxim7219 = 1;
int resetPin = 2;
volatile bool sequenceActivated = true;

LedControl lc = LedControl(dataIn, clock, load, numMaxim7219);

void setColorWithFade(int ornament, byte color, bool isFade){
  //RGB_0 = [0, 1, 2]; RGB_1 = [3, 4, 5]
  int whichRGB = ornament/NUM_MAX_SEG;
  int whichDIG = ornament % (NUM_LEDS-1);  
  
  if(isFade){
    for(int intensity =0 ; intensity <16; intensity++){
      lc.setIntensity(0, intensity);
      setColor(whichDIG, color, whichRGB*3);
      delay(DELAY_FADE);
    }
    for(int intensity =15 ; intensity >=0; intensity--){
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
  pinMode(resetPin, OUTPUT);
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);
  attachInterrupt(0, resetSequence, CHANGE);
  
  if(TEST)
    testLEDs(0);  
}

void resetSequence(){
  sequenceActivated = !sequenceActivated;
}
void loop(){
  sequence();
  //if switch[0] = on => reset
  //else => continue
}

void sequence(){    
  if(sequenceActivated){
    Serial.println("true"); 
  } else {
    Serial.println("false"); 
  }
  for(int color = 0; (color< 7 && sequenceActivated); color++){
    setColorWithFade(0, LIST[color], false);
    delay(DELAY);  
    setColorWithFade(1, LIST[color], false);
    delay(DELAY);
    setColorWithFade(2, LIST[color], false);
    delay(DELAY);
    setColorWithFade(3, LIST[color], false);
    delay(DELAY);
    setColorWithFade(4, LIST[color], false);
    delay(DELAY);
    setColorWithFade(5, LIST[color], false);
    delay(DELAY);
    setColorWithFade(6, LIST[color], false);
    delay(DELAY);
  }

  if(!sequenceActivated)
    for(int ornament = 0; ornament< NUM_LEDS; ornament++)
      reset(ornament);
}

//note: specificLED starts at 0
void testLEDs(int specificLED){
  if(specificLED == -1){
    for(int led = 0; led<NUM_LEDS; led++){  
      for(int color = 0; color<7; color++){
        setColorWithFade(led, LIST[color], false);
        delay(250);
      }
    }
  } else {
    for(int color = 0; color<7; color++){
      setColorWithFade(specificLED, LIST[color], false);
      delay(500);
    }
  }
}
