#!/usr/bin/env python

import rospy
import sys
import time
import math
import tf
import tf2_ros
import serial
import csv

from rugby_lib.imu_gy85 import GY85
from geometry_msgs.msg import Twist, TransformStamped
from rugby.msg import *
from nav_msgs.msg import Odometry
from std_msgs.msg import String
from std_msgs.msg import Bool

megaSerial = serial.Serial("/dev/mega_wheel", 1000000)
last_time = 0

wheelSeparation = 0
wheelRadius = 0
rate = 0

x = 0.0
y = 0.0
th = 0.0

rcvbuff = ''

def broadcastodom():
    global x,y,th
    odom_broadcaster =  tf2_ros.TransformBroadcaster()
    odom_quat = tf.transformations.quaternion_from_euler(0,0,th)
    odom_trans = TransformStamped()
    
    odom_trans.header.stamp = rospy.Time.now()
    odom_trans.header.frame_id = "odom"
    odom_trans.child_frame_id = "rugby_base"
    odom_trans.transform.translation.x = x
    odom_trans.transform.translation.y = y
    odom_trans.transform.translation.z = 0.0
    odom_trans.transform.rotation.x = odom_quat[0]
    odom_trans.transform.rotation.y = odom_quat[1]
    odom_trans.transform.rotation.z = odom_quat[2]
    odom_trans.transform.rotation.w = odom_quat[3]
    
    odom_broadcaster.sendTransform(odom_trans)

def pubodom(vel_x, omega_z):
    global x,y,th, EKF
    odom_pub = rospy.Publisher('/odom', Odometry, queue_size = 50)
    odom_quat = tf.transformations.quaternion_from_euler(0,0,th)
    odom = Odometry()
    
    odom.header.stamp = rospy.Time.now()
    odom.header.frame_id = "odom"
    odom.pose.pose.position.x = x
    odom.pose.pose.position.y = y
    odom.pose.pose.position.z = 0.0
    odom.pose.pose.orientation.x =  odom_quat[0]
    odom.pose.pose.orientation.y =  odom_quat[1]
    odom.pose.pose.orientation.z =  odom_quat[2]
    odom.pose.pose.orientation.w =  odom_quat[3]

    odom.pose.covariance[0] = 1e-3
    odom.pose.covariance[7] = 1e-3
    odom.pose.covariance[14] = 1e6
    odom.pose.covariance[21] = 1e6
    odom.pose.covariance[28] = 1e6
    odom.pose.covariance[35] = 0.1
    
    odom.child_frame_id = "rugby_base"
    odom.twist.twist.linear.x = vel_x
    odom.twist.twist.linear.y = 0.0
    odom.twist.twist.angular.z = omega_z
    odom_pub.publish(odom)


def odometry(omega_left, omega_right):
    global last_time, EKF
    global x,y,th
    
    vel_x = wheelRadius * (omega_right + omega_left) / 2
    omega_z = wheelRadius * (omega_right - omega_left) / wheelSeparation
    current_time = rospy.Time.now()
    dt = current_time.to_sec() - last_time.to_sec()
    last_time = current_time
    
    delta_x = vel_x * math.cos(th) * dt
    delta_y = vel_x * math.sin(th) * dt
    delta_th = omega_z * dt
    
    x += delta_x
    y += delta_y
    th += delta_th
    
    pubodom(vel_x,omega_z)

    if not EKF:
        broadcastodom()

def checksum(omega_left, omega_right, megasum):
    mySum = omega_left + omega_right
    error = mySum - megasum
    if error > -0.01 and error < 0.01:
        return True
    else:
        return False


def serialRead():
    global rcvbuff, imu, EKF
    serin = megaSerial.read(megaSerial.inWaiting())
    rcvbuff = rcvbuff + serin
    time.sleep(0.001)
    try:
        omega_left = float(rcvbuff[rcvbuff.index('ls')+2:rcvbuff.index('rs')])
        omega_right = float(rcvbuff[rcvbuff.index('rs')+2:rcvbuff.index('sum')])
        megasum = float(rcvbuff[rcvbuff.index('sum')+3:rcvbuff.index('y')])
        yaw = float(rcvbuff[rcvbuff.index('y')+1:rcvbuff.index('end')])
        # rospy.loginfo(rcvbuff)
        if checksum(omega_left, omega_right, megasum):
            odometry(omega_left, omega_right)
            vel_feedback_pub = rospy.Publisher('/rugby/vel_feedback',String,queue_size = 10)
            actual_speed = "L" + str(omega_left) + " R" + str(omega_right)
            vel_feedback_string = actual_speed
            vel_feedback_pub.publish(vel_feedback_string)

        if EKF:
            imu.get_imuData(yaw)
        
    except ValueError:
        pass
        
    try:
        gotEnd = rcvbuff.index('end')
        rcvbuff = ''
    except:
        pass
    
def serialSend_wheelCmd(cmdsend):
    if megaSerial.in_waiting == 0:
        megaSerial.write(cmdsend)
        # pub cmd data
        cmdsend_pub = rospy.Publisher('/rugby/cmdsend',String,queue_size = 10)
        cmdsend_string = cmdsend
        cmdsend_pub.publish(cmdsend_string)
        time.sleep(.01)

def cmd_velCallback(twist_aux):

    vel_x = twist_aux.linear.x
    vel_th = twist_aux.angular.z
    
    right_vel = 0.0
    left_vel = 0.0
    
    left_vel = (2*vel_x - vel_th * wheelSeparation) / 2 / wheelRadius
    right_vel =(2*vel_x + vel_th * wheelSeparation) / 2 / wheelRadius

    if left_vel > 0:
        left_vel_string = str(left_vel)[0:5]
    else:
        left_vel_string = str(left_vel)[0:6]
    
    if right_vel > 0:
        right_vel_string = str(right_vel)[0:5]
    else:
        right_vel_string = str(right_vel)[0:6]
    
    checksum = float(left_vel_string) + float(right_vel_string)
    
    cmdsend = 'L' + left_vel_string + 'R' + right_vel_string + 'sum' + str(checksum) +'end'
    serialSend_wheelCmd(cmdsend)

def unlockCallback(unlock):
    if unlock.data:
        cmdsend = 'unlock'
        serialSend_wheelCmd(cmdsend)

if __name__ == '__main__':
    rospy.init_node("mybot_base_controller",anonymous = False)

    current_time = rospy.Time.now()
    last_time = rospy.Time.now()

    wheelSeparation = rospy.get_param("wheelSeparation")
    rospy.loginfo("wheelSeparation from param = %s", wheelSeparation)
    
    wheelRadius = rospy.get_param("wheelRadius")
    rospy.loginfo("wheelRadius from param = %s", wheelRadius)
    
    rate = rospy.get_param('rate')
    rospy.loginfo("rate from param = %s", rate)

    EKF = rospy.get_param('EKF')
    rospy.loginfo("EKF mode %s", EKF)

    if EKF:
        rospy.loginfo('waiting for IMU calibration...')
        imu = GY85()

    # rospy.sleep(0.5)
    # serin = megaSerial.read(megaSerial.inWaiting())

    rospy.Subscriber('/rugby/cmd_vel', Twist, cmd_velCallback)
    rospy.Subscriber('/unlockBaseProtection', Bool, unlockCallback)
    
    while not rospy.is_shutdown():
        serialRead()
    
    if rospy.is_shutdown:
        megaSerial.close()
        rospy.loginfo("base serial closed")
    rospy.spin()
    

        
