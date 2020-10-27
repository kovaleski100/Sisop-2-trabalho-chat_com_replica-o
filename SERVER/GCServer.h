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
	int main_socket;
	bool main_replica;
	Mensagem build_Mensagem(char *buffer);
	void listen_app(int newsockfd);
	int create_socket(int port);
	void handle_new_conections(int sockfd);
	void connect_to_main_server(string server_adress, int port);
	void listen_main_server();
	void register_itself(int port);

public:
	GCServer(GGServer *ggs_, int port);
	GCServer(GGServer *ggs_, int port, int main_port);
	~GCServer();
	void Send_all(Mensagem message);
};

#endif