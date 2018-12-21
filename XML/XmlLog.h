#include "./lib/tinyxml.h"
#include "../common/common.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/time.h>

class XmlLog;

struct logBuf{
    char ip[32];
    char username[32];
    char type[16];
    char time[16];
};

class XLLogin{
private:
    TiXmlDocument doc;

    TiXmlElement* xXLLogin;
    int id;

public:
    XLLogin();
    ~XLLogin();
    void setLogin();
    void setItem(const logBuf &);

    friend XmlLog;

};

struct dataBuf{
    char sndIp[32];
    char sndUsername[32];
    char recvIp[32];
    char recvUsername[32];
    char type[16];
    char time[16];
};

class XLDataTransform{

private:
    TiXmlDocument doc;

    TiXmlElement* xXLDataTransform;

    int id;

public:
    XLDataTransform();
    ~XLDataTransform();
    void setDataTransform();
    void setItem(const dataBuf &);

    friend XmlLog;

};

class XmlLog{

private:

    XLLogin *xlLogin;
    XLDataTransform *xlDataTransform;
    struct timeval start;
    std::map<unsigned char, string>mapType;


public:

    XmlLog();
    ~XmlLog();

    void writeLogin(unsigned char subType, struct sockaddr_in &sockAddr, const char* username = "");

    void writeDataTransform(unsigned char subType, struct sockaddr_in &sndSock, struct sockaddr_in &recvSock, const char* sndUsername, const char* recvUsername);

    bool saveLog();
    void initMap();
};

static void setText(TiXmlElement*& element, const char* str);

static void setLinkNode(TiXmlElement*& father,TiXmlElement*& child, const char*str);

static char* itoa(int id);