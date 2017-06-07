#include "protocolthread.h"
#include <stdio.h> 
#include <time.h> 
#include <iostream>
using namespace std;
#include <winsock2.h>
#include "globaldef.h"

#include <errno.h>


extern SETTINGS g_settings;
extern CLIENTSTAT* g_pClientStat;
extern int g_linkNum;

void savelog( char *buf, int id )
{
	return;
	FILE *fp;
	int i;
	char filename[60];
	sprintf(filename,"%s/log/dat/storm%d.txt",getenv("NBENV"), id );
	fp = fopen(filename,"a+");
	fprintf( fp, "%s\n", buf );
	fclose( fp );
}

QProtocolThread::QProtocolThread(QWidget* o, int ID)
:QThread()
{
	int i;
	m_ID = ID;
	m_step = UNINIT;
	m_stop = FALSE;
	m_lastSendYxNo = 0;
	m_yxList.resize(g_settings.clientYxNum);
	m_ycList.resize(g_settings.clientYcNum);

	srand((unsigned)time(NULL) + m_ID*1000);

	//使用随机数产生初始的遥测遥信值
	for (i = 0; i < g_settings.clientYcNum; i++)
	{
		m_ycList[i] = YCBASE + rand()%YCBAND;
	}

	for (i = 0; i < g_settings.clientYxNum; i++)
	{
		m_yxList[i] = 0/*rand()%2*/;
	}

	m_saveStream.open("log.txt", ios::out|ios::trunc);
	m_saveStream.clear();
	m_stormNum = 0;
	m_yxSendCount = 0;
	m_soeSendCount = 0;
}


void QProtocolThread::run(void)
{
	bool makeAStorm;
	while (!m_stop)
	{
		cout<<m_ID<<" is running"<<endl;
		cmdProc();

		m_mutex.lock();

		if (m_stormNum > 0)
		{
			makeAStorm = true;
			m_stormNum--;
		}
		else
		{
			makeAStorm = false;
		}

		m_mutex.unlock();

		if (makeAStorm)
		{
			stormProc();
		}
		else
		{
			dataProc();
		}

		msleep(1000);
	}
}

void QProtocolThread::init(void)
{
	m_recvNum = 0;
	m_sendNum = 0;
	g_pClientStat[m_ID].clientChgYcCount = 0;
	g_pClientStat[m_ID].clientChgYcCount = 0;
	g_pClientStat[m_ID].readyFlag = false;
	g_pClientStat[m_ID].stormFlag = false;
	m_step = UNINIT;
}

void QProtocolThread::cmdProc(void)
{
	int i = 0;

	if (FALSE == linkValid())
	{
		return;
	}

	m_recvNum = read();

	if (m_recvNum < 0)
	{
		return;
	}
	else if (m_recvNum < FIXMSG_LEN)
	{
		return;
	}

	while (i < m_recvNum)
	{
		if (m_recvBuf[i] == STARTCODE)
		{
			//启动帧
			if (m_recvBuf[i + 1] == 0x04 && m_recvBuf[i + 2] == 0x07)
			{
				m_step = ANSSTARTDT;
				memcpy(m_sendBuf, m_recvBuf + i, 6);
				ansStartDt();
				i += 6;
				break;
			}
			//测试帧
			else if (m_recvBuf[i + 1] == 0x04 && m_recvBuf[i + 2] == 0x43)
			{
				m_step = NORMAL;
				memcpy(m_sendBuf, m_recvBuf + i, 6);
				ansTestFr();
				i += 6;
				break;
			}
			//确认帧
			else if (m_recvBuf[i + 1] == 0x04 && m_recvBuf[i + 2] == 0x01
				&& m_recvBuf[i + 2] == 0x00)
			{
				i += 6;
				m_step = NORMAL;
			}
			//总召
			else if (m_recvBuf[i + 1] == 0x0E && m_recvBuf[i + 6] == 0x64
					&& m_recvBuf[i + 8] == 0x06)
			{
				m_step = RECVCALLALL;
				memcpy(m_sendBuf, m_recvBuf + i, 0x0E + 2);
				memcpy(m_tmpBuf, m_recvBuf + i, 0x0E + 2);

				ansCallAll();
				m_step = SENDALLYC;
				i += 0x0E + 2;
			}
			//电度召唤
			else if (m_recvBuf[i + 1] == 0x0E && m_recvBuf[i + 6] == 0x65
				&& m_recvBuf[i + 8] == 0x06)
			{
				memcpy(m_sendBuf, m_recvBuf + i, 0x0E + 2);
				ansCallKwh();
				i += 0x0E + 2;
			}
			//对时
			else if (m_recvBuf[i + 1] == 0x14 && m_recvBuf[i + 6] == 0x67
					&& m_recvBuf[i + 8] == 0x06)
			{	
				memcpy(m_sendBuf, m_recvBuf + i, 0x14 + 2);
				ansSetTime();
				i += 0x14 + 2;
			}
			else
			{
				i++;
			}
		}
		else
		{
			i++;
		}
	}//while (i < m_recvNum)
}

