#include <core/threads.hpp>

using namespace tesis;

std::vector< std::string > splitRequest( char delimiter, std::string request_message );

void message_service_thread( boost::shared_ptr<MessageServer> messageServer )
{
    bool quit = false;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor( io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), MessageServer::MESSAGE_SERVER_PORT_NO ) );

    boost::system::error_code error;

    while( !quit )
    {
        boost::asio::ip::tcp::socket socket( io_service );
        acceptor.accept( socket );

        std::string response;
        boost::array<char, 4096> buf;

        std::size_t _ = socket.read_some( boost::asio::buffer( buf ), error );

        if( error == boost::asio::error::eof )
            break;
        else if( error )
            break;

        std::string request( buf.begin(), buf.end() );
        std::vector<std::string> request_split = splitRequest( '|', request );

        // Message example: announce|test/topic/1
        if( request_split.size() == 2 && request_split[0].find( "announce" ) != std::string::npos )
        {
            messageServer->announce( request_split[1] );
        }
        // Message example: get|test/topic/3|defaul value
        else if( request_split.size() >= 2 && request_split[0].find( "get" ) != std::string::npos )
        {
            if( request_split.size() == 2 )
            {
                response = messageServer->get( request_split[1] );
            }
            else
            {
                response = messageServer->get( request_split[1], request_split[2] );
            }

            if( request_split[1].compare( "gui/finish" ) ==  0 )
            {
                std::istringstream( response ) >> std::boolalpha >> quit;
            }
        }
        // Message example: publish|test/topic/2|message example for topic
        else if( request_split.size() == 3 && request_split[0].find( "publish" ) != std::string::npos )
        {
            messageServer->publish( request_split[1], request_split[2] );
        }

        boost::asio::write( socket, boost::asio::buffer( response ), error );
    }
}

std::vector< std::string > splitRequest( char delimiter, std::string request_message )
{
    std::stringstream stream( request_message );
    std::string segment;
    std::vector<std::string> segments;

    while( std::getline( stream, segment, delimiter ) )
    {
        segments.push_back( segment );
    }

    return segments;
}

