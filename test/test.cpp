#include "../common/common.h"
#include "../common/mypacket.h"

int main(int argc, char** argv)
{
	struct sockaddr_in server_addr; 
	int port =atoi(argv[2]);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port= htons(port);
	server_addr.sin_family=AF_INET;

	int cfd ; 

	if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(-1);


	if(connect(cfd,(struct sockaddr *)&server_addr,sizeof(sockaddr))==-1) {
		cerr<<"connect error !\n";
		exit(0);
    }


	cout <<"connect ok "<<endl;
    Packet packet;

//	setNonBlock(cfd);

    sndLogin(cfd, argv[3], argv[4]);


	cout <<"snd ok "<<endl;

	//sleep(1);
    clientRecv(cfd, packet);

	cout <<"maintype "<< hex << (int)packet.header.mainType<<endl;
	cout <<"subtype "<< hex << (int)packet.header.subType<<endl;
	cout << dec ;
	if(packet.isType(mt::resLogin,sbt::changepwd))
	{
		
		char passwd[] = "xixihaha";
		cout <<passwd<<endl;
		fillPacketHeader(packet.header,mt::login,sbt::changepwd,strlen(passwd)+1);
		
		strcpy(packet.msg,passwd);
		socketSend(cfd , packet);

		clientRecv(cfd, packet);
		cout <<"maintype "<< hex << (int)packet.header.mainType<<endl;
		cout <<"subtype "<< hex << (int)packet.header.subType<<endl;
		cout << dec ;

	}

	if(packet.isType(mt::resLogin, sbt::success))
	{
		fillPacketHeader(packet.header,mt::login,sbt::success , 0 );

		socketSend(cfd , packet);


		cout <<"login ok" <<endl;

		string toname , msg ;

		while(1)
		{
			if (strcmp(argv[5],"send")==0)
			{
				cout << "请输入 用户名 and txt " << endl;
				cin >> toname >> msg;
				char buff[2048];
				cout <<"msg "<<msg<<endl;
				strcpy(buff, toname.c_str());
				strcpy(buff + 32, msg.c_str());

				packet.fillPacket(mt::sndTxt, 1, buff, msg.length() + 1 + 32);

				socketSend(cfd, packet);
				
				if(clientRecv(cfd , packet)>=0)
				{
					cout << "maintype " << hex << (int)packet.header.mainType << endl;
					cout << "subtype " << hex << (int)packet.header.subType << endl;
					cout << dec;
				}

			}

			else 
			{
				getchar();
				if(clientRecv(cfd , packet)>=0)
				{
					cout << "maintype " << hex << (int)packet.header.mainType << endl;
					cout << "subtype " << hex << (int)packet.header.subType << endl;
					cout <<"from "<<packet.msg<<endl;
					cout << "msg "<<packet.msg+32 << endl;

					cout << dec;
				}

				cout << dec;
			}
					

		}

		

	}

    cout << packet.isType(mt::resLogin, sbt::changepwd) << endl;
    return 0;
}   
