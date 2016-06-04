#ifndef ENVIRONMENT_CONFIGURATION_H
#define ENVIRONMENT_CONFIGURATION_H

#include "config/configutarion.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <exception>
#include <iostream>
#include <string>

namespace tesis
{

class EnvironmentConfiguration
{
public:
	EnvironmentConfiguration ( std::string path );
	EnvironmentConfig get();

	~EnvironmentConfiguration();
private:
	EnvironmentConfig configuration;
	std::string path;

	void parse();
};

};
#endif                                                      // ENVIRONMENT_CONFIGURATION_H


