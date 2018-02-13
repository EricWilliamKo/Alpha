#!/usr/bin/env python
import rospy
import sys
import time
import math
import random
import tf
import tf2_ros
from geometry_msgs.msg import PoseWithCovarianceStamped

amcl_pose = PoseWithCovarianceStamped()
uwb_pose = PoseWithCovarianceStamped()
seq = 0
amclInited = False
last_init = 0
init_count = 0

def initAmcl():
    global uwb_pose,tfBuffer
    rospy.loginfo('reset init')
    initPosePub = rospy.Publisher('init_amcl_only',PoseWithCovarianceStamped, queue_size=5)
    
    # random_yaw = tf.transformations.quaternion_from_euler(0, 0, random.uniform(0,6.28))
    new_pose = PoseWithCovarianceStamped()
    new_pose.header.stamp = rospy.Time.now()
    new_pose.header.frame_id = 'map'
    new_pose.pose.pose.position.x = uwb_pose.pose.pose.position.x
    new_pose.pose.pose.position.y = uwb_pose.pose.pose.position.y
    # new_pose.pose.pose.orientation.x = random_yaw[0]
    # new_pose.pose.pose.orientation.y = random_yaw[1]
    # new_pose.pose.pose.orientation.z = random_yaw[2]
    # new_pose.pose.pose.orientation.w = random_yaw[3]
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
    # rospy.loginfo(str(new_pose))
    initPosePub.publish(new_pose)

def uwb_amcl_error():
    global amcl_pose, uwb_pose
    error = math.sqrt(pow(amcl_pose.pose.pose.position.x-uwb_pose.pose.pose.position.x, 2)+
                      pow(amcl_pose.pose.pose.position.y-uwb_pose.pose.pose.position.y, 2))
    return error

def updateAmcl(pose):
    global amcl_pose, uwb_pose, positionPub, amclInited
    amcl_pose = pose
    amclInited = True


def updateUWB(pose):
    global uwb_pose
    uwb_pose = pose
    

def fuseTwoPoses():
    global amcl_pose,uwb_pose,amclInited, seq, init_count, tfBuffer
    fused_pose = PoseWithCovarianceStamped()

    fused_pose.header.seq = seq
    fused_pose.header.stamp = rospy.Time.now()
    fused_pose.header.frame_id = 'map'
    # rospy.loginfo('error='+str(uwb_amcl_error()))

    if uwb_amcl_error() > 1:
        initAmcl()
    
    if amcl_pose.pose.covariance[0]<0.05 or amcl_pose.pose.covariance[7]<0.05:
        w_amcl_x = (1/amcl_pose.pose.covariance[0])/((1/amcl_pose.pose.covariance[0])+(1/uwb_pose.pose.covariance[0]))
        w_uwb_x = (1/uwb_pose.pose.covariance[0])/((1/amcl_pose.pose.covariance[0])+(1/uwb_pose.pose.covariance[0]))
        w_amcl_y = (1/amcl_pose.pose.covariance[7])/((1/amcl_pose.pose.covariance[7])+(1/uwb_pose.pose.covariance[7]))
        w_uwb_y = (1/uwb_pose.pose.covariance[7])/((1/amcl_pose.pose.covariance[7])+(1/uwb_pose.pose.covariance[7]))
        x = (amcl_pose.pose.pose.position.x*w_amcl_x + uwb_pose.pose.pose.position.x*w_uwb_x)
        y = (amcl_pose.pose.pose.position.y*w_amcl_y + uwb_pose.pose.pose.position.y*w_uwb_y)
        fused_pose.pose.pose.orientation = amcl_pose.pose.pose.orientation
        for i in range(len(fused_pose.pose.covariance)-1):
            fused_pose.pose.covariance[i] = (amcl_pose.pose.covariance[i] + uwb_pose.pose.covariance[i])/2
        fused_pose.pose.covariance[35] = amcl_pose.pose.covariance[35]
    else:
        try:
            trans = tfBuffer.lookup_transform('map', 'rugby_base', rospy.Time())
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
        else:
            pass
            # fused_pose.pose.covariance[35] = amcl_pose.pose.covariance[35]
            # fused_pose.pose.pose.orientation = amcl_pose.pose.pose.orientation
    fused_pose.pose.pose.position.x = x
    fused_pose.pose.pose.position.y = y
    positionPub.publish(fused_pose)
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
    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)
    r = rospy.Rate(3)
    # rospy.loginfo('amclpose='+str(amcl_pose))
    amcl_pose.pose.covariance[0] = 100
    amcl_pose.pose.covariance[7] = 100

    while not rospy.is_shutdown():
        fuseTwoPoses()
        r.sleep()

    rospy.spin()