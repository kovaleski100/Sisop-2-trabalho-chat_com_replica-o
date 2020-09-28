#include <iostream>
#include "user.cpp"
#include <string.h>
#include "Connection.cpp"


using namespace std;

void main(int argc, char *argv[])
{
    string username;
    string group_name;
    string server_ip_address;
    int port;
    if(argc != 5)
    {
        cout<< "Numeros de argumentos errados" <<endl;
    }
    else
    {
        username = argv[1];
        group_name = argv[2];
        server_ip_address = argv[3];
        port = stoi(argv[4]);
    }
    if(validation(username))
    {
        if(validation(group_name))
        {
            if(validation_port(port))
            {
                
            }
            else
            {
                cout << "invalid port"<< endl;
            }
        }
        else
        {
            cout << "invalid groupname"<< endl;
        }
    }
    else
    {
        cout<< "invalid username"<< endl;
    }
}
