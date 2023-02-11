#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "Server.h"

Server::Server()
{
    std::cout << "Учетная запись администратора: " << std::endl;
    std::cout << "Логин: ";
    std::cin >> adminLogin;
    std::cout << "Пароль: ";
    std::cin >> adminPassword;
}

void Server::administrator()
{
    std::string input;
    downloadData();
    while (true)
    {
        std::cout << "1 - Запустить работу чата" << std::endl;
        std::cout << "0 - Остановить работу чата" << std::endl;
        std::cin >> input;
        if (input == "1")
        {
            start();
        }
        else if (input == "0")
        {
            saveData();
            break;
        }
    }
}

void Server::start()
{
    std::string login;
    std::string password;
    std::string input;
    while (true)
    {
        std::cout << "1 - Регистрация" << std::endl;
        std::cout << "2 - Вход" << std::endl;
        std::cin >> input;
        std::cout << "Логин: ";
        std::cin >> login;
        std::cout << "Пароль: ";
        std::cin >> password;
        if (input == "1")
        {
            registration(login, password);
        }
        else if (input == "2")
        {
            if (login == adminLogin && password == adminPassword)
            {
                break;
            }
            authorization(login, password);
        }
    }
}

void Server::registration(std::string & login, std::string & password)
{
    if (users.find(login) != users.end())
    {
        std::cout << "Пользователь с таким логином уже зарегестрирован" << std::endl;
        return;
    }
    User newUser(login, password);
    std::pair<std::string, User> p = std::make_pair(login, newUser);
    users.insert(p);
}

void Server::authorization(std::string & login, std::string & password)
{
    std::map<std::string, User>::iterator it = users.find(login);
    if (it == users.end() || it->second.password_ != password)
    {
        std::cout << "Неверный логин или пароль" << std::endl;
        return;
    }
    userSession(login);
}

void Server::userSession(std::string & user)
{
    std::string input;
    while (true)
    {
        std::cout << "1 - Добавить новый контакт" << std::endl;
        std::cout << "2 - Просмотреть сообщения" << std::endl;
        std::cout << "3 - Написать всем контактам" << std::endl;
        std::cout << "0 - Завершить сессию" << std::endl;
        std::cin >> input;
        if (input == "1")
        {
            std::cout << "Список зарегестрированных пользователей:" << std::endl; 
            showUsers(user);
            std::cout << "Логин нового контакта: ";
            std::cin >> input;
            if (checkLogin(user, input) == false)
            {
                continue;
            }
            createNewChat(user, input);
        }
        else if (input == "2")
        {
            User & us = users.find(user)->second;
            us.showChats();
            std::cout << "Выберите чат: ";
            std::cin >> input;
            std::map<std::string, std::string>::iterator key = us.contacts.find(input);
            if (key == us.contacts.end())
            {
                std::cout << "Ошибка ввода!!!" << std::endl;
                continue;
            }
            Chat & chat = chats.find(key->second)->second;
            chat.showMessages();
            std::cout << "Написать сообщение?" << std::endl;
            std::cout << "1 - Да" << std::endl;
            std::cout << "2 - Нет" << std::endl;
            std::cin >> input;
            if (input != "1")
            {
                continue;
            }
            chat.writeMessage(user);
        }
        else if (input == "3")
        {
            writeToEveryone(user);
        }
        else if (input == "0")
        {
            break;
        }
    }
}

void Server::createNewChat(std::string & user, std::string & newContact)
{
    std::map<std::string, User>::iterator us = users.find(user);
    std::map<std::string, User>::iterator nc = users.find(newContact);
    Chat newChat(user, newContact);
    Message newMessage(user, "Привет!");
    newChat.addMessage(newMessage);
    std::string chatKey;
    chatKey.append(user);
    chatKey.push_back('+');
    chatKey.append(newContact);
    us->second.addNewContact(newContact, chatKey);
    nc->second.addNewContact(user, chatKey);
    std::pair<std::string, Chat> p = std::make_pair(chatKey, newChat);
    chats.insert(p);
}

