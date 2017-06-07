#ifndef GLOBALDEF_H_
#define GLOBALDEF_H_

#include <QString>

#define  _Q2C(str) (str.toAscii().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))

const int IP_LEN = 32;
const int FILENAME_LEN = 256;
const int BUF_LEN = 4096;
const int READ_LEN = 512;
const int FIXMSG_LEN = 6;
const int STARTCODE = 0x68;
const int YCBASE = 462;
const int YCBAND = 40;
const int YCBEGIN = 0x4001;
const int YXBEGIN = 0x0001;
const int YCNUMPERFRAME = 100;
const int YXNUMPERFRAME = 100;
const int YCNUMPERSTORMFRAME = 32;
const int YXNUMPERSTORMFRAME = 64;
const int SOENUMBERSTORMFRAME = 18;
const int YCTYPE = 0x15;
const int YXTYPE = 0x01;
const int SOETYPE = 0x1E;

enum SENDCMD {SENDCHG, SENDNORMAL};

typedef struct  
{
	int clientNum;		//客户端数量
	int clientYxNum;	//每个客户端的遥信数量
	int clientYcNum;	//每个客户端的遥测数量
	int changeYcNum;	//每个客户端每次遥信变位次数
	int changeYxNum;	//每个客户端每次遥测变化次数
	int sendGap;		//发送间隔
	int startPort;		//首个客户端的端口
	int	countLimit;		//发送次数
}SETTINGS;

typedef struct
{
	int listenSock;	
	int linkSock;
	char ip[IP_LEN];

	int linkSock2;
	char ip2[IP_LEN];

	int clientChgYxCount;	//每个客户端发送的变位遥信总数
	int clientChgYcCount;	//每个客户端发送的变化遥测总数
	bool readyFlag;			//每个客户端准备接受风暴的标志
	bool stormFlag;			//每个客户端发送风暴的标志
	int sendCount;			//总发送次数
}CLIENTSTAT;

typedef struct
{
	ushort		 aMs;
	uchar   	 aSec;
	uchar 		 aMin;
	uchar		 aHour;
	uchar		 aDay;
	uchar   	 aMon;
	uchar   	 aYear;          //start 1900
}FETIME;
#endif