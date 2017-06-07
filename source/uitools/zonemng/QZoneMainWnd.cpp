// QZoneMainWnd.cpp: implementation of the QZoneMainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "QZoneMainWnd.h"
#include "globaldef.h"
#include "sysdef.h"
#include "db/dbapi.h"
#include "QLoginWnd.h"
#include "uitools/login_widget.h"
#include "uitools/dmessagebox.h"

#include <QtGui/QTreeWidget>
#include <QtGui/QToolButton>
#include <QtGui/QMessageBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QInputDialog>
#include <QtGui/QComboBox>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QZoneMainWnd::QZoneMainWnd( QWidget* parent, const char* name, Qt::WindowFlags fl ):QMainWindow( parent )
{
	setupUi(this);

	m_userChanged = FALSE;
	m_zoneChanged = FALSE;
	m_objChanged = FALSE;
	m_userPageFresh = TRUE;
	m_zoneCfgPageFresh = TRUE;
	m_zoneViewPageFresh = TRUE;

	if( !m_zonePara.readPara() )
	{
		DMessageBox::information( this , tr("警告"), tr( "读数据库失败!" ) );
		close();
	}

	m_zoneLV->sortByColumn( -1 );
	m_userZoneLV->sortByColumn( -1 );
	m_zoneCfgLV->sortByColumn( -1 );
	m_zoneViewLV->sortByColumn( -1 );
	m_zoneLV->setSortingEnabled( false );
	m_userZoneLV->setSortingEnabled( false );
	m_zoneCfgLV->setSortingEnabled( false );
	m_zoneViewLV->setSortingEnabled( false );
	// 根据内容改变列宽 [5/7/2013 ljs]
	m_userCfgLV->header()->setResizeMode(QHeaderView::ResizeToContents);
	m_userCfgLV->header()->setStretchLastSection(false);
	m_zoneCfgLV->header()->setResizeMode(QHeaderView::ResizeToContents);
	m_zoneCfgLV->header()->setStretchLastSection(false);

	freshZoneDefinePage();
	freshZoneViewPage();
	m_tabWidget->widget( 0 )->setEnabled( false );
	m_tabWidget->widget( 1 )->setEnabled( false );
	m_tabWidget->widget( 2 )->setEnabled( false );
	m_tabWidget->setCurrentIndex( 3 );

	connect(action_login, SIGNAL(triggered()), this, SLOT(onLogin()));
	connect(action_exit, SIGNAL(triggered()), this, SLOT(onExit()));
	connect(action_save, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(action_load, SIGNAL(triggered()), this, SLOT(onLoad()));
	connect(m_addZoneBtn, SIGNAL(clicked()), this, SLOT(onAddZone()));
	connect(m_delZoneBtn, SIGNAL(clicked()), this, SLOT(onDeleteZone()));
	connect(m_modZoneBtn, SIGNAL(clicked()), this, SLOT(onModifyZone()));
	connect(m_ObjFuzzySel, SIGNAL(clicked()), this, SLOT(onFuzzySelObj()));
	connect(m_objSelAll, SIGNAL(clicked()), this, SLOT(onSelAllObj()));
	connect(m_objNotSelAll, SIGNAL(clicked()), this, SLOT(onNotSelAllObj()));
	connect(m_appSel, SIGNAL(clicked()), this, SLOT(onAppSel()));
	connect(m_appAll, SIGNAL(clicked()), this, SLOT(onAppAll()));
	connect(m_zoneCfgLV, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onZoneCfgLVclicked(QTreeWidgetItem*)));
	connect(m_tabWidget, SIGNAL(currentChanged(QWidget*)), this, SLOT(onCurrentPageChanged(QWidget*)));
	connect(m_userZoneSelAll, SIGNAL(clicked()), this, SLOT(onSelAllZone()));
	connect(m_userZoneNotSelAll, SIGNAL(clicked()), this, SLOT(onNotSelAllZone()));
	connect(m_userCfgLV, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onZoneCfgLVclicked(QTreeWidgetItem*)));
	connect(m_userAppSel, SIGNAL(clicked()), this, SLOT(onUserAppSel()));
	connect(m_userAppAll, SIGNAL(clicked()), this, SLOT(onUserAppAll()));
	connect( m_zoneCfgLV, SIGNAL( itemExpanded ( QTreeWidgetItem*) ) , this , SLOT( showRtuObj(QTreeWidgetItem*) ) );
	connect( m_zoneViewLV, SIGNAL( itemExpanded ( QTreeWidgetItem*) ) , this , SLOT( showRtuObj(QTreeWidgetItem*) ) );

	initStyleSheet() ;
}

QZoneMainWnd::~QZoneMainWnd()
{

}


