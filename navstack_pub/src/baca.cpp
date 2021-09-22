#include "ros/ros.h"
#include "std_msgs/String.h"
// #include "stdlib.h"


std_msgs::String Index1,Index2,Index3,Index4,Index5,Index6;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  // msg->data.c_str();
  std::getline(msg->data.c_str(), Index1.data, '|');
  std::getline(msg->data.c_str(), Index2.data, '|');
  std::getline(msg->data.c_str(), Index3.data, '|');
  ROS_INFO("I heard: [%s]", msg->data.c_str());
   ROS_INFO("[%s]",Index1.data);
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  ros::spin();
 
  return 0;
}