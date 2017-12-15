#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <system_error>
#include <cstring>      // para std::strerror()



Socket::Socket(struct sockaddr_in& address){

    descriptor = socket(AF_INET, SOCK_DGRAM, 0);  //devuelve un descriptor de archivo con el que identificar al socket en cuestión en las futuras operaciones

    if (descriptor < 0)
         throw std::system_error(errno, std::system_category(), "ERROR DE CONEXIÓN."); //Esta excepción se usa para notificar errores en llamadas al sistema


      //Una dirección se asigna aun socket con la llamada al sistema bind()

    int result = bind(descriptor, reinterpret_cast<const sockaddr*>(&address), sizeof(address));

    if (result < 0)
        throw std::system_error(errno, std::system_category(), "ERROR DE DIRECCIÓN" );     //Esta excepción se usa para notificar errores en llamadas al sistema


}

Socket::~Socket(){

    close(descriptor);
    std::cout << "Ejecución terminada..." << '\n';
}

void Socket::send_to(const Message message, const sockaddr_in address){


       int  result  = sendto(descriptor, message.text, sizeof(message.text), 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address));

    if (result < 0)
        throw std::system_error(errno, std::system_category());     //Esta excepción se usa para notificar errores en llamadas al sistema

}

void Socket::receive_from(Message message, sockaddr_in address, std::string &ip_address) {

    // Dirección del socket remoto
    socklen_t src_len = sizeof(address);


   int result = recvfrom(descriptor, &message, sizeof(message), 0, reinterpret_cast<sockaddr*>(&address), &src_len);

    if (result < 0)
         throw std::system_error(errno, std::system_category());
    std::cout << std::endl;
    std::cout << ip_address << ":";
    std::cout << message.text << std::endl;
}
