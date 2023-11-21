#include <signal.h>
#include "MyHttpServer.h"
#include "EpollServer.h"

int main(int argc, char *argv[]) {
    //对端已经关闭了，服务端如果还在发送数据给客户段，则内核会发送SIGPIPE信号给进程，导致进程崩溃，这里忽略此信号
    signal(SIGPIPE, SIG_IGN);
    unsigned short port = 8000;
//    unsigned short port1 = 8888;
//    unsigned short port2 = 8889;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    MyHttpServer server;
    server.Start(port);
//    server.Start1(port);
//    EpollServer epoll;
//    MyHttpServer server1;
//    server1.initSocket(port1);
//    MyHttpServer server2;
//    server2.initSocket(port2);
//    epoll.addServer(server1);
//    epoll.addServer(server2);
//    epoll.Start();
    return 0;
}
