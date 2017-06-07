// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "MainFrm.h"
#include "scada.h"
#include "net/netdef.h"
//#include "afertscd/law.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//add for test
#include "scadaproc.h"
#include "SelectDlg.h"
#include "SelectYxDlg.h"
#include "scd/scdapi.h" 
//#include "afertscd/afert.h" 
//end

#define ScadaTestName	"sa"
#define ScadaTestPassWord	"sac"
#define TestAnalogName  "Ksys110P"
#define TestBreakName   "Klsgb108Kg"
#define TestBreakDevName   "Kzxb1239"

#define TestProtsigName "test0prot0"
#define TestStatusName  "test0status0"
#define TestSwitchName  "test0switch0"
#define TestLineName    ""
#define TestLineReplaceName  ""

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
NOTIFYICONDATA	nid;
#define SCADA_CLICK1		WM_USER+1101

//extern COMPARA   compara;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_MESSAGE(PROC_CHECK, OnProcCheck)
	ON_MESSAGE(RDB_EVENT_MESG, OnRdbEvent)
	ON_WM_TIMER()
	ON_MESSAGE(SCADA_CLICK1,onclick)
	ON_COMMAND(IDM_OLEVENT_TEST, OnOleventTest)
	ON_COMMAND(IDM_SGF,OnSgfTest)
	ON_COMMAND(IDM_STAEVENT_TEST, OnStaChgTest)
	ON_MESSAGE(RAWD_MESSAGE, OnRawDataReady)
	ON_MESSAGE(MAIL_MESSAGE, OnMessageReceived)
	ON_COMMAND(IDM_STARTPDR, OnStartpdrTest)
	ON_COMMAND(IDM_TESTTERMINALSOE, OnTestTERMINALSOE)
	ON_COMMAND(IDM_TESTSERIALYK, OnTestserialyk)
	ON_COMMAND(IDM_TESTYK, OnTestyk)
	ON_COMMAND(IDM_REPLACE, OnReplace)
	ON_COMMAND(IDM_CLEARREPLACE, OnClearreplace)
	ON_COMMAND(IDM_CLEARCHGSTATUS, OnClearChgStatus)
	ON_COMMAND(IDM_TESTDAS, OnTestdas)
	ON_COMMAND(IDM_MENUITEM_testyx, OnMENUITEMtestyx)
	ON_COMMAND(IDM_PROTECT, OnISAProtectEvent)
	ON_COMMAND(IDM_PROTECT2, OnSELProtectEvent)
	ON_COMMAND(IDM_MASKENT, OnMaskent)
	ON_COMMAND(IDM_CLEARMASKENT, OnClearmaskent)
	ON_COMMAND(ID_STARTPDR, OnStartpdr)
	ON_COMMAND(IDM_CCFORMULA, OnCcformula)
	ON_COMMAND(IDM_TESTGROUPYK, OnTestgroupyk)
	ON_COMMAND(IDM_TESTHOSTSOE, OnTestHOSTSOE)
	ON_COMMAND(IDM_TESTUPDOWN, OnTestUpdown)
	ON_COMMAND(IDM_YXSOE, OnTestYxSOE)
	ON_COMMAND(IDM_PROTECT_DETECT, OnISAProtectDetect)
	ON_WM_ACTIVATE()
	ON_COMMAND(IDM_PROT_LFP_EVENT, OnProtLfpEvent)
	ON_COMMAND(IDM_PROT_LFP_DETECT, OnProtLfpDetect)
	ON_COMMAND(IDM_TESTMANSET, OnTestmanset)
	ON_COMMAND(IDM_TESTCLEARMANSET, OnTestclearmanset)
	ON_COMMAND(IDM_YKEXE, OnYkexe)
	ON_COMMAND(IDR_YKKF, OnYkkf)
	ON_COMMAND(IDR_YKKFZX, OnYkkfzx)
	ON_COMMAND(IDM_SETGROUND, OnSetground)
	ON_COMMAND(IDM_CLEARGROUND, OnClearground)
	ON_COMMAND(IDM_ONCHECK, OnOncheck)
	ON_COMMAND(IDM_CLEARCHECK, OnClearcheck)
	ON_COMMAND(IDM_TESTWFBS, OnTestwfbs)
	ON_COMMAND(IDM_CANCELWFBS, OnCancelwfbs)
	ON_COMMAND(IDM_TESTMULTIHOSTSOE, OnTestmultihostsoe)
	ON_COMMAND(IDM_OLRAWEVENT_TEST, OnOlraweventTest)
	ON_COMMAND(IDM_NETSENDYC, OnNetsendyc)
	ON_COMMAND(IDM_TESTYT, OnTestyt)
	ON_COMMAND(IDM_NETSENDYX, OnNetsendyx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};



CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{

}

Rdb_EventWithUser  rdbevent;

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
//SCADA FOR TEST
/* 
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *this;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	nid.uCallbackMessage = SCADA_CLICK1;
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(nid.szTip,"SCADA处理主进程");
	Shell_NotifyIcon(NIM_ADD,&nid);
*/	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
 
	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);// no debug window
	

//	ProcMan proc;
//	proc.StartProc("scdsvc", "SCADA处理主进程", "", m_hWnd);
	
	rdbevent.Register(m_hWnd, 1, MyUserName, MyPassWord);
	rdbevent.RdbNotifyEventType(RDB_EVENT_LOADTAB);

    //CreateAllMem(&compara);
	NetStatus netstat;  //add
    int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	for(int i=0; i<grpnum; i++)
	{
		//CreateShmMem(host_group_no[i],&compara);
	}
	NetDbg  dbg;
	dbg.RegProc("SCADA处理主进程");

//	SetTimer(CHECKMAINPROC, 1000, NULL); 
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszName = SCD_SERVER;
//	cs.style =WS_POPUP | WS_DISABLED;
//SCADA FOR TEST
/*
	cs.style = WS_POPUP | WS_MINIMIZE;	//使主窗口不可见最小化
	cs.dwExStyle |=WS_EX_TOOLWINDOW;	//不显示任务按钮
	cs.dwExStyle |=WS_EX_TOPMOST;	
*/	
    cs.lpszClass = AfxRegisterWndClass (CS_NOCLOSE, 0, 0, AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	return CFrameWnd::PreCreateWindow(cs);
}

