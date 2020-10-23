#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Otto9.h> //-- Otto Library version 9
Otto9 Otto;  //This is Otto!

#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
int YL;
int YR;
int RL;
int RR;
double charRead;
void setup(){
    Otto.init(2, 3, 4, 5, true, A6, 13, 8, 9);
    Serial.begin(115200);
    YL = EEPROM.read(0);
    if (YL > 128) YL -= 256;
    YR = EEPROM.read(1);
    if (YR > 128) YR -= 256;
    RL = EEPROM.read(2);
    if (RL > 128) RL -= 256;
    RR = EEPROM.read(3);
    if (RR > 128) RR -= 256;
    Otto.home();
    Serial.println("OTTO CALIBRATION PROGRAM");
    Serial.println("PRESS a or z for adjusting Left Leg");
    Serial.println("PRESS s or x for adjusting Left Foot");
    Serial.println("PRESS k or m for adjusting Right Leg");
    Serial.println("PRESS j or n for adjusting Right Foot");
    Serial.println();
    Serial.println("PRESS f to test Otto walking");
    Serial.println("PRESS h to return servos to home position"); 
}

void loop(){

    if((Serial.available()) > (0)){
        charRead = Serial.read();
    }
    if(((charRead)==('a' ))){
        YL++;
        Otto.setTrims(YL,YR,RL,RR);
        calib_homePos();
        Otto.saveTrimsOnEEPROM();
    }else{
        if(((charRead)==( 'z' ))){
            YL--;
            Otto.setTrims(YL,YR,RL,RR);
            calib_homePos();
            Otto.saveTrimsOnEEPROM();
        }else{
            if(((charRead)==( 's' ))){
                RL++;
                Otto.setTrims(YL,YR,RL,RR);
                calib_homePos();
                Otto.saveTrimsOnEEPROM();
            }else{
                if(((charRead)==( 'x' ))){
                    RL--;
                    Otto.setTrims(YL,YR,RL,RR);
                    calib_homePos();
                    Otto.saveTrimsOnEEPROM();
                }else{
                    if(((charRead)==( 'k' ))){
                        YR++;
                        Otto.setTrims(YL,YR,RL,RR);
                        calib_homePos();
                        Otto.saveTrimsOnEEPROM();
                    }else{
                        if(((charRead)==( 'm' ))){
                            YR--;
                            Otto.setTrims(YL,YR,RL,RR);
                            calib_homePos();
                            Otto.saveTrimsOnEEPROM();
                        }else{
                            if(((charRead)==( 'j' ))){
                                RR++;
                                Otto.setTrims(YL,YR,RL,RR);
                                calib_homePos();
                                Otto.saveTrimsOnEEPROM();
                            }else{
                                if(((charRead)==( 'n' ))){
                                    RR--;
                                    Otto.setTrims(YL,YR,RL,RR);
                                    calib_homePos();
                                    Otto.saveTrimsOnEEPROM();
                                }else{
                                    if(((charRead)==( 'f' ))){
                                        Otto.walk(1,1000,1);
                                    }else{
                                        if(((charRead)==( 'h' ))){
                                            Otto.home();
                                        }else{
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } 
}

void calib_homePos() {
  int servoPos[4];
  servoPos[0]=90;
  servoPos[1]=90;
  servoPos[2]=90;
  servoPos[3]=90;
  Otto._moveServos(500, servoPos);
  Otto.detachServos();
}
