#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

#include <nav_msgs/Odometry.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/AccelStamped.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/Imu.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <map>

#define RAD2DEG(x) ((x)*180./M_PI)
float laser[9]={5,5,5,5,5,5,5,5,5};

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    for(int i = 0; i < 8; i++) {
      laser[i] =scan->ranges[i*3];
      
      float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i*3);
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


// Map for movement keys
std::map<char, std::vector<float>> moveBindings{
    //Moving and Rotating
    {'q', {1, 0, 0, 1}},
    {'w', {1, 0, 0, 0}},
    {'e', {1, 0, 0, -1}},
    {'a', {0, 0, 0, 1}},
    {'s', {0, 0, 0, 0}},
    {'d', {0, 0, 0, -1}},
    {'z', {-1, 0, 0, -1}},
    {'x', {-1, 0, 0, 0}},
    {'c', {-1, 0, 0, 1}},
    //Holomonic Move
    {'Q', {1, -1, 0, 0}},
    {'W', {1, 0, 0, 0}},
    {'E', {1, 1, 0, 0}},
    {'A', {0, -1, 0, 0}},
    {'S', {0, 0, 0, 0}},
    {'D', {0, 1, 0, 0}},
    {'Z', {-1, -1, 0, 0}},
    {'X', {-1, 0, 0, 0}},
    {'C', {-1, 1, 0, 0}}};

//step
char a_gerak[]  ={'w','a','q','s','D','w','a','w','A','w','Q','C','d','w','s'};

std::map<int, std::vector<float>> step{
  // {1, {0,0,0,0,0,0,0,0}},
  // {0, {6, 0.35, 0.25, 0.33, 6, 6, 6, 6}} //kiri
  {0, {0.32,0,0,0,0,0,0,0}},
  {1, {0.432,0,0,0,0,0.25,0,0}},
  {2, {0.65,0,0,0,0.3,0.33,0,0}},

  {10, {0,0,0,0,0,0,0,0}} //kiri
  
};
std::map<int, std::vector<bool>> _f_{
  // {1, {false,false,false,false,false,false,false,false}},
  // {0, {true,true,true,true,true,true,true,true}},
  // {1, {true,true,true,true,true,true,true,true}}
  {0, {true,false,false,false,false,false,false,false}},
  {1, {false,false,false,false,false,true,false,false}},
  {2, {false,false,false,false,true,false,false,false}},

  {10, {true,true,true,true,true,true,true,true}}
};

// Init variables
float speed(0.5);                                                 // Linear velocity (m/s)
float turn(0.5);                                                  // Angular velocity (rad/s)
float x(0), y(0), z(0), xa(0), ya(0), za(0), xb(0), yb(0), th(0); // Forward/backward/neutral direction vars
char key(' ');
geometry_msgs::Twist twist;
int flag1=0;

void kontrol(char arah_, int step_){
  key=arah_;
  if (moveBindings.count(key) == 1)
    {
      // Grab the direction data
      x = moveBindings[key][0];
      y = moveBindings[key][1];
      z = moveBindings[key][2];
      th = moveBindings[key][3];
      // imu_override_.data = false;
      ROS_INFO("\rCurrent: speed %f\tturn %f | Last command: %c   ", speed, turn, key);
    }
  float batas[8];
  if (step.count(step_) == 1)
    {
      for(int a=0;a<8;a++){
        batas[a]=step[step_][a];
      }
    }
  bool flag_[8];
  if (_f_.count(step_) == 1)
    {
      for(int a=0;a<8;a++){
        flag_[a]=_f_[step_][a];
      }
    }
    ROS_INFO("%f, %f, %f, %f, %f, %f,%f, %f,", batas[0], batas[1], batas[2], batas[3], batas[4], batas[5], batas[6], batas[7]);
    ROS_INFO("%f, %f, %f, %f, %f, %f,%f, %f,",laser[0],laser[1],laser[2],laser[3],laser[4],laser[5],laser[6],laser[7]);
    ROS_INFO("%d, %d, %d, %d, %d, %d, %d, %d, ",flag_[0],flag_[1],flag_[2],flag_[3],flag_[4],flag_[5],flag_[6],flag_[7]);

     // Update the Twist message
    twist.linear.x = x * speed;
    twist.linear.y = y * speed;
    twist.linear.z = z * speed;

    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = th * turn;

    bool s[8]={false,false,false,false,false,false,false,false};

    for (int a=0; a<8; a++){
      if(flag_[a]==true){
        if(laser[a]<=batas[a])
        {
          s[a]=true;
        }
        else{s[a]=false;}
      }
      else{
        if(laser[a]>=batas[a])
        {
          s[a]=true;
        }
        else{s[a]=false;}
      }
    }
  ROS_INFO("%d, %d, %d, %d, %d, %d, %d, %d, ",s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
  if(s[0]==true && s[1]==true && s[2]==true && s[3]==true && s[4]==true && s[5]==true && s[6]==true && s[7]==true){
    flag1++;
    ROS_INFO("clear");
  }
}

 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "Move_Control");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/scan", 50, scanCallback);
  ros::Subscriber sub1 = n.subscribe("/odom_data_quat", 50, chatterCallback);
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1); 
  flag1=0;
  ros::Rate r(200); 
  while (ros::ok())
  {
    //baca setpoin
    // ROS_INFO("%f, %f, %f, %f, %f, %f,", secs,gerak_.pose.pose.position.x,gerak_.pose.pose.position.y,gerak_.pose.pose.position.z,gerak_.pose.pose.orientation.z,gerak_.pose.pose.orientation.w);
    // for(int i = 0; i < 9; i++) {
    //   ROS_INFO(": [%f]", laser[i]);
    // }

    //eksekusi
      kontrol(a_gerak[flag1],flag1);
      pub.publish(twist);
      ROS_INFO("step: %d", flag1);


    ros::spinOnce();
    r.sleep();
  }
  return 0;
}