void QProtocolThread::ansStartDt(void)
{
	m_sendBuf[2] = 0x0B;

	if (FALSE == linkValid())
	{
		return;
	}

	m_sendNum = 6;
	write();
}

void QProtocolThread::ansTestFr(void)
{
	m_sendBuf[2] = 0x83;
	
	if (FALSE == linkValid())
	{
		return;
	}
	
	m_sendNum = 6;
	write();
}

void QProtocolThread::ansCallAll(void)
{
	m_sendBuf[8] = 0x07;
	
	if (FALSE == linkValid())
	{
		return;
	}
	
	m_sendNum = 16;
	write();
}

void QProtocolThread::ansCallKwh(void)
{
	m_sendBuf[8] = 0x0a;
	
	if (FALSE == linkValid())
	{
		return;
	}
	
	m_sendNum = 16;
	write();
}

void QProtocolThread::ansSetTime(void)
{
	m_sendBuf[8] = 0x07;
	
	if (FALSE == linkValid())
	{
		return;
	}
	
	m_sendNum = 22;
	write();
}

void QProtocolThread::sendAllDataOver(void)
{
	m_tmpBuf[8] = 0x0a;
	memcpy(m_sendBuf, m_tmpBuf, 16);
	
	if (FALSE == linkValid())
	{
		return;
	}
	
	m_sendNum = 16;
	write();
}


void QProtocolThread::dataProc(void)
{
	if (m_step == SENDALLYC)
	{
		sendAllYC();
		m_step = SENDALLYX;
		return;
	}
	else if (m_step == SENDALLYX)
	{
		sendAllYX();
		m_step = NORMAL;
		return;
	}
	else if (m_step == NORMAL)
	{
		sendRandYC();
	}
}

void QProtocolThread::sendAllYC(void)
{
	QVector<char> tmpMsg;
	char* buf;
	int startNo;
	int ycNo;
	int frameNum = (g_settings.clientYcNum + YCNUMPERFRAME - 1)/YCNUMPERFRAME;

	for (int i = 0; i < frameNum; i++)
	{
		tmpMsg.clear();

		//开始组帧
		tmpMsg.push_back(STARTCODE);
		tmpMsg.push_back(13 + YCNUMPERFRAME * 2);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);

		tmpMsg.push_back(YCTYPE);
		tmpMsg.push_back(0x80 + YCNUMPERFRAME);
		tmpMsg.push_back(0x14);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x01);
		tmpMsg.push_back(0x00);

		startNo = i * YCNUMPERFRAME + YCBEGIN;

		tmpMsg.push_back(startNo % 256);
		tmpMsg.push_back(startNo / 256);
		tmpMsg.push_back(0x00);

		for (int j = 0; j < YCNUMPERFRAME; j++)
		{
			ycNo = i * YCNUMPERFRAME + j;

			if (ycNo < g_settings.clientYcNum)
			{
				tmpMsg.push_back(m_ycList[ycNo] % 256);
				tmpMsg.push_back(m_ycList[ycNo] / 256);
			}
			else
			{
				tmpMsg.push_back(0x00);
				tmpMsg.push_back(0x00);
			}
		}//for (j = 0; j < YCNUMPERFRAME; j++)

		//组帧完成，发送报文
		buf = tmpMsg.data();
		memcpy(m_sendBuf, buf, tmpMsg.size());
		m_sendNum = tmpMsg.size();

		if (FALSE == linkValid())
		{
			return;
		}

		write();

		msleep(50);	
	}//for (int i = 0; i < frameNum; i++)
}

