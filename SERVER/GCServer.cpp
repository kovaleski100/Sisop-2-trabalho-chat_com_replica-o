#include "GCServer.h"
#include "ggServer.h"

GCServer::GCServer(GGServer *ggs_, int port)
{
	main_replica = true;
	self_port = port;
	Replica_name = "replica_" + to_string(port);
	ggs = ggs_;
	ggs_->set_GCS(this);
	int socket = create_socket(port);
	handle_new_conections(socket);
}

GCServer::~GCServer()
{
}

Mensagem GCServer::build_Mensagem(char *buffer)
{
	string buff(buffer);
	string delimiter = "/";

	string grupo = buff.substr(0, buff.find(delimiter));
	buff.erase(0, buff.find(delimiter) + delimiter.length());

	string user = buff.substr(0, buff.find(delimiter));
	buff.erase(0, buff.find(delimiter) + delimiter.length());

	string text = buff.substr(0, buff.find(delimiter));
	buff.erase(0, buff.find(delimiter) + delimiter.length());

	Mensagem m1;
	m1.grupo = grupo;
	m1.usuario = user;
	m1.texto = text;

	return m1;
}

void GCServer::Send_all(Mensagem message)
{
	shared_lock<shared_timed_mutex> lock1(main_replica_mutex);
	if (!main_replica)
		return;

	string text = message.grupo + "/" + message.usuario + "/" + message.texto;
	send_all_backups(text);

	std::map<std::string, std::vector<Dispositivo>>::iterator it;

	shared_lock<shared_timed_mutex> lock2(group_map_mutex);
	it = group_map.find(message.grupo);

	for (auto i : it->second)
	{
		int n = write(i.socket, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket app\n");
	}
}

void GCServer::register_new_backup(int socket, int port)
{
	cout << "Registrando novo backup" << endl;

	Backup new_backup;
	new_backup.socket = socket;
	new_backup.port = port;
	unique_lock<shared_timed_mutex> lock(backup_vector_mutex);
	backup_vector.push_back(new_backup);
	if (backup_vector.size() == 1)
		return;

	string text;
	// send new next to the previous last on the list
	text = "next/" + to_string(port);
	while (text.length() < 256)
	{
		text += "/";
	}
	int n = write(backup_vector[backup_vector.size() - 2].socket, text.c_str(), strlen(text.c_str()));
	if (n <= 0)
		printf("ERROR writing to socket backup 1\n");
	// send new next to the new backup
	text = "next/" + to_string(backup_vector[0].port);
	while (text.length() < 256)
	{
		text += "/";
	}
	n = write(socket, text.c_str(), strlen(text.c_str()));
	if (n <= 0)
		printf("ERROR writing to socket backup 2\n");
}

bool GCServer::kill_app_if_to_many_devices(string username, int socket)
{
	int num_devices = 0;
	std::map<std::string, std::vector<Dispositivo>>::iterator it;
	shared_lock<shared_timed_mutex> lock(group_map_mutex, defer_lock);
	lock.lock();
	for (it = group_map.begin(); it != group_map.end(); it++)
	{
		for (auto &disp : it->second)
		{
			if (disp.username == username)
			{
				num_devices++;
			}
		}
		if (num_devices >= 2)
		{
			// ja possui dois dispositivos conectados, nao precisa procurar mais
			break;
		}
	}
	lock.unlock();
	if (num_devices >= 2)
	{
		// kill new app
		string text = "kill/";
		int n = write(socket, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket app to kill it\n");
		return true;
	}

	return false;
}

void GCServer::register_new_connection(int newsocket)
{
	int n;
	char buffer[256];
	bzero(buffer, 256);
	n = read(newsocket, buffer, 256);
	if (n <= 0)
	{
		cout << "ERROR registrando nova conexao" << endl;
		return;
	}

	string buff(buffer);
	string delimiter = "/";

	string type = buff.substr(0, buff.find(delimiter));
	buff.erase(0, buff.find(delimiter) + delimiter.length());

	if (type == "app")
	{
		cout << "Registrando novo app" << endl;
		//  app/grupo/usuario/porta
		string group = buff.substr(0, buff.find(delimiter));
		buff.erase(0, buff.find(delimiter) + delimiter.length());

		string user = buff.substr(0, buff.find(delimiter));
		buff.erase(0, buff.find(delimiter) + delimiter.length());
		string port = buff;
		if (kill_app_if_to_many_devices(user, newsocket))
		{
			close(newsocket);
			cout << "To many devices per user, killing it!" << endl;
			return;
		}

		Dispositivo device;
		device.socket = newsocket;
		device.username = user;
		device.app_reconnection_port = port;
		cout<<"COMECO"<<endl;
		unique_lock<shared_timed_mutex> lock(group_map_mutex, defer_lock);
		lock.lock();
		if(group_map.find(group) == group_map.end()){
			cout<<"GRUPO NOVO"<< endl;
			group_map[group].push_back(device);
			lock.unlock();
		}
		
		else{
			cout<<"GRUPO EXISTENTE"<< endl;
			group_map[group].push_back(device);
			lock.unlock();
			send_last_messages(ggs->ReadMessage(group),user);
		}
		
		const string tmp = boost::lexical_cast<string>(device.uuid);
		string text = "app/" + group + "/" + user + "/" + port + "/" + tmp;
		while (text.length() < 256)
		{
			text += "/";
		}
		send_all_backups(text);
		thread t(&GCServer::listen_app, this, newsocket, device.uuid, group);
		t.detach();
		return;
		
	}
	else if (type == "backup")
	{
		// backup/port
		string backup_port = buff;
		register_new_backup(newsocket, stoi(backup_port));

		std::map<std::string, std::vector<Dispositivo>>::iterator it;

		shared_lock<shared_timed_mutex> lock(group_map_mutex, defer_lock);
		lock.lock();
		for (it = group_map.begin(); it != group_map.end(); it++)
		{
			for (auto &disp : it->second)
			{
				disp.username;
				disp.app_reconnection_port;
				string tmp = boost::lexical_cast<string>(disp.uuid);
				string M = "app/" + it->first + "/" + disp.username + "/" + disp.app_reconnection_port + "/" + tmp;
				while (M.length() < 256)
				{
					M += "/";
				}

				int n = write(newsocket, M.c_str(), strlen(M.c_str()));
				if (n <= 0)
					printf("ERROR writing to transfer apps to backup\n");
			}
			vector<Mensagem> lastM = ggs->ReadMessage(it->first);

			for (auto &message : lastM)
			{
				string text = message.grupo + "/" + message.usuario + "/" + message.texto;
				while (text.length() < 256)
				{
					text += "/";
				}

				int n = write(newsocket, text.c_str(), strlen(text.c_str()));
				if (n <= 0)
					printf("ERROR writing to transfer message to backup\n");
			}
		}
		lock.unlock();
	}
	else if (type == "election")
	{
		// election/id
		string id = buff;
		handle_election(stoi(id));
		close(newsocket);
	}
	else if (type == "elected")
	{
		// elected/port
		shared_lock<shared_timed_mutex> lock1(main_replica_mutex);
		if (main_replica)
			return;

		cout << "Novo lider eleito! Repassando as boas novas!" << endl;
		string new_main_port = buff;
		shared_lock<shared_timed_mutex> lock4(next_port_ring_mutex, defer_lock);
		lock4.lock();
		int sock = connect_to_port("127.0.0.1", next_port_ring_election);
		lock4.unlock();
		if (sock == -1)
			return;
		string text = "elected/" + new_main_port;
		int n = write(sock, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket backup on election repassing elected\n");
		close(sock);
		unique_lock<shared_timed_mutex> lock2(election_mutex, defer_lock);
		lock2.lock();
		participant = false;
		lock2.unlock();
		unique_lock<shared_timed_mutex> lock5(next_port_ring_mutex, defer_lock);
		lock5.lock();
		next_port_ring_election = 0;
		lock5.unlock();
		unique_lock<shared_timed_mutex> lock6(group_map_mutex, defer_lock);
		lock6.lock();
		group_map.clear();
		lock6.unlock();
		cout << "Conectando com novo lider!" << endl;
		sock = connect_to_port("127.0.0.1", stoi(new_main_port));
		if (sock == -1)
			return;

		unique_lock<shared_timed_mutex> lock3(main_socket_mutex, defer_lock);
		lock3.lock();
		main_socket = sock;
		lock3.unlock();

		register_itself(self_port);
		thread t2(&GCServer::listen_main_server, this);
		t2.detach();
	}
	else
	{
		cout << "error registering new connection: type not foud!" << endl;
		close(newsocket);
	}
}

void GCServer::listen_app(int newsockfd, boost::uuids::uuid uuid, string group)
{
	int n;
	char buffer[256];

	// keep listening app
	bzero(buffer, 256);
	while (1)
	{
		n = read(newsockfd, buffer, 256);
		if (n <= 0)
			break;
		cout << "mensagem recebida!" << endl;
		Mensagem m1 = build_Mensagem(buffer);
		ggs->WriteMessage(m1);
		bzero(buffer, 256);
	}
	close(newsockfd);

	remove_device(uuid, group);
}

void GCServer::remove_device(boost::uuids::uuid uuid, string group)
{
	// remove the device from the group that matches the uuid
	unique_lock<shared_timed_mutex> lock1(group_map_mutex, defer_lock);
	lock1.lock();
	group_map[group].erase(
		std::remove_if(group_map[group].begin(), group_map[group].end(), [&](Dispositivo const &disp) {
			if (disp.uuid == uuid)
			{
				cout << "Device desconectado" << endl;
				return true;
			}
			return false;
		}),
		group_map[group].end());
	lock1.unlock();

	shared_lock<shared_timed_mutex> lock2(main_replica_mutex);
	if (main_replica)
	{
		string tmp = boost::lexical_cast<string>(uuid);
		send_all_backups("kill/" + group + "/" + tmp);
	}
}

int GCServer::create_socket(int port)
{
	int sockfd;

	struct sockaddr_in serv_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < -1)
		printf("ERROR opening socket\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding\n");

	listen(sockfd, 5);

	return sockfd;
}

void GCServer::handle_new_conections(int sockfd)
{
	int newsockfd;
	socklen_t clilen;
	clilen = sizeof(struct sockaddr_in);
	struct sockaddr_in cli_addr;

	while (1)
	{
		if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1)
			printf("ERROR on accept\n");

		thread t(&GCServer::register_new_connection, this, newsockfd);
		t.detach();
	}
	close(sockfd);
}

void GCServer::send_all_backups(string text)
{
	shared_lock<shared_timed_mutex> lock(backup_vector_mutex);
	for (auto const &x : backup_vector)
	{
		int backup_socket = x.socket;
		int n = write(backup_socket, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket backup\n");
	}
}

void GCServer::send_last_messages(vector<Mensagem> last_messages,string user)
{
	std::map<std::string, std::vector<Dispositivo>>::iterator it;
	
	shared_lock<shared_timed_mutex> lock2(group_map_mutex);
	it = group_map.find(last_messages[0].grupo);

	for(auto i : it->second)
	{
		if(i.username == user)
		{
			for(auto j : last_messages)
			{
				string text = j.grupo + "/" + j.usuario + "/" + j.texto + "§";
					
				while (text.length() < 256)
				{
					text += "";
				}
				
				int n = write(i.socket, text.c_str(), strlen(text.c_str()));
				if (n <= 0)
					printf("ERROR writing to socket app\n");
			}
			break;
		}
	
		
	}
	return;
}

// Backup replica ----------------

GCServer::GCServer(GGServer *ggs_, int port, int main_port)
{
	main_replica = false;
	Replica_name = "replica_" + to_string(port);
	next_port_ring_election = 0;
	ggs = ggs_;
	ggs_->set_GCS(this);
	int socket = create_socket(port);
	thread t1(&GCServer::handle_new_conections, this, socket);
	int sock = connect_to_port("127.0.0.1", main_port);
	if (sock == -1)
		return;
	main_socket = sock;
	register_itself(port);
	thread t2(&GCServer::listen_main_server, this);
	election_id = port;
	self_port = port;

	t1.join();
	t2.join();
}

int GCServer::connect_to_port(string server_adress, int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	server = gethostbyname(server_adress.c_str());
	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("ERROR opening socket\n");
		return -1;
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		  (char *)&serv_addr.sin_addr.s_addr,
		  server->h_length);
	serv_addr.sin_port = htons(port);

	bzero(&(serv_addr.sin_zero), 8);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("ERROR connecting\n");
		return -1;
	}
	return sockfd;
}

