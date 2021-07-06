# ROS Hexapod Stack
## 1. Dependencies

```
sudo apt-get install git
sudo apt-get install ros-noetic-sound-play
sudo apt-get install ros-noetic-diagnostic-updater
sudo apt-get install ros-noetic-xacro
sudo apt-get install ros-noetic-openni2-launch
sudo apt-get install ros-noetic-depthimage-to-laserscan
sudo apt-get install ros-noetic-joystick-drivers
sudo apt-get install ros-noetic-imu-filter-madgwick
sudo apt-get install ros-noetic-robot-localization
sudo apt-get install ros-noetic-rtabmap
sudo apt-get install ros-noetic-rtabmap-ros
sudo apt-get install ros-noetic-robot-state-publisher
sudo apt-get install ros-noetic-gazebo-ros-control
sudo apt-get install ros-noetic-navigation
sudo apt-get install ros-noetic-move_base
sudo apt-get install ros-noetic-navfn
sudo apt-get install ros-noetic-amcl
sudo apt-get install libusb-1.0-0-dev
```

## 2. Nodes Descriptions

**_hexapod_controller_**

This is the main node of the stack. It handles all control, gait, IK and servo communications with the legs. Minimal latency was required to keep the gait smooth and synced with odometry hence the reason they are all combined in this one node.

*Subscribed Topics*

     cmd_vel (geometry_msgs/Twist) Velocity command. 
     body_scalar (geometry_msgs::AccelStamped) Scalar to modifiy the orientation of the body.
     head_scalar (geometry_msgs::AccelStamped) Scalar to modifiy the pan and tilt of the optional turret.
     state (std_msgs::Bool) Bool array to record state of the hexapod. Standing up, sitting etc.
     imu/data (sensor_msgs::Imu) Used in optional auto body leveling on non level ground.
     
*Published Topics*

    sounds (hexapod_msgs::Sounds) Custom message to send sound cues to the optional sound package.
    joint_states (sensor_msgs::JointState) Joint states for rviz and such.
    odometry/calculated (nav_msgs::Odometry) Calculated odometry from the gait system in the package.
    twist (geometry_msgs::TwistWithCovarianceStamped) Twist message syncronized with the gait system. 
     

**_hexapod_bringup_**

This package has all the launch files. From simple locomotion only to full mapping and localization examples. 

**_hexapod_description_**

This package has all the param files. You will start with one of the param config files to describe your hexapod. It also has params for different telop controllers. The xacro and meshes also reside in this package.


**Example Launch Command**
```
roslaunch hexapod_bringup hexapod_full.launch config:=phantomX joy_mapping:=joystick_ds3
```
## 3. Install

```
git clone https://github.com/KevinOchs/hexapod_ros.git . 
```

For Raspberry Pi2 please add these compiler optimizations after first build.
```
[workspace]/build/CMakeCache.txt
Change: CMAKE_CXX_FLAGS:STRING=-O3 -mfloat-abi=hard -mfpu=neon-vfpv4 -mcpu=cortex-a7
```

For ODROID XU3 please add these compiler optimizations after first build.
```
[workspace]/build/CMakeCache.txt
Change: CMAKE_CXX_FLAGS:STRING=-O3 -pipe -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=hard
```
