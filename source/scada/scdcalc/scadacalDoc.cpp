// scadacalDoc.cpp : implementation of the CScadacalDoc class
//

#include "stdafx.h"
#include "scadacal.h"

#include "scadacalDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScadacalDoc

IMPLEMENT_DYNCREATE(CScadacalDoc, CDocument)

BEGIN_MESSAGE_MAP(CScadacalDoc, CDocument)
	//{{AFX_MSG_MAP(CScadacalDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScadacalDoc construction/destruction

CScadacalDoc::CScadacalDoc()
{
	// TODO: add one-time construction code here

}

CScadacalDoc::~CScadacalDoc()
{
}

BOOL CScadacalDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CScadacalDoc serialization

void CScadacalDoc::Serialize(CArchive& ar)
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
// CScadacalDoc diagnostics

#ifdef _DEBUG
void CScadacalDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScadacalDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScadacalDoc commands
