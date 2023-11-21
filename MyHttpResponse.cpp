//
// Created by admin on 2023/11/20.
//

#include "MyHttpResponse.h"
#include <string>
#include <iostream>
#include <regex>


MyHttpResponse::MyHttpResponse() {

}

MyHttpResponse::~MyHttpResponse() {

}

std::string MyHttpResponse::GetHead() {
    //发送相应头
    std::string content_length = std::to_string(filesize);
    std::string return_msg = "";
    return_msg += "HTTP/1.1 200 OK\r\n";
    return_msg += "Content-length: " + content_length + "\r\n";
    return_msg += "Content-Type: text/html\r\n";
    return_msg += "\r\n";
    return return_msg;
}

bool MyHttpResponse::SetRequest(std::string request) {
    //正则表达式匹配http请求
    std::string src = request;
    std::string pattern = "^([A-Z]+) /([^.]*([.]([a-zA-Z]*))?)([?]?(.*))? HTTP/1";
    std::regex r(pattern);
    std::smatch mas;
    std::regex_search(src, mas, r);
    if (mas.size() == 0) {
        printf("not a true http request!!\n");
        return false;
    }
    std::string type = mas[1];
    std::string path = std::string("/") + std::string(mas[2]);//访问路径，例如xxx/index.php
//            std::string filetype = mas[3];//后缀名，例如.php，包括.符号
    std::string filetype = mas[4];//后缀名，例如php,不包括.符号
    std::string params = mas[6];//get参数

    if (type != "GET") {
        printf("not a get request!!\n");
        return false;
    }


    std::string filename = path;
    if (path == "/") {
        filename = "/index.html";
    }
    std::string filepath = "www";//文件路径
    filepath += filename;

    if (filetype == "php") {
        std::string cmd = "php " + filepath;
        if(!params.empty()){
            //传递get参数
            for (int i = 0; i < params.size(); ++i) {
                if(params[i] == '&') params[i] = ' ';
            }
            cmd += " " + params;
        }
        cmd += " > ";
        filepath += ".html";
        cmd += filepath;
        system(cmd.c_str());
        std::cout << cmd << std::endl;
    }

    fp = fopen(filepath.c_str(), "rb");
    if (fp == NULL) {
        printf("don't have the file!!\n");
        return false;
    }
    fseek(fp, 0, SEEK_END);//把文件指针移到结尾处
    filesize = ftell(fp);//文件指针的位置，也就是获取到了文件的字节大小
    fseek(fp, 0, 0);//文件指针移回去
    printf("filesize = %d\n", filesize);
    return true;
}

int MyHttpResponse::Read(char *buf, int bufsize) {
    return fread(buf, 1, bufsize, fp);
}