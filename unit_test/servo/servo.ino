//using esp32, ESP32Servo.h, not using Servo.h
//connect S to GPIO 26

#include <ESP32Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600);
  myservo.attach(26);
}
void loop() {
  myservo.write(0);
  delay(5000);
  myservo.write(90);
  delay(5000);
}