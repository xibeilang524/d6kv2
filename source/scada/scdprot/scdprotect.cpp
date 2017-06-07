// scdprotect.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "scdprotect.h"

#include "MainFrm.h"
#include "scdprotectDoc.h"
#include "scdprotectView.h"

#include "net/netapi.h"
#include "procthread.h"
#include "verapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CHECKPROC_TIMERID     100

/////////////////////////////////////////////////////////////////////////////
// CScdprotectApp

BEGIN_MESSAGE_MAP(CScdprotectApp, CWinApp)
	//{{AFX_MSG_MAP(CScdprotectApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScdprotectApp construction

HANDLE hThread;	//线程对象 global
//线程函数声明
extern UINT proc_scadamail_level2(LPVOID pParam);//处理邮件

CScdprotectApp::CScdprotectApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hInstanceMutex = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScdprotectApp object

CScdprotectApp theApp;

/*
// 用于禁止同时运行同一进程的多个实例
#pragma data_seg("Shared")
int nInstanceExist=0;
#pragma data_seg()

#pragma comment(linker,"-section:Shared,rws")*/

/////////////////////////////////////////////////////////////////////////////
// CScdprotectApp initialization

BOOL CScdprotectApp::InitInstance()
{
	//  [1/15/2013 qianjin]
/*
    if(nInstanceExist>0) return FALSE;
	nInstanceExist=1;
*/
	if(__argc > 1)
	{
		for(int i=1; i<__argc; i++)
		{
			if(strcmp("--version", __targv[i]) == 0)
			{
				printf("scdprot,1.51.1");
				writever("scdprot,1.51.1");
				return 0;
			}
		}
	}
	
	m_hInstanceMutex = ::CreateMutex(NULL,TRUE,m_pszExeName);
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}

//	if(procman.IsProcExist("scdprot"))return FALSE;
	


	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CScdprotectDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CScdprotectView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();

//	parent.SetMainWnd(m_pMainWnd->m_hWnd);

	//启动线程；
	hThread = (HANDLE) scd_pthread_create((PTHREAD_CALLFUNC)proc_scadamail_level2, NULL, 100);		//邮件处理 level 2
//	AfxBeginThread(proc_scadamail_level2,NULL);

	m_pMainWnd->SetTimer(CHECKPROC_TIMERID,1*1000,NULL);
	NetDbg  dbg;
	dbg.RegProc("保护事件处理");
	return TRUE;
}

int CScdprotectApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	TerminateThread(hThread,0);
	
	if (m_hInstanceMutex)
	{
		CloseHandle(m_hInstanceMutex);
	}

	NetDbg  dbg;
	dbg.UnRegProc();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CScdprotectApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CScdprotectApp commands