// usado para replicacao passiva
void GCServer::listen_main_server()
{
	int n;
	char buffer[256];
	bzero(buffer, 256);
	while (1)
	{
		shared_lock<shared_timed_mutex> lock(main_socket_mutex, defer_lock);
		lock.lock();
		n = read(main_socket, buffer, 256);
		lock.unlock();
		if (n <= 0)
			break;

		//check if message or new backup
		string buff(buffer);
		string delimiter = "/";
		string type = buff.substr(0, buff.find(delimiter));
		if (type == "app")
		{
			// app/group/user/port/uuid
			cout << "Registrando novo app - backup" << endl;
			backup_register_app(buff);
		}
		else if (type == "next")
		{
			// next/port
			// Proximo backup a direita no anel das eleicoes
			cout << "Novo backup a direita - backup" << endl;
			// erase "next/"
			buff.erase(0, buff.find(delimiter) + delimiter.length());

			string next_port = buff.substr(0, buff.find(delimiter));
			unique_lock<shared_timed_mutex> lock(next_port_ring_mutex, defer_lock);
			lock.lock();
			next_port_ring_election = stoi(next_port);
			lock.unlock();
		}
		else if (type == "kill")
		{
			// kill/group/uuid
			// erase "kill/"
			buff.erase(0, buff.find(delimiter) + delimiter.length());
			// get rest
			string group = buff.substr(0, buff.find(delimiter));
			buff.erase(0, buff.find(delimiter) + delimiter.length());
			string tmp = buff.substr(0, buff.find(delimiter));
			boost::uuids::string_generator gen;
			boost::uuids::uuid uuid = gen(tmp);
			remove_device(uuid, group);
		}
		else
		{
			cout << "Mensagem recebida - backup" << endl;
			Mensagem m1 = build_Mensagem(buffer);
			ggs->WriteMessage(m1);
		}
		bzero(buffer, 256);
	}
	shared_lock<shared_timed_mutex> lock(main_socket_mutex, defer_lock);
	lock.lock();
	close(main_socket);
	lock.unlock();
	start_election();
}

