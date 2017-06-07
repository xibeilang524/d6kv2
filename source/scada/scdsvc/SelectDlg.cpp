// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scada.h"
#include "SelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog


CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDlg)
	m_endbreak = _T("");
	m_endftu = _T("");
	m_faulttype = 0;
	m_startbreak = _T("");
	m_startftu = _T("");
	//}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDlg)
	DDX_Text(pDX, IDC_EDIT_endbreak, m_endbreak);
	DDV_MaxChars(pDX, m_endbreak, 12);
	DDX_Text(pDX, IDC_EDIT_endftu, m_endftu);
	DDV_MaxChars(pDX, m_endftu, 12);
	DDX_Text(pDX, IDC_EDIT_faulttype, m_faulttype);
	DDV_MinMaxInt(pDX, m_faulttype, 1, 7);
	DDX_Text(pDX, IDC_EDIT_startbreak, m_startbreak);
	DDV_MaxChars(pDX, m_startbreak, 12);
	DDX_Text(pDX, IDC_EDIT_startftu, m_startftu);
	DDV_MaxChars(pDX, m_startftu, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers
