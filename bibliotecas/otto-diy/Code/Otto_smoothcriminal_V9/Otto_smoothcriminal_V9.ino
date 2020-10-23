//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY APP Firmware version 9 (V09) with standard baudrate of 9600 for Bluetooth modules.
//-- This code will have all modes and functions therefore memory is almost full but ignore the alert it works perfectly.
//-- Designed to work with the basic Otto or PLUS or Humanoid or other biped robots. some of these functions will need a good power source such as a LIPO battery.
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -- ADDED Progmem for MOUTHS and GESTURES: Paul Van De Veen October 2018
// -- ADDED PIN definitions for ease of use: Jason Snow November 2018
// -- ADDED NEOPIXEL: Paul Van De Veen November 2018
// -- ADDED Eye Matrix Progmem and control: Jason Snow November 2018
// -- DELETED noise sense in mode 3 Jason Snow August 2019
// -- REMOVED Eye Matrix Progmem and control: Jason Snow AUG 2019
// -- ADDED Battery meassurementin in mode 3 Jason Snow August 2019
//-------------------------------------------------------------------------
#include <EEPROM.h>
#include <EnableInterrupt.h>
// Library to manage the Neopixel RGB led
#include <Adafruit_NeoPixel.h>
//-- Library to manage serial commands
#include <OttoSerialCommand.h>
OttoSerialCommand SCmd;  // The SerialCommand object
//-- Otto Library version 9
#include <Otto9.h>
Otto9 Otto;  //This is Otto!

