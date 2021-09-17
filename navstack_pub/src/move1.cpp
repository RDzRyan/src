#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

#define RAD2DEG(x) ((x)*180./M_PI)
float laser[9];

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->scan_time / scan->time_increment;
    ROS_INFO("I heard a laser scan %s[%d]:", scan->header.frame_id.c_str(), count);
    // ROS_INFO("angle_range, %f, %f", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
    // ROS_INFO("aaaa %d, %d",scan->scan_time,scan->time_increment);
  
    for(int i = 0; i < 9; i++) {
      laser[i] =scan->ranges[i*80]);
      float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i*80);
      ROS_INFO(": [%f, %f]", degree, laser[i]);
    }

}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "Move_Control");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 50, scanCallback);

    ros::spin();

    return 0;
}