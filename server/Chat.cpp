#include <iostream>
#include <cstring>
#include "Chat.h"
#include "Demo_perms.h"
#include "BadLoginRegistr.h"
#include "BadLoginMessage.h"


#if defined(_WIN32) 
#define PLATFORM_NAME "windows 32-bit" // Windows 32-bit
#elif defined(_WIN64)
#define PLATFORM_NAME "windows 64-bit" // Windows 64-bit
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android"   // Android 
#elif defined(__linux__)
#define PLATFORM_NAME "linux"     // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos и другие
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios"       // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx"       // Apple OSX
#else
#define PLATFORM_NAME  "OS not detected" // ОС не распознана
#endif


//Функция установки включения программы
void Chat::start()
{
    _chatOn = true;
    read_vector_users();
    read_vector_messages();
    std::cout << "\033[1;31m" << PLATFORM_NAME << std::endl;
}

//Функция возврата включенного состояния программы
bool Chat::chatOn() const
{
    return _chatOn;
}

//Функция установки авторизации пользователя
void Chat::loginOn()
{
    _checkLogin = true;
}

//Функция возврата режима авторизованного пользователя
bool Chat::checkLogin() const
{
    return _checkLogin;
}

//Функция вывода главного меню
void Chat::showLoginMenu()
{
    char operation;

    while (_chatOn && !_checkLogin)
    {
        std::cout << "\033[1;32m" << "(1) Login" << std::endl;
        std::cout << "(2) Regisration" << std::endl;
        std::cout << "(0) Exit" << std::endl;
        std::cout << ">> " << std::endl;
        std::cin >> operation;

        switch (operation)
        {
        case '1':
            if (login())
            {
                _checkLogin = false;
            }
            break;
        case '2':
            registrUser();
            break;
        case '0':
            _chatOn = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2..." << std::endl;
            break;
        }
    }
}

//Функция авторизации пользователя
bool Chat::login()
{
    std::string check_login;
    std::string check_password;
    std::string login;
    std::string password;
    bool is_found = false;
    char operation;

    while (!is_found)
    {
        std::cout << "login: ";
        std::cin >> check_login;
        std::cout << "password: ";
        std::cin >> check_password;
        user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!user_list.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            while (getline(user_list, login, ' ') && getline(user_list, password))
            {
                if (check_login == login && check_password == password)
                {
                    std::cout << "\033[31m" << "LOGIN SUCCESS" << std::endl;
                    _admin_online = check_login;
                    _active_user[_admin_online] = true;
                    loginOn();
                    user_list.close();
                    return is_found;
                }
            }
        }
        if (!is_found) {
            std::cout << "\033[31m" << "LOGIN OR PASSWORD FAILED" << std::endl;
            std::cout << "\033[32m" << "Press (0) for exit or press any button to try again" << std::endl;
            user_list.close();
            std::cin >> operation;
            if (operation == '0') {
                break;
            }
        }
    }
    return is_found;
}