void QProtocolThread::sendAllYX(void)
{
	QVector<char> tmpMsg;
	int startNo;
	int yxNo;
	char* buf;
	int frameNum = (g_settings.clientYxNum + YXNUMPERFRAME - 1)/YXNUMPERFRAME;

	for (int i = 0; i < frameNum; i++)
	{
		tmpMsg.clear();
		
		//开始组帧
		tmpMsg.push_back(STARTCODE);
		tmpMsg.push_back(13 + YXNUMPERFRAME);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		
		tmpMsg.push_back(YXTYPE);
		tmpMsg.push_back(0x80 + YXNUMPERFRAME);
		tmpMsg.push_back(0x14);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x01);
		tmpMsg.push_back(0x00);
		
		startNo = i * YXNUMPERFRAME + YXBEGIN;
		
		tmpMsg.push_back(startNo % 256);
		tmpMsg.push_back(startNo / 256);
		tmpMsg.push_back(0x00);
		
		for (int j = 0; j < YXNUMPERFRAME; j++)
		{
			yxNo = i * YXNUMPERFRAME + j;

			if (yxNo < g_settings.clientYxNum)
			{
				tmpMsg.push_back(m_yxList[yxNo]);
				
			}
			else
			{
				tmpMsg.push_back(0x00);
			}
			
		}//for (j = 0; j < YXNUMPERFRAME; j++)
		
		//组帧完成，发送报文
		buf = tmpMsg.data();
		memcpy(m_sendBuf, buf, tmpMsg.size());
		
		m_sendNum = tmpMsg.size();
		
		if (FALSE == linkValid())
		{
			return;
		}
		
		write();
		
		msleep(50);	
	}//for (int i = 0; i < frameNum; i++)

	sendAllDataOver();
}

void QProtocolThread::sendRandYC(void)
{
	QVector<char> tmpMsg;

	srand((unsigned)time(NULL) + m_ID * 1000);
	int ycNo = rand()%g_settings.clientYcNum;
	int ycValue = YCBASE + rand()%YCBAND;
	char* buf;

	m_ycList[ycNo] = ycValue;

	tmpMsg.clear();
	
	//开始组帧
	tmpMsg.push_back(STARTCODE);
	tmpMsg.push_back(13 + 2);
	tmpMsg.push_back(0x00);
	tmpMsg.push_back(0x00);
	tmpMsg.push_back(0x00);
	tmpMsg.push_back(0x00);
	
	tmpMsg.push_back(YCTYPE);
	tmpMsg.push_back(0x01);
	tmpMsg.push_back(0x03);
	tmpMsg.push_back(0x00);
	tmpMsg.push_back(0x01);
	tmpMsg.push_back(0x00);
	
	tmpMsg.push_back((ycNo + YCBEGIN) % 256);
	tmpMsg.push_back((ycNo + YCBEGIN) / 256);
	tmpMsg.push_back(0x00);

	tmpMsg.push_back(ycValue % 256);
	tmpMsg.push_back(ycValue / 256);
	
	//组帧完成，发送报文
	buf = tmpMsg.data();
	memcpy(m_sendBuf, buf, tmpMsg.size());
	
	m_sendNum = tmpMsg.size();
	
	if (FALSE == linkValid())
	{
		return;
	}
	
	write();

//	msleep(1000);	
}

