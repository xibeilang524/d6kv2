// f1jdll.h : main header file for the F1JDLL DLL
//
#ifdef WIN32
	#if !defined(AFX_F1JDLL_H__16A6FE7D_5987_4178_AF2F_6170E47A2381__INCLUDED_)
	#define AFX_F1JDLL_H__16A6FE7D_5987_4178_AF2F_6170E47A2381__INCLUDED_

	#if _MSC_VER > 1000
	#pragma once
	#endif // _MSC_VER > 1000

	#ifndef __AFXWIN_H__
		#error include 'stdafx.h' before including this file for PCH
	#endif

	#include "resource.h"		// main symbols
/////////////////////////////////////////////////////////////////////////////
// CF1jdllApp
// See f1jdll.cpp for the implementation of this class
//

class CF1jdllApp : public CWinApp
{
public:
	CF1jdllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CF1jdllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CF1jdllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_F1JDLL_H__16A6FE7D_5987_4178_AF2F_6170E47A2381__INCLUDED_)
#endif
