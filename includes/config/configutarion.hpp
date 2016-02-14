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

struct Size
{
    float height;
    float width;
};

struct EnvironmentConfig
{
    std::vector< Color > safe_spot;
    std::vector< Color > unsafe_spot;
    Color robot_id;
    float camera_height;
    int camera_number;
    Size space;
};

};

#endif                                                      //  CONFIGURATION_HPP
