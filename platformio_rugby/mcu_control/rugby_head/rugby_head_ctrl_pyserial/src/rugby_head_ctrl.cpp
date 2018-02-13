#include "Arduino.h"
#include "batteryMonitor.h"
#include "xyplatform.h"
#include <avr/io.h>
#include <avr/wdt.h>

#define LOOPTIME 10
#define PubTime 100
#define BatteryObserverTime 500

long dT = 0;

xyPlatform::SetCmdLimit InputLimit = {Umax_volt, Umin_volt};
xyPlatform::ParamPack PanCtrlAxis = {Pan_axis, CTRLMode, FbreqCode,
                                     DISABLE,  0.0,      0.0};
xyPlatform::ParamPack TiltCtrlAxis = {Tilt_axis, CTRLMode, FbreqCode,
                                      DISABLE,   0.0,      0.0};

xyPlatform Pan(&PanCtrlAxis, &InputLimit);
xyPlatform Tilt(&TiltCtrlAxis, &InputLimit);

BatteryMonitor bm;

char rec;
bool recieveComplete = false;
String recbuf;
String feedback;
int voltage = 0;
int current = 0;
int capacity = 0;
int cuvoswitch = 0;
bool set_;
bool HEADDrvStatus = DISABLE;
int STATE = WAIT4INIT_STATE;
bool SetDrvParamStatus; // TBD

void TiltCtrl(double target) { Tilt.DrvCtrl->PositionCmd = target; }
void PanCtrl(double target) { Pan.DrvCtrl->PositionCmd = target; }
void SetDrvParam(float Kp, float Ki, float Kd) {
  // To be continue..
  // Pan.DrvCtrl->mode = req;
  Pan.DrvCtrl->Kp = Kp;
  Pan.DrvCtrl->Ki = Ki;
  Pan.DrvCtrl->Kd = Kd;
  if (SetDrvParamStatus == ENABLE) {
    // res.driverstate = true;
    // nh.loginfo("Write parameters to Drivers");
  }
}
void DriverState(bool driverstate) {
  HEADDrvStatus = driverstate;
  if (HEADDrvStatus == ENABLE) {
    // driverstate = true;
    Pan.Enable();
    Tilt.Enable();
  } else if (HEADDrvStatus == DISABLE) {
    // driverstate = false;
    Pan.Disable();
    Tilt.Disable();
  }
}

bool decodeCMD(String buff) {
  double pan_target, tilt_target;
  int pan_index, tilt_index, setpara_index, setdrv_index, end_index;
  pan_index = buff.indexOf("pan");
  tilt_index = buff.indexOf("tilt");
  setpara_index = buff.indexOf("setpara");
  setdrv_index = buff.indexOf("setdrv");
  end_index = buff.indexOf("end");

  if (pan_index != -1 && end_index != -1) {
    pan_target = (buff.substring(pan_index + 3, end_index)).toDouble();
    PanCtrl(pan_target);
  }
  if (tilt_index != -1 && end_index != -1) {
    tilt_target = (buff.substring(tilt_index + 4, end_index)).toDouble();
    TiltCtrl(tilt_target);
  }
  if (setpara_index != -1 && end_index != -1) {
    int Kp_index, Ki_index, Kd_index;
    float Kp, Ki, Kd;
    Kp_index = buff.indexOf("kp");
    Ki_index = buff.indexOf("ki");
    Kd_index = buff.indexOf("kd");
    Kp = (buff.substring(Kp_index + 2, Ki_index)).toFloat();
    Ki = (buff.substring(Ki_index + 2, Kd_index)).toFloat();
    Kd = (buff.substring(Kd_index + 2, end_index)).toFloat();
    SetDrvParam(Kp, Ki, Kd);
  }
  if (setdrv_index != -1 && end_index != -1) {
    String state;
    state = buff.substring(setdrv_index + 6, end_index);
    if (state == "enable") {
      DriverState(true);
    } else if (state == "disable") {
      DriverState(false);
    }
  }
  if (end_index != -1) {
    return true;
  } else {
    return false;
  }
}

// to odorid
void sendFeedBack() {
  static unsigned long PrePubMilli = 0;
  String HeadPos, motorCurrent, drvstate, pwm, battery;
  if(millis()-PrePubMilli > 100){
  if (recieveComplete) {
    PrePubMilli = millis();
    HeadPos =
        "pp" + String(Pan.Fb.angularPos) + "tp" + String(Tilt.Fb.angularPos);
    motorCurrent = "pc" + String(Pan.Fb.currentSense) + "tc" +
                   String(Tilt.Fb.currentSense);
    drvstate = "ps" + String(Pan.Fb.DRVSTATE) + "ts" + String(Tilt.Fb.DRVSTATE);
    pwm = "pwmp" + String(Pan.Fb.PWM_val) + "pwmt" + String(Tilt.Fb.PWM_val);
    battery = "vo" + String(voltage) + "cu" + String(current) + "cp" + String(capacity);
    feedback = HeadPos + battery + "end";
    Serial.print(feedback);
    recieveComplete = false;
    delay(1);
  }
  }
}

void readCMD() {
  while (!recieveComplete) {
    if (Serial.available() > 0) {
      rec = Serial.read();
      recbuf += rec;
    } else {
      recieveComplete = true;
    }
  }
  if (decodeCMD(recbuf)) {
    recbuf = "";
  }
}

void batteryRead() {
  static unsigned long lastMilli = 0;
  if ((millis() - lastMilli) >= BatteryObserverTime) {
    cuvoswitch++;
    if (cuvoswitch % 3 == 0) {
      voltage = bm.readVoltage();
    } else if(cuvoswitch %3 == 1){
      current = bm.readCurrent();
    } else if(cuvoswitch %3 == 2){
      capacity = bm.remainCapacity();
    }
    if (cuvoswitch > 100) {
      cuvoswitch = 0;
    }
    lastMilli = millis();
  }
}

void setup() {
  Serial.begin(1000000);
  bm.startMonitorBattery();
  Pan.SerialBegin(1000000);
  Tilt.SerialBegin(1000000);
  wdt_enable(WDTO_1S);
  // Initialization
  Pan.DrvCtrl->code = FbreqCode;
  Tilt.DrvCtrl->code = FbreqCode;
}

void loop() {
  readCMD();
  Pan.SerialGetDriverData(&Pan.Fb);
  Tilt.SerialGetDriverData(&Tilt.Fb);
  sendFeedBack();
  switch (STATE) // To be continue
  {
  case WAIT4INIT_STATE:
    if (Pan.Fb.DRVSTATE == WORKING_RANGE_STATE &&
        Tilt.Fb.DRVSTATE == WORKING_RANGE_STATE) {
      STATE = NORMAL_STATE;
      Pan.Enable();
      Tilt.Enable();
    } else {
      STATE = WAIT4INIT_STATE;
    }
    break;
  case NORMAL_STATE:
    Pan.SendCmd();
    Tilt.SendCmd();
    // nh.loginfo("NORMAL_STATE");
    break;
  case IDLE_STATE:
    break;
  }
  batteryRead();
  wdt_reset();
}
