/*
 * Author: Maclsk
 * Latest update: 11/01/18
 * Version: 2.1
 * Description: Controlls the WS2812B LED chain in the perodic table display. Accepts commands via Bluetooth from an android phone app.
 * Note: Reddit user u/LincageMap and I were working on very similar projects at the same time so shared some ideas!
 *       I borrowed some of his ideas and code for the temperature, density, discovory and electronegativity features.
 *   
 */

/*
 * Things to do:
 * -
 * -commenting
 * 
 * -
 * 
 * Possible optimisations
 * -Change allgroups function to use allmetals and allnonmetals functions
 * -Get rid of hydrogen in groupColours (saves 3 bytes)
 */

#include <avr/pgmspace.h>
#include "FastLED.h" //Importing the fastLED library https://github.com/FastLED/FastLED/releases

#define NUM_LEDS 90 //Defining that there are 90 LEDs in the display
#define DATA_PIN 9 //The data wire from the LED chain connects to D9 on the Arduino

CRGB leds[NUM_LEDS]; //Array for the LED colours
char inSerial[15];   //bluetooth commands get turned into character array for parsing                                                                                        

CRGB ledColumns[18]; //Array for some animations
byte hue = 0; //For color fades

//These arrays store the proton numbers of the elements that belong to different groups. PROGMEM used to store arrays in flash memory to free up RAM
//The rest of the groups are lit up through selectColumn and selectElement functions.
const byte transitionMetals[38] PROGMEM = {21,22,23,24,25,26,27,28,29,30,39,40,41,42,43,44,45,46,47,48,72,73,74,75,76,77,78,79,80,104,105,106,107,108,109,110,111,112};
const byte metals[11] PROGMEM = {13,31,49,50,81,82,83,113,114,115,116};
const byte semimetals[7] PROGMEM = {5,14,32,33,51,52,84};
const byte nonmetals[7] PROGMEM = {1,83, 98, 99,100, 113,111};
const byte radioactive[23] PROGMEM = {43,83,84,85,86,87,88,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118};                                                               
const byte Metais_Alcalinos[7]PROGMEM ={2,3,4,5,6,7,8};
const byte Metais_Alcalino_Terrosos[6]PROGMEM={9,10,11,12,13,14};
const byte Semimetais[7]PROGMEM={82,84,85,96,95,102,103};
const byte Halogenios[8]PROGMEM={114,113,112,111,110,109,108,107};
const byte Outros_metais[5]PROGMEM={81,80,79,78,77};
const byte Metais_de_transicao[]PROGMEM={};


//Stores RGB colours. It is easier to play with colours when they are all together like this rather than looking for them through the code.               
//                                 R    G    B          
const byte groupColours[19][3] = {{220, 0,   220},  //Hydrogen          (0)
                                  {63,  0,   255},  //Alkali            (1)
                                  {21,  150, 0},    //Alkali Earth      (2)
                                  {255, 255, 255},  //Lanthanides       (3)
                                  {200, 255, 0},    //Actinides         (4)
                                  {0,   255, 135},  //Transition Metals (5)
                                  {137, 53,  255},  //Metals            (6)
                                  {0,   0,   255},  //Semi Metals       (7)
                                  {255, 255, 0},    //Non Metal         (8)
                                  {255, 150, 0},    //Halogen           (9)
                                  {200, 0,   0},    //Noble Gas         (10)
                                  {255, 255, 0},    //Radioactive       (11)
                                  {220, 0,   220},  //icosagens         (12)
                                  {63,  0,   255},  //crystallogens     (13)
                                  {21,  150, 0},    //pnictogens        (14)
                                  {0,   255, 135},  //chalcogens        (15)
                                  {0,   255, 0},    //solids            (16)
                                  {0,   0,   255},  //liquids           (17)
                                  {255, 255, 0}};   //gasses            (18)
                                        
//Converts proton number to led index; protons[i] where i is proton number gives correct led position; eg the led for helium = proton[2] = led 35;
//proton number            1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57 58 59 60 61 62 63 64 65 66 67 68 69 70 71  72  73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118  
const byte protons[] = {0, 0,35, 1, 2,29,30,31,32,33, 34, 4,  3,  28, 27, 26, 25, 24, 23, 5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 54, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 89, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72};

