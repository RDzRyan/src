#ifndef IK_H_
#define IK_H_

#include <cmath> // acos atan
#include <ros/ros.h>
#include <hexapod_msgs/Pose.h>
#include <hexapod_msgs/LegsJoints.h>
#include <hexapod_msgs/FeetPositions.h>

//=============================================================================
// Define structs and classes for gait system
//=============================================================================

struct Trig
{
    double sine;
    double cosine;
};

class Ik
{
public:
    Ik(void);
    void calculateIK(const hexapod_msgs::FeetPositions &feet, const hexapod_msgs::Pose &body, hexapod_msgs::LegsJoints *legs);

private:
    Trig getSinCos(double angle_rad);
    std::vector<double> COXA_TO_CENTER_X, COXA_TO_CENTER_Y;                // Distance from coxa joint to the center pivot
    std::vector<double> INIT_COXA_ANGLE;                                   // Initial coxa offsets in radians
    std::vector<double> INIT_FOOT_POS_X, INIT_FOOT_POS_Y, INIT_FOOT_POS_Z; // Start position Of feet
    double COXA_LENGTH, FEMUR_LENGTH, TIBIA_LENGTH, TARSUS_LENGTH;         // Leg segment measurements
    int NUMBER_OF_LEGS;                                                    // Number of legs
};

#endif // IK_H_
