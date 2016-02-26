#include <iostream>

#include <boost/bind.hpp>

#include <opencv.hpp>

#include "core/threads.hpp"
#include "structures.hpp"
#include "classes/environment.hpp"

using namespace tesis;

void show_usage();

int main( int argc, char** argv )
{

    if( argc < 2 )
    {
        show_usage();
        return 1;
    }

    boost::shared_ptr<Environment> env( new Environment( argv[1] ) );
    boost::shared_ptr<MessageServer> msg_server( new MessageServer() );
    boost::shared_ptr<VideoData> videoProxy( new VideoData() );

    boost::thread_group threads;

    threads.create_thread( boost::bind( gui_thread, msg_server, env, videoProxy ) );
    threads.create_thread( boost::bind( camera_thread, msg_server, env, videoProxy ) );
    threads.create_thread( boost::bind( robot_thread, msg_server ) );

    threads.join_all();

    return 0;
}

void show_usage()
{
    std::cout << "Welcome to the quadcopter framework " << std::endl;
    std::cout << "To use this framework you should create a configuration file, " << std::endl << "and place it in the config folder." << std::endl;
    std::cout << "Then, run the application with the path to the environment " << std::endl << "configuration file as the first parameter" << std::endl;
    std::cout << std::endl << "Example: \ttest@pc:~/bin$ tesis-ardrone config/environment.json";
}

