#include "Arduino.h"
#include <Wire.h>
#define cn75address 0x48 // with pins 5~7 set to GND, the device address is 0x48
#define slaveAddress 0x0B

class BatteryMonitor{
public:
  void startMonitorBattery();
  int readVoltage();
  int readCurrent();
  int remainCapacity();
};