//Stores locations of the elements on the table.                          
const byte matrix[7][18] = {{ 1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  110},
                            { 2, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 77, 78, 93, 94,  109, 111},
                            { 3, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76, 79, 92, 95,  108, 112},
                            { 4, 11, 14, 16, 27, 28, 39, 40, 51, 52, 63, 64, 75, 80, 91, 96,  107, 113},
                            { 5, 10, 15, 17, 26, 29, 38, 41, 50, 53, 62, 65, 74, 81, 90, 97,  106, 114},
                            { 6, 9,   0, 18, 25, 30, 37, 42, 49, 54, 61, 66, 73, 82, 89, 98,  105, 115}};
                            { 7, 9,   0, 19, 24, 31, 36, 43, 48, 55, 60, 67, 72, 83, 88, 99,  104, 116}};
                            { 0, 0,   0, 20, 23, 32, 35, 44, 47, 56, 59, 68, 71, 84, 87, 100, 103, 117}};
                            { 0, 0,   0, 21, 22, 33, 34, 45, 46, 57, 58, 69, 70, 85, 86, 101, 102, 118}};

//Stores the 
const uint8_t collection[26] PROGMEM = {51,28,20,54,83,52,29,16,14,95,53,12,23,30,80,6,42,24,1,49,13,10,82,31,22,79};

//Data for determining what state elements are in at dif
const uint8_t solid[118] PROGMEM = {0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
const uint16_t melting[118] PROGMEM = {14,  10000,  454,  1560,  2348,  3823,  63,  55,  54,  25,  371,  923,  933,  1687,  317,  388,  172,  84,  337,  1115,  1814,  1941,  2183,  2180,  1519,  1811,  1768,  1728,  1358,  693,  303,  1211,  1090,  494,  266,  116,  312,  1050,  1799,  2128,  2750,  2896,  2430,  2607,  2237,  1828,  1235,  594,  430,  505,  904,  723,  387,  161,  302,  1000,  1193,  1071,  1204,  1294,  1373,  1345,  1095,  1586,  1629,  1685,  1747,  1770,  1818,  1092,  1936,  2506,  3290,  3695,  3459,  3306,  2739,  2041,  1337,  234,  577,  601,  544,  527,  575,  202,  10000,  973,  1323,  2023,  1845,  1408,  917,  913,  1449,  1618,  1323,  1173,  1133,  1800,  1100,  1100,  1900,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000};
const uint16_t boiling[118] PROGMEM = {20,  4,  1615,  2743,  4273,  4300,  77,  90,  85,  27,  1156,  1363,  2792,  3173,  554,  718,  239,  87,  1032,  1757,  3103,  3560,  3680,  2944,  2334,  3134,  3200,  3186,  3200,  1180,  2477,  3093,  887,  958,  332,  120,  961,  1655,  3618,  4682,  5017,  4912,  4538,  4423,  3968,  3236,  2435,  1040,  2345,  2875,  1860,  1261,  457,  165,  944,  2143,  3737,  3633,  3563,  3373,  3273,  2076,  1800,  3523,  3503,  2840,  2973,  3141,  2223,  1469,  3675,  4876,  5731,  5828,  5869,  5285,  4701,  4098,  3129,  630,  1746,  2022,  1837,  1235,  10000,  211,  10000,  2010,  3473,  5093,  4273,  4200,  4273,  3503,  2284,  3383,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000};

//const double density[118] PROGMEM = {8.99e-05,  0,  0.54,  1.85,  2.46,  2.26,  0,  0,  0,  0,  0.97,  1.74,  2.7,  2.33,  1.82,  1.96,  0,  0,  0.86,  1.55,  2.99,  4.51,  6.11,  7.14,  7.47,  7.87,  8.9,  8.91,  8.92,  7.14,  5.9,  5.32,  5.73,  4.82,  3.12,  0,  1.53,  2.63,  4.47,  6.51,  8.57,  10.28,  11.5,  12.37,  12.45,  12.02,  10.49,  8.65,  7.31,  7.31,  6.7,  6.24,  4.94,  0.01,  1.88,  3.51,  6.15,  6.69,  6.64,  7.01,  7.26,  7.35,  5.24,  7.9,  8.22,  8.55,  8.8,  9.07,  9.32,  6.57,  9.84,  13.31,  16.65,  19.25,  21.02,  22.59,  22.56,  21.09,  19.3,  13.53,  11.85,  11.34,  9.78,  9.2,  -1,  0.01,  -1,  5,  10.07,  11.72,  15.37,  19.05,  20.45,  19.82,  13.67,  13.51,  14.78,  15.1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}; //actual density - not used
const uint8_t density[118] = {0, 0, 6, 20, 27, 25, 0, 0, 0, 0, 10, 19, 30, 26, 20, 22, 0, 0, 9, 17, 33, 50, 68, 80, 84, 88, 100, 100, 100, 80, 66, 60, 64, 54, 35, 0, 17, 29, 50, 73, 96, 116, 129, 139, 140, 135, 118, 97, 82, 82, 75, 70, 55, 0, 21, 39, 69, 75, 74, 79, 81, 82, 59, 89, 92, 96, 99, 102, 105, 74, 111, 150, 187, 217, 237, 255, 254, 238, 217, 152, 133, 128, 110, 103, 127, 0, 127, 56, 113, 132, 173, 215, 230, 223, 154, 152, 166, 170, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127}; //density mapped to 0-255 for heatmap

const uint16_t discovered[118] PROGMEM = {1766,  1868,  1817,  1798,  1807,  0,  1772,  1774,  1670,  1898,  1807,  1808,  0,  1854,  1669,  0,  1774,  1894,  1807,  0,  1876,  1791,  1803,  0,  1774,  0,  0,  1751,  0,  1746,  1875,  1886,  0,  1817,  1826,  1898,  1861,  1790,  1794,  1789,  1801,  1778,  1937,  1827,  1803,  1803,  0,  1817,  1863,  0,  0,  1782,  1811,  1898,  1860,  1808,  1839,  1803,  1885,  1885,  1947,  1853,  1901,  1880,  1843,  1886,  1878,  1842,  1879,  1878,  1907,  1923,  1802,  1783,  1925,  1803,  1803,  0,  0,  0,  1861,  0,  0,  1898,  1940,  1900,  1939,  1898,  1899,  1828,  1913,  1789,  1940,  1940,  1944,  1944,  1949,  1950,  1952,  1952,  1955,  1957,  1961,  1969,  1967,  1974,  1976,  1984,  1982,  1994,  1994,  1996,  2003,  1998,  2003,  2000,  2010,  2002};

void setup() {
  delay(2000); // sanity check delay - allows reprogramming if accidently blowing power w/leds
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  randomSeed(analogRead(0)); //random seed for future random functions
  Serial.begin(9600); //set up serial line with bluetooth module
  set_max_power_in_volts_and_milliamps(5, 4000); //sets the max power of the display
  
  allElements(); //initialize display with all groups
}

void loop(){
  String state = "";//to store the command
  delayMicroseconds(1);
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.readString(); // Reads the data from the serial port
    Serial.println("Command: " + String(state));
    state.toCharArray(inSerial, 15);
    checkCommands(inSerial);
  }
}     

