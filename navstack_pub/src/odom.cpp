#include "ros/ros.h"
#include "std_msgs/String.h"
//odom_pub_ = nh_.advertise<nav_msgs::Odometry>("/odom_data_quat", 50);

void chatterCallback(const nav_msgs::Odometryr& odom)
{
  waktu =ros::Time::now();
  pos_x=odom.pose.pose.position.x;
  pos_y=odom.pose.pose.position.y;
  pos_z=odom.pose.pose.position.z;
  or_z=odom.pose.pose.orientation.z;
  or_w=odom.pose.pose.orientation.w;
  ROS_INFO("%d, %d, %d, %d, %d, %d,", waktu,pos_x,pos_y,pos_z,or_z,or_w);
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "read odom");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("odom_data_quat", 50, chatterCallback);
  ros::spin();
 
  return 0;
}