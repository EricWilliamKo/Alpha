#!/usr/bin/env python

import rospy
import sys
import time
import math
import tf
import tf2_ros
import serial

from geometry_msgs.msg import Twist
from std_msgs.msg import Bool

irSerial = serial.Serial("/dev/328p_dock", 115200)

Slow_turn_Right = 'A'
Slow_turn_Left = 'B'
Slow_go_Staright = 'C'
Fast_turn_Right = 'D'
Fast_turn_Left = 'E'
Fast_go_Stright = 'F'
Stop = 'G'
Nothing = 'H'

SignalLostCounter = 0
DockingStationFound = False
autoDockingStatus = False
pubStationReached = True

def sendCMD(linear, angular):
    motion_pub = rospy.Publisher('/rugby/cmd_vel', Twist, queue_size = 10)
    motion_cmd = Twist()
    motion_cmd.linear.x = linear
    motion_cmd.angular.z = angular
    motion_pub.publish(motion_cmd)

def statusCallBack(status):
    global autoDockingStatus, pubStationReached
    if status.data == True:
        rospy.loginfo('start auto docking')
        pubStationReached = True

    autoDockingStatus = status.data

def pubReachedSignal():
    global pubStationReached,status_pub
    if pubStationReached:
        status_pub.publish(True)
        pubStationReached = False

def lookingForDockingStation():
    rospy.loginfo('lookingForDockingStation')
    sendCMD(0,-0.7)

def motionCMD(mode):
    global DockingStationFound, SignalLostCounter, autoDockingStatus

    if mode == Stop:
        sendCMD(0,0)
        SignalLostCounter = 0
        pubReachedSignal()
        autoDockingStatus = False
    elif mode == Slow_turn_Right:
        sendCMD(-0.06,-0.15)
        SignalLostCounter = 0
    elif mode == Slow_turn_Left:
        sendCMD(-0.06,0.15)
        SignalLostCounter = 0
    elif mode == Slow_go_Staright:
        sendCMD(-0.06,-0.005)
        SignalLostCounter = 0
    elif mode == Fast_turn_Right:
        sendCMD(-0.08,-0.3)
        SignalLostCounter = 0
    elif mode == Fast_turn_Left:
        sendCMD(-0.08,0.3)
        SignalLostCounter = 0
    elif mode == Fast_go_Stright:
        sendCMD(-0.08,0.005)
        SignalLostCounter = 0
    elif mode == Nothing:
        SignalLostCounter += 1

    if SignalLostCounter > 10:
        DockingStationFound = False
        pubStationReached  = True
        SignalLostCounter = 0

    rospy.loginfo(mode)

def serialRead():
    global DockingStationFound
    if irSerial.in_waiting > 0:
        serin = irSerial.read()
        if serin != Nothing and not DockingStationFound:
            DockingStationFound = True
            lookingForDockingStation()
        if DockingStationFound:
            motionCMD(serin)
        else:
            lookingForDockingStation()

if __name__ == '__main__':
    global last_time, autoDockingStatus

    rospy.init_node("AutoDocking",anonymous = False)
    rospy.Subscriber('autoDocking', Bool, statusCallBack)
    status_pub = rospy.Publisher('/docking_station_reached', Bool, queue_size = 10)

    while not rospy.is_shutdown():
        serin = irSerial.read()
        while autoDockingStatus and not rospy.is_shutdown():
            serialRead()
            time.sleep(0.01)

    if rospy.is_shutdown:
        autoDockingStatus = False
        irSerial.close()
        rospy.loginfo("serial closed")


    rospy.spin()
