#!/usr/bin/env python

from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
import rospy
import sys
import time
import math
import tf
import tf2_ros

class MarkerPub:
    markerArray = MarkerArray()
    publisher = rospy.Publisher("UWB_Markers", MarkerArray, queue_size=5)
    lastpub = 0
    sqe = 0

    anchor0 = Marker()
    anchor0.header.frame_id = "map"
    anchor0.id = 0
    anchor0.type = Marker.CYLINDER
    anchor0.action = Marker.ADD
    anchor0.scale.x = 0.2
    anchor0.scale.y = 0.2
    anchor0.scale.z = 3
    anchor0.color.a = 1.0
    anchor0.color.r = 0.2
    anchor0.color.g = 1.0
    anchor0.color.b = 1.0
    anchor0.pose.orientation.w = 0.0
    anchor0.pose.position.x = 12.50
    anchor0.pose.position.y = 8.00
    anchor0.pose.position.z = 0

    anchor1 = Marker()
    anchor1.header.frame_id = "map"
    anchor1.id = 1
    anchor1.type = Marker.CYLINDER
    anchor1.action = Marker.ADD
    anchor1.scale.x = 0.2
    anchor1.scale.y = 0.2
    anchor1.scale.z = 3
    anchor1.color.a = 1.0
    anchor1.color.r = 0.2
    anchor1.color.g = 1.0
    anchor1.color.b = 1.0
    anchor1.pose.orientation.w = 0.0
    anchor1.pose.position.x = 12.50
    anchor1.pose.position.y = 16.50
    anchor1.pose.position.z = 0

    anchor2 = Marker()
    anchor2.header.frame_id = "map"
    anchor2.id = 2
    anchor2.type = Marker.CYLINDER
    anchor2.action = Marker.ADD
    anchor2.scale.x = 0.2
    anchor2.scale.y = 0.2
    anchor2.scale.z = 3
    anchor2.color.a = 1.0
    anchor2.color.r = 0.2
    anchor2.color.g = 1.0
    anchor2.color.b = 1.0
    anchor2.pose.orientation.w = 0.0
    anchor2.pose.position.x = 19.50
    anchor2.pose.position.y = 16.50
    anchor2.pose.position.z = 0

    anchor3 = Marker()
    anchor3.header.frame_id = "map"
    anchor3.id = 3
    anchor3.type = Marker.CYLINDER
    anchor3.action = Marker.ADD
    anchor3.scale.x = 0.2
    anchor3.scale.y = 0.2
    anchor3.scale.z = 3
    anchor3.color.a = 1.0
    anchor3.color.r = 0.2
    anchor3.color.g = 1.0
    anchor3.color.b = 1.0
    anchor3.pose.orientation.w = 0.0
    anchor3.pose.position.x = 19.50
    anchor3.pose.position.y = 8.00
    anchor3.pose.position.z = 0

    markerArray.markers.append(anchor0)
    markerArray.markers.append(anchor1)
    markerArray.markers.append(anchor2)
    markerArray.markers.append(anchor3)

    tag = Marker()
    tag.header.frame_id = "map"
    tag.id = 4
    tag.action = Marker.ADD
    tag.type = Marker.SPHERE
    tag.scale.x = 0.2
    tag.scale.y = 0.2
    tag.scale.z = 0.2
    tag.color.a = 1.0
    tag.color.r = 0.0
    tag.color.g = 0.0
    tag.color.b = 1.0
    tag.pose.orientation.w = 0.0
    tag.pose.position.x = 0
    tag.pose.position.y = 0
    tag.pose.position.z = 0

    markerArray.markers.append(tag)

    apose = Marker()
    apose.header.frame_id = "map"
    apose.id = 5
    apose.action = Marker.ADD
    apose.type = Marker.SPHERE
    apose.scale.x = 0.2
    apose.scale.y = 0.2
    apose.scale.z = 0.2
    apose.color.a = 1.0
    apose.color.r = 0.0
    apose.color.g = 1.0
    apose.color.b = 0.0
    apose.pose.orientation.w = 0.0
    apose.pose.position.x = 0
    apose.pose.position.y = 0
    apose.pose.position.z = 0
    markerArray.markers.append(apose)

    def addMarker(self,marker):
        self.markerArray.markers.append(marker)

    def updateTagPose(self,x,y,z):
        self.tag.pose.position.x = x
        self.tag.pose.position.y = y
        # self.tag.pose.position.z = z
        self.tag.header.stamp = rospy.Time.now()
        self.tag.header.seq = self.sqe

    def updateAmclPose(self,x,y):
        self.apose.pose.position.x = x
        self.apose.pose.position.y = y
        self.apose.header.stamp = rospy.Time.now()
        self.apose.header.seq = self.sqe

    def pubMarkerArray(self):
        self.publisher.publish(self.markerArray)
        # while not rospy.is_shutdown():
        #     if rospy.Time.now().to_sec() - self.lastpub > 0.2:
        #         self.publisher.publish(self.markerArray)
        #         self.sqe = self.sqe + 1
        #         self.lastpub = rospy.Time.now().to_sec()
            
            
        


    
