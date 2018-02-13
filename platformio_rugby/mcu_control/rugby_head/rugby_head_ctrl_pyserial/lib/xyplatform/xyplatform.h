/*
 * =====================================================================================
 *
 *       Filename:  xyplatform.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2017年1月11日
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Team Power Rangers
 *        Company:  Advanced Robotics Corporation
 *
 * =====================================================================================
 */
#ifndef xyplatform_h
#define xyplatform_h

#if ARDUINO >= 100
        #include "Arduino.h"
#else
        #include "WProgram.h"
#endif
#endif

//#define ANDBOT 1
#define Rugby 2
//#define ANGELBOT 3

/* =====================================================================================
 * Rugby Head Control Parameters
 * ===================================================================================*/
#define ENABLE true
#define DISABLE false

/* =====================================================================================
 * RS232 protocol
 * ===================================================================================*/
#define FbreqCode 99
#define SetDrvCode 51
#define Sendpacksize 8
#define SendDrvParampacksize 13
#define RecvDrvpacksize 11
/* =====================================================================================
 * End of RS232 protocol
 * ===================================================================================*/

#ifdef Rugby
#define Pan_axis 0 // SLAVE 0
#define Tilt_axis 1 // SLAVE 1
#define Umax_volt 12
#define Umin_volt -12
#define MaxPWMDutyCycle 255 /*! duty cycle (25% = 64; 50% = 127; 75% = 191; 100% = 255) */
// STATE MACHINE (Driver)
#define INIT_STATE 100
#define HOME_STATE 101
#define FRONTLIMIT_STATE 102
#define BACKLIMIT_STATE 103
#define WORKING_RANGE_STATE 104
// STATE MACHINE (Ctrl)
#define WAIT4INIT_STATE 001
#define NORMAL_STATE 002
#define IDLE_STATE 003
#endif

// Motor control mode
#define _OpenLoop 0
#define _Current 1
#define _Velocity 2
#define _Position 3
#define CTRLMode _Position
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
	double angularVelSense;
	unsigned int currentSense;
	double angularPos;
	int DRVSTATE;
	int PWM_val;
	int ID;
}DataMeasure;

typedef struct{
	SerialDataDouble Kp;
	SerialDataDouble Ki;
	SerialDataDouble Kd;
}SerialDataParamPack;
/* =====================================================================================
 * End of Data type declaration
 * ===================================================================================*/

class xyPlatform {
	public:
		typedef struct{
			int axis;
			int mode;
			int code;
			bool drvStatus;
			double VoltCmd;
			double CurrentCmd;
			double VelocityCmd;
			double PositionCmd;
			double Kp;
			double Ki;
			double Kd;
		}ParamPack;

		typedef struct{
			int MAXVoltCmd;
			int MINVoltCmd;
		}SetCmdLimit;

		union Data_Setting{
			struct _ByteSet {
			  byte L;
			  byte H;
			} Byte;
			int Data;
		};

		DataMeasure Fb;
		ParamPack *DrvCtrl;
		void SendCmd();
		void SerialGetDriverData(DataMeasure* Fb);
		void Enable();
		void Disable();
		void SetDrvParam();
		void SerialSend2Drv(byte* sendData, int DataSize);
		void SerialBegin(long int baudrate);
		xyPlatform(ParamPack* Ctrl_param,SetCmdLimit* Limit);
	private:
		byte getData[8];
		SetCmdLimit *InputLimit;
};
