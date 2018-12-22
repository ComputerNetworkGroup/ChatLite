#include "./lib/tinyxml.h"
#include "../common/common.h"
#include "../common/mypacket.h"
// #include "../Server/server.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/time.h>

struct ClientInfo{
    int cfd;
    string name;
    bool wake ;
    sockaddr_in sockaddr; 
    
    ClientInfo(string _name , int _cfd = -1){
        name = _name ;
        cfd = _cfd;
        wake = false ;
    }
};

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
    struct timeval timePresent;
    char timeBuf[32];
    std::map<unsigned char, string>mapType;


public:

    XmlLog();
    ~XmlLog();

    void _writeLogin(const unsigned char , const struct sockaddr_in &, const char*  = "");

    void _writeDataTransform(const unsigned char , const struct sockaddr_in &, const struct sockaddr_in &, const char* , const char* );

    
    void writeNorm(const ClientInfo* , const ClientInfo* , const Packet* );

    void writeError(const ClientInfo* , const unsigned char );

    bool saveLog();
    void initMap();
};

static void setText(TiXmlElement*& , const char* );

static void setLinkNode(TiXmlElement*& ,TiXmlElement*& , const char*);

static char* itoa(int id);