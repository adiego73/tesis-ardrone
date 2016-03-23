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

struct Spot
{
    Color color;
    int id;
    int altitude;
    float time;
    std::string comment;
    
};

struct EnvironmentConfig
{
    std::vector< Spot > safe_spot;
    std::vector< Color > unsafe_spot;
    std::vector< Color > robot_id;
    //Color robot_id;
    float camera_height;
    int camera_number;
    Size space;
    std::string path_videos;
};

};

#endif                                                      //  CONFIGURATION_HPP
