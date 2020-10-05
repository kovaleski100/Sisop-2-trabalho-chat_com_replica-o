#include "gmServer.h"
#include <map>

using namespace std;

class Group_M{
	private:
	map<string,GMServer*> GM_map;
	mutable shared_timed_mutex mutex;
	public:
	vector<Mensagem> ReadMessage(string group);
	void WriteMessage(Mensagem mensagem);
	
	Group_M(){
	
	
	}
};

vector<Mensagem>  Group_M::ReadMessage(string group){
	map<string,GMServer*>::iterator it;
	
	unique_lock<shared_timed_mutex> lock(mutex);
	it = GM_map.find(group);
	
	if(it == GM_map.end()){
		GMServer *new_GM = new GMServer(group);
		GM_map.insert(pair<string,GMServer*>(group,new_GM));
		it = GM_map.find(group);
	}
	unique_lock<shared_timed_mutex> unlock(mutex);
	return it->second->ReadLastMessages(N);
}

void Group_M::WriteMessage(Mensagem mensagem){
	map<string,GMServer*>::iterator it;
	
	unique_lock<shared_timed_mutex> lock(mutex);
	it = GM_map.find(mensagem.grupo);
	
	if(it == GM_map.end()){
		GMServer *new_GM = new GMServer(mensagem.grupo);
		GM_map.insert(pair<string,GMServer*>(mensagem.grupo,new_GM));
		it = GM_map.find(mensagem.grupo);
	}
	unique_lock<shared_timed_mutex> unlock(mutex);
	it->second->WriteMessage(mensagem);
	return;
}
