#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nav_msgs/Odometry.h>
//odom_pub_ = nh_.advertise<nav_msgs::Odometry>("/odom_data_quat", 50);
nav_msgs::Odometry gerak_;
double secs;

void chatterCallback(const nav_msgs::Odometry& odom)
{
  // gerak_.header.stamp=odom.header.stamp;
  secs =ros::Time::now().toSec();
  gerak_.pose.pose.position.x=odom.pose.pose.position.x;
  gerak_.pose.pose.position.y=odom.pose.pose.position.y;
  gerak_.pose.pose.position.z=odom.pose.pose.position.z;
  gerak_.pose.pose.orientation.z=odom.pose.pose.orientation.z;
  gerak_.pose.pose.orientation.w=odom.pose.pose.orientation.w;
  
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "read_odom");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("odom_data_quat", 50, chatterCallback);
  
  ros::Rate r(1); 
  while (ros::ok())
  {
    //... do some work ...
    ROS_INFO("%f, %f, %f, %f, %f, %f,", secs,gerak_.pose.pose.position.x,gerak_.pose.pose.position.y,gerak_.pose.pose.position.z,gerak_.pose.pose.orientation.z,gerak_.pose.pose.orientation.w);
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}