#include <fstream>
#include <sys/prctl.h>
#include "server.h"

int ctr_clinet = 0;
int max_fd, server_fd;
int max_clientinfo;
//ClientInfo clientinfos[SERVER_MAX_CONNECT];
ClientInfo * clientinfos ; 
fd_set read_fds, write_fds;

int totalConnectCfd = 0;

using namespace std;

void killClient(ClientInfo &);

void serverInit()
{
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		myExit();

	max_fd = server_fd;
	//memset(clientinfos, 0, sizeof(clientinfos));
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

	return;
}

int selectNewClientinfo()
{
	int i;
	for (i = 0; i < SERVER_MAX_CONNECT; i++)
	{
		if (clientinfos[i].flag == DONE || clientinfos[i].flag == DEAD)
			return i;
	}
	return -1 ;
}

void killClient(ClientInfo &client_info)
{

	if(client_info.flag == ALIVE &&client_info.count>8) {
		if(writeFile(client_info, true)==false)
			return ;
		//cout<<"ready write \n";
	}
	client_info.flag = DEAD;
	close(client_info.cfd);
	FD_CLR(client_info.cfd, &read_fds);
	FD_CLR(client_info.cfd, &write_fds);
	totalConnectCfd--;
	cout <<"totalConnectCfd "<<totalConnectCfd<<endl;

	return;
}


bool sSend(ClientInfo &client_info)
{
	int sndNum = send(client_info.cfd, sndMsg + client_info.rwLen, MsgLen - client_info.rwLen, 0);

	if (sndNum <= 0) {
		// TODO
		cerr << "cllient_cfd :" << client_info.cfd << "snd error !\n";
		killClient(client_info);
		return false;
	}
	return true;
}

bool sRecv(ClientInfo &client_info)
{
	int readNum = recv(client_info.cfd, &(client_info.msg.StuNo) + client_info.rwLen, MsgLen, 0);
	if (readNum <= 0) {
         // TODO
		cerr << "cllient_cfd :" << client_info.cfd << "recv error !"<<client_info.count<<endl;
		killClient(client_info);
		return false;
	}

	client_info.rwLen += readNum;
	if (client_info.rwLen == MsgLen)
	{
		if(count==GETSTUNO)
			client_info.msg.StuNo = ntohl(client_info.msg.StuNo);
		else if(count == GETPID)
			client_info.msg.clientPid= ntohl(client_info.msg.clientPid);
		else if (count == GETTIME)
			client_info.msg.clientTime[MsgLen] = 0;
		client_info.count++;
		client_info.rwLen = 0;
	}
	return true;
}

void dealFork(const int cfd)
{
	int pid = fork();
	if (pid < 0) {
		close(cfd);
		cerr << "fork failed !\n";
		return;
	}
	if (pid > 0){
		close(cfd);
		return;
	}

	prctl(PR_SET_PDEATHSIG,SIGKILL);

	ClientInfo client_info;
	client_info.cfd = cfd;
	client_info.count = 0;
	client_info.flag = ALIVE;
	client_info.rwLen = 0;
	memset(&(client_info.msg), 0, sizeof(client_info.msg));


    //cout <<"unblock !\n";
    FD_ZERO(&read_fds);
    FD_SET(cfd, &read_fds);
    write_fds = read_fds;

    fd_set rfd_cpy, wfd_cpy;
    timeval wait_time;
    while(client_info.flag == ALIVE) {
        
        setTime(wait_time,1);
        rfd_cpy = read_fds;
        wfd_cpy = write_fds;
        switch (select(cfd + 1, &rfd_cpy, &wfd_cpy, NULL, &wait_time)) {
            case -1:
                cerr<<"select error !\n";
                break;
            case 0 :
                // cerr<<"waiting for events !\n";
                break;
            default :

                if(FD_ISSET(cfd,&rfd_cpy)) {
                    
                    sRecv(client_info);
                }
                if(FD_ISSET(cfd,&wfd_cpy)){

                    sSend(client_info);
                }
        }
    }

	exit(0);
	return;
}



int main(int argc, char *argv[])
{
	// create_daemon();
	parseCMD(argc, argv, false);

	signal(SIGPIPE,SIG_IGN);
	signal(SIGCHLD, SIG_IGN);


	serverInit();

	fd_set rfd_cpy, wfd_cpy;
	timeval wait_time;

	char msgTest [5];


    while (true) {
        setTime(wait_time, 1);
        rfd_cpy = read_fds;
        
        switch (select(server_fd + 1, &rfd_cpy, NULL, NULL, &wait_time)) {
            case -1:
                cerr<<"select error ! "<<strerror(errno)<<"max ="<<max_fd<<endl;
                break;
            case 0:
                cout << "waiting for client connecting...\n";
                break;
            default:
                //cout<<"select success !\n";
                if (FD_ISSET(server_fd, &rfd_cpy)) {
                    int new_client_fd = accept(server_fd, NULL, NULL);
                    if (new_client_fd < 0) {
                        cerr << new_client_fd  << "failed to connect !\n";
                        break;
                    }
                    setNonBlock(new_client_fd);
                    
                    dealFork(new_client_fd);
                    cout << new_client_fd <<" has connected !\n";
                }
                break;
        }
    }
	
	return 0;
}
