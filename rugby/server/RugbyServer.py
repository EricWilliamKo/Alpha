#!/usr/bin/env python
import rospy
import sys
import os.path
import cherrypy
import json

from rosTopicAPI.rosTopicAPI import RosTopicAPI

class CherryPyServer:
    topicHandler = RosTopicAPI()

    def __init__(self):
        rospy.loginfo('start topic handler')

    @cherrypy.expose
    def index(self):
        rospy.loginfo('hello')
        return 'Hi~ My name is Rugby!!'

    @cherrypy.expose
    @cherrypy.tools.json_out()
    def voltage(self):
        voltage = self.topicHandler.getVoltage()
        return {'voltage': voltage}
    
    @cherrypy.expose
    def shutdown(self):  
        cherrypy.engine.exit()

rugbyconf = os.path.join(os.path.dirname(__file__), 'rugby_server.conf')


if __name__ == '__main__':
    # rospy.init_node("Rugby_Server",anonymous = False)
    cherrypy.quickstart(CherryPyServer(), config=rugbyconf)