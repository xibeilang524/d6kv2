#ifndef PROTOCOLTHREAD_H_
#define PROTOCOLTHREAD_H_

#include <QThread>
#include <QVector>
#include <QMap>
#include <QMutex>
#include <fstream>
using namespace std;
#include "globaldef.h"


typedef unsigned char BYTE;


enum PROTOCOLSTEP{UNINIT = 0xFF, RECVSTARTDT = 0x01, ANSSTARTDT = 0x02, RECVCALLALL = 0x03,
	  SENDALLYC = 0x04, SENDALLYX = 0x05, NORMAL = 0x06};



class QProtocolThread: public QThread
{
	Q_OBJECT
public:
	QProtocolThread(QWidget* o, int ID);
	void run(void);
	void init(void);
	void stop(void);

public:
	void addAStorm(void);


private:
	void cmdProc(void);
	void dataProc(void);
	void stormProc(void);
	void ansStartDt(void);
	void ansTestFr(void);
	void ansCallAll(void);
	void ansSetTime(void);
	void ansCallKwh(void);
	void sendAllDataOver(void);
	void sendAllYX(void);
	void sendAllYC(void);
	void sendRandYC(void);
	void sendStormYX(void);
	void sendStormYC(void);
	void sendStormSOE(void);
	int ReadPort(int sock, char *buf, int size, int retry);
	int WritePort(int sock, char *buf,int size, int retry);
	int ClosePort(int sockid);
	int _get_last_error();
	void getTime(FETIME *atime);
	int linkValid(void);
	int read(void);
	int write(void);
#if defined (WIN32)
	void gettimeofday(struct timeval *x,void *y);
#endif // WIN32

public:
	int		m_yxSendCount;
	int		m_soeSendCount;

private:
	int		m_ID;
	char	m_sendBuf[BUF_LEN];
	int		m_sendNum;
	char	m_recvBuf[BUF_LEN];
	int		m_recvNum;

	char	m_tmpBuf[BUF_LEN];

	PROTOCOLSTEP	m_step;
	QVector<BYTE>	m_yxList;
	QVector<int>	m_ycList;

	QMap<int, BYTE>	m_chgYxList;
	QMap<int, int>  m_chgYcList;
	fstream			m_saveStream;
	int				m_stormNum;
	QMutex			m_mutex;
	int				m_stop;
	int				m_lastSendYxNo;

};
#endif