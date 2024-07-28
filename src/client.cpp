//
// Created by joe on 7/27/24.
//

#include "client.h"


client::client(const unsigned int &port, const char* serverIP) {

    client_addr.sin_family = AF_INET; // Socket will use IPv4
    client_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if((client_fd = socket(AF_INET,SOCK_STREAM, 0)) == 0){
        perror("Socket Failed");
    }

    // prepare the server's ip address for a connection attempt and check if it's good
    if(inet_pton(AF_INET, (const char *) serverIP, &client_addr.sin_addr) <= 0){
        perror("Server Address Failed");
    }

    // connect to the server
    if(connect(client_fd, (struct sockaddr*)&client_addr,sizeof(client_addr)) < 0){
        perror("Server connection Failed");
    }
}

void client::operate() {

    send(client_fd, "Client says hello", strlen("Client says hello"), 0);
    std::cout << "Hello message sent" << std::endl;

    read(client_fd, buffer, 1024);
    std::cout << "Message from server: " << buffer << std::endl;

    close(client_fd);
}
