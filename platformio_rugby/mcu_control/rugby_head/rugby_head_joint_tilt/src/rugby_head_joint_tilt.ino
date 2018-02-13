
#include "DRVMC33926_mega328p.h"

#define LOOPTIME 10
#define PRINTTIME 100
#define FEEDBACKTIME 40
//variables for controller
DrvCtrlPack drvInput;
CtrlerPack current;
CtrlerPack velocity;
CtrlerPack position;
DataPack drvMeasure;

// Rugby Tilt_axis
// STATE MACHINE
#define INIT_STATE 100
#define HOME_STATE 101
#define FRONTLIMIT_STATE 102 //HOME_ZONE_STATE 2
#define BACKLIMIT_STATE 103
#define WORKING_RANGE_STATE 104
#define MINLIMITREACHED BACKLIMIT_STATE
#define MAXLIMITREACHED FRONTLIMIT_STATE
int STATE = HOME_STATE;
int INIT1stSeqFlag = false;
int INIT2ndSeqFlag = false;
double WORKING_RANGE = 0.0;
const double INIT_STATE_Volt =  8.0; // Unit : V

//moving average filter for encoder
int arraysize_=100;
uint8_t arrayPtr = 0;
int *countArray = new int[arraysize_];

volatile long Encoderpos = 0;
unsigned long lastMilli = 0;
unsigned long lastPrint = 0;
long dT = 0;

int pinAState = 0;
int pinAStateOld = 0;
int pinBState = 0;
int pinBStateOld = 0;

//current sense
//unsigned int current = 0;

void setup() 
{
	//Initialization
	drvInput.Status = ENABLE;
	drvInput.mode = _Position;//_OpenLoop;
	drvInput.code = FbreqCode;
	position.Kp = 0.5;
	position.Ki = 0.0;

	pinMode(encoderPinA, INPUT); digitalWrite(encoderPinA, HIGH); // turn on pullup resistor
	pinMode(encoderPinB, INPUT); digitalWrite(encoderPinB, HIGH);

	attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
	attachInterrupt(1, doEncoder, CHANGE);

	pinMode(FrontLimitPin, INPUT); pinMode(BackLimitPin, INPUT);
	pinMode(In1, OUTPUT);pinMode(In2, OUTPUT); pinMode(MC33926Enable, OUTPUT);
	digitalWrite(MC33926Enable, LOW);//digitalWrite(MC33926Enable, HIGH);

	Serial.begin(1000000);//115200);
}

