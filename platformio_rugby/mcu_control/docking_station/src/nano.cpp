#include "Arduino.h"
#include <IRremote.h>

#define navi 0 //guiding robot to the station
#define charging 1 //charging mode

IRsend irsend;

bool status;
byte navi_small = 0x11; //Code of small hole
byte navi_big = 0x10; //Code of big hole
byte charging_both = 0x20; //Code of stoping


void setup()
{
  Serial.begin(9600);
  status = navi;
  pinMode(5, INPUT);
}

void loop() {

  status = digitalRead(5);

  if (status == navi) {
    irsend.sendRC5(navi_big, 8 , 10);
    delay(40);
    irsend.sendRC5(navi_small, 8, 9);
    delay(40);
    Serial.print(status);
  }

  if (status == charging) {
    irsend.sendRC5(charging_both, 8 , 10);
    delay(40);
    irsend.sendRC5(charging_both, 8, 9);
    delay(40);
    Serial.print(status);
  }
}
