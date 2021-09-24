#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import time
from std_msgs.msg import UInt16
from std_msgs.msg import Bool
from std_msgs.msg import UInt16 as a
#urusan sama GPIO
kondisi = ""
ir=0

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
        

def listener():
    head = rospy.Publisher('head_scalar', String, queue_size=10)
    pub = rospy.Publisher('servo', UInt16, queue_size=1)
    pub_ir = rospy.Publisher('ir', UInt16, queue_size=1)
    pub_pompa = rospy.Publisher('pompa', Bool, queue_size=1)
    rospy.init_node('control_servo', anonymous=False)
    rospy.Subscriber('chatter', String, callback)
    rospy.Subscriber('f_servo', String, callback_f_servo)
    rospy.spin()

def griper():
    if kondisi == "a":
        pub.publish(45)
    elif kondisi == "b":
        pub.publish(15)
    elif kondisi == "c":
        head.publish(45)
    elif kondisi == "d":
        head.publish(0)
    elif kondisi== "e":
        if ta5 > 47:
            pub_pompa.publish("y")
        else:
            pub_pompa.publish("n")
    pub_ir.publish(ir)
            
    
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

