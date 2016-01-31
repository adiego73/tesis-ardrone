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
    std::string def = "{\"error\" : \"No topic with name '" + topic + "' was found\"}";
    this->get( topic, def );
}

std::string MessageServer::get( std::string topic, std::string default_value )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );

    if( this->topic_map.count( topic ) > 0 )
    {
        return this->topic_map[topic];
    }
    else
    {
        return default_value;
    }
}


void MessageServer::publish( std::string topic, std::string message )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );

    if( this->topic_map.count( topic ) > 0 )
    {
        this->topic_map[topic] =  message;
    }
    else
    {
        std::string ex_msg = "The topic '" +topic+ "' does not exist. Announce it before publish a message";
        throw std::exception(ex_msg);
    }
}