void GCServer::register_itself(int port)
{
	string texto = "backup/" + to_string(port);
	/* write in the socket */
	const char *buff = texto.c_str();
	int size = strlen(buff);
	shared_lock<shared_timed_mutex> lock(main_socket_mutex);
	int n = write(main_socket, buff, size);
	if (n <= 0)
		printf("ERROR writing to socket\n");
}

void GCServer::backup_register_app(string app_info)
{
	//  app/grupo/usuario/uuid
	string delimiter = "/";
	// erase "app/"
	app_info.erase(0, app_info.find(delimiter) + delimiter.length());

	string group = app_info.substr(0, app_info.find(delimiter));
	app_info.erase(0, app_info.find(delimiter) + delimiter.length());
	string user = app_info.substr(0, app_info.find(delimiter));
	app_info.erase(0, app_info.find(delimiter) + delimiter.length());
	string port = app_info.substr(0, app_info.find(delimiter));
	app_info.erase(0, app_info.find(delimiter) + delimiter.length());
	string uuid = app_info.substr(0, app_info.find(delimiter));
	Dispositivo device;
	device.username = user;
	device.app_reconnection_port = port;
	boost::uuids::string_generator gen;
	device.uuid = gen(uuid);
	unique_lock<shared_timed_mutex> lock(group_map_mutex);
	group_map[group].push_back(device);
}

