#include "gmClient.h"


void serialize_data(Mensagem* m1,string data){
    
    string local_ip = get_local_ip();
    data.append(local_ip);
    data.append("/");
    data.append(m1->usuario);
    data.append("/");
    data.append(m1->grupo);
    data.append("/");
    data.append(m1->texto);
    data.append("/");
    
    
}
void deserialize_data(string data,Mensagem* m1){
    return;
    
}

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
    string data;
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
