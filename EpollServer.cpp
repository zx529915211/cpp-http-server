//
// Created by admin on 2023/11/21.
//

#include "EpollServer.h"
#include "MyHttpServer.h"
#include "MyHttpConnection.h"
#include <sys/epoll.h>
#include "MyTcp.h"


EpollServer::EpollServer() {
    epfd = epoll_create(1024);
}

EpollServer::~EpollServer() {

}

void EpollServer::Start() {
    epoll_event events[20],ev;;
    while (1) {
//        std::this_thread::sleep_for(std::chrono::seconds(3));//当前线程休眠三秒
        int count = epoll_wait(epfd, events, 20, -1);
        if (count <= 0) continue;
        for (int i = 0; i < count; ++i) {
            auto listen =  listenFd.find(events[i].data.fd);
            if (listen != listenFd.end()) {
                //新的连接
                while (1) {
                    //一个文件描述符上可能有多个事件同时到达，在这里就是可能同时有多个新连接，所以要循环取出来
                    int sock = MyTcp::acceptFd(*listen);
                    if (sock <= 0) break;
                    ev.data.fd = sock;
                    ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
                    connectFd.insert(sock);
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
}

void EpollServer::addServer(MyHttpServer &httpserver) {
    epoll_event ev;
    int listenfd = httpserver.server.sock;
    ev.data.fd = listenfd;
    // EPOLLET边缘触发，只触发一次不管有没有处理消息
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    listenFd.insert(listenfd);
}
