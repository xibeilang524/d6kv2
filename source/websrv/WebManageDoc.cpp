// WebManageDoc.cpp : implementation of the CWebManageDoc class
//

#include "stdafx.h"
#include "WebManage.h"

#include "WebManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebManageDoc

IMPLEMENT_DYNCREATE(CWebManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CWebManageDoc, CDocument)
	//{{AFX_MSG_MAP(CWebManageDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebManageDoc construction/destruction

CWebManageDoc::CWebManageDoc()
{
	// TODO: add one-time construction code here

}

CWebManageDoc::~CWebManageDoc()
{
}

BOOL CWebManageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWebManageDoc serialization

void CWebManageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWebManageDoc diagnostics

#ifdef _DEBUG
void CWebManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWebManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebManageDoc commands
