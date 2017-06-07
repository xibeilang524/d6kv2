// DlgSysConfig.cpp : implementation file
//

#include "stdafx.h"
#include "WebManage.h"
#include "DlgSysConfig.h"
#include "webstruct.h"
#include "db.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConfig dialog
extern int readRecorders(char* dbname,char* tablename,char* fieldname, char* where,char* orderfn,int bdistinct,int *fieldcount,void **buf);
extern CDB dbo;
extern void loadExcludeEvent();
extern char DBNAME[64];

#ifdef WIN32
extern int strcmpNoCase(const char * string1, const char * string2);
#else
int strcmpNoCase(const char * string1, const char * string2)
{
#ifdef WIN32
	return _stricmp(string1,string2);
#else
	return strcasecmp(string1,string2);
#endif
}
#endif

CDlgSysConfig::CDlgSysConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysConfig)
	//}}AFX_DATA_INIT
}


void CDlgSysConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysConfig)
	DDX_Control(pDX, IDC_COMBO_GROUP, m_combo_group);
	DDX_Control(pDX, IDC_LIST_SELTYPE, m_list_seltype);
	DDX_Control(pDX, IDC_LIST_EVENTSORT, m_list_eventsort);
	DDX_Control(pDX, IDC_LIST_UNSEL, m_list_unsel);
	DDX_Control(pDX, IDC_BTN_DOWNALL, m_btn_downall);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btn_down);
	DDX_Control(pDX, IDC_BTN_ADDALL, m_btn_addall);
	DDX_Control(pDX, IDC_BTN_ADD, m_btn_all);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgSysConfig)
	ON_BN_CLICKED(IDC_BTN_ADDALL, OnBtnAddall)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_DOWNALL, OnBtnDownall)
	ON_LBN_SELCHANGE(IDC_LIST_EVENTSORT, OnSelchangeListEventsort)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelchangeComboGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConfig message handlers

void CDlgSysConfig::OnBtnAddall() 
{
	// TODO: Add your control notification handler code here
	int count = m_list_seltype.GetCount();
	CString selStr;
	for(int i=0;i<count;i++)
	{
		m_list_seltype.GetText(i,selStr);
		m_list_unsel.AddString(selStr);
		DWORD itemdata = m_list_seltype.GetItemData(i);
		m_list_unsel.SetItemData(m_list_unsel.GetCount()-1,itemdata);
	}

	m_list_seltype.ResetContent();
	changef = true;

}

void CDlgSysConfig::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	int count = m_list_seltype.GetCount();
	if(count>0)
	{
		for(int i=0;i<count;i++)
		{
			int selstatus = m_list_seltype.GetSel(count-i-1); 
			if(selstatus>0)
			{
				CString selStr;
				m_list_seltype.GetText(count-i-1,selStr);
				m_list_unsel.AddString(selStr);
				DWORD itemdata = m_list_seltype.GetItemData(count-i-1);
				m_list_unsel.SetItemData(m_list_unsel.GetCount()-1,itemdata);
				m_list_seltype.DeleteString(count-i-1);
				changef = true;
			}
		}
	}
}

void CDlgSysConfig::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	int count = m_list_unsel.GetCount();
	if(count>0)
	{
		for(int i=0;i<count;i++)
		{
			int selstatus = m_list_unsel.GetSel(count-i-1); 
			if(selstatus>0)
			{
				CString selStr;
				m_list_unsel.GetText(count-i-1,selStr);
				m_list_seltype.AddString(selStr);
				DWORD itemdata = m_list_unsel.GetItemData(count-i-1);
				m_list_seltype.SetItemData(m_list_seltype.GetCount()-1,itemdata);
				m_list_unsel.DeleteString(count-i-1);
				changef = true;
			}
		}
	}
}

void CDlgSysConfig::OnBtnDownall() 
{
	// TODO: Add your control notification handler code here
	int count = m_list_unsel.GetCount();
	CString selStr;
	for(int i=0;i<count;i++)
	{
		m_list_unsel.GetText(i,selStr);
		m_list_seltype.AddString(selStr);
		DWORD itemdata = m_list_unsel.GetItemData(i);
		m_list_seltype.SetItemData(m_list_seltype.GetCount()-1,itemdata);
	}
	m_list_unsel.ResetContent();
	changef = true;
}