void QProtocolThread::stormProc(void)
{
	int i;
	int ycNo, yxNo;
	int ycValue;
	BYTE yxValue;

	srand((unsigned)time(NULL) + m_ID * 3000);

	m_chgYcList.clear();
	m_chgYxList.clear();

	//产生遥测变化的列表
	for (i = 0; i < g_settings.changeYcNum; i++)
	{
		do 
		{
			ycNo = rand()%g_settings.clientYcNum;
		} while (m_chgYcList.contains(ycNo));
		
		ycValue = YCBAND + rand()%YCBAND;
		m_ycList[ycNo] = ycValue;
		m_chgYcList.insert(ycNo, ycValue);
	}

	//产生遥信变位列表
	for (i = 0; i < g_settings.changeYxNum; i++)
	{
// 		do 
// 		{
// 			yxNo = rand()%g_settings.clientYxNum;
// 		} while (m_chgYxList.contains(yxNo));
		yxNo = m_lastSendYxNo;
		yxValue = (~m_yxList[yxNo]) & 0x01;
		m_yxList[yxNo] = yxValue;
		m_chgYxList.insert(yxNo, yxValue);
		if (m_lastSendYxNo >= (g_settings.clientYxNum - 1))
		{
			m_lastSendYxNo = 0;
		}
		else
		{
			m_lastSendYxNo++;
		}
	}

	sendStormYC();
	sendStormYX();
	sendStormSOE();
}

void QProtocolThread::sendStormYC(void)
{
	int i, j;
	int chgYCNum = m_chgYcList.size();
	int frameNum = (chgYCNum + YCNUMPERSTORMFRAME - 1)/YCNUMPERSTORMFRAME;
	int ycNo, ycValue;
	QVector<char> tmpMsg;
	QMap<int ,int>::const_iterator it = m_chgYcList.constBegin();

	for (i = 0; i < frameNum; i++)
	{
		tmpMsg.clear();
		
		//开始组帧
		tmpMsg.push_back(STARTCODE);
		tmpMsg.push_back(0x00);	//最后填写
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		
		tmpMsg.push_back(YCTYPE);
		tmpMsg.push_back(0x00);	//最后填写
		tmpMsg.push_back(0x03);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x01);
		tmpMsg.push_back(0x00);
		
		for (j = 0; j < YCNUMPERSTORMFRAME; j++)
		{
			ycNo = it.key();
			ycValue = it.value();

			if (it++ == m_chgYcList.constEnd())
			{
				break;
			}

			tmpMsg.push_back((ycNo + YCBEGIN) % 256);
			tmpMsg.push_back((ycNo + YCBEGIN) / 256);
			tmpMsg.push_back(0x00);
			tmpMsg.push_back(ycValue % 256);
			tmpMsg.push_back(ycValue / 256);
		}//for (j = 0; j < YCNUMPERFRAME; j++)

		//最后计算长度
		tmpMsg[1] = 10 + j * 5;
		tmpMsg[7] = j;
		
		//组帧完成，发送报文
		memcpy(m_sendBuf, tmpMsg.data(), tmpMsg.size());
		
		m_sendNum = tmpMsg.size();
		
		if (FALSE == linkValid())
		{
			//msleep(50);	
			return;
		}
		
		write();
		//msleep(50);	
	}//for (int i = 0; i < frameNum; i++)
}


