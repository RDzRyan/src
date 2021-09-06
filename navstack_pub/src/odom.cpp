#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nav_msgs/Odometry.h>
//odom_pub_ = nh_.advertise<nav_msgs::Odometry>("/odom_data_quat", 50);
ros::Time waktu;;

void chatterCallback(const nav_msgs::Odometryr& odom)
{
  gerak_.header.stamp=odom.header.stamp;
  gerak_.pose.pose.position.x=odom.pose.pose.position.x;
  gerak_.pose.pose.position=odom.pose.pose.position.y;
  gerak_.pose.pose.position=odom.pose.pose.position.z;
  gerak_.pose.pose.orientation.z=odom.pose.pose.orientation.z;
  gerak_.pose.pose.orientation.w=odom.pose.pose.orientation.w;
  ROS_INFO("%d, %f, %f, %f, %f, %f,", gerak_.header.stamp,gerak_.pose.pose.position.x,gerak_.pose.pose.position.y,gerak_.pose.pose.position.z,gerak_.pose.pose.orientation.z,gerak_.pose.pose.orientation.w);
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "read odom");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("odom_data_quat", 50, chatterCallback);
  ros::spin();
 
  return 0;
}