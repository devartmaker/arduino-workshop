#include <SoftwareSerial.h>
#include "Adafruit_NeoPixel.h"
#include "DFRobotDFPlayerMini.h"

Adafruit_NeoPixel strip(24, 4, NEO_GRB + NEO_KHZ800);

const int sensor = A5;
const int lamp = 13;
const int mp3_rx = 2;
const int mp3_tx = 3;
const int volume = A1;
int nowVolume;

int value = 1023;
boolean touched = false;
boolean state;
unsigned long stopTime = 0;

SoftwareSerial mp3Serial(mp3_rx, mp3_tx);
DFRobotDFPlayerMini myDFPlayer;

int rainbowIndex = 0;
int brightness = 0;

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A2, LOW);
  
  Serial.begin(9600);
  mp3Serial.begin(9600);

  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }

  myDFPlayer.setTimeOut(500);
  changeVolume(getVolume());

  pinMode(lamp, OUTPUT);

  strip.begin();
  strip.clear();
  strip.setBrightness(brightness);
  strip.show();
}

void loop() {
  rainbow();
  check();
  int v = getVolume();
  if (abs(nowVolume - v) > 2) {
    changeVolume(v);
  }
  delay(10);
}


int getVolume() {
  int v = analogRead(volume);
  v = map(v, 0, 1023, 1, 30);
  return v;
}

void changeVolume(int v) {
  nowVolume = v;
  myDFPlayer.volume(v);
}

void check() {
  int raw = analogRead(sensor);
  value += (raw - value) * 0.1;

  Serial.println(value);

  if (!touched && value < 500) {
    touched = true;
    digitalWrite(lamp, HIGH);

    if (millis() - stopTime > 3000) {
      myDFPlayer.next();
    } else {
      myDFPlayer.start();
    }
  } else if (touched && value >= 600) {
    touched = false;
    digitalWrite(lamp, LOW);

    myDFPlayer.pause();
    stopTime = millis();
  }
}

void rainbow() {
  if (touched) {
    brightness += 2;
    if (brightness > 255) brightness = 255;
  } else {
    brightness -= 5;
    if (brightness < 0) brightness = 0;
  }

  strip.setBrightness(brightness);
  
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((i + rainbowIndex) & 255));
  }
  strip.show();
  rainbowIndex++;
  if (rainbowIndex == 255) rainbowIndex = 0;
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
