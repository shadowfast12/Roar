//
// Created by joe on 7/27/24.
//

#include "server.h"

void server::establish(const unsigned int &port, const unsigned int &addr)
{
    int opt = 1;

    server_addr.sin_family = AF_INET;   // Socket will use IPv4
    server_addr.sin_addr.s_addr = addr; // will connect to any ip address that requests access
    server_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    // create the new tcp socket using IPv4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Failed");
    }

    // setting up the socket options so that the socket can be reused and multiple sockets can bind to the same port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("Set sock opt Failed");
    }

    // binding the socket with the address and port desired
    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind Failed");
    }
}

void server::listen()
{
    // listening for connections with a backlog queue of up to 3 pending connections
    if (::listen(server_fd, 3) == -1)
    {
        perror("Listen Failed");
    }
    cout << "listening..." << endl;
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
        if (errno == EINTR) // signal handler doesn't prevent signals from interrupting blocking calls (ex: select())
        {
            perror("interruption signal was sent");
        }
        else
        {
            perror("readiness error");
        }
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
            return -1;
        }

        // add the client to the list
        client_fds.push_back(client_fd);
        cout << "client accepted" << endl;
        cout << "new client: " << client_fd << ". IP: " << inet_ntoa(client_addr.sin_addr) << endl;
        return client_fd;
    }
    return -1;
}

void server::send(int client_fd)
{
    cout << "Message to client: " << endl;
    char message[100];
    fgets(message, sizeof(message), stdin);
    if (::send(client_fd, message, strlen(message), 0) == -1)
    {
        perror("Sending request unsuccessful");
    }
}

int server::selectClient(int client_fd)
{
    int recipient_client = -1;

    string client_list;
    string full_message;
    char init_message[1024];

    while (client_fd != -1 && client_fds.size() > 1)
    {
        client_list = ""; // resetting every loop
        full_message = "";
        for (int i = 0; i < client_fds.size(); i++)
        {
            if (i == client_fds.size() - 1)
            {
                client_list += std::to_string(client_fds[i]);
            }
            else
            {
                client_list += (std::to_string(client_fds[i]) + ", ");
            }
        }
        full_message = "give the number of the client you wish to send message to: " + client_list;
        std::strcpy(init_message, full_message.c_str());
        if (::send(client_fd, init_message, sizeof(init_message), 0) < 0)
        {
            perror("sending message failed");
        }
        if (read(client_fd, buffer, sizeof(buffer)) < 0)
        {
            perror("Message not received");
        }
        std::string str(buffer);
        recipient_client = std::stoi(str);
        cout << "wishlist client: " << recipient_client << endl;
        clearBuffer();
        if (std::find(client_fds.begin(), client_fds.end(), recipient_client) == client_fds.end())
        {
            char error_message[] = "client not found!";
            if (::send(client_fd, error_message, sizeof(error_message), 0) < 0)
            {
                perror("sending message failed");
            }
        }
        else
        {
            continue;
        }
    }

    return recipient_client;
}

void server::handleComm(int client_fd, int recipient) // add threading for receive
{
    while (client_fd != -1 && recipient != -1)
    {
        cout << "trying to receive message from client " << client_fd << endl;
        // the client is ready to read
        valread = read(client_fd, buffer, sizeof(buffer));
        cout << valread << " bytes of data were read" << endl;
        if (valread < 0)
        {
            perror("Message not received");
            break;
        }

        if (valread == 0)
        {
            cout << "client disconnected" << endl;
            getpeername(client_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr);
            cout << "IP: " << inet_ntoa(client_addr.sin_addr) << endl;

            // remove the client from the list
            close(client_fd);
            auto client_loc = find(client_fds.begin(), client_fds.end(), client_fd);
            client_fds.erase(client_loc);
            break;
        }
        // received message and now send it back to the client
        if (::send(recipient, buffer, sizeof(buffer), 0) < 0)
        {
            perror("sending message failed");
        }
        cout << buffer << endl;

        clearBuffer();
    }
}

void server::clearBuffer()
{
    memset(buffer, 0, sizeof(buffer));
}

void server::terminate()
{
    try
    {
        int result;
        // make a vector of client file descriptors
        for (int client_fd : client_fds)
        {
            result = close(client_fd);
            if (result != -1)
            {
                cout << "client " << client_fd << " has been closed successfully" << endl;
            }
        }
        result = close(server_fd);
        if (result != -1)
        {
            cout << "server " << server_fd << " has been closed successfully" << endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception during cleanup: " << e.what() << std::endl;
    }
}
