#include "core/message/message_server.hpp"

using namespace tesis;

MessageServer::MessageServer()
{

}

MessageServer::~MessageServer()
{

}

void MessageServer::announce( std::string topic )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );
    this->topic_map[topic] =  "";
}

std::string MessageServer::get( std::string topic )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );

    if( this->topic_map.count( topic ) > 0 )
    {
        return this->topic_map[topic];
    }
    else
    {
        return "{\"error\" : \"No topic with name '" + topic + "' was found\"}";
    }
}

void MessageServer::publish( std::string topic, std::string message )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );
    this->topic_map[topic] =  message;
}
