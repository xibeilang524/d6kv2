// scdprotectDoc.cpp : implementation of the CScdprotectDoc class
//

#include "stdafx.h"
#include "scdprotect.h"

#include "scdprotectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScdprotectDoc

IMPLEMENT_DYNCREATE(CScdprotectDoc, CDocument)

BEGIN_MESSAGE_MAP(CScdprotectDoc, CDocument)
	//{{AFX_MSG_MAP(CScdprotectDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScdprotectDoc construction/destruction

CScdprotectDoc::CScdprotectDoc()
{
	// TODO: add one-time construction code here

}

CScdprotectDoc::~CScdprotectDoc()
{
}

BOOL CScdprotectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CScdprotectDoc serialization

void CScdprotectDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CScdprotectDoc diagnostics

#ifdef _DEBUG
void CScdprotectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScdprotectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScdprotectDoc commands
