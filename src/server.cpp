//
// Created by joe on 7/27/24.
//

#include "server.h"

bool server::establish(const unsigned int &port, const unsigned int &addr) {
    int opt = 1;

    serv_addr.sin_family = AF_INET; // Socket will use IPv4
    serv_addr.sin_addr.s_addr = addr; // will connect to any ip address that requests access
    serv_addr.sin_port = htons(port); // sets up the port and converts the port to socket numbers

    FD_ZERO(&master_set);
    // create the new tcp socket using IPv4
    if((server_fd = socket(AF_INET,SOCK_STREAM, 0)) == 0){
        perror("Socket Failed");
        return false;
    }

    // setting up the socket options so that the socket can be reused and multiple sockets can bind to the same port
    if(setsockopt(server_fd,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("Set sock opt Failed");
        return false;
    }

    // binding the socket with the address and port desired
    if(bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Bind Failed");
        return false;
    }
    // add the new server fd to the set, so it can listen for incoming connections for select/activity
    FD_SET(server_fd,&master_set);
    return true;
}

fd_ctl server::pullIn(){
    fd_ctl incoming_fdCtl;

    // listening for connections with a backlog queue of up to 3 pending connections
//    if(listen(server_fd, 3) < 0){
//        perror("Listen Failed");
//    }

    int addrLen = (sizeof(incoming_fdCtl.addr_in));

    // accepting connections and returns a new socket descriptor for the connection
    if((incoming_fdCtl.fd = accept(server_fd, (struct sockaddr*)&incoming_fdCtl.addr_in, (socklen_t*)&addrLen)) < 0){
        perror("Accept Failed");
    }
    return incoming_fdCtl;
}

bool server::connect(const fd_ctl &incoming_fdCtl){
    // check if the file descriptor is valid
    if(incoming_fdCtl.fd <= 0){
        return false;
    }
    // update the max_fd if valid
    if(incoming_fdCtl.fd > max_fd) {
        max_fd = incoming_fdCtl.fd;
    }
    // Add to the set for action
    FD_SET(incoming_fdCtl.fd,&master_set);

    for(auto & fd_client : fd_attache){
        //todo check if the fd_client is full and respond on that
        if(fd_client.id == -1){ // If it has not been set since default init is -1
            fd_client = incoming_fdCtl;
            break;
        }
    }
    std::cout << "Connection Established: Socket Fd: " << incoming_fdCtl.fd << ", IP: " << inet_ntoa(incoming_fdCtl.addr_in.sin_addr)
        << ", Port: " << ntohs(incoming_fdCtl.addr_in.sin_port) << std::endl;

    return true;
}

void server::relay(){}

void server::operate() {
    // wait for one of the sockets in the master set to do an activity
    int activity = select(max_fd, &master_set,nullptr,nullptr,nullptr);

    // check if it's valid and good
    if((activity < 0) && (errno != EINTR)){
        perror("Select Error");
    }
    // check if there's any incoming connections in the server fd then connects them with server if so
    if(FD_ISSET(server_fd,&master_set)){
        fd_ctl incomingPull = pullIn();
        connect(incomingPull);
    }
    // Check incoming operations from any of the connected fds
    for(fd_ctl &fdCtl : fd_attache){
        if(fdCtl.id != -1){ // check if valid
            if(FD_ISSET(fdCtl.fd,&master_set)){ // if operation occurred on
                read(fdCtl.id,buffer,1024);
                std::cout << "Message From Client: " << buffer << std::endl;
                send(fdCtl.fd, "Server Received Message", strlen("Server Received Message"),0);
            }
        }
    }
}
