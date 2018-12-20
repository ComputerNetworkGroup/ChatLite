#include "mypacket.h"

int socketSend (int cfd , const Packet & packet);

int socketRecv(int cfd , Packet & packet);



int getPacketLen(const Packet & packet)
{
	return ntohs(packet.header.length);
}


int getFileSize(const char * filePath)
{
	FILE * file = fopen(filePath,"r");
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
	fileHeader * headp = (fileHeader *)p.msg;
	int fileSize = getFileSize(filepath);
	if(fileSize <0)
		return -1 ;
	
	strcpy(headp->friName,id);
	strcpy(headp->fileName,filepath+getFileName(filepath));
	int fileId ; 
	// TODO    计算id值
	headp->fileId = fileId ;
	int packNum = fileSize / MAXLENGTH + (fileSize % MAXLENGTH)!=0;
	headp->packNum =  htonl(packNum);

	socketSend(cfd,p);
	
	// send fileData 

	fillPacketHeader(p.header,mt::sndFile,sbt::myDefault,sizeof(fileData));

	FILE * filep = fopen(filepath , "r");
	fileData * datap = (fileData *)p.msg ; 

	strcpy(datap->friName , id );
	datap->fileId = htonl(fileId) ; 
	datap->count = htonl(0) ;


	for(int i =0 ; i< packNum ; i++)
	{
		fread(datap->data,MAXLENGTH,1,filep);

		//  TEST 
		if(i==packNum-1 && (fileSize % MAXLENGTH))
			p.header.length = htonl(fileSize % MAXLENGTH + 40 );
		
		datap->count = htonl(i);
		socketSend(cfd , p);
	}

	return 0;

}


int alterTxtPack( Packet &  desPack , Packet & srcPack , const char * srcId )
{
	int idNum = (int )srcPack.header.subType ;
	int idLen = idNum * MAXNAMELEN ;
	
	int txtLen = getPacketLen(srcPack) - idLen ;
	
	strcpy (desPack.msg , srcId );
	strcpy(desPack.msg+MAXNAMELEN , srcPack.msg + idLen);

	fillPacketHeader(desPack.header,srcPack.header.mainType,srcPack.header.subType,txtLen +MAXNAMELEN );

	return 0;
}

int alterFileHeaderPack(Packet & desPack , Packet & srcPack , const char * srcId )
{
	memcpy( &desPack , & srcPack , sizeof(desPack));
	
	fileHeader * fhp = (fileHeader *) desPack.msg;
	strcpy(fhp->friName , srcId );

	return 0;
}

int alterFileDataPack (Packet & desPack , Packet & srcPack , const char * srcId )
{
	memcpy( &desPack , & srcPack , sizeof(desPack));
	
	fileData * fdp = (fileData *) desPack.msg;
	strcpy(fdp->friName , srcId );

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int fillPacketHeader(packetHeader & header , unsigned char mainType , unsigned char resType , unsigned short msgLen)
{
	header.mainType = mainType ;
	header.subType = resType;
	header.length = htons(msgLen+4);
}


int socketSend (int cfd , const Packet & packet)
{
	int msgLen = getPacketLen(packet);
	int totalLen = 0;
	int sndLen ;

	char * buff = (char *) &packet ;
	while(totalLen <msgLen)
	{
		sndLen = send(cfd, buff+totalLen , msgLen - totalLen,0);
		if(sndLen <=0)
		{
			cout << strerror(errno)<<endl;
			cerr<<"socket snd msg error !\n";
			return -1 ;
		}
		else 
			totalLen +=sndLen ;
	}
	cout << "0 snd len = "<< sndLen << endl ;
	return 0;
}

int socketRecv(int cfd , Packet & packet)
{

	int msgLen = 4;
	int totalLen = 0;
	int recvNum;


	// unsolved 
	char * buff = (char *)& packet ;

	while(totalLen <msgLen)
	{
		recvNum =recv(cfd,buff+totalLen,msgLen- totalLen,0);
		cout <<"recv NUm "<< recvNum<<endl ;
		if(recvNum<=0)
		{
			cout << strerror(errno)<<endl;
			cerr<<"0 socket recv msg error !\n ";
			return -1 ;
		}
		else 
			totalLen +=recvNum ;
	}

	msgLen = getPacketLen(packet);

	cout << "msgLen "<< msgLen <<endl;

	while(totalLen <msgLen)
	{

		recvNum =recv(cfd,buff+totalLen,msgLen-totalLen,0);
		cout << "totallen = "<< totalLen <<endl ;
		if(recvNum<=0)
		{
			cout << strerror(errno)<<endl;
			cerr<<"1 socket recv msg error !\n ";
			return -1 ;
		}
		else
			totalLen +=recvNum ;
	}

	cout << "totalLen  "<< totalLen <<endl;
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


	cout <<"login len = "<< getPacketLen(p)<<endl ; 
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


int alterPack( Packet &  desPack , Packet & srcPack , const char * srcId )
{
	if(srcPack.isMainType(mt::sndTxt))
		alterTxtPack(desPack , srcPack , srcId );
	else if (srcPack.isMainType(mt::sndFileHead))
		alterFileHeaderPack(desPack , srcPack , srcId );
	else if (srcPack.isMainType(mt::sndFile))
		alterFileDataPack(desPack , srcPack , srcId );
	else 
		return -1 ;  //不需要转发
	
	return 0 ;
}


