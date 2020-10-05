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

using namespace std;

void client_tcp(char* data,string server_ip, int port);





void open_connection(string user, string groupname, string server_ip, int port)
{
    try
    {
        //client_tcp(user,groupname, server_ip, port);
    }
    catch(const std::exception& e)
    {
        cout << "sorry, erro 404"<<endl;
    }
    
}

void client_tcp(char* data,string server_ip, int port)
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    //char buffer[256];
    /*if (argc < 2) {
		fprintf(stderr,"usage %s hostname\n", argv[0]);
		exit(0);
    }
    
	server = gethostbyname(argv[1]);
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("ERROR opening socket\n");
        return;
    }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);


	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        printf("ERROR connecting\n");
        return;
    }

   // printf("Enter the message: ");
   // bzero(buffer, 256);
  // fgets(buffer, 256, stdin);

	/* write in the socket */
	n = write(sockfd, data, strlen(data));
    if (n < 0)
		printf("ERROR writing to socket\n");
    
    char buffer[256];
    bzero(buffer,256);

	/* read from the socket */
    n = read(sockfd, buffer, 256);
    if (n < 0)
		printf("ERROR reading from socket\n");

    printf("%s\n",buffer);

	close(sockfd);
}

