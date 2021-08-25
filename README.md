# ROS Hexapod Stack

https://rdzryan.github.io/src/

## 1. Dependencies

```
ros-noetic-sound-play
ros-noetic-diagnostic-updater
ros-noetic-xacro
ros-noetic-openni2-launch
ros-noetic-depthimage-to-laserscan
ros-noetic-joystick-drivers
ros-noetic-imu-filter-madgwick
ros-noetic-robot-localization
ros-noetic-rtabmap
ros-noetic-rtabmap-ros
ros-noetic-robot-state-publisher
ros-noetic-gazebo-ros-control
ros-noetic-navigation
ros-noetic-move-base
ros-noetic-navfn
ros-noetic-amcl
libusb-1.0-0-dev
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
     
```
hexapod_controller/include/servo_driver.h Dynamixel Driver Settings (USB Port, Protocol, Baud Rate)
```


**_hexapod_bringup_**

This package has all the launch files. From simple locomotion only to full mapping and localization examples. 

**_hexapod_description_**

This package has all the param files. You will start with one of the param config files to describe your hexapod. It also has params for different telop controllers. The xacro and meshes also reside in this package.

```
params/hexapod.yaml Describe robot parameters.
params/dynamixel.yaml Dynamixel Control Table
```

**_hexapod_twist_keyboard_**

Reading from the keyboard and Publishing to Twist!

```
Moving around:
   u    i    o
   j    k    l
   m    ,    .
   ```

```
For Holonomic mode (strafing), hold down the shift key:
   U    I    O
   J    K    L
   M    <    >
   ```

```
t : up (+z)
b : down (-z)
anything else : stop
q/z : increase/decrease max speeds by 10%
w/x : increase/decrease only linear speed by 10%
e/c : increase/decrease only angular speed by 10%
```

```CTRL-C to quit```


**Example Launch Command**
```
roslaunch hexapod_bringup hexapod_simple.launch
```
## 3. Install

Clone Git Repository :
```
git clone https://github.com/RDzRyan/src.git
```

Install All Dependencies :
```
sudo apt-get install -y ros-noetic-sound-play
sudo apt-get install -y ros-noetic-diagnostic-updater
sudo apt-get install -y ros-noetic-xacro
sudo apt-get install -y ros-noetic-openni2-launch
sudo apt-get install -y ros-noetic-depthimage-to-laserscan
sudo apt-get install -y ros-noetic-joystick-drivers
sudo apt-get install -y ros-noetic-imu-filter-madgwick
sudo apt-get install -y ros-noetic-robot-localization
sudo apt-get install -y ros-noetic-rtabmap
sudo apt-get install -y ros-noetic-rtabmap-ros
sudo apt-get install -y ros-noetic-robot-state-publisher
sudo apt-get install -y ros-noetic-gazebo-ros-control
sudo apt-get install -y ros-noetic-navigation
sudo apt-get install -y ros-noetic-move-base
sudo apt-get install -y ros-noetic-navfn
sudo apt-get install -y ros-noetic-amcl
sudo apt-get install -y libusb-1.0-0-dev
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

Fix DNS Problem :
```
echo "nameserver 8.8.8.8" | sudo tee /etc/resolv.conf > /dev/null
```
TEST :
```
ROBOT : AncabotK21 || COBA
```