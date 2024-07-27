//
// Created by joe on 7/27/24.
//

#ifndef ROAR_CLIENT_H
#define ROAR_CLIENT_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// TODO ADD UNIQUE USERNAMES PER CLIENT FOR CONNECTIONS (OR SPECIAL ID)
// TODO ADD A PING SYSTEM TO SEE THE PING TO SEND AND RECEIVE MESSAGE
// TODO ADD MORE INFORMATION WHEN THE MESSAGE IS SENT (MESSAGE RECEIVED, SIZE, ETC)
// TODO MULTI-CLIENT CHAT ROOM WHERE THERE'S A RELAY TO ALL THE CLIENTS (MOSTLY SERVER SOCKET WORK)
// TODO BETTER INTERFACE AND EASY ACCESS
class client {
public:
    client(const unsigned int &port){
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


#endif //ROAR_CLIENT_H
