// scdpdr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "scdpdr.h"
#include "pdr.h"
#include "MainFrm.h"
#include "procthread.h"
#include "db/dbapi.h"
#include "net/netdef.h"
#include "scadaproc.h"
#include "verapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
DBApi dbapi;
/////////////////////////////////////////////////////////////////////////////
// CScdpdrApp

BEGIN_MESSAGE_MAP(CScdpdrApp, CWinApp)
	//{{AFX_MSG_MAP(CScdpdrApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

CScdpdrApp theApp;
HANDLE hThread[MAXTHREADNUM];	//线程对象
/////////////////////////////////////////////////////////////////////////////
// CScdpdrApp construction

CScdpdrApp::CScdpdrApp()
{
	for(int i=0;i<MAXTHREADNUM;i++)hThread[i]=NULL;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScdpdrApp object


/////////////////////////////////////////////////////////////////////////////
// CScdpdrApp initialization
extern UINT PdrCheckStartMsg(void *argp);
extern UINT PdrCheckSaveTask(void *argp);
extern UINT PdrDataFreshTask(void *argp);
extern UINT PdrDataSaveToDB(void *argp);
extern UINT PdrParaUpdCheck(void *argp);
extern int IsScadaServer(void);


//用于禁止同时运行同一进程的多个实例
#pragma data_seg("Shared")
int nInstanceExist=0;
#pragma data_seg()

#pragma comment(linker,"-section:Shared,rws")

BOOL CScdpdrApp::InitInstance()
{
	if(__argc > 1)
	{
		for(int i=1; i<__argc; i++)
		{
			if(strcmp("--version", __targv[i]) == 0)
			{
				printf("scdpdr,1.51.1");
				writever("scdpdr,1.51.1");
				return 0;
			}
		}
	}
	
	ProcMan proc;
//	if(proc.IsProcExist("pdr"))return FALSE;

    if(nInstanceExist>0) return FALSE;
	nInstanceExist=1;

	if(::FindWindow(NULL,"事故追忆")!=NULL)return FALSE;


	pFrame=new CMainFrame;
	pFrame->LoadFrame(IDR_MAINFRAME);
	pFrame->SetWindowText("事故追忆");
	pFrame->ShowWindow(SW_HIDE);
	m_pMainWnd=pFrame;

	proc.StartProc("scdpdr", "事故追忆进程", "supernb",pFrame->m_hWnd);
	NetDbg  dbg;
	dbg.RegProc("事故追忆进程");

	//启动历史数据库
	int m;

//	for(m=0;m<4;m++) {
	dbapi.Open(DB_OP_W,FALSE,TRUE);
//		sleepms(2,0);
//	}

	//lhl
// 	if(m>=4)
// 		return FALSE;

/*
	AfxBeginThread(PdrDataFreshTask,NULL);
	AfxBeginThread(PdrCheckStartMsg,NULL);
	AfxBeginThread(PdrCheckSaveTask,NULL);
	AfxBeginThread(PdrDataSaveToDB,NULL);
	AfxBeginThread(PdrParaUpdCheck,NULL);
*/	

	//启动所有线程；
	hThread[0] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)PdrDataFreshTask, NULL, 100);
	hThread[1] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)PdrCheckStartMsg, NULL, 100);
	hThread[2] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)PdrCheckSaveTask, NULL, 100);
	hThread[3] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)PdrDataSaveToDB, NULL, 100);
	hThread[4] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)PdrParaUpdCheck, NULL, 100);


	pFrame->SetTimer(CHECKMAINPROC,2000, NULL); 
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

int CScdpdrApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	for(int i=0;i<MAXTHREADNUM;i++){
		if(hThread[i])TerminateThread(hThread[i],0);
	}
	
	NetDbg  dbg;
	dbg.UnRegProc();	
	return CWinApp::ExitInstance();
}

CScdpdrApp::~CScdpdrApp()
{
}
