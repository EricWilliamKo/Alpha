import rospy
import sys
import cherrypy
import json

from std_msgs.msg import *

class RosTopicAPI():
    voltage = 0.0

    def updateVoltage(self, newVoltage):
        self.voltage = newVoltage.data

    def getVoltage(self):
        return self.voltage

    def __init__(self):
        rospy.Subscriber('/voltage', Float32, self.updateVoltage)