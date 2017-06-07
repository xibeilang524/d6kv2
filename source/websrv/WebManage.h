// WebManage.h : main header file for the WEBMANAGE application
//

#if !defined(AFX_WEBMANAGE_H__8D51FA5B_1C10_439E_88DF_1A8537D68788__INCLUDED_)
#define AFX_WEBMANAGE_H__8D51FA5B_1C10_439E_88DF_1A8537D68788__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWebManageApp:
// See WebManage.cpp for the implementation of this class
//

class CWebManageApp : public CWinApp
{
public:
	CWebManageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebManageApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWebManageApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBMANAGE_H__8D51FA5B_1C10_439E_88DF_1A8537D68788__INCLUDED_)
