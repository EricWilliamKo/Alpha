#!/usr/bin/env python
# File name: follow_me_v2 
# Copyright (c) 2017,
# Latest Editor: HaoChih, LIN (2017/05/21)
# Original Author: Eric
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of the Willow Garage, Inc. nor the names of its
#      contributors may be used to endorse or promote products derived from
#       this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import rospy
import sys
import time
import math
import tf
from geometry_msgs.msg import Twist, Pose2D, PoseStamped
from std_msgs.msg import *
from leg_tracker.msg import *

class FollowMe:
    def __init__(self, status):
        self.following_distance = 0.75
        self.Kp_linear = 0.8
        self.Ki_linear = 0.0
	self.Ki_linear_window = 0.95
	self.linear_bound = 1.0
        self.Kp_angular = 1.5
        self.Ki_angular = 0.0
	self.Ki_angular_window = 0.95
	self.angular_bound = 1.0
        self.target_id = -1
        self.status = status
        self.sum_error_r = 0
        self.sum_error_th = 0
        self.previous = 0
        self.listener = tf.TransformListener()
        self.basePerson = PoseStamped()
        self.base_frame = rospy.get_param('~base_frame', "/rugby_base")
        self.motion_pub = rospy.Publisher('/rugby/cmd_vel', Twist, queue_size = 50)
        rospy.Subscriber('/follow_me', String, self.followCB)
        rospy.Subscriber('/people_tracked', PersonArray, self.trackerCB)
        #rospy.Timer(rospy.Duration(0.1), self.timerCB)
        #self.rugby2odomPose = Pose2D()
        rospy.spin()

    """
    def timerCB(self, event):
        print 'Timer called at ' + str(event.current_real)
        try:
            (pos, quat) = self.listener.lookupTransform('/rugby_base', '/odom', rospy.Time(0))
            self.rugby2odomPose.x = pos[0]
            self.rugby2odomPose.y = pos[1]
            self.rugby2odomPose.theta = tf.transformations.euler_from_quaternion(quat)[2]
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            rospy.loginfo('tf listening failed')
        rospy.loginfo('Pose from rugby_base to odom: ' + str(self.rugby2odomPose.x) + ', ' + str(self.rugby2odomPose.y) + ', ' + str(self.rugby2odomPose.theta))
    """

    def followCB(self, String):
        if String.data == 'start':
            self.status = True
            rospy.loginfo('start following')
        elif String.data == 'stop':
            self.status = False
            rospy.loginfo('stop following')
        else:
            rospy.logerr('wrong type msg')

    def trackerCB(self, PersonArray):
        if self.status:
            if self.target_id == -1:
                self.lock_target_id(PersonArray)
            elif PersonArray.people == []:
                self.target_id = -1
                rospy.loginfo('target lost')
            else:
                r,th = self.get_target_position(PersonArray)
                self.following(r,th)

    def xyToPolar(self, PersonArray, i):
        # transform PoseStamped from /odom to /rugby_base
        try:
            odomPerson = PoseStamped()
            odomPerson.header = PersonArray.header
            odomPerson.pose = PersonArray.people[i].pose
            self.basePerson = self.listener.transformPose(self.base_frame, odomPerson)
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            rospy.loginfo('tf listening failed')
        target_x = self.basePerson.pose.position.x
        target_y = self.basePerson.pose.position.y
        r = math.sqrt( math.pow(target_x, 2) + math.pow(target_y, 2) )
        th = math.atan2(target_y,target_x)
        #rospy.loginfo('ID: ' + str(PersonArray.people[i].id) + ', x: ' + str(target_x) + ', y:' + str(target_y) + ', th:' + str(th))
        #th = tf.transformations.euler_from_quaternion([0.0, 0.0, self.basePerson.pose.orientation.z, self.basePerson.pose.orientation.w])[2]
        return r,th

    def get_target_position(self, PersonArray):
        target_get = False
        for i in range(0,len(PersonArray.people)):
            if PersonArray.people[i].id == self.target_id:
                r,th = self.xyToPolar(PersonArray,i)
                #rospy.loginfo('ID: ' + str(PersonArray.people[i].id) + ', r: ' + str(r) + ', th:' + str(th))
                target_get = True
                return r,th
        if not target_get:
            self.target_id = -1
            rospy.loginfo('target lost')

    def lock_target_id(self, PersonArray):
        for i in range(0, len(PersonArray.people)):
            r,th = self.xyToPolar(PersonArray,i)
            if r <= 1 and -0.6 <= th and th <= 0.6:
                self.target_id = PersonArray.people[i].id
                rospy.loginfo('target locked')

    def constrain(self, num, under_linit, up_limit):
        if num >= up_limit:
            num = up_limit
            return num
        elif num <= under_linit:
            num = under_linit
            return num
        else:
            return num

    def following(self, r,th):
        current_time = rospy.Time.now().to_sec()
        dt = current_time - self.previous
        # Calculate PI control
        error_r = r - self.following_distance
        self.sum_error_r = self.Ki_linear_window*self.sum_error_r + error_r*dt
        self.sum_error_th = self.Ki_angular_window*self.sum_error_th + th*dt
        follow_speed_x = self.Kp_linear * error_r + self.Ki_linear * (self.sum_error_r)
        follow_speed_z = self.Kp_angular * (th) + self.Ki_angular * (self.sum_error_th)
        follow_speed_x = self.constrain(follow_speed_x, -self.linear_bound, self.linear_bound)
        follow_speed_z = self.constrain(follow_speed_z, -self.angular_bound, self.angular_bound)
        #pub vel cmd
        motion_cmd = Twist()
        motion_cmd.angular.z = follow_speed_z
        motion_cmd.linear.x = follow_speed_x
	rospy.loginfo('PID linear: ' + str(follow_speed_x) + ', angular: ' + str(follow_speed_z))
        self.motion_pub.publish(motion_cmd)
        self.previous = current_time


# Main function.
if __name__ == '__main__':
    # Initialize the node and name it.
    rospy.init_node("Follow_me",anonymous = False)
    rospy.loginfo("===== legs follower node =====")
    rospy.loginfo("Waiting for leg tracker data ...")
    # Go to class functions that do all the heavy lifting. Do error checking.
    try:
        fm = FollowMe(False)
    except rospy.ROSInterruptException: 
        pass
