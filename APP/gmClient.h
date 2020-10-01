#include <string>
#include <iostream>
#include "../mensagemStruct.h"

using namespace std;

class GMClient
{
private:
    string user;
    string group;
    // Um ponteiro pra uma instancia do GCClient
    // Vai ser usado pra chamar a funcao pra enviar uma mensagem pro server.
    //GCClient * gcClient;

    // ponteiro pra interface, pra chamar funcao que mostra mensagem na tela
    //Interface * interface;

    Mensagem buildMensagem(string text);

public:
    GMClient(string user_, string group_)
    {
        user = user_;
        group = group_;
    };

    bool SendMessage(string text);
    // essa função DisplayMessage vai ser chamada pelo GCClient
    // para mostrar a mensagem na interfarce.
    // GCClient vai ter que ter um ponteiro pra uma instancia de GMClient.
    bool DisplayMessage(Mensagem message);
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
    //interface->DisplayText(text);
}
