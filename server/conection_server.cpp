#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ggServer.h"
#include "gmServer.h"
#include "../mensagemStruct.h"

using namespace std;

struct dispositivo
{
	int socket;
	string username;
	set<std::string> ip;
}typedef dispositivo DISP;

std::map<std::string, std::vector<dispositivo>> gruop;

void tolken(char* buffer, int socket)
{	
    DISP device = NULL, temp = NULL;
    ///"ip/usuario/grupo/texto"
    string username;
	string iṕ;
    string groupname;
    string text;

    strcpy(ip,strtok(buffer, "/"));
    strcpy(username,strtok(buffer, "/"));
    strcpy(gruopname,strtok(buffer, "/"));
    strcpy(text,strtok(buffer, "/"));

	device->socket = socket;

    device->ip.add(ip);
    grupo[gruopname] = device;

}

void send_all(Mensagem message)
{
	std::map<std::string, std::vector<dispositivo>>::iterator it;

	it = group.find(message.group);

	for(auto i : it->second)
	{
		String text = message.grupo+"/"+message.usuario+"/"+message.texto;

		n = write(i.socket,text, strlen(text));
		if (n < 0)
			printf("ERROR writing to socket");
	}
}

void read_app(int newsockfd,char* buffer)
{
	while(1)
	{
		n = read(newsockfd, buffer, 256);

		if(n < 0)
			break;
		printf("Here is the message: %s\n", buffer);

    	tolken(buffer, newsockfd);
	}
}

int create_socket(int port)
{
	int keep_alive;
	int new_message;
	int sockfd, newsockfd, n;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		printf("ERROR on binding");

	listen(sockfd, 5);
	while(1)
	{
        clilen = sizeof(struct sockaddr_in);

		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
			printf("ERROR on accept");

		bzero(buffer, 256);



		/* read from the socket */
		thread t(&read_app, newsockfd, buffer);
        t.detach();
		/* write in the socket */
	}
	close(sockfd);
	return 0;
}