
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <sys/epoll.h>
#include "MyTcp.h"

class TcpThread {
public:
    void main() {
        char buf[1024] = {0};
        while (1) {
            int msg_len = client.recvMsg(buf, sizeof(buf) - 1);
            if (msg_len <= 0)break;
            buf[msg_len] = '\0';
            if (strstr(buf, "quit") != NULL) {
                client.sendMsg("quit", 5);
                break;
            }
            int sendlen = client.sendMsg("ok", 3);
            return;
            printf("recv %s\n", buf);
        }
        client.close();
        delete this;
    }

    MyTcp client;
};


int main(int argc, char *argv[]) {
    unsigned short port = 8600;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    MyTcp server;
    server.createSocket();
    bool ret = server.bind(port);
    //连接socket设置为非阻塞，accept的时候没有数据才不会阻塞
    server.setBlock(false);
    if (!ret) {
        std::cout << "bind fail!" << std::endl;
        return -2;
    }
    int epfd = epoll_create(1024);
    epoll_event ev;
    ev.data.fd = server.sock;
//    EPOLLET边缘触发，只触发一次不管有没用处理消息
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);

    epoll_event events[20];//最多返回20个事件
    char buf[1024] = {0};
    const char *msg = "HTTP/1.1 200 OK\r\nContent-length: 5\r\n\r\nhello";
    while (1) {
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
                //可读事件 有可能是连接关闭
                MyTcp client;
                client.sock = events[i].data.fd;
                client.recvMsg(buf, 1024);
                client.sendMsg(msg, strlen(msg));
                epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);
                client.close();
            }
        }
//        MyTcp client = server.accept();
//        TcpThread *th = new TcpThread();
//        th->client = client;
//        std::thread sock_th(&TcpThread::main, th);
//        sock_th.detach();
    }
    server.close();
    return 0;
}
