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
	string username;
	set<std::string> ip;
}typedef dispositivo DISP;

std::map<std::string, std::vector<dispositivo>> gruop

void tolken(char* buffer)
{
    DISP device = NULL, temp = NULL;

    ///"iṕ/usuario/grupo/texto"
    string iṕ;
    string groupname;
    string text;

    strcpy(ip,strtok(buffer, "/"));
    strcpy(device->username,strtok(buffer, "/"));
    strcpy(gruopname,strtok(buffer, "/"));
    strcpy(text,strtok(buffer, "/"));


    device->ip.add(ip)
    grupo[gruopname] = device;

}


int create_socket(int port, Mensagem message)
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

	while(1)
	{
        listen(sockfd, 5);

        clilen = sizeof(struct sockaddr_in);

		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
			printf("ERROR on accept");

		bzero(buffer, 256);

		/* read from the socket */
		n = read(newsockfd, buffer, 256);

		if(n < 0)
			printf("ERROR reading from socket");
		printf("Here is the message: %s\n", buffer);

        tolken(buffer, group);

		/* write in the socket */
		if(new_message && keep_alive)
			n = write(newsockfd,"I got your message", strlen());
			if (n < 0)
				printf("ERROR writing to socket");
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}