#include "QuserWnd.h"

#include "globaldef.h"
#include "sysdef.h"
#include "db/dbapi.h"
#include "QLoginWnd.h"
#include "QAddNode.h"
#include "QusercfgDlg.h"
#include "uitools/login_widget.h"
#include "uitools/dmessagebox.h"

#include <QListView>
#include <qtoolbutton.h>
#include <QMessageBox>
#include <qtabwidget.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <qcombobox.h>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtDebug>


extern NetDbg netdbg;

QuserWnd::QuserWnd( QWidget* parent, const char* name ):QMainWindow( parent )
{
	setupUi(this);
	m_userChanged = FALSE;
	m_roleChanged = FALSE;
	m_userAuthChanged = FALSE;


	m_userPageFresh = TRUE;
	m_rolecfgFresh = FALSE;
	m_MangerPageFresh = TRUE;


	m_beforeSelRow = -1;
	if( !m_zonePara.readPara() )		// modify [4/9/2012 zhoujian]
	{
		DMessageBox::warning(this, _C2Q("警告"), _C2Q( "读数据库失败!" ) );
		close();
	}

	m_bchangedAuth = FALSE;
	m_roleauthChanged = FALSE;	

	m_userTbWidget->setEditTriggers(QAbstractItemView::NoEditTriggers ); // 不可编辑
	m_userTbWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // ljs(整行选中方式) [4/17/2012]

	m_tabWidget->setTabEnabled( 0,TRUE );
	m_tabWidget->setTabEnabled( 1,FALSE );
	m_tabWidget->setTabEnabled( 2,FALSE );
	m_tabWidget->setCurrentIndex(3);
	actionSave->setEnabled(FALSE);
	actionLoad->setEnabled(FALSE);
	m_addRoleBtn->setEnabled(FALSE);
	m_delRoleBtn->setEnabled(FALSE);
	m_appBtn->setEnabled(FALSE);
	m_rolelAll->setEnabled(FALSE);
	m_rolelNotAll->setEnabled(FALSE);

// 	QPixmap *pixmap0 = new QPixmap( ":/res/login.png" );
// 	ui.m_loginBtn->setIconSet(QIconSet(pixmap0));

// 	m_loginBtn->setIcon(QIcon(":/res/login.png"));
// 	m_exitBtn->setIcon(QIcon(":/res/application-exit.png"));
// 	m_saveBtn->setIcon(QIcon(":/res/dbsave.png"));
// 	m_loadBtn->setIcon(QIcon(":/res/dbupdate.png"));
	m_aplauthBtn->setIcon(QIcon(":/res/Node-apply.png"));
	m_addauthBtn->setIcon(QIcon(":/res/Node-more.png"));
	m_delauthBtn->setIcon(QIcon(":/res/Node-fewer.png"));
	m_appBtn->setIcon(QIcon(":/res/Node-apply.png"));
	m_addRoleBtn->setIcon(QIcon(":/res/Node-more.png"));
	m_delRoleBtn->setIcon(QIcon(":/res/Node-fewer.png"));
	m_addUserBtn->setIcon(QIcon(":/res/user-new.png"));
	m_delUserBtn->setIcon(QIcon(":/res/user-delete.png"));
	m_modUserBtn->setIcon(QIcon(":/res/Node-apply.png"));

	connect(m_tabWidget, SIGNAL(currentChanged(QWidget*)), this, SLOT( onCurrentPageChanged(QWidget*) ) ); // TAB换页
// 	connect(m_loginBtn, SIGNAL(clicked() ), this, SLOT( onLogin() ) );  // 登录按钮
// 	connect(m_exitBtn, SIGNAL(clicked() ), this, SLOT( onExit() ) );    // 退出按钮
// 	connect(m_saveBtn, SIGNAL(clicked() ), this, SLOT( onSave() ) );		// 保存按钮
// 	connect(m_loadBtn, SIGNAL( clicked() ), this, SLOT( onLoad() ) );	// 加载按钮
	connect(actionLogin, SIGNAL(triggered() ), this, SLOT( onLogin() ) );  // 登录按钮
	connect(actionExit, SIGNAL(triggered() ), this, SLOT( onExit() ) );    // 退出按钮
	connect(actionSave, SIGNAL(triggered() ), this, SLOT( onSave() ) );		// 保存按钮
	connect(actionLoad, SIGNAL( triggered() ), this, SLOT( onLoad() ) );	// 加载按钮

	connect(m_addRoleBtn, SIGNAL( clicked() ), this, SLOT( onAddRole() ) );		//	角色界面 添加按钮
	connect(m_delRoleBtn, SIGNAL( clicked() ), this, SLOT( onDeleteRole() ) );		//角色界面 删除按钮
	connect(m_appBtn, SIGNAL( clicked() ), this, SLOT( onApply() ) );		//角色界面 修改按钮
	connect(m_rolelAll, SIGNAL( clicked() ), this, SLOT( onSelAllRole() ) );	//角色界面 全选按钮
	connect(m_rolelNotAll, SIGNAL( clicked() ), this, SLOT( onNotSelAllRole() ) );		//角色界面 全不选按钮

	connect(m_aplauthBtn, SIGNAL(clicked() ), this, SLOT( onModifyNode() ) );	// 权限管理界面 应用按钮
	connect(m_addauthBtn, SIGNAL(clicked() ), this, SLOT( onAddNode() ) );	// 权限管理界面 添加按钮
	connect(m_delauthBtn, SIGNAL(clicked() ), this, SLOT( onDelNode() ) );	// 权限管理界面 删除按钮
	connect(m_userTW, SIGNAL(itemSelectionChanged () ), this, SLOT( 	ViewAuth() ) );		//权限管理界面 读取选中角色的权限

	connect(m_roleTable, SIGNAL(itemSelectionChanged()), this, SLOT( onreadAuth() ) ); // 角色界面 读取选中角色的权限
	connect(m_selAllAuth, SIGNAL(clicked () ), this, SLOT( onSelAllAuth() ) );	
	connect(m_selEptAuth, SIGNAL(clicked () ), this, SLOT( onSelEptAuth() ) );	

    connect(m_addUserBtn, SIGNAL(clicked()), this, SLOT(onAddUser()));		// 用户管理界面 添加用户
    connect(m_delUserBtn, SIGNAL(clicked()), this, SLOT(onDeleteUser()));	// 用户管理界面 删除用户
	connect(m_modUserBtn, SIGNAL(clicked()),this,SLOT(onModifyUser()));		// 用户管理界面 修改用户

	initStyleSheet() ;
}

