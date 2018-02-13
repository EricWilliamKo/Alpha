#include "Arduino.h"

bool current_protection = false;

bool checksum(float firstpara, float secondpara, float sum) {
  float mySum = firstpara + secondpara;
  if ((mySum - sum) > -0.1 && (mySum - sum) < 0.1) {
    return true;
  } else {
    return false;
  }
}

void disableCurrentProtect(bool cmd) {
  if (cmd) {
    current_protection = false;
  }
}

bool left_motor_protection(unsigned int current) {
  static bool start_overload = false;
  static unsigned long last_overload_time;
  unsigned int current_max = 1000;

  if (current > current_max && !start_overload) {
    last_overload_time = millis();
    start_overload = true;
  } else if (current > current_max && start_overload) {
    if (millis() - last_overload_time > 5000) {
      current_protection = true;
      return current_protection;
    } else {
      return current_protection;
    }
  }

  if (current < current_max) {
    start_overload = false;
    return current_protection;
  }
}

bool right_motor_protection(unsigned int current) {
  static bool start_overload = false;
  static unsigned long last_overload_time;
  unsigned int current_max = 1000;

  if (current > current_max && !start_overload) {
    last_overload_time = millis();
    start_overload = true;
  } else if (current > current_max && start_overload) {
    if (millis() - last_overload_time > 5000) {
      current_protection = true;
      return current_protection;
    } else {
      return current_protection;
    }
  }

  if (current < current_max) {
    start_overload = false;
    return current_protection;
  }
}
