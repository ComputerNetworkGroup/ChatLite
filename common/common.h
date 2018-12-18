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
#include <sys/prctl.h>//�޸Ľ�����
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


//   clinet   �������е��β�int cfd ����client���Լ���socket

//  client�˽������ݰ�  �����Լ���cfd ��  д���ݰ��ı���
int clientRecv(int cfd , msgPacket & packet );

//  client�� ���͵�¼����  �� �����û���������
int sndLogin(int cfd , const char * username , const char * passwd );

//  client�˷����ı���Ϣ�� idΪָ����ʽ���û�����  
//  �û�����������Ϊ "/name1/name2/name3/name4/end"  ������Ҫ���͸�name1-4
//  �� "/@all"   ������Ҫ���͸�������Ա
int sndText(int cfd , const char * id , const char * text );

//  client�˷���jpg  �� ����ָ��ͼƬ�ļ���λ��
int sndJPG (int cfd , const char * id , const void * jpgpath );

// client�˷���gif  �� �β�����ͬ��
int sndGIF(int cfd , const char * id , const void * gifpath);

//  ����˺��ѷ����ļ� �� �β�����ͬ��
int sndFile (int cfd , const char * id , const void * filepath);

// client�� �״ε�¼�ĸ�������  ����һ���µ�����
int firstChangePwd(int cfd , const char * newPwd );


// server 

//server ����packet
int serverRecv(int cfd , msgPacket & packet);

//server �������ݣ�ת����Ϣ��
int serverSend (int cfd ,const msgPacket & packet );

//server ���ͻ�Ӧ��ֻ�б�ͷ��Ϣ��û��msg��
int sndResponse(int cfd , unsigned char maintype ,unsigned char subtype );


