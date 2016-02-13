#ifndef THREADS_HPP
#define THREADS_HPP

#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <chrono>

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>

#include "classes/environment.hpp"
#include "config/configutarion.hpp"
#include "core/message/message_server.hpp"

void gui_thread( boost::shared_ptr<tesis::MessageServer> messageServer );
void camera_thread( std::string env_config_path, boost::shared_ptr<tesis::MessageServer> messageServer );
void robot_thread( boost::shared_ptr<tesis::MessageServer> messageServer );


#endif
