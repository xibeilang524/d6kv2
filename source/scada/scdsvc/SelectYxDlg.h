#if !defined(AFX_SELECTYXDLG_H__137DC489_6D63_11D2_88E4_0000F805FD11__INCLUDED_)
#define AFX_SELECTYXDLG_H__137DC489_6D63_11D2_88E4_0000F805FD11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SelectYxDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SelectYxDlg dialog

class SelectYxDlg : public CDialog
{
// Construction
public:
	SelectYxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SelectYxDlg)
	enum { IDD = IDD_DIALOG_YX };
	CString	m_sName;
	//}}AFX_DATA
	int breakorswitch;
	int offoron;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SelectYxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SelectYxDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTYXDLG_H__137DC489_6D63_11D2_88E4_0000F805FD11__INCLUDED_)
