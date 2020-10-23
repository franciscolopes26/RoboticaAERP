#include <SPI.h>                   // required by BnrOne.cpp
#include <BnrOneA.h>
#include <EEPROM.h>    // EEPROM reading and writing

//Constants definitions
//Definir constantes

#define SSPIN  2    //Slave Select (SS) pin for SPI communication
#define M1  1       //Motor1
#define M2  2       //Motor2
BnrOneA one;        //declaration of object variable to control the Bot'n Roll ONE

float batmin = 10.5; // safety voltage for discharging the battery

#define echoPin_3 5    // Echo Pin  (Right)
#define trigPin_3 10    // Trigger Pin  (Right)
#define echoPin_2 8    // Echo Pin  (Center)
#define trigPin_2 9    // Trigger Pin (Center)
#define echoPin_1 6    // Echo Pin  (Left)
#define trigPin_1 7    // Trigger Pin (Left)
int sensorValues[8];






int SumLeft = 0;
int SumRight = 0;
int SumDifference = 0;
int last = 0;

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
  
  //sonar 
  long distance, tempo1,tempo ;
  bool a= true ;
  
    tempo1 = Sonar(trigPin_1, echoPin_1);
    distance = converte(tempo);
    Serial.print(distance);
    Serial.print("\t");
    delay (5);

    tempo = Sonar(trigPin_2, echoPin_2);
    distance = converte(tempo);
    Serial.print(distance);
    Serial.print("\t");
    delay (5);

    tempo = Sonar(trigPin_3, echoPin_3);
    distance = converte(tempo);
    Serial.println(distance);
    delay(5);

    if(tempo <= 30)
  }

  
  //linha 

  
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = one.readAdc(i);
  }

  for (int i = 0; i < 8; i++) {
    Serial.print(sensorValues[i]);  Serial.print(" ");
    //delay(5);
  }

  if (sensorValues[3] > 350 || sensorValues[4] > 350)//sensor 3 e 4
  {
    one.move(40, 40);
  }
  if (sensorValues[2] > 350)//sensor 2
  {
    one.move(-10, 40);
  }
  if (sensorValues[1] > 350)//sensor 1
  {
    one.move(-20, 40);
  }
  if (sensorValues[0] > 350)//sensor 0
  {
    one.move(-30, 40);
  }
  if (sensorValues[5] > 350)//sensor 6
  {
    one.move(40, -10);
  }
  if (sensorValues[6] > 350)//sensor 7
  {
    one.move(40, -20);
  }
  if (sensorValues[7] > 350) // sensor 8
  {
    one.move(40, -30);
  }

  
}
