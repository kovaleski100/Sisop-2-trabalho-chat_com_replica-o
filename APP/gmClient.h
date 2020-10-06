#pragma once
#ifndef GMCLIENT_H
#define GMCLIENT_H
#include <string>
#include <iostream>
#include "../mensagemStruct.h"
#include "interface.h"
#include "Connection_client.cpp"
#include <sys/socket.h>

using namespace std;



class GMClient
{
private:
    string user;
    string group;
    string server_ip;
    int port;
    
    // Um ponteiro pra uma instancia do GCClient
    // Vai ser usado pra chamar a funcao pra enviar uma mensagem pro server.
    //GCClient * gcClient;

    Interface *interface;

    Mensagem buildMensagem(string text);

public:
    GMClient(string user_, string group_, int port_, string server_ip_)
    {
        user = user_;
        group = group_;
        port = port_;
        server_ip = server_ip_;
    };

    bool SendMessage(string text);
    // essa função DisplayMessage vai ser chamada pelo GCClient
    // para mostrar a mensagem na interfarce.
    // GCClient vai ter que ter um ponteiro pra uma instancia de GMClient.
    bool DisplayMessage(Mensagem message);

    void setInterface(Interface *interface_)
    {
        interface = interface_;
    }
};

void serialize_data(Mensagem* m1,string data);
void deserialize_data(string data, Mensagem* m1);

#endif
