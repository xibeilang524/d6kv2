// scadacalView.cpp : implementation of the CScadacalView class
//

#include "stdafx.h"
#include "scadacal.h"

#include "scadacalDoc.h"
#include "scadacalView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScadacalView

IMPLEMENT_DYNCREATE(CScadacalView, CView)

BEGIN_MESSAGE_MAP(CScadacalView, CView)
	//{{AFX_MSG_MAP(CScadacalView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScadacalView construction/destruction

CScadacalView::CScadacalView()
{
	// TODO: add construction code here

}

CScadacalView::~CScadacalView()
{
}

BOOL CScadacalView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CScadacalView drawing

void CScadacalView::OnDraw(CDC* pDC)
{
	CScadacalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CScadacalView diagnostics

#ifdef _DEBUG
void CScadacalView::AssertValid() const
{
	CView::AssertValid();
}

void CScadacalView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScadacalDoc* CScadacalView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScadacalDoc)));
	return (CScadacalDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScadacalView message handlers
