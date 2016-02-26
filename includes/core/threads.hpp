#ifndef THREADS_HPP
#define THREADS_HPP

#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <chrono>
#include <dlfcn.h>
#include <vector>
#include <tuple>

#include <opencv.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>

#include "classes/environment.hpp"
#include "config/configutarion.hpp"
#include "core/message/message_server.hpp"
#include "core/data/videodata.hpp"

void gui_thread( boost::shared_ptr<tesis::MessageServer> messageServer, boost::shared_ptr<tesis::Environment> env, boost::shared_ptr<tesis::VideoData> videoData);
void camera_thread( boost::shared_ptr<tesis::MessageServer> messageServer, boost::shared_ptr<tesis::Environment> env, boost::shared_ptr<tesis::VideoData> videoData);
void robot_thread( boost::shared_ptr<tesis::MessageServer> messageServer );


#endif
