#pragma once
#ifndef GGSERVER_H
#define GGSERVER_H

#include "GCServer.h"
#include <shared_mutex>
#include <map>

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
	}
	
	GGServer(){};
};

#endif