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
    int server_socket;
	int port_n;
	int other_port;	//nova porta "reservada"
	string ip;
	string user_n;
	string group_n;
    GMClient *gmc;
    void connect_server(string server_adress, int port);
    void register_itself(string user, string group);
    void listen_server();
	int	resv_port(); // "reserva" uma porta para ser usada caso perda de conexao
    Mensagem build_Mensagem(char *buffer);
	void handle_new_conections(int port);
	int create_socket(int port);
	void conection();

public:
    GCClient(string server_adress, int port, string user, string group);
    ~GCClient();
    void Send_server(Mensagem mensagem);
    void Set_gmc(GMClient *gmc_)
    {
        gmc = gmc_;
    }
};

#endif
