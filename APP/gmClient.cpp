#include "gmClient.h"

void serialize_data(Mensagem* m1,char *data){
    
}
void deserialize_data(char *data,Mensagem* m1){
    data = 0;
};

Mensagem GMClient::buildMensagem(string text)
{
    Mensagem m1;
    m1.grupo = group;
    m1.usuario = user;
    m1.texto = text;

    return m1;
}

// Send to GCClient
bool GMClient::SendMessage(string text)
{
    Mensagem m1 = buildMensagem(text);
    char data[sizeof(Mensagem)];
    serialize_data(&m1,data);
    client_tcp(data, server_ip,port);

    // Chama uma função do GCClient chamada algo assim,
    // essa função do GCClient vai mandar pro server.
    // duvida: essa função do GCClient precisa lançar uma thread pra mandar pro server?
    //gcClient->SendMessage(m1);
}

bool GMClient::DisplayMessage(Mensagem message)
{
    string textToDisplay;
    if (message.usuario == user)
    {
        message.usuario = "você:";
    }
    else
    {
        message.usuario = "[" + message.usuario + "]";
    }

    textToDisplay = message.usuario + " " + message.texto;

    // Send to interface
    interface->DisplayText(textToDisplay);
}
