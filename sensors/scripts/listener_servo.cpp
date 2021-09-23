#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO("I Heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv){
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    ros::Subcriber sub = n.subscribe("chatter", 1000, chatterCallback);



    ros::spin();

    return 0;
}