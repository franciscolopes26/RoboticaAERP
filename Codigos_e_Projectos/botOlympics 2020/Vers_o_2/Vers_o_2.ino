#include <SPI.h>                   // required by BnrOne.cpp
#include <BnrOneA.h>
#include <EEPROM.h>    // EEPROM reading and writing
#include <Ultrasonic.h>

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

Ultrasonic sonar1(trigPin_1, echoPin_1);
Ultrasonic sonar2(trigPin_2, echoPin_2);
Ultrasonic sonar3(trigPin_3, echoPin_3);

void setup() {
  // put your setup code here, to run once:
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

/*void sensordireita2(){//feito
  long tempo2 = sonar2.timing();
  float  distancia2 = sonar2.convert(tempo2, Ultrasonic::CM);
  
if(distancia2>=0 && distancia2<=20){ 
  Serial.print("     Stop   \n");
  one.stop();               // Stop Motors
  delay(500);
 // Serial.print("   Backwards \n");
  //one.move(-100, -100);       // Backwards
  //delay(1000);
}

else if(distancia2>20) 
{
 Serial.print("    Forward \n"); // print data on LCD line 2
  //one.stop();               // Stop Motors
  one.move(100, 100);         // Forward
  //delay(1000);              // wait 1 second 
}
float  distancia1 = sonar1.convert(tempo, Ultrasonic::CM);
}*/

/*void sensoresquerda3(){//feito
  long tempo3 = sonar3.timing();
  float  distancia3 = sonar3.convert(tempo3, Ultrasonic::CM);
  
if(distancia3>=0 && distancia3<=20){ 
  Serial.print("     Stop   \n");
  one.stop();// Stop Motors
  delay(500); 
 // Serial.print("   Backwards \n");
  //one.move(-100, -100);       // Backwards
  //delay(1000);
}

else if(distancia3>20) 
{
  Serial.print("    Forward \n"); // print data on LCD line 2
  //one.stop();               // Stop Motors
  one.move(100, 100);         // Forward
  //delay(1000);              // wait 1 second 
}
}*/

/*void sensormeio1(){
  long tempo = sonar1.timing();
  float  distancia1 = sonar1.convert(tempo, Ultrasonic::CM);
  
if(distancia1>=0 && distancia1<=20){  
  Serial.print("     Stop   \n");
  one.stop();               // Stop Motors
  delay(500); 
  sensordireita2();
}

else if(distancia1>20) 
{
 Serial.print("    Forward \n"); // print data on LCD line 2
 one.move(100, 100);         // Forward
 
}
}*/
 void loop() 
 {


  // put your main code here, to run repeatedly:  

    long tempo3 = sonar3.timing();//tempo meio
    float  distanciaesquerda = sonar3.convert(tempo3, Ultrasonic::CM);//sensor meio
  
    long tempo2 = sonar2.timing();//tempo meio
    float  distanciameio2 = sonar2.convert(tempo2, Ultrasonic::CM);//sensor meio
    
<    long tempo1 = sonar1.timing();//esquerda
    float  distanciadireita1= sonar1.convert(tempo1, Ultrasonic::CM);//esquerda
    
    
   if(distanciameio2>=0 && distanciameio2<=45)//meio
   {
    
    Serial.print(distanciadireita1);
    one.brake(100,100);
      if(distanciadireita1>=0&&distanciadireita1<=45)
      {
        Serial.print(distanciadireita1);
        one.move(100,0);
      }
      if(distanciaesquerda>=0&&distanciaesquerda<=45){
         Serial.print(distanciaesquerda);
        one.move(0,100);
        
      }
      }
   
   else{
      one.move(60,60);
    
   }

   
}
   
  
   
