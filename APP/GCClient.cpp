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
    Mensagem m1;
    m1.grupo = group;
    m1.usuario = user;
    m1.texto = "register dummy text";
    Send_server(m1);
}


void GCClient::listen_server()
{
    int n;
    char buffer[256];
    while (1)
    {
        n = read(server_socket, buffer, 256);
        if (n <= 0)
            break;

        Mensagem m1 = build_Mensagem(buffer);
        gmc->DisplayMessage(m1);
        bzero(buffer, 256);
    }
    close(server_socket);
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
