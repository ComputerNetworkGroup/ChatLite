#include "common.h"

int socketSend (int cfd , const Packet & packet);

int socketRecv(int cfd , Packet & packet);



int getPacketLen(const Packet & packet)
{
	return ntohs(packet.header.length);
}

unsigned char getMainType (const Packet & packet)
{
	return packet.header.mainType;
}

unsigned char getSubType (const Packet & packet )
{
	return packet.header.subType;
}

int getFileSize(const char * filePath)
{
	FILE * file = fopen("233.jpg","r");
	int size ;
	if(file)
	{
		fseek(file,0,SEEK_END);
		size = ftell(file);
		fclose(file);
		return size ;
	}
	
	return -1 ;
}

int getFileName( const char * filePath)
{
	int i ;
	for(i=strlen(filePath);i>=0;i--)
		if(filePath[i]=='\\'||filePath[i]=='/')
			break;
	return i+1;

}

int sndFileKind(int cfd , const char * id , unsigned char subType, const char * filepath)
{

	Packet p ;

	// send fileHeader 
	fillPacketHeader(p.header,mt::sndFileHead, subType ,sizeof(fileHeader));
	fileHeader * headp = (fileHeader *)&p.msg;
	int fileSize = getFileSize(filepath);
	if(fileSize <0)
		return -1 ;
	
	strcpy(headp->friName,id);
	strcpy(headp->fileName,filepath+getFileName(filepath));
	int fileId ; 
	// TODO    ¼ÆËãidÖµ
	headp->fileId = fileId ;
	int packNum = fileSize / MAXLENGTH + (fileSize % MAXLENGTH)!=0;
	headp->packNum =  htonl(packNum);

	socketSend(cfd,p);
	
	// send fileData 

	fillPacketHeader(p.header,mt::sndFile,sbt::myDefault,sizeof(fileData));

	FILE * filep = fopen(filepath , "r");
	fileData * datap = (fileData *)&p.msg ; 

	strcpy(datap->friName , id );
	datap->fileId = htonl(fileId) ; 
	datap->count = htonl(0) ;


	for(int i =0 ; i< packNum ; i++)
	{
		fread(datap->data,MAXLENGTH,1,filep);
		if(i==packNum-1 && (fileSize % MAXLENGTH))
			p.header.length = htonl(fileSize % MAXLENGTH );
		datap->count = htonl(i);
		socketSend(cfd , p);
	}

	return 0;

}


int fillPacketHeader(packetHeader & header , unsigned char mainType , unsigned char resType , unsigned short msgLen)
{
	header.mainType = mainType ;
	header.subType = resType;
	header.length = htons(msgLen+32);
}


int socketSend (int cfd , const Packet & packet)
{
	int msgLen = getPacketLen(packet);
	int totalLen = 0;
	int sndLen ;

	while(totalLen <msgLen)
	{
		sndLen = send(cfd, &packet+totalLen , msgLen - totalLen,0);
		if(sndLen <=0)
		{
			cerr<<"socket snd msg error !\n";
			exit(0);
		}
		totalLen +=sndLen ;
	}
	return 0;
}

int socketRecv(int cfd , Packet & packet)
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
	
	Packet p ;
	
	fillPacketHeader(p.header,mt::login,sbt::request , sizeof(loginData));

	loginData * datap = (loginData *) p.msg ;

	strcpy(datap->username,username);
	strcpy(datap->passwd, passwd);

	return socketSend ( cfd , p);

}


//   clinet 

int clientRecv(int cfd , Packet & packet )
{
	return socketRecv (cfd , packet );
}

int sndText(int cfd , const char * ids  , int idNum  , const char * text )
{
	Packet p ;
	
	int idLen = idNum * 32 ; 
	memcpy(p.msg , ids ,idLen);

	int textLen = strlen(text);
	
	memcpy(p.msg + idLen ,  text , textLen );

	fillPacketHeader(p.header, mt::sndTxt, (unsigned char) idNum , idLen + textLen );

	socketSend(cfd, p);

	return 0 ;
}

int sndJPG (int cfd , const char * id , const char * jpgpath )
{

	return sndFileKind(cfd, id , sbt::jpg , jpgpath);

}

int sndGIF(int cfd , const char * id , const char * gifpath)
{

	return sndFileKind(cfd, id , sbt::gif , gifpath);

}

int sndFile (int cfd , const char * id , const char * filepath)
{

	return sndFileKind (cfd , id , sbt::file , filepath);
}


int firstChangePwd(int cfd , const char * newPwd )
{
	Packet p ;
	int msgLen = strlen(newPwd);
	
	strcpy(p.msg,newPwd);
	
	fillPacketHeader(p.header,mt::login,sbt::changepwd,msgLen);
	
	return socketSend(cfd, p);
}

// server 

int serverRecv(int cfd , Packet & packet)
{
	return socketRecv(cfd , packet);
}

int serverSend (int cfd , Packet & packet )
{
	return socketSend (cfd , packet );
}

int sndResponse(int cfd , unsigned char maintype ,unsigned char subtype )
{
	Packet p ;
	fillPacketHeader(p.header,maintype, subtype, 0 );

	return socketSend(cfd , p);
}


