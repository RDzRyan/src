#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import time
import RPi.GPIO as GPIO

#urusan sama GPIO


GPIO.setwarnings(False) 
GPIO.setmode(GPIO.BCM) #BCM jadi GPIO6 dst bukan pin BOARD
GPIO.setup(6,GPIO.OUT) #GPIO6 pin 31

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
        
        
        if ta5 > 47:
            GPIO.output(6, GPIO.HIGH)
            time.sleep(3)
        else:
            GPIO.output(6, GPIO.LOW)



def listener():
    rospy.init_node('listener', anonymous=False)
    rospy.Subscriber('chatter', String, callback)
    rospy.spin()


if __name__ == '__main__':
    listener()





# global ir,ta1,ta2,ta3,ta4,ta5,ta6,ta7,ta8,uvtron
#     print(dataArdiuno)
#    rospy.loginfo('Data terbaca : %s', arduinoData.data)
# spin() simply keeps python from exiting until this node is stopped
#rospy.get_caller_id() +
#    print(arduinoData)
#    print()
#   dataTerima = int(arduinoData)
#   print(dataTerima)
#     print(dataTerima)
#     print(type(dataTerima))

