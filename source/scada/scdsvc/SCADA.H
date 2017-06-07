// scada.h : main header file for the SCADA application
//

#if !defined(AFX_SCADA_H__0BC6F759_412C_11D2_81E8_0060B0CD7857__INCLUDED_)
#define AFX_SCADA_H__0BC6F759_412C_11D2_81E8_0060B0CD7857__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "net/netdef.h"

#define  MAXTHREADNUM   13


/////////////////////////////////////////////////////////////////////////////
// CScadaApp:
// See scada.cpp for the implementation of this class
//
#include "MainFrm.h"

class CScadaApp : public CWinApp
{
public:
	CScadaApp();
	~CScadaApp();
	CMainFrame* pFrame;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScadaApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CScadaApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCADA_H__0BC6F759_412C_11D2_81E8_0060B0CD7857__INCLUDED_)
