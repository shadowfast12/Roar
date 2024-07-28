//
// Created by joe on 7/27/24.
//

#include "server.h"

server::server(const unsigned int &port, const unsigned int& addr){
    int opt = 1;

    serv_addr.sin_family = AF_INET; // Socket will use IPv4
    serv_addr.sin_addr.s_addr = addr; // will connect to any ip address that requests access
    serv_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if((server_fd = socket(AF_INET,SOCK_STREAM, 0)) == 0){
        perror("Socket Failed");
    }

    // setting up the socket options so that the socket can be reused and multiple sockets can bind to the same port
    if(setsockopt(server_fd,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("Set sock opt Failed");
    }

    // binding the socket with the address and port desired
    if(bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Bind Failed");
    }

}

void server::operate() {

    // listening for connections with a backlog queue of up to 3 pending connections
    if(listen(server_fd, 3) < 0){
        perror("Listen Failed");
    }

    int addrLen = (sizeof(serv_addr));

    // accepting connections and returns a new socket desciptor for the connection
    if((incoming_fd = accept(server_fd, (struct sockaddr*)&serv_addr, (socklen_t*)&addrLen)) < 0){
        perror("Accept Failed");
    }

    read(incoming_fd, buffer, 1024);
    std::cout << "Message from client: " << buffer << std::endl;

    send(incoming_fd, "Server says hello", strlen("Server says hello"), 0);
    std::cout << "Hello message sent" << std::endl;

    close(incoming_fd);
    close(server_fd);
}
