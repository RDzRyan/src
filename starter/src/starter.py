#!/usr/bin/env python3
# license removed for brevity

import rospy
from std_msgs.msg import Bool
State = Bool()
State.data = True

def talker():
    pub = rospy.Publisher('/State', Bool,queue_size=1)
    rospy.init_node('starter')
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        pub.publish(State)
        rate.sleep()
        break

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass


