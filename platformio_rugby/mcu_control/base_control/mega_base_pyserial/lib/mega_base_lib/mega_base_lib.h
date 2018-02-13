#include "Arduino.h"
#define MaxSpeed 25
#define LOOPTIME 10
#define RIGHT_WHEEL 1
#define LEFT_WHEEL 2
#define BatteryObserverTime 500
#define TimeOut 5000 //TimeOut = Max.Distance(cm) * 58
#define FeedBackTime 30

typedef union{
	double Data;
	byte binary[4];
}SerialDataDouble;

typedef union{
	float Data;
	byte binary[4];
}SerialDataFloat;

typedef struct{
	SerialDataDouble Kp;
	SerialDataDouble Ki;
	SerialDataDouble Kd;
}SerialDataParamPack;


float omega_left_target = 0.0;
float omega_right_target = 0.0;
float omega_left_actual = 0;
float omega_right_actual = 0;
float omega_L_drv = 0;
float omega_R_drv = 0;
int pwm_L = 0;
int pwm_R = 0;
int kp_L = 0;
int kp_R = 0;

SerialDataFloat yaw;

unsigned long lastMilli = 0;
unsigned long lastRecvMilli = 0;
unsigned long lastBool = 0;
unsigned long lastFeedBack = 0;

unsigned int current_left = 0;
unsigned int current_right = 0;

char rec;
bool recieveComplete = false;
String recbuf;
String feedback;
int voltage = 0;
int current = 0;
bool connected;
bool disableProtection = false;


bool checksum(float firstpara, float secondpara, float sum);
bool left_motor_protection(unsigned int current);
bool right_motor_protection(unsigned int current);
void disableCurrentProtect(bool cmd);