void QZoneMainWnd::onLogin()
{
	action_login->setChecked(false);
// 	QLoginWnd loginwnd( this );
	Login_widget loginwnd ;
	if( loginwnd.exec() == QDialog::Accepted )
	{
		action_login->setEnabled(FALSE);
		m_tabWidget->widget( 0 )->setEnabled( TRUE );
		m_tabWidget->widget( 1 )->setEnabled( TRUE );
		m_tabWidget->widget( 2 )->setEnabled( TRUE );
	}
}

void QZoneMainWnd::onExit()
{
	close();
}

void QZoneMainWnd::onSave()
{
	action_save->setChecked(false);
	action_save->setEnabled( FALSE );
	action_load->setEnabled( TRUE );

	m_zonePara.savePara();
}

void QZoneMainWnd::onLoad()
{
	action_load->setChecked(false);
	action_load->setEnabled( FALSE );

	RtdbProc rtdbproc;

	if( m_userChanged )
	{
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_ALLHOST, "节点权限参数表" );
	}

	if( m_objChanged )
	{
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_SCADA, "模拟量视图" );
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_SCADA, "开关量视图" );
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_SCADA, "状态量视图" );
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_SCADA, "刀闸量视图" );
		rtdbproc.Recall( "scada", "scada", RDBSERVNAME_SCADA, "保护量视图" );
	}

	m_zoneChanged = FALSE;
	m_userChanged = FALSE;
	m_objChanged = FALSE;
}

void QZoneMainWnd::onAddZone()
{
	int addindex = -1;
	uint i;

	for( i = 0; i < m_zonePara.m_zoneNames.count(); i ++ )
	{
		if( m_zonePara.m_zoneNames[ i ].isEmpty() )
		{
			addindex = i;
			break;
		}
	}

	if( i >= 32 )
	{
		DMessageBox::information( this, tr( "提示" ), tr( "监控区个数已到上限，不能添加!" ) );
		return;
	}
	bool ok;
	QString zonename = QInputDialog::getText(this, tr( "请输入监控区名称" ), tr( "监控区名称" ), 
		QLineEdit::Normal, QString::null, &ok );

	if( ok && !zonename.isEmpty() )
	{
		if( addindex == - 1 )
		{
			m_zonePara.m_zoneNames.append( zonename );
		}
		else
		{
			m_zonePara.m_zoneNames[ addindex ] = zonename;
		}
		freshZoneDefinePage();

		m_zoneChanged = TRUE;
		m_userPageFresh = TRUE;
		m_zoneCfgPageFresh = TRUE;
		m_zoneViewPageFresh = TRUE;

		action_save->setEnabled( TRUE );
	}
}

void QZoneMainWnd::onDeleteZone()
{
	int i;

	for( i = 0; i < m_zoneTable->rowCount(); i ++ )
	{
		if( m_zoneTable->currentRow() == i )
		{
			break;
		}
	}

	if( i >= m_zoneTable->rowCount() )
	{
		return;
	}

	QString zonename;
	int delzone = m_zoneNoList[ i ];

	if( !delzone )
	{
		DMessageBox::information( this, tr( "警告" ), tr( "默认监控区不能删除" ) );
		return;
	}

	zonename = m_zonePara.m_zoneNames[ delzone ];

	if( DMessageBox::question( this, tr( "警告" ), tr( "确定要删除监控区:") + zonename + tr( "吗?"), QMessageBox::Yes|QMessageBox::No,QMessageBox::No ) != 0 )
		return;

	m_zonePara.m_zoneNames[ delzone ] = "";

	PNODEPARA pNode;

	for( i = 0; i < m_zonePara.m_userList.count(); i ++ )
	{
		pNode = m_zonePara.m_userList.at( i );

		if( pNode->zone == delzone )
		{
			pNode->zone = 0;
			m_userChanged = TRUE;
		}
	}

	for( i = 0; i < m_zonePara.m_devList.count(); i ++ )
	{
		pNode = m_zonePara.m_devList.at( i );
		
		if( pNode->zone == delzone )
		{
			pNode->zone = 0;
			m_objChanged = TRUE;
		}
	}

	freshZoneDefinePage();
	
	m_zoneChanged = TRUE;
	m_userPageFresh = TRUE;
	m_zoneCfgPageFresh = TRUE;
	m_zoneViewPageFresh = TRUE;

	action_save->setEnabled( TRUE );
}

void QZoneMainWnd::onModifyZone()
{
	int i;
	
	for( i = 0; i < m_zoneTable->rowCount(); i ++ )
	{
		if( m_zoneTable->currentRow() == i )
		{
			break;
		}
	}

	if( i >= m_zoneTable->rowCount() )
	{
		return;
	}
	
	bool ok;
	QString zonename = QInputDialog::getText(this, tr( "请输入监控区名称" ), tr( "监控区名称" ), 
		QLineEdit::Normal, m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ], &ok );

	if( ok && zonename.isEmpty() )
	{
		DMessageBox::information( this, tr( "提示" ), tr( "监控区名称不能为空!" ) );
		return;
	}
	
	if( ok )
	{
		m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ] = zonename;
		freshZoneDefinePage();
		
		m_zoneChanged = TRUE;
		m_userPageFresh = TRUE;
		m_zoneCfgPageFresh = TRUE;
		m_zoneViewPageFresh = TRUE;

		action_save->setEnabled( TRUE );
	}

}

