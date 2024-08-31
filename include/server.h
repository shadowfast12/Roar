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
#include <vector>
#include <algorithm>

using namespace std;

// TODO ADD A HANDLER SERVER THAT WILL DIRECT THE DIFFERENT SOCKETS ACCORDINGLY TO A CLIENT-SIDE SERVER
// TODO ABILITY TO CREATE A CHATROOM AND INVITE/ALLOW CERTAIN USERNAMES/USER-IDS
// TODO ABILITY TO SEARCH FOR A USERNAME AND CONNECT WITH THAT PERSON IF DESIRED
// TODO ADD WINDOWS VERSION USING WIN-SOCKETS

// My Goal:
// send client-to-client messages
// implement threading
// set up the standard practice to decide when to close the file descriptors

// PROBLEMS:
// 1. client file descriptor suddenly changes to -1 (most likely from corrupted file. bad closure of program)
// 2. when client tries to send() after the server has disconnected, it should return -1 (and an error), but
//    not run at all
// 3. find a way to gracefully end the connection

class server
{
public:
    // port to connect to & desired address to allow a socket connection
    void establish(const unsigned int &port = 8080, const unsigned int &addr = INADDR_ANY);
    void listen();
    int accept(); // to handle messages from different clients
    void send(int client_fd);
    void receive(int client_fd);
    void clearBuffer();
    void terminate();

private:
    int server_fd = -1;                          // server file descriptor
    struct sockaddr_in server_addr, client_addr; // stores server and client address info
    char buffer[1024] = {0};                     // buffer for receiving message
    vector<int> client_fds;                      // stores all client file descriptors
    // we keep client_fds in case we want to send client's message to another client.
    // to do that, we need a list of client file descriptors

    // this is for select()
    fd_set readfds;      // a set of file descriptors
    ssize_t valread = 0; // number of bytes read from clients
    // SIZE_T IS UNSIGNED. WHEN NEGATIVE, IT RETURNS A LARGE INTEGER
    int maxfd = 0;     // the highest number of file descriptor
    int readiness = 0; // holds the number of file descriptors ready
};

// use select() to manage connection
#endif
