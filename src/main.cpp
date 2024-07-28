//
// Created by joe on 7/26/24.
//

#include "server.h"
#include "client.h"


int main(){
    int mode = 0;

    if(mode == 1){
        server testServer = server();
        testServer.operate();
    }else{
        client testClient = client();
        testClient.operate();

    }


    return 0;

}

