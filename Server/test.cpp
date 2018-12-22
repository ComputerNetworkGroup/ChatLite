#include "myutil.h"
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

    Packet packet;
    sndLogin(cfd, "root", "123");
    clientRecv(cfd, packet);
    cout << packet.isType(mt::resLogin, sbt::changepwd) << endl;
    return 0;
}   