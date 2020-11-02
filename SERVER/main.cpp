#include <iostream>
#include <string>
#include "gmServer.h"


using namespace std;

int main(int argc, char *argv[])
{
    int port, main_port;
    try
    {
        port = stoi(argv[1]);
        if (argc > 2)
        {
            main_port = stoi(argv[2]);
        }
        
    }
    catch (const std::exception &e)
    {
        std::cerr << "./server <port> [<main_port>]" << endl << "port precisa ser um número!" << endl;
        return 1;
    }

    GGServer *ggs = new GGServer();
    GCServer *gcs;
    if (argc == 2)
    {
        cout << "Starting main server" << endl;
        gcs = new GCServer(ggs, port);
    } else
    {
        cout << "Starting backup server" << endl;
        gcs = new GCServer(ggs, port, main_port);
    }

    delete gcs;
    delete ggs;
    

}