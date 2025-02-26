#pragma once
#ifndef GMCLIENT_H
#define GMCLIENT_H
#include <string>
#include <iostream>
#include "interface.h"
#include "GCClient.h"

class GCClient;

using namespace std;

class GMClient
{
private:
    string user;
    string group;
    // Um ponteiro pra uma instancia do GCClient
    // Vai ser usado pra chamar a funcao pra enviar uma mensagem pro server.
    GCClient *gcClient;
    Interface *interface;

    Mensagem buildMensagem(string text);

public:
    GMClient(GCClient *gcClient_, string user_, string group_);
    ~GMClient();
    void Start();
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

#endif