void QZoneMainWnd::onSelAllZone()
{
	QTreeWidgetItem *pItem;

	for ( int i=0;i < m_userZoneLV->topLevelItemCount();i++)
	{
		pItem = m_userZoneLV->topLevelItem( i );
		pItem->setCheckState( 0,Qt::Checked );
	}
}

void QZoneMainWnd::onNotSelAllZone()
{
	QTreeWidgetItem *pItem;

	for ( int i=0;i < m_userZoneLV->topLevelItemCount();i++)
	{
		pItem = m_userZoneLV->topLevelItem( i );
		pItem->setCheckState( 0,Qt::Unchecked );
	}

}

void QZoneMainWnd::onFuzzySelObj()
{
	QTreeWidgetItem *pSubItem,*pSubSubItem, *pSubSubSubItem;
	QString tmptext, tmptext1;
	tmptext1 = m_objLE->text();

	for ( int i=0;i < m_zoneCfgLV->topLevelItemCount();i++)
	{
		pSubItem = m_zoneCfgLV->topLevelItem( i );

		if( pSubItem->text( 0 ).indexOf( m_objLE->text() ) >= 0 )
		{
			pSubItem->setCheckState( 0,Qt::Checked );

			onZoneCfgLVclicked( pSubItem );
			continue;
		}

		for( int j =0;j<pSubItem->childCount();j++)
		{
			pSubSubItem = pSubItem->child( j );
			if( pSubSubItem->text( 0 ).indexOf( m_objLE->text() ) >= 0 )
			{
				pSubSubItem->setCheckState( 0,Qt::Checked );
				onZoneCfgLVclicked( pSubSubItem );
				continue;
			}

			for( int k =0;k<pSubSubItem->childCount();k++)
			{
				pSubSubSubItem = pSubSubItem->child( k );
				if( pSubSubSubItem->text( 0 ).indexOf( m_objLE->text() ) >= 0 )
				{
					pSubSubSubItem->setCheckState( 0,Qt::Checked );
					onZoneCfgLVclicked( pSubSubSubItem );
				}
			}
		}
	}
}

void QZoneMainWnd::onSelAllObj()
{
	QTreeWidgetItem *pItem;

	for( int i = 0;i<m_zoneCfgLV->topLevelItemCount();i++)
	{
		pItem = m_zoneCfgLV->topLevelItem( i );
		pItem->setCheckState( 0,Qt::Checked );
		if( pItem->childCount() > 0 )
		{
			onZoneCfgLVclicked( pItem );
		}
		else
		{
			showRtuObj( pItem ) ;
		}
//		onZoneCfgLVclicked( pItem );
	}
}

void QZoneMainWnd::onNotSelAllObj()
{
	QTreeWidgetItem *pItem;

	for( int i = 0;i<m_zoneCfgLV->topLevelItemCount();i++)
	{
		pItem = m_zoneCfgLV->topLevelItem( i );
		pItem->setCheckState( 0,Qt::Unchecked );
		if( pItem->childCount() > 0 )
		{
			onZoneCfgLVclicked( pItem );
		}
		else
		{
			showRtuObj( pItem ) ;
		}
//		onZoneCfgLVclicked( pItem );
	}
}

void QZoneMainWnd::onAppSel()
{
	QTreeWidgetItem *pItem,*pSubItem,*pSubSubItem;
	PNODEPARA pNode;
	uint zone = 0;
	QString zonedesc = "";

	int i;

	for( int i = 0;i<m_zoneLV->topLevelItemCount();i++)
	{
		pItem = m_zoneLV->topLevelItem( i );
		if( pItem->isSelected() )
		{
			zone = m_zoneNoList[ i ];
			zonedesc = m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ];
			break;
		}
	}

	if( zonedesc.isEmpty() )
		return;

	for( int i = 0;i<m_zoneCfgLV->topLevelItemCount();i++)
	{
		pItem = m_zoneCfgLV->topLevelItem( i );
//         if (pItem->data(0,Qt::UserRole).toInt()>0)
//         {
// 			for (int j=0;j<pItem->childCount();j++)
// 			{
//                 pSubItem =pItem->child(j);
// 			    if (pSubItem->data(0,Qt::UserRole).toInt()>0)
// 			    {
// 					for (int k=0;k<pSubItem->childCount();k++)
// 					{
// 						pSubSubItem = pSubItem->child(k);
// 					    if (pSubSubItem->data(0,Qt::UserRole).toInt()>0)
// 					    {
//                              
// 					    }
// 						else
// 						{
							if( pItem->checkState( 0 )==Qt::Checked && m_item2Node.contains( pItem ) )
							{
								pNode = m_item2Node[ pItem ];
								pNode->zone = zone;
								pItem->setText( 1, zonedesc );

								if( pNode->zone != pNode->oldzone )
									m_objChanged = TRUE;
							}

							if( pItem->childCount() > 0 )
							{
								setObjZone( pItem, zone, zonedesc );
							}
// 						}
// 					}
// 			    }
		//	}
			
		//} 
		
	}

	m_zoneViewPageFresh = TRUE;
	action_save->setEnabled( TRUE );
}

