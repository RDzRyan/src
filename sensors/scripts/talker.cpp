#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <serial/serial.h>

serial::Serial ser;

int main(int argc, char **argv){

    ros::init(argc, argv, "talker");
    ros:: init NodeHandle n;

    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 100);

    ros::Rate loop_rate(10);

    try 
    {
   ser.setPort("/dev/ttyUSB-Arduino1");
   ser.setBaudrate(9600);
   serial::Timeout to = serial::Timeout::simpleTimeout(1000);
   ser.setTimeout(to);
   ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("serial port initialized");
    } 
    else{
         return -1;
    }

    int count = 0;
    while (ros::ok())
    {   
        if(ser.available()){
        ROS_INFO_STREAM("reading from serial port ");
        std_msgs::String result;
        result.data = ser.read(ser.available());
        ROS_INFO_STREAM("Read: " << result.data);
        chatter_pub.publish(result);

                    
    }

    ros::spinOnce();
    r.sleep();
             } 

}

