#include <ros.h>
#include <DriverState.h>
#include <HeadCtrlFb.h>
#include <SetDrvParam.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>

#define LOOPTIME 10
#define PUBLISHRATE 100
#include "xyplatform.h"

unsigned long PrePubMilli = 0;
unsigned long lastMilli = 0;
long dT = 0;

xyPlatform::SetCmdLimit InputLimit = {Umax_volt,Umin_volt};
xyPlatform::ParamPack PanCtrlAxis = {Pan_axis, CTRLMode, FbreqCode,DISABLE, 0.0, 0.0};
xyPlatform::ParamPack TiltCtrlAxis = {Tilt_axis, CTRLMode, FbreqCode,DISABLE, 0.0, 0.0};

xyPlatform Pan(&PanCtrlAxis,&InputLimit);
xyPlatform Tilt(&TiltCtrlAxis,&InputLimit);

ros::NodeHandle nh;
bool set_; 
bool HEADDrvStatus = DISABLE ;
int STATE = WAIT4INIT_STATE;
bool SetDrvParamStatus; // TBD
//void HeadCtrlCallBack(const rugby::HeadCtrlCmd& msg);
void TiltCtrlCb(const std_msgs::Float32& msg);
void PanCtrlCb(const std_msgs::Float32& msg);
void DriverState_service_callback(const rugby::DriverStateRequest& req, rugby::DriverStateResponse& res);
void SetDrvParam_service_callback(const rugby::SetDrvParamRequest& req, rugby::SetDrvParamResponse& res);

/* ************  declarations for ROS usages *****************************************/

//rugby::HeadCtrlCmd HeadCmd_msgs;
rugby::HeadCtrlFb HeadFb_msgs;
std_msgs::Float32 Tilt_msgs;
std_msgs::Float32 Pan_msgs;
ros::Subscriber<std_msgs::Float32> Tilt_sub("rugby/Head/Ctrl/TiltPose",TiltCtrlCb);
ros::Subscriber<std_msgs::Float32> Pan_sub("rugby/Head/Ctrl/PanPose",PanCtrlCb);
//ros::Subscriber<rugby::HeadCtrlCmd> s1("rugby/Head/Ctrl", HeadCtrlCallBack);
ros::Publisher HeadFb_pub("rugby/Head/Fb",&HeadFb_msgs);
ros::ServiceServer<rugby::DriverStateRequest, rugby::DriverStateResponse> service("DriverState_service", &DriverState_service_callback);
ros::ServiceServer<rugby::SetDrvParamRequest,rugby::SetDrvParamResponse> param_service("SetDrvParam_service",&SetDrvParam_service_callback);

/* ************  End of declarations for ROS usages ****************/

//void HeadCtrlCallBack(const rugby::HeadCtrlCmd& msg)
//{
//	Pan.DrvCtrl->PositionCmd = msg.PanCmdPos;
//	Tilt.DrvCtrl->PositionCmd = msg.TiltCmdPos;
//}
void TiltCtrlCb(const std_msgs::Float32& msg)
{
	Tilt.DrvCtrl->PositionCmd = msg.data;
}
void PanCtrlCb(const std_msgs::Float32& msg)
{
	Pan.DrvCtrl->PositionCmd = msg.data;
}
void SetDrvParam_service_callback(const rugby::SetDrvParamRequest& req, rugby::SetDrvParamResponse& res)
{
	// To be continue..
	//Pan.DrvCtrl->mode = req;
	Pan.DrvCtrl->Kp = req.Kp;
	Pan.DrvCtrl->Ki = req.Ki;
	Pan.DrvCtrl->Kd = req.Kd;
    if (SetDrvParamStatus == ENABLE)
    {
		//res.driverstate = true;
		nh.loginfo("Write parameters to Drivers");
    }
    else;
}
void DriverState_service_callback(const rugby::DriverStateRequest& req, rugby::DriverStateResponse& res)
{
	HEADDrvStatus = req.driverstate;
    if (HEADDrvStatus == ENABLE)
    {
		res.driverstate = true;
		Pan.Enable();
		Tilt.Enable();
		nh.loginfo("Drivers Enabled");
    }
    else if (HEADDrvStatus == DISABLE)
    {
		res.driverstate = false;
		Pan.Disable();
		Tilt.Disable();
		nh.loginfo("Drivers Disabled");
    }
}
void FbVelCalculation()
{
	// for publish
	HeadFb_msgs.PanFbPos = Pan.Fb.angularPos;
	HeadFb_msgs.TiltFbPos = Tilt.Fb.angularPos;
	HeadFb_msgs.PanFbCurrent = Pan.Fb.currentSense;
	HeadFb_msgs.TiltFbCurrent = Tilt.Fb.currentSense;
	HeadFb_msgs.PanFbDrvState = Pan.Fb.DRVSTATE;
	HeadFb_msgs.TiltFbDrvState = Tilt.Fb.DRVSTATE;
	HeadFb_msgs.PanFbDrvPWM = Pan.Fb.PWM_val;
	HeadFb_msgs.TiltFbDrvPWM = Tilt.Fb.PWM_val;
}
void setup()
{
	nh.getHardware()->setBaud(1000000);
	nh.initNode();
	nh.subscribe(Pan_sub);
	nh.subscribe(Tilt_sub);
	nh.advertise(HeadFb_pub);
	nh.advertiseService(service);
	nh.advertiseService(param_service);

	while(!nh.connected()) nh.spinOnce();
	nh.loginfo("Startup complete");

	Pan.SerialBegin(1000000);
	Tilt.SerialBegin(1000000);

	//Initialization
	Pan.DrvCtrl->code = FbreqCode;
	Tilt.DrvCtrl->code = FbreqCode;
}
void loop() 
{
	if((millis()-lastMilli) >= LOOPTIME)
	{
		lastMilli = millis();
		Pan.SerialGetDriverData(&Pan.Fb);
		Tilt.SerialGetDriverData(&Tilt.Fb);
		FbVelCalculation();

		switch (STATE) // To be continue
		{
		case WAIT4INIT_STATE:
			if (Pan.Fb.DRVSTATE == WORKING_RANGE_STATE && Tilt.Fb.DRVSTATE == WORKING_RANGE_STATE)
			{
				STATE = NORMAL_STATE;
				Pan.Enable();
				Tilt.Enable();
			}
			else
			{
				STATE = WAIT4INIT_STATE;
			}
			break;
		case NORMAL_STATE:
			Pan.SendCmd();
			Tilt.SendCmd();
			//nh.loginfo("NORMAL_STATE");
			break;
		case IDLE_STATE:
			break;
		}
	}
	else;

	if((millis()-PrePubMilli) >= PUBLISHRATE)
	{
		PrePubMilli = millis();
		HeadFb_pub.publish(&HeadFb_msgs);
	}
	else;
	nh.spinOnce();
}
