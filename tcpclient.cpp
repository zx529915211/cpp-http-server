//
// Created by admin on 2023/11/16.
//
#include "MyTcp.h"

int main() {
    MyTcp client;
    client.createSocket();
//    client.setBlock(false);
    client.connect("127.0.0.1", 8500,3000);
    client.sendMsg("client", 6);
    char msg[1024] = {0};
    client.recvMsg(msg, sizeof(msg));
    printf("%s\n", msg);
    return 0;

}