//When a command is recieved this function is called.
//The function compares its known commands to the one recieved. If there is a match
void checkCommands(char inStr[]){
  Serial.print("Check commands: ");
  Serial.println(inStr);
  if(!strcmp(inStr,"1")) progressiveScan(); 
  if(!strcmp(inStr,"allgroups")) allElements();
  if(!strcmp(inStr,"alkali")){FastLED.clear(); selectAlkali();}
  if(!strcmp(inStr,"alkaliearth")){FastLED.clear(); selectColumn(1,2);}
  if(!strcmp(inStr,"lanthanides")){FastLED.clear(); selectElement(71);} //lights up lanthanides
  if(!strcmp(inStr,"actinides")){FastLED.clear(); selectElement(103);} //lights up actinides
  if(!strcmp(inStr,"transition")){FastLED.clear(); selectTransition();}
  if(!strcmp(inStr,"metals")){FastLED.clear(); selectMetals();}
  if(!strcmp(inStr,"semimetals")){FastLED.clear(); selectSemimetals();}
  if(!strcmp(inStr,"nonmetals")){FastLED.clear(); selectNonmetals();}
  if(!strcmp(inStr,"halogen")){FastLED.clear(); selectColumn(16,9);}
  if(!strcmp(inStr,"noblegas")){FastLED.clear(); selectColumn(17,10);}
  if(!strcmp(inStr,"coinage")){FastLED.clear(); selectColumn(10,10);}
  if(!strcmp(inStr,"volatile")){FastLED.clear(); selectColumn(11,11);}
  if(!strcmp(inStr,"icosagens")){FastLED.clear(); selectColumn(12,12);}
  if(!strcmp(inStr,"crystal")){FastLED.clear(); selectColumn(13,13);}
  if(!strcmp(inStr,"pnictogens")){FastLED.clear(); selectColumn(14,14);}
  if(!strcmp(inStr,"chalcogens")){FastLED.clear(); selectColumn(15,15);}
  if(!strcmp(inStr,"allmetals")){FastLED.clear(); selectAllMetals();}
  if(!strcmp(inStr,"allnonmetals")){FastLED.clear(); selectAllNonMetals();}
  if(!strcmp(inStr,"radioactive")){FastLED.clear(); selectRadioactive();}
  if(!strcmp(inStr,"collection")){FastLED.clear(); displayCollection();}
  if(isPrefix(inStr, "temper") > 0) displayTemps(isPrefix(inStr, "temper"));
  if(isPrefix(inStr, "discovered") > 0) displayDiscoveryYear(isPrefix(inStr, "discovered")); 
  if(isPrefix(inStr, "select") > 0){FastLED.clear(); selectElement(isPrefix(inStr, "select"));}
}

