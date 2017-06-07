// scadacalDoc.h : interface of the CScadacalDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCADACALDOC_H__884DD678_7E7A_11D2_88F2_0000F805FD11__INCLUDED_)
#define AFX_SCADACALDOC_H__884DD678_7E7A_11D2_88F2_0000F805FD11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CScadacalDoc : public CDocument
{
protected: // create from serialization only
	CScadacalDoc();
	DECLARE_DYNCREATE(CScadacalDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScadacalDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScadacalDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CScadacalDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCADACALDOC_H__884DD678_7E7A_11D2_88F2_0000F805FD11__INCLUDED_)
