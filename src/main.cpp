//
// Created by joe on 7/26/24.
//

#include "server.h"
#include "client.h"
#include <thread> // this enables multiple connections
#include <signal.h>
#include <atomic>
std::atomic<bool> running(true);

void signalHandler(int signal)
{
    cout << "caught a signal " << signal << endl;
    running = false;
}

int main()
{
    vector<thread> threads; // to store all individual threads (sockets)
    string mode;
    struct sigaction sigIntHandler; // handles Ctrl+C, so it doesn't unexpectedly halt the program
    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    cout << "MODE: ";
    getline(cin, mode);

    if (mode == "server")
    {
        server testServer = server();
        testServer.establish();
        testServer.listen();
        while (running)
        {
            int sd = testServer.accept();
            threads.emplace_back(&server::receive, &testServer, sd); // cannot read the first message for some reason
        }
        for (auto &thr : threads)
        {
            thr.join();
            cout << "thread closed" << endl;
        }
        cout << "Server disconnected" << endl;
        testServer.terminate();
    }
    else
    {
        client testClient = client();
        testClient.connect();
        char message[1024];

        while (running)
        {
            cout << "message: ";
            fgets(message, sizeof(message), stdin);
            cout << "sending..." << endl;
            testClient.send(message);
        }
        testClient.terminate();
    }
    return 0;
}
