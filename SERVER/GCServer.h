#pragma once
#ifndef GCSERVER_H
#define GCSERVER_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include <map>
#include "../mensagemStruct.h"

class GGServer;

using namespace std;

struct Dispositivo
{
	int socket;
	string username;
};

class GCServer
{
private:
	std::map<std::string, std::vector<Dispositivo>> group_map;
	GGServer *ggs;
	Mensagem build_Mensagem(char *buffer);
	void listen_app(int newsockfd);
	int create_socket(int port);
	void handle_new_conections(int sockfd);

public:
	GCServer(GGServer *ggs_, int port);
	~GCServer();
	void Send_all(Mensagem message);
};

#endif