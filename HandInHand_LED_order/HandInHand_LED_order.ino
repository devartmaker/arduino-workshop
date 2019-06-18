#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "SimpleTimer.h"

#define LED_DELAY 50

SoftwareSerial mp3Serial(2, 3); //RX, TX
DFRobotDFPlayerMini mp3;
SimpleTimer timer;

boolean active = false;
float sensorValue = 1023.0f;
int current;

void setup() {
  Serial.begin(9600);

  for (int i = 4; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }

  mp3Serial.begin(9600);
  mp3.begin(mp3Serial);

  mp3.volume(25);
  timer.setInterval(10, checkSensor);
  timer.setInterval(LED_DELAY, changeLED);
}

void loop() {
  timer.run();
}

void checkSensor() {
  int value = analogRead(A0);
  sensorValue += (value - sensorValue) * 0.1;
  Serial.println(sensorValue);

  if (!active && sensorValue < 500) {
    active = true;
    mp3.next();
  } else if (active && sensorValue > 800) {
    active = false;
    mp3.stop();
  }
}

void changeLED() {
  current++;
  if (current > 9) {
    current = 0;
  }

  for (int i = 4; i <= 12; i++) {
    if (active && i - 4 == current) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}
