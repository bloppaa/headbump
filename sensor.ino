// include the library code:
#include <LiquidCrystal.h>

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int rojo = 6;
const int verde = 7;
const int azul = 8;
const int trig = 9;
const int echo = 10;
void setup() 
{
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

	// Clears the LCD screen
	lcd.clear();
  Serial.begin(9600);
}

void loop() 
{
  long duration, distance;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);         
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);         
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration / 59;  // Dividido por 59 para convertir microsegundos en centímetros
  

  if (distance < 50) {
    digitalWrite(rojo, HIGH);
    digitalWrite(verde, LOW);
    digitalWrite(azul, LOW);
  	lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Choca");
    lcd.setCursor(0,1);
    lcd.print("Distancia:");
    lcd.setCursor(10,1);
    lcd.print(distance);
    lcd.setCursor(14,1);
    lcd.print("cm");
  } else if (distance < 150) {
    digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Permitido");
    lcd.setCursor(0,1);
    lcd.print("Distancia:");
    lcd.setCursor(10,1);
    lcd.print(distance);
    lcd.setCursor(14,1);
    lcd.print("cm");
  }
   else if (distance > 3000){
    return;
   }
   
   else {
    digitalWrite(rojo, LOW);
    digitalWrite(verde, LOW);
    digitalWrite(azul, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Nada");
    lcd.setCursor(0,1);
    lcd.print("Distancia:");
    lcd.setCursor(10,1);
    lcd.print(distance);
    lcd.setCursor(14,1);
    lcd.print("cm");
  }

  
  delay(50);  // Retardo opcional para reducir la velocidad de actualización del sensor
}