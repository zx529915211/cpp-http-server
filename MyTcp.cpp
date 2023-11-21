//
// Created by admin on 2023/11/15.
//

#include "MyTcp.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>

MyTcp::MyTcp() {
}

MyTcp::MyTcp(const MyTcp &t) = default;

//移动构造函数
MyTcp::MyTcp(const MyTcp &&t) {
    std::cout << "move consturct!!" << std::endl;
}

MyTcp &MyTcp::operator=(const MyTcp &t) = default;

int MyTcp::createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    return sock;
};

bool MyTcp::setOption()
{
    int report = 1;
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&report,sizeof(report))!=0){
        printf("设置socket选项失败\n");
        return false;
    }
    return true;
}

bool MyTcp::bind(unsigned short port) {
    if (sock <= 0) createSocket();
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = 0;

    int ret = ::bind(sock, (sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error:");
        return false;
    }
    listen(sock, 10);
    return true;
}

MyTcp MyTcp::accept() {
    MyTcp tcp;
    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd = ::accept(sock, (sockaddr *) &client_addr, &len);
    if (client_fd <= 0) {
        tcp.sock = 0;
        return tcp;
    }
    tcp.ip = inet_ntoa(client_addr.sin_addr);
    tcp.port = ntohs(client_addr.sin_port);
    tcp.sock = client_fd;
    printf("ip:%s\n", tcp.ip.c_str());
    printf("port:%d\n", tcp.port);
    return tcp;
}


void MyTcp::close() {
    if (sock <= 0)return;
    ::close(sock);
}

int MyTcp::recvMsg(char *buf, int bufsize) {
    return recv(sock, buf, bufsize, 0);
}

int MyTcp::sendMsg(const char *buf, int sendsize) {
    int sendedSize = 0;//已发送数据长度
    while (sendedSize != sendsize) {
        int len = send(sock, buf + sendedSize, sendsize - sendedSize, 0);
        if (len <= 0)break;
        sendedSize += len;
    }
    return sendedSize;
}

bool MyTcp::connect(const char *ip, unsigned short port, int timeout) {
    if (sock <= 0) {
        createSocket();
    }
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);
    setBlock(false);
    fd_set set;
    if (::connect(sock, (sockaddr *) &saddr, sizeof(saddr)) != 0) {
        FD_ZERO(&set);
        FD_SET(sock, &set);
        timeval tm;
        tm.tv_sec = 0;//秒
        tm.tv_usec = timeout * 1000;//微妙
        if (select(sock + 1, 0, &set, 0, &tm) <= 0) {
            printf("connect timeout or error!\n");
            return false;
        }
    }
    setBlock(true);
    return true;
}

/**
 * 设置非阻塞
 * @param isBlock false非阻塞 true阻塞
 * @return
 */
bool MyTcp::setBlock(bool isBlock) {
    if (sock <= 0) return false;

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }
    if (isBlock) {
        flags = flags & ~O_NONBLOCK;
    } else {
        flags = flags | O_NONBLOCK;
    }
    if (fcntl(sock, F_SETFL, flags) != 0) {
        return false;
    }
    return true;
}

int MyTcp::acceptFd(int fd) {
    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd = ::accept(fd, (sockaddr *) &client_addr, &len);
    if (client_fd <= 0) {
        return 0;
    }
    return client_fd;
}


MyTcp::~MyTcp() {
}