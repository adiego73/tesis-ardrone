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
    this->topic_map[topic] = boost::any();
}

// Get a message stored on a topic
template <typename T>
T MessageServer::get( std::string topic, T default_value )
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

// Publish a message on a topic. The topic must be announced first.
template <typename T>
void MessageServer::publish( std::string topic, T message )
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
