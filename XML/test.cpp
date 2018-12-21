#include <iostream>
#include "XmlLog.h"

using namespace std;

int main()
{
    XmlLog log;
    unsigned char subType = sbt::file;
    const char* username = "ºÃÏëË¯¾õ";

    struct sockaddr_in sock;
    sock.sin_addr.s_addr = inet_addr("192.168.80.230");
    log.writeLogin(subType, sock, username);
    sock.sin_addr.s_addr = inet_addr("1.168.80.230");
    log.writeLogin(subType, sock, username);
    sleep(1);
    sock.sin_addr.s_addr = inet_addr("2.168.80.230");
    log.writeLogin(subType, sock, username);
    sock.sin_addr.s_addr = inet_addr("3.168.80.230");
    log.writeLogin(subType, sock, username);
    sleep(1);
    subType = sbt::repeaton;
    sock.sin_addr.s_addr = inet_addr("4.168.80.230");
    log.writeLogin(subType, sock, username);


    struct sockaddr_in sock2;
    sock2.sin_addr.s_addr = inet_addr("192.168.80.230");
    log.writeDataTransform(sbt::failed,sock,sock2,"jiahaolin", "menglingchen");
    sleep(1);
    log.writeDataTransform(sbt::pwderror,sock,sock2,"jiahaolin", "wanghan");
    log.writeDataTransform(sbt::repeatout,sock,sock2,"wanghan", "menglingchen");
    log.writeDataTransform(sbt::success,sock,sock2,"xiaowei", "jiahaolin");
    
    log.saveLog();

    return 0;
}