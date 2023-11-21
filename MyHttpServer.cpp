//
// Created by admin on 2023/11/20.
//

#include "MyHttpServer.h"
#include "MyHttpConnection.h"
#include <sys/epoll.h>
#include <thread>
#include <iostream>

MyHttpServer::MyHttpServer() {

}

MyHttpServer::~MyHttpServer() {

}

bool MyHttpServer::initSocket(unsigned short port) {
    server.createSocket();
    server.setOption();
    //server要设置为非阻塞，不然下面accept时没有连接时会阻塞
    if (!server.bind(port)) {
        std::cout << "bind fail!" << std::endl;
        return false;
    }
    server.setBlock(false);
    return true;
}

//不使用epoll ，跟下面的start方法可以互相替换
bool MyHttpServer::Start1(unsigned short port) {
    isexit = false;
    initSocket(port);
    std::thread th([this]() {
        this->Main();
    });
    th.detach();
    getchar();

    return true;
}

//使用epoll，跟上面的start方法可以互相替换
bool MyHttpServer::Start(unsigned short port) {
    isexit = false;
    initSocket(port);
    int epfd = epoll_create(1024);
    epoll_event ev, events[20];
    ev.data.fd = server.sock;
//    EPOLLET边缘触发，只触发一次不管有没用处理消息
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);
    while (1) {
//        std::this_thread::sleep_for(std::chrono::seconds(3));//当前线程休眠三秒
        int count = epoll_wait(epfd, events, 20, -1);
        if (count <= 0) continue;
        for (int i = 0; i < count; ++i) {
            if (events[i].data.fd == server.sock) {
                //新的连接
                while (1) {
                    //一个文件描述符上可能有多个事件同时到达，在这里就是可能同时有多个新连接，所以要循环取出来
                    MyTcp client = server.accept();
                    if (client.sock <= 0) break;
                    ev.data.fd = client.sock;
                    ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);
                }
            } else {
                epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                //可读事件 有可能是连接关闭
                MyTcp client;
                client.sock = events[i].data.fd;
                MyHttpConnection *conn = new MyHttpConnection(client);
                conn->Start();
            }
        }
    }
    return true;
}

void MyHttpServer::Stop() {
    isexit = true;
}

void MyHttpServer::Main() {
    while (!isexit) {
        MyTcp client = server.accept();
        if (client.sock <= 0)continue;
        MyHttpConnection *conn = new MyHttpConnection(client);
        conn->Start();
    }
}