void QZoneMainWnd::onAppAll()
{
	QTreeWidgetItem *pItem;
	PNODEPARA pNode;
	uint zone = 0;
	QString zonedesc = "";
	
	int i;
	
	for( int i = 0;i<m_zoneLV->topLevelItemCount();i++)
	{
		pItem = m_zoneLV->topLevelItem( i );

		if( pItem->isSelected() )
		{
			zone = m_zoneNoList[ i ];
			zonedesc = m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ];
			break;
		}
	}

	if( zonedesc.isEmpty() )
		return;

	for( int i = 0;i<m_zoneCfgLV->topLevelItemCount();i++)
	{
		pItem = m_zoneCfgLV->topLevelItem( i );

		if( m_item2Node.contains( pItem ) )
		{
			pNode = m_item2Node[ pItem ];
			pNode->zone = zone;
			pItem->setText( 1, zonedesc );
			if( pNode->zone != pNode->oldzone )
				m_objChanged = TRUE;
		}
		
		if( pItem->childCount() > 0 )
		{
			setObjZone( pItem, zone, zonedesc, FALSE );
		}
	}

	m_zoneViewPageFresh = TRUE;

	action_save->setEnabled( TRUE );
}

void QZoneMainWnd::onZoneCfgLVclicked( QTreeWidgetItem * pItem )
{
	if( pItem == NULL )
		return;
	
	if ( pItem->childCount() <= 0 )
	{
		showRtuObj(pItem) ;
	}
	bool checked;

	Qt::CheckState state = pItem->checkState( 0 );
 	if( state == Qt::PartiallyChecked )
 		return;
	
	QTreeWidgetItem *pSubItem,*pSubSubItem, *pSubSubSubItem,*pSubSubSubSubItem;

	for( int i=0;i <pItem->childCount();i++ )
	{
		pSubItem = pItem->child( i );
		pSubItem->setCheckState( 0,state );
		if ( pSubItem->data(0,Qt::UserRole).toInt()== 1 )
		{
			showRtuObj(pSubItem) ;
		}
		for( int j=0;j <pSubItem->childCount();j++ )
		{
			pSubSubItem = pSubItem->child( j );
			pSubSubItem->setCheckState( 0,state );
			if ( pSubSubItem->data(0,Qt::UserRole).toInt()== 1 )
			{
				showRtuObj(pSubSubItem) ;
			}
			for( int k=0;k <pSubSubItem->childCount();k++ )
			{
				pSubSubSubItem = pSubSubItem->child( k );
				pSubSubSubItem->setCheckState( 0,state );
				if ( pSubSubSubItem->data(0,Qt::UserRole).toInt()== 1 )
				{
					showRtuObj(pSubSubSubItem) ;
				}
				for( int h=0;h <pSubSubSubItem->childCount();h++ )
				{
					pSubSubSubSubItem = pSubSubSubItem->child( h );
					pSubSubSubSubItem->setCheckState( 0,state );
					
				}
			}
			
		}
	}
	
	if( state == Qt::Unchecked )
		return;

	QTreeWidgetItem *pParentItem = pItem->parent();
	while( pParentItem != NULL )
	{
		pParentItem->setCheckState(0, state);
		pParentItem = pParentItem->parent();
	}
}

void QZoneMainWnd::freshZoneDefinePage()
{
	uint i;
	uint zonenum = 0;

	for( i = 0; i < m_zonePara.m_zoneNames.count(); i ++ )
	{
		if( !m_zonePara.m_zoneNames[ i ].isEmpty() )
		{
			zonenum ++;
		}
	}

	m_zoneTable->setRowCount( zonenum );

	QString tmptext;
	m_zoneNoList.clear();
	int row = 0;
	for( i = 0; i < m_zonePara.m_zoneNames.count(); i ++ )
	{
		if( m_zonePara.m_zoneNames[ i ].isEmpty() )
		{
			continue;
		}
		tmptext.sprintf( "%d", i );

		m_zoneTable->setItem( row, 0, new QTableWidgetItem( tmptext ) );
		m_zoneTable->setItem( row, 1, new QTableWidgetItem( m_zonePara.m_zoneNames[ i ] ) );
		m_zoneNoList.append( i );
		row ++;
	}
}

