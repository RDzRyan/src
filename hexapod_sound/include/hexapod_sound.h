#ifndef HEXAPOD_SOUNDS_H_
#define HEXAPOD_SOUNDS_H_

#include <ros/ros.h>
#include <sound_play/sound_play.h>
#include <hexapod_msgs/Sounds.h>

class HexapodSound
{
public:
    HexapodSound(void);
    sound_play::SoundRequest sound_req_;
    hexapod_msgs::Sounds sounds_;
    ros::Publisher sound_pub_;
    void playSoundFile(std::string sound_file, int delay_time);

private:
    ros::NodeHandle nh_;
    std::string sound_package_path_;
    void soundsCallback(const hexapod_msgs::SoundsConstPtr &sounds_msg);
    ros::Subscriber sounds_sub_;
};

#endif // HEXAPOD_SOUNDS_H_
