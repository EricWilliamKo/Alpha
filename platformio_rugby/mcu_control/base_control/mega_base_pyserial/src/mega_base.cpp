#include "Arduino.h"
#include "mega_base_lib.h"
#include <avr/io.h>
// This code uses direct port manipulation which only works on Arduino Mega 2560
// Serial1 L
// Serial2 R

// left wheel CCW forward
void sendCmd_wheel_angularVel_L() {
  // convert rad/s to 16 bit integer to send
  int left_target_send = int(-omega_left_target / (double(MaxSpeed) / 32767));
  byte buf[4];
  buf[0] = '{';                        // send start byte
  buf[1] = highByte(left_target_send); // send high byte
  buf[2] = lowByte(left_target_send);  // send low byte
  buf[3] = '}';                        // send stop byte
  Serial1.write(buf, sizeof(buf));
}

// right wheel CW forward
void sendCmd_wheel_angularVel_R() {
  // convert rad/s to 16 bit integer to send
  int right_target_send = int(omega_right_target / (double(MaxSpeed) / 32767));
  byte buf[4];
  buf[0] = '{';                         // send start byte
  buf[1] = highByte(right_target_send); // send high byte
  buf[2] = lowByte(right_target_send);  // send low byte
  buf[3] = '}';                         // send stop byte
  Serial2.write(buf, sizeof(buf));
}

// from 328P
void readFeadback_angularVel_L() {
  int actual_receive;
  if (Serial1.available() >= 5) {
    char rT = (char)Serial1.read(); // read actual speed from Uno
    if (rT == '{') {
      char commandArray[4];
      Serial1.readBytes(commandArray, 4);
      byte rH = commandArray[0];
      byte rL = commandArray[1];
      byte rCS = commandArray[2];
      char rP = commandArray[3];
      if (rP == '}') {
        actual_receive = (rH << 8) + rL;
        // convert received 16 bit integer to actual speed
        omega_left_actual = double(-actual_receive * (double(MaxSpeed) / 32767));
        // max current is 20400mA, 255 * 80 = 20400mA
        current_left = (rCS * 80);
      }
    }
  }
}

// from 328P
void readFeadback_angularVel_R() {
  int actual_receive;
  if (Serial2.available() >= 5) {
    char rT = (char)Serial2.read(); // read actual speed from Uno
    if (rT == '{') {
      char commandArray[4];
      Serial2.readBytes(commandArray, 4);
      byte rH = commandArray[0];
      byte rL = commandArray[1];
      byte rCS = commandArray[2];
      char rP = commandArray[3];
      if (rP == '}') {
        actual_receive = (rH << 8) + rL;
        // convert received 16 bit integer to actual speed
        omega_right_actual = double(actual_receive * (double(MaxSpeed) / 32767));
        // max current is 20400mA, 255 * 80 = 20400mA
        current_right = (rCS * 80);
      }
    }
  }
}

void readIMU() {
  if (Serial3.available() >= 6) {
    char rT = (char)Serial3.read(); // read target speed from mega
    if (rT == '{') {
      char buf[5];
      Serial3.readBytes(buf, 5);
      char rP = buf[4];
      if (rP == '}') {
        yaw.binary[0] = buf[0];
        yaw.binary[1] = buf[1];
        yaw.binary[2] = buf[2];
        yaw.binary[3] = buf[3];
      }
    }
  }
}

void readCMD() {
  int L_index, R_index, sum_index, end_index, unlock_index;
  float Lspeed, Rspeed, sum;

  while (!recieveComplete) {
    if (Serial.available() > 0) {
      rec = Serial.read();
      recbuf += rec;
    } else {
      recieveComplete = true;
    }
  }

  // stop when connection lost
  if ((millis() - lastRecvMilli) > 1000) {
    omega_left_target = 0;
    omega_right_target = 0;
  }

  L_index = recbuf.indexOf('L');
  R_index = recbuf.indexOf('R');
  sum_index = recbuf.indexOf("sum");
  end_index = recbuf.indexOf("end");
  unlock_index = recbuf.indexOf("unlock");

  if (L_index != -1 && recbuf.indexOf('R') != -1) {
    Lspeed = (recbuf.substring(L_index + 1, R_index)).toFloat();
    Rspeed = (recbuf.substring(R_index + 1, sum_index)).toFloat();
    sum = (recbuf.substring(sum_index + 3, end_index)).toFloat();

    if (checksum(Lspeed, Rspeed, sum)) {
      omega_left_target = Lspeed;
      omega_right_target = Rspeed;
      lastRecvMilli = millis();
    }
  }

  if (unlock_index != -1) {
    disableCurrentProtect(true);
    recbuf = "";
  }

  if (end_index != -1 || recbuf.length() > 50) {
    recbuf = "";
  }
}

// to odorid
void sendFeedBack() {
  String speedFb, currentFb, sumFb, gyro;
  float sum;
  if (recieveComplete) {
  speedFb =
      "ls" + String(omega_left_actual) + "rs" + String(omega_right_actual);
  currentFb = "lc" + String(current_left) + "rc" + String(current_right);
  sum = omega_left_actual + omega_right_actual;
  sumFb = "sum" + String(sum);
  gyro = "y" + String(yaw.Data);
  feedback = speedFb + sumFb + gyro + "end";
  Serial.print(feedback);
  recieveComplete = false;
  }
}

void setup() {
  Serial.begin(1000000);
  Serial1.begin(57600);
  Serial2.begin(57600);
  Serial3.begin(115200);
}

void loop() {
  readCMD();                   // read from odroid
  readFeadback_angularVel_L(); // read from 328P
  readFeadback_angularVel_R();
  readIMU();
  if (!left_motor_protection(current_left) &&
      !right_motor_protection(current_right)) {
    sendCmd_wheel_angularVel_R();
    sendCmd_wheel_angularVel_L();
  }
  if (millis() - lastFeedBack >= FeedBackTime) {
    lastFeedBack = millis();
    sendFeedBack();
  }
}
