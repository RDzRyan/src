import rospy
from std_msgs.msg import Bool
    
def callback(data):
    rospy.loginfo("I heard %s",data.data)
     
def listener():
     rospy.init_node('reader')
     rospy.Subscriber("pushed", Bool, callback)
     # spin() simply keeps python from exiting until this node is stopped
     rospy.spin()