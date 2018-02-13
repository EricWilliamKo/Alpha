#include "PID.h"
#define encoder0PinA  2
#define encoder0PinB  3

#define In1 6
#define In2 5

#define MC33926Enable 7

#define LOOPTIME 40
#define MaxPWM 255

static double MaxSpeed = 25;

int pinAState = 0;
int pinAStateOld = 0;
int pinBState = 0;
int pinBStateOld = 0;

char commandArray[3];


volatile long Encoderpos = 0;
volatile long unknownvalue = 0;

volatile int lastEncoded = 0;
unsigned long lastMilli = 0;                    // loop timing
unsigned long lastRecvMilli = 0;
long dT = 0;
unsigned long cc = 0;

double omega_target = 0.0;
//double omega_target = 2.0;
double omega_actual = 0.0;

int PWM_val = 0;                                // (25% = 64; 50% = 127; 75% = 191; 100% = 255)
int CPR = 64;                                   // encoder count per revolution
int gear_ratio = 56;							//166RPM->30   80RPM->56
int actual_send = 0;
int target_receive = 0;

int analogPin = A0;
unsigned int current = 0;

PID pid;

typedef union{
	double Data;
	byte binary[4];
}SerialDataDouble;

typedef struct{
	SerialDataDouble Kp;
	SerialDataDouble Ki;
	SerialDataDouble Kd;
}SerialDataParamPack;
