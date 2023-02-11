#include "Server.h"
#include <iostream>

Server::Chat::Chat(std::string & user1, std::string & user2) : user1_(user1), user2_(user2) {}

void Server::Chat::addMessage(Message & m)
{
    messages.push_back(m);
}

void Server::Chat::showMessages()
{
    std::list<Message>::iterator begin = messages.begin();
    std::list<Message>::iterator end = messages.end();
    for (begin; begin != end; begin++)
    {
        std::cout << begin->author_ << ": " << begin->_message << std::endl;
    }
}

void Server::Chat::writeMessage(std::string & author)
{
    std::string input;
    std::cout << "Сообщение: ";
    std::getline(std::cin, input);//очистка потока ввода
    std::getline(std::cin, input);//ввод сообщения
    Message newMessage(author, input);
    addMessage(newMessage);
}