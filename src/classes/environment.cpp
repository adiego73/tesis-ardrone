#include "classes/environment.hpp"

using namespace tesis;

Environment::Environment( std::string config_path )
{
    EnvironmentConfiguration env_configuration(config_path);
    RobotConfiguration robot_configuration(config_path);
    
    this->env_config = env_configuration.get();
    this->robot_config = robot_configuration.get();
    this->camera_height = this->env_config.camera_height;
    this->video_camera.reset( new Video( this->env_config.camera_number ) );
}

Environment::~Environment()
{
}