void loop() 
{
	switch (drvInput.code)
	{
	case SetDrv: //To be continue..
		ReadParams(&drvInput);
		break;
	case FbreqCode:
		ReadCmd(&drvInput);
		break;
	}

	if((millis()-lastMilli) >= LOOPTIME)
	{
        dT = millis()-lastMilli;
        lastMilli = millis();
        //GET ENC
        getENC(&drvMeasure);
        if (drvInput.Status == ENABLE)
		{
        	digitalWrite(MC33926Enable, HIGH);
        	switch (STATE)
        	{
        	case INIT_STATE: //STATE 100:  // check hardware status
        		if (digitalRead(FrontLimitPin) == true && INIT1stSeqFlag == false)
        		{
        			drvInput.PWM_val = INIT_STATE_Volt/(double(Umax_volt)/double(MaxPWM));
        			WORKING_RANGE = abs(drvMeasure.angularPos);
        			INIT1stSeqFlag = true;
        			INIT2ndSeqFlag = true;
        			Encoderpos = 0;// Initialization for center calibration
        			drvMeasure.angularPos = 0.0;
        		}
        		else;

				drvMeasure.PWM_val = drvInput.PWM_val; //for monitor

				if (drvInput.PWM_val < 0)         	{ analogWrite(In1,0);analogWrite(In2,abs(drvInput.PWM_val)); }
				else if (drvInput.PWM_val > 0)    	{ analogWrite(In1,abs(drvInput.PWM_val)); analogWrite(In2,0);}

				if (abs(drvMeasure.angularPos) >= (WORKING_RANGE/2) && INIT2ndSeqFlag == true) // Origin reached!
				{
					drvInput.PWM_val = 0.0;
					analogWrite(In1,0); analogWrite(In2,0);
					drvMeasure.angularPos = 0.0;
					Encoderpos = 0;
					STATE = WORKING_RANGE_STATE;
					drvInput.Status = DISABLE;
				}
				else;
        		break;
        	case HOME_STATE: //STATE 101:
        		drvInput.PWM_val = INIT_STATE_Volt/(double(Umax_volt)/double(MaxPWM));
				drvMeasure.PWM_val = drvInput.PWM_val; //for monitor

				if (drvInput.PWM_val < 0)         	{ }
				else if (drvInput.PWM_val > 0)    	{ analogWrite(In1,abs(drvInput.PWM_val)); analogWrite(In2,0);}
				else                     			{ analogWrite(In1,0); analogWrite(In2,0);} //for brake
				if (digitalRead(BackLimitPin) == true)
				{
					analogWrite(In1,0); analogWrite(In2,0);
					drvInput.PWM_val = -INIT_STATE_Volt/(double(Umax_volt)/double(MaxPWM));
					//drvMeasure.angularPos = 0.0;
					Encoderpos = 0; // Initialization for center calibration
					STATE = INIT_STATE;
				}
				else;
        		break;
        	case MINLIMITREACHED://FRONTLIMIT_STATE: //STATE 102:
				control_loop(&drvInput, &drvMeasure);
				if (drvInput.Volt >= Umax_volt)			drvInput.Volt = Umax_volt;
				else if (drvInput.Volt <= Umin_volt)	drvInput.Volt = Umin_volt;

				drvInput.PWM_val = drvInput.Volt/(double(Umax_volt)/double(MaxPWM));
				drvMeasure.PWM_val = drvInput.PWM_val; //for monitor

				if (drvInput.PWM_val < 0)         	{ }
				else if (drvInput.PWM_val > 0)    	{ analogWrite(In1,abs(drvInput.PWM_val)); analogWrite(In2,0);}
				else                     			{ analogWrite(In1,0); analogWrite(In2,0);} //for brake
				if (digitalRead(FrontLimitPin) == true)		{STATE = MINLIMITREACHED;}
				else											{STATE = WORKING_RANGE_STATE;}
        		break;
        	case MAXLIMITREACHED://BACKLIMIT_STATE:103
				control_loop(&drvInput, &drvMeasure);
				if (drvInput.Volt >= Umax_volt)			drvInput.Volt = Umax_volt;
				else if (drvInput.Volt <= Umin_volt)	drvInput.Volt = Umin_volt;

				drvInput.PWM_val = drvInput.Volt/(double(Umax_volt)/double(MaxPWM));
				drvMeasure.PWM_val = drvInput.PWM_val; //for monitor

				if (drvInput.PWM_val < 0)         	{ analogWrite(In1,0);analogWrite(In2,abs(drvInput.PWM_val));}
				else if (drvInput.PWM_val > 0)    	{ }
				else                     			{ analogWrite(In1,0); analogWrite(In2,0);} //for brake

				if (digitalRead(BackLimitPin) == true)		{STATE = MAXLIMITREACHED;}
				else										{STATE = WORKING_RANGE_STATE;}
        		break;

        	case WORKING_RANGE_STATE: // 104
				control_loop(&drvInput, &drvMeasure);
				if (drvInput.Volt >= Umax_volt)			drvInput.Volt = Umax_volt;
				else if (drvInput.Volt <= Umin_volt)	drvInput.Volt = Umin_volt;

				drvInput.PWM_val = drvInput.Volt/(double(Umax_volt)/double(MaxPWM));
				drvMeasure.PWM_val = drvInput.PWM_val; //for monitor

				if (drvInput.PWM_val < 0)         	{ analogWrite(In1,0);analogWrite(In2,abs(drvInput.PWM_val)); }
				else if (drvInput.PWM_val > 0)    	{ analogWrite(In1,abs(drvInput.PWM_val)); analogWrite(In2,0);}
				else                     			{ analogWrite(In1,0); analogWrite(In2,0);} //for brake

				if (digitalRead(FrontLimitPin) == true)		{ analogWrite(In1,0); analogWrite(In2,0); STATE = MINLIMITREACHED;}
				else if (digitalRead(BackLimitPin) == true)	{ analogWrite(In1,0); analogWrite(In2,0); STATE = MAXLIMITREACHED;}
				else											{ STATE = WORKING_RANGE_STATE;}
				break;
        	}
        	drvMeasure.DRVSTATE = STATE;
		}
        else
        {
        	analogWrite(In1,0);analogWrite(In2,0); //for brake
        }
	}
	else;

	if(millis() - lastPrint >= FEEDBACKTIME){
		lastPrint = millis();
		sendFbData(&drvInput,&drvMeasure);
	}
}

