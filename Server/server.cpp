
#include "server.h"


bool debug = true;



Server::Server()
{
    clientList.clear();
    loginList.clear();
    nameIndex.clear();
    cfdIndex.clear();

    dataBase = new SERVER_MYSQL();

    serverInit();

}

Server::~Server()
{
    clientList.clear();
    loginList.clear();
    nameIndex.clear();
    cfdIndex.clear();
}


void Server::initClientSetup()
{
    vector <string > nameList ; 

    dataBase->get_userlist(nameList);
    //  sql 
    int count = 0 ;

    for (auto i =0 ; i<nameList.size();i++)
    {
        clientList.push_back(ClientInfo(nameList[i]));
        nameIndex.insert(make_pair(nameList[i],i));
    }

    cout << "Total user " << nameList.size()<<endl;

    return ;
}



void Server::serverInit()
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

	
	setNonBlock(server_fd);

	setReusePort(server_fd );


    // 客户端配置初始化
    initClientSetup();
}


void Server::close_cfd(int cfd )
{
    FD_CLR( cfd , &read_fds);
    FD_CLR (cfd , &write_fds);
    close(cfd);  
}

void Server::add_cfd (int cfd )
{
    FD_SET(cfd, &read_fds);
	FD_SET(cfd, &write_fds);
    max_fd = max_fd > cfd ? max_fd : cfd;
}

void Server::removeLogin (vector<loginAction>::iterator i)
{
    cout << "ready erase "<<endl ; 
    close_cfd(i->cfd);
    loginList.erase(i);
    cout <<"erase ok !\n";
}



void Server::solveLogin(std::vector<loginAction>::iterator i)
{

    int cfd = i->cfd ; 
    cout <<"cfd = "<<cfd <<endl ;
    Packet p ;

    if(serverRecv(cfd , p )==-1)
    {
        removeLogin (i);
        return ;
    }

    //  如果不是login类型  则错误
    if(!p.isMainType(mt::login))
    {
        cout <<"not login "<<endl ;
        removeLogin(i);
        return ;
    }
    if(i->state == sbt::request &&p.isSubType(sbt::request))
    {
        loginData * datap = (loginData *) p.msg;


        //  用户名不存在
        if(nameIndex.find(datap->username)==nameIndex.end())
        {
            cout <<"not exit user name  "<<endl ;
            sndResponse(cfd , mt::resLogin, sbt::idNotExit);
            removeLogin(i);
            return ;
        }

        // 密码错误
        if(dataBase->check_user(datap->username , datap->passwd))
        {
            cout <<"not correct passwd  "<<endl ;
            sndResponse(cfd , mt::resLogin,sbt::pwderror);
            removeLogin(i);
            return ;
        }

        // 
        else 
        {
            cout <<"passwd ok "<<endl ;
            i->username = datap->username ;
            i->index = nameIndex[i->username];

            if(dataBase->need_set_passwd(datap->username))
            {
                cout << "change passwd "<<endl;
                i->state = sbt::changepwd;
                sndResponse(cfd , mt::resLogin , sbt::changepwd);
                return ;
            }
            else if ( clientList[i->index].cfd >0)
            {
                // debug
                cout << i->username <<"重复登录"<<endl;
                sndResponse(clientList[i->index].cfd , mt::login , sbt::repeatoff);
                close_cfd (clientList[i->index].cfd);
                clientList[i->index].cfd = -1 ;

                sndResponse(cfd , mt::resLogin , sbt::success);
                i->state = sbt::success;

            }
            else 
            {
                i->state = sbt::success;
                sndResponse(cfd , mt::resLogin , sbt::success);
            }

        }
    }
    else if (i->state == sbt::changepwd && p.isSubType(sbt::changepwd))
    {
        changePwdData * datap = (changePwdData *)p.msg ;
        if(dataBase->set_passwd(i->username.c_str() , datap->newPasswd)==-1 )
        {
            sndResponse(cfd , mt::resLogin,sbt::pwderror);
            removeLogin(i);
            return ;
        }
        else 
        {
            sndResponse(cfd , mt::resLogin , sbt::success );
            i->state = sbt::success;
        }

    }
    else if (i->state == sbt::success && p.isSubType(sbt::success))
    {
        clientList[i->index].cfd = cfd ;
        tell_clinet_online (i->index);
        loginList.erase(i);
    }

    cout <<"state "<< i->state <<' '<< endl ;
    return ;

}

void Server::tell_clinet_online (int index )
{

}


bool Server::newConnect()
{
	struct sockaddr_in client_addr;
	socklen_t len_client_addr = sizeof(client_addr);

	int cfd = accept(server_fd, (struct sockaddr *)&client_addr, &len_client_addr);
    if(cfd == -1){
        cerr<<"beyond ablility !\n";
        return false;
    }

    loginList.push_back(loginAction(cfd));

    cout <<"accpet ok !\n"<<endl;


    add_cfd(cfd);

	return true ;
}


void Server::run()
{

	// create_daemon();

	timeval wait_time;

    fd_set rfd_cpy , wfd_cpy ;

    while (true) {
        setTime(wait_time, 1);
        rfd_cpy = read_fds;
        wfd_cpy = write_fds ;


        switch (select(max_fd + 1, &rfd_cpy, &wfd_cpy, NULL, &wait_time))
        {
            case -1:
                cerr<<"select error ! "<<strerror(errno)<<"max ="<<max_fd<<endl;

                break;
            case 0:
                cerr<<"select time out !\n";
                break;
            default:
                if (FD_ISSET(server_fd, &read_fds)){
                    while(newConnect()==true)
                    {
                        ;
                    }
                }

                // login 
                for (auto i = loginList.begin();i != loginList.end();i++)
                    if(FD_ISSET(i->cfd,&read_fds))
                    {
                        cout << i->cfd << endl ;
                        solveLogin(i);
                        break ;
                    }
                
                // send data 
                for (auto i = clientList.begin(); i!=clientList.end();i++)
                    if(i->cfd == -1 )
                        continue ;
                    else 
                    {

                    }
        }

    }


}


int main(int argc, char *argv[])
{

    parseCMD(argc, argv, false);

	signal(SIGPIPE,SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

    Server * server = new Server();

    server->run();

	return 0;
}
