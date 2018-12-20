#include <sys/prctl.h>
#include "server.h"
#include "../common/common.h"

bool debug = true;


int max_fd, server_fd;
//ClientInfo clientList[SERVER_MAX_CONNECT];
fd_set read_fds, write_fds;


void initClientSetup()
{
    // �ͻ������ó�ʼ��
    // while(username�� k = getFromDatabase()){
    //     // �����ݿ��еõ�һ������ username, ������� k
    //     // (select DISTINCT name, id...)

    //     mapIndex[one.name] = k      // username.to_string -> int
    //     userOffline.insert(k);
    //     firstLog.push_back(true);   // �ڵ�k������true ������û���δ��½��
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


    // �ͻ������ó�ʼ��
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

    // �ж��Ƿ���û�����
    if(userOffline.find(index) == userOffline.end()){

        // ����Ѱ��֮ǰ��clientList�д洢�ľ�����
        auto j=clientList.begin();
        for(;j!=clientList.end() ; j++)
            if(j->name == i->name){
                //  ��ԭ����j->cfdǿ������
                sndResponse(j->cfd, mt::resLogin, sbt::repeatout);

                // ��֮ǰ��client�����б����޳�
                clientList.erase(j);
                break;
            }

        // �������
        if(j == clientList.end()){
            cerr << "something went wrong!\n ";
            myExit();
        }

        // �����¿ͻ������߳ɹ�
        sndResponse(i->cfd, mt::resLogin, sbt::repeaton);
    }
    else{
        // ���Ϊ�״ε�½
        if(firstLog[index] == true){
            firstLog[index] = false;
            // ����ǿ�Ƹ�������
            sndResponse(i->cfd, mt::resLogin, sbt::changepwd);
        }
        // �ɹ���½
        else{
            // ��userOffline��ɾ������ʾ���û�������
            userOffline.erase(index); 
            // active״̬����Ϊtrue����ʾ���û��Ѽ�����Դ����ı�������
            i->alive = true;
            // �������ӳɹ�����
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
                    if(serverRecv(i->cfd, nwPacket) ==  -1){ // �������ʧ��
                        cerr << i->cfd << "send ERROR!\n";
                        continue;
                    }
                    // ��½����
                    if(nwPacket.isType(mt::login, sbt::request)){
                        loginData* p = (loginData*)nwPacket.msg;
                        i->name = p->username;// �õ��û��� TODO

                        if(!debug /*��ƥ��*/) // ��ѯ���ݿ��������û��Ƿ�ƥ�䣬���͵�½ʧ�� TODO
                            sndResponse(i->cfd, mt::resLogin, sbt::failed);

                        solveLogIn(i);
                        
                    }

                    // �����ı�

                    // ����ͷ����


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
