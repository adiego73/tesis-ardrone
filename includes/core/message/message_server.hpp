#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H

#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>

#include <iostream>

#include <boost/thread.hpp>
#include <boost/thread/lock_guard.hpp>

namespace tesis
{

class MessageServer
{
    public:
        MessageServer();
        ~MessageServer();

        void announce( std::string topic );
        void publish( std::string topic, std::string message );
        std::string get( std::string topic );
        std::string get( std::string topic, std::string default_value );
        std::vector<std::string> topics();

    private:
        std::map<std::string, std::string> topic_map;
        boost::mutex msg_mutex;
};

}
#endif // MESSAGESERVER_H
