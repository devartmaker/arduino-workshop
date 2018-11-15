#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mp3Serial(2, 3);
DFRobotDFPlayerMini mp3;

boolean active = false;
float sensorValue = 1023.0f;

void setup() {
  Serial.begin(9600);

  for (int i = 3; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }

  mp3Serial.begin(9600);
  mp3.begin(mp3Serial);
  Serial.begin(9600);

  mp3.volume(25);
}

void loop() {
  int value = analogRead(A0);
  sensorValue += (value - sensorValue) * 0.1;
  Serial.println(sensorValue);

  if (!active && sensorValue < 500) {
    active = true;

    turnOnLED();

    mp3.next();
  } else if (active && sensorValue > 800) {
    active = false;

    turnOffLED();

    mp3.stop();
  }

  delay(10);
}

void turnOnLED() {
  for (int i = 4; i <= 12; i++) {
    digitalWrite(i, HIGH);
  }
}

void turnOffLED() {
  for (int i = 4; i <= 12; i++) {
    digitalWrite(i, LOW);
  }
}
