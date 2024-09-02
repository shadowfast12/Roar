//
// Created by joe on 7/26/24.
//

#include "server.h"
#include "client.h"
#include <thread> // this enables multiple connections
#include <signal.h>
#include <atomic>
#include <fcntl.h>

int main()
{
    vector<thread> threads; // to store all individual threads (sockets)
    string mode;

    cout << "MODE: ";
    getline(cin, mode);

    if (mode == "server")
    {
        server testServer = server();
        string command = "";

        testServer.establish();
        testServer.listen();
        while (true)
        {
            int sd = testServer.accept();
            int recipient = testServer.selectClient(sd);
            threads.emplace_back(&server::handleComm, &testServer, sd, recipient); // cannot read the first message for some reason
            cout << "do you wish to disconnect the server? (Press yes to disconnect)";
            getline(cin, command);
            if (command == "yes")
            {
                for (auto &thr : threads)
                {
                    thr.join();
                    cout << "thread closed" << endl;
                }
                cout << "Server disconnected" << endl;
                testServer.terminate();
            }
        }
    }
    else
    {
        client testClient = client();
        testClient.connect();
        char message[1024];

        string command;

        while (true)
        {
            testClient.receive();
            cout << "message: ";
            fgets(message, sizeof(message), stdin);
            cout << "sending..." << endl;
            testClient.send(message);
            cout << "do you wish to disconnect? Press yes to disconnect";
            getline(cin, command);
            if (command == "yes")
            {
                testClient.terminate();
            }
        }
    }
    return 0;
}
