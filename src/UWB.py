#!/usr/bin/env python

import rospy
import sys
import time
import math
import tf
import tf2_ros
import serial
import csv

from rugby_lib.imu_gy85 import Imu
from rugby_lib.markerPub import MarkerPub
from geometry_msgs.msg import Twist, TransformStamped
from rugby.msg import *
from nav_msgs.msg import Odometry
from std_msgs.msg import *
# from std_msgs.msg import Bool
from geometry_msgs.msg import PoseWithCovarianceStamped
from visualization_msgs.msg import Marker

stmSerial = serial.Serial("/dev/UWB", 115200)
rcvbuff = ''
seq = 0
# errorArray = []
# errorCount = 0
last_error = 0

def pubAmclPose(pose):
     global UWBmarkers
     UWBmarkers.updateAmclPose(pose.pose.pose.position.x, pose.pose.pose.position.y)

def avgError(e):
    global errorArray,errorCount
    errorArray.insert(errorCount,e)
    errorAvg = sum(errorArray)/len(errorArray)
    errorCount = errorCount + 1
    if errorCount > 10:
        errorCount = 0
    return errorAvg



def postPoseWithCov(x,y,z,e):
    global seq,positionPub,errorPub, UWBmarkers, last_error
    pwcs = PoseWithCovarianceStamped()
    pwcs.header.seq = seq
    pwcs.header.stamp = rospy.Time.now()
    pwcs.header.frame_id = 'map'
    pwcs.pose.pose.position.x = x
    pwcs.pose.pose.position.y = y
    pwcs.pose.pose.position.z = z
    pwcs.pose.covariance = [
        0.4, 0.1, 0.0, 0.0, 0.0, 0.0,
        0.1, 0.4, 0.0, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 0.0, 0.0, 1]

    pwcs.pose.covariance[0] = 0.425*e -2.375
    pwcs.pose.covariance[7] = 0.425*e -2.375
    # pwcs.pose.pose.covariance[35] = 100

    positionPub.publish(pwcs)
    # n = 10.00
    # avgError = e*(1/n) + last_error*((n-1)/n)
    errorPub.publish(e)
    # last_error = avgError
    seq = seq + 1
    UWBmarkers.updateTagPose(x,y,z)
    UWBmarkers.pubMarkerArray()

def serialRead():
    global rcvbuff
    serin = stmSerial.read(stmSerial.inWaiting())
    rcvbuff = serin + rcvbuff
    # rospy.loginfo(rcvbuff)
    time.sleep(0.005)

    try:
        xyz = rcvbuff[rcvbuff.index('S')+1:rcvbuff.index('E')]
        x,y,z,e = xyz.split(',')
        postPoseWithCov(float(x),float(y),float(z),float(e))
    except:
        pass

    try:
        gotEnd = rcvbuff.index('E')
        rcvbuff = ''
    except:
        pass


if __name__ == '__main__':
    rospy.init_node("rugby_uwb",anonymous = False)
    positionPub = rospy.Publisher('uwb_pose',PoseWithCovarianceStamped, queue_size=5)
    errorPub = rospy.Publisher('uwb_error',Float64, queue_size=5)
    rospy.Subscriber('/amcl_pose', PoseWithCovarianceStamped, pubAmclPose)

    UWBmarkers = MarkerPub()
    UWBmarkers.pubMarkerArray()

    while not rospy.is_shutdown():
        serialRead()
      
    rospy.spin()

    if rospy.is_shutdown:
        stmSerial.close()
        rospy.loginfo("Serial closed")
