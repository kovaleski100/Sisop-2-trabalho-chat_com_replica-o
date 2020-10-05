#include "GM.h"
#include <map>

using namespace std;

class Group_M{
	private:
	map<string,GMServer*> GM_map;
	public:
	vector<Mensagem> ReadMessage(string group);
	void WriteMessage(Mensagem mensagem);
};

vector<Mensagem>  Group_M::ReadMessage(string group){
	map<string,GMServer*>::iterator it;
	vector<Mensagem> teste;
	it = GM_map.find(group);
	
	if(it == GM_map.end()){
		GMServer *new_GM = new GMServer(group);
		GM_map.insert(pair<string,GMServer*>(group,new_GM));
		it = GM_map.find(group);
	}

	teste = it->second->ReadLastMessages(N);
	return teste;
}

void Group_M::WriteMessage(Mensagem mensagem){
	map<string,GMServer*>::iterator it;
	vector<Mensagem> teste;
	it = GM_map.find(mensagem.grupo);
	
	if(it == GM_map.end()){
		GMServer *new_GM = new GMServer(mensagem.grupo);
		GM_map.insert(pair<string,GMServer*>(mensagem.grupo,new_GM));
		it = GM_map.find(mensagem.grupo);
	}

	it->second->WriteMessage(mensagem);
	return;
}
