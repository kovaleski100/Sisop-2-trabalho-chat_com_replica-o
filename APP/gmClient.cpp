#include "gmClient.h"

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
    interface->DisplayText(textToDisplay);
}
