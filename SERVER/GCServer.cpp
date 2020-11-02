#include "GCServer.h"
#include "ggServer.h"

GCServer::GCServer(GGServer *ggs_, int port)
{
	main_replica = true;
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

	string text = buff; //pega o resto

	Mensagem m1;
	m1.grupo = grupo;
	m1.usuario = user;
	m1.texto = text;

	return m1;
}

void GCServer::Send_all(Mensagem message)
{

	if(!main_replica) return;

	send_all_backups(message);

	std::map<std::string, std::vector<Dispositivo>>::iterator it;

	it = group_map.find(message.grupo);
	string text = message.grupo + "/" + message.usuario + "/" + message.texto;

	for (auto i : it->second)
	{
		int n = write(i.socket, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket app");
	}
}

void GCServer::register_new_connection(int newsocket)
{
	int n;
	char buffer[256];
	n = read(newsocket, buffer, 256);
	if (n <= 0)
	{
		cout << "ERROR registrando nova conexao";
		return;
	}

	string buff(buffer);
	string delimiter = "/";

	string type = buff.substr(0, buff.find(delimiter));
	buff.erase(0, buff.find(delimiter) + delimiter.length());

	if (type == "app")
	{
		//  app/grupo/usuario
		string group = buff.substr(0, buff.find(delimiter));
		buff.erase(0, buff.find(delimiter) + delimiter.length());

		string user = buff; //pega o resto
		Dispositivo device;
		device.socket = newsocket;
		device.username = user;
		group_map[group].push_back(device);

		thread t(&GCServer::listen_app, this, newsocket);
		t.detach();
	}
	else if (type == "backup")
	{
		//backup/port
		string backup_port = buff;
		backup_map[newsocket] = stoi(backup_port);
	}
	else
	{
		cout << "error registering new connection: type not foud!" << endl;
	}
}

void GCServer::listen_app(int newsockfd)
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

		Mensagem m1 = build_Mensagem(buffer);
		ggs->WriteMessage(m1);
		bzero(buffer, 256);
	}
	close(newsockfd);
}

int GCServer::create_socket(int port)
{
	int sockfd;

	struct sockaddr_in serv_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < -1)
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");

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
			printf("ERROR on accept");

		thread t(&GCServer::register_new_connection, this, newsockfd);
		t.detach();
	}
	close(sockfd);
}

void GCServer::send_all_backups(Mensagem message){

	string text = message.grupo + "/" + message.usuario + "/" + message.texto;

	for (auto const& x : backup_map)
	{
		int backup_socket = x.first;
		int n = write(backup_socket, text.c_str(), strlen(text.c_str()));
		if (n <= 0)
			printf("ERROR writing to socket backup");
	}
}

// Backup replica ----------------

GCServer::GCServer(GGServer *ggs_, int port, int main_port)
{
	main_replica = false;
	Replica_name = "replica_" + to_string(port);
	ggs = ggs_;
	ggs_->set_GCS(this);
	int socket = create_socket(port);
	thread t1(&GCServer::handle_new_conections, this, socket);
	connect_to_main_server("127.0.0.1", main_port);
	register_itself(port);
	thread t2(&GCServer::listen_main_server, this);

	t1.join();
	t2.join();
}

void GCServer::connect_to_main_server(string server_adress, int port)
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
		return;
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
		return;
	}

	main_socket = sockfd;
}

void GCServer::listen_main_server()
{
	int n;
	char buffer[256];
	while (1)
	{
		n = read(main_socket, buffer, 256);
		if (n <= 0)
			break;

		//check if message or new backup

		Mensagem m1 = build_Mensagem(buffer);
		ggs->WriteMessage(m1);
		bzero(buffer, 256);
	}
	//eleicao()
	close(main_socket);
}

void GCServer::register_itself(int port)
{
	string texto = "backup/" + to_string(port);
	/* write in the socket */
	const char *buff = texto.c_str();
	int size = strlen(buff);
	int n = write(main_socket, buff, size);
	if (n <= 0)
		printf("ERROR writing to socket\n");
}
