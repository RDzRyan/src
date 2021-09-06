#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
// void Control::partitionCmd_vel(geometry_msgs::Twist *cmd_vel)
// cmd_vel_sub_ = nh_.subscribe<geometry_msgs::Twist>("/cmd_vel", 1, &Control::cmd_velCallback, this);
geometry_msgs::Twist cmd_vel_incoming_;


void chatterCallback(const geometry_msgs::Twist& cmd_vel_msg)
{
  // gerak_.header.stamp=odom.header.stamp;
  
  cmd_vel_incoming_.linear.x = cmd_vel_msg.linear.x;
  cmd_vel_incoming_.linear.y = cmd_vel_msg.linear.y;
  cmd_vel_incoming_.angular.z = cmd_vel_msg.angular.z;
  
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "read_cmd");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("cmd_vel", 1, chatterCallback);
  
  ros::Rate r(1); 
  while (ros::ok())
  {
    //... do some work ...
    ROS_INFO(" %f, %f, %f,", cmd_vel_incoming_.linear.x,cmd_vel_incoming_.linear.y ,cmd_vel_incoming_.angular.z);
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}