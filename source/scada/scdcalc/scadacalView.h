// scadacalView.h : interface of the CScadacalView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCADACALVIEW_H__884DD67A_7E7A_11D2_88F2_0000F805FD11__INCLUDED_)
#define AFX_SCADACALVIEW_H__884DD67A_7E7A_11D2_88F2_0000F805FD11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CScadacalView : public CView
{
protected: // create from serialization only
	CScadacalView();
	DECLARE_DYNCREATE(CScadacalView)

// Attributes
public:
	CScadacalDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScadacalView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScadacalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CScadacalView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in scadacalView.cpp
inline CScadacalDoc* CScadacalView::GetDocument()
   { return (CScadacalDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCADACALVIEW_H__884DD67A_7E7A_11D2_88F2_0000F805FD11__INCLUDED_)
