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


class Autodocking {

public:
char robotMotion;



decode_results results;

byte big_hole = 0x10;
byte small_hole = 0x11;
byte stop_sgnal = 0x20;

bool LB = false;   //"left" hole get signal from "big" hole
bool LS = false;
bool RB = false;
bool RS = false;
bool stop = false;

unsigned long lastMilli = 0;

void startAutoDocking();
void decodeIRsingnal();
};
