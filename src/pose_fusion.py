#!/usr/bin/env python
import rospy
import sys
import time
import math
import random
import tf
import tf2_ros
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import Odometry
from std_msgs.msg import *

amcl_pose = PoseWithCovarianceStamped()
uwb_pose = PoseWithCovarianceStamped()
odom = Odometry()
seq = 0
uwb_error = 0.000
amcl_involved = False
amcl_updated = False

def initAmcl():
    global uwb_pose,tfBuffer
    rospy.loginfo('reset init')
    initPosePub = rospy.Publisher('init_amcl_only',PoseWithCovarianceStamped, queue_size=5)
    
    new_pose = PoseWithCovarianceStamped()
    new_pose.header.stamp = rospy.Time.now()
    new_pose.header.frame_id = 'map'
    new_pose.pose.pose.position.x = uwb_pose.pose.pose.position.x
    new_pose.pose.pose.position.y = uwb_pose.pose.pose.position.y
    
    try:
        trans = tfBuffer.lookup_transform('map', 'rugby_base', rospy.Time())
        new_pose.pose.pose.orientation.x = trans.transform.rotation.x
        new_pose.pose.pose.orientation.y = trans.transform.rotation.y
        new_pose.pose.pose.orientation.z = trans.transform.rotation.z
        new_pose.pose.pose.orientation.w = trans.transform.rotation.w
    except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
        pass
    new_pose.pose.covariance[0] = 0.1
    new_pose.pose.covariance[7] = 0.1
    new_pose.pose.covariance[14] = 0
    new_pose.pose.covariance[21] = 0
    new_pose.pose.covariance[28] = 0
    new_pose.pose.covariance[35] = 0.2
    initPosePub.publish(new_pose)

def uwb_amcl_error():
    global amcl_pose, uwb_pose
    error = math.sqrt(pow(amcl_pose.pose.pose.position.x-uwb_pose.pose.pose.position.x, 2)+
                      pow(amcl_pose.pose.pose.position.y-uwb_pose.pose.pose.position.y, 2))
    return error

def updateAmcl(pose):
    global amcl_pose, amcl_updated
    amcl_pose = pose
    amcl_updated = True


def updateUWB(pose):
    global uwb_pose
    uwb_pose = pose

def updateUWBerror(e):
    global uwb_error
    uwb_error = e.data

def updateVelocity(msg):
    global odom
    odom = msg

def fuseTwoPoses():
    global amcl_pose,uwb_pose,uwb_error, seq, tfBuffer, amcl_involved, amcl_updated, odom
    fused_pose = PoseWithCovarianceStamped()

    fused_pose.header.seq = seq
    fused_pose.header.stamp = rospy.Time.now()
    fused_pose.header.frame_id = 'map'

    # rospy.loginfo(str(uwb_amcl_error()))
    # rospy.loginfo(str(uwb_error))

    if uwb_amcl_error() > 1 and uwb_error < 6.2:
        initAmcl()
        amcl_involved = False
    
    if amcl_pose.pose.covariance[0]<0.05 or amcl_pose.pose.covariance[7]<0.05:
        amcl_involved = True
        w_amcl_x = (1/amcl_pose.pose.covariance[0])/((1/amcl_pose.pose.covariance[0])+(1/uwb_pose.pose.covariance[0]))
        w_uwb_x = (1/uwb_pose.pose.covariance[0])/((1/amcl_pose.pose.covariance[0])+(1/uwb_pose.pose.covariance[0]))
        w_amcl_y = (1/amcl_pose.pose.covariance[7])/((1/amcl_pose.pose.covariance[7])+(1/uwb_pose.pose.covariance[7]))
        w_uwb_y = (1/uwb_pose.pose.covariance[7])/((1/amcl_pose.pose.covariance[7])+(1/uwb_pose.pose.covariance[7]))
        x = (amcl_pose.pose.pose.position.x*w_amcl_x + uwb_pose.pose.pose.position.x*w_uwb_x)
        y = (amcl_pose.pose.pose.position.y*w_amcl_y + uwb_pose.pose.pose.position.y*w_uwb_y)
        fused_pose.pose.pose.orientation = amcl_pose.pose.pose.orientation
        rospy.loginfo(str(odom.twist.twist.angular.z))
        for i in range(len(fused_pose.pose.covariance)-1):
            fused_pose.pose.covariance[i] = (amcl_pose.pose.covariance[i] + uwb_pose.pose.covariance[i])/2
        if odom.twist.twist.angular.z > -0.3 and odom.twist.twist.angular.z < 0.3:
            fused_pose.pose.covariance[35] = amcl_pose.pose.covariance[35]
    else:
        amcl_involved = False
        try:
            trans = tfBuffer.lookup_transform('map', 'rugby_base', rospy.Time.now())
            fused_pose.pose.pose.orientation.x = trans.transform.rotation.x
            fused_pose.pose.pose.orientation.y = trans.transform.rotation.y
            fused_pose.pose.pose.orientation.z = trans.transform.rotation.z
            fused_pose.pose.pose.orientation.w = trans.transform.rotation.w
        except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
            pass
        x = uwb_pose.pose.pose.position.x
        y = uwb_pose.pose.pose.position.y
        fused_pose.pose.covariance[0] = uwb_pose.pose.covariance[0]
        fused_pose.pose.covariance[7] = uwb_pose.pose.covariance[7]
        fused_pose.pose.covariance[35] = uwb_pose.pose.covariance[35]

        if amcl_pose.pose.covariance[0]>0.2 or amcl_pose.pose.covariance[7]>0.2:
            initAmcl()
            amcl_involved = False
        else:
            pass

    fused_pose.pose.pose.position.x = x
    fused_pose.pose.pose.position.y = y
    if not amcl_involved:
        positionPub.publish(fused_pose)
        # rospy.loginfo('1111111')
    if amcl_involved and amcl_updated:
        positionPub.publish(fused_pose)
        amcl_updated = False
        # rospy.loginfo('222222')
    seq = seq + 1

def updateFusedPose(pose):
    global positionPub
    initPosePub = rospy.Publisher('init_amcl_only',PoseWithCovarianceStamped, queue_size=5)
    rospy.loginfo('update fuse pose')
    positionPub.publish(pose)
    initPosePub.publish(pose)
    


if __name__ == '__main__':
    rospy.init_node("rugby_uwb",anonymous = False)
    positionPub = rospy.Publisher('fused_pose',PoseWithCovarianceStamped, queue_size=5)
    rospy.Subscriber('/amcl_pose', PoseWithCovarianceStamped, updateAmcl)
    rospy.Subscriber('/uwb_pose', PoseWithCovarianceStamped, updateUWB)
    rospy.Subscriber('/initialpose', PoseWithCovarianceStamped, updateFusedPose)
    rospy.Subscriber('/uwb_error', Float64, updateUWBerror)
    rospy.Subscriber('/odom', Odometry, updateVelocity)
    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)
    r = rospy.Rate(3)
    amcl_pose.pose.covariance[0] = 100
    amcl_pose.pose.covariance[7] = 100

    while not rospy.is_shutdown():
        fuseTwoPoses()
        r.sleep()

    rospy.spin()