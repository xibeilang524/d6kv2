// scdpdrView.h : interface of the CScdpdrView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCDPDRVIEW_H__2EDDEB71_7014_42A5_8DB7_EBA1E92731EA__INCLUDED_)
#define AFX_SCDPDRVIEW_H__2EDDEB71_7014_42A5_8DB7_EBA1E92731EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CScdpdrView : public CView
{
protected: // create from serialization only
	CScdpdrView();
	DECLARE_DYNCREATE(CScdpdrView)

// Attributes
public:
	CScdpdrDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScdpdrView)
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
	virtual ~CScdpdrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CScdpdrView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in scdpdrView.cpp
inline CScdpdrDoc* CScdpdrView::GetDocument()
   { return (CScdpdrDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDPDRVIEW_H__2EDDEB71_7014_42A5_8DB7_EBA1E92731EA__INCLUDED_)
