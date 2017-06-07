#if !defined(AFX_CLIENTINFO_H__C43D3C24_9B5E_4813_817F_71E6C1471B09__INCLUDED_)
#define AFX_CLIENTINFO_H__C43D3C24_9B5E_4813_817F_71E6C1471B09__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClientInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClientInfo dialog

class CClientInfo : public CDialog
{
// Construction
public:
	RECT m_rect;
	void AlignControls();
	void ListctrlAutoSizeColumns(CListCtrl & listctrl);
	void ListCtrlAddExStyle(HWND hWnd ,DWORD dwNewStyle);
	void listclntinfo();
	CClientInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClientInfo)
	enum { IDD = IDD_DIALOG_CLIENTINFO };
	CStatic	m_static_frame;
	CButton	m_idok;
	CStatic	m_static_clntsum;
	CEdit	m_edit_clntsum;
	CListCtrl	m_clntinfo_list;
	CButton	m_btn_kill;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientInfo)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClientInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnKill();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTINFO_H__C43D3C24_9B5E_4813_817F_71E6C1471B09__INCLUDED_)
