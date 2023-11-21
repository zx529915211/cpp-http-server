//
// Created by admin on 2023/11/20.
//

#ifndef SOCKET_STUDY_MAKEFILE_MYHTTPRESPONSE_H
#define SOCKET_STUDY_MAKEFILE_MYHTTPRESPONSE_H
#include <string>

class MyHttpResponse {
public:
    MyHttpResponse();
    ~MyHttpResponse();
    std::string GetHead();
    bool SetRequest(std::string request);
    int Read(char *buf,int bufsize);
private:
    int filesize = 0;
    FILE *fp = NULL;
};


#endif //SOCKET_STUDY_MAKEFILE_MYHTTPRESPONSE_H