LRESULT CMainFrame::onclick(WPARAM wParam,LPARAM lParam)
{
//SCADA FOR TEST
/*	static int flag=FALSE;
	int msg=(UINT) lParam;
	if (flag) return;

	int id = (UINT)wParam;
	if(id !=IDR_MAINFRAME) return ;

	if(msg != WM_LBUTTONDOWN) return;
	flag=TRUE;
	if(MessageBox("确定退出吗?","SCADA主进程",MB_YESNO)==IDYES)
		::PostMessage(*this,WM_CLOSE,0,0);
	flag=FALSE;
	ShowWindow(SW_HIDE);
*/
	return 0;
}
 

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	ShowWindow(SW_HIDE);
//	ShowWindow(SW_SHOW);
	// TODO: Add your message handler code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnClose() 
{
	rdbevent.RdbUnReg();

//SCADA FOR TEST
//	Shell_NotifyIcon(NIM_DELETE,&nid);
	
	CFrameWnd::OnClose();
	
}

LRESULT CMainFrame::OnProcCheck(WPARAM,LPARAM)
{
	return 0;
}

LRESULT CMainFrame::OnRdbEvent(WPARAM,LPARAM)
{

	char send_username[16];
	RDB_EVENT_LOADTAB_STRU *event_msg;
	int  event_len;

	RDB_DATETIME_MS_STRU  rdbclock;
	int eventtype;

	do {
		eventtype = rdbevent.RdbGetEvent(send_username, &rdbclock, (void **)&event_msg, &event_len);
		if(eventtype == RDB_EVENT_LOADTAB)
		{
			if(strcmp(event_msg->tabname,SDPARA_TABNAME)==0 ||
				strcmp(event_msg->tabname,DAYSDPARA_TABNAME)==0) 
			{
					//创建时段参数
					UpdateTimePeriodParaTab();
					FreshCurrentTimePeriodType();
			}
		}
	} while(eventtype==RDB_EVENT_LOADTAB);
	return 0;
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
/*	if(nIDEvent==CHECKMAINPROC)
	{
		ProcMan proc;
		if(!proc.CheckParentProc())PostMessage(WM_QUIT);
	}*/
	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnRawDataReady(WPARAM,LPARAM)
{
	return 0;
}


LRESULT CMainFrame::OnMessageReceived(WPARAM,LPARAM)
{
	return 0;
}



//////////////////////////////////---------------------------------------------------------
//------------------------------          测试开始           ------------------------------
//-----------------------------------------------------------------------------------------
//操作测试一-------------------------

//旁路替代开始
void CMainFrame::OnReplace() 
{
	ScadaServer scdserv;
	scdserv.SendLineReplaceReq(1,ScadaTestName,"Kfh2800Kg","Kfh2801Kg",1);
}

//旁路替代结束
void CMainFrame::OnClearreplace() 
{
	ScadaServer scdserv;
	scdserv.SendLineReplaceReq(1,ScadaTestName,"Kfh2800Kg","Kfh2801Kg",0);
}



////////////////////////////
//操作测试二------------------------

//清除变位测试
void CMainFrame::OnClearChgStatus()
{
	ScadaServer scdserv;
	scdserv.SendClrChgFlagReq(1,ScadaTestName, "lsgb");
}

//置封锁生数据事项标志
void CMainFrame::OnMaskent() 
{
	ScadaServer scdserv;
	char *name=TestBreakName;		//遥信对象名
	char isstation=0;
	char setflag=1;
	scdserv.SendMaskChgEventReq(0,ScadaTestName, name, isstation, setflag);
}

//清封锁生数据事项标志
void CMainFrame::OnClearmaskent() 
{
	ScadaServer scdserv;
	char *name=TestBreakName;		//遥信对象名
	char isstation=0;
	char setflag=0;
	scdserv.SendMaskChgEventReq(0,ScadaTestName, name, isstation, setflag);

}

//系统事件测试-------------------------------
//遥测越限事项
void CMainFrame::OnOleventTest() 
{
	// TODO: Add your command handler code here
	ENT_OVERLIMIT_STRU olapent;
	APEvent  apevent;

	strcpy(olapent.tabname,ANALOG_TABNAME);
	strcpy(olapent.objectname,TestAnalogName);
	olapent.olgrade=1;
	olapent.olvalue=49.8f;
	apevent.AddApEvent(APEVENT_OVERLIMIT,&olapent,sizeof(olapent));
}

//异常变位事项测试
void CMainFrame::OnStaChgTest() 
{
	// TODO: Add your command handler code here
	ENT_STATUSCHG_STRU stachgapent;
	APEvent  apevent;

	memset(&stachgapent,0,sizeof(ENT_STATUSCHG_STRU));
	strcpy(stachgapent.tabname,STATUS_TABNAME);
	strcpy(stachgapent.objectname,TestStatusName);
	strcpy(stachgapent.substation,"测试站0");
	GetCurrentClock(&stachgapent.chgclock);
	stachgapent.oddflag=1;
	stachgapent.staval=1;
	apevent.AddApEvent(APEVENT_STACHG,&stachgapent,sizeof(stachgapent));
}

//遥信SOE事项
void CMainFrame::OnTestYxSOE() 
{
	// TODO: Add your command handler code here
	ENT_STATUSCHG_STRU stachgapent;
	APEvent  apevent;

	memset(&stachgapent,0,sizeof(ENT_STATUSCHG_STRU));
	strcpy(stachgapent.tabname,BREAK_TABNAME);
	strcpy(stachgapent.objectname,TestBreakName);
	strcpy(stachgapent.substation,"测试站0");
	GetCurrentClock(&stachgapent.chgclock);
	stachgapent.oddflag=0;
	stachgapent.staval=1;
	apevent.AddApEvent(APEVENT_STASOE,&stachgapent,sizeof(stachgapent));

	memset(&stachgapent,0,sizeof(ENT_STATUSCHG_STRU));
	strcpy(stachgapent.tabname,SWITCH_TABNAME);
	strcpy(stachgapent.objectname,TestSwitchName);
	strcpy(stachgapent.substation,"测试站0");
	GetCurrentClock(&stachgapent.chgclock);
	stachgapent.oddflag=0;
	stachgapent.staval=1;
	apevent.AddApEvent(APEVENT_STASOE,&stachgapent,sizeof(stachgapent));

	memset(&stachgapent,0,sizeof(ENT_STATUSCHG_STRU));
	strcpy(stachgapent.tabname,STATUS_TABNAME);
	strcpy(stachgapent.objectname,TestStatusName);
	strcpy(stachgapent.substation,"测试站0");
	GetCurrentClock(&stachgapent.chgclock);
	stachgapent.oddflag=0;
	stachgapent.staval=1;
	apevent.AddApEvent(APEVENT_STASOE,&stachgapent,sizeof(stachgapent));

	memset(&stachgapent,0,sizeof(ENT_STATUSCHG_STRU));
	strcpy(stachgapent.tabname,PROT_TABNAME);
	strcpy(stachgapent.objectname,TestProtsigName);
	strcpy(stachgapent.substation,"测试站0");
	GetCurrentClock(&stachgapent.chgclock);
	stachgapent.oddflag=0;
	stachgapent.staval=1;
	apevent.AddApEvent(APEVENT_STASOE,&stachgapent,sizeof(stachgapent));

}

//遥信状态正常变位事项测试
void CMainFrame::OnMENUITEMtestyx() 
{
	// TODO: Add your command handler code here
	APEvent	apevent;
	SYSTEMTIME	systime;
	ENT_STATUSCHG_STRU stachgmsg;
	stachgmsg.substation[0]=0;
	SelectYxDlg dlg;
	if(dlg.DoModal()!=IDOK)return;

	::GetSystemTime(&systime);
	stachgmsg.chgclock.year=systime.wYear;
	stachgmsg.chgclock.month=(char)systime.wMonth;
	stachgmsg.chgclock.day=(char)systime.wDay;
	stachgmsg.chgclock.hour=systime.wHour+8;
	stachgmsg.chgclock.minute=(char)systime.wMinute;
	stachgmsg.chgclock.second=(char)systime.wSecond;
	stachgmsg.chgclock.msecond=systime.wMilliseconds;

	Rdb_QuickQuery breaktab;
	strcpy(stachgmsg.objectname, dlg.m_sName);
	if(breaktab.RdbLogin(MyUserName, MyPassWord)==RDB_OK && stachgmsg.objectname!=NULL)
	{

		stachgmsg.sgzflag=0;
		if(dlg.breakorswitch==0)
		{
			stachgmsg.oddflag=0;
			memcpy(stachgmsg.tabname,BREAK_TABNAME, TABNAME_LEN);
			BASESTATUS_STRU *bstat = (BASESTATUS_STRU *)breaktab.RdbGetRecordAddr(BREAK_TABNAME, stachgmsg.objectname);
			if(bstat!=NULL)
			{
				memcpy(stachgmsg.substation, bstat->substation, OBJECTNAME_LEN);
			}
		}
		else if(dlg.breakorswitch==1)
		{
			stachgmsg.oddflag=0;
			memcpy(stachgmsg.tabname,SWITCH_TABNAME, TABNAME_LEN);
			BASESTATUS_STRU *bstat = (BASESTATUS_STRU *)breaktab.RdbGetRecordAddr(SWITCH_TABNAME, stachgmsg.objectname);
			if(bstat!=NULL)
			{
				memcpy(stachgmsg.substation, bstat->substation, OBJECTNAME_LEN);
			}
		}
		else if(dlg.breakorswitch==2)
		{
			stachgmsg.sgzflag=1;
			stachgmsg.oddflag=0;
			 memcpy(stachgmsg.tabname,PROT_TABNAME,TABNAME_LEN);
			 PROTSIG_STRU *bstat = (PROTSIG_STRU *)breaktab.RdbGetRecordAddr(PROT_TABNAME, stachgmsg.objectname);
			 if(bstat!=	NULL)
			 {
				 memcpy(stachgmsg.substation, bstat->basestatus.substation, OBJECTNAME_LEN);
			 }
		}

		stachgmsg.staval=dlg.offoron;

		apevent.AddApEvent(APEVENT_STACHG,&stachgmsg,sizeof(ENT_STATUSCHG_STRU));
	}
	
}



///////////////////
//生数据事项测试----------------
//TERMINAL SOE事项测试
void CMainFrame::OnTestTERMINALSOE() 
{
	// TODO: Add your command handler code here
	static int kkk=0;
	SCADA_RAWEVENT_STRU rawevent;
	SYSTEMTIME	systime;
	int kk;
	ScadaServer service;

	kk=(++kkk)%16;


	::GetSystemTime(&systime);
	rawevent.groupno=1;
	rawevent.Year=systime.wYear;
	rawevent.Month=(char)systime.wMonth;
	rawevent.Day=(char)systime.wDay;
	rawevent.Hour=systime.wHour+8;
	rawevent.Minute=(char)systime.wMinute;
	rawevent.Second=(char)systime.wSecond;
	rawevent.Ms=systime.wMilliseconds;
	//rawevent.Source=RTUSOE;
	rawevent.Source=RTUSOE;
	rawevent.TERMINALNo=0;
	rawevent.Val.yx.YXVal=(kkk/16)%2;
	rawevent.Val.yx.yxOrder=kk;
	rawevent.Val.yx.YXVal=1;
	rawevent.Val.yx.yxOrder=0;
//	service.ForeSendRawEventToScd(ScadaTestName,&rawevent,1);
		
}

//HOSTSOE事项测试
void CMainFrame::OnTestHOSTSOE() 
{
	// TODO: Add your command handler code here
	static int kkk=0;
	SCADA_RAWEVENT_STRU rawevent[2];
	SYSTEMTIME	systime;
	int kk;
	ScadaServer service;

	kk=(++kkk)%16;


	::GetSystemTime(&systime);
	rawevent[0].groupno=1;
	rawevent[0].Year=systime.wYear;
	rawevent[0].Month=(char)systime.wMonth;
	rawevent[0].Day=(char)systime.wDay;
	rawevent[0].Hour=systime.wHour+8;
	rawevent[0].Minute=(char)systime.wMinute;
	rawevent[0].Second=(char)systime.wSecond;
	rawevent[0].Ms=systime.wMilliseconds;
	rawevent[0].Source=RTUSOE;
/*	rawevent.TERMINALNo=0;
	rawevent.Val.yx.YXVal=(kkk/16)%2;
	rawevent.Val.yx.yxOrder=kk;*/
	rawevent[0].TERMINALNo=1;
	rawevent[0].Val.yx.YXVal=0;
	rawevent[0].Val.yx.yxOrder=119;
//	service.ForeSendRawEventToScd(ScadaTestName,&rawevent[0],1);
}

void CMainFrame::OnTestmultihostsoe() 
{
	// TODO: Add your command handler code here
	SCADA_RAWEVENT_STRU rawevent[5];
	SYSTEMTIME	systime;
	ScadaServer service;

	::GetSystemTime(&systime);
//	rawevent=(SCADA_RAWEVENT_STRU *)malloc(3*sizeof(SCADA_RAWEVENT_STRU));
	rawevent[0].groupno=1;
	rawevent[0].Year=systime.wYear;
	rawevent[0].Month=(char)systime.wMonth;
	rawevent[0].Day=(char)systime.wDay;
	rawevent[0].Hour=systime.wHour+8;
	rawevent[0].Minute=(char)systime.wMinute;
	rawevent[0].Second=(char)systime.wSecond;
	rawevent[0].Ms=systime.wMilliseconds;
	rawevent[0].Source=HOSTSOE;
	rawevent[0].TERMINALNo=1;
	rawevent[0].Val.yx.YXVal=0;
	rawevent[0].Val.yx.yxOrder=0;
//	service.ForeSendRawEventToScd(ScadaTestName,(SCADA_RAWEVENT_STRU *)&rawevent[0],1);

	rawevent[1].groupno=1;
	rawevent[1].Year=systime.wYear;
	rawevent[1].Month=(char)systime.wMonth;
	rawevent[1].Day=(char)systime.wDay;
	rawevent[1].Hour=systime.wHour+8;
	rawevent[1].Minute=(char)systime.wMinute;
	rawevent[1].Second=(char)systime.wSecond;
	rawevent[1].Ms=systime.wMilliseconds+10;
	rawevent[1].Source=HOSTSOE;
	rawevent[1].TERMINALNo=2;
	rawevent[1].Val.yx.YXVal=0;
	rawevent[1].Val.yx.yxOrder=0;
//	service.ForeSendRawEventToScd(ScadaTestName,(SCADA_RAWEVENT_STRU *)&rawevent[1],1);

	rawevent[2].groupno=1;
	rawevent[2].Year=systime.wYear;
	rawevent[2].Month=(char)systime.wMonth;
	rawevent[2].Day=(char)systime.wDay;
	rawevent[2].Hour=systime.wHour+8;
	rawevent[2].Minute=(char)systime.wMinute;
	rawevent[2].Second=(char)systime.wSecond;
	rawevent[2].Ms=systime.wMilliseconds+20;
	rawevent[2].Source=HOSTSOE;
	rawevent[2].TERMINALNo=3;
	rawevent[2].Val.yx.YXVal=0;
	rawevent[2].Val.yx.yxOrder=0;
//	service.ForeSendRawEventToScd(ScadaTestName,(SCADA_RAWEVENT_STRU *)&rawevent[2],1);

	rawevent[3].groupno=1;
	rawevent[3].Year=systime.wYear;
	rawevent[3].Month=(char)systime.wMonth;
	rawevent[3].Day=(char)systime.wDay;
	rawevent[3].Hour=systime.wHour+8;
	rawevent[3].Minute=(char)systime.wMinute;
	rawevent[3].Second=(char)systime.wSecond;
	rawevent[3].Ms=systime.wMilliseconds+30;
	rawevent[3].Source=HOSTSOE;
	rawevent[3].TERMINALNo=4;
	rawevent[3].Val.yx.YXVal=0;
	rawevent[3].Val.yx.yxOrder=0;
//	service.ForeSendRawEventToScd(ScadaTestName,(SCADA_RAWEVENT_STRU *)&rawevent[3],1);

	rawevent[4].groupno=1;
	rawevent[4].Year=systime.wYear;
	rawevent[4].Month=(char)systime.wMonth;
	rawevent[4].Day=(char)systime.wDay;
	rawevent[4].Hour=systime.wHour+8;
	rawevent[4].Minute=(char)systime.wMinute;
	rawevent[4].Second=(char)systime.wSecond;
	rawevent[4].Ms=systime.wMilliseconds+40;
	rawevent[4].Source=HOSTSOE;
	rawevent[4].TERMINALNo=5;
	rawevent[4].Val.yx.YXVal=0;
	rawevent[4].Val.yx.yxOrder=0;
//	service.ForeSendRawEventToScd(ScadaTestName,(SCADA_RAWEVENT_STRU *)&rawevent[4],1);
//	if(rawevent) free(rawevent);
	
}

//配电诊断事项测试
void CMainFrame::OnTestdas() 
{
	// TODO: Add your command handler code here
/*	
	APEvent	apevent;
	Ftu fftu;
	SYSTEMTIME	systime;
	ENT_DADIAGNOSE_STRU diagnosemsg;

	CSelectDlg selectdlg;
	int ret;
	if((ret=selectdlg.DoModal())!=IDOK)return;

	::GetSystemTime(&systime);
	diagnosemsg.eclock.year=systime.wYear;
	diagnosemsg.eclock.month=(unsigned char)systime.wMonth;
	diagnosemsg.eclock.day=(unsigned char)systime.wDay;
	diagnosemsg.eclock.hour=systime.wHour+8;
	diagnosemsg.eclock.minute=(unsigned char)systime.wMinute;
	diagnosemsg.eclock.second=(unsigned char)systime.wSecond;
	diagnosemsg.eclock.msecond=systime.wMilliseconds;
	strcpy(diagnosemsg.startbreakname,LPCTSTR(selectdlg.m_startbreak));
	strcpy(diagnosemsg.endbreakname,LPCTSTR(selectdlg.m_endbreak));
	strcpy(diagnosemsg.startftuname,LPCTSTR(selectdlg.m_startftu));
	strcpy(diagnosemsg.endftuname,LPCTSTR(selectdlg.m_endftu));
		
	diagnosemsg.faulttype=selectdlg.m_faulttype;
	apevent.AddApEvent(ENT_DADIAGNOSE_STRU,&diagnosemsg,sizeof(APENTMSG_DASDIAGNOSE));
*/
	SCADA_RAWEVENT_STRU rawevent;
	SYSTEMTIME	systime;
	ScadaServer service;

	::GetSystemTime(&systime);
	rawevent.Year=systime.wYear;
	rawevent.Month=(char)systime.wMonth;
	rawevent.Day=(char)systime.wDay;
	rawevent.Hour=systime.wHour+8;
	rawevent.Minute=(char)systime.wMinute;
	rawevent.Second=(char)systime.wSecond;
	rawevent.Ms=systime.wMilliseconds;
	rawevent.Source=DAS_DIAGNOSE;
	rawevent.TERMINALNo=1;
	rawevent.YxOrder1=4;	//起始开关
	rawevent.YxOrder2=5;	//结束开关
	rawevent.FTUNo1=1;		//起始FTU
	rawevent.FTUNo2=2;		//结束FTU
	rawevent.accidenttype=2;//事故类型:0--无法区分 1-单相接地 2-相间短路
	rawevent.accidentline=12;//0-保留 1-A相 2-B相 3-C相 4-8保留 9-AB 10-BC 11-AC 12-ABC
//	service.ForeSendRawEventToScd(ScadaTestName,&rawevent,1);
	return;
}




/////////////////////
//追忆测试--------------------------------------------
//启动追忆邮件-》启动追忆线程
void CMainFrame::OnStartpdrTest() 
{
	ScadaServer service;
	service.SendStartPdrCmd(0,ScadaTestName, TestBreakName,0);
}

//直接启动追忆线程
void CMainFrame::OnStartpdr() 
{
	/*
	ENT_STARTPDR_STRU  pdrmsg;
	APEvent    apevent;

	GetInterTime(&pdrmsg.timeflag);
	ConvertTime(pdrmsg.timeflag, &pdrmsg.clock);
	pdrmsg.clock.msecond = 0;
	strcpy(pdrmsg.breakname,TestBreakName);
	pdrmsg.zygroupno=0;
	pdrmsg.causetype=0;
	strcpy(pdrmsg.cause,"手工启动追忆");
	apevent.AddApEvent(ENT_STARTPDR_STRU,&pdrmsg,sizeof(ENT_STARTPDR_STRU));
	*/
}

//添加开关事故消息，产生开关事故变位事项，进一步触发-》记录事故前后点各有关数据（快照或分组）到SYBASE服务器上
void CMainFrame::OnSgfTest() 
{
	ENT_BREAK_STRU breakapent;
	APEvent  apevent;

	memset(&breakapent,0,sizeof(ENT_BREAK_STRU));
	strcpy(breakapent.breakname,TestBreakName);
	breakapent.staval=1;
	GetCurrentClock(&breakapent.entclock);
	breakapent.sgf=1;
	apevent.AddApEvent(APEVENT_BREAK,&breakapent,sizeof(breakapent));
}



//////////////////
//遥控，遥调，组控，序列控制测试--------------------
//遥控预置(控合)测试
void CMainFrame::OnTestyk() 
{
	ScadaServer scdserv;
	scdserv.SendYkCmdReq(1,ScadaTestName, ScadaTestPassWord,TestBreakName,"break", YKCMD_PRESET, ACTTYPE_ON,0);
	_delay_time(2000*1000);
//	scdserv.SendYkCmdReq(1,ScadaTestName, ScadaTestPassWord,TestBreakName,"break", YKCMD_EXEC, ACTTYPE_ON,0);
}
//遥控执行(控合)测试
void CMainFrame::OnYkexe() 
{
	ScadaServer scdserv;
	scdserv.SendYkCmdReq(1,ScadaTestName, ScadaTestPassWord,TestBreakName,"break", YKCMD_EXEC, ACTTYPE_ON,0);
	
}

//遥控预置(控分)测试

void CMainFrame::OnYkkf() 
{
	ScadaServer scdserv;
	scdserv.SendYkCmdReq(1,ScadaTestName, ScadaTestPassWord,TestBreakName,"break", YKCMD_PRESET, ACTTYPE_OFF,0);

}

//遥控执行(控分)测试

void CMainFrame::OnYkkfzx() 
{
	ScadaServer scdserv;
	scdserv.SendYkCmdReq(1,ScadaTestName, ScadaTestPassWord,TestBreakName,"break", YKCMD_EXEC, ACTTYPE_OFF,0);
	
}
//遥调预置测试
void CMainFrame::OnTestUpdown() 
{
	// TODO: Add your command handler code here
	ScadaServer scdserv;
	scdserv.SendUpdownCmdReq(1,ScadaTestName, ScadaTestPassWord,TestAnalogName, YKCMD_PRESET, ACTTYPE_UP);
}

//序列控制测试
void CMainFrame::OnTestserialyk() 
{
	// TODO: Add your command handler code here
	ScadaServer scdserv;
	//scdserv.serialykcmd
	int yknum=3;
	SCADA_YKOBJNAME_STRU *serobjp=NULL;

	if (serobjp) delete[] serobjp;
	serobjp = new SCADA_YKOBJNAME_STRU[yknum];
	memset(serobjp, 0, sizeof(SCADA_YKOBJNAME_STRU)*yknum);

    strcpy(serobjp[0].objname, "Klsgb108Kg");
	strcpy(serobjp[0].tablename, "break");
	serobjp[0].acttype = 0;
    strcpy(serobjp[1].objname, "Klsgb109Kg");
    strcpy(serobjp[1].tablename, "break");
	serobjp[1].acttype = 1;
    strcpy(serobjp[2].objname, "Klsgb110Kg");
    strcpy(serobjp[2].tablename, "break");
	serobjp[2].acttype = 1;

	scdserv.SendSerialYkCmd(1,"sa", "sac","scode2", 0,1,yknum,serobjp);
	if (serobjp) delete[] serobjp;
}

//组控（群控）测试
void CMainFrame::OnTestgroupyk() 
{
	// TODO: Add your command handler code here
	ScadaServer scdserv;
	int yknum=1;
	SCADA_YKOBJNAME_STRU *ykobj=NULL;

	if (ykobj) delete[] ykobj;
	ykobj = new SCADA_YKOBJNAME_STRU[yknum];
	memset(ykobj, 0, sizeof(SCADA_YKOBJNAME_STRU)*yknum);

	strcpy(ykobj[0].objname, "Kdyb2#b10Kg");
	ykobj[0].acttype = 1;
//	strcpy(ykobj[1].objname, "KFtu2Kg");
//	ykobj[1].acttype = 1;
 	int ret = scdserv.SendGroupYkCmd(1,"sa", "sac", "test", yknum, ykobj);
	if (ykobj) delete[] ykobj;

}




/////////////////
//启动计算表达式表计算
void CMainFrame::OnCcformula() 
{
	// TODO: Add your command handler code here

}
//-----


//////////////////
//保护事项测试---------------------
//LFP保护
void CMainFrame::OnProtLfpEvent() 
{
	// TODO: Add your command handler code here
	SCADA_PROT_STRU	protect;
	ScadaServer	scadaSrv;
	uchar gram[256];

	memset(&protect,0,sizeof(protect));
	memset(&gram[0],0,sizeof(gram));
	protect.terminalno=4;//终端号
	protect.protocoltype=PROTTYPE_LFP;
	protect.gram_len=255; 
	gram[0]=14; //box_no保护柜号,亦即微机保护参数表的模块地址1
			//example: 18-->LFP961模块编号0
			//		29	-->	LFP963A模块编号1
			//		13	-->	LFP965B模块编号2
			//		12	-->	LFP973A模块编号3
			//		11	-->	LFP971A模块编号4
			//		15 or 16	-->	LFP941模块编号7
	gram[1]=0x46;	//报文特征码
	SYSTEMTIME	systime;
	::GetSystemTime(&systime);
	gram[5]=(char)(systime.wYear%100);
	gram[6]=(char)systime.wMonth;
	gram[7]=(char)systime.wDay;
	gram[8]=systime.wHour+8;
	gram[9]=(char)systime.wMinute;
	gram[10]=(char)systime.wSecond;
	gram[11]=systime.wMilliseconds%256;
	gram[12]=systime.wMilliseconds/256;
	gram[13]=255;		//保护事件低位
	gram[14]=63;			//保护事件高位
//TEST LFP961  LFP963A  LFP965B  LFP973A
	gram[15]=1;			//故障电流：gram[15]*gram[16]*256
	gram[16]=1;
	gram[17]=3;		//选相：AB
/*
//TEST LFP971A AND LFP941

	gram[25]=1;		//重合闸动作信息
//	gram[27]+gram[28]*256);		//重合闸时间 HZTime(ms)
	gram[29]=2;
	gram[30]=1;		//测距值FaultLocation: gram[29]+gram[30]*256);
	gram[31]=3;
	gram[32]=1;     //最大相电流MaxPhaseI: gram[31]+gram[32]*256
	gram[33]=4;
	gram[34]=1;	    //最大零序电流MaxZeroI: gram[33]+gram[34]*256
	gram[35]=3;		//选相：phase 
*/	
	memcpy((char *)protect.gram,(char *)gram,256);
	scadaSrv.ForeSendProtInfo(0,"",&protect);
}

void CMainFrame::OnProtLfpDetect() 
{
	// TODO: Add your command handler code here
	SCADA_PROT_STRU	protect;
	ScadaServer	scadaSrv;
	uchar gram[256];

	memset(&protect,0,sizeof(protect));
	memset(&gram[0],0,sizeof(gram));
	protect.terminalno=4;//终端号
	protect.protocoltype=PROTTYPE_LFP;
	protect.gram_len=255;
	gram[0]=16; //box_no保护柜号,亦即微机保护参数表的模块地址1
			//example: 18-->LFP961模块编号0
			//		29	-->	LFP963A模块编号1
			//		13	-->	LFP965B模块编号2
			//		12	-->	LFP973A模块编号3
			//		11	-->	LFP971A模块编号4
			//		15 or 16	-->	LFP941模块编号7
	gram[1]=0x47;	//报文特征码
	SYSTEMTIME	systime;
	::GetSystemTime(&systime);
	gram[5]=(char)(systime.wYear%100);
	gram[6]=(char)systime.wMonth;
	gram[7]=(char)systime.wDay;
	gram[8]=systime.wHour+8;
	gram[9]=(char)systime.wMinute;
	gram[10]=(char)systime.wSecond;
	gram[11]=systime.wMilliseconds%256;
	gram[12]=systime.wMilliseconds/256;
	gram[14]=255;		//自检信息
	gram[15]=255;		//自检信息 
	
	//以下针对LFP971A
/*	gram[5]=2;	//	LFP971A：0->管理板，2->保护板
	gram[6]=(char)(systime.wYear%100);
	gram[7]=(char)systime.wMonth;
	gram[8]=(char)systime.wDay;
	gram[9]=systime.wHour+8;
	gram[10]=(char)systime.wMinute;
	gram[11]=(char)systime.wSecond;
	gram[12]=systime.wMilliseconds%256;
	gram[13]=systime.wMilliseconds/256;
	gram[15]=255;		//自检信息
	gram[16]=63;		//自检信息
*/	memcpy((char *)protect.gram,(char *)gram,256);
	scadaSrv.ForeSendProtInfo(0,"",&protect);
	
}

//ISA保护事项测试
void CMainFrame::OnISAProtectEvent() 
{
	SCADA_PROT_STRU	protect;
	ScadaServer	scadaSrv;
	uchar gram[256];

	memset(&protect,0,sizeof(protect));
	memset(&gram[0],0,sizeof(gram));
	protect.terminalno=4;//终端号
	protect.protocoltype=PROTTYPE_ISA;
	protect.gram_len=255;
	gram[0]=1; //box_no保护柜号,亦即微机保护参数表的模块地址1
	gram[1]=0x40;	//报文特征码
	SYSTEMTIME	systime;
	::GetSystemTime(&systime);
	gram[3]=(char)((systime.wYear%100)/10*16+(systime.wYear%100)%10);
	gram[5]=(char)((systime.wMonth%100)/10*16+(systime.wMonth%100)%10);
	gram[6]=(char)((systime.wDay%100)/10*16+(systime.wDay%100)%10);
	gram[7]=(((systime.wHour+8)%100)/10*16+((systime.wHour+8)%100)%10);
	gram[8]=(char)((systime.wMinute%100)/10*16+(systime.wMinute%100)%10);
	gram[9]=(char)((systime.wSecond%100)/10*16+(systime.wSecond%100)%10);
	gram[10]=(((systime.wMilliseconds%256)%100)/10*16+((systime.wMilliseconds%256)%100)%10);
	gram[11]=(((systime.wMilliseconds/256)%100)/10*16+((systime.wMilliseconds/256)%100)%10);
	gram[12]=0;		//保护编号
	gram[13]=1;		//事件编号
	gram[14]=1;		//事故跳闸次数
	gram[15]=1;		//故障类型
	gram[16]=1;		//动作类型
	gram[17]=1;		//测量类型
	gram[18]=1;		//值：低位
	gram[19]=1;		//值：高位
	memcpy((char *)protect.gram,(char *)gram,256);
	scadaSrv.ForeSendProtInfo(0,"",&protect);
}

//SEL保护事项测试
void CMainFrame::OnSELProtectEvent() 
{
	SCADA_PROT_STRU	protect;
	ScadaServer	scadaSrv;
	uchar gram[256];

	memset(&protect,0,sizeof(protect));
	memset(&gram[0],0,sizeof(gram));
    protect.terminalno=4;                  //终端号
    protect.protocoltype=2;               //规约类型  (决定保护信息的解释方法)	
											//0-->LFP, 1-->ISA ,   2-->SEL,		3-->DFP
	protect.gram_len=255;
	gram[0]=12; //box_no保护柜号,亦即微机保护参数表的模块地址1
	gram[1]=0;	//prot_id保护编号
	gram[2]=3;	//device_type装置类型	S551
	SYSTEMTIME	systime;
	::GetSystemTime(&systime);
	gram[5]=(char)systime.wMonth;
	gram[7]=(char)systime.wDay;
	gram[9]=(char)(systime.wYear%100);
	gram[11]=systime.wHour+8;
	gram[13]=(char)systime.wMinute;
	gram[15]=(char)systime.wSecond;
	gram[16]=systime.wMilliseconds/256;
	gram[17]=systime.wMilliseconds%256;

	gram[18]='E';	//action_string 18-23 动作代码
	gram[19]='R';
	gram[20]='2';
	gram[21]='\0';
	gram[26]=1;		//totalact_nums=gram[26]*256+gram[27]  -->  SEL Shot Number
	gram[27]=1;
	gram[28]=2;	//value=gram[28]*256+gram[29] ---> Fault Current
	gram[29]=2;

	memcpy((char *)protect.gram,(char *)gram,256);
	scadaSrv.ForeSendProtInfo(0,"",&protect);
}


void CMainFrame::OnISAProtectDetect() 
{
	SCADA_PROT_STRU	protect;
	ScadaServer	scadaSrv;
	uchar gram[256];

	memset(&protect,0,sizeof(protect));
	memset(&gram[0],0,sizeof(gram));
	protect.terminalno=4;//终端号
	protect.protocoltype=PROTTYPE_ISA;
	protect.gram_len=255;
	gram[0]=1; //box_no保护柜号,亦即微机保护参数表的模块地址1
	gram[1]=0x51;	//报文特征码
	SYSTEMTIME	systime;
	::GetSystemTime(&systime);
	gram[3]=(char)((systime.wYear%100)/10*16+(systime.wYear%100)%10);
	gram[5]=(char)((systime.wMonth%100)/10*16+(systime.wMonth%100)%10);
	gram[6]=(char)((systime.wDay%100)/10*16+(systime.wDay%100)%10);
	gram[7]=(((systime.wHour+8)%100)/10*16+((systime.wHour+8)%100)%10);
	gram[8]=(char)((systime.wMinute%100)/10*16+(systime.wMinute%100)%10);
	gram[9]=(char)((systime.wSecond%100)/10*16+(systime.wSecond%100)%10);
	gram[10]=(((systime.wMilliseconds%256)%100)/10*16+((systime.wMilliseconds%256)%100)%10);
	gram[11]=(((systime.wMilliseconds/256)%100)/10*16+((systime.wMilliseconds/256)%100)%10);
	gram[12]=0;		//保护编号
	gram[13]=1;		//自诊断类型
	memcpy((char *)protect.gram,(char *)gram,256);
	scadaSrv.ForeSendProtInfo(0,"",&protect);
}

void CMainFrame::OnTestmanset() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
	char yxvalue=1;
	float ycvalue=55.05f;

//    scadaSrv.SendManSetValReq(1,ScadaTestName,"otestZt","status",1,RDB_DATATYPE_UCHAR,&yxvalue);
    scadaSrv.SendManSetValReq(1,ScadaTestName,TestAnalogName,"analog",1,RDB_DATATYPE_FLOAT,&ycvalue);
	
}

