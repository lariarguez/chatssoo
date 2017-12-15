#include <iostream>
#include <thread> //----->Se usa para crear hilos
#include <stdlib.h>
#include <system_error>
#include <atomic>

#include "socket.h"

sockaddr_in make_ip_address(const std::string& ip_address, int port){   //Se usa para direccionar cada punto de conexión


    sockaddr_in local_address{};
    local_address.sin_family = AF_INET;
     if (ip_address == ""){
        local_address.sin_addr.s_addr = htonl(INADDR_ANY);
     }
     else{
        inet_aton(ip_address.c_str(), &local_address.sin_addr);
     }
        local_address.sin_port = htons(port);
    return local_address;
}


void comunicate(Message& message,std::string &ip_address_local){   //Función que guarda el mensaje

    std::string screen_message= "";
    std::cout << ip_address_local << ":";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin,screen_message);
    int sz = screen_message.copy(message.text, screen_message.size());
    message.text[sz]='\0';
}

void send_message(Socket& socket, sockaddr_in& remote_address, std::atomic<bool>& quit,std::string &ip_address_local){  //Función que envía el mensaje


    while(!quit){
        Message message;
        comunicate(message,ip_address_local);

        if(strcmp(message.text, "\\quit") == 0){    //Comprueba si la cadena es igual, si es igual devuelve 0 y se ejecuta el if dado que la condición es correcta
          quit=true;
          std::cout << "El usuario ha abandonado la sala de CHAT:" << std::endl;
        }
        else{

          socket.send_to(message, remote_address);
        }

    }

}
void recv_message(Socket& socket, sockaddr_in& local_address, std::atomic<bool>& quit, std::string &ip_address_remote){



    while(!quit){
          Message message;
          socket.receive_from(message,local_address, ip_address_remote);

    }
}

int request_cancellation(std::thread& thread){

        return pthread_cancel(thread.native_handle());    //native_handle() para obtener el manejador interno tipo “pthread_t” que cada objeto std::thread guarda del hilo

}
int main(){

  int port_local, port_remote;

  std::string ip_address_local="";
  std::string ip_address_remote="";

  std::cout << "Puerto Local:  \n";    //Recoge los puertos y las ip por pantalla
          std::cin >> port_local;
   std::cout << "IP: ";
          std::cin >> ip_address_local;
   std::cout << "Puerto Remoto:  \n";
          std::cin >> port_remote;
   std::cout << "IP: ";
          std::cin >> ip_address_remote;

       sockaddr_in address_local =  make_ip_address(ip_address_local,port_local);
       sockaddr_in address_remote = make_ip_address(ip_address_remote,port_remote);


       Socket socket(address_local);
       std::atomic<bool> quit(false);           //Variable bool

        try{

            std::thread send(&send_message, std::ref(socket), std::ref(address_remote), std::ref(quit), std::ref(ip_address_local));
            std::thread recv(&recv_message, std::ref(socket), std::ref(address_local), std::ref(quit), std::ref(ip_address_remote));
            send.join();
                request_cancellation(recv);
            recv.join();
        }

        catch(std::bad_alloc& e){ //hace internamente el operador "new" si no puede reservar la memoria solicitada.
             std::cerr << program_invocation_name <<  ": memoria insuficiente\n";
             return 1;
        }
        catch(std::system_error& e) { //Esta excepción se usa para notificar errores en llamadas al sistema
           std::cerr << program_invocation_name << ": " << e.what() << '\n';
           return 2;
        }
       catch (...) {
           std::cout << "Error desconocido\n";
       }

        return 0;
}




