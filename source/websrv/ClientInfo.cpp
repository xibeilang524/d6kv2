// ClientInfo.cpp : implementation file
//

#include "stdafx.h"
#include "WebManage.h"
#include "ClientInfo.h"

#include "webstruct.h"
#include "externdefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientInfo dialog

extern THREADMANAGE threadinfo[INITCLIENTNUMS];
extern int  clntchangef ;//客户端变动标志
extern int clientnums;


CClientInfo::CClientInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CClientInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientInfo)
	//}}AFX_DATA_INIT
}


void CClientInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientInfo)
	DDX_Control(pDX, IDC_STATIC_FRAME, m_static_frame);
	DDX_Control(pDX, IDOK, m_idok);
	DDX_Control(pDX, IDC_STATIC_CLNTSUM, m_static_clntsum);
	DDX_Control(pDX, IDC_EDIT_CLNTSUM, m_edit_clntsum);
	DDX_Control(pDX, IDC_CLNTINFO_LIST, m_clntinfo_list);
	DDX_Control(pDX, IDC_BTN_KILL, m_btn_kill);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientInfo, CDialog)
	//{{AFX_MSG_MAP(CClientInfo)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_KILL, OnBtnKill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientInfo message handlers

BOOL CClientInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText("在线客户列表");
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);

	m_clntinfo_list.SetImageList(NULL,LVSIL_NORMAL);
	m_clntinfo_list.SetImageList(NULL,LVSIL_STATE);

	/*m_clntinfo_list.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	m_clntinfo_list.InsertColumn(1,"主机名",LVCFMT_CENTER,120);
	m_clntinfo_list.InsertColumn(2,"IP地址",LVCFMT_CENTER,150);
	m_clntinfo_list.InsertColumn(3,"起始连接时间",LVCFMT_CENTER,200);
	m_clntinfo_list.InsertColumn(4,"状态",LVCFMT_CENTER,100);
	m_clntinfo_list.InsertColumn(5,"备注",LVCFMT_CENTER,200);
	m_clntinfo_list.InsertColumn(6,"管理号",LVCFMT_CENTER,200);*/

	m_clntinfo_list.InsertColumn(0,"序号",LVCFMT_CENTER,50);
	//m_clntinfo_list.InsertColumn(1,"主机名",LVCFMT_CENTER,120);
	m_clntinfo_list.InsertColumn(1,"IP地址",LVCFMT_CENTER,150);
	m_clntinfo_list.InsertColumn(2,"起始连接时间",LVCFMT_CENTER,200);
	m_clntinfo_list.InsertColumn(3,"状态",LVCFMT_CENTER,100);
	m_clntinfo_list.InsertColumn(4,"备注",LVCFMT_CENTER,200);
	m_clntinfo_list.InsertColumn(5,"管理号",LVCFMT_CENTER,200);

	ListCtrlAddExStyle(m_clntinfo_list.GetSafeHwnd(),LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
	listclntinfo();
	SetTimer(1,2000,NULL);
	AlignControls();
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClientInfo::listclntinfo()
{
	int order = 0;
	m_clntinfo_list.DeleteAllItems();
	LV_ITEM lv; 
	lv.mask = LVIF_TEXT;
	CString strtmp;
	for(int threadno=0;threadno<clientnums;threadno++)
	{
		if(threadinfo[threadno].runf == 1)
		{
			lv.iItem = order;
			lv.iSubItem = 0;//refers to an item rather than a subitem
			strtmp.Format("%d",order);
			lv.pszText = (char*)LPCSTR(strtmp);//sztext;
			lv.cchTextMax = 6;
			m_clntinfo_list.InsertItem(&lv);

			//m_clntinfo_list.SetItemText(order,1,threadinfo[threadno].client_name);//Hostname
			m_clntinfo_list.SetItemText(order,1,threadinfo[threadno].client_addr);//Nodes	
			
			CTime t = CTime(threadinfo[threadno].starttime);
			strtmp.Format("%02d日%02d:%02d:%02d",t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
			m_clntinfo_list.SetItemText(order,2,LPCSTR(strtmp));
			if(threadinfo[threadno].validf < 0) //-1,-2
				strtmp = "异常";
			else
				strtmp = "正常";
			m_clntinfo_list.SetItemText(order,3,LPCSTR(strtmp));

			if(threadinfo[threadno].clienttype == WEBSCADA)
			{
				strtmp = "Scada";
			}
			else if(threadinfo[threadno].clienttype == GISTOWEB)
			{
				strtmp = "Gis";
			}
			else if(threadinfo[threadno].clienttype == WEBTABLE)
			{
				strtmp = "报表";
			}
			else
			{
				strtmp = "";
			}
			m_clntinfo_list.SetItemText(order,4,LPCSTR(strtmp));
			strtmp.Format("No:%d",threadno);
			m_clntinfo_list.SetItemText(order,5,LPCSTR(strtmp));
			m_clntinfo_list.SetItemData(order,threadno);
			order++;
		}
	}//for(i=0...)
	ListctrlAutoSizeColumns(m_clntinfo_list);
	strtmp.Format("%d",order);
	GetDlgItem(IDC_EDIT_CLNTSUM)->SetWindowText(strtmp);
}

void CClientInfo::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//判断客户连接情况是否变化，否则不刷新
	if(clntchangef)
	{
		listclntinfo();
		clntchangef = FALSE;
	}
	CDialog::OnTimer(nIDEvent);
}

void CClientInfo::ListCtrlAddExStyle(HWND hWnd, DWORD dwNewStyle)
{
	// Returns the current extended style ( a DWORD ).
	LRESULT dwStyle = ::SendMessage (hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

	// Add the full row select and grid line style to the existing extended styles.
	dwStyle |= dwNewStyle;

	// Sets the current extended style ( a DWORD ).
	::SendMessage (hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

void CClientInfo::ListctrlAutoSizeColumns(CListCtrl & listctrl)
{
	 //listctrl.SetRedraw(FALSE);
	 CHeaderCtrl* pHeader = (CHeaderCtrl*)listctrl.GetDlgItem(0);

	 int maxcol = pHeader->GetItemCount()-1;
	 for(int col = 0; col <= maxcol; col++){
          listctrl.SetColumnWidth(col,LVSCW_AUTOSIZE);
          int wc1 = listctrl.GetColumnWidth(col);
          listctrl.SetColumnWidth(col,LVSCW_AUTOSIZE_USEHEADER);
          int wc2 = listctrl.GetColumnWidth(col);
		  int wc = max(10,max(wc1,wc2));
          listctrl.SetColumnWidth(col,wc);
     }
}

void CClientInfo::OnSize(UINT nType, int cx, int cy) 
{

	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(cx >= 578 && cy >= 362)
	{
		GetWindowRect(&m_rect);
		AlignControls();
	}
	else
	{
		this->MoveWindow(&m_rect);
	}

}

void CClientInfo::AlignControls()
{
	RECT rect,rect1;
	GetClientRect(&rect);
	rect1.left = rect.left;
	rect1.top = rect.top;
	rect1.right = rect.right;
	rect1.bottom = rect.bottom - 60;
	if(::IsWindow(m_clntinfo_list.m_hWnd))
	{
		m_clntinfo_list.MoveWindow(rect1.left,rect1.top,rect1.right,rect1.bottom);//CListCtrl
		rect1.left = rect.left ;//delete
		rect1.top = rect1.bottom + 3;
		rect1.right = rect.right ;
		rect1.bottom = rect.bottom - 3;
		m_static_frame.MoveWindow(&rect1);//Frame
		rect1.top = rect1.top + 15;
		rect1.left = rect1.left + 30;
		rect1.right = rect1.left + 130;//int(rect.right * 0.25) -3;
		rect1.bottom = rect1.top + 30;
		m_static_clntsum.MoveWindow(&rect1); //在线客户总数  
		rect1.top = rect1.top -3 ;
		rect1.left = rect1.right + 3;//int(rect.right * 0.25) + 3;
		rect1.right = int(rect.right * 0.5) - 30;
		rect1.bottom = rect1.top + 30;
		m_edit_clntsum.MoveWindow (&rect1);
		rect1.top = rect1.top ;
		rect1.left = int(rect.right*0.5) +1 ;
		rect1.right = int(rect.right*0.75) - 3;
		rect1.bottom = rect1.top + 30;
		m_btn_kill.MoveWindow(&rect1);//断开连接
		rect1.top = rect1.top ;
		rect1.left = int(rect.right*0.75) + 3;
		rect1.right = rect.right - 3;
		rect1.bottom = rect1.top + 30;
		m_idok.MoveWindow (&rect1);//退出
		Invalidate();
	}
}

void CClientInfo::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CClientInfo::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

void CClientInfo::OnBtnKill() //断开连接
{
	//只能关掉1个，关多时，刷新快，会出错
	// TODO: Add your control notification handler code here
	int selcount = m_clntinfo_list.GetSelectedCount() ;
	if(selcount>0)
	{
		int threadno = 0;
		POSITION pos;
		pos = m_clntinfo_list.GetFirstSelectedItemPosition();

		if (pos == NULL)
		   TRACE0("No items were selected!\n");
		else
		{
		   while (pos)
		   {
			  int nItem = m_clntinfo_list.GetNextSelectedItem(pos);
			  threadno = m_clntinfo_list.GetItemData(nItem);
		   }
		}
		MESSAGE msg;
		msg.type = S_SRVEXIT;//设置消息类型
		msg.length = 0;//设置消息的长度
		writemsg(threadinfo[threadno].sockid,&msg);
		threadinfo[threadno].validf = INVALIDF;

	}
}
