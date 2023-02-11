#pragma once
#include <map>
#include <list>
#include <string>

class Server
{
private:
    std::string adminLogin;
    std::string adminPassword;
    struct Message
    {
        const std::string  author_;
        const std::string _message;
        Message(std::string &, std::string &&);
        Message(std::string &, std::string &);
        ~Message() = default;
    };
    struct Chat
    {
        std::string user1_;
        std::string user2_;
        std::list<Message> messages;
        Chat(std::string &, std::string &);
        ~Chat() = default;
        void addMessage(Message &);
        void showMessages();
        void writeMessage(std::string &);
    };
    struct User
    {
        std::string login_;
        std::string password_;
        std::map<std::string, std::string> contacts;
        User(std::string &, std::string &);
        ~User() = default;
        void addNewContact(std::string &, std::string &);
        void showChats();
    };
    std::map<std::string, User> users;
    std::map<std::string, Chat> chats; 
    void start();
    void registration(std::string &, std::string &);
    void authorization(std::string &, std::string &);
    void userSession(std::string &);
    void showUsers(std::string &);
    void createNewChat(std::string &, std::string &);
    bool checkLogin(std::string &, std::string &);
    void writeToEveryone(std::string &);
    void downloadData();
    void saveData();
public:
    Server();
    ~Server() = default;
    void administrator();
};