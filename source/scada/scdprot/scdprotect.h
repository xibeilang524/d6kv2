// scdprotect.h : main header file for the SCDPROTECT application
//

#if !defined(AFX_SCDPROTECT_H__682B9706_C355_11D3_B860_00C04FCCE0E8__INCLUDED_)
#define AFX_SCDPROTECT_H__682B9706_C355_11D3_B860_00C04FCCE0E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// CScdprotectApp:
// See scdprotect.cpp for the implementation of this class
//

class CScdprotectApp : public CWinApp
{
public:
	CScdprotectApp();
	CMainFrame *pFrame;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScdprotectApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	HANDLE m_hInstanceMutex;
// Implementation

	//{{AFX_MSG(CScdprotectApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDPROTECT_H__682B9706_C355_11D3_B860_00C04FCCE0E8__INCLUDED_)
