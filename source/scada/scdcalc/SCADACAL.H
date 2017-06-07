// scadacal.h : main header file for the SCADACAL application
//

#if !defined(AFX_SCADACAL_H__884DD672_7E7A_11D2_88F2_0000F805FD11__INCLUDED_)
#define AFX_SCADACAL_H__884DD672_7E7A_11D2_88F2_0000F805FD11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"

#define  MAXTHREADNUM   10

/////////////////////////////////////////////////////////////////////////////
// CScadacalApp:
// See scadacal.cpp for the implementation of this class
//
#define CHECKMAINPROC 7
class CScadacalApp : public CWinApp
{
public:
	CScadacalApp();
	~CScadacalApp();
	CMainFrame *pFrame;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScadacalApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CScadacalApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCADACAL_H__884DD672_7E7A_11D2_88F2_0000F805FD11__INCLUDED_)
