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

// Map for Base manipulation keys
std::map<char, std::vector<float>> baseBindings{
    //Body manipulating
    {'u', {-1, 0, 0, 0}},
    {'i', {1, 0, 0, 0}},
    {'j', {0, -1, 0, 0}},
    {'k', {0, 1, 0, 0}},
    //Head Manipulating
    {'o', {0, 0, -1, 0}},
    {'p', {0, 0, 1, 0}},
    {'l', {0, 0, 0, -1}},
    {';', {0, 0, 0, 1}}};

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

u : Body x-
i : Body x+
j : Body y-
k : Body y+

o : Head z-
p : Head z+
l : Head y-
; : Head y+

r : Stand up
t : Sit down
f : Normal Terrain
g : Uneven Terrain
v : IMU Override Off
b : IMU Override On

anything else : stop

CTRL-C to quit

)";

// Init variables
float speed(0.0);                                          // Linear velocity (m/s)
float turn(0.0);                                           // Angular velocity (rad/s)
float x(0), y(0), z(0), xa(0), ya(0), xb(0), yb(0), th(0); // Forward/backward/neutral direction vars
char key(' ');

// Function for non-blocking keyboard inputs
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
  ros::NodeHandle nh_;
  // Get ROS Parameters
  ros::param::get("MAX_METERS_PER_SEC", speed);
  ros::param::get("MAX_RADIANS_PER_SEC", turn);
  // Init publisher
  ros::Publisher pub = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  ros::Publisher state_pub_ = nh_.advertise<std_msgs::Bool>("/state", 100);
  ros::Publisher imu_override_pub_ = nh_.advertise<std_msgs::Bool>("/imu/imu_override", 100);
  ros::Publisher leg_height_pub_ = nh_.advertise<std_msgs::Bool>("/leg", 100);
  ros::Publisher body_scalar_pub_ = nh_.advertise<geometry_msgs::AccelStamped>("/body_scalar", 100);
  ros::Publisher head_scalar_pub_ = nh_.advertise<geometry_msgs::AccelStamped>("/head_scalar", 100);
  // Create message
  geometry_msgs::Twist twist;
  geometry_msgs::AccelStamped body_scalar_;
  geometry_msgs::AccelStamped head_scalar_;
  std_msgs::Bool state_;
  std_msgs::Bool imu_override_;
  std_msgs::Bool leg_height_;
  // Init Publisher variable
  state_.data = false;
  imu_override_.data = false;
  leg_height_.data = false;
  // Print Reminder Message
  printf("%s", msg);
  printf("\rCurrent: speed %f\tturn %f | Awaiting command...\r", speed, turn);
  // Main Program
  while (true)
  {
    ros::Time current_time = ros::Time::now();
    // Get the pressed key
    key = getch();
    if (key == 'r') // Stand Up button
    {
      if (state_.data == false)
      {
        imu_override_.data = false;
        state_.data = true;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Stand Up! ", speed, turn, key);
      }
    }

    else if (key == 't') // Sit Down button
    {
      if (state_.data == true)
      {
        imu_override_.data = false;
        state_.data = false;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Sit Down! ", speed, turn, key);
      }
    }

    else if (key == 'f') // Normal Terrain/ Short Leg Height
    {
      if (leg_height_.data == true)
      {
        imu_override_.data = false;
        leg_height_.data = false;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Normal Terrain ", speed, turn, key);
      }
    }

    else if (key == 'g') // Uneven Terrain/ Tall Leg Height
    {
      if (leg_height_.data == false)
      {
        imu_override_.data = false;
        leg_height_.data = true;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | Uneven Terrain ", speed, turn, key);
      }
    }

    else if (key == 'v') // Force IMU Override Off
    {
      if (imu_override_.data == true)
      {
        imu_override_.data = false;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | IMU Override Off ", speed, turn, key);
      }
    }

    else if (key == 'b') // Force IMU Override On
    {
      if (imu_override_.data == false)
      {
        imu_override_.data = true;
        printf("\rCurrent: speed %f\tturn %f | Last command: %c  | IMU Override On ", speed, turn, key);
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
      imu_override_.data = false;
      printf("\rCurrent: speed %f\tturn %f | Last command: %c   ", speed, turn, key);
    }

    // Otherwise if it corresponds to a key in baseBindings
    else if (baseBindings.count(key) == 1)
    {
      // Grab the manipulating data
      imu_override_.data = true;
      xa = baseBindings[key][0];
      ya = baseBindings[key][1];
      xb = baseBindings[key][2];
      yb = baseBindings[key][3];
      printf("\rCurrent: speed %f\tturn %f | Last command: %c   ", speed, turn, key);
    }

    // Otherwise, set the robot to stop
    else
    {
      x = 0;
      y = 0;
      z = 0;
      th = 0;
      xa = 0;
      ya = 0;
      xb = 0;
      yb = 0;
      imu_override_.data = false;

      // If ctrl-C (^C) was pressed, terminate the program
      if (key == '\x03')
      {
        printf("\n\n                 .     .\n              .  |\\-^-/|  .    \n             /| } O.=.O { |\\\n\n                 Bye Bye\n\n                RESP 2021\n\n\n");
        ros::shutdown();
        break;
      }

      printf("\rCurrent: speed %f\tturn %f | Invalid command! %c", speed, turn, key);
    }

    // Update the Twist message
    twist.linear.x = x * speed;
    twist.linear.y = y * speed;
    twist.linear.z = z * speed;

    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = th * turn;

    body_scalar_.header.stamp = current_time;
    body_scalar_.accel.angular.x = xa * turn;
    body_scalar_.accel.angular.y = ya * turn;

    head_scalar_.header.stamp = current_time;
    head_scalar_.accel.angular.z = xb * turn;
    head_scalar_.accel.angular.y = yb * turn;

    // Publish it and resolve any remaining callbacks
    pub.publish(twist);
    state_pub_.publish(state_);
    imu_override_pub_.publish(imu_override_);
    leg_height_pub_.publish(leg_height_);
    body_scalar_pub_.publish(body_scalar_);
    head_scalar_pub_.publish(head_scalar_);

    ros::spinOnce();
  }
  return 0;
}
