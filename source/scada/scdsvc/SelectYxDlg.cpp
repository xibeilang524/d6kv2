// SelectYxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scada.h"
#include "SelectYxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SelectYxDlg dialog


SelectYxDlg::SelectYxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SelectYxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SelectYxDlg)
	m_sName = _T("");
	//}}AFX_DATA_INIT
}


void SelectYxDlg::DoDataExchange(CDataExchange* pDX)
{
	CheckRadioButton(IDC_RADIO_BREAK,IDC_RADIO_PROTSIGN,IDC_RADIO_BREAK);
	CheckRadioButton(IDC_RADIO_OFF,IDC_RADIO_ON,IDC_RADIO_OFF);
	
	int err = GetLastError();
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectYxDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectYxDlg, CDialog)
	//{{AFX_MSG_MAP(SelectYxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SelectYxDlg message handlers

void SelectYxDlg::OnOK() 
{
	// TODO: Add extra validation here
	breakorswitch=GetCheckedRadioButton
		(IDC_RADIO_BREAK,IDC_RADIO_PROTSIGN)-IDC_RADIO_BREAK;
	offoron=GetCheckedRadioButton
		(IDC_RADIO_OFF,IDC_RADIO_ON)-IDC_RADIO_OFF;
	int err = GetLastError();
	CDialog::OnOK();
}
