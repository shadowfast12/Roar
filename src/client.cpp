//
// Created by joe on 7/27/24.
//

#include "client.h"

bool client::connect(const unsigned int &port, const char *serverIP)
{

    server_addr.sin_family = AF_INET;   // Socket will use IPv4
    server_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Failed");
        return false;
    }

    // prepare the server's ip address for a connection attempt and check if it's good
    // storing the binary form of "serverIP" into "server_addr.sin_addr"
    if (inet_pton(AF_INET, (const char *)serverIP, &server_addr.sin_addr) < 0)
    {
        perror("Server Address Failed");
        return false;
    }

    // connect to the server
    if (::connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Server connection Failed");
        return false;
    }
    cout << "connection successful" << endl;
    return true;
}

bool client::send(const char *message)
{

    if (::send(client_fd, message, strlen(message), 0) < 0)
    {
        perror("Sending message Failed");
        return false;
    }

    int bytes_read;
    while ((bytes_read = ::read(client_fd, buffer, 1024)) > 0) // to constantly check if the message is read
    {
        std::cout << "FROM SERVER: " << buffer << std::endl;
    }

    if (bytes_read < 0)
    {
        perror("Relay from server Failed");
        return false;
    }
    return true;
}

bool client::receive()
{
    int bytes_size = read(client_fd, buffer, sizeof(buffer));
    if (bytes_size < 0)
    {
        perror("Message not received");
        return false;
    }

    cout << "Server: " << buffer << endl;
    return true;
}
void client::terminate()
{
    close(client_fd);
}
