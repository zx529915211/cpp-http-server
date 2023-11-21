//
// Created by admin on 2023/11/21.
//

#ifndef SOCKET_STUDY_MAKEFILE_EPOLLSERVER_H
#define SOCKET_STUDY_MAKEFILE_EPOLLSERVER_H
#include <set>
class MyHttpServer;
class EpollServer {
public:
    EpollServer();
    ~EpollServer();
    void Start();
    void addServer(MyHttpServer &server);

private:
    std::set<int> listenFd;
    std::set<int> connectFd;
    int epfd;
};


#endif //SOCKET_STUDY_MAKEFILE_EPOLLSERVER_H
