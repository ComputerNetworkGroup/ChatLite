#include "common.h"

int getPacketLen(const msgPacket & packet)
{
	return noths(packet.header.length);
}

unsigned char getMainType (const msgPacket & packet)
{
	return packet.header.mainType;
}

unsigned char getSubType (const mysPacket & packet )
{
	return packet.header.subType;
}


int fillPacketHeader(packetHeader & header , unsigned char mainType , unsigned char resType , unsigned short length)
{
	header.mainType = mainType ;
	header.subType = resType;
	header.length = htons(length);
}


int socketSend (int cfd , const msgPacket & packet)
{
	int msgLen = getPacketLen(packet);
	int totalLen = 0;
	int sndLen ;

	while(totalLen <msgLen)
	{
		sndLen = send(cfd, &msgPacket+totalLen , msgLen - totalLen,0);
		if(sndLen <=0)
		{
			cerr<<"socket snd msg error !\n";
			exit(0);
		}
		totalLen +=sndLen ;
	}
	return 0;
}

int socketRecv(int cfd , msgPackt & packet)
{

	int msgLen = 32;
	int totalLen = 0;
	int recvNum;

	while(totalLen <msgLen)
	{
		recvNum =recv(cfd,&packet.header+totalLen,msgLen-totalLen,0);
		if(recvNum<=0)
		{
			cerr<<"socket recv msg error !\n ";
			exit(0);
		}
		totalLen +=recvNum ;
	}

	msgLen = getPacketLen(packet);

	while(totalLen <msgLen)
	{
		recvNum =recv(cfd,&packet+totalLen,msgLen-totalLen,0);
		if(recvNum<=0)
		{
			cerr<<"socket recv msg error !\n ";
			exit(0);
		}
		totalLen +=recvNum ;
	}

	return 0;
}

int sndLogin(int cfd , const char * username , const char * passwd )
{
	int ulen = strlen(username);
	int plen = strlen(passwd);
	if(ulen >32 || plen >32)
		return -1 ;
	

	//TODO
}


//   clinet 

int clientRecv(int cfd , msgPacket & packet )
{

}

int sndText(int cfd , const char * id , const char * text )
{

}

int sndJPG (int cfd , const char * id , const void * jpg , int size )
{

}

int sndGIF(int cfd , const char * id , const void * gif , int size )
{

}

// server 

int serverRecv(int cfd , msgPacket & packet)
{

}

int serverSend (int cfd , msgPacket & packet )
{

}

int sndResponse(int cfd , unsigned char maintype ,unsigned char subtype )
{

}


