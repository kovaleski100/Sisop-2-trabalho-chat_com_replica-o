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
#include <vector>
#include "../mensagemStruct.h"

class GGServer;

using namespace std;

struct Dispositivo
{
	int socket;
	string username;
};

struct Backup
{
	int socket;
	int port;
};

class GCServer
{
private:
	std::map<std::string, std::vector<Dispositivo>> group_map;
	GGServer *ggs;
	int main_socket;
	std::vector<Backup> backup_vector;
	bool main_replica;
	Mensagem build_Mensagem(char *buffer);
	void listen_app(int newsockfd);
	int create_socket(int port);
	void handle_new_conections(int sockfd);
	void register_new_connection(int newsocket);
	void register_new_backup(int socket, int port);
	void send_all_backups(string text);
	//backup ------
	void connect_to_main_server(string server_adress, int port);
	void listen_main_server();
	void register_itself(int port);	
	void backup_register_app(string app_info);
	int next_port_ring_election;
	void start_election();

public:
	GCServer(GGServer *ggs_, int port);
	GCServer(GGServer *ggs_, int port, int main_port); // backup
	~GCServer();
	void Send_all(Mensagem message);
	string Replica_name;
};

#endif