void QProtocolThread::sendStormYX(void)
{
	int i, j;
	int chgYXNum = m_chgYxList.size();
	int frameNum = (chgYXNum + YXNUMPERSTORMFRAME - 1) / YXNUMPERSTORMFRAME;
	int yxNo;
	BYTE yxValue;
	QVector<char> tmpMsg;
	QMap<int ,BYTE>::const_iterator it = m_chgYxList.constBegin();


	for (i = 0; i < frameNum; i++)
	{
		tmpMsg.clear();
		
		//开始组帧
		tmpMsg.push_back(STARTCODE);
		tmpMsg.push_back(0x00);	//最后填写
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		
		tmpMsg.push_back(YXTYPE);
		tmpMsg.push_back(0x00); //最后填写
		tmpMsg.push_back(0x03);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x01);
		tmpMsg.push_back(0x00);
		
		for (j = 0; j < YXNUMPERSTORMFRAME; j++)
		{
			yxNo = it.key();
			yxValue = it.value();
			
			if (it++ == m_chgYxList.constEnd())
			{
				break;
			}
			
			tmpMsg.push_back((yxNo + YXBEGIN) % 256);
			tmpMsg.push_back((yxNo + YXBEGIN) / 256);
			tmpMsg.push_back(0x00);
			tmpMsg.push_back(yxValue);
		}//for (j = 0; j < YXNUMPERFRAME; j++)
		
		//最后计算长度
		tmpMsg[1] = 10 + j * 4;
		tmpMsg[7] = j;
		
		//组帧完成，发送报文
		memcpy(m_sendBuf, tmpMsg.data(), tmpMsg.size());
		
		m_sendNum = tmpMsg.size();
		
		if (FALSE == linkValid())
		{
			//msleep(50);	
			return;
		}
		
		write();
		//msleep(50);	
		m_yxSendCount++;
	}//for (int i = 0; i < frameNum; i++)

	char buf[ 156 ];
	sprintf( buf, "id=%d, 遥信帧%d", m_ID, m_yxSendCount );
	savelog( buf, m_ID );
}

void QProtocolThread::sendStormSOE(void)
{
	int i, j;
	int chgYXNum = m_chgYxList.size();
	int frameNum = (chgYXNum + SOENUMBERSTORMFRAME - 1) / SOENUMBERSTORMFRAME;
	int yxNo;
	BYTE yxValue;
	QVector<char> tmpMsg;
	QMap<int ,BYTE>::const_iterator it = m_chgYxList.constBegin();
	FETIME curTime;
	getTime(&curTime);
	
	for (i = 0; i < frameNum; i++)
	{
		tmpMsg.clear();
		
		//开始组帧
		tmpMsg.push_back(STARTCODE);
		tmpMsg.push_back(0x00);	//最后填写
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x00);
		
		tmpMsg.push_back(SOETYPE);
		tmpMsg.push_back(0x00); //最后填写
		tmpMsg.push_back(0x03);
		tmpMsg.push_back(0x00);
		tmpMsg.push_back(0x01);
		tmpMsg.push_back(0x00);
		
		for (j = 0; j < SOENUMBERSTORMFRAME; j++)
		{
			yxNo = it.key();
			yxValue = it.value();
			
			if (it++ == m_chgYxList.constEnd())
			{
				break;
			}
			
			tmpMsg.push_back((yxNo + YXBEGIN) % 256);
			tmpMsg.push_back((yxNo + YXBEGIN) / 256);
			tmpMsg.push_back(0x00);
			tmpMsg.push_back(yxValue);

			tmpMsg.push_back((curTime.aSec * 1000 + curTime.aMs) % 256);
			tmpMsg.push_back((curTime.aSec * 1000 + curTime.aMs) / 256);
			tmpMsg.push_back(curTime.aMin);
			tmpMsg.push_back(curTime.aHour);
			tmpMsg.push_back(curTime.aDay);
			tmpMsg.push_back(curTime.aMon);
			tmpMsg.push_back(curTime.aYear - 100);
		}//for (j = 0; j < YXNUMPERFRAME; j++)
		
		//最后计算长度
		tmpMsg[1] = 10 + j * 11;
		tmpMsg[7] = j;
		
		//组帧完成，发送报文
		memcpy(m_sendBuf, tmpMsg.data(), tmpMsg.size());
		
		m_sendNum = tmpMsg.size();
		
		if (FALSE == linkValid())
		{
			//msleep(50);	
			return;
		}
		
		write();
		//msleep(50);
		m_soeSendCount++;
	}//for (int i = 0; i < frameNum; i++)
}


