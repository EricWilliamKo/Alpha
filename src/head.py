#!/usr/bin/env python

import rospy
import sys
import time
import math
import tf
import tf2_ros
import serial

from std_msgs.msg import Float32

headSerial = serial.Serial("/dev/mega_head", 1000000)
rcvbuff = ''

def pubPos(pan,tilt):
    global panPosPub,tiltPosPub
    panPosPub.publish(pan)
    tiltPosPub.publish(tilt)

def pubBatteryStatus(voltage,current,capacity):
    global voltagePub, currentPub, capacityPub
    voltagePub.publish(voltage)
    currentPub.publish(current)
    capacityPub.publish(capacity)

def panCmdCb(target):
    pan = target.data
    cmd = 'pan' + str(pan) + 'end'
    # if headSerial.in_waiting == 0:
    headSerial.write(cmd)
    # rospy.loginfo(cmd)

def tiltCmdCb(target):
    tilt = target.data
    cmd = 'tilt' + str(tilt) + 'end'
    # if headSerial.in_waiting == 0:
    headSerial.write(cmd)
    # rospy.loginfo(cmd)

def serialRead():
    global rcvbuff
    serin = headSerial.read(headSerial.inWaiting())
    rcvbuff = rcvbuff + serin
    time.sleep(0.005)

    try:
        PanPos = float(rcvbuff[rcvbuff.index('pp')+2:rcvbuff.index('tp')])
        TiltPos = float(rcvbuff[rcvbuff.index('tp')+2:rcvbuff.index('vo')])
        pubPos(PanPos,TiltPos)


        # PanCurrent = float(rcvbuff[rcvbuff.index('pc')+2:rcvbuff.index('tc')])
        # TiltCurrent = float(rcvbuff[rcvbuff.index('tc')+2:rcvbuff.index('ps')])
        # PanDrvstate = float(rcvbuff[rcvbuff.index('ps')+2:rcvbuff.index('ts')])
        # TiltDrvstate = float(rcvbuff[rcvbuff.index('ts')+2:rcvbuff.index('pwmp')])
        # PanPWM = float(rcvbuff[rcvbuff.index('pwmp')+4:rcvbuff.index('pwmt')])
        # TiltPWM = float(rcvbuff[rcvbuff.index('pwmt')+4:rcvbuff.index('vo')])


        voltage = float(rcvbuff[rcvbuff.index('vo')+2:rcvbuff.index('cu')])
        current = float(rcvbuff[rcvbuff.index('cu')+2:rcvbuff.index('cp')])
        capacity = float(rcvbuff[rcvbuff.index('cp')+2:rcvbuff.index('end')])
        pubBatteryStatus(voltage,current,capacity)

    except:
        pass

    try:
        gotEnd = rcvbuff.index('end')
        rcvbuff = ''
    except:
        pass

    

if __name__ == '__main__':
    global current_time, last_time, autoDockingStatus
    
    rospy.init_node("rugby_head",anonymous = False)
    panPosPub = rospy.Publisher('panPosFb',Float32,queue_size = 10)
    tiltPosPub = rospy.Publisher('tiltPosFb',Float32,queue_size = 10)
    voltagePub = rospy.Publisher('voltage',Float32,queue_size = 10)
    currentPub = rospy.Publisher('current',Float32,queue_size = 10)
    capacityPub = rospy.Publisher('capacity',Float32,queue_size = 10)

    rospy.Subscriber('rugby/Head/Ctrl/PanPose', Float32, panCmdCb)
    rospy.Subscriber('rugby/Head/Ctrl/TiltPose', Float32, tiltCmdCb)
    
    while not rospy.is_shutdown():
        serialRead()
      
    rospy.spin()

    if rospy.is_shutdown:
        headSerial.close()
        rospy.loginfo("head serial closed")