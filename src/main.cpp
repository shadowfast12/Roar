//
// Created by joe on 7/26/24.
//

#include "server.h"
#include "client.h"

int main()
{
    string mode;

    cout << "MODE: ";
    getline(std::cin, mode);

    if (mode == "server")
    {
        server testServer = server();
        testServer.establish();
        string disconnect;
        testServer.listen();
        while (true)
        {
            testServer.accept();
            testServer.registerUser();
        }
    }
    else
    {
        client testClient = client();
        testClient.connect();
        cout << "Login: ";
        string message;
        getline(std::cin, message);

        cout << std::endl;
        testClient.login(message.c_str());
        testClient.terminate();
    }
    return 0;
}
