#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import serial

ser = serial.Serial('/dev/ttyUSB-Arduino', baudrate = 9600, timeout=1)


def talker():
    pub = rospy.Publisher('chatter', String, queue_size=10)
    rospy.init_node('talker', anonymous=False)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        arduinoData = str(ser.readline().decode('ascii'))
        rospy.loginfo(arduinoData)
        pub.publish(arduinoData)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass


# , timeout=1
# hello_str = "hello world %s" % rospy.get_time()
# print(arduinoData)
# print(type(arduinoData))
