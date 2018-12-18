#include <iostream>
#include <fstream>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/file.h>//flock,lockf
#include <errno.h>
#include <signal.h>
#include <sys/prctl.h>//修改进程名
#include <queue>

#define MAXLENGTH 2048

 namespace MT
 {
	 // client  send 
	const unsigned char login = 0x11;
	const unsigned char sndFileHead = 0x12 ;
	const unsigned char sndFile = 0x13;
	const unsigned char sndTxt = 0X14

	const unsigned char resLogin = 0x71;
	const unsigned char resConf = 0X81 ;
	const unsigned char resSend = 0X72;
 };

namespace SBT
{
	const unsigned char request = 0x00 ;
	const unsigned char success = 0x01 ;
	const unsigned char changepwd = 0x02 ;

	const unsigned char txt = 0x01;
	const unsigned char jpg = 0x02;
	const unsigned char gif = 0x03 ;

	const unsigned char failed = 0x00;
	const unsigned char pwderror = 0x03 ;
	const unsigned char repeatout = 0x04 ;
	const unsigned char repeaton = 0x05 ;

	const unsigned char idNotExit = 0xfe;
	const unsigned char idOffline = 0xfd;
};

struct packetHeader{
	unsigned char mainType ;
	unsigned char subType ;
	unsigned short length;
};

struct msgPacket{
	packetHeader header ;
	char msg[MAXLENGTH+1024];
};

struct fileData{
	int fileId ;
	int count ;
	char data[MAXLENGTH];
};


int getPacketLen(const msgPacket & packet);

unsigned char getMainType (const msgPacket & packet);

unsigned char getSubType (const mysPacket & packet );


int fillPacketHeader(packetHeader & header , unsigned char mainType , unsigned char resType , unsigned short length);

int socketSend (int cfd , const msgPacket & packet);

int socketRecv(int cfd , msgPackt & packet);


//   clinet   以下所有的形参int cfd 都是client端自己的socket

//  client端接收数据包  输入自己的cfd 和  写数据包的变量
int clientRecv(int cfd , msgPacket & packet );

//  client端 发送登录请求  ， 输入用户名和密码
int sndLogin(int cfd , const char * username , const char * passwd );

//  client端发送文本消息， id为指定格式的用户名串  
//  用户名串的类型为 "/name1/name2/name3/name4/end"  代表想要发送给name1-4
//  或 "/@all"   代表想要发送给所有组员
int sndText(int cfd , const char * id , const char * text );

//  client端发送jpg  ， 传入指向图片文件的位置
int sndJPG (int cfd , const char * id , const void * jpgpath );

// client端发送gif  ， 形参意义同上
int sndGIF(int cfd , const char * id , const void * gifpath);

//  向个人好友发送文件 ， 形参意义同上
int sndFile (int cfd , const char * id , const void * filepath);

// client端 首次登录的更改密码  传入一个新的密码
int firstChangePwd(int cfd , const char * newPwd );


// server 

//server 接收packet
int serverRecv(int cfd , msgPacket & packet);

//server 发送数据（转发消息）
int serverSend (int cfd ,const msgPacket & packet );

//server 发送回应（只有报头信息，没有msg）
int sndResponse(int cfd , unsigned char maintype ,unsigned char subtype );


