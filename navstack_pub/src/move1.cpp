#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const nav_msgs::Odometry& odom)
{
 
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "Move_Control");
  ros::NodeHandle n;
  // ros::Subscriber sub = n.subscribe("odom_data_quat", 50, chatterCallback);
  
  ros::Rate r(200); 
  while (ros::ok())
  {
    //... do some work ...
    // ROS_INFO("%f, %f, %f, %f, %f, %f,", secs,gerak_.pose.pose.position.x,gerak_.pose.pose.position.y,gerak_.pose.pose.position.z,gerak_.pose.pose.orientation.z,gerak_.pose.pose.orientation.w);
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}