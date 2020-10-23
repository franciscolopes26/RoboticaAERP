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

int lastPosition = 0;
int sensorValues[8];

//=======================================================================
int Sonar(int trigPin, int echoPin)
{

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(50);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.

  return ( pulseIn(echoPin, HIGH));
}

double converte(long duration) {
  // Convert the time into a distance
  return ((duration / 2) / 29.1);  // Divide by 29.1 or multiply by 0.0343

}


//=====================================================================


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

  Serial.print("\n _________________________________________________ \n");
  Serial.print("                Sonar Test \n");
  while (one.readButton() == 0) {
    // -------------------Sonar test---------------
    long distance, tempo;
    tempo = Sonar(trigPin_1, echoPin_1);
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
  }

  Serial.print("\n _________________________________________________ \n");
  Serial.print("                 Line Test \n");

  while (one.readButton() != 0) {}

  while (one.readButton() == 0) {
    //Read values from the 8 sensors
    //Ler o valor dos 8 sensores

    for (int i = 0; i < 8; i++) {
      sensorValues[i] = one.readAdc(i);

    }
    
    for (int i = 0; i < 8; i++) {
      Serial.print(sensorValues[i]);  Serial.print(" ");
      delay(5);
    }
    calcularPosicao();
    delay(1
    000);
    
   
    //Serial.println(lastPosition);
  }

  Serial.print("\n _________________________________________________ \n");
  Serial.print("                Motors Test \n");

  while (one.readButton() != 0) {}

  Serial.print("    Forward \n"); // print data on LCD line 2
  one.stop();               // Stop Motors
  one.move(100, 100);         // Forward
  delay(1000);              // wait 1 second

  Serial.print("     Stop   \n");
  one.stop();               // Stop Motors
  delay(500);

  Serial.print("   Backwards \n");
  one.move(-100, -100);       // Backwards
  delay(1000);

  Serial.print("     Stop   \n");
  one.stop();               // Stop Motors
  delay(500);


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
