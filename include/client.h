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
    void connect(const unsigned int &port = 8080, const char *serverIP = "127.0.0.1");
    void send(const char *message);
    void receive();
    void terminate();

private:
    int client_fd = -1; // socket file descriptor
    struct sockaddr_in server_addr;
    char buffer[1024] = {0}; // buffer for receiving information
    // double array buffer to keep multiple messages?
};

#endif // ROAR_CLIENT_H
