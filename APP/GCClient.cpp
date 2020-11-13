#include "GCClient.h"

GCClient::GCClient(string server_adress, int port, string user, string group)
{
   connect_server(server_adress, port);
    register_itself(user, group);
    /* read from the socket */
    thread t(&GCClient::listen_server, this);
    t.detach();
}

GCClient::~GCClient()
{
    close(server_socket);
}

int GCClient::resv_port(){
	int port;
	int sockfd;
	struct sockaddr_in serv_addr;
	socklen_t addrlen = sizeof(serv_addr);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < -1)
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(0);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");
	
	getsockname(sockfd,(struct sockaddr *)&serv_addr, &addrlen);
	port = ntohs(serv_addr.sin_port);
	close(sockfd);
	return port;
}

void GCClient::connect_server(string server_adress, int port)
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

    server_socket = sockfd;
}

void GCClient::register_itself(string user, string group){
    string port;
	other_port = resv_port();
	port = to_string(other_port);
	string texto = "app/" + group + "/" + user + "/" + port;
    /* write in the socket */
    const char *buff = texto.c_str();
    int size = strlen(buff);
    int n = write(server_socket, buff, size);
    if (n <= 0)
        printf("ERROR writing to socket\n");
}


void GCClient::listen_server()
{
    int n;
    char buffer[256];
    while (1)
    {
        n = read(server_socket, buffer, 256);
        if (n <=0)
            break;
		
        Mensagem m1 = build_Mensagem(buffer);
        gmc->DisplayMessage(m1);
        bzero(buffer, 256);
    }
    close(server_socket);
	handle_new_conections(other_port);
}

Mensagem GCClient::build_Mensagem(char *buffer)
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

void GCClient::Send_server(Mensagem mensagem)
{
    string texto = mensagem.grupo + "/" + mensagem.usuario + "/" + mensagem.texto;
    /* write in the socket */
    const char *buff = texto.c_str();
    int size = strlen(buff);
    int n = write(server_socket, buff, size);
    if (n <= 0)
        printf("ERROR writing to socket\n");
}

void GCClient::handle_new_conections(int port)
{
	int newsocket = create_socket(port);
	server_socket = newsocket;
	register_itself("andrews","teste12");
	thread t(&GCClient::listen_server,this);
	t.detach();
	return;
	
}

int GCClient::create_socket(int port)
{
	int sockfd,newsockfd, n;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < -1)
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");

	while (1)
	{
		if(listen(sockfd,1) == 0){
		//cout<< "Conectou"<<endl;
		break;
		}
	}
	
	clilen = sizeof(struct sockaddr_in);
	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
		printf("ERROR on accept");
	
	bzero(buffer, 256);
	
	return newsockfd;
}