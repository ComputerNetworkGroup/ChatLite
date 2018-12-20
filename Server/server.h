#include "myutil.h"
#define MAX_CONNECT 1000
#define MAX_LISTEN 20
const bool flag_block = true;


struct ClientInfo{
    bool alive;
    int cfd;
    string name;
    ClientInfo(int _cfd = -1){
        cfd = _cfd;
        alive = false;
    }
};

std::vector<ClientInfo> clientList; // 客户端列表
std::vector<bool> firstLog;         // 客户端首次登陆标记数组，下标与map映射的k一致
std::set<int> userOffline;          // 下线用户集合
std::map<string, int> mapIndex;     // 用户名 --映射--> 下线用户集合index