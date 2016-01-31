#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "structures.hpp"
#include "classes/environment_configuration.hpp"
#include "classes/video.hpp"

namespace tesis
{

class Environment
{
    public:
        Point getRobotPosition();
        std::vector<Point> getDestinations();
        bool isRobotVisible();
        
        Environment(std::string config_path);
        ~Environment();

    private:
        EnvironmentConfig env_config;
        
        std::vector<Point> safe_spots;
        std::vector<Point> unsafe_spots;
        std::vector<Point> robot_positions;
        
        boost::shared_ptr<Video> video_camera;
        
        float camera_height;
        Point robot_position;
};


};
#endif // ENVIRONMENT_HPP
