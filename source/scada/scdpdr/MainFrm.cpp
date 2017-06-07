// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "scdpdr.h"

#include "MainFrm.h"
#include "net/netdef.h"
#include "rdb/rdbapi.h"
#include "scd/cmddef.h"
#include "db/dbapi.h"
#include "scadaproc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern DBApi dbapi;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//    cs.lpszClass = AfxRegisterWndClass (CS_NOCLOSE, 0, 0, AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
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
extern HANDLE hThread[3];
extern HANDLE hKillThread;


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==CHECKMAINPROC)
	{
		ProcMan proc;
		if(!proc.CheckParentProc())PostMessage(WM_CLOSE);
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	dbapi.Close();

	ProcMan proc;
	proc.EndProc();	
	CFrameWnd::OnClose();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ShowWindow(SW_MINIMIZE);	
	
}
