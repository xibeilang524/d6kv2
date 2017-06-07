///initialize.h
///程序启动起来的初始化代码
///
///主要进行保证唯一实例，程序注册，数据库开启等工作，和退出清理工作
///
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _INITIALIZE_H
#define _INITIALIZE_H
#include <QThread>

#include <net/netapi.h>

class DaThread : public QThread 
{    
	Q_OBJECT	
public:    
	DaThread();    
protected:    
	void run();	
private:    
};

class EvtThread : public QThread 
{    
	Q_OBJECT	
public:    
	EvtThread();    
protected:    
	void run();	
private:    
};

class CInitialize : public NetDbg
{
public:
	CInitialize(){};
	~CInitialize(){};
// #ifdef _WINDOWS
// 	int BeginInit(CWnd* pWnd);
// #else
	int BeginInit();
// #endif
	int EndCleanup();
	int CheckParentProc();
	int IsMainDAServer();

private:
	DaThread *pDaThread; //线程
	EvtThread *pEvtThread; //线程


// #ifdef WIN32
// 	HANDLE m_hInstance;
// #endif
	NetStatus netstate;
#ifndef __PROGRAM_DEBUG_XXX
	ProcMan netmanage;
#endif
};


#endif