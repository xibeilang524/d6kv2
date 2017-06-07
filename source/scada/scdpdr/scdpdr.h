// scdpdr.h : main header file for the SCDPDR application
//

#if !defined(AFX_SCDPDR_H__56E021B2_0D9B_4E40_B0F0_88C2219AE002__INCLUDED_)
#define AFX_SCDPDR_H__56E021B2_0D9B_4E40_B0F0_88C2219AE002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"


#define  MAXTHREADNUM   10

/////////////////////////////////////////////////////////////////////////////
// CScdpdrApp:
// See scdpdr.cpp for the implementation of this class
//
#define CHECKMAINPROC	5

class CScdpdrApp : public CWinApp
{
public:
	CScdpdrApp();
	~CScdpdrApp();
	CMainFrame* pFrame;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScdpdrApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CScdpdrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDPDR_H__56E021B2_0D9B_4E40_B0F0_88C2219AE002__INCLUDED_)
