#include "Connection_client.h"

using namespace std;





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

void client_tcp(string data,string server_ip, int port)
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
	serv_addr.sin_addr.s_addr = inet_addr(server_ip.c_str()); //converte string com ip do servidor para o formato in_addr a ser passado para serv_addr
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
        return;
    
    char buffer[256];
    bzero(buffer,256);

	/* read from the socket */
    n = read(sockfd, buffer, 256);
    if (n < 0)
		printf("ERROR reading from socket\n");

    printf("%s\n",buffer);

	close(sockfd);
}






void configure_keepalive(int switch_ka,float idle,int interval,int count){
    /*
     int switch_ka = 1;	// 1=KeepAlive On, 0=KeepAlive Off. //
        float idle = 0.1;	// Number of idle seconds before sending a KeepAlive probe. //
        int interval = 1;	// How often in seconds to resend an unacked KeepAlive probe. //
        int count = 3;	// How many times to resend a KA probe if previous probe was unacked. //
     
     */
    
    
    
    if (setsockopt(socket_ka, SOL_SOCKET, SO_KEEPALIVE, &switch_ka, sizeof(switch_ka)) < 0)
    {
        /* Error occurred, so output an error message containing:
        __LINE__, socket, SO_KEEPALIVE, switch_ka, errno, etc. */
    }

    if (switch_ka)
    {
        /* Set the number of seconds the connection must be idle before sending a KA probe. */
        if (setsockopt(socket_ka, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle)) < 0)
        {
        /* Error occurred, so output an error message containing:
        __LINE__, socket_ka, TCP_KEEPIDLE, idle, errno, etc. */
        }

        /* Set how often in seconds to resend an unacked KA probe. */
        if (setsockopt(socket_ka, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)) < 0)
        {
        /* Error occurred, so output an error message containing:
        __LINE__, socket_ka, TCP_KEEPINTVL, interval, errno, etc. */
        }

        /* Set how many times to resend a KA probe if previous probe was unacked. */
        if (setsockopt(socket_ka, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count)) < 0)
        {
        /* Error occurred, so output an error message containing:
        __LINE__, socket_ka, TCP_KEEPCNT, count, errno, etc. */
        }
    }
}

string get_local_ip(){
    struct sockaddr_in local_addr;
    socklen_t addrlen = sizeof(local_addr);
    char myIP[16] = {0};
    
    bzero(&local_addr, sizeof(local_addr));
	int len = sizeof(local_addr);
	getsockname(socket_ka, (struct sockaddr *) &local_addr, &addrlen);
	inet_ntop(AF_INET, &local_addr.sin_addr, myIP, sizeof(myIP));
    
    string temp;
    temp.append(myIP);
    return temp;
    
}


