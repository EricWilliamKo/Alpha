#!/usr/bin/env python
import rospy
import os

if __name__ == '__main__':
    rospy.init_node("Rugby_Server",anonymous = False)
    os.system('python RugbyServer.py')
    # rospy.spin()