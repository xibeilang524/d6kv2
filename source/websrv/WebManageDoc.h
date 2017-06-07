// WebManageDoc.h : interface of the CWebManageDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBMANAGEDOC_H__2DD32AEF_8A81_445F_8589_53792ADF9BBB__INCLUDED_)
#define AFX_WEBMANAGEDOC_H__2DD32AEF_8A81_445F_8589_53792ADF9BBB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CWebManageDoc : public CDocument
{
protected: // create from serialization only
	CWebManageDoc();
	DECLARE_DYNCREATE(CWebManageDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebManageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWebManageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWebManageDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBMANAGEDOC_H__2DD32AEF_8A81_445F_8589_53792ADF9BBB__INCLUDED_)
