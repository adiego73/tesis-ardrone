#include "core/threads.hpp"

using namespace tesis;

void robot_thread( boost::shared_ptr<MessageServer> messageServer )
{
    std::string path = "./lib/libardrone.so";

    void* handle = dlopen( path.c_str(), RTLD_NOW );

    if( !handle )
    {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
    }
    else
    {
        typedef void ( *robot_t )( MessageServer* );

        const char* dlsym_error = dlerror();

        if( dlsym_error )
        {
            std::cerr << "Cannot load symbol 'hello': " << dlsym_error << std::endl;
        }
        else
        {
            robot_t run = ( robot_t ) dlsym( handle, "run" );
            std::cout << "robot thread" << std::endl;
            run( messageServer.get() );
        }

        dlclose( handle );
    }
}
