// dlllink.h : main header file for the DLLLINK DLL
//

#if !defined(AFX_DLLLINK_H__894D59B8_C6CA_4D4C_B4FE_AE1DF4F7E7A2__INCLUDED_)
#define AFX_DLLLINK_H__894D59B8_C6CA_4D4C_B4FE_AE1DF4F7E7A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDlllinkApp
// See dlllink.cpp for the implementation of this class
//

class CDlllinkApp : public CWinApp
{
public:
	CDlllinkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlllinkApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDlllinkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


// !defined(AFX_DLLLINK_H__894D59B8_C6CA_4D4C_B4FE_AE1DF4F7E7A2__INCLUDED_)
