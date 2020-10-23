//////////////////////////
//  DEFINIR PROTÓTIPOS  //
//////////////////////////
void leDistancias( float &dist1, float &dist2, float &dist3);
int Sonar(int trigPin, int echoPin);
double converte(long duration);
void seguirDireita();
void seguirEsquerda();
void virarEsquerda();
void virarDireita();
bool verificaSeExisteParedeEsquerda();
bool verificaSeExisteParedeDireita();
bool temParedeEmFrente();
void movimentoParaDireita();
void movimentoParaEsquerda();
bool detetouLinha();

///////////////
//  FUNCOES  //
///////////////
void leDistancias( float & dist1, float & dist2, float & dist3) {
  long tempo1, tempo2, tempo3;

  tempo1 = Sonar(trigPin_1, echoPin_1);
  dist1 = converte(tempo1);
  tempo2 = Sonar(trigPin_2, echoPin_2);
  dist2 = converte(tempo2);
  tempo3 = Sonar(trigPin_3, echoPin_3);
  dist3 = converte(tempo3);

  if (false) {
    Serial.print(dist1);
    Serial.print("\t");
    Serial.print(dist2);
    Serial.print("\t");
    Serial.println(dist3);
    Serial.print("\t");
  }
}


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

  return ( pulseIn(echoPin, HIGH, 10000));
}

double converte(long duration) {
  // Convert the time into a distance
  return ((duration / 2) / 29.1);  // Divide by 29.1 or multiply by 0.0343

}

void seguirDireita() {//seguir direita
  //leDistancias( distancia1, distancia2, distancia3);

  if (distancia3 > 8) {
    one.move(40, 8);//25,5
  }
  else if (distancia3 < 6) {
    one.move(8, 40);
  } else {
    one.move(velocidadeSeguirEmFrente, velocidadeSeguirEmFrente);
  }
}

bool temParedeEmFrente() {//parede frente
  //leDistancias( distancia1, distancia2, distancia3);
  if ( distancia2 > 12 )
    return false;
  return true;
}

void virarEsquerda() {//viraresq

  if (distancia1 < 10 && distancia2 > 20)
  {
    one.move(100, 100);
  } else
    one.move(-64, 64);
}

void virarDireita() {//virar dir

  if (distancia3 > 30 && distancia2 > 20)
  {
    one.move(100, 100);
  } else
    one.move(64, -64);
}

void virarDireitaSemParede() {//virar dir

  //if ((distancia3 > 30 && distancia2 > 20 && distancia1<10)||(distancia3 > 30 && distancia2 > 20 && distancia1>30))
  one.move(64, -64);


}

bool verificaSeExisteParedeDireita() {
  if ( distancia3 > 30 )
    return false;
  return true;
}

bool verificaSeExisteParedeEsquerda() {
  if ( distancia1 > 30 )
    return false;
  return true;
}

void seguirEsquerda() {
  //leDistancias( distancia1, distancia2, distancia3);
  if (distancia1 > 10) {
    one.move(8, 48);//25,5
  }
  else if (distancia1 < 8) {
    one.move(48, 8);
  } else {
    one.move(velocidadeSeguirEmFrente, velocidadeSeguirEmFrente);
  }
}

void movimentoParaEsquerda()
{
  leDistancias( distancia1, distancia2, distancia3);// adicionei aqui
  if (/*temParedeEmFrente() && */ !verificaSeExisteParedeEsquerda() )
    virarEsquerda();
  else if (temParedeEmFrente() &&  verificaSeExisteParedeEsquerda() )
    virarDireita();
  else
    seguirEsquerda();
}

void movimentoParaDireita()
{
  leDistancias( distancia1, distancia2, distancia3); // adicionei aqui
  if (/*temParedeEmFrente() && */ !verificaSeExisteParedeDireita() )
    virarDireita();
  else if (temParedeEmFrente() &&  verificaSeExisteParedeDireita() )
    virarEsquerda();
  else
    seguirDireita();


}
bool detetouLinha()
{
  bool detetaPreto = false;

  for (int i = 0; i < 8; i++) {
    if ( one.readAdc(i) > 300) {
      detetaPreto = true;
    }
  }

  return detetaPreto;
}
