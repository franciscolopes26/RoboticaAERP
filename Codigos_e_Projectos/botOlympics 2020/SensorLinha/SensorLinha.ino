#include <SPI.h>                   // required by BnrOne.cpp
#include <BnrOneA.h>
#include <EEPROM.h>    // EEPROM reading and writing
#include <DRV8835MotorShield.h>

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

int lastPosition = 0;
int sensorValues[8];
int lastError = 0;
#define Kp 0.05 // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd 2 // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define rightMaxSpeed 200 // max speed of the robot
#define leftMaxSpeed 200 // max speed of the robot
#define rightBaseSpeed 150 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 150  // this is the speed at which the motors should spin when the robot is perfectly on the line

DRV8835MotorShield motors; //don't forget this line






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


  Serial.print(" ================================================= \n");
  Serial.print(" ================================================= \n");
  Serial.print("\n                Botn'Roll Test  \n");
  Serial.print(" ================================================= \n");
  Serial.print(" ================================================= \n");
}

void loop() {

 


//  Serial.print("\n _________________________________________________ \n");
//  Serial.print("                Sonar Test \n");
//  while (one.readButton() == 0) {
//    // -------------------Sonar test---------------
//    long distance, tempo;
//    tempo = Sonar(trigPin_1, echoPin_1);
//    distance = converte(tempo);
//    Serial.print(distance);
//    Serial.print("\t");
//    delay (5);
//
//    tempo = Sonar(trigPin_2, echoPin_2);
//    distance = converte(tempo);
//    Serial.print(distance);
//    Serial.print("\t");
//    delay (5);
//
//    tempo = Sonar(trigPin_3, echoPin_3);
//    distance = converte(tempo);
//    Serial.println(distance);
//    delay(5);
//  }

  Serial.print("\n _________________________________________________ \n");
  Serial.print("                 Line Test \n");

  while (one.readButton() != 0) {}

  while (one.readButton() == 0) {
    //Read values from the 8 sensors
    //Ler o valor dos 8 sensores

  unsigned int sensors[8];
  //int position = qtr.readLineBlack(sensors); // get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.
  int error = lastPosition - 3500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  int rightMotorSpeed = rightBaseSpeed + motorSpeed;
  int leftMotorSpeed = leftBaseSpeed - motorSpeed;
  
    if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive
  
   
  // move forward with appropriate speeds

  motors.setM1Speed(rightMotorSpeed); //only works if "DRV8835MotorShield motors;" is defined in the beginning
  motors.setM2Speed(leftMotorSpeed);
  


    for (int i = 0; i < 8; i++) {
      sensorValues[i] = one.readAdc(i);

    }
    
    for (int i = 0; i < 8; i++) {
      Serial.print(sensorValues[i]);  Serial.print(" ");
      delay(5);
    }
    calcularPosicao();
    delay(1000);
        
  }

}


void calcularPosicao() {
  bool onLine = false;
  uint32_t avg = 0; // this is for the weighted total
  uint16_t sum = 0; // this is for the denominator, which is <= 64000

  for (int i = 0; i < 8; i++)
  {
    uint16_t value = sensorValues[i];

    // keep track of whether we see the line at all
    if (value > 200) {
      onLine = true;
    }

    // only average in values that are above a noise threshold
    if (value > 50)
    {
      avg += (uint32_t) value * (i * 1000);
      sum += value;
    }
  }
 
  if (!onLine)
  {
    // If it last read to the left of center, return 0.
    if (lastPosition < (8 - 1) * 1000 / 2)
    {
      lastPosition = 0;
    }
    // If it last read to the right of center, return the max.
    else
    {
      lastPosition = (8 - 1) * 1000;
    }
  }

  lastPosition = avg / sum;
 Serial.println(lastPosition);
}
