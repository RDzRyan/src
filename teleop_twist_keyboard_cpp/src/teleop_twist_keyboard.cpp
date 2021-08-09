#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/AccelStamped.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/Imu.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <map>

// Map for movement keys
std::map<char, std::vector<float>> moveBindings{
    {'q', {1, 0, 0, 1}},
    {'w', {1, 0, 0, 0}},
    {'e', {1, 0, 0, -1}},
    {'a', {0, 0, 0, 1}},
    {'s', {0, 0, 0, 0}},
    {'d', {0, 0, 0, -1}},
    {'z', {-1, 0, 0, -1}},
    {'x', {-1, 0, 0, 0}},
    {'c', {-1, 0, 0, 1}},

    {'Q', {1, -1, 0, 0}},
    {'W', {1, 0, 0, 0}},
    {'E', {1, 1, 0, 0}},
    {'A', {0, -1, 0, 0}},
    {'S', {0, 0, 0, 0}},
    {'D', {0, 1, 0, 0}},
    {'Z', {-1, -1, 0, 0}},
    {'X', {-1, 0, 0, 0}},
    {'C', {-1, 1, 0, 0}}};

// Map for speed keys
std::map<char, std::vector<float>> speedBindings{
    {'u', {1, 0, 0}},
    {'i', {-1, 0, 0}},
    {'j', {0, 1, 0}},
    {'k', {0, -1, 0}},
    {'m', {0, 0, 1}},
    {',', {0, 0, -1}},
};

// Reminder message
const char *msg = R"(

Reading from the keyboard and Publishing to Twist!
---------------------------
Moving around:
   q    w    e
   a    s    d
   z    x    c

For Holonomic mode (strafing), hold down the shift key:
---------------------------
   Q    W    E
   A    S    D
   Z    X    C
u/i : Body Roll
j/k : Body Pitch
m/, : Body Yaw

r : Stand up
f : Sit down
t : Normal Terrain
g : Uneven Terrain

anything else : stop

CTRL-C to quit

)";

// Init variables
float speed(0.1);                                   // Linear velocity (m/s)
float turn(0.38);                                   // Angular velocity (rad/s)
float x(0), y(0), z(0), xb(0), yb(0), zb(0), th(0); // Forward/backward/neutral direction vars
char key(' ');

// For non-blocking keyboard inputs
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
  ros::init(argc, argv, "teleop_twist_keyboard");
  ros::param::get("MAX_METERS_PER_SEC", speed);
  ros::param::get("MAX_RADIANS_PER_SEC", turn);
  ros::NodeHandle nh_;
  // Init cmd_vel publisher
  ros::Publisher pub = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  ros::Publisher imu_pub_ = nh_.advertise<sensor_msgs::Imu>("/imu/data", 1);
  ros::Publisher body_pub_ = nh_.advertise<geometry_msgs::AccelStamped>("/body_scalar", 1);
  ros::Publisher state_pub_ = nh_.advertise<std_msgs::Bool>("/state", 100);
  ros::Publisher imu_override_pub_ = nh_.advertise<std_msgs::Bool>("/imu/imu_override", 100);
  ros::Publisher leg_height_pub_ = nh_.advertise<std_msgs::Bool>("/leg", 100);

  // Create Twist message
  geometry_msgs::Twist twist;
  geometry_msgs::AccelStamped body_;
  sensor_msgs::Imu imu_;
  std_msgs::Bool state_;
  std_msgs::Bool imu_override_;
  std_msgs::Bool leg_height_;

  state_.data = false;
  imu_override_.data = false;
  leg_height_.data = false;

  printf("%s", msg);
  printf("\rCurrent: speed %f\tturn %f | Awaiting command...\r", speed, turn);

  while (true)
  {

    // Get the pressed key
    key = getch();

    if (key == 'r')
    {
      if (state_.data == false)
      {
        state_.data = true;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Stand Up! ", speed, turn, key);
      }
    }

    else if (key == 'f')
    {
      if (state_.data == true)
      {
        state_.data = false;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Sit Down! ", speed, turn, key);
      }
    }

    else if (key == 't')
    {
      if (leg_height_.data == true)
      {
        leg_height_.data = false;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Normal Terrain ", speed, turn, key);
      }
    }

    else if (key == 'g')
    {
      if (leg_height_.data == false)
      {
        leg_height_.data = true;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Uneven Terrain ", speed, turn, key);
      }
    }

    // If the key corresponds to a key in moveBindings
    else if (moveBindings.count(key) == 1)
    {
      // Grab the direction data
      x = moveBindings[key][0];
      y = moveBindings[key][1];
      z = moveBindings[key][2];
      th = moveBindings[key][3];

      printf("\rCurrent: speed %f\tturn %f | Last command: %c   ", speed, turn, key);
    }

    // Otherwise if it corresponds to a key in speedBindings
    else if (speedBindings.count(key) == 1)
    {
      xb = speedBindings[key][0];
      yb = speedBindings[key][1];
      zb = speedBindings[key][2];

      printf("\rCurrent: speed %f\tturn %f | Last command: %c   ", speed, turn, key);
    }

    // Otherwise, set the robot to stop
    else
    {
      x = 0;
      y = 0;
      z = 0;
      th = 0;
      xb = 0;
      yb = 0;
      zb = 0;

      // If ctrl-C (^C) was pressed, terminate the program
      if (key == '\x03')
      {
        printf("\n\n                 .     .\n              .  |\\-^-/|  .    \n             /| } O.=.O { |\\\n\n                 Bye Bye\n\n               RESP 2021");
        break;
      }

      printf("\rCurrent: speed %f\tturn %f | Invalid command! %c", speed, turn, key);
    }

    // Update the Twist message
    twist.linear.x = x * speed;
    twist.linear.y = y * speed;
    twist.linear.z = z * speed;

    imu_.linear_acceleration.x = xb;
    imu_.linear_acceleration.y = yb;
    imu_.linear_acceleration.z = zb;
    
    imu_.orientation.x = xb;
    imu_.orientation.y = yb;
    imu_.orientation.z = zb;

    body_.accel.angular.x = xb;
    body_.accel.angular.y = yb;
    body_.accel.angular.z = zb;

    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = th * turn;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    body_pub_.publish(body_);
    imu_pub_.publish(imu_);
    state_pub_.publish(state_); // Always publish for means of an emergency shutdown type situation
    imu_override_pub_.publish(imu_override_);
    leg_height_pub_.publish(leg_height_);
    ros::spinOnce();
  }

  return 0;
}
