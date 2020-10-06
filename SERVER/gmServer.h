#pragma once
#ifndef GMSERVER_H
#define GMSERVER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include "ggServer.h"

using namespace std;

//GMServer Gerenciador De Mensagens Do Server
//sua função é gerenciar um conversa para um grupo, um gmserver por grupo.
//uma copia da conversa é mantida em um arquivo para persistencia e outra no vetor conversa para rapido acesso
//a consistencia entre as duas é garantida.
//possui uma função para escrever uma mensagem na conversa e outra para ler as ultimas N mensagens,
//mutex com locks para readers e writers.
//Ao ser criado, caso já exista o arquivo do grupo, reconstroi a conversa.
//Guarda o arquivo do grupo em server/conversas/<nome_do_grupo>.txt
//guarda em plain text para facilitar debugacao

class GMServer
{
private:
  string grupo;
  string fileName;
  vector<Mensagem> conversa;
  GCServer *GCS;
  mutable shared_timed_mutex mutex; //mutex for locking access to file and to vector conversa

  bool fileExists();
  Mensagem lineToStruct(string line);
  //buildConversa reconstroi a conversa a partir dos arquivo quando recomecando o server
  bool buildConversa();

public:
  GMServer(GCServer *GCS_, string nomeGrupo);

  vector<Mensagem> ReadLastMessages(int n);
  bool WriteMessage(Mensagem mensagem);
};

#endif