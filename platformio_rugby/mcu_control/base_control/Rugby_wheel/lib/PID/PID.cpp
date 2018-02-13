#include "PID.h"

PID::PID() {}

void PID::setPID(double P, double I, double D) {
  Kp = P;
  Ki = I;
  Kd = D;
}

void PID::setboundary(double upper, double lower) {
  upperbound = upper;
  lowerbound = lower;
}

void PID::setMaxSpeedPWM(double Speed){
    MaxSpeed = Speed;
}

double constrain(double input, double max, double min){
    if(input > max){
        input = max;
    }else if(input < min){
        input = min;
    }
    return input;
}

double PID::calPID(double targetValue, double currentValue, long dT) {
  static double last_error = 0;
  static double sum_error = 0;

  targetValue = constrain(targetValue,MaxSpeed,-(MaxSpeed));
  double error = targetValue - currentValue;
  sum_error = sum_error + error * dT;
  double d_error = (error - last_error) / dT;
  double pidTerm = Kp * error + Ki * sum_error + Kd * d_error;
  last_error = error;
  if(pidTerm > upperbound){
      sum_error = sum_error - error * dT;
      pidTerm = upperbound;
  }else if(pidTerm < lowerbound){
      sum_error = sum_error - error * dT;
      pidTerm = lowerbound;
  }
  if(targetValue == 0){
      sum_error = 0;
  }
  return pidTerm;
}

int PID::readKp(){
    int kpvalue = Kp;
    return kpvalue;
}

