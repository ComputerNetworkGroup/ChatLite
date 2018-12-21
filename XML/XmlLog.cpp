#include "XmlLog.h"


static void setText(TiXmlElement*& element, const char* str)
{
    TiXmlText* text = new TiXmlText(str);
    element->LinkEndChild(text);
}

static void setLinkNode(TiXmlElement*& father,TiXmlElement*& child, const char*str)
{
    father->LinkEndChild(child);
    setText(child, str);
}

static char* itoa(int id)
{
    char *tmp = new char[32];
    sprintf(tmp,"%d",id);
    return tmp;
}

XLLogin::XLLogin()
{
    id = 1;
}

XLLogin::~XLLogin()
{
    doc.Clear();
}

void XLLogin::setLogin()
{
    xXLLogin = new TiXmlElement("Login");
    doc.LinkEndChild(xXLLogin);
}

void XLLogin::setItem(const logBuf &buffer)
{
    TiXmlElement* xItem = new TiXmlElement("Item");
    xXLLogin->LinkEndChild(xItem);
    xItem->SetAttribute("id", itoa(id++));

    TiXmlElement* xIp = new TiXmlElement("Ip");
    TiXmlElement* xUsername = new TiXmlElement("Username");
    TiXmlElement* xType = new TiXmlElement("Type");
    TiXmlElement* xTime = new TiXmlElement("Time");

    setLinkNode(xItem, xIp, buffer.ip);

    setLinkNode(xItem, xUsername, buffer.username);

    setLinkNode(xItem, xType, buffer.type);

    setLinkNode(xItem, xTime, buffer.time);
}





XLDataTransform::XLDataTransform()
{
    id = 1;
}

XLDataTransform::~XLDataTransform()
{
    doc.Clear();
}

void XLDataTransform::setDataTransform()
{
    xXLDataTransform = new TiXmlElement("DataTransform");
    doc.LinkEndChild(xXLDataTransform);
}

void XLDataTransform::setItem(const dataBuf &buffer)
{
    TiXmlElement* xItem = new TiXmlElement("Item");
    xXLDataTransform->LinkEndChild(xItem);
    xItem->SetAttribute("id", itoa(id++));

    TiXmlElement* xSndIP = new TiXmlElement("SendIP");
    TiXmlElement* xSndUsername = new TiXmlElement("xSndUsername");
    TiXmlElement* xRecvIP = new TiXmlElement("RecieveIP");
    TiXmlElement* xRecvUsername = new TiXmlElement("xRecvUsername");
    TiXmlElement* xType = new TiXmlElement("Type");
    TiXmlElement* xTime = new TiXmlElement("Time");

    setLinkNode(xItem, xSndIP, buffer.sndIp);

    setLinkNode(xItem, xSndUsername, buffer.sndUsername);

    setLinkNode(xItem, xRecvIP, buffer.recvIp);

    setLinkNode(xItem, xRecvUsername, buffer.recvUsername);

    setLinkNode(xItem, xType, buffer.type);

    setLinkNode(xItem, xTime, buffer.time);
}






XmlLog::XmlLog()
{
    gettimeofday(&start, NULL); 
    xlLogin = new XLLogin;
    xlLogin->setLogin();
    xlDataTransform = new XLDataTransform;
    xlDataTransform->setDataTransform();
    initMap();
}

XmlLog::~XmlLog()
{
    delete xlLogin;
    delete xlDataTransform;
    mapType.clear();
}

void XmlLog::writeLogin(unsigned char subType, struct sockaddr_in &sockAddr, const char* username)
{
    struct timeval end;
    gettimeofday(&end, NULL);
    int t = end.tv_sec - start.tv_sec;

    logBuf buf;
    strcpy(buf.ip, inet_ntoa(sockAddr.sin_addr));
    strcpy(buf.username, username);
    strcpy(buf.type, mapType[subType].c_str());
    strcpy(buf.time, itoa(t));
    strcat(buf.time, "s");
    
    xlLogin->setItem(buf);
}

void XmlLog::writeDataTransform(unsigned char subType, struct sockaddr_in &sndSock, struct sockaddr_in &recvSock, const char* sndUsername, const char* recvUsername)
{
    struct timeval end;
    gettimeofday(&end, NULL);
    int t = end.tv_sec - start.tv_sec;

    dataBuf buf;
    strcpy(buf.sndIp, inet_ntoa(sndSock.sin_addr));
    strcpy(buf.sndUsername, sndUsername);
    strcpy(buf.recvIp, inet_ntoa(recvSock.sin_addr));
    strcpy(buf.recvUsername, recvUsername);
    strcpy(buf.type, mapType[subType].c_str());
    strcpy(buf.time, itoa(t));
    strcat(buf.time, "s");
    
    xlDataTransform->setItem(buf); 
}

bool XmlLog::saveLog()
{
    bool flag1 = xlLogin->doc.SaveFile("log_in.xml");

    bool flag2 = xlDataTransform->doc.SaveFile("data_transform.xml");

    if(flag1 && flag2)
        return true;
    return false;
}

void XmlLog::initMap()
{
    mapType[sbt::failed] = "验证失败";
    mapType[sbt::success] = "验证成功";
    mapType[sbt::pwderror] = "密码错误";
    mapType[sbt::repeaton] = "新上线重复登陆";
    mapType[sbt::repeatout] = "强制下线重复登陆";
    // mapType[sbt::sndTxt] = "发送文本";
    mapType[sbt::file] = "发送文件";
}