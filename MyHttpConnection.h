//
// Created by admin on 2023/11/20.
//

#ifndef SOCKET_STUDY_MAKEFILE_MYHTTPCONNECTION_H
#define SOCKET_STUDY_MAKEFILE_MYHTTPCONNECTION_H
#include "MyTcp.h"
#include "MyHttpResponse.h"

class MyHttpConnection {
public:
    MyHttpConnection(MyTcp& client);
    ~MyHttpConnection();
    bool Start();
    void Main();
    void Close();
    MyTcp& client;
    MyHttpResponse res;
};


#endif //SOCKET_STUDY_MAKEFILE_MYHTTPCONNECTION_H
