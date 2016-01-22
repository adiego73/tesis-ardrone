#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "config/configutarion.hpp"

# include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <boost/smart_ptr.hpp>

#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

namespace tesis
{

class EnvironmentConfiguration
{
    public:
        EnvironmentConfiguration(std::string path);
        EnvironmentConfig get();

    private:
        EnvironmentConfig configuration;
        std::string path;

        void parse();
};

class RobotConfiguration
{
    public:
        RobotConfiguration( std::string path );
        RobotConfig get();

    private:
        RobotConfig configuration;
        std::string path;

        void parse();

};


};
#endif // CONFIGURATIONMANAGER_H


