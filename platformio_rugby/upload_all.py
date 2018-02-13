#!/usr/bin/env python2
# upload using ArduinoISP
import argparse
import os
import time

def update(args):
	os.system('platformio run -d ArduinoISP/{} --target upload'.format(args))

def upload(args):
	os.system('platformio run -t program -d {}'.format(args))
	
if __name__ == '__main__':
    update('mega_wheel')
    upload('mcu_control/base_control/mega_base_pyserial')
    update('mega_head')
    upload('mcu_control/rugby_head/rugby_head_ctrl_pyserial')
    update('328p_pan')
    upload('mcu_control/rugby_head/rugby_head_joint_pan')
    update('328p_tilt')
    upload('mcu_control/rugby_head/rugby_head_joint_tilt')
    update('328p_left')
    upload('mcu_control/base_control/Rugby_wheel')
    update('328p_right')
    upload('mcu_control/base_control/Rugby_wheel')
    update('328p_dock')
    upload('mcu_control/autodocking')
    update('328p_imu')
    upload('mcu_control/base_control/GY-85-yaw')