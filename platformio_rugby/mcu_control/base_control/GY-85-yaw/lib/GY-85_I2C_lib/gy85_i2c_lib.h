#include "Arduino.h"
#include <Wire.h>

#define HW__VERSION_CODE 10736

#define ACCEL_ADDRESS ((int16_t)0x53) // 0x53 = 0xA6 / 2
#define MAGN_ADDRESS ((int16_t)0x1E)  // 0x1E = 0x3C / 2
#define GYRO_ADDRESS ((int16_t)0x68)  // 0x68 = 0xD0 / 2

#define Gyro_sensitivity 14.375 //ITG-3200 with a sensitivity of 14.375 LSBs per °/sec

typedef union{
	float Data;
	byte binary[4];
}SerialDataFloat;

typedef struct {
  float roll;
  float yaw;
  float pitch;
} Gyro;

typedef struct {
  float x;
  float y;
  float z;
} Accl;

typedef struct {
  float x;
  float y;
  float z;
} Magnetom;

class IMUreader {
public:
  void IMU_setup();
  // Reads x, y and z gyroscope registers
  Gyro Read_Gyro();
  Accl Read_Accel();
  Magnetom Read_Magn();
};