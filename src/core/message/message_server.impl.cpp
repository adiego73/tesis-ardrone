#include "./message_server.cpp"

using namespace tesis;

extern template std::string MessageServer::get(std::string, std::string);
extern template int MessageServer::get(std::string, int);
extern template long MessageServer::get(std::string, long);
extern template long int MessageServer::get(std::string, long int);
extern template float MessageServer::get(std::string, float);
extern template double MessageServer::get(std::string, double);
extern template bool MessageServer::get(std::string, bool);

extern template void MessageServer::publish( std::string, std::string);
extern template void MessageServer::publish( std::string, int);
extern template void MessageServer::publish( std::string, long);
extern template void MessageServer::publish( std::string, long int);
extern template void MessageServer::publish( std::string, float);
extern template void MessageServer::publish( std::string, double);
extern template void MessageServer::publish( std::string, bool);


