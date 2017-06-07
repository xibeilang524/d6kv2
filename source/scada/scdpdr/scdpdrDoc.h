// scdpdrDoc.h : interface of the CScdpdrDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCDPDRDOC_H__7C1A22A6_DDD0_4E8E_BC88_810393FA964F__INCLUDED_)
#define AFX_SCDPDRDOC_H__7C1A22A6_DDD0_4E8E_BC88_810393FA964F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CScdpdrDoc : public CDocument
{
protected: // create from serialization only
	CScdpdrDoc();
	DECLARE_DYNCREATE(CScdpdrDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScdpdrDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScdpdrDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CScdpdrDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDPDRDOC_H__7C1A22A6_DDD0_4E8E_BC88_810393FA964F__INCLUDED_)
