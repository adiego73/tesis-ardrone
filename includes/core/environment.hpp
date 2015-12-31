#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "structures.hpp"

namespace tesis {

class Environment {
  
public:
    Environment();
    ~Environment();

private:
    std::vector<Point> safe_spots;
    std::vector<Point> unsafe_spots;
    std::vector<Point> last_positions;
    //Robot ardrone;
    //Video camera_video;
    bool is_visible;
};


};
#endif // ENVIRONMENT_HPP