//Функция регистрации пользователя
void Chat::registrUser()
{
    std::string login;
    std::string new_login;
    std::string password;
    std::cout << "new login: ";
    std::cin.ignore();
    std::cin >> new_login;
    try
    {
        if (new_login == "All")
        {
            throw BadLoginRegistr();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    try
    {
        user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!user_list.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            while (getline(user_list, login, ' ') && getline(user_list, password))
            {
                if (new_login == login)
                {
                    throw BadLoginRegistr();
                }
            }
            user_list.close();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!user_list.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        std::cout << "password: ";
        std::cin.ignore();
        std::cin >> password;
        User user(new_login, password);
        user_list << user << "\n";
        std::cout << "\033[1;31m" << "REGISTRATION COMPLETED SUCCESSFULLY!" << std::endl;
    }
    _active_user[new_login] = false;
    user_list.close();
}

//Функция вывода меню пользователя
void Chat::showUserMenu()
{
    char operation;
    std::cout << "\033[32m" << "Hi, " << "\033[37m" << _admin_online << std::endl;

    while (_checkLogin) {
        std::cout << "\033[36m" << "Menu: (#) SERVER MODE | (1) Chat Public |" << "\033[35m" << " (2) Chat Private |" << "\033[37m" << " (3) Users |" << "\033[32m" << " (0) Logout";

        std::cout << std::endl
            << "\033[37m" << ">> ";
        std::cin >> operation;

        switch (operation)
        {
        case '#':
            serverOn();
            break;
        case '1':
            showMenuChatPublic();
            break;
        case '2':
            showMenuChatPrivate();
            break;
        case '3':
            showUsersList();
            break;
        case '0':
            _active_user.at(_user_online) = false;
            _checkLogin = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2 or 3..." << std::endl;
            break;
        }
    }
    std::cout << "\033[36m" << "--------------" << std::endl;
}


//Функция вывода меню публичного чата
void Chat::showMenuChatPublic()
{
    bool chat_public_on = true;
    char operation;
    std::cout << "\033[36m" << "--- Chat Public ---" << std::endl;
    while (chat_public_on) {
        std::cout << "\033[36m" << "Menu:  (1) ShowChat |  (2) Add Message |  (0) Back"
            << std::endl
            << ">> ";
        std::cin >> operation;

        switch (operation)
        {
        case '1':
            showChatPublic();
            break;
        case '2':
            addMessagePublic();
            break;
        case '0':
            chat_public_on = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2..." << std::endl;
            break;
        }
    }
    std::cout << "\033[36m" << "--------------" << std::endl;
}

//Функция вывода меню приватного чата
void Chat::showMenuChatPrivate()
{
    bool chat_private_on = true;
    std::string from;
    std::string to;
    std::string text;
    char operation;

    while (chat_private_on) {
        std::cout << "\033[35m" << "--- Chat Private ---" << std::endl;
        std::cout << "Menu: (1) ShowChat | (2) Add Message | (0) Back"
            << std::endl
            << ">> ";
        std::cin >> operation;

        switch (operation)
        {
        case '1':
            showChatPrivate();
            break;
        case '2':
            addMessagePrivate();
            break;
        case '0':
            chat_private_on = false;
            break;
        default:
            std::cout << "\033[31m" << "Choose 1 or 2..." << std::endl;
            break;
        }
    }
    std::cout << "\033[35m" << "--------------" << std::endl;
}

//Функция вывода списка пользователей
void Chat::showUsersList()
{
    std::string login;
    std::string password;
    std::cout << "\033[37m" << "--- Users ---" << std::endl;
    for (const auto& u : _active_user)
    {
        if (u.first == _admin_online) {
            std::cout << "\033[37m" << u.first << "\033[32m" << " online" << "\033[37m" << " (you)" << std::endl;
        }
        else if (u.second == true) {
            std::cout << "\033[37m" << u.first << "\033[32m" << " online" << std::endl;
        }
        else {
            std::cout << "\033[37m" << u.first << "\033[31m" << " offline" << std::endl;
        }
    }
    std::cout << "\033[37m" << "--------------" << std::endl;
}

//Функция вывода публичного чата
void Chat::showChatPublic()
{
    std::string from;
    std::string to;
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        fs::permissions("messages.txt", fs::perms::owner_all);
        while (getline(msg_file, from, ' ') && getline(msg_file, to, ' ') && getline(msg_file, text))
        {
            if (to == "All")
            {
                std::cout << "From " << from << " To " << to << " : " << text;
                std::cout << std::endl;
            }
        }
    }
    msg_file.close();
}

//Функция добавления публичных сообщений в файл сообщений
void Chat::addMessagePublic() {
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        std::cout << "Text: ";
        std::cin.ignore();
        std::getline(std::cin, text);
        Message msg(_admin_online, "All", text);
        msg_file << msg << "\n";
    }
    msg_file.close();
}

//Функция вывода приватного чата
void Chat::showChatPrivate()
{
    std::string from;
    std::string to;
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        while (getline(msg_file, from, ' ') && getline(msg_file, to, ' ') && getline(msg_file, text))
        {
            if (to == _admin_online || from == _admin_online && to != "All")
            {
                std::cout << "From " << from << " To " << to << " : " << text;
                std::cout << std::endl;
            }
        }
    }
    msg_file.close();
}

