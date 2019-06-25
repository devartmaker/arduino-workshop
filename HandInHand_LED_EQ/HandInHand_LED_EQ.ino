#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "SimpleTimer.h"

const int sensor = A0;
const int mp3_rx = 2;
const int mp3_tx = 3;

int value = 1023;
boolean touched = false;
boolean state;
unsigned long stopTime = 0;
int current = 0;

SoftwareSerial mp3Serial(mp3_rx, mp3_tx);
DFRobotDFPlayerMini myDFPlayer;
SimpleTimer timer;

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

  for (int i = 4; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }
  
  timer.setInterval(10, checkSensor);
  timer.setInterval(35, changeLED);
}

void loop() {
  timer.run();
}

void checkSensor() {
  int raw = analogRead(sensor);
  value += (raw - value) * 0.1;

  Serial.println(value);

  if (!touched && value < 500) {
    touched = true;

    if (millis() - stopTime > 2000) {
       myDFPlayer.next();
    } else {
       myDFPlayer.start();
    }
  } else if (touched && value >= 700) {
    touched = false;
    
    myDFPlayer.pause();
    stopTime = millis();
  }
}

void changeLED() {
  if (touched) {
    int r = random(10);
    if (current < r && random(10) < 5) {
      current = r;
    } else {
      current--;
      if (current < 0) current = 0;
    }
  } else {
    current = 0;
  }

  for (int i = 4; i <= 12; i++) {
    if (i - 4 < current) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}
