//
// Created by joe on 7/27/24.
//

#include "server.h"

bool server::establish(const unsigned int &port, const unsigned int &addr)
{
    int opt = 1;

    server_addr.sin_family = AF_INET;   // Socket will use IPv4
    server_addr.sin_addr.s_addr = addr; // will connect to any ip address that requests access
    server_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket Failed");
        close(server_fd);
        return false;
    }

    // setting up the socket options so that the socket can be reused and multiple sockets can bind to the same port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("Set sock opt Failed");
        close(server_fd);
        return false;
    }

    // binding the socket with the address and port desired
    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
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

int server::accept() // continue to accept clients until an error
{
    cout << "trying to accept a client" << endl;

    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    for (auto sd : client_fds) // add client file descriptors into the fd_set
    {
        cout << "sd: " << sd << endl;
        FD_SET(sd, &readfds);
        if (sd > maxfd)
        {
            maxfd = sd;
        }
    }
    if (server_fd > maxfd)
    {
        maxfd = server_fd;
    }

    readiness = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (readiness < 0)
    {
        perror("readiness error");
        terminate();
        return -1;
    }
    cout << "there are " << readiness << " ready socket descriptors" << endl;

    if (FD_ISSET(server_fd, &readfds)) // server is ready to accept
    {
        int addrLen = (sizeof(client_addr));
        int client_fd;
        client_fd = ::accept(server_fd, (sockaddr *)&client_addr, (socklen_t *)&addrLen);
        if (client_fd < 0)
        {
            perror("Accept failed");
            terminate();
            return -1;
        }

        // add the client to the list
        client_fds.push_back(client_fd);
        cout << "client accepted" << endl;
        cout << "new client: " << client_fd << ". IP: " << inet_ntoa(client_addr.sin_addr) << endl;
    }

    return 0;
}

bool server::send(int client_fd)
{
    cout << "Message to client: " << endl;
    char message[100];
    fgets(message, sizeof(message), stdin);
    if (::send(client_fd, message, strlen(message), 0) == -1)
    {
        perror("Sending request unsuccessful");
        close(client_fd);
        close(server_fd);
        return false;
    }
    return true;
}

bool server::receive() // add threading for receive
{
    while (true)
    {
        cout << "trying to receive" << endl;
        for (int i = 0; i < client_fds.size(); i++)
        {
            cout << "there are " << client_fds.size() << " client(s)" << endl;
            int sd = client_fds[i];
            if (FD_ISSET(sd, &readfds))
            { // the client is ready to read
                cout << "ready to read from clients" << endl;
                valread = read(sd, buffer, sizeof(buffer));
                cout << "JUST READ" << endl;
                if (valread < 0)
                {
                    perror("Message not received");
                    return false;
                }

                if (valread == 0)
                {
                    cout << "client disconnected" << endl;
                    getpeername(sd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr);
                    cout << "IP: " << inet_ntoa(client_addr.sin_addr) << endl;

                    // remove the client from the list
                    close(sd);
                    client_fds.erase(client_fds.begin() + i);
                }
                else
                {
                    cout << buffer << endl;
                }
                memset(buffer, 0, sizeof(buffer));
            }
            else
            {
                cout << "no client has sent a message yet" << endl;
            }
        }
    }

    return true;
}

bool server::communicate(int client_fd)
{
    while (true)
    {
        if (::read(client_fd, buffer, sizeof(buffer)) == -1)
        {
            perror("reading message failed");
            close(server_fd);
            close(client_fd);
            break;
        }

        // echo the message back to the client
        if (::send(client_fd, buffer, sizeof(buffer), 0) == -1)
        {
            perror("sending message failed");
            close(server_fd);
            close(client_fd);
            break;
        }

        // clear the buffer?
        clearBuffer();
    }
    close(server_fd);
    close(client_fd);
    return true;
}

// vector<thread> *server::getThreads()
// {
//     return &threads;
// }

// void server::joinThreads() // to end all threads
// {
//     for (auto &t : threads)
//     {
//         t.join();
//     }
// }

void server::clearBuffer()
{
    memset(buffer, 0, sizeof(buffer));
}

void server::terminate()
{
    // make a vector of client file descriptors
    for (int client_fd : client_fds)
    {
        close(client_fd);
    }
    close(server_fd);
}
