//
// Created by admin on 2023/11/15.
//

#ifndef MYTCP_H
#define MYTCP_H

#include <string>

class MyTcp {
public:
    MyTcp();

    MyTcp(const MyTcp &t);

    MyTcp(const MyTcp &&t);

    MyTcp &operator=(const MyTcp &t);

    int createSocket();

    bool bind(unsigned short port);

    MyTcp accept();

    void close();

    int recvMsg(char *buf, int bufsize);

    int sendMsg(const char *buf, int sendsize);

    bool connect(const char *ip, unsigned short port,int timeout = 1000);

    bool setBlock(bool isBlock);

    bool setOption();

    int static acceptFd(int fd);


    virtual ~MyTcp();

    int sock = 0;
    unsigned short port = 0;
    std::string ip;
};

#endif