void CMainFrame::OnTestclearmanset() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
	char yxvalue=1;
	float ycvalue=55.05f;
//    scadaSrv.SendManSetValReq(1,ScadaTestName,"otestZt","status",0,RDB_DATATYPE_UCHAR,&yxvalue);
    scadaSrv.SendManSetValReq(1,ScadaTestName,TestAnalogName,"analog",0,RDB_DATATYPE_FLOAT,&ycvalue);

}






void CMainFrame::OnSetground() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
	CARD_RCD_STRU cardrcdp;

    cardrcdp.groupno=1;
	cardrcdp.firststate=0;
	cardrcdp.secondstate=0;
	cardrcdp.objtype=6;
	strcpy(cardrcdp.objname,"Ktz500Kg");
	cardrcdp.cardtype=10;
	cardrcdp.cardno=0;
	cardrcdp.startx=500;
	cardrcdp.starty=100;
#ifndef QTGRAPH
	cardrcdp.width=20;
	cardrcdp.height=20;
	cardrcdp.imagetype=0;
	strcpy(cardrcdp.imagename,"test.gr");
#endif
	strcpy(cardrcdp.hostname,"gepeng");
	strcpy(cardrcdp.username,"sa");
	strcpy(cardrcdp.cardtime,"挂牌时间: 2002年11月1日 11时32分35秒 星期5");
