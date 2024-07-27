//
// Created by joe on 7/27/24.
//

#include "client.h"


bool client::setup(){
    const int opt = 1;

    // create the new tcp socket using IPv4
    if(!(server_fd = socket(AF_INET,SOCK_STREAM, 0))){
        perror("Socket Failed");
        return false;
    }

    // binding the socket with the address and port desired
    if(bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Bind Failed");
        return false;
    }

    return true;
}

void client::operate() {

    // listening for connections with a backlog queue of up to 3 pending connections
    if(listen(server_fd, 3) < 0){
        perror("Listen Failed");
    }

    int addrLen = (sizeof(serv_addr));

    // accepting connections and returns a new socket desciptor for the connection
    if((incoming_fd = accept(server_fd, (struct sockaddr*)&serv_addr, (socklen_t*)&addrLen))){
        perror("Accept Failed");
    }

    read(incoming_fd, buffer, 1024);
    std::cout << "Message from client: " << buffer << std::endl;

    send(incoming_fd, "Server says hello", strlen("Server says hello"), 0);
    std::cout << "Hello message sent" << std::endl;

    close(incoming_fd);
    close(server_fd);
}
