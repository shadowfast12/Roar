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
#include <string>

using namespace std;

// TODO ADD UNIQUE USERNAMES PER CLIENT FOR CONNECTIONS (OR SPECIAL ID)
// TODO ADD A PING SYSTEM TO SEE THE PING TO SEND AND RECEIVE MESSAGE
// TODO ADD MORE INFORMATION WHEN THE MESSAGE IS SENT (MESSAGE RECEIVED, SIZE, ETC)
// TODO MULTI-CLIENT CHAT ROOM WHERE THERE'S A RELAY TO ALL THE CLIENTS (MOSTLY SERVER SOCKET WORK)
// TODO BETTER INTERFACE AND EASY ACCESS

class client
{
public:
    bool connect(const unsigned int &port = 8080, const char *serverIP = "127.0.0.1");
    bool login(const char *username);
    bool send(const char *message);
    void terminate();

private:
    int client_fd; // socket file descriptor
    char username[100];
    struct sockaddr_in client_addr; // socket address information for the client
    char buffer[1024] = {0};        // buffer for receiving information
};

#endif // ROAR_CLIENT_H
