CC=g++
APPMAIN=APP/main.cpp
GMCLIENT=APP/gmClient.cpp
INTERFACE=APP/interface.cpp
GCCLIENT=APP/GCClient.cpp

SERVERMAIN=SERVER/main.cpp
GCSERVER=SERVER/GCServer.cpp
GGSERVER=SERVER/ggServer.cpp
GMSERVER=SERVER/gmServer.cpp

.PHONY: app server

all: app server

app: ${APPMAIN} ${GMCLIENT} ${INTERFACE} ${GCCLIENT}
	$(CC) -std=c++14 -pthread ${APPMAIN} ${GCCLIENT} ${GMCLIENT} ${INTERFACE} -lcurses -lboost_system -o app

server: ${SERVERMAIN} ${GCSERVER} ${GGSERVER} ${GMSERVER}
	$(CC) -std=c++14 -pthread ${SERVERMAIN} ${GMSERVER} ${GGSERVER}  ${GCSERVER} -lboost_system -o server

clean:
	rm -f app server