//Set an led(picked by specifying the proton number) to an RGB colour
void setColourRgb(byte protonNumber, unsigned int red, unsigned int green, unsigned int blue){
  if(protonNumber == 0) return; //0 is used as placeholder in the preriodic table matrix so just exit function
  if(protons[protonNumber] == 0 && protonNumber != 1) return; //lanthanides and actinides have a default pixelIndex of 0. This conflicts with hydrogen. If 0 default occurs and element is not hydrogen (1), exit function.
  leds[protons[protonNumber]].setRGB(green,red,blue);
 }
 
//Light up every cell - Colour coded to display element groups
void allElements(){
  FastLED.clear();
  selectAlkali();
  selectColumn(1,2);
  selectElement(71);
  selectElement(103);
  selectTransition();
  selectMetals();
  selectSemimetals();
  selectNonmetals();
  selectColumn(16,9);
  selectColumn(17,10);
}

//lights up alkali group
void selectAlkali(){
  for(byte i = 1; i < 7; i++){
    setColourRgb(matrix[i][0], groupColours[1][0], groupColours[1][1], groupColours[1][2]);
    FastLED.show();
    delay(15);
  }
}

//lights up transition metal group
void selectTransition(){
  Serial.println("TranAition"); 
  for(byte i = 0; i < 38; i++){                                              
    setColourRgb(pgm_read_word_near(transitionMetals + i), groupColours[5][0], groupColours[5][1], groupColours[5][2]); //Sets each element to its correct colour based on its group
    FastLED.show();
    delay(15);
  }
}

//lights up metals group
void selectMetals(){
  Serial.println("MetAls");
  for(byte i = 0; i < 11; i++){                                                 
    setColourRgb(pgm_read_word_near(metals + i), groupColours[6][0], groupColours[6][1], groupColours[6][2]); //Sets each element to its correct colour based on its group
    FastLED.show();
    delay(15);
  }
}

//lights up semi-metals group
void selectSemimetals(){
  for(byte i = 0; i < 7; i++){                                                
    setColourRgb(pgm_read_word_near(semimetals + i), groupColours[7][0], groupColours[7][1], groupColours[7][2]); //Sets each element to its correct colour based on its group
    FastLED.show();
    delay(15);
  }
}

//lights up non-metals group
void selectNonmetals(){
  for(byte i = 0; i < 7; i++){                                                
    setColourRgb(pgm_read_word_near(nonmetals + i), groupColours[8][0], groupColours[8][1], groupColours[8][2]); //Sets each element to its correct colour based on its group
    FastLED.show();
    delay(15);
  }
}

//lights up every radioactive group
void selectRadioactive(){
  FastLED.clear();
  for(byte i = 0; i < 23; i++){                                       
    setColourRgb(pgm_read_word_near(radioactive + i), groupColours[11][0], groupColours[11][1], groupColours[11][2]); //Sets each element to its correct colour based on its group
    FastLED.show();
    delay(15);
  }
}

//lights up a column(group) based on a given column number. Colouring chosen by given "groupColours" number.
void selectColumn(byte column, byte colour){
  for(byte i = 0; i < 7; i++){                                             
    setColourRgb(matrix[i][column], groupColours[colour][0], groupColours[colour][1], groupColours[colour][2]); //Sets each element to its correct colour based on its group
    FastLED.show();
    delay(15);
  }
}

