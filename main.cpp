//
// Created by joe on 7/26/24.
//

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
    int clientSocket
            = accept(serverSocket, nullptr, nullptr);

    // recieving data
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer
         << std::endl;

    // closing the socket.
    close(serverSocket);

    return 0;

}

