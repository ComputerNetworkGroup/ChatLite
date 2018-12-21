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

struct ClientInfo{
    int cfd;
    string name;
    bool wake ;
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
    unsigned char state ;

    loginAction ( int _cfd )
    {
        cfd = _cfd;
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

    // name ╣╫ clientList об╠Й╣дmap
    map<string, int> nameIndex;
    
    // cfd ╣╫ clientList  об╠Й╣дmap
    map<int, int> cfdIndex;

    SERVER_MYSQL * dataBase;


    bool newConnect ();

    void solveLogin(int index);

    void removeLogin (vector<loginAction>::iterator i);

    void tell_clinet_online (int index );

    void close_cfd(int cfd );

    void add_cfd (int cfd );

    void initClientSetup();

    //server О©╫О©╫О©╫О©╫packet
    int serverRecv(int cfd , Packet & packet);

    //server О©╫О©╫О©╫О©╫О©╫О©╫О©╫щёО©╫в╙О©╫О©╫О©╫О©╫о╒О©╫О©╫
    int serverSend (int cfd ,const Packet & packet );


    //server О©╫О©╫О©╫м╩О©╫с╕О©╫О©╫ж╩О©╫п╠О©╫м╥О©╫О©╫о╒О©╫О©╫ц╩О©╫О©╫msgО©╫О©╫
    int sndResponse(int cfd , unsigned char maintype ,unsigned char subtype , const char * name =NULL );


    //   О©╫О©╫О©╫у╣О©╫О©╫О©╫ txt О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫о╥О©╫О©╫О©╫О©╫ъ╣О©╫О©╫ц╩О©╫О©╫О©╫О©╫О©╫
    int alterPack( Packet &  desPack , Packet & srcPack , const char * srcId );

    int alterTxtPack(Packet &desPack, Packet &srcPack, const char *srcId);

    int alterFileHeaderPack(Packet &desPack, Packet &srcPack, const char *srcId);

    int alterFileDataPack(Packet &desPack, Packet &srcPack, const char *srcId);

    void solveMsg(int index );

    int sndOneMsg(int index ,const char * rcvName , const Packet & packet );

  public:

    Server();

    void initClinetSetup();

    void serverInit();

    void run();

    ~Server();


    
};