void control_loop(DrvCtrlPack* Drv, DataPack* Fb){

//	static double last_error=0;
	double error,pTerm, iTerm, pidTerm;

	//targetValue - currentValue;
	if (Drv->mode == _OpenLoop)
	{
		Drv->Volt = 1 * Drv->Volt; //remained
	}
	else if (Drv->mode == _Current) // not done
	{
		current.error = Drv->CurrentRef - Fb->current;
		pTerm = current.Kp * error;
		current.sum_error = current.sum_error + current.error * dT;
		iTerm = current.Ki * current.sum_error;
		Drv->Volt = pTerm + iTerm; //PI controller
	}
	else if (Drv->mode == _Velocity) // not done
	{
		velocity.error = Drv->VelocityRef - Fb->angularVel;
		pTerm = velocity.Kp * error;
		velocity.sum_error = velocity.sum_error + velocity.error * dT;
		iTerm = velocity.Ki * velocity.sum_error;
		Drv->Volt = pTerm + iTerm; //PI controller
	}
	else if (Drv->mode == _Position) // not done
	{
		position.error = Drv->PositionRef - Fb->angularPos;
		pTerm = position.Kp * position.error;
		position.sum_error = position.sum_error + position.error * dT;
		iTerm = position.Ki * position.sum_error;
		Drv->Volt = pTerm + iTerm; //PI controller
	}

}
/**
 * Get Encoder data of each wheel.
 * actual anglar velocity = diff(Encoderpos) * 1000 / dT
 */
void getMotorData()
{
	static long EncoderposPre = 0;
	drvMeasure.angularVel = ((Encoderpos - EncoderposPre)*(1000/dT))*2*PI/(CPR*gear_ratio);  //ticks/s to rad/s
	EncoderposPre = Encoderpos;
}
void getMotorData_Moving_Filter()
{
	countArray[arrayPtr] = Encoderpos;
	uint8_t nextPtr = arrayPtr + 1;
	if(nextPtr > (arraysize_ - 1) )		{nextPtr = 0;}
	else;
	drvMeasure.angularVel = ((double) 1000*(countArray[arrayPtr] - countArray[nextPtr])/ arraysize_)*2*PI/(CPR*gear_ratio);
	arrayPtr ++;
	if(arrayPtr > (arraysize_ -1) )		{arrayPtr = 0;}
	else;
}
void doEncoder()
{
	pinAState = digitalRead(2);
	pinBState = digitalRead(3);

	if (pinAState == 0 && pinBState == 0) {
	if (pinAStateOld == 1 && pinBStateOld == 0) // forward
		Encoderpos ++;
	if (pinAStateOld == 0 && pinBStateOld == 1) // reverse
		Encoderpos --;
	}
	if (pinAState == 0 && pinBState == 1) {
	if (pinAStateOld == 0 && pinBStateOld == 0) // forward
		Encoderpos ++;
	if (pinAStateOld == 1 && pinBStateOld == 1) // reverse
		Encoderpos --;
	}
	if (pinAState == 1 && pinBState == 1) {
	if (pinAStateOld == 0 && pinBStateOld == 1) // forward
		Encoderpos ++;
	if (pinAStateOld == 1 && pinBStateOld == 0) // reverse
		Encoderpos --;
	}

	if (pinAState == 1 && pinBState == 0) {
	if (pinAStateOld == 1 && pinBStateOld == 1) // forward
		Encoderpos ++;
	if (pinAStateOld == 0 && pinBStateOld == 0) // reverse
		Encoderpos --;
	}
	pinAStateOld = pinAState;
	pinBStateOld = pinBState;
}
void getENC(DataPack* Fb)
{
    Fb->angularPos = Encoderpos* ENC2DEG;//360/(CPR*gear_ratio);
}