int QProtocolThread::_get_last_error()
{
#if defined (WIN32)
    return GetLastError();
#elif defined (__unix)
	
#if   defined (__alpha)
    return _Geterrno();
#elif defined (__sun)
    return errno;
#elif defined (_AIX)
		  return errno;//thread
#endif
#endif
}


int QProtocolThread::ReadPort(int sock, char *buf, int size, int retry)
{
	int  rxn,rxntime=0;
	char *ptr = buf;
	int  rxnum=0;
	if(size<=0) 
	{
		return 0;
	}
	if(sock<=0)
	{
		return -1;
	}
	
	fd_set rd;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO( &rd );
	FD_SET( sock, &rd );
	int ret = select( sock + 1, &rd, NULL, NULL, &tv );
	if( ret <= 0 )
		return 0;
	
	rxn = recv(sock,ptr,size,0);
	if(rxn<0)
	{
		int err=_get_last_error();
		if(err==WSAEWOULDBLOCK || err==EINTR
#if defined(WIN32)				
			|| err == WSAECONNABORTED
#endif
#if !defined(WIN32)
			//				|| err==EAGAIN || err==ENOENT || err==EINVAL || err==EIO || err==150
			|| err==EAGAIN || err==ENOENT || err==EINVAL || err==EIO || err==EINPROGRESS || err==ENOTTY
#endif
			)
		{
			return rxnum;
		}
		else
		{
			
			return -1;
		}
	}
	else if(rxn==0)
	{
		printf( "rxn == 0\n" );
		return -1;
	}

	m_saveStream<<"Rx: ";
// 	char tmpStr[24];
// 	
// 	for (int i = 0; i < rxn; i++)
// 	{
// 		sprintf(tmpStr, "%02X", (BYTE)buf[i]);
// 		m_saveStream<<tmpStr<<" ";
// 	}
// 	m_saveStream<<endl;
	
	return rxn;
}

int QProtocolThread::WritePort(int sock, char *buf, int size, int retry)
{
	int txn,txntime=0;
	char *ptr=buf;
	int txnum=0;
	
	if(size<=0) return 0;
	if(sock<=0) return -1;
	
	fd_set wd;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO( &wd );
	FD_SET( sock, &wd );
	int ret = select( sock + 1, NULL, &wd, NULL, &tv );
	if( ret <= 0 )
		return 0;
	
	txn=send(sock,ptr,size,0);
	if(txn<0)
	{
		int err=_get_last_error();
		if(err==WSAEWOULDBLOCK || err==EINTR
#if defined(__unix)
			|| err==EAGAIN || err==ENOENT || err==EINVAL || err==EIO || err==150 || err==ENOTTY
#endif	
			)
		{
			return txnum;
		}
		else return -1;
	}

	m_saveStream<<"Tx: ";
	char tmpStr[24];
	
// 	for (int i = 0; i < txn; i++)
// 	{
// 		sprintf(tmpStr, "%02X", (BYTE)buf[i]);
// 		m_saveStream<<tmpStr<<" ";
// 	}
// 	m_saveStream<<endl;
	
	return txn;
}

int QProtocolThread::ClosePort(int sockid)
{
	if(sockid<0) return FALSE;
	
	int   ret;
	for(int i=0; i<10; i++)
	{
#if defined (WIN32)
		unsigned long block_flag = FALSE;
		ioctlsocket(sockid,FIONBIO,&block_flag);
		ret= closesocket(sockid);
#elif defined (__unix)
		fcntl(sockid,F_SETFL,0);
		ret=close(sockid);
#endif
		if(ret==0) return TRUE;
	}
	return FALSE;
}


