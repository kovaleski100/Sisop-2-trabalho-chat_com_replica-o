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
  vector<Mensagem> conversa;

  bool fileExists();
  Mensagem lineToStruct(string line);
  bool buildConversa();

public:
  GMServer(string nomeGrupo)
  {
    grupo = nomeGrupo;
    fileName = nomeGrupo + ".txt";
    if (fileExists())
    {
      buildConversa();
    }
    else
    {
      ofstream arquivoGrupo(fileName);
      arquivoGrupo.close();
    }
  };

  vector<Mensagem> ReadLastMessages(int n);
  bool WriteMessage(Mensagem mensagem);
};

vector<Mensagem> GMServer::ReadLastMessages(int n)
{
  int size = conversa.size();
  n = min(n, size);
  vector<Mensagem> mensagens(conversa.end() - n, conversa.end());
  return mensagens;
}

bool GMServer::WriteMessage(Mensagem mensagem)
{
  ofstream arquivoGrupo(fileName, ios::app);
  arquivoGrupo << mensagem.uuid << ";" << mensagem.grupo << ";" << mensagem.usuario << ";" << mensagem.texto << endl;
  conversa.push_back(mensagem);
  arquivoGrupo.close();
}

bool GMServer::fileExists()
{
  ifstream f(fileName);
  bool exist = f.good();
  f.close();
  return exist;
}

Mensagem GMServer::lineToStruct(string line)
{
  string delimiter = ";";

  // vai copiando os campos e apagando da string
  string uuid = line.substr(0, line.find(delimiter));
  line.erase(0, line.find(delimiter) + delimiter.length());

  string grupo = line.substr(0, line.find(delimiter));
  line.erase(0, line.find(delimiter) + delimiter.length());

  string user = line.substr(0, line.find(delimiter));
  line.erase(0, line.find(delimiter) + delimiter.length());

  string text = line; //pega o resto

  return Mensagem(uuid, grupo, user, text);
};

bool GMServer::buildConversa()
{
  ifstream f(fileName);
  while (f.good())
  {
    string line;
    getline(f, line);
    if (line == "")
    {
      continue;
    }
    
    conversa.push_back(lineToStruct(line));
  }
  f.close();
};