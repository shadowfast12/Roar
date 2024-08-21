//
// Created by joe on 7/27/24.
//

#ifndef ROAR_SERVER_H
#define ROAR_SERVER_H

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// TODO ADD A HANDLER SERVER THAT WILL DIRECT THE DIFFERENT SOCKETS ACCORDINGLY TO A CLIENT-SIDE SERVER
// TODO ABILITY TO CREATE A CHATROOM AND INVITE/ALLOW CERTAIN USERNAMES/USER-IDS
// TODO ABILITY TO SEARCH FOR A USERNAME AND CONNECT WITH THAT PERSON IF DESIRED
// TODO ADD WINDOWS VERSION USING WIN-SOCKETS

struct fd_ctl{
    int fd = -1; // the file descriptor
    char id = -1; // identify the type of the file descriptor (server, client, etc)
    sockaddr_in addr_in{}; // socket address information

    fd_ctl(int fd, char id, sockaddr_in addr_in): fd(fd),id(id),addr_in(addr_in){}
    fd_ctl()= default;
};

class server {
public:
    // port to connect to & desired address to allow a socket connection
    bool establish(const unsigned int &port = 8080, const unsigned int &addr = INADDR_ANY);

    // pulls in the incoming connections and accepts them into the file descriptor then returns the fd
    // returns the incoming file descriptor int & sock addr info
    fd_ctl pullIn();

    // Adds connects to ONE file descriptor; could be used with pullIn
    bool connect(const fd_ctl &incoming_fdCtl);
    void relay();
    void operate();


private:
    int server_fd; // socket file descriptor
    int max_fd; // Primarily used for the master_set for the searching; largest fd of the set & array
    fd_ctl fd_attache[MAX_CLIENTS]; // used to store the file descriptors
    fd_set master_set; // socket file descriptor set r/ client; used for checking on the file descriptors
    struct sockaddr_in serv_addr; // socket address information for the client
    char buffer[BUFFER_SIZE] = {0}; // buffer for receiving information
};


#endif //ROAR_SERVER_H
