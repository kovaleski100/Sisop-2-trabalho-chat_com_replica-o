#include <iostream>

using namespace std;

void main(int argc, char *argv[])
{
    string username;
    string group_name;
    string server_ip_address;
    int port;
    if(argc != 5)
    {
        cout<< "Numeros de argumentos errados"
    }
    else
    {
        username = argv[1];
        group_name = argv[2];
        server_ip_address = argv[3];
        port = argv[4];
    }
}
