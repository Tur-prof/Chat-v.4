#include <iostream>
#include "Client.h"



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
    

    bool Client::createSock()
    {
        std::cout << "\033[1;31m" << PLATFORM_NAME << "\033[32m" << std::endl;
        // Создадим сокет
        socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_file_descriptor == -1) {
            std::cout << "Creation of Socket failed!" << std::endl;
            return false;
        }

        // Установим адрес 
        serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        // Зададим номер порта
        serveraddress.sin_port = htons(PORT);
        // Используем IPv4
        serveraddress.sin_family = AF_INET;
        return true;
    }

void Client::connectServer()
{
    // Установим соединение с сервером
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        std::cout << "Connection with the server failed!" << std::endl;
        exit(1);
    }
    std::cout << "Connection with the server is Ok..." << std::endl;
    std::cout << "Hello!" << std::endl;
    communication();
}

void Client::communication()
{
    // Взаимодействие с сервером
    while (1) {
    
    	// чтение сообщения от сервера
        bzero(message, sizeof(message));
        // Ждем ответа от сервера
        read(socket_file_descriptor, message, sizeof(message));
        std::cout /*<< "Message from server: "*/ << message << std::endl;
        if (message == "end")
        {
          close(socket_file_descriptor);
          break;
        } 

        // отправка сообщения серверу
        bzero(message, sizeof(message));
        //std::cout << "Message from client: ";
        std::cin.getline(message, MESSAGE_LENGTH);
	
        ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if (bytes < 0) {
            std::cout << "Error!" << std::endl;

        }

        
    }
}
Client::~Client()
{
    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
}






