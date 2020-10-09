#include <iostream>
#include <string>
#include "gmServer.h"


using namespace std;

int main(int argc, char *argv[])
{
    int port;
    try
    {
        port = stoi(argv[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "./server <port>" << endl << "port precisa ser um número!" << endl;
        return 1;
    }

    GGServer *ggs = new GGServer();
    GCServer *gcs = new GCServer(ggs, port);

    delete gcs;
    delete ggs;
    

}