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
float laser[9]={2,2,2,2,2,2,2,2,2};

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    for(int i = 0; i < 9; i++) {
      laser[i] =scan->ranges[i*80];
      // if (scan->ranges[i*80]== ynf){
      //   laser[i]=3;
      // }
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
char a_gerak[]  ={'D'   ,'w'  };
float batasan0[]={6     ,6    };
float batasan1[]={0.372 ,0.385};
float batasan2[]={6     ,6    };
float batasan3[]={0.44  ,0.51 };
float batasan4[]={0.233 ,0.51 };
float batasan5[]={0.232 ,0.685};
float batasan6[]={6     ,6    };
float batasan7[]={6     ,6    };
float batasan8[]={6     ,6    };

// Init variables
float speed(0.5);                                                 // Linear velocity (m/s)
float turn(0.5);                                                  // Angular velocity (rad/s)
float x(0), y(0), z(0), xa(0), ya(0), za(0), xb(0), yb(0), th(0); // Forward/backward/neutral direction vars
char key(' ');
geometry_msgs::Twist twist;
int flag1=0;
// void kontrol(char arah_, float batas[9]){ //,nav_msgs::Odometry posisi_
void kontrol(char arah_, float batas0,float batas1,float batas2,float batas3,float batas4,float batas5,float batas6,float batas7,float batas8){
  
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
      ROS_INFO("%f, %f, %f, %f, %f, %f,",batas0, batas1, batas2,batas3, batas4,batas5);
       ROS_INFO("%f, %f, %f, %f, %f, %f,",laser[0],laser[1],laser[2],laser[3],laser[4],laser[5]);
    }

     // Update the Twist message
    twist.linear.x = x * speed;
    twist.linear.y = y * speed;
    twist.linear.z = z * speed;

    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = th * turn;
    
    
    if (laser[0]<=batas0 && laser[1]<=batas1 && laser[2]<=batas2 && laser[3]<=batas3 && laser[4]<=batas4 && laser[5]<=batas5 && laser[6]<=batas6 && laser[7]<=batas7 && laser[8]<=batas8 ){
      flag1++;
      ROS_INFO("clear");
    }
  
}

// int mode;
// void pergerakan(int mode_){ 
//   while(mode_==1){

//   }
//   while(mode_==2){

//   }
// }
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "Move_Control");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/scan", 50, scanCallback);
  ros::Subscriber sub1 = n.subscribe("/odom_data_quat", 50, chatterCallback);

  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1); 
  // ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  flag1=0;
  ros::Rate r(30); 
  while (ros::ok())
  {
    //baca setpoin
    // ROS_INFO("%f, %f, %f, %f, %f, %f,", secs,gerak_.pose.pose.position.x,gerak_.pose.pose.position.y,gerak_.pose.pose.position.z,gerak_.pose.pose.orientation.z,gerak_.pose.pose.orientation.w);
    // for(int i = 0; i < 9; i++) {
    //   ROS_INFO(": [%f]", laser[i]);
    // }

    //eksekusi
      kontrol(a_gerak[flag1],batasan0[flag1],batasan1[flag1],batasan2[flag1],batasan3[flag1],batasan4[flag1],batasan5[flag1],batasan6[flag1],batasan7[flag1],batasan8[flag1]);
      pub.publish(twist);
      ROS_INFO("step: %d", flag1);


    ros::spinOnce();
    r.sleep();
  }
  return 0;
}