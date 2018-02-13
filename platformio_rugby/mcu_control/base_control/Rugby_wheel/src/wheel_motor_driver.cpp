#include "Arduino.h"
#include "wheel_drv.h"

void readCmd_wheel_angularVel() {
  if (Serial.available() > 4) {
    char rT = (char)Serial.read(); // read target speed from mega
    if (rT == '{') {
      char commandArray[3];
      // mySerial.readBytes(commandArray,3);
      Serial.readBytes(commandArray, 3);
      byte rH = commandArray[0];
      byte rL = commandArray[1];
      char rP = commandArray[2];
      if (rP == '}') {
        target_receive = (rH << 8) + rL;
        // convert received 16 bit integer to actual speed
        omega_target = double(target_receive * (double(MaxSpeed) / 32767));
        lastRecvMilli = millis();
      }
    }
  }
}

void sendFeedback_wheel_angularVel() {
  // getMotorData();
  byte current_send;
  // convert rad/s to 16 bit integer to send
  int actual_send = int(omega_actual / (double(MaxSpeed) / 32767));
  // max current is 20400mA 20400/255 = 80
  current_send = current / 80;
  int kpvalue = pid.readKp();
  byte buf[5];
  buf[0] = '{';                   // send start byte
  buf[1] = highByte(actual_send); // send high byte
  buf[2] = lowByte(actual_send);  // send low byte
  buf[3] = current_send;
  buf[4] = '}'; // send stop byte
  Serial.write(buf, sizeof(buf));
}

void CurrentMonitor() {
  current = analogRead(analogPin) *
            9.3; // 5V / 1024 ADC counts / 525 mV per A = 34 mA per count
}

void getMotorData() {
  static long EncoderposPre = 0;
  // converting ticks/s to rad/s
  omega_actual = ((Encoderpos - EncoderposPre) * (1000 / dT)) * 2 * PI /
                 (CPR * gear_ratio);
  EncoderposPre = Encoderpos;
}

void doEncoder() {
  pinAState = digitalRead(encoder0PinA);
  pinBState = digitalRead(encoder0PinB);

  if (pinAState == 0 && pinBState == 0) {
    if (pinAStateOld == 1 && pinBStateOld == 0) // forward
      Encoderpos++;
    if (pinAStateOld == 0 && pinBStateOld == 1) // reverse
      Encoderpos--;
  }
  if (pinAState == 0 && pinBState == 1) {
    if (pinAStateOld == 0 && pinBStateOld == 0) // forward
      Encoderpos++;
    if (pinAStateOld == 1 && pinBStateOld == 1) // reverse
      Encoderpos--;
  }
  if (pinAState == 1 && pinBState == 1) {
    if (pinAStateOld == 0 && pinBStateOld == 1) // forward
      Encoderpos++;
    if (pinAStateOld == 1 && pinBStateOld == 0) // reverse
      Encoderpos--;
  }

  if (pinAState == 1 && pinBState == 0) {
    if (pinAStateOld == 1 && pinBStateOld == 1) // forward
      Encoderpos++;
    if (pinAStateOld == 0 && pinBStateOld == 0) // reverse
      Encoderpos--;
  }
  pinAStateOld = pinAState;
  pinBStateOld = pinBState;
}

void setup() {
  // TCCR0B = TCCR0B & B11111000 | B00000010;
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH); // turn on pullup resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH); // turn on pullup resistor

  attachInterrupt(0, doEncoder, CHANGE); // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoder, CHANGE);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(MC33926Enable, OUTPUT);
  Serial.begin(57600);
  digitalWrite(MC33926Enable, HIGH);
  double Kp = 30;
  double Ki = 0.3;
  double Kd = 0;

  pid.setPID(Kp, Ki, Kd);
  pid.setboundary(MaxPWM, -(MaxPWM));
  pid.setMaxSpeedPWM(MaxSpeed);
}

void loop() {
  readCmd_wheel_angularVel();
  CurrentMonitor();

  if ((millis() - lastRecvMilli) > 500) {
    omega_target = 0;
  }

  if ((millis() - lastMilli) >= LOOPTIME) { // enter tmed loop
    dT = millis() - lastMilli;
    lastMilli = millis();

    getMotorData();                  // calculate speed
    sendFeedback_wheel_angularVel(); // send actually speed to mega

    PWM_val = int(pid.calPID(omega_target, omega_actual,
                             dT)); // compute PWM value from rad/s

    if (omega_target == 0) {
      PWM_val = 0;
    }
    if (PWM_val == 0) {
      analogWrite(In1, 0);
      analogWrite(In2, 0);
    }
    if (PWM_val > 0) {
      analogWrite(In1, abs(PWM_val));
      analogWrite(In2, 0);
    }
    if (PWM_val < 0) {
      analogWrite(In2, abs(PWM_val));
      analogWrite(In1, 0);
    }
  }
}
