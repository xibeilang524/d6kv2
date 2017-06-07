// WebManageView.h : interface of the CWebManageView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBMANAGEVIEW_H__B23F2ED6_7849_42B1_B8F7_FBB04810B981__INCLUDED_)
#define AFX_WEBMANAGEVIEW_H__B23F2ED6_7849_42B1_B8F7_FBB04810B981__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWebManageView : public CView
{
protected: // create from serialization only
	CWebManageView();
	DECLARE_DYNCREATE(CWebManageView)

// Attributes
public:
	CWebManageDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebManageView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWebManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWebManageView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WebManageView.cpp
inline CWebManageDoc* CWebManageView::GetDocument()
   { return (CWebManageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBMANAGEVIEW_H__B23F2ED6_7849_42B1_B8F7_FBB04810B981__INCLUDED_)
