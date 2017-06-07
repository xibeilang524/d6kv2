#if !defined(AFX_SELECTDLG_H__D7D8BF46_6C8C_11D2_9C24_0000F875C60F__INCLUDED_)
#define AFX_SELECTDLG_H__D7D8BF46_6C8C_11D2_9C24_0000F875C60F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog

class CSelectDlg : public CDialog
{
// Construction
public:
	CSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectDlg)
	enum { IDD = IDD_DIALOG_SELECT };
	CString	m_endbreak;
	CString	m_endftu;
	int		m_faulttype;
	CString	m_startbreak;
	CString	m_startftu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDLG_H__D7D8BF46_6C8C_11D2_9C24_0000F875C60F__INCLUDED_)