//Функция добавления приватных сообщений в файл сообщений
void Chat::addMessagePrivate() {
    std::string to;
    std::string text;
    int _found = 0;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        std::cout << "To: ";
        std::cin.ignore();
        std::cin >> to;
        try
        {
            for (const auto& u : _active_user)
            {
                if (u.first == to)
                {
                    _found = 1;
                }
            }
            if (_found == 0) {
                throw BadLoginMessage();
            }
            else {
                std::cout << "Text: ";
                std::cin.ignore();
                std::getline(std::cin, text);
                Message msg(_admin_online, to, text);
                msg_file << msg << "\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    msg_file.close();
}

//Функция считывания файла с данными пользователей
void Chat::read_vector_users()
{
    std::string login;
    std::string password;
    user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs::permissions("users.txt", fs::perms::owner_all);
    if (!user_list.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        fs::permissions("users.txt", fs::perms::owner_all);
        while (getline(user_list, login, ' ') && getline(user_list, password))
        {
            User _user = User(login, password);
            _users.emplace_back(_user);
            _active_user[login] = false;
        }
    }
    user_list.close();
}

//Функция считывания файла сообщений
void Chat::read_vector_messages()
{
    std::string from;
    std::string to;
    std::string text;
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs::permissions("messages.txt", fs::perms::owner_all);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        fs::permissions("messages.txt", fs::perms::owner_all);
        while (getline(msg_file, from, ' ') && getline(msg_file, to, ' ') && getline(msg_file, text))
        {
            Message _message = Message(from, to, text);
            _messages.emplace_back(_message);
        }
    }
    msg_file.close();
}

// Далее идут функции работы СЕРВЕРА - SERVER MODE (SM)

void Chat::sm_send(int connection, char* i_message)
{
    ssize_t bytes = write(connection, i_message, MESSAGE_LENGTH);
    // Если передали >= 0  байт, значит пересылка прошла успешно
    if (bytes >= 0)
    {
        //std::cout << "Data successfully sent to the client.!" << std::endl;
    }

}

void Chat::sm_get(int connection, char* i_message) 
{
    read(connection, i_message, MESSAGE_LENGTH);
    if (strncmp("QQQ", i_message, 3) == 0) {
        std::cout << "Client Exited." << std::endl;
        std::cout << "Server is Exiting..!" << std::endl;
        return;
    }
    //std::cout << "Data received from client: " << i_message << std::endl;
}

void Chat::serverOn()
{
    int socket_file_descriptor, connection, bind_status, connection_status;
    char msg[MESSAGE_LENGTH];
    std::string buff_mess;
    bzero(msg, MESSAGE_LENGTH);
    // Создадаем сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    // если не удалось 
    if (socket_file_descriptor == -1) {
        std::cout << "Socket creation failed!" << std::endl;
        exit(1);
    }
    // содержит IP-адрес хоста
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Привяжем сокет
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    // если не удалось
    if (bind_status == -1) {
        std::cout << "Socket binding failed!" << std::endl;
        exit(1);
    }


    // сокет переводится в режим ожидания запросов со стороны клиентов
    connection_status = listen(socket_file_descriptor, 5);
    // если не удалось
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection..." << std::endl;
    }

    length = sizeof(client);
    // создаём для общения с клиентом новый сокет и возвращаем его дескриптор
    connection = accept(socket_file_descriptor, (struct sockaddr*)&client, &length);
    // если не удалось
    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client!" << std::endl;
        exit(1);
    }

    std::cout << "Server: got connection from " << inet_ntoa(client.sin_addr) << " port " << ntohs(client.sin_port) << std::endl;
    
    server_online = true;
    
    while (server_online)
    {
        char operation;
        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "(1) Login\n(2) Regisration\n(0) Exit\n>>";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);
        
        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        operation = msg[0];

        switch(operation)
        {
        case '1':
            checkLoginSM(connection);
            break;
        case '2':
            registrUserSM(connection);
            break;
        case '0':
        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "end";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
         
	    server_online = false;
        close(socket_file_descriptor);
            return;
        default:
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "Bad choise...\nPress any button to repeat";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg); 
            break;
        }
    }
}

void Chat::checkLoginSM(int connection)
{
    checkLog = true;
    std::string login, password, buff_mess;
    char operation;
    char msg[MESSAGE_LENGTH];

    while (checkLog)
    {
        std::string check_login, check_password;
        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "login:";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        check_login = msg;

        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "password:";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        check_password = msg;

        for (const User& u : _users)
        {
            if (u.getUserLogin() == check_login && u.getUserPassword() == check_password)
            {
                _user_online = check_login;
                _active_user[check_login] = true;
                _clientLogin = true;
                showUserMenuSM(connection);
                break;
            }
        }
        if (!_clientLogin)
        {
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "LOGIN OR PASSWORD FAILED\nPress any button to repeat";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg);
            break;
        }
    }
}