void Server::showUsers(std::string & user)
{
    std::map<std::string, User>::iterator begin = users.begin();
    std::map<std::string, User>::iterator end = users.end();
    for (begin; begin != end; begin++)
    {
        std::cout << begin->first << std::endl;
    }
}

bool Server::checkLogin(std::string & user, std::string & contact)
{
    bool flag = true;
    std::map<std::string, User>::iterator newContact = users.find(contact);
    std::map<std::string, User>::iterator usr = users.find(user);
    if (newContact == users.end() || user == contact || usr->second.contacts.find(contact) != usr->second.contacts.end())
    {
        std::cout << "Неверный логин!!!" << std::endl;
        flag = false;
    }
    return flag;
}

void Server::writeToEveryone(std::string & user)
{
    std::string input;
    User & us = users.find(user)->second;
    std::map<std::string, std::string>::iterator begin = us.contacts.begin();
    std::map<std::string, std::string>::iterator end = us.contacts.end();
    std::cout << "Сообщение: ";
    std::getline(std::cin, input);//очистка потока ввода
    std::getline(std::cin, input);//ввод сообщения
    Message m(user, input);
    for (begin; begin != end; begin++)
    {
        Chat & ch = chats.find(begin->second)->second;
        ch.addMessage(m);
    }
}

void Server::downloadData()
{
    std::ifstream download;
    download.open("DataUsers.txt");
    if (download.is_open() == 0)
    {
        std::cout << "Внимание!!! Файл DataUsers.txt не был открыт!" << std::endl;
    }
    std::string log;
    std::string pas;
    while (true)
    {
        std::getline(download, log);
        if(download.eof() == true)
        {
            break;
        }
        std::getline(download, pas);
        User user(log, pas);
        std::pair<std::string, User> pUs = std::make_pair(log, user);
        users.insert(pUs);
    }
    download.close();
    download.open("DataChats.txt");
    if (download.is_open() == 0)
    {
        std::cout << "Внимание!!! Файл DataChats.txt не был открыт!" << std::endl;
    }
    std::string chatKey;
    std::string user1;
    std::string user2;
    std::string author;
    std::string msg;
    while (true)
    {
        std::getline(download, chatKey);
        if(download.eof() == true)
        {
            break;
        }
        std::getline(download, user1);
        std::getline(download, user2);
        Chat chat(user1, user2);
        while (true)
        {
            std::getline(download, author);
            if (author == "--------------------------------------------")
            {
                std::pair<std::string, Chat> pCh = std::make_pair(chatKey, chat);
                chats.insert(pCh);
                break;
            }
            std::getline(download, msg);
            Message message(author, msg);
            chat.addMessage(message);
        }
        std::map<std::string, User>::iterator Us = users.find(user1);
        Us->second.addNewContact(user2, chatKey);
        Us = users.find(user2);
        Us->second.addNewContact(user1, chatKey);
    }
    download.close();
}

void Server::saveData()
{
    std::ofstream save;
    save.open("DataUsers.txt");
    std::map<std::string, User>::iterator beginUs = users.begin();
    std::map<std::string, User>::iterator endUs = users.end();
    for (beginUs; beginUs != endUs; beginUs++)
    {
        User & us = beginUs->second;
        save << us.login_ << std::endl << us.password_ << std::endl;
    }
    save.close();
    save.open("DataChats.txt");
    std::map<std::string, Chat>::iterator beginCh = chats.begin();
    std::map<std::string, Chat>::iterator endCh = chats.end();
    for (beginCh; beginCh != endCh; beginCh++)
    {
        Chat & ch = beginCh->second;
        save << beginCh->first << std::endl << ch.user1_ << std::endl << ch.user2_ << std::endl;
        std::list<Message>::iterator msgsBegin = ch.messages.begin();
        std::list<Message>::iterator msgsEnd = ch.messages.end();
        for (msgsBegin; msgsBegin != msgsEnd; msgsBegin++)
        {
            save << msgsBegin->author_ << std::endl;
            save << msgsBegin->_message << std::endl;
        } 
        save << "--------------------------------------------" << std::endl;
    }
    save.close();
}