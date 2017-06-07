// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0BC6F75D_412C_11D2_81E8_0060B0CD7857__INCLUDED_)
#define AFX_MAINFRM_H__0BC6F75D_412C_11D2_81E8_0060B0CD7857__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#define CHECKMAINPROC 2

class CMainFrame : public CFrameWnd
{
public: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
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
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg LRESULT OnProcCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnRdbEvent(WPARAM,LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT onclick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRawDataReady(WPARAM,LPARAM);
	afx_msg LRESULT OnMessageReceived(WPARAM, LPARAM);
	afx_msg void OnOleventTest();
	afx_msg void OnSgfTest();
	afx_msg void OnStaChgTest();
	afx_msg void OnStartpdrTest();
	afx_msg void OnTestTERMINALSOE();
	afx_msg void OnTestserialyk();
	afx_msg void OnTestyk();
	afx_msg void OnReplace();
	afx_msg void OnClearreplace();
	afx_msg void OnClearChgStatus();
	afx_msg void OnTestdas();
	afx_msg void OnMENUITEMtestyx();
	afx_msg void OnISAProtectEvent();
	afx_msg void OnSELProtectEvent();
	afx_msg void OnMaskent();
	afx_msg void OnClearmaskent();
	afx_msg void OnStartpdr();
	afx_msg void OnCcformula();
	afx_msg void OnTestgroupyk();
	afx_msg void OnTestHOSTSOE();
	afx_msg void OnTestUpdown();
	afx_msg void OnTestYxSOE();
	afx_msg void OnISAProtectDetect();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnProtLfpEvent();
	afx_msg void OnProtLfpDetect();
	afx_msg void OnTestmanset();
	afx_msg void OnTestclearmanset();
	afx_msg void OnYkexe();
	afx_msg void OnYkkf();
	afx_msg void OnYkkfzx();
	afx_msg void OnSetground();
	afx_msg void OnClearground();
	afx_msg void OnOncheck();
	afx_msg void OnClearcheck();
	afx_msg void OnTestwfbs();
	afx_msg void OnCancelwfbs();
	afx_msg void OnTestmultihostsoe();
	afx_msg void OnOlraweventTest();
	afx_msg void OnNetsendyc();
	afx_msg void OnTestyt();
	afx_msg void OnNetsendyx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0BC6F75D_412C_11D2_81E8_0060B0CD7857__INCLUDED_)
