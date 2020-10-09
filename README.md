# Sisop-2-trabalho-1

git Criado para a implementação do trabalho de sisop 2.

# Requisitos

boost:
```
sudo apt-get install libboost-all-dev
```
ncurses:
```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

# Modo de uso

```
make
./server <port>
./app <username> <groupname> <server_ip_address> <port>
```




## end to end, mensagem sentido cliente -> servidor

### Interface
Usuario digita o texto e pressiona enter. 

Interface manda o texto pro gerenciador de mensagens do cliente.

### Gerenciador de Mensagens (cliente)
Gerenciador de mensagens monta a struct da mensagem, a struct vai ter campos como nome, grupo, texto e outra coisas auxiliar.

GM manda pro gerenciador de comunicacao do cliente.

### Gerenciador de comunicação (cliente)
GC serializa a struct da mensagem para bytes(chars) e envia para o servidor em pacotes.

A serializacao é feita por uma biblioteca do g++

### Gerenciador de comunicação (server)
Recebe os pacotes dos clientes e deserializa para a struct de mensagem(a mesma do cliente)

Passa a mensagem para o gerenciador de grupos.

### Gerenciador de grupos
Apenas um no servidor, vai criar um gerenciador de mensagens pra cada grupo e vai manter um dicionario de nome do grupo pra GM.

Recebe a mensagem cria um GM pro novo grupo ou manda pro gerenciador de mensagem responsavel por aquele grupo.

### Gerenciador de mensagens (server)
Um GM e um arquivo pra cada grupo, basicamente um monitor de leitores e escritores e um arquivo, que nem visto em aula.

Quando recebe a mensagem, serializa a struct pra alguma forma de melhor armezanar no arquivo.

Escreve uma nova linha do arquivo, com a nova mensagem
