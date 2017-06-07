/*************************************************
Copyright (C),SHENZHEN SNR TECH.CO.,LTD
File name: dbitcp.h
Author: zhaolong
Version: 1.0.0.0
Date: 2011.2.15
Description: 数据库TCP通信头文件
Others: 
Function List: DBITcp
History: 
*************************************************/

#ifndef	___DBI_TCP_H
#define	___DBI_TCP_H


#include <signal.h>
class IMEXPORT DBITcp : public NetDbg 
{
public:
	DBITcp(void);			//for tcp client
	DBITcp(int socketid);	//for tcp server
	virtual ~DBITcp();
	virtual int Create();
	virtual int Connect(struct in_addr,int);
	virtual int Listen( int ,int backlog=SOMAXCONN); 
	virtual int	Accept(struct sockaddr *, int *);
	virtual int Send(void *,int);
	virtual int Recv(void *,int);
	virtual void Close(void);
	virtual int IsReadable(int sec = 0,int usec = 0);
	virtual int IsWritable(int sec = 0,int usec = 0);
	virtual int	GetSocket(void) { return socket_id; }
	virtual char SocketStatus();
	virtual int  IsValidAddr();
protected:
	int	socket_id;
	struct in_addr m_addr;
	int32s m_port;
};

#endif