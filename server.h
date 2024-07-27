//
// Created by joe on 7/27/24.
//

#ifndef ROAR_SERVER_H
#define ROAR_SERVER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// TODO ADD A HANDLER SERVER THAT WILL DIRECT THE DIFFERENT SOCKETS ACCORDINGLY TO A CLIENT-SIDE SERVER
// TODO ABILITY TO CREATE A CHATROOM AND INVITE/ALLOW CERTAIN USERNAMES/USER-IDS
// TODO ABILITY TO SEARCH FOR A USERNAME AND CONNECT WITH THAT PERSON IF DESIRED
// TODO ADD WINDOWS VERSION USING WIN-SOCKETS

class server {
public:
    server(const unsigned int &port){
        serv_addr.sin_family = AF_INET; // Socket will use IPv4
        serv_addr.sin_addr.s_addr = INADDR_ANY; // will connect to any ip address that requests access
        serv_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    }
private:
    int server_fd, incoming_fd; // socket file descriptor
    struct sockaddr_in serv_addr; // socket address information for the client
    char buffer[1024] = {0}; // buffer for receiving information

    void operate();
    bool setup();

};


#endif //ROAR_SERVER_H
