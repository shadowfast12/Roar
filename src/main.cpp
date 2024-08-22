//
// Created by joe on 7/26/24.
//

#include "server.h"
#include "client.h"
#include <thread> // this enables multiple connections

int main()
{
    vector<thread> threads; // to store all individual threads (sockets)
    string mode;

    cout << "MODE: ";
    getline(cin, mode);

    if (mode == "server")
    {
        server testServer = server();
        testServer.establish();
        testServer.listen();
        while (true)
        {
            testServer.accept();
            threads.emplace_back(&server::receive, &testServer);
        }
        testServer.terminate();
    }
    else
    {
        client testClient = client();
        testClient.connect();
        cout << "message: ";
        char message[1024];
        fgets(message, sizeof(message), stdin);

        while (true)
        {
            fgets(message, sizeof(message), stdin);
            testClient.send(message);
        }
        testClient.terminate();
    }
    return 0;
}
