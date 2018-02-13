#include "autodocking.h"

IRrecv irrecv2(2);
IRrecv irrecv3(3);

void Autodocking::startAutoDocking(){
  irrecv2.enableIRIn();
	irrecv3.enableIRIn();
	robotMotion = Nothing;
}

void Autodocking::decodeIRsingnal(){
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
}

// void update
