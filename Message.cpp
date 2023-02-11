#include "Server.h"

Server::Message::Message(std::string & author, std::string && message) : author_(author), _message(message){}

Server::Message::Message(std::string & author, std::string & message) :author_(author), _message(message){}