#ifndef QTGRAPH
	strcpy(cardrcdp.bitmap,"manjx.bmp");
	strcpy(cardrcdp.userdefgr,"");
#endif
#ifdef QTGRAPH
	cardrcdp.metaid=1;
	strcpy(cardrcdp.graphpath,"c:\\graphpath");
	strcpy(cardrcdp.metapath,"c:\\metapath");
#endif

    scadaSrv.SendSetSpecialFlagReq(1,"sa", 1, &cardrcdp);
}

void CMainFrame::OnClearground() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;	
	CARD_RCD_STRU cardrcdp;
    cardrcdp.groupno=1;
	cardrcdp.firststate=0;
	cardrcdp.secondstate=0;
	cardrcdp.objtype=6;
	strcpy(cardrcdp.objname,"Ktz500Kg");
	cardrcdp.cardtype=10;
	cardrcdp.cardno=0;
	cardrcdp.startx=500;
	cardrcdp.starty=100;
#ifndef QTGRAPH
	cardrcdp.width=20;
	cardrcdp.height=20;
	cardrcdp.imagetype=0;
	strcpy(cardrcdp.imagename,"test.gr");
#endif
	strcpy(cardrcdp.hostname,"gepeng");
	strcpy(cardrcdp.username,"sa");
	strcpy(cardrcdp.cardtime,"挂牌时间: 2002年11月1日 11时32分35秒 星期5");
