// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2F6D0CD1_A7D4_4B2D_BE2B_CBB1BA23347F__INCLUDED_)
#define AFX_MAINFRM_H__2F6D0CD1_A7D4_4B2D_BE2B_CBB1BA23347F__INCLUDED_

#include "db/dbapi.h"
#include "ClientInfo.h"	// Added by ClassView
#include "DlgSysConfig.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnAbout();
	void PreExit();
	void getMacLicense();
	SYS_EVENT m_sysevent;
	//ProcMan proc;
	EventProc m_eventproc;
	NetDbg m_netbug;
	CClientInfo *m_pdlg;
	CDlgSysConfig *m_pSysConfigDlg;
	CMenu * pPopMenu;

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	LRESULT OnBarIconMsg(WPARAM wParam , LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnExit();
	afx_msg void OnShowMainwindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHideMainwindow();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClientInfo();
	afx_msg void OnSet();
	afx_msg LRESULT OnDataMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2F6D0CD1_A7D4_4B2D_BE2B_CBB1BA23347F__INCLUDED_)
