#if !defined(AFX_DLGSYSCONFIG_H__A57458DB_E3A6_4448_B1BC_58A3C0DB880B__INCLUDED_)
#define AFX_DLGSYSCONFIG_H__A57458DB_E3A6_4448_B1BC_58A3C0DB880B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysConfig.h : header file
//
#include "rdb/rdbapi.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConfig dialog

class CDlgSysConfig : public CDialog
{
// Construction
public:
	BOOL m_bDbFlag;//数据库连接标志
	DBApi m_db;
	BOOL changef;
	int m_curSOrt;
	void listType(int sort);
	CDlgSysConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysConfig)
	enum { IDD = IDD_DLG_SYSCONFIG };
	CComboBox	m_combo_group;
	CListBox	m_list_seltype;
	CListBox	m_list_eventsort;
	CListBox	m_list_unsel;
	CButton	m_btn_downall;
	CButton	m_btn_down;
	CButton	m_btn_addall;
	CButton	m_btn_all;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysConfig)
	afx_msg void OnBtnAddall();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnDownall();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListEventsort();
	afx_msg void OnSelchangeComboGroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSCONFIG_H__A57458DB_E3A6_4448_B1BC_58A3C0DB880B__INCLUDED_)