#ifndef QTGRAPH
	strcpy(cardrcdp.bitmap,"manjx.bmp");
	strcpy(cardrcdp.userdefgr,"");
#endif
#ifdef QTGRAPH
	cardrcdp.metaid=1;
	strcpy(cardrcdp.graphpath,"c:\\graphpath");
	strcpy(cardrcdp.metapath,"c:\\metapath");
#endif
    scadaSrv.SendSetSpecialFlagReq(1,"sa", 0, &cardrcdp);	
}

void CMainFrame::OnOncheck() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
	CARD_RCD_STRU cardrcdp;

    cardrcdp.groupno=1;
	cardrcdp.firststate=0;
	cardrcdp.secondstate=0;
	cardrcdp.objtype=1;
	strcpy(cardrcdp.objname,"Kfh501");
	cardrcdp.cardtype=1;
	cardrcdp.cardno=0;
	cardrcdp.startx=500;
	cardrcdp.starty=100;
#ifndef QTGRAPH
	cardrcdp.width=20;
	cardrcdp.height=20;
	cardrcdp.imagetype=0;
	strcpy(cardrcdp.imagename,"12345678901234567890123456789012345678901234567890");
#endif
	strcpy(cardrcdp.hostname,"gepeng");
	strcpy(cardrcdp.username,"sa");
	strcpy(cardrcdp.cardtime,"挂牌时间: 2002年11月1日 11时32分35秒 星期5");
