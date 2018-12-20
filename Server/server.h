#include <fstream>
#include <string>
#include <sys/prctl.h>
#include <vector>
#include <set>
#include <map>
#include <sys/prctl.h>
#include "myutil.h"
#include "../sql2/sql.h"
#include "../common/mypacket.h"

#define MAX_CONNECT 1000
#define MAX_LISTEN 1000
const bool flag_block = true;

using namespace std ;

struct ClientInfo{
    int cfd;
    string name;
    ClientInfo(string _name , int _cfd = -1){
        name = _name ;
        cfd = _cfd;
    }
};

namespace lg 
{
    const unsigned char request = 0x00;
    const unsigned char changepwd = 0x01 ;
    const unsigned char success = 0x02 ;

}

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

    map<string, int> nameIndex;
    
    map<int, int> cfdIndex;

    SERVER_MYSQL * dataBase;


    bool newConnect ();

    void solveLogin(std::vector<loginAction>::iterator i);

    void removeLogin (vector<loginAction>::iterator i);

    void tell_clinet_online (int index );

    void close_cfd(int cfd );

    void add_cfd (int cfd );

    void initClientSetup();

public :

    Server();

    void initClinetSetup();

    void serverInit();

    void run();

    ~Server();


    
};