//---------------------------------------------------------
//-- First step: Configure the pins where the servos are attached
/*
         ---------------
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         ---------------
            ||     ||
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// BATTERY SENSE PIN //////////////////////////////////////////////////////////////////////////
boolean BATTcheck = true;    // SET TO FALSE IF NOT USING THIS OPTION
#define PIN_Battery   A7  //3v7 BATTERY MONITOR   ANALOG pin (A7)
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)
// RGB NEOPIXEL LED PIN   /////////////////////////////////////////////////////////////////////
boolean enableRGB = false;    // SET TO FALSE IF NOT USING THIS OPTION
#define NeopixelRGB_PIN  12 // NEOPIXEL pin   DIGITAL PIN (12)
#define NUMPIXELS       1   // There is only one Neopixel use in MY Otto, chnage for more than 1
Adafruit_NeoPixel NeopixelLed = Adafruit_NeoPixel(NUMPIXELS, NeopixelRGB_PIN, NEO_RGB + NEO_KHZ800);
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 7 and GND
#define PIN_ASSEMBLY    7   //ASSEMBLY pin (7) LOW = assembly    HIGH  = normal operation
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
#define N_SERVOS 4
const char programID[] = "Otto_BLANK9"; //Each program will have a ID
unsigned long final_time;
unsigned long interval_time;
int oneTime;
int iteration;
//float increment[N_SERVOS]; 
//int oldPosition[]={90,90,90,90};
void goingUp(int tempo);
void drunk (int tempo);
void noGravity(int tempo);
void kickLeft(int tempo);
void kickRight(int tempo);
//void run(int steps, int T=500);
//void walk(int steps, int T=1000);
void backyard(int steps, int T=3000);
void backyardSlow(int steps, int T=5000);
// TEMPO: 121 BPM
int t=495;
double pause=0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
  Otto.initBatLevel(PIN_Battery);// set up Battery percent read pin - MUST BE AN ANALOG PIN
  pinMode(PIN_ASSEMBLY,INPUT_PULLUP); // - Easy assembly pin - LOW is assembly Mode
  //Otto wake up!
  Otto.sing(S_connection);
  Otto.home();
// if Pin 7 is LOW then place OTTO's servos in home mode to enable easy assembly, 
// when you have finished assembling Otto, remove the link between pin 7 and GND
  while (digitalRead(PIN_ASSEMBLY) == LOW) {
    Otto.home();
    Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
    delay(5000);
  }
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop()
{

dance();

}

void dance(){
  primera_parte();
  segunda_parte();
  Otto.moonwalker(4, t*2, 20, 1);
  Otto.moonwalker(4, t*2, 20, -1);
  Otto.moonwalker(4, t*2, 20, 1);
  Otto.moonwalker(4, t*2, 20, -1);
  primera_parte(); 
  Otto.crusaito(1, t*8, 20, 1);
  Otto.crusaito(1, t*8, 20, 1);

  for (int i=0; i<16; i++){
    flapping2(1,t/4);
    delay(3*t/4);
  }
  Otto.moonwalker(4, t*2, 20, -1);
  Otto.moonwalker(4, t*2, 20, 1);
  Otto.moonwalker(4, t*2, 20, -1);
  Otto.moonwalker(4, t*2, 20, 1);

  drunk(t*4);
  drunk(t*4);
  drunk(t*4);
  drunk(t*4);
  kickLeft(t);
  kickRight(t);
  drunk(t*8);
  drunk(t*4);
  drunk(t/2);
  delay(t*4); 

  drunk(t/2);

  delay(t*4); 
  Otto.walk(2, t*2, 1);
  backyard(2,t*2);
  goingUp(t*2);
  goingUp(t*1);
  noGravity(t*2);
  Otto.crusaito(1, t*2, 20, 1);
  Otto.crusaito(1, t*8, 20, 1);
  Otto.crusaito(1, t*2, 20, 1);
  Otto.crusaito(1, t*8, 20, 1);
  Otto.crusaito(1, t*2, 20, 1);
  Otto.crusaito(1, t*3, 20, 1);

  delay(t);
  primera_parte();
    for (int i=0; i<32; i++){
    Otto.flapping(1, t/2, 20, 1);
    delay(t/2);
  }
  
  Otto.home();
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////PASOS DE BAILE////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void goingUp(int tempo){
  
      pause=millis();
      Otto.home();
      delay(tempo);
      Otto._moveSingle(80, 0);
      Otto._moveSingle(100, 1);
      delay(tempo);
      Otto._moveSingle(70, 0);
      Otto._moveSingle(110, 1);
      delay(tempo);
      Otto._moveSingle(60, 0);
      Otto._moveSingle(120, 1);
      delay(tempo);
      Otto._moveSingle(50, 0);
      Otto._moveSingle(130, 1);
      delay(tempo);
      Otto._moveSingle(40, 0);
      Otto._moveSingle(140, 1);
      delay(tempo);
      Otto._moveSingle(30, 0);
      Otto._moveSingle(150, 1);
      delay(tempo);
      Otto._moveSingle(20, 0);
      Otto._moveSingle(160, 1);
      delay(tempo);
      
      while(millis()<pause+8*t);

}

void primera_parte(){
  
  int move1[4] = {60,120,90,90};
  int move2[4] = {90,90,90,90};
  int move3[4] = {40,140,90,90};
  
  for(int x=0; x<3; x++){
    for(int i=0; i<3; i++){
      lateral_fuerte(1,t/2);
      lateral_fuerte(0,t/4);
      lateral_fuerte(1,t/4);
      delay(t);
    }
  
    pause=millis();
    Otto.home();
    Otto._moveServos(t*0.4,move1);
    Otto._moveServos(t*0.4,move2);
    while(millis()<(pause+t*2));
  }
  
  for(int i=0; i<2; i++){
    lateral_fuerte(1,t/2);
    lateral_fuerte(0,t/4);
    lateral_fuerte(1,t/4);
    delay(t);
  }
    
  pause=millis();
  Otto.home();
  Otto.crusaito(1, t*1.4, 20, 1);
  Otto._moveServos(t*1,move3);
  Otto.home();
  while(millis()<(pause+t*4));
}

void segunda_parte(){
  
  int move1[4] = {90,90,80,100};
  int move2[4] = {90,90,100,80};
  int move3[4] = {90,90,80,100};
  int move4[4] = {90,90,100,80};
    
  int move5[4] = {40,140,80,100};
  int move6[4] = {40,140,100,80};
  int move7[4] = {90,90,80,100};
  int move8[4] = {90,90,100,80};
       
  int move9[4] = {40,140,80,100};
  int move10[4] = {40,140,100,80};
  int move11[4] = {90,90,80,100};
  int move12[4] = {90,90,100,80};
  
  for(int x=0; x<7; x++){ 
    for(int i=0; i<3; i++){
      pause=millis();
      Otto._moveServos(t*0.15,move1);
      Otto._moveServos(t*0.15,move2);
      Otto._moveServos(t*0.15,move3);
      Otto._moveServos(t*0.15,move4);
      while(millis()<(pause+t));
    }
    pause=millis();
    Otto._moveServos(t*0.15,move5);
    Otto._moveServos(t*0.15,move6);
    Otto._moveServos(t*0.15,move7);
    Otto._moveServos(t*0.15,move8);
    while(millis()<(pause+t));
  }
 
  for(int i=0; i<3; i++){
    pause=millis();
    Otto._moveServos(t*0.15,move9);
    Otto._moveServos(t*0.15,move10);
    Otto._moveServos(t*0.15,move11);
    Otto._moveServos(t*0.15,move12);
    while(millis()<(pause+t));
  }
}

void lateral_fuerte(boolean side, int tempo){
  
  Otto.home();
  if (side) Otto._moveSingle(40, 0);
  else Otto._moveSingle(140, 1);
  delay(tempo/2);
  Otto._moveSingle(90, 0);
  Otto._moveSingle(90, 1);
  delay(tempo/2);

}

void drunk (int tempo){
  
  pause=millis();
  
  int move1[] = {60,70,90,90};
  int move2[] = {110,120,90,90};
  int move3[] = {60,70,90,90};
  int move4[] = {110,120,90,90};
  
  Otto._moveServos(tempo*0.235,move1);
  Otto._moveServos(tempo*0.235,move2);
  Otto._moveServos(tempo*0.235,move3);
  Otto._moveServos(tempo*0.235,move4);
  while(millis()<(pause+tempo));

}


void noGravity(int tempo){
  
  int move1[4] = {120,140,90,90};
  int move2[4] = {140,140,90,90};
  int move3[4] = {120,140,90,90};
  int move4[4] = {90,90,90,90};
  
  Otto.home();
  Otto._moveServos(tempo*2,move1);
  Otto._moveServos(tempo*2,move2);
  delay(tempo*2);
  Otto._moveServos(tempo*2,move3);
  Otto._moveServos(tempo*2,move4);

}

void kickLeft(int tempo){
  Otto.home();
  delay(tempo);
  Otto._moveSingle(50, 0);
  Otto._moveSingle(70, 1);
  delay(tempo);
  Otto._moveSingle(80, 0);
  Otto._moveSingle(70, 1);
  delay(tempo/4);
  Otto._moveSingle(30, 0);
  Otto._moveSingle(70, 1);
  delay(tempo/4);
  Otto._moveSingle(80, 0);
  Otto._moveSingle(70, 1);
  delay(tempo/4);
  Otto._moveSingle(30, 0);
  Otto._moveSingle(70, 1);
  delay(tempo/4);
  Otto._moveSingle(80, 0);
  Otto._moveSingle(70, 1);
  delay(tempo);
}
void kickRight(int tempo){
Otto.home();
  delay(tempo);
  Otto._moveSingle(110, 0);
  Otto._moveSingle(130, 1);
  delay(tempo);
  Otto._moveSingle(110, 0);
  Otto._moveSingle(100, 1);
  delay(tempo/4);
  Otto._moveSingle(110, 0);
  Otto._moveSingle(150, 1);
  delay(tempo/4);
 Otto._moveSingle(110, 0);
  Otto._moveSingle(80, 1);
  delay(tempo/4);
  Otto._moveSingle(110, 0);
  Otto._moveSingle(150, 1);
  delay(tempo/4);
  Otto._moveSingle(110, 0);
  Otto._moveSingle(100, 1);
  delay(tempo);
}

void backyard(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)};  
    for(int i=0;i<steps;i++)Otto.oscillateServos(A,O, T, phase_diff,1);
}

void backyardSlow(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)}; 
    for(int i=0;i<steps;i++)Otto.oscillateServos(A,O, T, phase_diff,1);
}
void flapping2(int steps, int T){
    int A[4]= {15, 15, 8, 8};
    int O[4] = {-A[0], A[1], 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(90), DEG2RAD(-90)};
    
    for(int i=0;i<steps;i++)Otto.oscillateServos(A,O, T, phase_diff,1);
}

