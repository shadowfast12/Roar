//
// Created by joe on 7/27/24.
//

#include "server.h"

bool server::establish(const unsigned int &port, const unsigned int &addr)
{
    int opt = 1;

    client_addr.sin_family = AF_INET;   // Socket will use IPv4
    client_addr.sin_addr.s_addr = addr; // will connect to any ip address that requests access
    client_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket Failed");
        close(server_fd);
        return false;
    }

    // setting up the socket options so that the socket can be reused and multiple sockets can bind to the same port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Set sock opt Failed");
        close(server_fd);
        return false;
    }

    // binding the socket with the address and port desired
    if (bind(server_fd, (sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    {
        perror("Bind Failed");
        close(server_fd);
        return false;
    }
    return true;
}

bool server::listen()
{
    // listening for connections with a backlog queue of up to 3 pending connections
    if (::listen(server_fd, 3) == -1)
    {
        perror("Listen Failed");
        close(server_fd);
        return false;
    }
    cout << "listening..." << endl;
    return true;
}

bool server::accept() // continue to accept clients until an error
{
    int addrLen = (sizeof(client_addr));

    // accepting connections and returns a new socket descriptor for the connection
    if ((incoming_fd = ::accept(server_fd, (sockaddr *)&client_addr, (socklen_t *)&addrLen)) == -1)
    {
        cout << "accept failed" << endl;
        perror("Accept Failed");
        close(incoming_fd);
        close(server_fd);
        return false;
    }
    cout << "client accepted" << endl;
    return true;
}

// to receive the client's username and bind it with the socket, basically registering the user.
bool server::registerUser()
{
    int bytes_read;

    while ((bytes_read = read(incoming_fd, buffer, 1024)) > 0)
    {
        std::cout << "Client's username: " << buffer << std::endl;
    }

    if (bytes_read < 0)
    {
        perror("Failed to receive username from client");
        close(incoming_fd);
        close(server_fd);
        return false;
    }

    // the client's file descriptor is now associate with the username
    system[buffer] = incoming_fd;

    cout << "inside the system: " << system[buffer] << endl;

    return true;
}

bool server::handle_client(char *username)
{
    server::accept();
    server::registerUser();
}

void server::terminate()
{
    close(server_fd);
}
