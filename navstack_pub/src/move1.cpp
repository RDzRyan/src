#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

#include <nav_msgs/Odometry.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/AccelStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
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


float xaa[8],yaa[8],xas[8];
bool ff1,ff2,ff3;
// void chatterCallback(const nav_msgs::Odometry& odom)
// {
//   xaa[0]=odom.pose.pose.position.x;
//   xaa[1]=odom.pose.pose.position.y;
//   xaa[2]=odom.pose.pose.position.z;
//   xaa[3]=odom.pose.pose.orientation.z;
//   xaa[4]=odom.pose.pose.orientation.w;
//   if (ff==false){
//     yaa[0]=xaa[0];
//     yaa[1]=xaa[1];
//     yaa[2]=xaa[2];
//     yaa[3]=xaa[3];
//     yaa[4]=xaa[4];
//     ff=true;
//   }
// }
void chatter1Callback(const std_msgs::Float32& msg)
{
  xaa[0]=msg.data;
  ROS_INFO("I heard: [%f]", xaa[0]);
  if(ff1==false){ yaa[0]=xaa[0]; ff1=true;}
}

void chatter2Callback(const std_msgs::Float32& msg)
{
  xaa[1]=msg.data;
  ROS_INFO("I heard: [%f]", xaa[1]);
  if(ff2==false){ yaa[1]=xaa[1];ff2=true;}
}

