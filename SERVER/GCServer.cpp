#include "GCServer.h"
#include "ggServer.h"

GCServer::GCServer(GGServer *ggs_, int port)
{
	main_replica = true;
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

	// if(!main_replica) return;


	//Send_all_backup(message);

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

void GCServer::listen_app(int newsockfd)
{
	int n;
	// register app
	char buffer[256];
	n = read(newsockfd, buffer, 256);
	if (n <= 0)
	{
		cout << "ERROR registrando app";
		return;
	}
	Mensagem reg = build_Mensagem(buffer);
	Dispositivo device;
	device.socket = newsockfd;
	device.username = reg.usuario;
	group_map[reg.grupo].push_back(device);

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

		/* read from the socket */
		// registrar app ou backup

		thread t(&GCServer::listen_app, this, newsockfd);
		t.detach();
	}
	close(sockfd);
}

// Backup replica ----------------

GCServer::GCServer(GGServer *ggs_, int port, int main_port)
{
	main_replica = false;
	ggs = ggs_;
	ggs_->set_GCS(this);
	int socket = create_socket(port);
	thread t(&GCServer::handle_new_conections, this, socket);
	t.detach();
	connect_to_main_server("127.0.0.1", main_port);
	register_itself(port);
	thread t2(&GCServer::listen_main_server, this);
	t2.detach();
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

void GCServer::register_itself(int port){
	//send_main_server()
}