//lights up all metals
void selectAllMetals(){
  selectAlkali();
  selectColumn(1,2);
  selectElement(71);
  selectElement(103);
  selectTransition();
  selectMetals();
  selectSemimetals();
}

//lights up all non-metals
void selectAllNonMetals(){
  selectNonmetals();
  selectColumn(16,9);
  selectColumn(17,10);
  selectSemimetals();
}

//lights up a single element or compound
void selectElement(byte protonNumber){
  if(protonNumber == 119){ //water
    setColourRgb(1,255,255,255); //hydrogen
    setColourRgb(8,255,255,255); //oxygen
  }
  else if(protonNumber == 120){ //table salt
    setColourRgb(11,255,255,255); //sodium
    setColourRgb(17,255,255,255); //chlorine
  }
  else if(protonNumber == 121){ //humans
    setColourRgb(8,255,255,255); //oxygen
    setColourRgb(6,255,255,255); //carbon
    setColourRgb(1,255,255,255); //hydrogen
    setColourRgb(7,255,255,255); //nitrogen
    setColourRgb(20,255,255,255); //calcium
    setColourRgb(15,255,255,255); //phosphorus
  }
  else if(protonNumber == 122){ //the sun
    setColourRgb(1,255,255,255); //hydrogen
    setColourRgb(2,255,255,255); //helium
  }
  else if(protonNumber == 123){ //earth
    setColourRgb(26,255,255,255); //iron
    setColourRgb(8,255,255,255); //oxygen
    setColourRgb(14,255,255,255); //silicon
    setColourRgb(12,255,255,255); //magnesium
  }
  else if(protonNumber == 124){ //the atmosphere
    setColourRgb(7,255,255,255); //nitrogen
    setColourRgb(8,255,255,255); //oxygen
    setColourRgb(18,255,255,255); //argon
  }
  else if(protonNumber == 125){ //bronze
    setColourRgb(27,255,255,255); //copper
    setColourRgb(50,255,255,255); //tin
  }
  else if(protonNumber == 126){ //brass
    setColourRgb(27,255,255,255); //copper
    setColourRgb(30,255,255,255); //zinc
  }
  else if(protonNumber == 127){ //steel
    setColourRgb(26,255,255,255); //iron
    setColourRgb(6,255,255,255); //carbon
  }
  else if(protonNumber == 128){ //stainless steel
    setColourRgb(26,255,255,255); //iron
    setColourRgb(24,255,255,255); //chromium
    setColourRgb(28,255,255,255); //nickel
  }
  else if(protonNumber == 129){ //plant
    setColourRgb(11,255,255,255); //oxygen
    setColourRgb(17,255,255,255); //carbon
    setColourRgb(11,255,255,255); //hydrogen
    setColourRgb(17,255,255,255); //nitrogen
    setColourRgb(11,255,255,255); //phosphorus
    setColourRgb(17,255,255,255); //potassium
  }
  else{
    setColourRgb(protonNumber,255,255,255);
  }
  FastLED.show();
  delay(15);
}

//densist elements are brightest
void displayDensityHeatmap(){
  for(int i = 0; i < 118; i++){ //go through all elements
    leds[protons[i+1]] = CHSV(160, 255, map(density[i],0,255,80,255)); //light up based on brightness
    if(density[i] == 127){ //bit hacky here - 127 was not used so using it for the elements with unknown densitys
      setColourRgb(i+1,0,0,0); //if unknown density - turn led off
    }
  }
  setColourRgb(1,0,0,0); //hydrogen is buggy - force it to black (very low density)
  FastLED.show();
}

//not yet created
void displayElectronegativityHeatmap(){
  
}

//uses collection array to light up current collection
void displayCollection(){
  FastLED.clear();
  for(int i = 0; i < 26; i++){
    setColourRgb(pgm_read_word_near(collection + i), 0, 255, 0);
    FastLED.show();
    delay(192);
  }
}

//lights up all elements discovered before the year that you give the function
void displayDiscoveryYear(int year){ //1600 to 2018
  FastLED.clear();
  for(int i = 0; i < 118; i++){ //go through all elements
    if(pgm_read_word_near(discovered + i) <= year){
      setColourRgb(i+1, 100, 0, 255);
    }
    else{
      setColourRgb(i+1, 0, 0, 0); //if element not discovered, turn off
    }
  }
  FastLED.show();
}

