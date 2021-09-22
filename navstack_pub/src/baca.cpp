#include "ros/ros.h"
#include "std_msgs/String.h"
// #include "stdlib.h"


int Index1,Index2,Index3,Index4,Index5,Index6;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  msg->data.c_str();
  if (msg->data.c_str().length()>0)
{
Index1 = msg->data.c_str().indexOf('/');
Index2 = msg->data.c_str().indexOf('/', Index1+1);
Index3 = msg->data.c_str().indexOf('/', Index2+1);

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