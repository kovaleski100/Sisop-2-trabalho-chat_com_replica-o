#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include "../mensagemStruct.h"

using namespace std;

class GMServer
{
private:
  string grupo;
  string fileName;
  //uma copia da conversa é mantida no arquivo para persistencia e outra no vetor conversa para rapido acesso
  //a consistencia entre as duas é garantida.
  vector<Mensagem> conversa;
  mutable shared_timed_mutex mutex; //mutex for locking access to file and to vector conversa

  bool fileExists();
  Mensagem lineToStruct(string line);
  //buildConversa reconstroi a conversa a partir dos arquivo quando recomecando o server
  bool buildConversa();

public:
  GMServer(string nomeGrupo)
  {
    grupo = nomeGrupo;
    fileName = "./server/conversas/" + nomeGrupo + ".txt";
    if (fileExists())
    {
      buildConversa();
    }
    else
    {
      // does not need a mutex because it is on the constructor of the class
      ofstream arquivoGrupo(fileName);
      arquivoGrupo.close();
    }
  };

  vector<Mensagem> ReadLastMessages(int n);
  bool WriteMessage(Mensagem mensagem);
};

vector<Mensagem> GMServer::ReadLastMessages(int n)
{
  //shared lock for readers, lock until the end of the function
  shared_lock<shared_timed_mutex> lock(mutex); 
  int size = conversa.size();
  n = min(n, size);
  vector<Mensagem> mensagens(conversa.end() - n, conversa.end());
  return mensagens;
}

bool GMServer::WriteMessage(Mensagem mensagem)
{
  //unique lock for writers, lock until the end of the function
  unique_lock<shared_timed_mutex> lock(mutex);
  ofstream arquivoGrupo(fileName, ios::app);
  //importante escrever primeiro no arquivo para persistencia
  arquivoGrupo << mensagem.uuid << ";" << mensagem.grupo << ";" << mensagem.usuario << ";" << mensagem.texto << endl;
  conversa.push_back(mensagem);
  arquivoGrupo.close();
}

bool GMServer::fileExists()
{
  // does not need a mutex because it is on the constructor of the class
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

  //buildConversa reconstroi a conversa a partir dos arquivo quando recomecando o server
bool GMServer::buildConversa()
{
  // does not need a mutex because it is on the constructor of the class
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