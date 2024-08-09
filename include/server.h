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
#include <unordered_map>
#include <string>
#include <thread> // this enables multiple connections

using namespace std;

// TODO ADD A HANDLER SERVER THAT WILL DIRECT THE DIFFERENT SOCKETS ACCORDINGLY TO A CLIENT-SIDE SERVER
// TODO ABILITY TO CREATE A CHATROOM AND INVITE/ALLOW CERTAIN USERNAMES/USER-IDS
// TODO ABILITY TO SEARCH FOR A USERNAME AND CONNECT WITH THAT PERSON IF DESIRED
// TODO ADD WINDOWS VERSION USING WIN-SOCKETS

class server
{
public:
    // port to connect to & desired address to allow a socket connection
    bool establish(const unsigned int &port = 8080, const unsigned int &addr = INADDR_ANY);
    bool listen();
    bool accept();
    // register the client's username into the system (dictionary)
    bool registerUser();
    bool handle_client(char *username); // to handle messages from different clients
    void terminate();

private:
    int server_fd, incoming_fd;     // socket file descriptor
    struct sockaddr_in client_addr; // client's address
    char buffer[1024] = {0};        // buffer for receiving information
    unordered_map<string, int> system;
};

#endif // ROAR_SERVER_H
