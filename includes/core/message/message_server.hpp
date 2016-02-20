#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H

#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>

#include <iostream>

#include <boost/any.hpp>
#include <boost/thread.hpp>

namespace tesis
{

class MessageServer
{
    public:
        MessageServer() {}
        ~MessageServer() {}

        void announce( std::string topic )
        {
            boost::lock_guard<boost::mutex> lock( this->msg_mutex );
            this->topic_map[topic] = boost::any();
        }
        template<typename T> void publish( std::string topic, T message )
        {
            boost::lock_guard<boost::mutex> lock( this->msg_mutex );

            if( this->topic_map.count( topic ) > 0 )
            {

                this->topic_map[topic] =  message;
            }
            else
            {
                std::string ex_msg = "The topic '" + topic + "' does not exist. Announce it before publish a message";
                throw std::invalid_argument( ex_msg );
            }
        }
        template<typename T> T get( std::string topic, T default_value )
        {
            boost::lock_guard<boost::mutex> lock( this->msg_mutex );

            //  if topic doesn't exist or the message is not "" (empty)
            if( this->topic_map.count( topic ) > 0 && !this->topic_map.at( topic ).empty() )
            {
                return boost::any_cast<T>( this->topic_map.at( topic ) );
            }
            else
            {
                return default_value;
            }
        }
        std::vector<std::string> topics()
        {
            boost::lock_guard<boost::mutex> lock( this->msg_mutex );
            std::vector<std::string> topics;

            for( auto const & topic :  this->topic_map )
            {
                topics.push_back( topic.first );
            }

            return topics;
        }

    private:
        std::map<std::string, boost::any> topic_map;
        boost::mutex msg_mutex;
};

}
#endif // MESSAGESERVER_H
