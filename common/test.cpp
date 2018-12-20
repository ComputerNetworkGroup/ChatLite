#include "mypacket.h"

int main ()
{

	Packet p ;
	
	fillPacketHeader(p.header,mt::login,sbt::request,0);
	
	cout << p.isType(mt::login,sbt::request)<<endl;

	cout <<p.isType (mt::resConf,sbt::pwderror)<<endl;


	return 0 ;
}
