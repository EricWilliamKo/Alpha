#include "gy85_i2c_lib.h"

#define LOOPTIME 10

IMUreader reader;
Gyro gyro;

SerialDataFloat roll;
SerialDataFloat yaw;
SerialDataFloat pitch;

unsigned long lasttime = 0;

void sendResult(SerialDataFloat result){
    byte buf[6];
    buf[0] = '{';
    buf[1] = result.binary[0];
    buf[2] = result.binary[1];
    buf[3] = result.binary[2];
    buf[4] = result.binary[3];
    buf[5] = '}';
    Serial.write(buf, sizeof(buf));
}

void setup() {
   Serial.begin(115200);
   reader.IMU_setup();
}

void loop() {
    if(millis() - lasttime >= LOOPTIME){
    gyro = reader.Read_Gyro();
    yaw.Data = gyro.yaw;
    sendResult(yaw);
    }
}