#include <hexapod_sound.h>
#include <ros/package.h>

//==============================================================================
// Constructor
//==============================================================================

HexapodSound::HexapodSound(void)
{
    sound_pub_ = nh_.advertise<sound_play::SoundRequest>("/robotsound", 1, 0);
    sounds_sub_ = nh_.subscribe<hexapod_msgs::Sounds>("/sounds", 1, &HexapodSound::soundsCallback, this);
    sound_package_path_ = ros::package::getPath("hexapod_sound");
}

void HexapodSound::soundsCallback(const hexapod_msgs::SoundsConstPtr &sounds_msg)
{
    if (sounds_msg->stand == true)
    {
        if (sounds_.stand != true)
        {
            sounds_.stand = true;
        }
    }

    if (sounds_msg->shut_down == true)
    {
        if (sounds_.shut_down != true)
        {
            sounds_.shut_down = true;
        }
    }

    if (sounds_msg->waiting == true)
    {
        if (sounds_.waiting != true)
        {
            sounds_.waiting = true;
        }
    }

    if (sounds_msg->auto_level == true)
    {
        if (sounds_.auto_level != true)
        {
            sounds_.auto_level = true;
        }
    }
}

void HexapodSound::playSoundFile(std::string sound_file, int delay_time)
{
    sound_req_.volume = 1;
    sound_req_.sound = sound_play::SoundRequest::PLAY_FILE;
    sound_req_.command = sound_play::SoundRequest::PLAY_ONCE;
    sound_req_.arg = sound_package_path_ + "/sounds/" + sound_file; // need to due this due to bug in sound_play
    sound_pub_.publish(sound_req_);
    ros::Duration(delay_time).sleep();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "hexapod_sound");
    HexapodSound hexapodSound;

    hexapodSound.playSoundFile("empty.ogg", 3);
    hexapodSound.playSoundFile("intelChime.ogg", 3);
    hexapodSound.playSoundFile("activeAwaitingCommands.ogg", 3);

    ros::AsyncSpinner spinner(1); // Using 1 threads
    spinner.start();
    ros::Rate loop_rate(10); // 10 hz
    while (ros::ok())
    {
        if (hexapodSound.sounds_.stand == true)
        {
            hexapodSound.playSoundFile("standingUp.ogg", 3);
            hexapodSound.sounds_.stand = false;
        }

        if (hexapodSound.sounds_.auto_level == true)
        {
            hexapodSound.playSoundFile("autoLevelingBody.ogg", 6);
            hexapodSound.sounds_.auto_level = false;
        }

        if (hexapodSound.sounds_.waiting == true)
        {
            hexapodSound.playSoundFile("activeAwaitingCommands.ogg", 3);
            hexapodSound.sounds_.waiting = false;
        }

        if (hexapodSound.sounds_.shut_down == true)
        {
            hexapodSound.playSoundFile("shuttingDown.ogg", 3);
            hexapodSound.sounds_.shut_down = false;
        }

        loop_rate.sleep();
    }
}
