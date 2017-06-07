// scdpdrView.cpp : implementation of the CScdpdrView class
//

#include "stdafx.h"
#include "scdpdr.h"

#include "scdpdrDoc.h"
#include "scdpdrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScdpdrView

IMPLEMENT_DYNCREATE(CScdpdrView, CView)

BEGIN_MESSAGE_MAP(CScdpdrView, CView)
	//{{AFX_MSG_MAP(CScdpdrView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScdpdrView construction/destruction

CScdpdrView::CScdpdrView()
{
	// TODO: add construction code here

}

CScdpdrView::~CScdpdrView()
{
}

BOOL CScdpdrView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CScdpdrView drawing

void CScdpdrView::OnDraw(CDC* pDC)
{
	CScdpdrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CScdpdrView printing

BOOL CScdpdrView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CScdpdrView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CScdpdrView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CScdpdrView diagnostics

#ifdef _DEBUG
void CScdpdrView::AssertValid() const
{
	CView::AssertValid();
}

void CScdpdrView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScdpdrDoc* CScdpdrView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScdpdrDoc)));
	return (CScdpdrDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScdpdrView message handlers