BOOL CDlgSysConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//defined in sysdef.h
	m_list_eventsort.AddString("设备及程序事件");//ENTCLASS_DEVICE//1 系统中的设备及程序运行事件
	m_list_eventsort.AddString("SCADA事件");//ENTCLASS_SCADA,//2 SCADA事件
	m_list_eventsort.AddString("综合自动化事件");//ENTCLASS_PROTECT,//3 综合自动化保护事件
	m_list_eventsort.AddString("DAS应用事件");//ENTCLASS_DAS,//4 DAS应用事件
	//m_list_eventsort.AddString("DA应用事件");//ENTCLASS_DA,	//4 DA应用事件
	//m_list_eventsort.AddString("DA研究态事件");//ENTCLASS_DA_STUDY,//4 DA研究态应用事件
	m_list_eventsort.AddString("EMS应用事件");//ENTCLASS_EMS,//5 EMS应用事件
	m_list_eventsort.AddString("操作事件");//ENTCLASS_OPERATION,//6 操作事件
	m_list_eventsort.AddString("通知事件");//ENTCLASS_NOTICE,//7 通知事件
	m_list_eventsort.AddString("WEB应用事件");//ENTCLASS_WEB,//8 WEB应用事件
	m_list_eventsort.AddString("维护类事件");//ENTCLASS_MAINT,	//9 维护类应用事件
	m_bDbFlag = m_db.Open(DB_OP_WR, FALSE);
	
	m_combo_group.AddString("管理员");
	m_combo_group.SetItemData(0,1);//根据web用户表，管理员就是1
	m_combo_group.AddString("SCADA");
	m_combo_group.SetItemData(1,2);
	m_combo_group.AddString("PAS");
	m_combo_group.SetItemData(2,3);
	m_combo_group.AddString("缺省");
	m_combo_group.SetItemData(3,0);

	m_curSOrt = -1;
	changef = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysConfig::OnSelchangeListEventsort() 
{
	// TODO: Add your control notification handler code here
	int item = m_list_eventsort.GetCurSel();
	if(item != LB_ERR )
	{
		int sort = -1;
		CString selStr;
		m_list_eventsort.GetText(item,selStr);
		if(selStr == "设备及程序事件")
		{
			sort = 1;
		}
		else if(selStr == "SCADA事件")
		{
			sort = 2;
		}
		else if(selStr == "综合自动化事件")
		{
			sort = 3;
		}
		else if(selStr == "DAS应用事件")
		{
			sort = 4;
		}
		else if(selStr == "DA应用事件")
		{
			sort = 4;
		}
		else if(selStr == "DA研究态事件")
		{
			sort = 4;
		}
		else if(selStr ==  "EMS应用事件")
		{
			sort = 5;
		}
		else if(selStr == "操作事件")
		{
			sort = 6;
		}
		else if(selStr == "通知事件")
		{
			sort = 7;
		}
		else if(selStr == "WEB应用事件")
		{
			sort = 8;
		}
		else if(selStr == "维护类事件")
		{
			sort = 9;
		}
		listType(sort);
		m_curSOrt = sort;
	}
}

void CDlgSysConfig::listType(int sort)
{
	int nindex = m_combo_group.GetCurSel();
	if(nindex == LB_ERR) return;

	if(changef)//改变标志
	{
		if(!m_bDbFlag) {m_bDbFlag = m_db.Open(DB_OP_WR, FALSE);}
		
		if(m_bDbFlag)
		{
			CS_DATAFMT *datafmt = NULL;
			void* pt = NULL;
			FUNC_STR func;

			memset(&func,0,sizeof(FUNC_STR));
			func.func = GET_TABLE_INFO;
			strcpy(func.table,"web事件管理表");
			strcpy(func.dbname,DBNAME);

			if(m_db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
			{
				func.func = ISQL_COMMAND;
				func.roxnum = 1;
				char where[128];
				memset(where,0,128);
				sprintf(where,"delete web事件管理表 where 分类号=%d and 组名=%d",m_curSOrt,m_combo_group.GetItemData(nindex));
				strcpy(func.isql,where);

				//char pt;
				//m_db.DeleteRcds(&func,datafmt,&pt);
				m_db.IsqlCommand (&func);

				func.func = INS_ONE_RECORDER;
				char listText[128];
				for(int k=0;k<m_list_unsel.GetCount();k++)
				{
					func.col[0].val.smallvalue = m_curSOrt;//分类号
					func.col[1].val.smallvalue = (short)m_list_unsel.GetItemData(k);//类型号
					memset(listText,0,128);
					m_list_unsel.GetText(k,listText);
					strcpy(func.col[2].val.charvalue,listText);//类型名
					func.col[3].val.tinyvalue = (CS_TINYINT)m_combo_group.GetItemData(nindex);
					m_db.InsertOneRcd(&func,datafmt);
				}
			}
			changef = false;
			loadExcludeEvent();
		}
	}
	int fieldcount;
	void *buf = NULL,*buf1 = NULL;
	NAME *pbuf = NULL,*pbuf1 = NULL;

	char where[128];
	memset(where,0,128);
	sprintf(where,"分类号=%d",sort);
	m_list_seltype.ResetContent();
	m_list_unsel.ResetContent();
	int sel_type_no = 0;
	int unsel_type_no = 0;
	int ret = readRecorders(DBNAME,"事项类型表","类型名,类型号",where,"",true,&fieldcount,&buf);
	if(ret>0)
	{
		pbuf = (NAME*)buf;
		memset(where,0,128);
		sprintf(where,"分类号=%d and 组名=%d",sort,m_combo_group.GetItemData(nindex));
		int ret1 = readRecorders(DBNAME,"web事件管理表","类型名,类型号",where,"",true,&fieldcount,&buf1);
		pbuf1 = (NAME*)buf1;
		int i, j;
		for(i=0;i<ret/2;i++)
		{
			for(j=0;j<ret1/2;j++)
			{
				if(!strcmpNoCase(pbuf[i*2].name,pbuf1[j*2].name))//类型名,相等
					break;
			}
			if(j >= ret1/2)//"web管理事件表"中没有此记录
			{
				m_list_seltype.AddString(pbuf[i*2].name);//选中
				m_list_seltype.SetItemData(sel_type_no,atoi(pbuf[i*2+1].name));
				sel_type_no ++;
			}
			else
			{
				m_list_unsel.AddString(pbuf1[j*2].name);//未选中
				m_list_unsel.SetItemData(unsel_type_no,atoi(pbuf1[j*2+1].name));
				unsel_type_no ++;
			}
		}
		FREE(pbuf);
		FREE(pbuf1);
	}
	else
	{
		//不处理
	}
}


void CDlgSysConfig::OnSelchangeComboGroup() 
{
	// TODO: Add your control notification handler code here
	m_list_seltype.ResetContent();
	m_list_unsel.ResetContent();
	m_list_eventsort.SetCurSel(-1);
}