#ifndef QTGRAPH
	strcpy(cardrcdp.bitmap,"manjx.bmp");
	strcpy(cardrcdp.userdefgr,"");
#endif
#ifdef QTGRAPH
	cardrcdp.metaid=1;
	strcpy(cardrcdp.graphpath,"c:\\graphpath");
	strcpy(cardrcdp.metapath,"c:\\metapath");
#endif

    scadaSrv.SendSetSpecialFlagReq(1,"sa", 1, &cardrcdp);	
}

void CMainFrame::OnClearcheck() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
	CARD_RCD_STRU cardrcdp;

    cardrcdp.groupno=1;
	cardrcdp.firststate=0;
	cardrcdp.secondstate=0;
	cardrcdp.objtype=1;
	strcpy(cardrcdp.objname,"Kfh501");
	cardrcdp.cardtype=1;
	cardrcdp.cardno=0;
	cardrcdp.startx=500;
	cardrcdp.starty=100;
#ifndef QTGRAPH
	cardrcdp.width=20;
	cardrcdp.height=20;
	cardrcdp.imagetype=0;
	strcpy(cardrcdp.imagename,"12345678901234567890123456789012345678901234567890");
#endif
	strcpy(cardrcdp.hostname,"gepeng");
	strcpy(cardrcdp.username,"sa");
	strcpy(cardrcdp.cardtime,"挂牌时间: 2002年11月1日 11时32分35秒 星期5");