//colour code elements based on their state at a temperature you give it in Kelvins (K)
void displayTemps(int selectedTemp){ //0 to 6000
  //Serial.println("TEMP: " + String(selectedTemp));
  for(int i = 0; i < 118; i++){
    if(pgm_read_word_near(boiling + i) <= selectedTemp){ //gas state
      setColourRgb(i+1, groupColours[18][0], groupColours[18][1], groupColours[18][2]);
    }
    else if(pgm_read_word_near(melting + i) <= selectedTemp){ //liquid state
      setColourRgb(i+1, groupColours[17][0], groupColours[17][1], groupColours[17][2]);
    }
    else if(pgm_read_word_near(solid + i) == 0){ //solid state
      setColourRgb(i+1, groupColours[16][0], groupColours[16][1], groupColours[16][2]);
    }
    else{
      setColourRgb(i+1,0,0,0); //for states that don't exist or unknown states - turn off
    }
  }
  FastLED.show();
}

//checks command for command prefix. If prefix exists - returns suffix.
//eg command = select24, prefix = select, suffix = 24
//only works with up to 4 digit suffixes. Prefix must be 6 digits.
int isPrefix(char command[], char prefix[]){
  //get length of character array
  int commandLength = 0;
  while(command[commandLength] != '\0'){ 
    commandLength++;
  }
  Serial.println("command length: " + String(commandLength));
  
  //compare letters
  for(int i = 0; i < 6; i++){
    if(command[i] != prefix[i]) return 0;
  }
  
  //work out suffix of command
  int suffix = 0;
  switch(commandLength){
    case 8: //one digit
      suffix = command[commandLength-2]-48;
      break;
    case 9: //two digits
      suffix = (command[commandLength-3]-48)*10;
      suffix += command[commandLength-2]-48;
      break;
    case 10: //three digits
      suffix = (command[commandLength-4]-48)*100;
      suffix += (command[commandLength-3]-48)*10;
      suffix += command[commandLength-2]-48;
      break;
    case 11: //four digits
      suffix = (command[commandLength-5]-48)*1000;
      suffix += (command[commandLength-4]-48)*100;
      suffix += (command[commandLength-3]-48)*10;
      suffix += command[commandLength-2]-48;
      break;
  }
  Serial.println("Suffix: " + String(suffix));
  return suffix;
}

//-------------------------- animations -------------------------------//

//progressively lights up rows - random colour
void progressiveScan(){
  byte randRed = 255;
  byte randGreen = 255;
  byte randBlue = 255;

  while(randRed + randGreen + randBlue > 250){
    randRed = random(0, 255);
    randGreen = random(0, 255);
    randBlue = random(0, 255);
  }
  
  for(byte i = 1; i <= 56; i++){
    setColourRgb(i, randRed, randGreen, randBlue);
    FastLED.show();
    delay(60);
  }
  for(byte i = 71; i <= 88; i++){
    setColourRgb(i, randRed, randGreen, randBlue);
    FastLED.show();
    delay(60);
  }
  for(byte i = 103; i <= 118; i++){
    setColourRgb(i, randRed, randGreen, randBlue);
    FastLED.show();
    delay(60);
  }
}

//progressively lights up columns - random colour
void progressiveScanVertical(){
  byte randRed = 255;
  byte randGreen = 255;
  byte randBlue = 255;

  while(randRed + randGreen + randBlue > 250){
    randRed = random(0, 255);
    randGreen = random(0, 255);
    randBlue = random(0, 255);
  }

  for(int i = 0; i < 18; i++){
    for(int j = 0; j < 7; j++){
      if(matrix[j][i] != 0){
        setColourRgb(matrix[j][i], randRed, randGreen, randBlue);
      }
      FastLED.show();
      delay(60);
    }
  }
}

//fades entire display through rainbow
void rainbowFade(){
  static uint8_t hue = random(0, 255);
  FastLED.showColor(CHSV(hue++, 255, 255)); 
  delay(50);
}

//cycles rainbow colours horizontally.
void rainbowCycle(){
  fill_rainbow(ledColumns, 18, hue);
  hue++;
  for(int x = 0; x < 18; x++){
    for(int y = 0; y < 7; y++){
      //protons[matrix[y][x]];
      leds[protons[matrix[y][x]]] = ledColumns[x];
      if(protons[matrix[y][x]] == 0){
        leds[0] = ledColumns[0];
      }
    }
  }
  FastLED.show();
  delay(10);
}
