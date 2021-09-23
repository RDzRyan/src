#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import time
from std_msgs.msg import UInt16

#urusan sama GPIO
kondisi = ""

def callback_f_servo(flag):
    kondisi = flag.data
    print(kondisi)


def callback(arduinoData):
     dataArdiuno = arduinoData.data
     parsed = dataArdiuno.split(',')
     parsed = [x.rstrip() for x in parsed]
     print(parsed)
     if(len(parsed) > 2):
        ir = int(int(parsed[0] + '0')/10)
        ta1 = int(int(parsed[1] + '0')/10)
        ta2 = int(int(parsed[2] + '0')/10)
        ta3 = int(int(parsed[3] + '0')/10)
        ta4 = int(int(parsed[4] + '0')/10)
        ta5 = int(int(parsed[5] + '0')/10)
        ta6 = int(int(parsed[6] + '0')/10)
        ta7 = int(int(parsed[7] + '0')/10)
        ta8 = int(int(parsed[8] + '0')/10)
        uvtron = int(int(parsed[9] + '0')/10)
        pub_ir.publish('ir')

def listener():
    head = rospy.Publisher('head_scalar', String, queue_size=10)
    pub = rospy.Publisher('servo', UInt16, queue_size=10)
    pub_ir = rospy.Publisher('ir', UInt16, queue_size=1)
    rospy.init_node('control_servo', anonymous=False)
    rospy.Subscriber('chatter', String, callback)
    rospy.Subscriber('f_servo', String, callback_f_servo)
    rospy.spin()

def griper():
    if kondisi.data == "a":
        pub.publish(45)
    elif kondisi.data == "b":
        pub.publish(15)
    elif kondisi.data == "c":
        head.publish(45)
    elif kondisi.data == "d":
        head.publish(0)
    
    #    if ir <= 3:
    #        head.publish("45")
    #        sudut = 15
    #    else:
    #        head.publish("0")
    #        sudut = 45
    #    pub.publish(sudut)
	

if __name__ == '__main__':
    listener()
    griper()

