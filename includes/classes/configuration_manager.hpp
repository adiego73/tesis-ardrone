#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "config/configutarion.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

namespace tesis
{

class ConfigurationManager
{
    public:
        static EnvironmentConfig *getEnvironmentConfiguration ( std::string path );
        static RobotConfig *getRobotConfiguration ( std::string path );

    private:
        ConfigurationManager();
};



};
#endif // CONFIGURATIONMANAGER_H
