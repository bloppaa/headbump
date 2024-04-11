const int rojo = 13;
const int verde = 12;
const int azul = 11;
const int trig = 5;
const int echo = 6;

void setup() {
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);         
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);         
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration / 59;  // Dividido por 59 para convertir microsegundos en centímetros
  
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 70) {
    digitalWrite(rojo, HIGH);
    digitalWrite(verde, LOW);
    digitalWrite(azul, LOW);
  } else if (distance < 100) {
    digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
  } else {
    digitalWrite(rojo, LOW);
    digitalWrite(verde, LOW);
    digitalWrite(azul, HIGH);
  }

  
  delay(450);  // Retardo opcional para reducir la velocidad de actualización del sensor
}