QuserWnd::~QuserWnd()
{
	
}

void QuserWnd::onLogin()
{
	actionLogin->setChecked(false);
// 	QLoginWnd loginwnd( this );
	Login_widget loginwnd ;
	if( loginwnd.exec() == QDialog::Accepted )
	{
		m_tabWidget->setTabEnabled( 0,TRUE );
		m_tabWidget->setTabEnabled( 1,TRUE );
		m_tabWidget->setTabEnabled( 2,TRUE );

		DISPATCHER_STRU oper_info ;
		loginwnd.get_oper_info( oper_info ) ;
		m_roletype = oper_info.type;
		m_grpleader = oper_info.ismonitor;
	}
	else
	{
		return;
	}

	freshrolePage();
	freshroleCfgPage();
//	freshMangerCfgPage();
	freshUserPage();

	m_userItem2Node.clear();

	actionLogin->setEnabled(FALSE);
//	m_MangerPageFresh = TRUE;
}

void QuserWnd::onExit()
{
	close();
}

void QuserWnd::onSave()
{
	actionSave->setChecked(false);

	actionSave->setEnabled( FALSE );
	actionLoad->setEnabled( TRUE );

	if(m_roleauthChanged) RoleToUser();

	if(m_delrolelist.count())
		m_zonePara.DelRolePara(&m_delrolelist);
	if(m_addrolelist.count())
		m_zonePara.AddRolePara(&m_addrolelist);

	if(m_delyuserlist.count())
		m_zonePara.DelNodePara(&m_delyuserlist);
	if(m_addyuserlist.count())
		m_zonePara.AddNodePara(&m_addyuserlist);

	if(m_addusercfglist.count())
		m_zonePara.AddUserPara(&m_addusercfglist);
	if(m_delusercfglist.count())
		m_zonePara.DelUserPara(&m_delusercfglist);
	if(m_chageusercfglist.count())
		m_zonePara.saveUserCfgPara(&m_chageusercfglist);

	m_delrolelist.clear();
	m_addrolelist.clear();
	m_addyuserlist.clear();
	m_delyuserlist.clear();
	m_addusercfglist.clear();
	m_delusercfglist.clear();
	m_chageusercfglist.clear();

	m_zonePara.savePara();

	m_roleauthChanged = FALSE;
	m_bchangedAuth = FALSE;
	m_roleChanged = FALSE;
	
	
	m_userPageFresh = TRUE;
}

void QuserWnd::onLoad()
{
	actionLoad->setChecked(false);

	actionLoad->setEnabled( FALSE );

	RtdbProc rtdbproc;

	if( m_userAuthChanged )
	{
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_ALLHOST, "节点权限参数表" );
	}

	m_userAuthChanged = FALSE;
}

void QuserWnd::onAddRole()    //添加角色 libinghong 2012-04-09vv
{
	
	PNODEPARA pn,pnpn;
	bool ok;
	uint num = 0;
	QString rolename = QInputDialog::getText( this, _C2Q( "请输入角色名称" ), _C2Q( "角色名称" ), 
		QLineEdit::Normal, QString::null, &ok  ); 
	pn = new NODEPARA;
	if (ok)	
	{
		pnpn=m_zonePara.m_roleList.last();
		pn->sname = rolename;
		pn->devtype = pnpn->devtype + 1 ;
		pn->zone1 = 0;
		pn->zone2 = 0;
		m_zonePara.m_roleList.append( pn );
		m_addrolelist.append(pn);
		freshrolePage();		
		m_roleChanged = TRUE;  
		m_userPageFresh = TRUE;
		m_rolecfgFresh = TRUE;

		
		actionSave->setEnabled( TRUE );
		onSave();
		
	}
	else 
		return;
}

