#include <ros/ros.h>
#include <control.h>
#include <gait.h>
#include <ik.h>
#include <servo_driver.h>


#include <geometry_msgs/PoseStamped.h>


// Initial pose

// Flag to see if initial pose has been received
// bool initialPoseRecieved = false;
// void set_initial_2d(const geometry_msgs::PoseStamped &rvizClick) {
 
//   control.odomOld.pose.pose.position.x = rvizClick.pose.position.x;
//   control.odomOld.pose.pose.position.y = rvizClick.pose.position.y;
//   control.odomOld.pose.pose.orientation.z = rvizClick.pose.orientation.z;
//   control.initialPoseRecieved = true;
// }
 
//=============================================================================
// Main
//=============================================================================

int main(int argc, char **argv)
{
    ros::init(argc, argv, "hexapod_controller");

    // ros::NodeHandle node;
    // ros::Subscriber subInitialPose = node.subscribe("initial_2d", 1, set_initial_2d);

    // Create class objects
    Control control;
    Gait gait;
    Ik ik;
    ServoDriver servoDriver;

    // Establish initial leg positions for default pose in robot publisher
    gait.gaitCycle(control.cmd_vel_, &control.feet_, &control.gait_vel_);
    ik.calculateIK(control.feet_, control.body_, &control.legs_);
    control.publishJointStates(control.legs_, control.head_, &control.joint_state_);
    control.publishOdometry(control.gait_vel_);
    control.publishTwist(control.gait_vel_);

    ros::Time current_time_, last_time_;
    current_time_ = ros::Time::now();
    last_time_ = ros::Time::now();

    ros::AsyncSpinner spinner(2); // Using 2 threads
    spinner.start();
    ros::Rate loop_rate(control.MASTER_LOOP_RATE); // Speed limit of loop ( Will go slower than this )
    while (ros::ok())
    {
        current_time_ = ros::Time::now();
        double dt = (current_time_ - last_time_).toSec();

        // Divide cmd_vel by the loop rate to get appropriate velocities for gait period
        control.partitionCmd_vel(&control.cmd_vel_);

        // Start button on controller has been pressed stand up
        if (control.getHexActiveState() == true && control.getPrevHexActiveState() == false)
        {
            ROS_INFO("Hexapod standing up.");
            while (control.body_.position.z < control.STANDING_BODY_HEIGHT)
            {
                control.body_.position.z = control.body_.position.z + 0.001; // 1 mm increment

                // IK solver for legs and body orientation
                ik.calculateIK(control.feet_, control.body_, &control.legs_);

                // Commit new positions and broadcast over USB2AX as well as jointStates
                control.publishJointStates(control.legs_, control.head_, &control.joint_state_);
                servoDriver.transmitServoPositions(control.joint_state_);
                control.publishOdometry(control.gait_vel_);
                control.publishTwist(control.gait_vel_);
            }
            control.setPrevHexActiveState(true);
        }

        // We are live and standing up
        if (control.getHexActiveState() == true && control.getPrevHexActiveState() == true)
        {
            // Gait Sequencer
            gait.gaitCycle(control.cmd_vel_, &control.feet_, &control.gait_vel_);
            control.publishTwist(control.gait_vel_);

            // IK solver for legs and body orientation
            ik.calculateIK(control.feet_, control.body_, &control.legs_);

            // Commit new positions and broadcast over USB2AX as well as jointStates
            control.publishJointStates(control.legs_, control.head_, &control.joint_state_);
            servoDriver.transmitServoPositions(control.joint_state_);
            control.publishOdometry(control.gait_vel_);
            control.publishTwist(control.gait_vel_);

            // Set previous hex state of last loop so we know if we are shutting down on the next loop
            control.setPrevHexActiveState(true);
        }

        // Shutting down hex so let us do a gradual sit down and turn off torque
        if (control.getHexActiveState() == false && control.getPrevHexActiveState() == true)
        {
            ROS_INFO("Hexapod sitting down.");
            while (control.body_.position.z > 0)
            {
                control.body_.position.z = control.body_.position.z - 0.001; // 1 mm increment

                // Gait Sequencer called to make sure we are on all six feet
                gait.gaitCycle(control.cmd_vel_, &control.feet_, &control.gait_vel_);

                // IK solver for legs and body orientation
                ik.calculateIK(control.feet_, control.body_, &control.legs_);

                // Commit new positions and broadcast over USB2AX as well as jointStates
                control.publishJointStates(control.legs_, control.head_, &control.joint_state_);
                servoDriver.transmitServoPositions(control.joint_state_);
                control.publishOdometry(control.gait_vel_);
                control.publishTwist(control.gait_vel_);
            }

            // Release torque
            ros::Duration(0.5).sleep();
            servoDriver.freeServos();
            ROS_INFO("Hexapod servos torque is now off.");

            // Locomotion is now shut off
            control.setPrevHexActiveState(false);
        }
        // Sitting down with servo torque off. Publish jointState message every half second
        if (control.getHexActiveState() == false && control.getPrevHexActiveState() == false)
        {
            ros::Duration(0.5).sleep();
            control.publishJointStates(control.legs_, control.head_, &control.joint_state_);
            control.publishOdometry(control.gait_vel_);
            control.publishTwist(control.gait_vel_);
        }

        loop_rate.sleep();
        last_time_ = current_time_;
    }
    return 0;
}
