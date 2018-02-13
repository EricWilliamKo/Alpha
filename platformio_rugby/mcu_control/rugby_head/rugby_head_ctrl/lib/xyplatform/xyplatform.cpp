#include "Arduino.h"
#include "xyplatform.h"

xyPlatform::xyPlatform(ParamPack* Ctrl_param, SetCmdLimit* Limit)
{
	DrvCtrl = Ctrl_param;
	InputLimit = Limit;
}

void xyPlatform::SendCmd()
{
	byte sendData[Sendpacksize];// = {123,0,0,0,0,125}; //{"{", 0, 0, 0, 0, "}"}
	sendData[0] = '{';
	SerialDataInt SendCode;
	SerialDataDouble CmdRef;

	SendCode.Data = DrvCtrl->code;
	sendData[1] = SendCode.binary[0];
	sendData[2] = SendCode.binary[1];

	switch (DrvCtrl->mode) // To be continue
	{
	case _OpenLoop:
		if 		(DrvCtrl->axis == Pan_axis)		{CmdRef.Data = DrvCtrl->VoltCmd;}
		else if (DrvCtrl->axis == Tilt_axis)  	{CmdRef.Data = -DrvCtrl->VoltCmd;}
		break;
	case _Current:
		break;
	case _Velocity:
		break;
	case _Position:
		if 		(DrvCtrl->axis == Pan_axis)		{CmdRef.Data = DrvCtrl->PositionCmd;}
		else if (DrvCtrl->axis == Tilt_axis)  	{CmdRef.Data = -DrvCtrl->PositionCmd;}
		break;
	}

	for (int i = 3; i <= 6; i++){
		sendData[i] = CmdRef.binary[i-3];
	}
	if 		(DrvCtrl->drvStatus == ENABLE)  	{sendData[Sendpacksize-1] = '}';} //send stop byte motor on
	else if (DrvCtrl->drvStatus == DISABLE)  	{sendData[Sendpacksize-1] = '|';} //send stop byte motor off
	SerialSend2Drv(sendData, Sendpacksize);
}
void xyPlatform::SetDrvParam() //To be continue
{
	byte sendData[SendDrvParampacksize];//{"{", 0, 0, 0, 0, "}"}
	SerialDataInt SetDrvMode;
	SerialDataParamPack SendPID;

	sendData[0] = '{';

	SetDrvMode.Data = DrvCtrl->mode;
	sendData[1] = SetDrvMode.binary[0];
	sendData[2] = SetDrvMode.binary[1];

	SendPID.Kp.Data = DrvCtrl->Kp;
	SendPID.Ki.Data = DrvCtrl->Ki;
	SendPID.Kd.Data = DrvCtrl->Kd;

	for (int i = 3; i <= 6; i++){
		sendData[i] = SendPID.Kp.binary[i-3];
	}
	for (int i = 7; i <= 10; i++){
		sendData[i] = SendPID.Ki.binary[i-7];
	}
	for (int i = 11; i <= 14; i++){
		sendData[i] = SendPID.Kd.binary[i-11];
	}
	sendData[SendDrvParampacksize-1] = '}'; //send stop byte motor on
	SerialSend2Drv(sendData, SendDrvParampacksize);
}
void xyPlatform::SerialGetDriverData(DataMeasure* Fb)
{
	byte getData[RecvDrvpacksize-1];
	SerialDataDouble tmpData;
	SerialDataInt PWM_val;
	SerialDataInt DRVSTATE;
	if (DrvCtrl->axis == Pan_axis)
	{
		if (Serial1.available())
		{
			if(Serial1.read() == '{')
			{
				Serial1.readBytes(getData,(RecvDrvpacksize-1));
			}
			else;
		}
		else;
	}
	else if (DrvCtrl->axis == Tilt_axis)
	{
		if (Serial2.available())
		{
			if(Serial2.read() == '{')
			{
				Serial2.readBytes(getData,(RecvDrvpacksize-1));
			}
			else;
		}
		else;
	}

	if(getData[RecvDrvpacksize-2] == '}')
	{
		for (int i = 0; i <= 3; i++){
			tmpData.binary[i] = getData[i];
		}
		if (DrvCtrl->axis == Pan_axis)			{Fb->angularPos = tmpData.Data;}
		else if (DrvCtrl->axis == Tilt_axis)  	{Fb->angularPos = -tmpData.Data;}
		Fb->currentSense = getData[4] * 80;//max current is 20400mA, 255 * 80 = 20400mA
		DRVSTATE.binary[0] = getData[5];
		DRVSTATE.binary[1] = getData[6];
		Fb->DRVSTATE = DRVSTATE.Data;
		PWM_val.binary[0] = getData[7];
		PWM_val.binary[1] = getData[8];
		Fb->PWM_val = PWM_val.Data;
	}
	else;
}

void xyPlatform::Enable()
{
	byte sendData[Sendpacksize] ;//{123,~,125}; //{"{", 0, 0, 0, 0, "}"}
	SerialDataInt SendCode;
	SerialDataDouble CmdRef;
	sendData[0] = '{';
	SendCode.Data = DrvCtrl->code;
	sendData[1] = SendCode.binary[0];
	sendData[2] = SendCode.binary[1];

	CmdRef.Data = 0.0;
	for (int i = 3; i <= 6; i++){
			sendData[i] = CmdRef.binary[i-3];
		}
	sendData[Sendpacksize-1] = '}';
	SerialSend2Drv(sendData, Sendpacksize);
    DrvCtrl->drvStatus = ENABLE;
}
void xyPlatform::Disable()
{
	byte sendData[Sendpacksize];// = {123,~,124}; //{"{", 0, 0, 0, 0, "}"}
	SerialDataInt SendCode;
	SerialDataDouble CmdRef;
	sendData[0] = '{';
	SendCode.Data = DrvCtrl->code;
	sendData[1] = SendCode.binary[0];
	sendData[2] = SendCode.binary[1];

	CmdRef.Data = 0.0;
	for (int i = 3; i <= 6; i++){
			sendData[i] = CmdRef.binary[i-3];
		}
	sendData[Sendpacksize-1] = '|';
	SerialSend2Drv(sendData, Sendpacksize);
    DrvCtrl->PositionCmd = 0.0;
    DrvCtrl->drvStatus = DISABLE;
}
void xyPlatform::SerialSend2Drv(byte* sendData, int DataSize)
{
	if 		(DrvCtrl->axis == Pan_axis)    	Serial1.write(sendData, DataSize);
	else if (DrvCtrl->axis == Tilt_axis) 	Serial2.write(sendData, DataSize);
}
void xyPlatform::SerialBegin(long int baudrate)
{
	if 		(DrvCtrl->axis == Pan_axis)    	Serial1.begin(baudrate);
	else if (DrvCtrl->axis == Tilt_axis) 	Serial2.begin(baudrate);
}
