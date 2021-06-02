#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

const int sensor = A5;
const int lamp = 13;
const int mp3_rx = 2;
const int mp3_tx = 3;

int value = 1023;
boolean touched = false;
boolean state;
unsigned long stopTime = 0;

SoftwareSerial mp3Serial(mp3_rx, mp3_tx);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);

  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }

  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(25); 

  pinMode(lamp, OUTPUT);
}

void loop() {
  check();
  delay(10);
}

void check() {
  int raw = analogRead(sensor);
  value += (raw - value) * 0.1;

  // Serial.println(value);

  if (!touched && value < 500) {
    touched = true;
    digitalWrite(lamp, HIGH);
    
    if (millis() - stopTime > 3000) {
      myDFPlayer.next();
    } else {
      myDFPlayer.start();
    }
  } else if (touched && value >= 700) {
    touched = false;
    digitalWrite(lamp, LOW);
    
    myDFPlayer.pause();
    stopTime = millis();
  }
}
