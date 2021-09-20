#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/AccelStamped.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/Imu.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <map>

char key(' ');

int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;
  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);
  // Get the current character
  ch = getchar();
  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}


int main(int argc, char **argv)
{
  // Init ROS node
  ros::init(argc, argv, "control_gripper");
  ros::NodeHandle nh_;
  // Init publisher

  ros::Publisher servo_pub_ = nh_.advertise<std_msgs::UInt16>("servo", 50);

  // Create message
  
  std_msgs::UInt16 sudut_;
  // Init Publisher variable
  
  sudut_.data = 0 ;
  
  while (true)
  {
    ros::Time current_time = ros::Time::now();
    // Get the pressed key
    key = getch();
    if (key == 'o')
    {
        sudut_.data= 0;

    }

    else if (key == 'p')
    {
      sudut_.data = 45;
    }
        servo_pub_.publish(sudut_);
    ros::spinOnce();
  }
  return 0;
}