void chatter3Callback(const std_msgs::Float32& msg)
{
  xaa[2]=msg.data;
  ROS_INFO("I heard: [%f]", xaa[2]);
  if(ff3==false){ yaa[2]=xaa[2];ff3=true;}
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
char a_gerak[]  ={'s','d','w','a','w','a','w','A','w','s','w','A','w','Q','C','d','w','s'};

std::map<int, std::vector<float>> step{
  // {1, {0,0,-2,0,0,0,0,0,0.5,0.5}},   //batas 0-7, speed, turn  //rotate kanan
  {0, {0,0,0,0,0,0,0,0,0,0}},
  // {1, {-0.05,0,0,0,0,0,0,0,0.5,0.5}},   //batas 0-7, speed, turn  //maju
  {1, {-4,-4,-2.605,0,0,0,0,0,0.5,0.5}},
  {2, {0.28,-4,-4,0,0,0,0,0, 0.5,0.5}},
  {3, {-4,-4,2.604,0,0,0,0,0,0.5,0.5}},     //rotate 90 derajat kanan d -2.604713
  {4, {0.864,-4,-4,0,0,0,0,0,0.5,0.5}},
  {5, {-4,-4,2.604,0,0,0,0,0,0.5,0.5}},
  {6, {0.823,-4,-4,0,0,0,0,0,0.5,0.5}},
  {7, {-4,0.34,-4,0,0,0,0,0,0.5,0.5}},
  {8, {0.178811,-4,-4,0,0,0,0,0,0.5,0.5}},

  {9, {-4,-4,-0.131329,1,0,0,0,0,0.5,0.5}}, // Mundur setelah mengambil korban dan memadamkan api
  {10, {0,0,-5.393876,0,0,0,0,0,0.5,0.5}}, // Rotate Kanan (180)
  {11, {0.396885,-4,-4,0,0,0,0,0,0.5,0.5}},
  
  {12, {-0.363460,-4,-4,0,0,0,0,0,0.5,0.5}}, // Mundur setelah menyimpan korban 
  {13, {-4,-4,2.260794,0,0,0,0,0,0.5,0.5}}, // Rotate Kiri __menuju Ruang 2 (90)
  {14, {0.393443,-4,-4,0,0,0,0,0,0.5,0.5}}, // Maju __menuju Ruang 2
  {15, {-4,-4,-2.7645,0,0,0,0,0,0.5,0.5}}, // Rotate Kanan Menuju __menuju Ruang 2 (90)
  {16, {0.821873,-4,-4,0,0,0,0,0,0.5,0.5}}, // maju dari lorong Ruang 1 __menuju Ruang 2
  {17, {-4,-4,2.468280,0,0,0,0,0,0.5,0.5}}, // rotate kiri __menuju Ruang 2 (90)
  {18, {0.831475,-4,-4,0,0,0,0,0,0.5,0.5}}, // maju dari lorong menuju ke Ruang 2 __menuju Ruang 2
  {19, {-4,-4,2.260794,0,0,0,0,0,0.5,0.5}}, // Rotate kiri menuju Ruang 2 (90)
  {20, {0.294402,-4,-4,0,0,0,0,0,0.5,0.5}}, // maju menuju titik pemadaman  



  {21, {0,0,0,0,0,0,0,0,0,0}}
  
};
std::map<int, std::vector<bool>> _f_{
  // {1, {0,0,1,0,0,0,0,0,0}},  //kompar 0-7 (0)(L>=b) (1)(L<=b), LaserOrOdom(1=lase && 0=odom) //odom
  {0, {0,0,0,0,0,0,0,0,0}},
  {1, {0,0,1,0,0,0,0,0,0}},
  {2, {0,0,0,0,0,0,0,0,0}},
  {3, {0,0,0,0,0,0,0,0,0}},
  {4, {0,0,0,0,0,0,0,0,0}},
  {5, {0,0,0,0,0,0,0,0,0}},
  {6, {0,0,0,0,0,0,0,0,0}},
  {7, {0,0,0,0,0,0,0,0,0}},

  {8, {0,0,0,0,0,0,0,0,0}},
  {9, {0,0,0,0,0,0,0,0,0}},

  {10, {1,1,1,1,1,1,1,1,1}},
  {11, {1,1,1,1,1,1,1,1,1}},
  {12, {1,1,1,1,1,1,1,1,1}},
  {13, {1,1,1,1,1,1,1,1,1}},
  {14, {1,1,1,1,1,1,1,1,1}},
  {15, {1,1,1,1,1,1,1,1,1}},
  {16, {1,1,1,1,1,1,1,1,1}},
  {17, {1,1,1,1,1,1,1,1,1}},
  {18, {1,1,1,1,1,1,1,1,1}},
  {19, {1,1,1,1,1,1,1,1,1}},

  {20, {0,0,0,0,0,0,0,0,0}}
};

// Init variables
float speed(0.5);                                                 // Linear velocity (m/s)
float turn(0.5);                                                  // Angular velocity (rad/s)
float x(0), y(0), z(0), xa(0), ya(0), za(0), xb(0), yb(0), th(0); // Forward/backward/neutral direction vars
char key(' ');
geometry_msgs::Twist twist;
int flag1=0;
bool pilih;
void kontrol(char arah_, int step_){
  key=arah_;
  float batas[8];
  if (step.count(step_) == 1)
    {
      for(int a=0;a<8;a++){
        batas[a]=step[step_][a];
      }
      speed=step[step_][8];
      turn=step[step_][9];
    }

  bool flag_[8];
  if (_f_.count(step_) == 1)
    {
      for(int a=0;a<8;a++){
        flag_[a]=_f_[step_][a];
      }
    pilih=_f_[step_][8];
    }

  if (moveBindings.count(key) == 1)
    {
      // Grab the direction data
      x = moveBindings[key][0];
      y = moveBindings[key][1];
      z = moveBindings[key][2];
      th = moveBindings[key][3];
      
      ROS_INFO("\rCurrent: speed %f   | turn %f | Last command: %c   ", speed, turn, key);
    }

    // Update the Twist message
    twist.linear.x = x * speed;
    twist.linear.y = y * speed;
    twist.linear.z = z * speed;

    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = th * turn;

  
    ROS_INFO("%f, %f, %f, %f, %f, %f,%f, %f,", batas[0], batas[1], batas[2], batas[3], batas[4], batas[5], batas[6], batas[7]);
    ROS_INFO("%f, %f, %f, %f, %f, %f,%f, %f,",laser[0],laser[1],laser[2],laser[3],laser[4],laser[5],laser[6],laser[7]);
    ROS_INFO("%d, %d, %d, %d, %d, %d, %d, %d, ",flag_[0],flag_[1],flag_[2],flag_[3],flag_[4],flag_[5],flag_[6],flag_[7]);


    bool s[8]={false,false,false,false,false,false,false,false};

  if(pilih==true){
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
      yaa[a]=xaa[a];
    }
  }

  else{

    for (int a=0; a<8; a++){
      xas[a]=xaa[a]-yaa[a];
      if(flag_[a]==true){
        if(xas[a]<=batas[a])
        {
          s[a]=true;
        }
        else{s[a]=false;}
      }
      else{
        if(xas[a]>=batas[a])
        {
          s[a]=true;
        }
        else{s[a]=false;}
      }
    }
  }

  ROS_INFO("%d, %d, %d, %d, %d, %d, %d, %d, ",s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
  
  if(s[0]==true && s[1]==true && s[2]==true && s[3]==true && s[4]==true && s[5]==true && s[6]==true && s[7]==true){
    flag1++;
    ROS_INFO("clear");
    yaa[0]=xaa[0];
    yaa[1]=xaa[1];
    yaa[2]=xaa[2];
  }
}

 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "Move_Control");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/scan", 50, scanCallback);
  // ros::Subscriber sub1 = n.subscribe("/odom_data_quat", 50, chatterCallback);

  ros::Subscriber _sub1 = n.subscribe("/chatter1", 1, chatter1Callback);
  ros::Subscriber _sub2 = n.subscribe("/chatter2", 1, chatter2Callback);
  ros::Subscriber _sub3 = n.subscribe("/chatter3", 1, chatter3Callback);

  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1); 
  flag1=0;
  ros::Rate r(100); 
  while (ros::ok())
  {
    //baca setpoin
     ROS_INFO("-------------------------");
     ROS_INFO("%f, %f, %f, %f, %f",xas[0],xas[1],xas[2],xas[3],xas[4]);
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