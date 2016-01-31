#include "core/threads.hpp"

using namespace tesis;

void camera_thread( std::string env_config_path, boost::shared_ptr<MessageServer> messageServer )
{
    Environment env(env_config_path);
    
    bool quit = false;
    
    messageServer->announce("camera/robot_position");
    messageServer->announce("camera/next_destination");
    messageServer->announce("camera/destinations");
    
    while (!quit)
    {
        if (env.isRobotVisible())
        {
            std::string altitude_msg = messageServer->get("robot/altitude", "0");
            float robot_altitude = std::stof(altitude_msg);
        }
        
    }    
    
}
