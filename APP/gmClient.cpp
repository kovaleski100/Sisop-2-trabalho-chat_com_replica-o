#include "gmClient.h"

GMClient::GMClient(GCClient *gcClient_, string user_, string group_)
{
    user = user_;
    group = group_;
    gcClient = gcClient_;
}
GMClient::~GMClient(){
    delete interface;
}

void GMClient::Start(){
    gcClient->Set_gmc(this);
    gcClient->Start();

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

    // Chama uma função do GCClient chamada algo assim,
    // essa função do GCClient vai mandar pro server.
    gcClient->Send_server(m1);
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
