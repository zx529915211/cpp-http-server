libxsocket.so:MyTcp.cpp MyTcp.h
	g++ -g $+ -o $@ -fpic -shared -std=c++11 -o libxsocket.so
	sudo cp *.so /usr/lib

tcpserver:tcpserver.cpp libxsocket.so
	g++ -g $+ -o $@ -I./ -std=c++11 -lpthread -lxsocket -L./

tcpclient:tcpclient.cpp libxsocket.so
	g++ -g $+ -o $@ -I./ -std=c++11 -lxsocket -L./

httpserver:httpserver.cpp  MyHttpServer.cpp MyHttpConnection.cpp MyHttpResponse.cpp EpollServer.cpp libxsocket.so
	g++ -g $+ -o $@ -I./ -std=c++11 -lpthread -lxsocket -L./

clean:
	sudo rm *.so