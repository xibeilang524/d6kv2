///initialize.h
///程序启动起来的初始化代码
///
///主要进行程序注册，线程创建、数据库开启 、退出清理等工作
///
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _INITIALIZE_H_XWQ_20030514
#define _INITIALIZE_H_XWQ_20030514
#include <net/netapi.h>

class CInitialize : public NetDbg
{
public:
	CInitialize();
	~CInitialize();
	int beginInit();	
	int endCleanup();	//结束事项线程、停电管理进程、注销服务、停止数据流刷新
	int checkParentProc();	

private:

#ifdef _WINDOWS
	HANDLE m_hInstance;
#endif
	NetStatus netstate;
	ProcMan netmanage;
};
#endif