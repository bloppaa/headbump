#include <ArduinoMqttClient.h>
#include <WiFi101.h>
#include "arduino_secrets.h"
#include <LiquidCrystal.h>

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
const int port = 1883;
const char topic[] = "bloppa/height";

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int red = 6;
const int green = 7;
const int blue = 8;
const int trig = 9;
const int echo = 10;

void setup() {
  // Initialize serial and wait for port to open
  Serial.begin(9600);
  while (!Serial);

  // Attempt to connect to Wifi network
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // Failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // Attempt to connect to MQTT broker
  Serial.print("Attempting to connect to MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed. Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

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
  mqttClient.poll();

  long duration, distance;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);         
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);         
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  // Microseconds to centimeters
  distance = duration / 59;
  
  if (distance < 3000) {
    long distance2 = duration / 59;
    delay(275);
    if (distance + 5 > distance2 && distance - 5 < distance2) {
      if (distance < 20) {
        delay(250);
        long distance3 = duration / 59;
        if (distance + 5 > distance3 && distance - 5 < distance3) {
          digitalWrite(red, 102);
          digitalWrite(green, LOW);
          digitalWrite(blue, LOW);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Choca");
          lcd.setCursor(0, 1);
          lcd.print("Distancia:");
          lcd.setCursor(10, 1);
          lcd.print(distance);
          lcd.setCursor(14, 1);
          lcd.print("cm");
        }
      } else if (distance < 90) {
        digitalWrite(red, LOW);
        digitalWrite(green, 51);
        digitalWrite(blue, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Permitido");
        if (distance > 70) {
          lcd.setCursor(10,0);
          lcd.print("CAR");
        } else if(distance > 50) {
          lcd.setCursor(10, 0);
          lcd.print("MTRUK");
        } else {
          lcd.setCursor(10, 0);
          lcd.print("TRUK");
        }
        lcd.setCursor(0, 1);
        lcd.print("Distancia:");
        lcd.setCursor(10, 1);
        lcd.print(distance);
        lcd.setCursor(14, 1);
        lcd.print("cm");
      } else {
        digitalWrite(red, 0);
        digitalWrite(green, 0);
        digitalWrite(blue, 1);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Asfalto");
        lcd.setCursor(0, 1);
        lcd.print("Distancia:");
        lcd.setCursor(10, 1);
        lcd.print(distance);
        lcd.setCursor(14, 1);
        lcd.print("cm");
      }
    }
    // Send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(distance);
    mqttClient.endMessage();
  }
}
