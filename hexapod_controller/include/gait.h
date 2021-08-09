#ifndef GAIT_H_
#define GAIT_H_

#include <cmath> // std::abs
#include <algorithm>
#include <ros/ros.h>
#include <hexapod_msgs/FeetPositions.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Bool.h>

//=============================================================================
// Define structs and classes for gait system
//=============================================================================
class Gait
{
public:
    Gait(void);
    void gaitCycle(const geometry_msgs::Twist &cmd_vel, hexapod_msgs::FeetPositions *feet, geometry_msgs::Twist *gait_vel);

private:
    ros::Subscriber leg_height_sub_;
    void heightChange(const std_msgs::BoolConstPtr &height_msg);
    void cyclePeriod(const geometry_msgs::Pose2D &base, hexapod_msgs::FeetPositions *feet, geometry_msgs::Twist *gait_vel);
    void sequence_change(std::vector<int> &vec);
    geometry_msgs::Pose2D smooth_base_;
    ros::Time current_time_, last_time_;
    bool is_travelling_;         // True if the robot is moving, not just in a cycle
    bool in_cycle_;              // True if the robot is in a gait cycle
    int CYCLE_LENGTH;            // Number of steps in cycle
    int NUMBER_OF_LEGS;          // Leg order in cycle of the leg
    double LEG_LIFT_HEIGHT;      // Height of a leg cycle
    double LEG_LIFT_HEIGHT_LOW;  // Height of a leg cycle
    double LEG_LIFT_HEIGHT_HIGH; // Height of a leg cycle
    std::string GAIT_STYLE;      // gait style Tripod or Ripple
    int cycle_period_;           // Current period in cycle
    int extra_gait_cycle_;       // Forcing some extra timed cycles
    double period_distance;
    double period_height;
    double gait_factor;
    std::vector<int> cycle_leg_number_; // Leg gait order (grouping) ['RR', 'RM', 'RF', 'LR', 'LM', 'LF']
    ros::NodeHandle nh_;
};

#endif // GAIT_H_
