// scdpdrDoc.cpp : implementation of the CScdpdrDoc class
//

#include "stdafx.h"
#include "scdpdr.h"

#include "scdpdrDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScdpdrDoc

IMPLEMENT_DYNCREATE(CScdpdrDoc, CDocument)

BEGIN_MESSAGE_MAP(CScdpdrDoc, CDocument)
	//{{AFX_MSG_MAP(CScdpdrDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScdpdrDoc construction/destruction

CScdpdrDoc::CScdpdrDoc()
{
	// TODO: add one-time construction code here

}

CScdpdrDoc::~CScdpdrDoc()
{
}

BOOL CScdpdrDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CScdpdrDoc serialization

void CScdpdrDoc::Serialize(CArchive& ar)
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
// CScdpdrDoc diagnostics

#ifdef _DEBUG
void CScdpdrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScdpdrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScdpdrDoc commands
