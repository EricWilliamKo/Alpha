#include "Arduino.h"
#include <IRremote.h>

#define Slow_turn_Right 'A'
#define Slow_turn_Left 'B'
#define Slow_go_Staright 'C'
#define Fast_turn_Right 'D'
#define Fast_turn_Left 'E'
#define Fast_go_Stright 'F'
#define Stop 'G'
#define Nothing 'H'

#define LOOPTIME 100

char robotMotion;

IRrecv irrecv2(6);
IRrecv irrecv3(5);

decode_results results;

byte big_hole = 0x10;
byte small_hole = 0x11;
byte stop_sgnal = 0x20;

bool LB = false; //"left" hole get signal from "big" hole
bool LS = false;
bool RB = false;
bool RS = false;
bool stop = false;

unsigned long lastMilli = 0;

void setup() {
	Serial.begin(115200);
	irrecv2.enableIRIn();
	irrecv3.enableIRIn();
	robotMotion = Nothing;
}

void loop() {

	if (irrecv2.decode(&results)) {
//		Serial.print("L ");
//		Serial.print(results.value, HEX);
		if (results.value == big_hole)
			LB = true;
		if (results.value == small_hole)
			LS = true;
		if (results.value == stop_sgnal)
			stop = true;
		irrecv2.resume();
	}

	if (irrecv3.decode(&results)) {
//		Serial.print("R ");
//		Serial.print(results.value, HEX);
		if (results.value == big_hole)
			RB = true;
		if (results.value == small_hole)
			RS = true;
		if (results.value == stop_sgnal)
			stop = true;
		irrecv3.resume();
	}

	if ((millis() - lastMilli) >= LOOPTIME) {
		lastMilli = millis();
		if (stop) {
			robotMotion = Stop;
		} else if (LS || RS) {
			if (LS)
				robotMotion = Slow_turn_Right;
			if (RS)
				robotMotion = Slow_turn_Left;
			if (RS && LS)
				robotMotion = Slow_go_Staright;
		} else if (LB || RB) {
			if (LB)
				robotMotion = Fast_turn_Right;
			if (RB)
				robotMotion = Fast_turn_Left;
			if (LB && RB)
				robotMotion = Fast_go_Stright;
		} else {
			robotMotion = Nothing;
		}
		Serial.print(robotMotion);
		robotMotion = Nothing;
		RS = false;
		LS = false;
		RB = false;
		LB = false;
		stop = false;
	}
}
