#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../mensagemStruct.h"

using namespace std;

class GMServer
{
private:
  string grupo;
  string fileName;
  vector<mensagem> conversa;

public:
  GMServer(string nomeGrupo)
  {
    grupo = nomeGrupo;
    fileName = nomeGrupo + ".txt";
    ofstream arquivoGrupo(fileName);
    arquivoGrupo.close();
  };

  vector<mensagem> ReadLastMessages(int n);
  bool WriteMessage(mensagem mensagem);
};

vector<mensagem> GMServer::ReadLastMessages(int n)
{
  int size = conversa.size();
  n = min(n, size);
  vector<mensagem> mensagens(conversa.end() - n, conversa.end());
  return mensagens;
}

bool GMServer::WriteMessage(mensagem mensagem)
{
  ofstream arquivoGrupo(fileName, ios::app);
  arquivoGrupo << mensagem.uuid << ";" << mensagem.grupo << ";" << mensagem.usuario << ";" << mensagem.texto << endl;
  conversa.push_back(mensagem);
  arquivoGrupo.close();
}