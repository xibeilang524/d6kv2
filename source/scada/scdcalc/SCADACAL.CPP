// scadacal.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "db/dbapi.h"
#include "net/netdef.h"
#include "scadacal.h"
#include "procthread.h"
#include "MainFrm.h"
#include "procthread.h"
#include "scadatime.h"
#include "verapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
DBApi dbapi;

/////////////////////////////////////////////////////////////////////////////
// CScadacalApp

BEGIN_MESSAGE_MAP(CScadacalApp, CWinApp)
	//{{AFX_MSG_MAP(CScadacalApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

HANDLE hThread[MAXTHREADNUM];	//线程对象

/////////////////////////////////////////////////////////////////////////////
// CScadacalApp construction

CScadacalApp::CScadacalApp()
{
	// TODO: add construction code here,
	for(int i=0;i<MAXTHREADNUM;i++)hThread[i]=NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScadacalApp object

CScadacalApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CScadacalApp initialization

extern UINT proc_timer_userproc(void *);
extern UINT proc_hdbtimer_userproc(void *);
extern UINT proc_stachange_userproc(void *);
extern UINT proc_sgalarm_userproc(void *);
extern UINT proc_formula_exec(void *);
extern int IsScadaServer(void);

// 用于禁止同时运行同一进程的多个实例
#pragma data_seg("Shared")
int nInstanceExist=0;
#pragma data_seg()

#pragma comment(linker,"-section:Shared,rws")


BOOL CScadacalApp::InitInstance()
{
	if(__argc > 1)
	{
		for(int i=1; i<__argc; i++)
		{
			if(strcmp("--version", __targv[i]) == 0)
			{
				printf("scdcalc,1.51.1");
				writever("scdcalc,1.51.1");
				return 0;
			}
		}
	}
	
	if(::FindWindow(NULL,"计算量处理")!=NULL)return FALSE;
	
	ProcMan proc;
//	if(proc.IsProcExist("scdcal"))return FALSE;

    if(nInstanceExist>0) return FALSE;
	nInstanceExist=1;

	pFrame=new CMainFrame;
	pFrame->LoadFrame(IDR_MAINFRAME);
	pFrame->SetWindowText("计算量处理");
	pFrame->ShowWindow(SW_HIDE);
	m_pMainWnd=pFrame;

	//proc.StartProc("scdcalc", "计算量处理进程", "supernb",pFrame->m_hWnd);
	//NetDbg  dbg;
	//dbg.RegProc("计算量处理进程");

	//启动历史数据库
	int m;

//	for(m=0;m<4;m++) {
	dbapi.Open(DB_OP_W,FALSE,TRUE);
//		sleepms(2,0);
//	}
//lhl 
// 	if(m>=4)
// 		return FALSE;
	proc.StartProc("scdcalc", "计算量处理进程", "supernb",pFrame->m_hWnd);
	NetDbg  dbg;
	dbg.RegProc("计算量处理进程");
    if(!IsScadaServer()){
        AfxMessageBox("本机不是SCADA服务器! 请将本机配置为SCADA服务器后运行!",MB_OK);
		proc.EndProc(); 
		return FALSE;
	}

/*
	AfxBeginThread(proc_timer_userproc,NULL);
	AfxBeginThread(proc_stachange_userproc,NULL);
	AfxBeginThread(proc_sgalarm_userproc,NULL);
	AfxBeginThread(proc_formula_exec,NULL);
	AfxBeginThread(proc_hdbtimer_userproc,NULL);
*/	
	hThread[0] =  scd_pthread_create((PTHREAD_CALLFUNC)proc_timer_userproc,NULL,300);  
	hThread[1] =  scd_pthread_create((PTHREAD_CALLFUNC)proc_stachange_userproc,NULL,100);
	hThread[2] =  scd_pthread_create((PTHREAD_CALLFUNC)proc_sgalarm_userproc,NULL,100);
	hThread[3] =  scd_pthread_create((PTHREAD_CALLFUNC)proc_formula_exec,NULL,100);
	hThread[4] =  scd_pthread_create((PTHREAD_CALLFUNC)proc_hdbtimer_userproc,NULL,100);


	pFrame->SetTimer(CHECKMAINPROC,2000, NULL); 
	return TRUE;
}


int CScadacalApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	dbapi.Close();

	for(int i=0;i<MAXTHREADNUM;i++){
		if(hThread[i]) TerminateThread(hThread[i],0);
	}
	NetDbg  dbg;
	dbg.UnRegProc();

	return CWinApp::ExitInstance();
}

CScadacalApp::~CScadacalApp() 
{
}