#ifndef QTGRAPH
	strcpy(cardrcdp.bitmap,"manjx.bmp");
	strcpy(cardrcdp.userdefgr,"");
#endif
#ifdef QTGRAPH
	cardrcdp.metaid=1;
	strcpy(cardrcdp.graphpath,"c:\\graphpath");
	strcpy(cardrcdp.metapath,"c:\\metapath");
#endif

    scadaSrv.SendSetSpecialFlagReq(1,"sa", 0, &cardrcdp);	
}

void CMainFrame::OnTestwfbs() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
    scadaSrv.SendWfbsReq(1, "", 3, WFBS_LOCK,WFBS_LOCKSINGLE);		
}

void CMainFrame::OnCancelwfbs() 
{
	// TODO: Add your command handler code here
	ScadaServer	scadaSrv;
    scadaSrv.SendWfbsReq(1, "", 3, WFBS_UNLOCK,WFBS_LOCKSINGLE);	
}


void CMainFrame::OnOlraweventTest() 
{
	// TODO: Add your command handler code here
	SCADA_OVERLIMIT_STRU olapent;
	strcpy(olapent.tabname,ANALOG_TABNAME);
	strcpy(olapent.objectname,TestAnalogName);
	olapent.olgrade=1;
	olapent.olvalue=49.8f;
	ScadaServer	scadaSrv;
    scadaSrv.ForeSendOverLimitEventToScd(1, "sa", &olapent);	
	
}

