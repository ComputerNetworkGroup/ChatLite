#include <fstream>
#include <string>
#include <sys/prctl.h>
#include <vector>
#include <set>
#include <map>
#include <sys/prctl.h>
#include "../common/common.h"
#include "../mysql/sql.h"
#include "../common/mypacket.h"

#define MAX_CONNECT 1000
#define MAX_LISTEN 1000
const bool flag_block = true;

using namespace std ;

namespace logt
{
    const unsigned char userDisconn = 0x01 ;
    
};
struct ClientInfo{
    int cfd;
    string name;
    bool wake ;
    sockaddr_in sockaddr ; 

    ClientInfo(string _name , int _cfd = -1){
        name = _name ;
        cfd = _cfd;
        wake = false ;
    }

};

struct loginAction 
{
    int cfd ; 
    int index ; 
    string username ;
    sockaddr_in sockaddr ; 
    unsigned char state ;

    loginAction ( int _cfd , const sockaddr_in & _sockaddr )
    {
        cfd = _cfd;
        sockaddr = _sockaddr ;
        state = sbt::request;

    }
};

class Server 
{
private :

    int max_fd, server_fd;
    fd_set read_fds, write_fds;


    vector<ClientInfo> clientList;

    vector<loginAction> loginList;

    // name 到 clientList 下标的map
    map<string, int> nameIndex;
    
    // cfd 到 clientList  下标的map
    map<int, int> cfdIndex;

    SERVER_MYSQL * dataBase;


    bool newConnect ();

    void solveLogin(int index);

    void removeLogin (vector<loginAction>::iterator i);

    void tell_clinet_onoffline (int index ,bool isOnline);

    void close_cfd(int cfd );

    void add_cfd (int cfd );

    void initClientSetup();

    //server 接收packet
    int serverRecv(int cfd , Packet & packet);

    //server 发送数据（转发消息）
    int serverSend (int cfd ,const Packet & packet );

    //server 发送回应（只有报头信息，没有msg）
    int sndResponse(int cfd , unsigned char maintype ,unsigned char subtype , const char * name =NULL );

    //   将收到的 txt 包 解包（附上发送者的用户名）
    int alterPack( Packet &  desPack , Packet & srcPack , const char * srcId );

    int alterTxtPack(Packet &desPack, Packet &srcPack, const char *srcId);

    int alterFileHeaderPack(Packet &desPack, Packet &srcPack, const char *srcId);

    int alterFileDataPack(Packet &desPack, Packet &srcPack, const char *srcId);

    void solveMsg(int index );

    int sndOneMsg(int index ,const char * rcvName , const Packet & packet );

    void user_online ( int index );

    void user_offline( int index );

    void user_leave( int index );

  public:

    Server();

    void initClinetSetup();

    void serverInit();

    void run();

    ~Server();


    
};