void GCServer::start_election()
{
	unique_lock<shared_timed_mutex> lock1(election_mutex);
	if (participant == true)
	{
		// ja participei
		return;
	}

	cout << "Iniciei a eleicao!" << endl;
	shared_lock<shared_timed_mutex> lock(next_port_ring_mutex);
	if (next_port_ring_election == 0)
	{
		cout << "Sou o unico backup, sou o lider agora!" << endl;
		unique_lock<shared_timed_mutex> lock2(main_replica_mutex);
		main_replica = true;
		reconnect_to_all_apps();
		return;
	}
	participant = true;
	int sock = connect_to_port("127.0.0.1", next_port_ring_election);
	if (sock == -1)
		return;
	string text = "election/" + to_string(election_id);
	int n = write(sock, text.c_str(), strlen(text.c_str()));
	if (n <= 0)
		printf("ERROR writing to socket backup on election\n");
	close(sock);
}

void GCServer::handle_election(int id_previous)
{
	unique_lock<shared_timed_mutex> lock(election_mutex);

	if (id_previous > election_id)
	{
		// repassa o id do anterior
		cout << "Repassando o id anterior" << endl;
		shared_lock<shared_timed_mutex> lock(next_port_ring_mutex);

		int sock = connect_to_port("127.0.0.1", next_port_ring_election);
		if (sock == -1)
			return;
		string text = "election/" + to_string(id_previous);
		int n = write(sock, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket backup on election repassing previous id\n");
		close(sock);
		participant = true;
	}
	else if (id_previous < election_id)
	{
		if (participant == false)
		{
			// repassando o meu id
			cout << "Repassando o meu id" << endl;
			shared_lock<shared_timed_mutex> lock(next_port_ring_mutex);

			int sock = connect_to_port("127.0.0.1", next_port_ring_election);
			if (sock == -1)
				return;
			string text = "election/" + to_string(election_id);
			int n = write(sock, text.c_str(), strlen(text.c_str()));
			if (n <= 0)
				printf("ERROR writing to socket backup on election repassing my id\n");
			close(sock);
			participant = true;
		}
	}
	else
	{
		// Recebi meu id, fui eleito
		// mando um "elected" com minha porta
		cout << "Fui eleito!" << endl;
		main_replica = true;
		shared_lock<shared_timed_mutex> lock(next_port_ring_mutex);

		int sock = connect_to_port("127.0.0.1", next_port_ring_election);
		if (sock == -1)
			return;
		string text = "elected/" + to_string(self_port);
		int n = write(sock, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket backup on election repassing my id\n");
		close(sock);
		reconnect_to_all_apps();
	}
}

void GCServer::reconnect_to_all_apps()
{
	cout << "Reconnecting to all apps" << endl;
	std::map<std::string, std::vector<Dispositivo>>::iterator it;
	unique_lock<shared_timed_mutex> lock(group_map_mutex);
	for (it = group_map.begin(); it != group_map.end(); it++)
	{
		for (auto &disp : it->second)
		{
			int newsocket = connect_to_port("127.0.0.1", stoi(disp.app_reconnection_port));
			if (newsocket == -1)
			{
				cout << "error reconnecting to " << disp.username << " on port " << disp.app_reconnection_port << endl;
				continue;
			}
			disp.socket = newsocket;
			thread t(&GCServer::listen_app, this, newsocket, disp.uuid, it->first);
			t.detach();
		}
	}
}
