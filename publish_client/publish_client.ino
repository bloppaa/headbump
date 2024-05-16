#include <ArduinoMqttClient.h>
#include <WiFi101.h>
#include "arduino_secrets.h"

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
const int port = 1883;
const char topic[] = "bloppa/height";

const int buttonPin = 2;
const int ledPin = 13;

int buttonState = 0;
int prevButtonState = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
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
}

void loop() {
  // Call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && prevButtonState == HIGH) {
    // Record random value from A0
    int rand_value = analogRead(A0);

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(rand_value);

    // Send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(rand_value);
    mqttClient.endMessage();

    Serial.println();
  }

  prevButtonState = buttonState;
}
