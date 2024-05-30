#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int red = 6;
const int green = 7;
const int blue = 8;
const int trig = 9;
const int echo = 10;
const int azfalto = 13;
const bool autopasando = false;

void setup() {
  
  // Initialize serial and wait for port to open
  Serial.begin(115200);
  //while (!Serial);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  int seg = 0;
	// Clears the LCD screen
	lcd.clear();
}

void loop() {
  // Call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  //mqttClient.poll();

  int duration, distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);         
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);         
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  // Microseconds to centimeters
  distance = duration / 59;
  
  if (distance < 3000 and distance > 0) {
    int distance2 = duration / 59;
    if (distance + 1 > distance2 && distance - 1 < distance2) { 
      if (distance < 30) {
        delay(250);
        int distance3 = duration / 59;
        if (distance + 1 > distance3 && distance - 1 < distance3) {
          color(distance,"Choca", autopasando);
        }
      }
      else if (distance < 100) {
          if (distance < 50) {
            //enviar mensaje
            msg("auto", autopasando, distance);
          } else if(distance < 70) {
            //enviar mensaje
            msg("camioneta", autopasando, distance);
          } else {
          //enviar mensaje
            msg("Camion", autopasando, distance);
          }
          color(distance,"Permitido", autopasando);
        }
      else{
        color(distance,"Azfalto", autopasando);
      }
    }
    msgsinestado(distance);
  }
  delay(50);
}

void color(int distancia, String color, bool autopasando){
  lcd.clear();
    lcd.setCursor(0,1);  
    lcd.print("Distancia:");
    lcd.setCursor(10, 1);
    lcd.print(distancia);
    lcd.setCursor(14, 1);
    lcd.print("cm");
    if(color == "Azfalto"){
      digitalWrite(red, 182);
      digitalWrite(green, 0);
      digitalWrite(blue, 179);
      if(autopasando == true){
        autopasando = false;
      }
    }
    else if(color == "Permitido"){
      digitalWrite(red, 0);
      digitalWrite(green, 165);
      digitalWrite(blue, 182);
      if(autopasando == false){
        autopasando = true;
      }
    }
    else if(color == "Choca"){
      digitalWrite(red, 215);
      digitalWrite(green, 30);
      digitalWrite(blue, 0);
      if(autopasando == false){
        autopasando = true;
      }
    }
}
void msg(String tipo, bool estado, int distance){
  if(estado){
    Serial.println(String(distance));
  }
}
void msgsinestado(int distance){
  Serial.println(String(distance));
  
}