#include "gy85_i2c_lib.h"

void IMUreader::IMU_setup() {
  Wire.begin();

  // init acc
  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.write(0x2D); // Power register
  Wire.write(0x08); // Measurement mode
  Wire.endTransmission();
  delay(5);
  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.write(0x31); // Data format register
  Wire.write(0x08); // Set to full resolution
  Wire.endTransmission();
  delay(5);
  // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz
  // (25Hz bandwidth)
  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.write(0x2C); // Rate
  Wire.write(0x09); // Set to 50Hz, normal operation
  Wire.endTransmission();
  delay(5);

  // init magn
  Wire.beginTransmission(MAGN_ADDRESS);
  Wire.write(0x02);
  Wire.write(0x00); // Set continuous mode (default 10Hz)
  Wire.endTransmission();
  delay(5);

  Wire.beginTransmission(MAGN_ADDRESS);
  Wire.write(0x00);
  Wire.write(0b00011000); // Set 50Hz
  Wire.endTransmission();
  delay(5);

  // init gyro
  // Power up reset defaults
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(0x3E);
  Wire.write(0x80);
  Wire.endTransmission();
  delay(5);

  // Select full-scale range of the gyro sensors
  // Set LP filter bandwidth to 42Hz
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(0x16);
  Wire.write(0x1B); // DLPF_CFG = 3, FS_SEL = 3
  Wire.endTransmission();
  delay(5);

  // Set sample rato to 50Hz
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(0x15);
  Wire.write(0x0A); //  SMPLRT_DIV = 10 (50Hz)
  Wire.endTransmission();
  delay(5);

  // Set clock to PLL with z gyro reference
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(0x3E);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(5);
}

Gyro IMUreader::Read_Gyro() {
  Gyro gyro;
  int i = 0;
  uint8_t buff[6];

  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(0x1D); // Sends address to read from
  Wire.endTransmission();

  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.requestFrom(GYRO_ADDRESS, 6); // Request 6 bytes
  while (Wire.available())           // ((Wire.available())&&(i<6))
  {
    buff[i] = Wire.read(); // Read one byte
    i++;
  }
  Wire.endTransmission();

  if (i == 6) // All bytes received?
  {
    // X axis (internal sensor -y axis) Roll
    gyro.roll = -1 * (int16_t)(((((uint16_t)buff[2]) << 8) | buff[3]));
    // Y axis (internal sensor -x axis) Pitch
    gyro.pitch = -1 * (int16_t)(((((uint16_t)buff[0]) << 8) | buff[1]));
    // Z axis (internal sensor -z axis) Yaw
    gyro.yaw = -1 * (int16_t)(((((uint16_t)buff[4]) << 8) | buff[5]));

    gyro.roll = gyro.roll/Gyro_sensitivity;
    gyro.pitch = gyro.pitch/Gyro_sensitivity;
    gyro.yaw = gyro.yaw/Gyro_sensitivity;
  }

  return gyro;
}

// Reads x, y and z accelerometer registers
Accl IMUreader::Read_Accel() {
  Accl accel;
  int i = 0;
  uint8_t buff[6];

  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.write(0x32); // Send address to read from
  Wire.endTransmission();

  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.requestFrom(ACCEL_ADDRESS, 6); // Request 6 bytes
  while (Wire.available())            // ((Wire.available())&&(i<6))
  {
    buff[i] = Wire.read(); // Read one byte
    i++;
  }
  Wire.endTransmission();

  if (i == 6) // All bytes received?
  {
    // No multiply by -1 for coordinate system transformation here, because of
    // double negation:
    // We want the gravity vector, which is negated acceleration vector.

    // X axis (internal sensor y axis)
    accel.x = (int16_t)((((uint16_t)buff[3]) << 8) | buff[2]);
    // Y axis (internal sensor x axis)
    accel.y = (int16_t)((((uint16_t)buff[1]) << 8) | buff[0]);
    // Z axis (internal sensor z axis)
    accel.z = (int16_t)((((uint16_t)buff[5]) << 8) | buff[4]);
  }
  return accel;
}

Magnetom IMUreader::Read_Magn() {
  Magnetom magnetom;
  int i = 0;
  uint8_t buff[6];

  Wire.beginTransmission(MAGN_ADDRESS);
  Wire.write(0x03); // Send address to read from
  Wire.endTransmission();

  Wire.beginTransmission(MAGN_ADDRESS);
  Wire.requestFrom(MAGN_ADDRESS, 6); // Request 6 bytes
  while (Wire.available())           // ((Wire.available())&&(i<6))
  {
    buff[i] = Wire.read(); // Read one byte
    i++;
  }
  Wire.endTransmission();

  if (i == 6) // All bytes received?
  {
// 9DOF Razor IMU SEN-10125 using HMC5843 magnetometer
#if HW__VERSION_CODE == 10125
    // MSB byte first, then LSB; X, Y, Z
    // X axis (internal sensor -y axis)
    magnetom.x = -1 * (int16_t)(((((uint16_t)buff[2]) << 8) | buff[3]));
    // Y axis (internal sensor -x axis)
    magnetom.y = -1 * (int16_t)(((((uint16_t)buff[0]) << 8) | buff[1]));
    // Z axis (internal sensor -z axis)
    magnetom.z = -1 * (int16_t)(((((uint16_t)buff[4]) << 8) | buff[5]));
// 9DOF Razor IMU SEN-10736 using HMC5883L magnetometer
#elif HW__VERSION_CODE == 10736
    // MSB byte first, then LSB; Y and Z reversed: X, Z, Y
    // X axis (internal sensor -y axis)
    magnetom.x = -1 * (int16_t)(((((uint16_t)buff[4]) << 8) | buff[5]));
    // Y axis (internal sensor -x axis)
    magnetom.y = -1 * (int16_t)(((((uint16_t)buff[0]) << 8) | buff[1]));
    // Z axis (internal sensor -z axis)
    magnetom.z = -1 * (int16_t)(((((uint16_t)buff[2]) << 8) | buff[3]));
// 9DOF Sensor Stick SEN-10183 and SEN-10321 using HMC5843 magnetometer
#elif (HW__VERSION_CODE == 10183) || (HW__VERSION_CODE == 10321)
    // MSB byte first, then LSB; X, Y, Z
    // X axis (internal sensor x axis)
    magnetom.x = (((uint16_t)buff[0]) << 8) | buff[1];
    // Y axis (internal sensor -y axis)
    magnetom.y = -1 * (int16_t)(((((uint16_t)buff[2]) << 8) | buff[3]));
    // Z axis (internal sensor -z axis)
    magnetom.z = -1 * (int16_t)(((((uint16_t)buff[4]) << 8) | buff[5]));
// 9DOF Sensor Stick SEN-10724 using HMC5883L magnetometer
#elif HW__VERSION_CODE == 10724
    // MSB byte first, then LSB; Y and Z reversed: X, Z, Y
    // X axis (internal sensor x axis)
    magnetom.x = (int16_t)((((uint16_t)buff[0]) << 8) | buff[1]);
    // Y axis (internal sensor -y axis)
    magnetom.y = -1 * (int16_t)(((((uint16_t)buff[4]) << 8) | buff[5]));
    // Z axis (internal sensor -z axis)
    magnetom.z = -1 * (int16_t)(((((uint16_t)buff[2]) << 8) | buff[3]));
#endif
  }
  return magnetom;
}