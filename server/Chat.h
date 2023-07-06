#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Message.h"
#include "User.h"

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

class Chat {
public:
    Chat() = default;
    void start();
    bool chatOn() const;
    void loginOn();
    bool checkLogin() const;
    void showLoginMenu();
    void showUserMenu();

    bool login();
    void registrUser();
    void showMenuChatPrivate();
    void showMenuChatPublic();
    void showUsersList();
    void showChatPublic();
    void addMessagePublic();
    void showChatPrivate();
    void addMessagePrivate();
    void read_vector_messages();
    void read_vector_users();

    // Далее идут функции работы СЕРВЕРА - SERVER MODE (SM)
    void serverOn();
    void sm_send(int, char[]);
    void sm_get(int, char[]);
    void checkLoginSM(int);
    void showUserMenuSM(int);
    void registrUserSM(int);
    void showMenuChatPublicSM(int);
    void showMenuChatPrivateSM(int);
    void showUsersListSM(int);
    void showChatPublicSM(int);
    void addMessagePublicSM(int);
    void showChatPrivateSM(int);
    void addMessagePrivateSM(int);

    ~Chat() = default;

    std::fstream user_list;
    std::fstream msg_file;

private:

    bool _chatOn = false;
    bool _checkLogin = false;
    bool _clientLogin = false;
    bool server_online = false;
    bool checkLog = false;
    std::string _admin_online;
    std::string _user_online;

    std::vector<User> _users;
    std::vector<Message> _messages;
    std::map<std::string, bool> _active_user;

    struct sockaddr_in serveraddress, client;
    socklen_t length;
};
