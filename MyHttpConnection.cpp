//
// Created by admin on 2023/11/20.
//

#include "MyHttpConnection.h"
#include <thread>

MyHttpConnection::MyHttpConnection(MyTcp& client):client(client) {

}

MyHttpConnection::~MyHttpConnection() {

}

bool MyHttpConnection::Start() {
    std::thread th([this]() {
        this->Main();
    });
    th.detach();
    return true;
}


void MyHttpConnection::Main() {
    char buf[10240] = {0};
    while (1) {
        int len = client.recvMsg(buf, sizeof(buf) - 1);
        if (len <= 0)break;
        buf[len] = '\0';
        if (!res.SetRequest(buf)) {
            break;
        }
        std::string head = res.GetHead();
        if (client.sendMsg(head.c_str(), head.size()) <= 0) {
            break;
        }
        int size = sizeof(buf);
        bool error = false;
        while (1) {
            len = res.Read(buf, size);
            if (len < 0){
                error = true;
                break;
            }
            if(len == 0){
                break;
            }
            if (client.sendMsg(buf, len) <= 0) {
                error = true;
                break;
            }
        }
    }
    return Close();
}

void MyHttpConnection::Close() {
    client.close();
    delete this;
    return;
}