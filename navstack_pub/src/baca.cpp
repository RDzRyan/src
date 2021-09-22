#include "ros/ros.h"
#include "std_msgs/String.h"
// #include "stdlib.h"


int Index1,Index2,Index3,Index4,Index5,Index6;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  string myString=msg->data.c_str();
  if (myString.length()>0)
{
Index1 = myString.indexOf('|');
Index2 = myString.indexOf('|', Index1+1);
Index3 = myString.indexOf('|', Index2+1);
Index4 = myString.indexOf('|', Index3+1);
Index5 = myString.indexOf('|', Index4+1);
Index6 = myString.indexOf('|', Index5+1);
}
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  ros::spin();
 
  return 0;
}