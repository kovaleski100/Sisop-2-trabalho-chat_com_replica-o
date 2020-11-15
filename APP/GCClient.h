#pragma once
#ifndef GCCLIENT_H
#define GCCLIENT_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include "../mensagemStruct.h"
#include <unistd.h>
#include "gmClient.h"

using namespace std;

class GCClient
{
private:
    string server_adress_;
    int server_port;
    string user_;
    string group_;
    int server_socket;
    int client_port;
    GMClient *gmc;
    void connect_server(string server_adress, int port);
    void register_itself(string user, string group);
    void listen_server();
    Mensagem build_Mensagem(char *buffer);
    int create_socket();
    void handle_new_main_servers_conections(int port);

public:
    GCClient(string server_adress, int port, string user, string group);
    ~GCClient();
    void Start();
    void Send_server(Mensagem mensagem);
    void Set_gmc(GMClient *gmc_)
    {
        gmc = gmc_;
    }
};

#endif
