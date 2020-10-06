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
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <netinet/in.h>	  
#include <netinet/tcp.h>  

int socket_ka;
void configure_keepalive(int switch_ka,float idle,int interval,int count);
void client_tcp(string data,string server_ip, int port);
string get_local_ip();
