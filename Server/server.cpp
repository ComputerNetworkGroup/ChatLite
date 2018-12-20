#include <sys/prctl.h>
#include "server.h"
#include "../common/common.h"

bool debug = true;


int max_fd, server_fd;
//ClientInfo clientList[SERVER_MAX_CONNECT];
fd_set read_fds, write_fds;


void initClientSetup()
{
    // 客户端配置初始化
    // while(username， k = getFromDatabase()){
    //     // 从数据库中得到一个名字 username, 及其序号 k
    //     // (select DISTINCT name, id...)

    //     mapIndex[one.name] = k      // username.to_string -> int
    //     userOffline.insert(k);
    //     firstLog.push_back(true);   // 在第k个加入true 代表该用户尚未登陆过
    // }

    mapIndex["root"]=0;
    userOffline.insert(0);
    firstLog.push_back(true);

}

void serverInit()
{
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		myExit();

	max_fd = server_fd;
	//memset(clientList, 0, sizeof(clientList));
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_SET(server_fd, &read_fds);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		myExit();

	if (listen(server_fd, MAX_LISTEN)==-1)
		myExit();

	if (!flag_block)
		setNonBlock(server_fd);

	setReusePort(server_fd );


    // 客户端配置初始化
    initClientSetup();
}


// void killClient(ClientInfo &client_info)
// {

// 	if(client_info.flag == ALIVE &&client_info.count>8) {
// 		if(writeFile(client_info, true)==false)
// 			return ;
// 		//cout<<"ready write \n";
// 	}
// 	client_info.flag = DEAD;
// 	close(client_info.cfd);
// 	FD_CLR(client_info.cfd, &read_fds);
// 	FD_CLR(client_info.cfd, &write_fds);
// 	totalConnectCfd--;
// 	cout <<"totalConnectCfd "<<totalConnectCfd<<endl;

// 	return;
// }



void solveLogIn(std::vector<ClientInfo>::iterator i)
{
    int index = mapIndex[i->name];

    // 判断是否该用户存在
    if(userOffline.find(index) == userOffline.end()){

        // 遍历寻找之前在clientList中存储的旧数据
        auto j=clientList.begin();
        for(;j!=clientList.end() ; j++)
            if(j->name == i->name){
                //  将原来的j->cfd强制下线
                sndResponse(j->cfd, mt::resLogin, sbt::repeatout);

                // 从之前的client链接列表中剔除
                clientList.erase(j);
                break;
            }

        // 程序出错
        if(j == clientList.end()){
            cerr << "something went wrong!\n ";
            myExit();
        }

        // 发送新客户端上线成功
        sndResponse(i->cfd, mt::resLogin, sbt::repeaton);
    }
    else{
        // 如果为首次登陆
        if(firstLog[index] == true){
            firstLog[index] = false;
            // 发送强制改密请求
            sndResponse(i->cfd, mt::resLogin, sbt::changepwd);
        }
        // 成功登陆
        else{
            // 从userOffline中删除，表示该用户已上线
            userOffline.erase(index); 
            // active状态设置为true，表示该用户已激活，可以传送文本等数据
            i->alive = true;
            // 发送链接成功请求
            sndResponse(i->cfd, mt::resLogin, sbt::success);
        }
    }
}


bool newConnect()
{
	struct sockaddr_in client_addr;
	socklen_t len_client_addr = sizeof(client_addr);

	int cfd = accept(server_fd, (struct sockaddr *)&client_addr, &len_client_addr);
    if(cfd == -1){
        cerr<<"beyond ablility !\n";
        return false;
    }

    ClientInfo nwClient(cfd);
    clientList.push_back(nwClient);

	FD_SET(cfd, &read_fds);
	FD_SET(cfd, &write_fds);

	max_fd = max_fd > cfd ? max_fd : cfd;

	return true ;
}

int main(int argc, char *argv[])
{
	// create_daemon();
	parseCMD(argc, argv, false);

	signal(SIGPIPE,SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	serverInit();

	timeval wait_time;

    while (true) {
        setTime(wait_time, 1);

        switch (select(max_fd + 1, &read_fds, &write_fds, NULL, &wait_time)){
        case -1:
            cerr<<"select error ! "<<strerror(errno)<<"max ="<<max_fd<<endl;
            sleep(1);
            //if (errno != EINTR)
            //	myExit();
            break;
        case 0:
            cerr<<"select time out !\n";
            break;
        default:
            if (FD_ISSET(server_fd, &read_fds)){
                while(newConnect()==true){
                    ;
                }
            }
            for(auto i = clientList.begin() ; i != clientList.end(); i++){
                if(i->cfd == -1)
                    continue;
                if (FD_ISSET(i->cfd, &read_fds)) {
                    Packet nwPacket;
                    if(serverRecv(i->cfd, nwPacket) ==  -1){ // 如果接收失败
                        cerr << i->cfd << "send ERROR!\n";
                        continue;
                    }
                    // 登陆请求
                    if(nwPacket.isType(mt::login, sbt::request)){
                        loginData* p = (loginData*)nwPacket.msg;
                        i->name = p->username;// 得到用户名 TODO

                        if(!debug /*不匹配*/) // 查询数据库密码与用户是否匹配，发送登陆失败 TODO
                            sndResponse(i->cfd, mt::resLogin, sbt::failed);

                        solveLogIn(i);
                        
                    }

                    // 发送文本

                    // 发送头报文


                }
                if (FD_ISSET(i->cfd, &write_fds) )  {
                    ;
                }
            }
            break;
        }
    }
    clientList.clear();


	return 0;
}
