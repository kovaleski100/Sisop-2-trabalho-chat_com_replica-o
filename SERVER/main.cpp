#include <iostream>
#include <string>
#include "gmServer.h"


using namespace std;

int main(int argc, char *argv[])
{

    int port = atoi(argv[1]);
    GGServer *ggs = new GGServer();
    GCServer *gcs = new GCServer(ggs, port);

    delete gcs;
    delete ggs;
    

}