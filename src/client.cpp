//
// Created by joe on 7/27/24.
//

#include "client.h"


bool client::connect(const unsigned int &port, const char* serverIP) {

    client_addr.sin_family = AF_INET; // Socket will use IPv4
    client_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if((client_fd = socket(AF_INET,SOCK_STREAM, 0)) == 0){
        perror("Socket Failed");
        return false;
    }

    // prepare the server's ip address for a connection attempt and check if it's good
    if(inet_pton(AF_INET, (const char *) serverIP, &client_addr.sin_addr) <= 0){
        perror("Server Address Failed");
        return false;
    }

    // connect to the server
    if(::connect(client_fd, (struct sockaddr*)&client_addr,sizeof(client_addr)) < 0){
        perror("Server connection Failed");
        return false;
    }
    return true;
}

bool client::send(const char* message){
    //
    if(::send(client_fd,message, strlen(message),0) < 0){
        perror("Sending message Failed");
        return false;
    }
    // check if message was received from server
    if(::read(client_fd,buffer,1024) == -1){
        perror("Relay from server Failed");
        return false;
    }
    std::cout << "FROM SERVER: " << buffer << std::endl;

    close(client_fd);
    return true;
}

