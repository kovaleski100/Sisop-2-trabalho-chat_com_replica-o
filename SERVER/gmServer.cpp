#include "gmServer.h"

GMServer::GMServer(GCServer *GCS_, string nomeGrupo)
{
    GCS = GCS_;
    grupo = nomeGrupo;
    fileName = "./SERVER/conversas/" + GCS->Replica_name + "/" + nomeGrupo + ".txt";
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
    GCS->Send_all(mensagem);
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