void QZoneMainWnd::freshZoneCfgPage()
{
	int i, j;
	QTreeWidgetItem *pItem, *pSubItem, *pSubSubItem;
	PNODEPARA pNode;

	QString zonename;
	QStringList zonenameList;
	int zoneno;

	m_zoneLV->clear();
	m_zoneCfgLV->clear();
	m_item2Node.clear();

	for( i = 0;i< m_zoneNoList.count(); i ++ )
	{
		zonenameList.clear();
		zoneno = m_zoneNoList[ i ];
		zonename = m_zonePara.m_zoneNames[ zoneno ];
		zonenameList << zonename;
		new QTreeWidgetItem( m_zoneLV, zonenameList ,0);
	}

	m_zoneLV->resizeColumnToContents( 0 );

	QMap< QString, QTreeWidgetItem* > sname2node;
	QMap< QString, QTreeWidgetItem* > snametype2node;

	QString desc;
	QStringList describe;
	PDEVTYPEPARA pDevType;
	QMap<int ,QTreeWidgetItem*> m_groupItemMap ;

 	QStringList desc_group;
 	desc_group.clear() ;
// 	desc_group << _C2Q("未分组厂站")  ;
// 	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( m_zoneCfgLV, desc_group ) ;
// 	firstStaItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
// 	firstStaItem->setData(0,Qt::UserRole,1);
// 	m_groupItemMap[0 ] = firstStaItem ;


	//先显示第一层
	QMapIterator<int , PSTATIONAREAPARA> area(m_zonePara.m_station_area);
	while (area.hasNext()) 
	{
		area.next();
		if (area.value()->masterid == NULL)
		{
			desc_group.clear() ;
			desc_group.append( _C2Q(area.value()->name) ) ;
			pItem = new QTreeWidgetItem( m_zoneCfgLV, desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			pItem->setData(0,Qt::UserRole,2);
			m_groupItemMap[area.value()->id ] = pItem ;
		}
	}
	//显示第二层,区域表只可以表示到2层
	QMapIterator<int , PSTATIONAREAPARA> area2(m_zonePara.m_station_area);
	while (area2.hasNext()) 
	{
		area2.next();
		if (area2.value()->masterid != NULL)
		{
			desc_group.clear() ;
			desc_group.append( _C2Q(area2.value()->name) ) ;
			pItem = new QTreeWidgetItem( m_groupItemMap[area2.value()->masterid ] , desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			pItem->setData(0,Qt::UserRole,2);
			m_groupItemMap[area2.value()->id ] = pItem ;
		}
	}
	//显示厂站
	/*QStringList desc_station;
	QMapIterator<int , PSTATIONINFO> station(m_station);
	while (station.hasNext()) 
	{
		station.next();
		desc_station.clear() ;
		desc_station.append( _C2Q(station.value()->desc) ) ;
		pSubItem = new QTreeWidgetItem( m_groupItemMap[station.value()->zoneNo] , desc_station ) ;
		// 		pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		// 		m_groupItemMap[station.value()->id ] = pSubItem ;
		pSubItem->setData(0,Qt::UserRole,QVariant(_C2Q(station.value()->code)));
		pSubItem->setIcon(0,iconStation);
	}*/

	if( m_zonePara.m_devList.count() > 0 )
	{
		for( i = m_zonePara.m_devList.count() - 1; i >= 0  ; i -- )
		{
			pNode = m_zonePara.m_devList.at( i );
			
			//厂站节点
			if( sname2node.contains( pNode->sname ) )
			{
				pItem = sname2node[ pNode->sname ];
			}
			else
			{
				describe.clear();
				desc = "";
				m_zonePara.getStationDesc( pNode->sname, desc );
				if( desc.isEmpty() )
					continue;
				describe << desc;
				//pItem = new QTreeWidgetItem( m_zoneCfgLV, describe,0/*, QCheckListItem::CheckBox */);
				int zoneno = m_zonePara.m_stanametoid[pNode->sname];
				pItem = new QTreeWidgetItem( m_groupItemMap[zoneno] , describe,0/*, QCheckListItem::CheckBox */);
				pItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
				pItem->setCheckState( 0, Qt::Unchecked );
				pItem->setData(0,Qt::UserRole,1);
				sname2node[ pNode->sname ] = pItem;
			}

			m_devPageItem2RtuCodeMap[ pItem ] = pNode->sname ;
		//	pItem->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
// 			pDevType = m_zonePara.m_devTypeList.at( pNode->devtype );
// 
// 			if( pDevType == NULL )
// 				continue;
// 
// 			//设备类型节点
// 			desc.sprintf( "%d", pNode->devtype );
// 			desc = pNode->sname + QString( "_" ) + desc;
// 			if( snametype2node.contains( desc ) )
// 			{
// 				pSubItem = snametype2node[ desc ];
// 			}
// 			else
// 			{
// // 				pSubItem = new QCheckListItem( pItem, pDevType->devname, 
// // 					QCheckListItem::CheckBox );
// 				describe.clear();
// 				describe << pDevType->devname;
// 				pSubItem = new QTreeWidgetItem( pItem, describe ,0 /*, QCheckListItem::CheckBox */);
// 				pSubItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
// 				pSubItem->setCheckState( 0, Qt::Unchecked );
// 				snametype2node[ desc ] = pSubItem;
// 			}
// 
// // 			pSubSubItem = new QCheckListItem( pSubItem, pNode->desc, 
// // 				QCheckListItem::CheckBox );
// 			describe.clear();
// 			describe << pNode->desc;
// 			pSubSubItem = new QTreeWidgetItem( pSubItem, describe ,0/*, QCheckListItem::CheckBox */);
// 			pSubSubItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
// 			pSubSubItem->setCheckState( 0, Qt::Unchecked );
// 
// 			desc = "";
// 			for( j = 0; j < m_zoneNoList.count() ; j ++ )
// 			{
// 				zoneno = m_zoneNoList[ j ];
// 				zonename = m_zonePara.m_zoneNames[ zoneno ];
// 				if( pNode->zone == zoneno )
// 				{
// 					desc = zonename;
// 					break;
// 				}
// 			}
// 			pSubSubItem->setText( 1, desc );
// 
// 			m_item2Node[ pSubSubItem ] = pNode;
 		}
 	}

	for( i = 0; i < m_zoneCfgLV->columnCount(); i ++ )
	{
		m_zoneCfgLV->resizeColumnToContents( i );
	}
	/*if (firstStaItem->childCount() == 0)
	{
		int index_no = m_zoneCfgLV->indexOfTopLevelItem( firstStaItem ) ;
		m_zoneCfgLV->takeTopLevelItem(index_no);
	}*/
}

void QZoneMainWnd::freshZoneViewPage()
{
	int i;
	QTreeWidgetItem *pRootItem, *pItem, *pSubItem, *pSubSubItem;
	PNODEPARA pNode;
	
	QMap< QString, QTreeWidgetItem* > sname2node;
	QMap< QString, QTreeWidgetItem* > snametype2node;
	
	QString desc;
	QStringList describe;
	PDEVTYPEPARA pDevType;
	QString key;

	int cnt;

	m_zoneViewLV->clear();

// 	for( cnt = m_zoneNoList.count() - 1; cnt >= 0 ; cnt -- )
	for( cnt =0;cnt < m_zoneNoList.count(); cnt ++ )
	{
		sname2node.clear();
		snametype2node.clear();
		describe.clear();
		describe << m_zonePara.m_zoneNames[ m_zoneNoList[ cnt ] ];
		pRootItem = new QTreeWidgetItem( m_zoneViewLV, describe ,0 );
		if( m_zonePara.m_devList.count() <= 0 )
			continue;
			
		for( i = m_zonePara.m_devList.count() - 1; i >= 0  ; i -- )
		{
			pNode = m_zonePara.m_devList.at( i );

			if( pNode->zone != m_zoneNoList[ cnt ] )
				continue;
			
			//厂站节点
			if( sname2node.contains(  pNode->sname  ) )
			{
				pItem = sname2node[  pNode->sname ];
			}
			else
			{
				desc = "";
				m_zonePara.getStationDesc( pNode->sname, desc );
				if( desc.isEmpty() )
					continue;
				describe.clear();
				describe << desc;
				pItem = new QTreeWidgetItem( pRootItem, describe ,0);
				pItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
				sname2node[ pNode->sname ] = pItem;
			}

			m_devPageItem2RtuCodeMap[ pItem ] = pNode->sname ;
			pItem->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
// 			pDevType = m_zonePara.m_devTypeList.at( pNode->devtype );
// 			if( pDevType == NULL )
// 				continue;
// 			
// 			//设备类型节点
// 			desc.sprintf( "%d", pNode->devtype );
// 			desc = pNode->sname + QString( "_" ) + desc;
// 			if( snametype2node.contains( desc ) )
// 			{
// 				pSubItem = snametype2node[ desc ];
// 			}
// 			else
// 			{
// 				describe.clear();
// 				describe << pDevType->devname;
// 				pSubItem = new QTreeWidgetItem( pItem, describe ,0);
// 				pSubItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
// 				snametype2node[ desc ] = pSubItem;
// 			}
// 			describe.clear();
// 			describe << pNode->desc;
// 			pSubSubItem = new QTreeWidgetItem( pSubItem, describe ,0 );
// 			pSubSubItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
		}
	}

	for( i = 0; i < m_zoneViewLV->columnCount(); i ++ )
	{
		m_zoneViewLV->resizeColumnToContents( i );
	}
}

void QZoneMainWnd::freshUserPage()
{
	int i, j;
	QTreeWidgetItem *pItem, *pSubItem,*userItem;
	PNODEPARA pNode;

	QString zonename;
	QStringList zonenameList;
	int zoneno;

	m_userZoneLV->clear();
	m_userCfgLV->clear();
	m_userItem2Node.clear();

// 	for( i = m_zoneNoList.count() - 1; i >= 0; i -- )
	for( i = 0;i< m_zoneNoList.count(); i ++ )
	{
		zonenameList.clear();
		zoneno = m_zoneNoList[ i ];
		zonename = m_zonePara.m_zoneNames[ zoneno ];
		zonenameList << zonename;
		userItem = new QTreeWidgetItem( m_userZoneLV, zonenameList,0/*, QCheckListItem::CheckBox */);
		userItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
		userItem->setCheckState( 0, Qt::Unchecked );
	}

// 	m_userZoneLV->adjustColumn( 0 );
	m_userZoneLV->resizeColumnToContents( 0 );

	QMap< QString, QTreeWidgetItem* > user2node;

	QString desc;

	if( m_zonePara.m_userList.count() > 0 )
	{
		for( i = m_zonePara.m_userList.count() - 1; i >= 0 ; i -- )
		{
			pNode = m_zonePara.m_userList.at( i );

			if( !m_zonePara.m_hostmap.contains( pNode->sname ) )
				continue;

			if( user2node.contains( pNode->desc ) )
			{
				pItem = user2node[ pNode->desc ];
			}
			else
			{
				zonenameList.clear();
				zonenameList << pNode->desc;
				pItem = new QTreeWidgetItem( m_userCfgLV, zonenameList,0/*, QCheckListItem::CheckBox */);
				pItem->setFlags(  Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
				pItem->setCheckState( 0, Qt::Unchecked );
				user2node[ pNode->desc ] = pItem;
			}

			zonenameList.clear();
			zonenameList << m_zonePara.m_hostmap[ pNode->sname ];
			pSubItem = new QTreeWidgetItem( pItem, zonenameList/*, QCheckListItem::CheckBox */ );
			pSubItem->setFlags( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
			pSubItem->setCheckState( 0, Qt::Unchecked );
			m_userItem2Node[ pSubItem ] = pNode;

			desc = "";
			for( j = 0; j < m_zoneNoList.count() ; j ++ )
			{
				zoneno = m_zoneNoList[ j ];
				zonename = m_zonePara.m_zoneNames[ zoneno ];
				if( pNode->zone & ( 0x01 << zoneno ) )
				{
					if( desc.isEmpty() )
					{
						desc = zonename;
					}
					else
					{
						desc += ",";
						desc += zonename;
					}
				}
			}
			pSubItem->setText( 1, desc );
		}
	}

	for( i = 0; i < m_userCfgLV->columnCount(); i ++ )
	{
		m_userCfgLV->resizeColumnToContents( i );
	}
}


void QZoneMainWnd::onCurrentPageChanged( QWidget * )
{
	int curpageid = m_tabWidget->currentIndex();

	switch ( curpageid )
	{
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
			if( m_zoneCfgPageFresh )
			{
				freshZoneCfgPage();
				m_zoneCfgPageFresh = FALSE;
			}	
		}
		break;
	case 3:
		{
			if( m_zoneViewPageFresh )
			{
				freshZoneViewPage();	
				m_zoneViewPageFresh = FALSE;
			}
		}
		break;
	default:
		break;
	}
}

void QZoneMainWnd::setObjZone( QTreeWidgetItem *pItem , uint zone, QString zonedesc, bool selon )
{
	QTreeWidgetItem *pSubItem;
	PNODEPARA pNode;
	
// 	for( pSubItem = pItem->firstChild(); pSubItem!= NULL;
// 		pSubItem = pSubItem->nextSibling() )

	for( int i=0;i<pItem->childCount();i++)
	{
		int a= pItem->childCount();
		pSubItem = pItem->child( i );
	
		if( ( !selon ||  pSubItem->checkState( 0 ) == Qt::Checked ) && m_item2Node.contains( pSubItem ) )
		{
			pNode = m_item2Node[ pSubItem ];
			pNode->zone = zone;
			pSubItem->setText( 1, zonedesc );

			if( pNode->zone != pNode->oldzone )
				m_objChanged = TRUE;
		}
		
		if( pSubItem->childCount() > 0 )
		{
			setObjZone( pSubItem, zone, zonedesc, selon );
		}
	}
}


void QZoneMainWnd::onUserAppSel()
{
	QTreeWidgetItem *pItem;
	PNODEPARA pNode;
	uint valid = 0xffffffff;
	uint zone = 0;
	QString zonedesc = "";
	
	int i;
	
// 	for( pItem = m_userZoneLV->firstChild(), i = 0; pItem!= NULL;
// 		pItem = pItem->nextSibling(), i ++ )
	for( i=0;i<m_userZoneLV->topLevelItemCount();i++)
	{
		pItem = m_userZoneLV->topLevelItem( i );
		valid &= (~( 0x01 << m_zoneNoList[ i ] ) );
		if( pItem->checkState( 0 ) == Qt::Checked )
		{
			zone |= 0x01 << m_zoneNoList[ i ];
			if( zonedesc.isEmpty() )
			{
				zonedesc = m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ];
			}
			else
			{
				zonedesc = zonedesc + QString( "," ) + m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ];
			}
		}
	}

	QMap<QTreeWidgetItem*, PNODEPARA>::Iterator it;

	for( it = m_userItem2Node.begin(); it != m_userItem2Node.end(); it ++ )
	{
		pItem = it.key();
		if( pItem->checkState( 0 ) == Qt::Unchecked )
			continue;
		pNode = it.value();

		pNode->zone = pNode->oldzone & valid | zone;
		pItem->setText( 1, zonedesc );
		if( pNode->zone != pNode->oldzone )
			m_userChanged = TRUE;
	}
	
	action_save->setEnabled( TRUE );
}

void QZoneMainWnd::onUserAppAll()
{
	QTreeWidgetItem *pItem;
	PNODEPARA pNode;
	uint valid = 0xffff; 
	uint zone = 0;
	QString zonedesc = "";
	
	int i;
	
// 	for( pItem = m_userZoneLV->firstChild(), i = 0; pItem!= NULL;
// 	pItem = pItem->nextSibling(), i ++ )
	for( i=0;i<m_userZoneLV->topLevelItemCount();i++)
	{
		pItem = m_userZoneLV->topLevelItem( i );
		valid &= (~( 0x01 << m_zoneNoList[ i ] ) );
		if( pItem->checkState( 0 ) == Qt::Checked )
		{
			zone |= 0x01 << m_zoneNoList[ i ];
			if( zonedesc.isEmpty() )
			{
				zonedesc = m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ];
			}
			else
			{
				zonedesc = zonedesc + QString( "," ) + m_zonePara.m_zoneNames[ m_zoneNoList[ i ] ];
			}
		}
	}
	
	QMap<QTreeWidgetItem*, PNODEPARA>::Iterator it;
	
	for( it = m_userItem2Node.begin(); it != m_userItem2Node.end(); it ++ )
	{
		pItem = it.key();
		pNode = it.value();
		
		pNode->zone = pNode->oldzone & valid | zone;
		pItem->setText( 1, zonedesc );

		if( pNode->zone != pNode->oldzone )
			m_userChanged = TRUE;
	}
	
	action_save->setEnabled( TRUE );
}

