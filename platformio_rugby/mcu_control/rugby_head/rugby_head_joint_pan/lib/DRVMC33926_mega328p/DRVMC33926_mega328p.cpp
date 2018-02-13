/*
 * =====================================================================================
 *
 *       Filename:  DRVvnh5019_mega328p.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月02日 00時27分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Weber-Hsu
 *        Company:  Advanced Robotics Corporation
 *
 * =====================================================================================
 */

#include "DRVMC33926_mega328p.h"

#include "Arduino.h"

/**
 * Read cmd [velocity] from mega2560.
 */
void ReadCmd(DrvCtrlPack* Cmd)
{
	SerialDataDouble recvFromUpperCtrl;
	SerialDataInt RecvCode;
	byte GetSerialData[Recvpacksize-1];
	if ( Serial.available() >= Recvpacksize )
	{
		if( Serial.read() == '{')
		{
			Serial.readBytes(GetSerialData,Recvpacksize-1);
			if(GetSerialData[Recvpacksize-2] =='}') //B01111101 motor driver on
			{
				RecvCode.binary[0] = GetSerialData[0];
				RecvCode.binary[1] = GetSerialData[1];
				Cmd->code = RecvCode.Data;

				for (int i = 2; i <= 5; i++){
					recvFromUpperCtrl.binary[i-2] = GetSerialData[i];
				}
				switch (Cmd->mode)
				{
				case _OpenLoop:
					Cmd->Volt = recvFromUpperCtrl.Data ;
					break;
				case _Current:
					break;
				case _Velocity:
					break;
				case _Position:
					Cmd->PositionRef = recvFromUpperCtrl.Data ;
					break;
				}
				Cmd->Status = ENABLE;
			}
			else if(GetSerialData[Recvpacksize-2]=='|') //B01111100 motor driver off
			{
				Cmd->PositionRef = recvFromUpperCtrl.Data ;
				Cmd->Status = DISABLE;
			}
		}
		else;
	}
	else;
}
void ReadParams(DrvCtrlPack* Cmd)
{
	SerialDataParamPack recvPID;
	SerialDataInt SetDrvMode;
	SerialDataInt RecvCode;
	byte GetSerialData[RecvDrvParampacksize-1];
	if ( Serial.available() >= RecvDrvParampacksize )
	{
		if( Serial.read() == '{')
		{
			Serial.readBytes(GetSerialData,RecvDrvParampacksize-1);
			if(GetSerialData[RecvDrvParampacksize-2] =='}' || GetSerialData[RecvDrvParampacksize-2]=='|')
			{
				SetDrvMode.binary[0] = GetSerialData[0];
				SetDrvMode.binary[1] = GetSerialData[1];
				Cmd->mode = SetDrvMode.Data;
				for (int i = 2; i <= 5; i++){
					recvPID.Kp.binary[i-2] = GetSerialData[i];
				}
				for (int i = 5; i <= 8; i++){
					recvPID.Ki.binary[i-5] = GetSerialData[i];
				}
				for (int i = 8; i <= 12; i++){
					recvPID.Kd.binary[i-8] = GetSerialData[i];
				}
				switch (Cmd->mode)
				{
				case _OpenLoop:
					//Cmd->Volt = recvPID.Data ;
					break;
				case _Current:
					break;
				case _Velocity:
					break;
				case _Position:
					Cmd->position.Kp = recvPID.Kp.Data ;
					Cmd->position.Ki = recvPID.Ki.Data ;
					Cmd->position.Kd = recvPID.Kd.Data ;
					break;
				}
				Cmd->Status = ENABLE;
			}
			else;
		}
		else;
	}
	else;
}

/**
 * Send feedback to calculate cmd for next sampling time.
 */
void sendFbData(DrvCtrlPack* Cmd,DataPack* Fb)
{
	SerialDataDouble sendtoUpperCtrl;
	SerialDataInt sendPWM_val;
	SerialDataInt sendSTATE;
	if (Cmd->code == FbreqCode)
	{
		byte sendData[Sendpacksize];// {123,0,0,0,0,0,0,0,125};
		byte current_send; // data type for current: unsigned int

		sendData[0] = '{';

		sendtoUpperCtrl.Data = Fb->angularPos;
		for(int i = 1; i <=4; i++){
		  sendData[i] = sendtoUpperCtrl.binary[i-1];
		}
		current_send = Fb->current/80; //max current is 20400mA 20400/255 = 80
		sendData[5] = current_send;

		sendSTATE.Data = Fb->DRVSTATE;
		sendData[6] = sendSTATE.binary[0];
		sendData[7] = sendSTATE.binary[1];
		sendPWM_val.Data = Fb->PWM_val;
		sendData[8] = sendPWM_val.binary[0];
		sendData[9] = sendPWM_val.binary[1];
		sendData[Sendpacksize-1] = '}';
		Serial.write(sendData, Sendpacksize);
	}
	else;
}