void Chat::registrUserSM(int connection)
{
    std::string new_login, password, buff_mess;
    char msg[MESSAGE_LENGTH];
    bool flag = false;
    
    bzero(msg, MESSAGE_LENGTH);
    buff_mess = "new login: ";
    strcpy(msg, buff_mess.c_str());
    sm_send(connection, msg);

    bzero(msg, MESSAGE_LENGTH);
    sm_get(connection, msg);
    new_login = msg;
    
    bzero(msg, MESSAGE_LENGTH);
    buff_mess = "password: ";
    strcpy(msg, buff_mess.c_str());
    sm_send(connection, msg);
    
    bzero(msg, MESSAGE_LENGTH);
    sm_get(connection, msg);
    password = msg;

    for (const auto& u : _active_user)
    {
        if (u.first == new_login)
        {
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "This username is taken, please try another one\nPress any button to repeat";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);
            flag = true;

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg);
            break;
        }
    }
    if (!flag)
    {
        user_list.open("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!user_list.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            User user(new_login, password);
            user_list << user << "\n";
            _users.emplace_back(user);
            _active_user[new_login] = false;
            user_list.close();
        }
    }
}

void Chat::showUserMenuSM(int connection)
{
    bool _menu_on = true;
    std::string login, password, buff_mess;
    char operation;
    char msg[MESSAGE_LENGTH];

    while (_menu_on) 
    {
        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "LOGIN SUCCESS\nMenu: (1) Chat Public | (2) Chat Private | (3) Users |(0) Logout\nHi!, " + _user_online + "\n>>";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        operation = msg[0];
        
        switch (operation)
        {
        case '1':
            showMenuChatPublicSM(connection);
            break;
        case '2':
            showMenuChatPrivateSM(connection);
            break;
        case '3':
            showUsersListSM(connection);
            break;
        case '0':
            _active_user.at(_user_online) = false;
            _menu_on = false;
            checkLog = false;
            break;
        default:
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "Choose 1 or 2 or 3...\n--------------\nPress any button to repeat";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg);
            break;
        }
    }
}

void Chat::showMenuChatPublicSM(int connection)
{
    bool chat_public_on = true;
    std::string buff_mess1;
    char operation;
    char msg[MESSAGE_LENGTH];
    while (chat_public_on) {
        bzero(msg, MESSAGE_LENGTH);
        buff_mess1 = "--- Chat Public ---\nMenu:  (1) ShowChat | (2) Add Message | (0) Back\n>>";
        strcpy(msg, buff_mess1.c_str());
        sm_send(connection, msg);

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        operation = msg[0];

        switch (operation)
        {
        case '1':
        
            showChatPublicSM(connection);
            break;
        case '2':
            addMessagePublicSM(connection);
            break;
        case '0':
            chat_public_on = false;
            break;
        default:
            bzero(msg, MESSAGE_LENGTH);
            buff_mess1 = "Choose 1 or 2...\n--------------\nPress any button to repeat";
            strcpy(msg, buff_mess1.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg);
            break;
        }
    }
}

void Chat::showMenuChatPrivateSM(int connection)
{
    bool chat_private_on = true;
    std::string buff_mess;
    char operation;
    char msg[MESSAGE_LENGTH];

    while (chat_private_on) {
        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "--- Chat Private ---\nMenu:  (1) ShowChat | (2) Add Message | (0) Back\n>>";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        operation = msg[0];

        switch (operation)
        {
        case '1':
            showChatPrivateSM(connection);
            break;
        case '2':
            addMessagePrivateSM(connection);
            break;
        case '0':
            chat_private_on = false;
            break;
        default:
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "Choose 1 or 2...";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg); 
        }
    }
}

void Chat::showChatPublicSM(int connection)
{
    std::string buff_mess;
    char msg[MESSAGE_LENGTH];
    for (const auto& m : _messages) 
    {
        if (m.getTo() == "All")
        {
            bzero(msg, MESSAGE_LENGTH);
            buff_mess += "From " + m.getFrom() + " To " + m.getTo() + " : " + m.getText() + "\n";
        }
    }
    buff_mess += "--------------\nPress any button to continue";
    strcpy(msg, buff_mess.c_str());
    sm_send(connection, msg);

    bzero(msg, MESSAGE_LENGTH);
    sm_get(connection, msg);   
}