void QZoneMainWnd::showRtuObj(  QTreeWidgetItem * item )
{
	if( item->childCount() > 0 )
		return;

	QString rtuCode;
	if( !m_devPageItem2RtuCodeMap.contains( item ) )
	{
		return;
	}

	rtuCode = m_devPageItem2RtuCodeMap[ item ];

	Qt::CheckState isChecked = item->checkState( 0 ) ;
	QStringList devlist;
	QTreeWidgetItem *pSubItem;
	QTreeWidgetItem *pSubSubItem;

	DEVTYPEPARA	devtype;
	int k, p , q;
	uint zoneno ;
	QString desc;

	for( k = 0; k < m_zonePara.m_devTypeList.count(); k ++ )
	{
		QStringList m_devtypelist ;

		if ( m_zonePara.m_devTypeList.at( k ) == NULL )
		{
			continue;
		}

		devlist.clear();

		devtype.devtype = m_zonePara.m_devTypeList.at( k )->devtype ;
		devtype.devname = m_zonePara.m_devTypeList.at( k )->devname ;

		if( !m_zonePara.getDevCode( rtuCode, devtype.devtype, devlist ) )
		{
			continue;
		}

		m_devtypelist << devtype.devname ;

		pSubItem = new QTreeWidgetItem( item, m_devtypelist) ;
		if ( m_tabWidget->currentIndex() == 2 )
		{
				pSubItem->setCheckState(0, isChecked); // 设定可核取的方块
		}

		for( p = 0; p < devlist.count(); p ++ )
		{
			QStringList m_devdesclist ;
			if( !m_zonePara.getDevDesc( devlist[ p ], desc ) )
				continue;
			m_devdesclist << desc ;
			pSubSubItem = new QTreeWidgetItem( pSubItem, m_devdesclist);
			if ( m_tabWidget->currentIndex() == 2 )
			{
				pSubSubItem->setCheckState(0, isChecked); // 设定可核取的方块
				desc = "";

					if( !m_zonePara.getZoneDesc( devlist[ p ], desc ) )
						continue;
				pSubSubItem->setText( 1, desc );
			}
			m_item2Node[ pSubSubItem ] = m_zonePara.m_devCode2DevParaMap[ devlist[ p ] ];
		}
	}
}

void QZoneMainWnd::initStyleSheet()
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
						   image:url(:/zonemng/png/checkbox_unchecked.png);\
						   }\
						   QTreeView::indicator:checked\
						   {\
						   image:url(:/zonemng/png/checkbox_checked.png);\
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