//
// Created by joe on 7/27/24.
//

#include "client.h"

void client::connect(const unsigned int &port, const char *serverIP)
{

    server_addr.sin_family = AF_INET;   // Socket will use IPv4
    server_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Failed");
        terminate();
    }

    // prepare the server's ip address for a connection attempt and check if it's good
    // storing the binary form of "serverIP" into "server_addr.sin_addr"
    if (inet_pton(AF_INET, (const char *)serverIP, &server_addr.sin_addr) < 0)
    {
        perror("Server Address Failed");
        terminate();
    }

    // connect to the server
    if (::connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Server connection Failed");
        terminate();
    }
    cout << "connection successful" << endl;
}

void client::send(const char *message)
{
    ssize_t bytes_sent = ::send(client_fd, message, strlen(message), 0); // send() not returning an error after trying to send to a disconnected server
    cout << "bytes sent: " << bytes_sent << endl;
    if (bytes_sent < 0)
    {
        if (errno == EPIPE)
        {
            cout << "server disconnected" << endl;
        }
        perror("Sending message Failed");
        terminate();
    }

    int bytes_read;
    cout << "receiving message back from server" << endl;
    // this while loop stops when false
    // CHANGE: deleted while loop
    bytes_read = ::read(client_fd, buffer, 1024);

    std::cout << "FROM SERVER: " << buffer << std::endl;

    if (bytes_read < 0)
    {
        perror("Relay from server Failed");
        terminate();
    }
}

void client::receive()
{
    int bytes_size = read(client_fd, buffer, sizeof(buffer));
    if (bytes_size < 0)
    {
        perror("Message not received");
        terminate();
    }

    cout << "Server: " << buffer << endl;
}
void client::terminate()
{
    close(client_fd);
}