void Chat::addMessagePublicSM(int connection)
{
    bool go_chat_public = true;
    std::string buff_mess;
    char msg[MESSAGE_LENGTH];
    std::cout << "User "<< _user_online << " has entered a public chat! Let's talk to him" << std::endl;
    bzero(msg, MESSAGE_LENGTH);
    buff_mess = "HI, " + _user_online + "\n(QQQ) - Exit\nPress any button to start";
    strcpy(msg, buff_mess.c_str());
    sm_send(connection, msg);

    bzero(msg, MESSAGE_LENGTH);
    sm_get(connection, msg);

    while(go_chat_public)
    {
        msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!msg_file.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            std::string text;
            std::getline(std::cin, text);
            if (text == "QQQ")
            {
                go_chat_public = false;
                break;
            }
            Message message(_admin_online, "All", text);
            msg_file << message << "\n";
            msg_file.close();
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = _admin_online + " : " + text;
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);
        }

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        buff_mess = msg;
        std::cout << _user_online << " : " << msg << std::endl;
        if (buff_mess == "QQQ")
            {
                go_chat_public = false;
                break;
            }
            
        msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!msg_file.is_open())
        {
            std::cout << "File openning error!" << std::endl;
        }
        else
        {
            Message message(_user_online, "All", buff_mess);
            _messages.emplace_back(message);
            msg_file << message << "\n";
        }
        msg_file.close();
    }
    
}

void Chat::showChatPrivateSM(int connection)
{
    std::string buff_mess;
    char msg[MESSAGE_LENGTH];
    for (const auto& m : _messages) 
    {
        if (m.getTo() == _user_online || m.getFrom() == _user_online && m.getTo() != "All") 
        {
            bzero(msg, MESSAGE_LENGTH);
            buff_mess += "From " + m.getFrom() + " To " + m.getTo() + " : " + m.getText() +"\n";
        }
    }
    buff_mess += "--------------\nPress any button to continue";
    strcpy(msg, buff_mess.c_str());
    sm_send(connection, msg);

    bzero(msg, MESSAGE_LENGTH);
    sm_get(connection, msg); 
}

void Chat::addMessagePrivateSM(int connection) {
    
    std::string buff_mess, to, text;
    char msg[MESSAGE_LENGTH];
    int _found = 1; 
    
    msg_file.open("messages.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (!msg_file.is_open())
    {
        std::cout << "File openning error!" << std::endl;
    }
    else
    {
        bzero(msg, MESSAGE_LENGTH);
        buff_mess = "To: ";
        strcpy(msg, buff_mess.c_str());
        sm_send(connection, msg);
        

        bzero(msg, MESSAGE_LENGTH);
        sm_get(connection, msg);
        to = msg;
        for (const auto& u : _active_user)
        {
            if (u.first == to)
            {
                _found = 0;
            }
        }
        if (_found == 1) 
        {
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "User with this login does not exist\n--------------\nPress any button to continue";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg);
            return;
        }
        else 
        {
            bzero(msg, MESSAGE_LENGTH);
            buff_mess = "Text: ";
            strcpy(msg, buff_mess.c_str());
            sm_send(connection, msg);

            bzero(msg, MESSAGE_LENGTH);
            sm_get(connection, msg);
            text = msg;

            Message msg(_user_online, to, text);
            msg_file << msg << "\n";
            _messages.emplace_back(msg);
        }
        msg_file.close();
    }
}

void Chat::showUsersListSM(int connection)
{
    std::string buff_mess;
    char msg[MESSAGE_LENGTH];
    bzero(msg, MESSAGE_LENGTH);
    std::string login;
    std::string password;
    for (const auto& u : _active_user)
    {
        if (u.second == true)
        {
            buff_mess += u.first + " online\n";
        }
        else 
        {
            buff_mess += u.first + " offline\n";
        }
    }
    buff_mess += "--------------\nPress any button to continue";
    strcpy(msg, buff_mess.c_str());
    sm_send(connection, msg);

    bzero(msg, MESSAGE_LENGTH);
    sm_get(connection, msg);   
}
