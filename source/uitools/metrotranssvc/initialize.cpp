
#include "stdafx.h"

#include "db/dbapi.h"
#include "globaldef.h"
// #include "SignalCtl.h"

#include "initialize.h"
#include "fileinfo.h"

#ifndef WIN32
#include "netdef.h"
#include <stdlib.h>
#endif

extern CFileInfo fileInfo;

extern int GrpNo;
// extern int bDebug;

int CInitialize::BeginInit()
{
	if( RegProc("滨州演示系统 服务") == FALSE )
	{
		printf( "程序注册失败，或已有程序运行！" );
		exit(0);
		return 55;
	}

// #ifdef WIN32
// 	m_hInstance = ::CreateMutex(NULL,FALSE,"bzdemo进程");
// 	if(GetLastError() == ERROR_ALREADY_EXISTS)
// 	{
// 		CloseHandle(m_hInstance);
// 		m_hInstance = 0;
// 		return 55;
// 	}
// #endif
	int grpnum = netstate.GetHostGrpNum("DA");
	printf("\n datasrv grpnum = %d",grpnum);

	if(grpnum == 0)
		return 1;
	if(grpnum != 1)
		return 2;
	GrpNo = *netstate.GetHostGrpEnm("DA");

	char localhost[CODE_LEN];
	_get_host_name(localhost);
	if(netstate.GetHostRstate(GrpNo,localhost) <= HOST_STATUS_STOP)
		return 55;

#ifndef __PROGRAM_DEBUG_XXX
// #ifdef WIN32
// 	netmanage.StartProc("bzdemo","bzdemo服务进程","",pWnd->m_hWnd);
// #else
	netmanage.StartProc("bzdemo","bzdemo服务进程","");
// #endif
#endif

	if(fileInfo.Init() < 0)
		return 3;
	if(fileInfo.ReadLineFaultPara() < 0)
		return 4;

// #ifndef _WINDOWS
// 	sigset(DATA_CHANGED,ReadStreamData);
// 	//sigset(SIGUSR1,SIG_IGN);
// 	sigset(END_PROC_SIGNAL,DaQuit);
// 	if(Stream.InitStreamPara() != 0)
// 		return 5;
// #else
// 	if(Stream.InitStreamPara(pWnd) != 0)
// 		return 5;
// #endif

	////启动线程
	pDaThread = new DaThread(); //线程
	pDaThread->start();
	if (!pDaThread->isRunning())
	{
		return	6;
	}

	pEvtThread = new EvtThread(); //线程
	pEvtThread->start();
	if (!pEvtThread->isRunning())
	{
		return	7;
	}
	return 0;
}

int CInitialize::EndCleanup()
{
	if(pDaThread->isRunning())
	{
		pDaThread->exit();
	}
	if(pEvtThread->isRunning())
	{
		pEvtThread->exit();
	}
	return 0;
}

int CInitialize::CheckParentProc()
{
#ifndef __PROGRAM_DEBUG_XXX
	if( netmanage.CheckParentProc() == 0)
		return FALSE;
	else 
		return TRUE;
#else
	return TRUE;
#endif
}

int CInitialize::IsMainDAServer()
{
	if(netstate.IsMainServer(GrpNo,"DA") == 0)
		return FALSE;
	else
		return TRUE;
}


DaThread::DaThread() 
{    
}

void DaThread::run() 
{
	DaProc();
}

EvtThread::EvtThread() 
{    
}

void EvtThread::run() 
{
	EvtProc();
}