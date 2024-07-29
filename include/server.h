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
    server(){}

    // port to connect to & desired address to allow a socket connection
    bool establish(const unsigned int &port = 8080, const unsigned int &addr = INADDR_ANY);
    void operate();

private:
    int server_fd, incoming_fd; // socket file descriptor
    struct sockaddr_in serv_addr; // socket address information for the client
    char buffer[1024] = {0}; // buffer for receiving information

};


#endif //ROAR_SERVER_H
