// scdprotectView.cpp : implementation of the CScdprotectView class
//

#include "stdafx.h"
#include "scdprotect.h"

#include "scdprotectDoc.h"
#include "scdprotectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScdprotectView

IMPLEMENT_DYNCREATE(CScdprotectView, CEditView)

BEGIN_MESSAGE_MAP(CScdprotectView, CEditView)
	//{{AFX_MSG_MAP(CScdprotectView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScdprotectView construction/destruction

CScdprotectView::CScdprotectView()
{
	// TODO: add construction code here

}

CScdprotectView::~CScdprotectView()
{
}

BOOL CScdprotectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CScdprotectView drawing

void CScdprotectView::OnDraw(CDC* pDC)
{
	CScdprotectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CScdprotectView printing

BOOL CScdprotectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CScdprotectView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CScdprotectView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CScdprotectView diagnostics

#ifdef _DEBUG
void CScdprotectView::AssertValid() const
{
	CEditView::AssertValid();
}

void CScdprotectView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CScdprotectDoc* CScdprotectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScdprotectDoc)));
	return (CScdprotectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScdprotectView message handlers
