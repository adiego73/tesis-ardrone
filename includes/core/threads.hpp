#ifndef THREADS_HPP
#define THREADS_HPP

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include "config/configutarion.hpp"
#include "core/message/message_server.hpp"

void gui_thread( tesis::EnvironmentConfig env_config, boost::shared_ptr<tesis::MessageServer> messageServer );
void camera_thread( tesis::EnvironmentConfig env_config, boost::shared_ptr<tesis::MessageServer> messageServer );
void robot_thread( tesis::EnvironmentConfig env_config, boost::shared_ptr<tesis::MessageServer> messageServer );


#endif