void QProtocolThread::getTime(FETIME *atime)
{
	struct tm *ttm;        
	time_t	ti;
	int		ms;
	struct timeval ttv;
    gettimeofday( &ttv ,NULL);
	ms = (ushort) (ttv.tv_usec/1000);
	time((time_t*)&ti);
	ttm = localtime(&ti);
    atime->aMs = ms;
    atime->aSec = (uchar) ttm->tm_sec;
    atime->aMin = (uchar) ttm->tm_min;
    atime->aHour = (uchar) ttm->tm_hour;
    atime->aDay = (uchar) ttm->tm_mday;
    atime->aMon = (uchar) ttm->tm_mon+1;
    atime->aYear = (uchar) ttm->tm_year;
}


#if defined (WIN32)
void QProtocolThread::gettimeofday(struct timeval *x,void *y)
{
	
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	x->tv_sec = time(0);
	x->tv_usec = systime.wMilliseconds*1000;
}
#endif


void QProtocolThread::addAStorm(void)
{
	m_mutex.lock();
// 	m_stormNum = g_settings.clientYxNum/g_settings.changeYxNum;
	m_stormNum = 1;
	cout<<m_ID<<" add a storm"<<endl;
	m_mutex.unlock();
}

int QProtocolThread::linkValid(void)
{
	if (g_pClientStat[m_ID].linkSock > 0)
	{
		return TRUE;
	}
	else if (g_pClientStat[m_ID].linkSock2 > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int QProtocolThread::read(void)
{
	int recvNum;

	if (g_pClientStat[m_ID].linkSock < 0 && g_pClientStat[m_ID].linkSock2 < 0)
	{
		return -1;
	}

	if (g_pClientStat[m_ID].linkSock > 0)
	{
		recvNum = ReadPort(g_pClientStat[m_ID].linkSock, m_recvBuf, READ_LEN, 1);

		if (recvNum > 0)
		{
			return recvNum;
		}
		else if (recvNum < 0)
		{
			ClosePort(g_pClientStat[m_ID].linkSock);
			g_pClientStat[m_ID].linkSock = -1;
			g_linkNum--;
		}
	}//if (g_pClientStat[m_ID].linkSock > 0)

	if (g_pClientStat[m_ID].linkSock2 > 0)
	{
		recvNum = ReadPort(g_pClientStat[m_ID].linkSock2, m_recvBuf, READ_LEN, 1);
		
		if (recvNum > 0)
		{
			return recvNum;
		}
		else if (recvNum < 0)
		{
			ClosePort(g_pClientStat[m_ID].linkSock2);
			g_pClientStat[m_ID].linkSock2 = -1;
			g_linkNum--;
		}
	}//if (g_pClientStat[m_ID].linkSock2 > 0)	

	return recvNum;
}

int QProtocolThread::write(void)
{
	if (g_pClientStat[m_ID].linkSock < 0 && g_pClientStat[m_ID].linkSock2 < 0)
	{
		return -1;
	}

	if (g_pClientStat[m_ID].linkSock > 0)
	{
		if (WritePort(g_pClientStat[m_ID].linkSock, m_sendBuf, m_sendNum, 1) < 0)
		{
			savelog( "发送失败!", m_ID );
			ClosePort(g_pClientStat[m_ID].linkSock);
			g_pClientStat[m_ID].linkSock = -1;
			g_linkNum--;
		}
	}//if (g_pClientStat[m_ID].linkSock > 0)

	if (g_pClientStat[m_ID].linkSock2 > 0)
	{
		if (WritePort(g_pClientStat[m_ID].linkSock2, m_sendBuf, m_sendNum, 1) < 0)
		{
			savelog( "发送失败!", m_ID );
			ClosePort(g_pClientStat[m_ID].linkSock2);
			g_pClientStat[m_ID].linkSock2 = -1;
			g_linkNum--;
		}
	}//if (g_pClientStat[m_ID].linkSock > 0)

	if (g_pClientStat[m_ID].linkSock < 0 && g_pClientStat[m_ID].linkSock2 < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

void QProtocolThread::stop(void)
{
	m_stop = true;	
}


