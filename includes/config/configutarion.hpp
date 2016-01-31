#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <string>

namespace tesis
{

struct Range
{
    int min;
    int max;
};

struct Color
{
    Range Hue;
    Range Saturation;
    Range Value;
};

// struct PIDValues
// {
//     double Kp;
//     double Kd;
//     double Ki;
// };
// 
// struct PIDConfig
// {
//     PIDValues Roll;
//     PIDValues Pitch;
//     PIDValues Yaw;
// };


struct EnvironmentConfig
{
    std::vector< Color > safe_spot;
    std::vector< Color > unsafe_spot;
    Color robot_id;
    float camera_height;
    int camera_number;
};

};

#endif                                                      //  CONFIGURATION_HPP
