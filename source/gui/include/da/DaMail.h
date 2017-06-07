//
//邮件接收管理类
//
//////////////////////////////////////////////////////////
#ifndef _DAMAIL_H_XWQ20030523_
#define _DAMAIL_H_XWQ20030523_
#include "netapi.h"
#include "GlobalDef.h"

class CDaMail
{
	//Functions
public:
	CDaMail();
	~CDaMail();
	int Initialize();
	int ReceiveMail();
	void GetFeederDealMailNum(int& Num);
	
	void Explain(BOOL bFlag = TRUE);//邮件解释函数
private:
	MailBox mailServer;
	BOOL bUsing;
	void Clear();
	int GetBrkAddr(int brkno);
	//Variables
public:
	SYS_TIME m_timeLastFault;
	SYS_TIME m_timeFirstFault;

private:
	int m_iMailNum;
	EX_REPORT_STRU* m_pRcvMail;
};

#endif