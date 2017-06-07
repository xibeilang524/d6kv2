// WebManageView.cpp : implementation of the CWebManageView class
//

#include "stdafx.h"
#include "WebManage.h"

#include "WebManageDoc.h"
#include "WebManageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebManageView

IMPLEMENT_DYNCREATE(CWebManageView, CView)

BEGIN_MESSAGE_MAP(CWebManageView, CView)
	//{{AFX_MSG_MAP(CWebManageView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebManageView construction/destruction

CWebManageView::CWebManageView()
{
	// TODO: add construction code here

}

CWebManageView::~CWebManageView()
{
}

BOOL CWebManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWebManageView drawing

void CWebManageView::OnDraw(CDC* pDC)
{
	CWebManageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWebManageView printing

BOOL CWebManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWebManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWebManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWebManageView diagnostics

#ifdef _DEBUG
void CWebManageView::AssertValid() const
{
	CView::AssertValid();
}

void CWebManageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWebManageDoc* CWebManageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWebManageDoc)));
	return (CWebManageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebManageView message handlers
