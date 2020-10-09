#include <iostream>
#include <string>
#include "validation.h"
#include "GCClient.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        // numero de parametros errados
        cout << "./app <username> <groupname> <server_ip_address> <port>" << endl;
        return 1;
    }

    string username = argv[1];
    string group_name = argv[2];
    string server_ip_address = argv[3];
    int port;
    try
    {
        port = stoi(argv[4]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "port precisa ser um número!" << '\n';
        return 1;
    }

    string err = Validation::ValidateInputs(username, group_name, server_ip_address, port);
    if (err != "")
    {
        // Um ou mais parametros estão errados
        cout << err << endl;
        return 1;
    }

    // Validacao terminou, começa o app

    // Instancia componentes
    GCClient *gcc = new GCClient(server_ip_address, port, username, group_name);
    GMClient *gmc = new GMClient(gcc, username, group_name);
    Interface *inte = new Interface(gmc, username);

    //roda o app
    inte->Start();

    delete inte;
    delete gmc;
    delete gcc;
}
