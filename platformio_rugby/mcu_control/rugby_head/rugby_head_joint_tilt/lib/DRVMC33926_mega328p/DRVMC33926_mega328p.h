/*
 * =====================================================================================
 *
 *       Filename:  DRVMC33926_mega328p.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月02日 00時27分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Team Power Rangers
 *        Company:  Advanced Robotics Corporation
 *
 * =====================================================================================
 */
#ifndef DRVMC33926_mega328p_h
#define DRVMC33926_mega328p_h

#if ARDUINO >= 100
        #include "Arduino.h"
#else
        #include "WProgram.h"
#endif
#endif

/* =====================================================================================
 * Motor params
 * ===================================================================================*/

#define CPR 64
#define MaxSumError 6000
#define gear_ratio 270
//	#define MaxSpeed 31
const double ENC2DEG = 0.0208333333;//360/(double(CPR)*double(gear_ratio));
//DIO sensors
#define FrontLimitPin 9
#define BackLimitPin 8

/* =====================================================================================
 * End of Motor params
 * ===================================================================================*/

/* =====================================================================================
 * Pin assignment on Motor Driver
 * ===================================================================================*/
#define encoderPinA  3 /*! encoder A phrase */
#define encoderPinB  2 /*! encoder B phrase */

#define In1 6 /*! 328p -> MC33926 ; PWM */
#define In2 5 /*! 328p -> MC33926 ; PWM */
#define MC33926Enable 7 /*! 328p -> MC33926 ; Enable command*/

//current sense
#define currentFbPin A0
/* =====================================================================================
 * End of Pin assignment on Motor Driver
 * ===================================================================================*/
/* =====================================================================================
 * RS232 protocol
 * ===================================================================================*/
#define FbreqCode 99 //Normal
#define SetDrv 51
#define Recvpacksize 8
#define RecvDrvParampacksize 13
#define Sendpacksize 11
/* =====================================================================================
 * End of RS232 protocol
 * ===================================================================================*/

#define Umax_volt 12
#define Umin_volt -12
#define MaxPWM 255
#define _OpenLoop 0
#define _Current 1
#define _Velocity 2
#define _Position 3
#define ENABLE true
#define DISABLE false

/* =====================================================================================
 * End
 * ===================================================================================*/

/* =====================================================================================
 * Data type declaration
 * ===================================================================================*/
typedef union{
	double Data;
	byte binary[4];
}SerialDataDouble;

typedef union{
	int Data;
	byte binary[2];
}SerialDataInt;

typedef struct{
	SerialDataDouble Kp;
	SerialDataDouble Ki;
	SerialDataDouble Kd;
}SerialDataParamPack;

typedef struct{
	double Kp;
	double Ki;
	double Kd;
	double error;
	double sum_error;
}CtrlerPack;

typedef struct{
	double angularVel;
	unsigned int current;
	double angularPos;
	int DRVSTATE;
	int req;
	int PWM_val;
}DataPack;

typedef struct{
	bool Status;
	unsigned int mode;
	double Volt;
	double Ref;
	double CurrentRef;
	double VelocityRef;
	double PositionRef;
	int PWM_val; /*! (25% = 64; 50% = 127; 75% = 191; 100% = 255) */
	int code;
	CtrlerPack position;
}DrvCtrlPack;
/* =====================================================================================
 * End of Data type declaration
 * ===================================================================================*/

//declarations
void ReadCmd(DrvCtrlPack* Cmd);
void ReadParams(DrvCtrlPack* Cmd);
void sendFbData(DrvCtrlPack* Cmd,DataPack* Fb);
void doEncoder();
void getENC(DataPack* Fb);
void printTemple();