void CMainFrame::OnNetsendyc() 
{
	// TODO: Add your command handler code here
	SCADA_NET_YCDATA_STRU *ycobj=NULL;
	int ycnum=1;

	if (ycobj) delete[] ycobj;
	ycobj = new SCADA_NET_YCDATA_STRU[ycnum];
	memset(ycobj, 0, sizeof(SCADA_NET_YCDATA_STRU)*ycnum);

	ycobj[0].groupno = 1;
	strcpy(ycobj[0].name, "Kdyb170P");
	ycobj[0].value = 65;
	ScadaServer	scadaSrv;
    scadaSrv.NetSendYcData(1, SYS_PWD,"",ycnum,ycobj);	
	
}

void CMainFrame::OnNetsendyx() 
{
	// TODO: Add your command handler code here
	SCADA_NET_YXDATA_STRU *yxobj=NULL;
	int yxnum=3;

	if (yxobj) delete[] yxobj;
	yxobj = new SCADA_NET_YXDATA_STRU[yxnum];
	memset(yxobj, 0, sizeof(SCADA_NET_YXDATA_STRU)*yxnum);

	yxobj[0].groupno = 1;
	strcpy(yxobj[0].name, "Kgyb121Kg");
	yxobj[0].value = 1;

	yxobj[1].groupno = 1;
	strcpy(yxobj[1].name, "Kgyb122Kg");
	yxobj[1].value = 0;

	yxobj[2].groupno = 1;
	strcpy(yxobj[2].name, "Kgyb123Kg");
	yxobj[2].value = 1;
	ScadaServer	scadaSrv;
    scadaSrv.NetSendYxData(1, SYS_PWD,"",yxnum,yxobj);	
}

void CMainFrame::OnTestyt() 
{
	// TODO: Add your command handler code here
	ScadaServer scdserv;
	scdserv.SendYtCmdReq(1,ScadaTestName, ScadaTestPassWord,TestAnalogName, YT_SENDCMD, 150.0);
}