void QuserWnd::onDeleteRole() //删除角色 libinghong 2012-04-09
{	
	PNODEPARA pNode;
	QTableWidgetItem  *item ,*pitem;
	QString rolename,type;
	uint ntype;
	QTableWidget * table = m_roleTable;
	uint rownum= table->currentRow();
	item = table->item(rownum , 1);
	pitem = table->item(rownum,0);
	rolename = item->text();
	type = pitem->text();
	ntype = type.toInt();
	if (ntype <1)
	{
		DMessageBox::information( this, _C2Q( "警告" ), _C2Q( "系统管理员不能删除" ) );
		return;
	}
	if( DMessageBox::question( this, _C2Q( "警告" ), _C2Q( "确定要删除角色:") + rolename + _C2Q( "吗?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No ) != 0 )
		return;
	
	table->removeRow(rownum);
	
	for(int i=0; i<m_zonePara.m_roleList.count(); i++)
	{
		pNode = m_zonePara.m_roleList.at(i);
/*		if( rolename == pNode->sname )*/
		if( ntype == pNode->devtype)
		{
			m_zonePara.m_roleList.removeAt(i);
			m_delrolelist.append(pNode);
		}
	}
	m_roleChanged = TRUE;   
	m_userPageFresh = TRUE;
	m_rolecfgFresh = TRUE;

	actionSave->setEnabled( TRUE );
}

void QuserWnd::onApply()  //应用修改  libinghong 2012-04-17
{
	PNODEPARA pNode;
	PNODEPARA pn;
	QString type,name;
	QTableWidgetItem * nitem  ;
	QTableWidgetItem * item  ;
	QTableWidget * table = m_roleTable;
	
	for(int ndel = 0; ndel<m_delrolelist.count(); ndel++)			//若最终操作后表中含有此记录，从删除List中把该项删除
	{
		pn = m_delrolelist.at(ndel);
		if (m_zonePara.m_userList.contains(pn))
		{
			m_delrolelist.removeOne(pn);
		}
	}

// 	for(int nadd = 0; nadd<m_addrolelist.count(); nadd++)			//若最终操作后表中不含有此记录，从增加List中把该项删除
// 	{
// 		pn = m_addrolelist.at(nadd);
// 		if (!m_zonePara.m_userList.contains(pn))
// 		{
// 			m_addrolelist.removeOne(pn);
// 		}
// 	}
	
	uint rownum= table->currentRow();
	nitem = table->item(rownum , 0);
	item = table->item(rownum , 1);
	type = nitem->text();
	name = item->text();
	for (int n =0; n < m_zonePara.m_roleList.count(); n++)
	{
		pNode = m_zonePara.m_roleList.at(n);
		
		uint roletype = pNode->devtype;
		QString stype = QString::number(roletype,10);
		
		if( stype == type )
		{
			pNode->sname = name;
			//			qDebug() << stype << pNode->sname;
			onmodAuth(pNode);
		}
	}
	
	m_roleChanged = TRUE;
	actionSave->setEnabled(TRUE);
	m_rolecfgFresh = TRUE;

	m_userItem2Node.clear();
	m_authTW->clear();

//	m_roleChanged = TRUE;   
}

void QuserWnd::onSelAllRole()  //全选所用权限 libinghong 2012-04-11
{	
	for(QList<QTreeWidgetItem *>::iterator it = m_treeItemList.begin(); it != m_treeItemList.end(); it++)
	{
		(*it)->setCheckState(0, Qt::Checked);
	}
}

void QuserWnd::onNotSelAllRole()  //全不选所用权限 libinghong 2012-04-11
{
	for(QList<QTreeWidgetItem *>::iterator it = m_treeItemList.begin(); it != m_treeItemList.end(); it++)
	{
		(*it)->setCheckState(0, Qt::Unchecked);
	}
}

void QuserWnd::onSelAllAuth()
{
	QTreeWidgetItem *pCurrentItem = NULL;
	QTreeWidgetItem *pSubItem = NULL;
	pCurrentItem = m_authTW->currentItem();
	if(!pCurrentItem) {
		DMessageBox::warning( this, _C2Q( "警告" ), _C2Q( "未选择节点" ) );
		return;
	}
	for (int ncnt =0;ncnt<42;ncnt++)	//设置全部勾选
	{
		pSubItem = pCurrentItem->child(ncnt);
		if( !pSubItem->isHidden() )
			pSubItem->setCheckState(0,Qt::Checked);
	}
}

void QuserWnd::onSelEptAuth()
{
	QTreeWidgetItem *pCurrentItem = NULL;
	QTreeWidgetItem *pSubItem = NULL;
	pCurrentItem = m_authTW->currentItem();
	if(!pCurrentItem) {
		DMessageBox::warning( this, _C2Q( "警告" ), _C2Q( "未选择节点" ) );
		return;
	}
	
	for (int ncnt =0;ncnt<42;ncnt++)	//设置全部勾选
	{
		pSubItem = pCurrentItem->child(ncnt);
		if( !pSubItem->isHidden() )
			pSubItem->setCheckState(0,Qt::Unchecked);
	}
}

void QuserWnd::m_roleCfgLVclicked( QListViewItem * pItem )
{

}


void QuserWnd::freshrolePage()   // 显示角色信息 libinghong 2012-04-10
{
	uint i;
	PNODEPARA pnoed;
	QTableWidgetItem *pItem0,*pItem1;
	if ( m_roletype == ROLETYPE_MANGER)
	{
		m_roleTable->setRowCount(m_zonePara.m_roleList.count());
		
		for( i = 0; i < m_zonePara.m_roleList.count(); i ++ )
		{
			pnoed = m_zonePara.m_roleList.at(i);
			
			
			
			QString strSa = pnoed->sname;
			uint type=pnoed->devtype;
			qDebug() << pnoed->sname << strSa <<type;
			QString stype =QString::number(type,10) ;
			
			pItem0 = new QTableWidgetItem(stype);
			pItem1 = new QTableWidgetItem(strSa);
			m_roleTable->setItem(i, 0, pItem0);
			m_roleTable->setItem(i, 1, pItem1);
			
			m_addRoleBtn->setEnabled(TRUE);
			m_delRoleBtn->setEnabled(TRUE);
			m_appBtn->setEnabled(TRUE);
		}
	}
	if ( m_roletype != ROLETYPE_MANGER)
	{
//		ui.m_roleTable->clear();
		m_addRoleBtn->setEnabled(FALSE);
		m_delRoleBtn->setEnabled(FALSE);
		m_appBtn->setEnabled(FALSE);
	}
	
}


void QuserWnd::freshroleCfgPage()   // 显示角色权限信息 libinghong 2012-04-10 
{
	QTreeWidgetItem* pItem;
	QString rolename;
	QTableWidget * curtable = m_roleTable;    
	QList <QTableWidgetSelectionRange*> m_list;
	char * colname[] = {"修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};
	if (m_roletype == ROLETYPE_MANGER)
	{
		for (int i = 0; i < 42; ++i)
		{
			QStringList col;
			col<<QString(_C2Q(colname[i]));
			pItem = new QTreeWidgetItem(m_roleTW, col);
			pItem->setCheckState(0,Qt::Unchecked);
			m_roleTW->insertTopLevelItem(0, pItem);
			m_treeItemList.append(pItem);

			m_rolelAll->setEnabled(TRUE);
			m_rolelNotAll->setEnabled(TRUE);
		}
	}
	if ( m_roletype != ROLETYPE_MANGER)
	{
		m_roleTW->clear();
		m_rolelAll->setEnabled(FALSE);
		m_rolelNotAll->setEnabled(FALSE);
	}
}

void QuserWnd::onreadAuth(  )   // 读取角色权限  libinghong 2012-04-12 
{
	QTreeWidgetItem* npItem;
	PNODEPARA pNode;
	QTableWidgetItem * nitem ;
	QTableWidgetItem * item;
	QTableWidgetItem * pp1, *pp2;
	QString ptyte,pname,oldname;
	uint m_ptype;
	QString type,name;
	QTableWidget * table = m_roleTable;
	
	uint rownum= table->currentRow();
	
	if(m_beforeSelRow != -1)
	{	
		for(int n=0; n<m_zonePara.m_roleList.count(); n++)
		{
			pNode = m_zonePara.m_roleList.at(n);
			
			pp1 = table->item(m_beforeSelRow,0);
			pp2 = table->item(m_beforeSelRow,1);
			ptyte = pp1->text();
			m_ptype = ptyte.toInt();
			pname = pp2->text();
			if (pNode->devtype == m_ptype )
			{
				onmodAuth(pNode);
				if (pNode->oldzone1 !=pNode->zone1 || (pNode->sname != pname) || pNode->oldzone2 != pNode->zone2)
				{
					if (DMessageBox::information(this, _C2Q("警告"), _C2Q( "当前角色权限已修改是否保存?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel )==QMessageBox::Ok)
					{
						pNode->sname = pname;
						onSave();
						break;
					}
					else
					{
						pNode->zone1 = pNode->oldzone1;
						pNode->zone2 = pNode->oldzone2;
						break;
					}
				}
				
			}
		}
	}
	
	freshrolePage();
	
	nitem = table->item(rownum , 0);
	item = table->item(rownum , 1);
	type = nitem->text();
	name = item->text();
	//	qDebug() << type;
	
	/*QTreewidght选择前先全部置数Unchecked;*/
	for(QList<QTreeWidgetItem *>::iterator it = m_treeItemList.begin(); it != m_treeItemList.end(); it++)
	{
		(*it)->setCheckState(0, Qt::Unchecked);
	}
	
	for (int num = 0; num<m_zonePara.m_roleList.count();num++ )
	{
		pNode = m_zonePara.m_roleList.at(num);
		
		uint roletype = pNode->devtype;
		QString stype = QString::number(roletype,10);
		if(stype == type || pNode->sname == name )
		{
			for (int num =0;num<32;num++)	
			{
				npItem =m_treeItemList.at(num);
				if( pNode->zone1 & ( 0x01 << num ) )
					npItem->setCheckState(0,Qt::Checked);
			}
			for (int num =32;num<42;num++)
			{
				int nmv  = num-32;
				npItem =m_treeItemList.at(num);		
				if( pNode->zone2 & ( 0x01 << nmv ) )
					npItem->setCheckState(0,Qt::Checked);
			}
		}
	}
	m_beforeSelRow = rownum;
}

void QuserWnd::onmodAuth(PNODEPARA pNode )   //设置角色勾选权限 libinghong 2012-04-12 

{
	QTreeWidgetItem* pItem;
	pNode->zone1 = 0;
	pNode->zone2 = 0;
	for (int i =0 ;i<42 ;i++)
	{
		pItem = m_roleTW->topLevelItem(i);
		if(i<32)
		{
			if(pItem->checkState(0)==Qt::Checked)
				pNode->zone1 |= 0x01 << i;
		}
		if(i>=32)
		{
			int nmv  = i-32;
			if(pItem->checkState(0)==Qt::Checked)
				pNode->zone2 |= 0x01 << nmv;
		}
	}
	m_roleauthChanged = TRUE;	//  [4/26/2012 zhoujian]
}


void QuserWnd::freshUserPage()
{
	int i;
	QList<QTableWidgetItem *> useritems;
	QTableWidgetItem *pItem[6];
	PUSERPARA pUser;
	PNODEPARA pNode;
	QStringList userlist;
	int usernum;
	roletypemap.clear();
	for (int n =0;n<m_zonePara.m_roleList.count(); n++)
	{
		pNode = m_zonePara.m_roleList.at(n);
		int roletype = pNode->devtype;
		QString sname = pNode->sname;
		roletypemap[roletype] = sname;
	}
	

	usernum = m_zonePara.m_usercfgList.count();
	m_userTbWidget->setRowCount(usernum);
	for( i = 0; i <usernum; i ++ )
	{
		pUser =	m_zonePara.m_usercfgList.at(i);

		QString item1 = pUser->code;
		QString item2 = pUser->desc;
		QString item3 = "***"/*pUser->password*/;
		QString item4 = pUser->roledesc;
		
		QString str=roletypemap.value(pUser->roletype);	//角色类别显示;
		QString item5 = str;
		QString item6;
		if (pUser->groupleader)	 item6 = _C2Q ("是");
		else  item6 = _C2Q ("否");
		
		pItem[0] = new QTableWidgetItem(item1);
		pItem[1] = new QTableWidgetItem(item2);
		pItem[2] = new QTableWidgetItem(item3);
		pItem[3] = new QTableWidgetItem(item4);
		pItem[4] = new QTableWidgetItem(item5);
		pItem[5] = new QTableWidgetItem(item6);
		for(int j=0;j<6;j++)
		{
			m_userTbWidget->setItem(i, j, pItem[j]);
		}
	}	
	if ( m_roletype!=ROLETYPE_MANGER && !m_grpleader )
	{
		m_addUserBtn->setEnabled(FALSE);
		m_delUserBtn->setEnabled(FALSE);
		m_modUserBtn->setEnabled(FALSE);
	}
	else
	{
		m_addUserBtn->setEnabled(TRUE);
		m_delUserBtn->setEnabled(TRUE);
		m_modUserBtn->setEnabled(TRUE);
	}
}

void QuserWnd::freshMangerCfgPage()
{
	int i;
	QList<QTreeWidgetItem *> useritems;
	QTreeWidgetItem *pItem;
	PUSERPARA pUser;
	QStringList userlist;
	int usernum;

	m_userTW->clear();	
	m_authTW->clear();
	
	usernum = m_zonePara.m_usercfgList.count();			//写入用户

	for( i = 0; i <usernum; i ++ )
	{
		userlist.clear();
		pUser =	m_zonePara.m_usercfgList.at(i);
		if (m_roletype==ROLETYPE_MANGER)
		{
			userlist.append(pUser->code);
			userlist.append(pUser->desc);
			pItem = new QTreeWidgetItem ( m_userTW, userlist);
			useritems.append(pItem);
		}
		else
		{
			if (pUser->roletype==m_roletype)
			{
				userlist.append(pUser->code);
				userlist.append(pUser->desc);
				pItem = new QTreeWidgetItem ( m_userTW, userlist);
				useritems.append(pItem);
			}
		}

		QString rolename = roletypemap.value(pUser->roletype);
		if (rolename.isEmpty())
		{
			pItem->setHidden(TRUE);
		}

	}	
	m_userTW->insertTopLevelItems(0, useritems);

	if(m_roletype!=0 && !m_grpleader)			//一般用户无法使用按钮
	{
		m_aplauthBtn->setEnabled(FALSE);
		m_addauthBtn->setEnabled(FALSE);
		m_delauthBtn->setEnabled(FALSE);
	}
	else
	{
		m_addauthBtn->setEnabled(TRUE);
		m_delauthBtn->setEnabled(TRUE);

	}
}

void QuserWnd::ViewAuth()	// modify [4/12/2012 zhoujian]
{
	CheckItem();
	if( m_bchangedAuth )
	{
		if (DMessageBox::information(this, _C2Q("警告"), _C2Q( "当前用户权限已修改是否保存?"),
			QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel )==QMessageBox::Cancel)
		{
			QList<QTreeWidgetItem*> pItemList;
			QTreeWidgetItem* pItem;
			
			pItemList = m_userItem2Node.keys();
			while(!pItemList.isEmpty())
			{
				pItem = pItemList.takeFirst();
				PNODEPARA pNode = m_userItem2Node[pItem];
				pNode->zone1 = pNode->oldzone1;
				pNode->zone2 = pNode->oldzone2;
			}
		}
		else
			onSave();
		m_bchangedAuth = FALSE;
	}

	m_authTW->clear();

	QString code;
	QTreeWidgetItem * pUserItem = m_userTW->currentItem ();	//取得当前用户
	code = pUserItem->text(0);

	qDebug() << code ;

	m_userItem2Node.clear();

	int i,j;
	QTreeWidgetItem  *pItem, *pSubItem;
	PNODEPARA pNode;
	QStringList strAuth;
	QList<QTreeWidgetItem *> authitems;

	char* usrcolname[] = {"修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};	

	int type = -1;		//获取当前用户的类型
	PUSERPARA pUser;

//	type = roletypemap.key(code);

	for (i = m_zonePara.m_usercfgList.count() - 1; i >= 0 ; i --)
	{
		pUser = m_zonePara.m_usercfgList.at( i );
		if(pUser->code==code)
			type = pUser->roletype;
	}

	QMap< QString, QTreeWidgetItem * > user2node;
	
	QString sname,nodename;
	
	if( m_zonePara.m_hostmap.count() > 0 )			//读取用户有权限的节点，及添加
	{

		for( i = m_zonePara.m_userList.count() - 1; i >= 0 ; i -- )
		{
			pNode = m_zonePara.m_userList.at( i );
			if(pNode->code==code)
			{
				sname = pNode->sname;
				if( !m_zonePara.m_hostmap.contains( sname ) )
					continue;

				nodename = m_zonePara.m_hostmap[sname];	

				qDebug() << code << nodename;
				if(nodename=="")
					continue;

				
				if( user2node.contains( sname ) )
				{
					pItem = user2node[ pNode->sname ];
				}
				else	
				{				
					pItem = new QTreeWidgetItem (m_authTW,QStringList(nodename) );
					
					user2node[ pNode->sname ] = pItem;
				}

				for (j=0;j<42;j++)
				{
					QStringList col;
					col<<QString(_C2Q(usrcolname[j]));
					pSubItem = new QTreeWidgetItem(pItem,col);
					pSubItem->setCheckState(0,Qt::Unchecked);
				
				}
				authitems.append(pItem);
				m_userItem2Node[ pItem ] = pNode;
			}	
		}
	}


	for (i=0;i<authitems.count();i++)
	{
		BOOL isAuth = FALSE;
		pItem = authitems.at(i);
		for(j = 0; j < m_zonePara.m_roleList.count() ; j ++ )	//读取选中用户的角色权限,非此角色权限则隐藏
		{
			pNode = m_zonePara.m_roleList.at(j);
			if(pNode->devtype == type)
			{
				for (int ncnt =0;ncnt<32;ncnt++)
				{
					pSubItem = pItem->child(ncnt);
					if( pNode->zone1 & ( 0x01 << ncnt )) isAuth = TRUE;
					if(!isAuth)
					{
//						pSubItem->setFlags(Qt::NoItemFlags);
						pSubItem->setHidden(TRUE);
					}
					isAuth = FALSE;
				}
				isAuth = FALSE;
				for (int ncnt =32;ncnt<42;ncnt++)
				{
					int nmv = ncnt-32;
					pSubItem = pItem->child(ncnt);
					if( pNode->zone2 & ( 0x01 << nmv ) ) isAuth = TRUE;
					if(!isAuth)
					{
//						pSubItem->setFlags(Qt::NoItemFlags);
						pSubItem->setHidden(TRUE);
					}
					isAuth = FALSE;
				}
				
			}
		}
		SetAuthCheck(authitems.at(i));
	}
	m_authTW->setRootIsDecorated(false);  //删除加号展开方式
	m_authTW->insertTopLevelItems(0,authitems);
	
}

void QuserWnd::SetAuthCheck(QTreeWidgetItem* pItem)		// modify [4/12/2012 zhoujian]
{
	PNODEPARA pNode;
	QTreeWidgetItem *pSubItem;
	
	int nchildcnt = pItem->childCount();
	
	pNode = m_userItem2Node[pItem];
	
	for (int ncnt =0;ncnt<32;ncnt++)	//设置权限勾选
	{
		pSubItem = pItem->child(ncnt);
		if( pNode->zone1 & ( 0x01 << ncnt ) )
			pSubItem->setCheckState(0,Qt::Checked);
		if(m_roletype!=0 && !m_grpleader)
			pSubItem->setFlags(Qt::NoItemFlags);
	}
	
	for (int ncnt =32;ncnt<42;ncnt++)
	{
		pSubItem = pItem->child(ncnt);
		int nmv  = ncnt-32;
		uint temp = pNode->zone2 & ( 0x01 << nmv );
		if( pNode->zone2 & ( 0x01 << nmv ) )
			pSubItem->setCheckState(0,Qt::Checked);
		if(m_roletype!=0 && !m_grpleader)
			pSubItem->setFlags(Qt::NoItemFlags);
	}
}

void QuserWnd::GetAuthCheck(QTreeWidgetItem* pItem)		// modify [4/12/2012 zhoujian]
{
	PNODEPARA pNode;
	QTreeWidgetItem *pSubItem;

	if (!pItem)
	{
		return;
	}
	
	int nchildcnt = pItem->childCount();

	pNode = m_userItem2Node[pItem];
//	qDebug() << pNode->sname;
	pNode->zone1 = 0;
	pNode->zone2 = 0;
	if (nchildcnt!=0)					// modify [4/26/2012 zhoujian]
	{
		for (int ncnt =0;ncnt<32;ncnt++)	//获取权限勾选
		{
			pSubItem = pItem->child(ncnt);
			if(pSubItem->checkState(0)==Qt::Checked)
				pNode->zone1 |= 0x01 << ncnt;
		}
		for (int ncnt =32;ncnt<nchildcnt;ncnt++)
		{
			int nmv  = ncnt-32;
			pSubItem = pItem->child(ncnt);
			if(pSubItem->checkState(0)==Qt::Checked)
				pNode->zone2 |= 0x01 << nmv;
		}
	}
	if(pNode->oldzone1 != pNode->zone1 || pNode->oldzone2!= pNode->zone2)
	{
		m_bchangedAuth = TRUE;
		m_userAuthChanged = TRUE;
	}
}


void QuserWnd::onModifyNode()
{
	PNODEPARA pNode,pTemp;

	CheckItem();

//	m_zonePara.DelNodePara(&m_delyuserlist);

	for(int ndel = 0; ndel<m_delyuserlist.count(); ndel++)			//若最终操作后表中含有此记录，从删除List中把该项删除
	{
		pNode = m_delyuserlist.at(ndel);
		if (m_zonePara.m_userList.contains(pNode))
		{
			m_delyuserlist.removeOne(pNode);
		}
	}
	for(int nadd = 0; nadd<m_addyuserlist.count(); nadd++)			//若最终操作后表中不含有此记录，从增加List中把该项删除
	{
		pNode = m_addyuserlist.at(nadd);
		if (!m_zonePara.m_userList.contains(pNode))
		{
			m_addyuserlist.removeOne(pNode);
		}
	}

	for(int nadd = 0; nadd<m_addyuserlist.count(); nadd++)			//增加节点的权限获取
	{
		for (int nuser = 0; nuser<m_zonePara.m_userList.count(); nuser++)
		{
			pNode = m_addyuserlist.at(nadd);
			pTemp = m_zonePara.m_userList.at(nuser);
			if( (pTemp->sname == pNode->sname) && (pTemp->code == pNode->code))
			{
				pNode->zone1 = pTemp->zone1;
				pNode->oldzone1 = pNode->zone1;
				pNode->zone2 = pTemp->zone2;
				pNode->oldzone2 = pNode->zone2;
			}
		}
	}

	actionSave->setEnabled(TRUE);
}
				


void QuserWnd::onCurrentPageChanged( QWidget * )
{
	int curpageid = m_tabWidget->currentIndex();
	
	if(m_roleChanged)
	{
		m_tabWidget->setCurrentIndex(0);
		DMessageBox::warning(this, _C2Q("警告"), _C2Q( "当前页面已做修改，请先保存!" ) );
	}

	switch ( curpageid )
	{
	case 0:
		{
			if( m_rolecfgFresh )
			{
				freshrolePage();
				m_rolecfgFresh = FALSE;
			}	
		}
		break;
	case 1:
		{
			if( m_userPageFresh )
			{
				freshUserPage();
				m_userPageFresh = FALSE;
			}	
		}
		break;
	case 2:
		{
			if( m_MangerPageFresh )
			{
				freshMangerCfgPage();	
				m_MangerPageFresh = FALSE;
			}
		}
		break;
	default:
		break;
	}
}

void QuserWnd::onAddNode()
{
	CheckItem();

	QList<QString> mapkey;
	QList<QTreeWidgetItem*> itemlist;
	QStringList sznamelist;
	QString szname;
	PNODEPARA pnode;
	QString code;
	
	QTreeWidgetItem * pUserItem = m_userTW->currentItem ();	//取得当前用户

	if ( pUserItem == NULL )
	{
		DMessageBox::warning(this, _C2Q("警告"), _C2Q( "未选择用户!" ) );
		return ;
	}
	code = pUserItem->text(0);


	QAddNodeDlg adddlg( this );
	
	
	mapkey = m_zonePara.m_hostmap.values();
	sznamelist << mapkey;
	adddlg.OnSetNodename(sznamelist);

	if( adddlg.exec() == QDialog::Accepted )
	{
		adddlg.OnGetNodename(szname);
		itemlist = m_authTW->findItems(szname,Qt::MatchFixedString);
		if(itemlist.isEmpty())
		{
			mapkey = m_zonePara.m_hostmap.keys(szname);
			pnode = new NODEPARA;
			pnode->code = code;
			pnode->sname = mapkey.first();
			pnode->zone1 = pnode->oldzone1 = 0;
			pnode->zone2 = pnode->oldzone2 = 0;
			m_zonePara.m_userList.append(pnode);
			m_addyuserlist.append(pnode);
			ViewAuth();
		}
		else
			DMessageBox::warning(this, _C2Q("警告"), _C2Q( "节点已存在!" ) );
	}	

	m_userAuthChanged = TRUE;
//	m_bchangedAuth = TRUE;
	m_aplauthBtn->setEnabled( TRUE );
}

void QuserWnd::onDelNode()
{

	CheckItem();

	QTreeWidgetItem* pItem;
	QList<PNODEPARA> pNodeList;
	PNODEPARA pnode;
	pItem = m_authTW->currentItem();
	if(!pItem)	return;
	if(pItem->childCount() == 0)
	{
		DMessageBox::warning(this, _C2Q("警告"), _C2Q( "未选中节点" ) );
		return;
	}


 	pNodeList = m_userItem2Node.values(pItem);
	pnode = pNodeList.first();

	m_zonePara.m_userList.removeOne(pnode);	
	
	ViewAuth();		
	
	m_delyuserlist.append(pnode);

// 	QString sname = pnode->sname;
// 	QString code = pnode->code;
// 	qDebug() << sname <<code;

	m_userAuthChanged = TRUE;
//	m_bchangedAuth = TRUE;
	m_aplauthBtn->setEnabled( TRUE );
}

void QuserWnd::CheckItem()	//
{
	QList<QTreeWidgetItem*> pItemList;
	QTreeWidgetItem* pItem;

	if(m_userItem2Node.isEmpty())	return; //当前权限框没有任何条目

	pItemList = m_userItem2Node.keys();
	while(!pItemList.isEmpty())
	{
		pItem = pItemList.takeFirst();
		GetAuthCheck(pItem);
	}
}

void QuserWnd::onAddUser()
{
	QUsercfgDlg usercfgdlg( this );
	PUSERPARA pNewuser;
	PNODEPARA pNode;
	QString sname;
	QComboBox* pTypeBox = usercfgdlg.m_roltype;

	for (int n =0;n<m_zonePara.m_roleList.count(); n++)
	{
		pNode = m_zonePara.m_roleList.at(n);
		int roletype = pNode->devtype;
		sname = pNode->sname;
		pTypeBox->addItem(sname);
		roletypemap[roletype] = sname;
	}

	if( usercfgdlg.exec() == QDialog::Accepted )
	{
		m_tabWidget->setTabEnabled( 0,TRUE );
		m_tabWidget->setTabEnabled( 1,TRUE );
		m_tabWidget->setTabEnabled( 2,TRUE );	
		sname = usercfgdlg.m_roltype->currentText();
		pNewuser = usercfgdlg.pUser;
		pNewuser->roletype = roletypemap.key(sname);
		pNewuser->userchanged = true;
		m_zonePara.m_usercfgList.append(pNewuser);
		m_addusercfglist.append(pNewuser);

		freshUserPage();
		m_userChanged = TRUE;
		m_userPageFresh = TRUE;
		m_MangerPageFresh = TRUE;
		actionSave->setEnabled( TRUE );
	}

}

void QuserWnd::onDeleteUser()
{
	QTableWidgetItem* pItem;
	PUSERPARA pnode;
	QString user;

	if(m_userTbWidget->currentRow()==-1)
	{
		DMessageBox::warning( this, _C2Q( "警告" ), _C2Q( "请选择需要删除的用户!" ) );
		return;
	}

	uint rownum = m_userTbWidget->currentRow();
	pItem = m_userTbWidget->item(rownum,0);
	if(pItem)
		user = pItem->text();
	if( DMessageBox::question( this, _C2Q( "警告" ), _C2Q( "确定要删除用户:") + user + _C2Q( "吗?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No ) != QMessageBox::Yes )
		return;

	for(int i=0; i<m_zonePara.m_usercfgList.count(); i++)
	{
		pnode = m_zonePara.m_usercfgList.at(i);
		if( user == pnode->code )
		{
			m_zonePara.m_usercfgList.removeAt(i);
			m_userTbWidget->removeRow(rownum);
			m_delusercfglist.append(pnode);
		}
	}
	actionSave->setEnabled( TRUE );
	m_MangerPageFresh = TRUE;
}

void QuserWnd::onModifyUser()
{
	QUsercfgDlg usercfgdlg( this );
	PUSERPARA puser1;
	PNODEPARA pNode;
	QString sname;
	QString scode;

	QComboBox* pTypeBox = usercfgdlg.m_roltype;

	for (int n =0;n<m_zonePara.m_roleList.count(); n++)
	{
		pNode = m_zonePara.m_roleList.at(n);
		int roletype = pNode->devtype;
		sname = pNode->sname;
		pTypeBox->addItem(sname);
		roletypemap[roletype] = sname;
	}
	if(m_userTbWidget->currentRow()==-1)
	{
		DMessageBox::warning( this, _C2Q( "警告" ), _C2Q( "请选择需要修改的用户!" ) );
		return;
	}
	uint rownum = m_userTbWidget->currentRow();
	puser1 = m_zonePara.m_usercfgList.at(rownum);
	scode = puser1->code;
	if(puser1)
	{
		pTypeBox->setCurrentIndex(puser1->roletype);

		usercfgdlg.refreshUserCfg(puser1);
		if( usercfgdlg.exec() == QDialog::Accepted )
		{
			m_tabWidget->setTabEnabled( 0,TRUE );
			m_tabWidget->setTabEnabled( 1,TRUE );
			m_tabWidget->setTabEnabled( 2,TRUE );	
			sname = usercfgdlg.m_roltype->currentText();
			puser1 = usercfgdlg.pUser;
			puser1->roletype = roletypemap.key(sname);
			if (puser1->code!=scode)
			{
				DMessageBox::warning( this, _C2Q( "警告" ), _C2Q( "不可更改用户名!" ) );
				return;
			}
			m_zonePara.m_usercfgList.replace(rownum,puser1);
			m_chageusercfglist.append(puser1);

			freshUserPage();
			m_userChanged = TRUE;
			m_userPageFresh = TRUE;
			m_MangerPageFresh = TRUE;
			actionSave->setEnabled( TRUE );
		}
	}
}

void QuserWnd::RoleToUser()
{
	int i,j;
	PNODEPARA pRole,pNode;
	QString code;
	int type = -1;	
	udword zone1,zone2;
	PUSERPARA pUser;
	QMap<QString,int> NameToTyp;

	for (i = m_zonePara.m_usercfgList.count() - 1; i >= 0 ; i --)
	{
		pUser = m_zonePara.m_usercfgList.at( i );
		NameToTyp[pUser->code] = pUser->roletype;
//		qDebug() << pUser->code << pUser->roletype;
	}
	
	for(i = 0; i < m_zonePara.m_userList.count() ; i ++ )
	{
		pNode = m_zonePara.m_userList.at(i);
		code = pNode->code;
		type = NameToTyp.value(code);
		qDebug() << code << type;
		for(j = 0; j < m_zonePara.m_roleList.count() ; j ++ )	//读取选中用户的角色权限
		{
			pRole = m_zonePara.m_roleList.at(j);
			if(pRole->devtype == type)
			{
				zone1 = pRole->zone1;
				zone2 = pRole->zone2;
				pNode->zone1 &=zone1; 
				pNode->zone2 &=zone2;
				break;
			}
		}
		if(j==m_zonePara.m_roleList.count())
		{
			m_zonePara.m_userList.removeOne(pNode);				
			m_delyuserlist.append(pNode);
		}

	}		
}

void QuserWnd::initStyleSheet()
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QToolBar\
						   {\
						   spacing:10px;\
						   min-height:35px;\
						   background:#FAFAFA;\
						   }\
						   QToolButton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   min-height:25px;\
						   }\
						   QToolButton:checked\
						   {\
						   background: #FFFFFF;\
						   border: 1px solid #4A90E2;\
						   }\
						   QTableWidget \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 12px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QTableWidget::item\
						   {\
						   text-align:center;\
						   }\
						   QScrollBar\
						   {\
						   background:white;\
						   height:8px;\
						   }\
						   QScrollBar::handle\
						   {\
						   background:lightgray;\
						   border:2px solid transparent;\
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar:vertical\
						   {\
						   width:8px;\
						   background:white;\
						   padding-top:9px;\
						   padding-bottom:9px;\
						   }\
						   QScrollBar::handle:vertical\
						   {\
						   background:lightgray; \
						   width:8px;\
						   min-height:20;\
						   border:2px solid transparent; \
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:vertical:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QPushButton\
						   {\
						   background:#4A90E2;\
						   border-radius: 2px;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: white;\
						   min-width:75px;\
						   min-height:25px;\
						   }\
						   QPushButton:hover\
						   {\
						   background:#0C2543;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QCheckBox::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QCheckBox::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QComboBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down\
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QComboBox::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QListView,QTreeView\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QListView::item:hover,QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QListView::item:selected,QTreeView::item:selected \
						   {\
						   background-color: lightblue;\
						   }\
						   QTreeView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QTreeView::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QSpinBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QSpinBox::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QSpinBox::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QDateTimeEdit::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QDateTimeEdit::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton\
						   {\
						   border:0px solid #000000;\
						   border-radius:3px;\
						   margin:3px 3px 3px 3px;\
						   padding:3px;\
						   background-color:rgba(0,0,0,0);\
						   color:#FFFFFF;\
						   }\
						   QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolButton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed\
						   {\
						   border:1px solid #D8D8D8;\
						   }\
						   QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   margin:3px 3px 3px 3px;\
						   padding:0px 0px 0px 0px;\
						   font-family: Microsoft YaHei;\
						   font-size: 9px;\
						   background-color:rgba(0,0,0,0);\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width:70px;\
						   max-width:70px;\
						   }\
						   QCalendarWidget QToolButton::menu-indicator\
						   {\
						   image:None;\
						   }\
						   QCalendarWidget QTableView\
						   {\
						   border-width:0px;\
						   }\
						   QCalendarWidget QWidget#qt_calendar_navigationbar\
						   {\
						   border:1px solid #575757;\
						   border-width:1px 1px 0px 1px;\
						   background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #4D4D4D,stop:1 #292929);\
						   }\
						   QTabBar::tab\
						   {\
						   color:#0F1923;\
						   font-family:Microsoft YaHei;\
						   font-size: 13px;\
						   min-width:100px;\
						   min-height:30px;\
						   border:0px;\
						   }\
						   QTabBar::tab:hover\
						   {\
						   background:rgb(255,255, 255, 100);\
						   }\
						   QTabBar::tab:selected\
						   {\
						   border-color:white;\
						   background:white;\
						   color:#5D9CE5;\
						   border-bottom: 2px solid #4A90E2;\
						   }\
						   QLineEdit\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QRadioButton::indicator:unchecked\
						   {\
						   image:url(:/res/radio.png);\
						   }\
						   QRadioButton::indicator:checked\
						   {\
						   image:url(:/res/radio_checked.png);\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QGroupBox,QRadioButton,QDockWidget\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   }\
						   QMenuBar\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:14px;\
						   color:#3E464E;\
						   background: #FAFAFA;\
						   min-width:60px;\
						   min-height:32px;\
						   border:1px solid #D5D5D5;\
						   }\
						   QMenuBar::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QMenu\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:black;\
						   background-color:#FAFAFA;\
						   }\
						   QMenu::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTimeEdit::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QTimeEdit::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QHeaderView::section \
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:#0F1923;\
						   min-width:50px;\
						   height:30px;\
						   background:#F1F1F1;\
						   border: 0px solid #567dbc;\
						   border-right:1px solid #B5B5B5;\
						   border-bottom:1px solid #B5B5B5;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}