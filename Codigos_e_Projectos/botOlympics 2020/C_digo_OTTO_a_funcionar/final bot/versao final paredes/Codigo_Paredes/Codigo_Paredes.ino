//#include <Ultrasonic.h>
#include <SPI.h>                   // required by BnrOne.cpp
#include <BnrOneA.h>
#include <EEPROM.h>    // EEPROM reading and writing
#include "pitches.h"

#define velocidadeSeguirEmFrente 40


//Constants definitions
//Definir constantes

#define SSPIN  2    //Slave Select (SS) pin for SPI communication
#define M1  1       //Motor1
#define M2  2       //Motor2
BnrOneA one;        //declaration of object variable to control the Bot'n Roll ONE

float distancia1, distancia2, distancia3;

float batmin = 10.5; // safety voltage for discharging the battery

#define echoPin_3 5    // Echo Pin  (Right)
#define trigPin_3 10    // Trigger Pin  (Right)
#define echoPin_2 8    // Echo Pin  (Center)
#define trigPin_2 9    // Trigger Pin (Center)
#define echoPin_1 6    // Echo Pin  (Left)
#define trigPin_1 7    // Trigger Pin (Left)

#include "funcoes.h"

bool direita = true;
void setup() {


  //setup routines
  Serial.begin(9600);     // set baud rate to 57600bps for printing values in serial monitor. Press (ctrl+shift+m) after uploading
  one.spiConnect(SSPIN);   // start the SPI module
  one.minBat(batmin);      // safety voltage for discharging the battery
  one.stop();              // stop motors
  one.lcd1(" Bot'n Roll ONE");
  one.lcd2(" Sensor Reading");
  delay(1000);


  // SONAR setup
  pinMode(trigPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);

}

void loop() {

    if( direita ){
      movimentoParaDireita();
    }else{
      movimentoParaEsquerda();
    }

//  if ( detetouLinha() ) {
//    direita = !direita;
//    tone(9, NOTE_C4, 1);
//  }
}
