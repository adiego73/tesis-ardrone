#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "structures.hpp"
#include "classes/configuration_manager.hpp"
#include "classes/video.hpp"

namespace tesis
{

class Environment
{
    public:
        Point getRobotPosition();
        std::vector<Point> getDestinations();
        
        Environment(std::string config_path);
        ~Environment();

    private:
        EnvironmentConfig env_config;
//         RobotConfig robot_config;
        
        std::vector<Point> safe_spots;
        std::vector<Point> unsafe_spots;
        std::vector<Point> robot_positions;
        
        boost::shared_ptr<Video> video_camera;
        
        float camera_height;
        bool is_visible;
};


};
#endif // ENVIRONMENT_HPP
