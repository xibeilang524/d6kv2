// scada.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "scada.h"
#include "verapi.h"

#include "MainFrm.h"
#include "db/dbapi.h"
#include "scadaproc.h"
#include "procthread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
DBApi dbapi;
extern SCADACHANPARA scadachanpara;
/////////////////////////////////////////////////////////////////////////////
// CScadaApp
BEGIN_MESSAGE_MAP(CScadaApp, CWinApp)
	//{{AFX_MSG_MAP(CScadaApp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

//进程定义全局变量
CScadaApp theApp;
HANDLE hThread[MAXTHREADNUM];	//线程对象


// CScadaApp construction
CScadaApp::CScadaApp()
{
	for(int i=0;i<MAXTHREADNUM;i++)hThread[i]=NULL;
}

//线程函数声明--BEGIN
extern UINT scadamailproc_level0(LPVOID);		//处理邮件(procdata.cxx)
extern UINT scadamailproc_level1(LPVOID);		//处理邮件(procdata.cxx)
extern UINT ykupdownproc(LPVOID);				//处理遥控遥调命令(procdata.cxx)
extern UINT accidentdealproc(LPVOID);			//事故处理(procdata.cxx)
extern UINT powereventproc(void *);			    //处理电力事项(procdata.cxx)
extern UINT timersavehisdataproc(LPVOID);		//定时存盘及初始化(savedata.cxx中)
extern UINT scada_timer_scheduler(void *);      //处理数据(procdata.cxx)
extern UINT proc_notify_event_scheduler(void*); //定时查找通知事项并加入实时库列表(procdata.cxx)
extern UINT proc_timer_task(void*);             //CheckParentproc
extern UINT scadamailproc_level2(LPVOID);        //处理前置数据邮件(procdata.cxx)
extern UINT changesaveproc(void*);              //变化存储
extern UINT proc_prot_resetting_check(void*);//保护信号未复归告警 , zl20140303
extern UINT TrendDBsaveproc(LPVOID);              //时序数据库存储
//线程函数声明--END



// 用于禁止同时运行同一进程的多个实例
/*
#pragma data_seg("Shared")
int nInstanceExist=0;
#pragma data_seg()

#pragma comment(linker,"-section:Shared,rws")
*/

void  *scada_main_win_handler;

HANDLE hSem = 0;

BOOL CScadaApp::InitInstance()
{
	if(__argc > 1) 
	{
		for(int i=1; i<__argc; i++)
		{
			if(strcmp("--version", __targv[i]) == 0)
			{
				printf("scdsvc,1.1");
				writever("scdsvc,1.1");
				return 0;
			}
		}
	}
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);// no debug window

	hSem = CreateSemaphore (NULL, 1, 1, "JustOnescdsrv");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle (hSem);	hSem = 0;
        return FALSE;
    }
	
	extern void scada_start_init(void);
	extern void SetScadaTimerTask(void);
	extern int IsScadaServer(void);
/*
    if(nInstanceExist>0) return FALSE;
	nInstanceExist=1;
*/
	//建立框架窗口

	pFrame=new CMainFrame;
	pFrame->LoadFrame(IDR_MAINFRAME);	
	pFrame->SetWindowText("Scada Service");

	m_pMainWnd=pFrame;
	ASSERT_VALID(this->m_pMainWnd);
	this->m_pMainWnd->SetWindowPos(0,0,0,600,45,SWP_SHOWWINDOW);
	pFrame->ShowWindow(SW_HIDE);
//	pFrame->ShowWindow(SW_SHOW);



	scada_main_win_handler=(void *)(m_pMainWnd->m_hWnd);

	ProcMan proc;
    if(!IsScadaServer()){
//        AfxMessageBox("本机不是SCADA服务器! 请将本机配置为SCADA服务器后运行!",MB_OK);
//        MessageBox("本机不是SCADA服务器! 请将本机配置为SCADA服务器后运行!", "请配置为SCADA服务器后运行!", MB_ICONINFORMATION|MB_OK);
        MessageBoxEx(m_pMainWnd->m_hWnd,"本机不是SCADA服务器! 请将本机配置为SCADA服务器后运行!", "请配置为SCADA服务器后运行!", MB_ICONWARNING|MB_OK,LANG_CHINESE);
		proc.EndProc(); 
		return FALSE;
	}
	proc.StartProc("scdsvc", "SCADA 服务", "", m_pMainWnd->m_hWnd);

	//启动历史数据库
// 	int m;
// 	for(m=0; m<4; m++) 
// 	{
	dbapi.Open(DB_OP_W,FALSE,TRUE); 
//			break;
// 		sleepms(2,0);
// 	}

// lhl 20110727 离库运行
// 	if(m>=4)
// 	{
// 		proc.EndProc(); 
// 		return FALSE;
// 	}
	
	//
	scada_start_init();
	SetScadaTimerTask();

	//启动所有线程；

	hThread[0] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)scadamailproc_level0, NULL, 100);		//邮件处理
	hThread[1] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)scadamailproc_level1, NULL, 100);		//邮件处理
	hThread[2] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)ykupdownproc,NULL,100);			        //遥控处理
	hThread[3] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)accidentdealproc,NULL,100);				//事故处理
	hThread[4] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)powereventproc,NULL,100);		        //事项处理
	hThread[5] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)timersavehisdataproc,NULL,100);		    //定时存盘线程
	hThread[6] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)scada_timer_scheduler,NULL,100);		    //定时存盘线程
	hThread[7] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)proc_notify_event_scheduler,NULL,100);	//定时处理通知事项线程
    hThread[8] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)proc_timer_task,NULL,30); //checkparentproc
	hThread[9] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)scadamailproc_level2, NULL, 100);		//邮件处理
	hThread[10] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)changesaveproc,NULL,100); //变化存储
	hThread[11] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)proc_prot_resetting_check,NULL,30); ////zl20140303 ,保护信号未复
	hThread[12] = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)TrendDBsaveproc,NULL,100); ////zxm20141008,TrendDB时序数据库存储
	return TRUE;
}

int CScadaApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
    CloseHandle (hSem);	hSem = 0;

    dbapi.Close();
 

	for(int i=0;i<MAXTHREADNUM;i++){
		if(hThread[i])TerminateThread(hThread[i],0);
	}

	ProcMan proc;
	proc.EndProc();  

	NetDbg  dbg;
	dbg.UnRegProc();
	return CWinApp::ExitInstance();
}


CScadaApp::~CScadaApp()
{
}

