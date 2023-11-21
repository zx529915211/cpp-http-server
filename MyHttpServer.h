//
// Created by admin on 2023/11/20.
//

#ifndef SOCKET_STUDY_MAKEFILE_MYHTTPSERVER_H
#define SOCKET_STUDY_MAKEFILE_MYHTTPSERVER_H

#include "MyTcp.h"

class MyHttpServer {
public:
    MyHttpServer();

    ~MyHttpServer();

    bool initSocket(unsigned short port);

    bool Start(unsigned short port);

    bool Start1(unsigned short port);

    void Main();

    void Stop();

    MyTcp server;
    bool isexit = false;
};


#endif //SOCKET_STUDY_MAKEFILE_MYHTTPSERVER_H
