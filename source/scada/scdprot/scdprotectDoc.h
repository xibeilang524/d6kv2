// scdprotectDoc.h : interface of the CScdprotectDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCDPROTECTDOC_H__682B970C_C355_11D3_B860_00C04FCCE0E8__INCLUDED_)
#define AFX_SCDPROTECTDOC_H__682B970C_C355_11D3_B860_00C04FCCE0E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CScdprotectDoc : public CDocument
{
protected: // create from serialization only
	CScdprotectDoc();
	DECLARE_DYNCREATE(CScdprotectDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScdprotectDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScdprotectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CScdprotectDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDPROTECTDOC_H__682B970C_C355_11D3_B860_00C04FCCE0E8__INCLUDED_)
