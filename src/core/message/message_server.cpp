#include "core/message/message_server.hpp"

using namespace tesis;

MessageServer::MessageServer()
{

}

MessageServer::~MessageServer()
{
}

// Announce a topic
void MessageServer::announce( std::string topic )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );
    this->topic_map[topic] =  "";
}

// Get a message on a topic
std::string MessageServer::get( std::string topic )
{
    std::string def = "{\"error\" : \"No topic with name '" + topic + "' was found\"}";
    return this->get( topic, def);
}

// Get a message stored on a topic
std::string MessageServer::get( std::string topic, std::string default_value)
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );

    //  if topic doesn't exist or the message is not "" (empty)
    if( this->topic_map.count( topic ) > 0 && this->topic_map[topic].length() > 0 )
    {
        return this->topic_map[topic];
    }
    else
    {
        return default_value;
    }
}

// Publish a message on a topic. The topic must be announced first.
void MessageServer::publish( std::string topic, std::string message )
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );

    if( this->topic_map.count( topic ) > 0 )
    {
        
        this->topic_map[topic] =  message;
    }
    else
    {
        std::string ex_msg = "The topic '" + topic + "' does not exist. Announce it before publish a message";
//         throw std::invalid_argument( ex_msg );
    }
}

float MessageServer::getFloat( std::string topic )
{
  std::string value = this->get( topic);
  return std::stof( value );
}
float MessageServer::getFloat( std::string topic, float default_value )
{
  std::string value = this->get( topic, std::to_string(default_value));
  return std::stof( value );
}
int MessageServer::getInt( std::string topic )
{
  std::string value = this->get( topic);
  return std::stod( value );
}
int MessageServer::getInt( std::string topic, int default_value )
{
  std::string value = this->get( topic, std::to_string(default_value));
  return std::stod( value );
}
long MessageServer::getLong( std::string topic )
{
  std::string value = this->get( topic);
  return std::stol( value );
}
long MessageServer::getLong( std::string topic, long default_value )
{
  std::string value = this->get( topic, std::to_string(default_value));
  return std::stol( value );
}

bool MessageServer::getBool( std::string topic )
{
  return this->get( topic).find( "false" ) == std::string::npos;
}

bool MessageServer::getBool( std::string topic, bool default_value )
{
  return this->get( topic, default_value == 0 ? "false": "true").find( "false" ) == std::string::npos;
}

// Get all announced topics
std::vector<std::string> MessageServer::topics()
{
    boost::lock_guard<boost::mutex> lock( this->msg_mutex );
    std::vector<std::string> topics;

    for( auto const & topic :  this->topic_map )
    {
        topics.push_back( topic.first );
    }

    return topics;
}
