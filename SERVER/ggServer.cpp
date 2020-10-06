#include "ggServer.h"
#include "gmServer.h"

vector<Mensagem> GGServer::ReadMessage(string group)
{
	map<string, GMServer *>::iterator it;

	unique_lock<shared_timed_mutex> lock(mutex, defer_lock);
	lock.lock();

	it = GM_map.find(group);
	if (it == GM_map.end())
	{
		GMServer *new_GM = new GMServer(GCS, group);
		GM_map.insert(pair<string, GMServer *>(group, new_GM));
		it = GM_map.find(group);
	}
	lock.unlock();

	return it->second->ReadLastMessages(N);
}

void GGServer::WriteMessage(Mensagem mensagem)
{
	map<string, GMServer *>::iterator it;

	unique_lock<shared_timed_mutex> lock(mutex, defer_lock);
	lock.lock();

	it = GM_map.find(mensagem.grupo);
	if (it == GM_map.end())
	{
		GMServer *new_GM = new GMServer(GCS, mensagem.grupo);
		GM_map.insert(pair<string, GMServer *>(mensagem.grupo, new_GM));
		it = GM_map.find(mensagem.grupo);
	}
	lock.unlock();

	it->second->WriteMessage(mensagem);
	return;
}
