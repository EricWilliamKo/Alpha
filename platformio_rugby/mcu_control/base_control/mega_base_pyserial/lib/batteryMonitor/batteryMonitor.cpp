#include "batteryMonitor.h"

void BatteryMonitor::startMonitorBattery(){
  Wire.begin();
}

int BatteryMonitor::readVoltage(){
  int result;
  byte a,b;
  Wire.beginTransmission(slaveAddress);
  Wire.write(0x09);
  Wire.endTransmission();
  Wire.requestFrom(slaveAddress, 2);
  a = Wire.read(); // first received byte stored here
  b = Wire.read();
  result = a;
  result |= b<<8;
  return result;
}

int BatteryMonitor::readCurrent(){
  int result;
  byte a,b;
  Wire.beginTransmission(slaveAddress);
  Wire.write(0x0a);
  Wire.endTransmission();
  Wire.requestFrom(slaveAddress, 2);
  a = Wire.read(); // first received byte stored here
  b = Wire.read();
  result = a;
  result |= b<<8;
  return result;
}
