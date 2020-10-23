#define rxpin 2
#define txpin 3

SoftwareSerial blue(rxpin, txpin);

void setup() {
  // put your setup code here, to run once:
  blue.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (blue.available()) {
    opcao = blue.read();
    Serial.println(opcao);
  }


}
