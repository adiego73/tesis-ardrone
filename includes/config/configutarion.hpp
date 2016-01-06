#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <string>

namespace tesis
{

typedef struct
{
    int min;
    int max;
} Range;

typedef struct
{
    Range Hue;
    Range Saturation;
    Range Value;
} Color ;

typedef struct
{
    double Kp;
    double Kd;
    double Ki;
} PIDValues;

typedef struct
{
    PIDValues Roll;
    PIDValues Pitch;
    PIDValues Yaw;
} PIDConfig;

typedef struct
{
    std::vector< Color > safe_spot;
    std::vector< Color > unsafe_spot;
} EnvironmentConfig;

typedef struct
{
    PIDConfig PID;
    std::string address;
} RobotConfig;

};

#endif                                                      //  CONFIGURATION_HPP
