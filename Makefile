CC=g++
APPMAIN=APP/main.cpp
GMCLIENT=APP/gmClient.cpp
INTERFACE=APP/interface.cpp

.PHONY: app

app: ${APPMAIN} ${GMCLIENT} ${INTERFACE}
	$(CC) -std=c++14 -pthread ${APPMAIN} ${GMCLIENT} ${INTERFACE} -lcurses -lboost_system -o app


clean:
	rm app