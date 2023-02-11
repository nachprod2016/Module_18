#include "Server.h"
#include <iostream>

Server::User::User(std::string & login, std::string & password) : login_(login), password_(password) {}

void Server::User::addNewContact(std::string & newContact, std::string & chatKey)
{
    std::pair<std::string, std::string> p = std::make_pair(newContact, chatKey);
    contacts.insert(p); 
}

void Server::User::showChats()
{
    std::map<std::string, std::string>::iterator begin = contacts.begin();
    std::map<std::string, std::string>::iterator end = contacts.end();
    for (begin; begin != end; begin++)
    {
        std::cout << begin->first << std::endl;
    }
}