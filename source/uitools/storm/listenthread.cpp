#include "listenthread.h"
#include "globaldef.h"
#include <winsock.h>
#include <iostream>
using namespace std;

extern SETTINGS g_settings;
extern CLIENTSTAT* g_pClientStat;
extern int g_stop;
extern int g_linkNum;
extern int g_lintenNum;


QListenThread::QListenThread()
:QThread()
{

}


void QListenThread::run(void)
{
	int i;
	
	struct sockaddr_in des;
	struct sockaddr_in cliaddr;
	int port;
	unsigned long block_flag;
	
	memset(&des, 0, sizeof(des));
	des.sin_addr.s_addr = htonl(INADDR_ANY);
	des.sin_family = AF_INET;
	block_flag = TRUE;
	int ret;
	
	for(i = 0; i < g_settings.clientNum; i ++)
	{
		port = g_settings.startPort + i;
		des.sin_port = htons(port);
		g_pClientStat[i].listenSock = socket(AF_INET, SOCK_STREAM, 0);;
		ret = ioctlsocket(g_pClientStat[i].listenSock, FIONBIO, &block_flag);
		if(ret < 0)
		{
			closesocket(g_pClientStat[i].listenSock);
			g_pClientStat[i].listenSock = -1;
			continue;
		}
		ret = bind(g_pClientStat[i].listenSock, (sockaddr *)&des, sizeof(des));
		if(ret < 0)
		{
			closesocket(g_pClientStat[i].listenSock);
			g_pClientStat[i].listenSock = -1;
			continue;
		}
		ret = listen(g_pClientStat[i].listenSock, SOMAXCONN);
		if(ret < 0)
		{
			closesocket(g_pClientStat[i].listenSock);
			g_pClientStat[i].listenSock = -1;
			continue;
		}
		g_lintenNum++;
	}
	
	timeval tv;
	fd_set rset;
	int clilen;
	
	
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	
	while(!g_stop)
	{
		for(i = 0; i < g_settings.clientNum; i ++)
		{
			FD_ZERO(&rset);
			FD_SET((unsigned int)g_pClientStat[i].listenSock, &rset);
			
			ret = select(g_pClientStat[i].listenSock+1, &rset, NULL, NULL, &tv);
			if(ret > 0)
			{
// 				if(g_pClientStat[i].linkSock > 0)
// 				{
// 					block_flag = FALSE;
// 					ioctlsocket(g_pClientStat[i].linkSock, FIONBIO, &block_flag);
// 					closesocket(g_pClientStat[i].linkSock);
// 					g_linkNum--;
// 				}

				if (g_pClientStat[i].linkSock < 0)
				{
					clilen = sizeof(struct sockaddr);
					g_pClientStat[i].linkSock = accept(g_pClientStat[i].listenSock, 
						(struct sockaddr*)&cliaddr, &clilen);
					strcpy(g_pClientStat[i].ip, inet_ntoa(cliaddr.sin_addr));
					if(g_pClientStat[i].linkSock < 0)		
						continue;
					block_flag = TRUE;
					
					ioctlsocket(g_pClientStat[i].linkSock,FIONBIO,&block_flag);
					g_linkNum++;
				}
				else if (g_pClientStat[i].linkSock2 < 0)
				{
					clilen = sizeof(struct sockaddr);
					g_pClientStat[i].linkSock2 = accept(g_pClientStat[i].listenSock, 
						(struct sockaddr*)&cliaddr, &clilen);
					strcpy(g_pClientStat[i].ip2, inet_ntoa(cliaddr.sin_addr));
					if(g_pClientStat[i].linkSock2 < 0)		
						continue;
					block_flag = TRUE;
					
					ioctlsocket(g_pClientStat[i].linkSock2,FIONBIO,&block_flag);
					g_linkNum++;
				}
			}
		}
		msleep(100);
	}
}

void QListenThread::stop(void)
{
	g_stop = 1;
	int i;
	unsigned long block_flag = FALSE;
	for(i = 0; i < g_settings.clientNum; i ++)
	{
		if(g_pClientStat[i].listenSock > 0)
		{
			ioctlsocket(g_pClientStat[i].listenSock, FIONBIO, &block_flag);
			closesocket(g_pClientStat[i].listenSock);
			g_pClientStat[i].listenSock = -1;
			g_lintenNum--;
		}
		
		if(g_pClientStat[i].linkSock > 0)
		{
			ioctlsocket(g_pClientStat[i].linkSock, FIONBIO, &block_flag);
			closesocket(g_pClientStat[i].linkSock);
			g_pClientStat[i].linkSock = -1;
			g_linkNum--;
		}

		if(g_pClientStat[i].linkSock2 > 0)
		{
			ioctlsocket(g_pClientStat[i].linkSock2, FIONBIO, &block_flag);
			closesocket(g_pClientStat[i].linkSock2);
			g_pClientStat[i].linkSock2 = -1;
			g_linkNum--;
		}
	}
}