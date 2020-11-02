#pragma once
#ifndef GGSERVER_H
#define GGSERVER_H

#include "GCServer.h"
#include <shared_mutex>
#include <map>
#include <sys/stat.h>
#include <unistd.h>

#define N 5

class GMServer;

using namespace std;

class GGServer{
	private:
	map<string,GMServer*> GM_map;
	mutable shared_timed_mutex mutex;
	GCServer* GCS;

	public:
	vector<Mensagem> ReadMessage(string group);
	void WriteMessage(Mensagem mensagem);

	void set_GCS(GCServer* GCS_){
		GCS = GCS_;
		struct stat st = {0};
    	string replica_folder = "./SERVER/conversas/" + GCS->Replica_name;
		
		if (stat(replica_folder.c_str(), &st) == -1) {
    		mkdir(replica_folder.c_str(), 0700);
		}
	}
	
	GGServer(){};
};

#endif