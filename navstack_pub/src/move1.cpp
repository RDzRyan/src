#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

#include <nav_msgs/Odometry.h>

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
      // ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
    }

}

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

void kontrol(string arah_, float batas0,float batas1,float batas2,float batas3,float batas4,float batas5,float batas6,float batas7,float batas8,nav_msgs::Odometry posisi_){

}

int mode;
void pergerakan(int mode_){ 
  while(mode_==1){

  }
  while(mode_==2){

  }
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "Move_Control");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/scan", 50, scanCallback);
  ros::Subscriber sub = n.subscribe("odom_data_quat", 50, chatterCallback);

  ros::Rate r(1); 
  while (ros::ok())
  {
    //... do some work ...
    ROS_INFO("%f, %f, %f, %f, %f, %f,", secs,gerak_.pose.pose.position.x,gerak_.pose.pose.position.y,gerak_.pose.pose.position.z,gerak_.pose.pose.orientation.z,gerak_.pose.pose.orientation.w);
    for(int i = 0; i < 9; i++) {
      ROS_INFO(": [%f]", laser[i]);
    }
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}