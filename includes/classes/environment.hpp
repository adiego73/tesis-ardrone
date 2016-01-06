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
        Environment();
        ~Environment();

    private:
        std::vector<Point> safe_spots;
        std::vector<Point> unsafe_spots;
        std::vector<Point> robot_positions;
        Video *camera_video;
        bool is_visible;
};


};
#endif // ENVIRONMENT_HPP
