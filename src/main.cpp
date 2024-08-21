//
// Created by joe on 7/26/24.
//

#include "server.h"
#include "client.h"


int main(){
    std::string mode;
    std::cout << "MODE: ";
    getline(std::cin,mode);

    if(mode == "server"){
        server testServer;
        testServer.establish();
        listen(server_fd, 3);
        testServer.operate();
    }else{
        client testClient = client();
        testClient.connect();
        std::cout << "Message: ";
        std::string message;
        getline(std::cin,message);

        std::cout << std::endl;
        testClient.send(message.c_str());

    }
    return 0;
}

