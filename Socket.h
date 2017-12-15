#define SOCKET_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <thread>


//Estructura de los mensajes
struct Message
{
    char text[3024];
};

class Socket{

    int descriptor;

public:
        //Constructor por defecto
        Socket();

        //Constructor con parametros

        Socket(struct sockaddr_in& address);

        ~Socket();

        void send_to(const Message message, const sockaddr_in address);

        void receive_from(Message message, sockaddr_in address, std::string &ip_address);

};


