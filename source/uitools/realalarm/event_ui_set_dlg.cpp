// QEventUISetDlg.cpp: implementation of the QEventUISetDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "event_ui_set_dlg.h"
#include "event_real_wnd.h"
#include "globaldef.h"
#include "event_table.h"

#include <QtGui/QScrollBar>
extern QWidget* getMainWnd();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Event_ui_set_dlg::Event_ui_set_dlg( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl ):QDialog( parent ,fl )
{
	setupUi(this);
	setModal(modal);
	connect( m_tabWidget, SIGNAL(currentChanged ( int )),this,SLOT(show_page ( int )));
	connect( m_addPageBtn, SIGNAL(clicked()), this, SLOT(slot_add_page()));
	connect( m_modPageBtn, SIGNAL(clicked()), this, SLOT(slot_modify_page()));
	connect( m_delPageBtn, SIGNAL(clicked()), this, SLOT(slot_del_page()));
	connect( m_eventPageCB, SIGNAL(activated(int)), this, SLOT(slot_refresh_filter(int)));
// 	connect( m_eventTypeListView, SIGNAL(itemClicked( QTreeWidgetItem* ,int)), this, SLOT(onEventTypeListViewClicked( QTreeWidgetItem*)));
	connect( m_eventTypeListView, SIGNAL(itemChanged( QTreeWidgetItem* ,int)), this, SLOT(slot_event_type_listview_clicked( QTreeWidgetItem*)));
	connect( m_rtuList, SIGNAL(itemChanged( QTreeWidgetItem* ,int)), this, SLOT(slot_event_type_listview_clicked( QTreeWidgetItem*)));
	connect( m_saveBtn, SIGNAL(clicked()), this, SLOT(slot_save_filter()));
	connect( m_cancelBtn, SIGNAL(clicked()), this, SLOT(slot_cancel_btn()));
	connect( m_pageList, SIGNAL(itemDoubleClicked(QTreeWidgetItem* ,int)), this, SLOT(slot_double_clicked_pagelist(QTreeWidgetItem*)));
	
	init_para();
	slot_refresh_pagelist();
	m_eventTypeListView->sortByColumn( -1 );
	m_rtuList->sortByColumn( -1 );
	m_pageList->sortByColumn( -1 );
	create_filter_tree();

	//设置QtabWidget头样式
// 	m_tabWidget->
// 		setStyleSheet("QTabWidget::tab-bar{alignment:left;}"
// 		"QTabBar::tab{color:#0F1923; font-size: 12px;min-width:100px; min-height:25px;border:0px;}"
// 		"QTabBar::tab:hover{background:rgb(255,255, 255, 100);}"
// 		"QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}");
	set_wnd_stylesheet();

}

Event_ui_set_dlg::~Event_ui_set_dlg()
{
	
}

void Event_ui_set_dlg::set_wnd_stylesheet()
{
	//设置QtabWidget头样式
	// 	m_tabWidget->
// 	setStyleSheet("QTabWidget::tab-bar{alignment:left;}"
// 		"QTabBar::tab{color:#0F1923; font-size: 12px;min-width:100px; min-height:25px;border:0px;}"
// 		"QTabBar::tab:hover{background:rgb(255,255, 255, 100);}"
// 		"QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}"
// 		);
	QFont font ;
 	font.setFamily( tr( "微软雅黑") ) ;
//	font.setFamily( tr( "黑体-简") ) ;
	setFont( font ) ;

// 	QString style_sheet;
// 	style_sheet = tr("QTabWidget::tab-bar{alignment:left;}\
// 		QTabBar::tab{color:#0F1923; font-size: 12px;min-width:100px; min-height:25px;border:0px;}\
// 		QTabBar::tab:hover{background:rgb(255,255, 255, 100);}\
// 		QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}\
// 		");
// 	m_tabWidget->setStyleSheet(style_sheet);

	m_tabWidget->setStyleSheet("QTabWidget::pane{border:none;}""QTabWidget::tab-bar{alignment:left;}"
		"QTabBar::tab{font-family: Microsoft YaHei;color:#0F1923; font-size: 12px;min-width:90px; min-height:30px;border:0px;}"
		"QTabBar::tab:hover{background:rgb(255,255, 255, 100);}"
		"QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}");

	QString style_sheet = tr("QDialog{background:white;}"
							"QComboBox {font-family: Microsoft YaHei;font-size: 12px;color: #3E464E;\
								min-width:60px;min-height:22px;border: 1px solid #C6CFD9;border-radius: 2px;}"
							"QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 28px;\
								border-top-right-radius: 2px;border-bottom-right-radius: 2px;}"
							"QComboBox::down-arrow{image: url(:/img/images/dropdown.png);}"
							"QComboBox::down-arrow:pressed{image: url(:/img/images/dropdown_pressed.png);}"
							"QLabel{font-family: Microsoft YaHei;font-size: 12px;color: #0F1923;}"
							"QLineEdit {font-family:Microsoft YaHei;font-size: 12px;border: 1px solid #C6CFD9;	border-radius: 2px;min-height:22px;}"
							"QPushButton{background:white;border-radius: 2px;border: 1px solid #4A90E2;\
								color: #4A90E2;font-family: Microsoft YaHei;font-size: 12px;\
								min-width:75px;min-height:23px;}"
							"QPushButton:hover{background:#0C2543;border: 0px;color:white;}"						
							"QHeaderView::section {font-family:Microsoft YaHei;font-size:12px;color: #0F1923;min-width:30;height:30px;\
							border: 0px solid #567dbc;border-left:0px;	border-right:0px;background:#F1F1F1}"
							);
	setStyleSheet(style_sheet);

	style_sheet = tr("QPushButton{background:#4A90E2;border-radius: 2px;\
						font-family: Microsoft YaHei;font-size: 12px;color: white;\
						min-width:75px;min-height:23px;}"
						"QPushButton:hover{background:#0C2543;}");
	m_addPageBtn->setStyleSheet(style_sheet);
	m_saveBtn->setStyleSheet(style_sheet);

	m_eventTypeListView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_eventTypeListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_rtuList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_rtuList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	//HorizontalScrollBarPolicy设置为needed和alwayson均可，不要设置成off
	//设置table的水平滚动条
	QHeaderView *pHeader=m_eventTypeListView->header();  
	pHeader->setResizeMode(QHeaderView::ResizeToContents);  
	pHeader->setStretchLastSection(false);

	pHeader=m_rtuList->header();  
 	pHeader->setMinimumSectionSize(186);
// 	pHeader->setResizeMode(QHeaderView::ResizeToContents);  
// 	pHeader->setResizeMode(QHeaderView::Stretch);  
	pHeader->setResizeMode(QHeaderView::Stretch);  
	pHeader->setStretchLastSection(false);

	style_sheet = tr(	"QTreeWidget{font-family: Microsoft YaHei;font-size: 12px;color:#3E464E;\
						background: #FAFAFA;border: none;}"
						"QTreeWidget::branch:closed:has-children { image: url(:img/images/drop_down_nomal.png);}\
						QTreeWidget::branch:open:has-children { image: url(:img/images/drop_down_click.png); }\
						QTreeWidget::branch{image:none;}\
						QTreeWidget::item{height:22px;}\
						QTreeWidget::item:hover{background-color:#7BBDEF}\
						QTreeWidget::item:selected{background-color:#4A90E2}"
						"QTreeWidget::indicator:unchecked{image:url(:img/images/checkbox_unchecked.png);}"
						"QTreeWidget::indicator:checked{image:url(:img/images/checkbox_checked.png);}"
						"QScrollBar{background:white;height:8px;}"
						"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:4px;}"
						"QScrollBar::handle:hover{background:gray;}"
						"QScrollBar::sub-line{background:transparent;}"
						"QScrollBar::add-line{background:transparent;}"
						"QScrollBar:vertical{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
						"QScrollBar::handle:vertical{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
						"QScrollBar::handle:vertical:hover{background:gray;}"
						"QScrollBar::add-line:vertical{{background:transparent;}"
						"QScrollBar::sub-line:vertical{background:transparent;}"	);
	m_eventTypeListView->setStyleSheet(style_sheet);
	m_pageList->setStyleSheet(style_sheet);
	m_rtuList->setStyleSheet(style_sheet);
	m_eventLevelList->setStyleSheet(style_sheet);

// 	style_sheet = tr("QScrollBar:vertical{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
// 		"QScrollBar::handle:vertical{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
// 		"QScrollBar::handle:vertical:hover{background:gray;}"
// 		"QScrollBar::add-line:vertical{{background:transparent;}"
// 		"QScrollBar::sub-line:vertical{background:transparent;}"	);
// 	
// 	m_eventTypeListView->verticalScrollBar()->setStyleSheet(style_sheet);
// 	m_pageList->verticalScrollBar()->setStyleSheet(style_sheet);
// 	m_rtuList->verticalScrollBar()->setStyleSheet(style_sheet);
// 	m_eventLevelList->verticalScrollBar()->setStyleSheet(style_sheet);

// 		m_eventTypeListView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
// 		m_eventTypeListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
// 		//HorizontalScrollBarPolicy设置为needed和alwayson均可，不要设置成off
// 		//设置table的水平滚动条
// 		QHeaderView *pHeader=m_eventTypeListView->header();  
// 		pHeader->setResizeMode(QHeaderView::ResizeToContents);  
// 		pHeader->setStretchLastSection(false);
// 	style_sheet = tr("QScrollBar:horizontal{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
// 		"QScrollBar::handle:horizontal{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
// 		"QScrollBar::handle:horizontal:hover{background:gray;}"
// 		"QScrollBar::add-line:horizontal{{background:transparent;}"
// 		"QScrollBar::sub-line:horizontal{background:transparent;}");
// 	m_eventTypeListView->horizontalScrollBar()->setStyleSheet(style_sheet);

}

void Event_ui_set_dlg::init_para()
{

	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_para *pPara = &( pMainWnd->m_db_para );

	m_station_para.clear();
	m_station_area_para.clear();
	m_station_area_map.clear();
	m_station_para = pPara->get_station_para() ;
	m_station_area_para = pPara->get_area_para();

	for (int i = 0; i < m_station_area_para.count(); i++)
	{
		m_station_area_map[m_station_area_para[i][0].toInt()] = m_station_area_para[i];
	}
}


void Event_ui_set_dlg::slot_refresh_pagelist( )
{
	m_pageList->clear();
	m_item_to_pageid_map.clear();

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_table	*pTable;
	int i;
	QTreeWidgetItem *pItem;
	uint count = pMainWnd->m_page_list.count() + pMainWnd->m_wnd_list.count() - 1;

	for( i = pMainWnd->m_wnd_list.count() - 1 ; i >=0 ; i -- )
	{
		pItem = new QTreeWidgetItem( m_pageList );
		pTable = pMainWnd->m_wnd_list.at( i );
		pItem->setText( 0, pTable->get_page_name() );
		pItem->setText( 1, tr( "窗口" ) );
		m_item_to_pageid_map[ pItem ] = count;
		count --;
	}

	for( i = pMainWnd->m_page_list.count() - 1 ; i >=0 ; i -- )
	{
		pItem = new QTreeWidgetItem( m_pageList );
		pTable = pMainWnd->m_page_list.at( i );
		pItem->setText( 0, pTable->get_page_name() );
		pItem->setText( 1, tr( "页面" ) );
		m_item_to_pageid_map[ pItem ] = count;
		count --;
	}
}

void Event_ui_set_dlg::show_page ( int )
{
	int index = m_tabWidget->currentIndex();
	
	switch( index )
	{
	case 0:
		slot_refresh_pagelist();
		break;
	case 1:
		slot_refresh_pageCB( 0 );
		break;
	}
}

void Event_ui_set_dlg::slot_add_page()
{
	if( m_pageNameEdit->text().isEmpty() )
	{
		DMessageBox::information( this ,tr( "提示" ), tr( "请输入页名称!" ) );
		return;
	}

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_table	*pTable ;

	pTable = new Event_table( &pMainWnd->m_event_cfg, &pMainWnd->m_db_para, pMainWnd,  "QEventTable"  );
	pTable->set_page_name( m_pageNameEdit->text() );
	pMainWnd->m_page_count++;
	pTable->set_page_index(pMainWnd->m_page_count);
	if( m_pageType->currentIndex() == 0 )
	{
		pMainWnd->add_page( pTable );
	}
	else
	{
// 		pTable = new QEventTable( &pMainWnd->m_eventCfg, &pMainWnd->m_dbPara, (QWidget*)(pMainWnd->m_splitter), "QEventTable"  );
// 		pTable->setPageName( m_pageNameEdit->text() );
		pMainWnd->add_wnd( pTable );
	}

	slot_refresh_pagelist( );
}


void Event_ui_set_dlg::slot_modify_page()
{
	if( m_pageNameEdit->text().isEmpty() )
	{
		DMessageBox::information( this ,tr( "提示" ), tr( "请输入页名称!" ) );
		return;
	}

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_table	*pTable;

	QTreeWidgetItem *pItem = m_pageList->currentItem();

	if( pItem == NULL )
		return;

	uint item = m_item_to_pageid_map[ pItem ];


	if( item >= pMainWnd->m_page_list.count() )
	{
		pTable = pMainWnd->m_wnd_list.at( item - pMainWnd->m_page_list.count() );
	}
	else
	{
		pTable = pMainWnd->m_page_list.at( item );
	}
	
	pTable->set_page_name( m_pageNameEdit->text() );
	pMainWnd->set_page_name( pTable, m_pageNameEdit->text() );

	slot_refresh_pagelist( );
}


void Event_ui_set_dlg::slot_del_page()
{

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();

	QTreeWidgetItem *pItem = m_pageList->currentItem();

	if( pItem == NULL )
		return;

	uint item = m_item_to_pageid_map[ pItem ];

	m_item_to_pageid_map.remove( pItem );

	if( item < pMainWnd->m_page_list.count() )
	{
		pMainWnd->del_page( item );
	}
	else
	{
		pMainWnd->del_wnd( item - pMainWnd->m_page_list.count() );
	}

	slot_refresh_pagelist( );
}


void Event_ui_set_dlg::slot_double_clicked_pagelist( QTreeWidgetItem * item)
{
	if( item == NULL )
		return;

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	m_pageNameEdit->setText( item->text( 0 ) );

	if( m_item_to_pageid_map[ item ] < pMainWnd->m_page_list.count() )
	{
		m_pageType->setCurrentIndex( 0 );
	}
	else
	{
		m_pageType->setCurrentIndex( 1 );
	}
}

void Event_ui_set_dlg::slot_refresh_pageCB( int item )
{
	m_eventPageCB->clear();
	
// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_table *pTable;
	uint i;
	int j = pMainWnd->m_page_list.count();
	for( i = 0; i < pMainWnd->m_page_list.count(); i ++ )
	{
		pTable = pMainWnd->m_page_list.at( i );
		m_eventPageCB->addItem( pTable->get_page_name() );
	}

	for( i = 0; i < pMainWnd->m_wnd_list.count(); i ++ )
	{
		pTable = pMainWnd->m_wnd_list.at( i );
		m_eventPageCB->addItem( pTable->get_page_name() );
	}

	slot_refresh_filter( 0 );
}


void Event_ui_set_dlg::slot_refresh_filter( int item )
{
// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_table *pTable;

	if( item < (int)pMainWnd->m_page_list.count() )
	{
		pTable = pMainWnd->m_page_list.at( item );
	}
	else
	{
		pTable = pMainWnd->m_wnd_list.at( item - pMainWnd->m_page_list.count() );
	}
	

	if( pTable == NULL )
		return;

	uint sort;
	QTreeWidgetItem * pItem, *pSubItem;
	EVENTTYPEDEF event;

	// 事项类别
// 	for( sort = 1, pItem = m_eventTypeListView->firstChild(); pItem != NULL; pItem = pItem->nextSibling(), sort ++ )
	for( sort = 1 ; sort < m_eventTypeListView->topLevelItemCount()+1 ; sort ++ )
	{
		pItem = m_eventTypeListView->topLevelItem(sort-1);
// 		((QCheckListItem *)pItem)->setOn( false );
		pItem->setCheckState( 0 ,Qt::Unchecked );
		event.sort = sort;
		if( pItem->childCount() == 0 )
		{
			event.type = 0;
			if( pTable->is_select( &event ) )
			{
// 				((QCheckListItem *)pItem)->setOn( true );
				pItem->setCheckState( 0 ,Qt::Checked );
			}
		}
		else
		{
			for ( int subIndex = 0; subIndex < pItem->childCount() ; subIndex++ )
			{
				pSubItem = pItem->child( subIndex );
				event.type = m_item_to_eventtype_map[ pSubItem ];
				if( pTable->is_select( &event ) )
				{
// 					pItem->setCheckState( 0 ,Qt::Checked );
					pSubItem->setCheckState( 0 ,Qt::Checked );
				}
				else
				{
					pSubItem->setCheckState( 0 ,Qt::Unchecked );
				}

			}
		}
	}

	// 场站代码
 	for( int rtuIndex = 0; rtuIndex < m_rtuList->topLevelItemCount() ;rtuIndex++)
 	{
		pItem = m_rtuList->topLevelItem(rtuIndex);
		pItem->setCheckState( 0 ,Qt::Unchecked );
		deal_child_items(pTable,pItem);
 	}


	// 事项级别
 	uint level;
	for( level = 0; level < m_eventLevelList->topLevelItemCount() ;level++)
	{
		pItem = m_eventLevelList->topLevelItem(level);
		if( pTable->is_select( level ))
		{
			pItem->setCheckState( 0 ,Qt::Checked );
		}
		else
		{
			pItem->setCheckState( 0 ,Qt::Unchecked );
		}
	}
}

void Event_ui_set_dlg::deal_child_items(Event_table *pTable,QTreeWidgetItem *pItem)
{
	QTreeWidgetItem *pSubItem;
	if( pItem->childCount() == 0 )
	{
		if( pTable->is_select(m_item_to_rtucode_map[ pItem ] ) )
		{
			pItem->setCheckState( 0 ,Qt::Checked );
		}
		else
		{
			pItem->setCheckState( 0 ,Qt::Unchecked );
		}
	}
	else
	{
		for ( int subIndex = 0; subIndex < pItem->childCount() ; subIndex++ )
		{
			pSubItem = pItem->child( subIndex );
			deal_child_items(pTable,pSubItem);
		}
	}

}

void Event_ui_set_dlg::create_filter_tree()
{
// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();

	Event_cfg *pCfg = &( pMainWnd->m_event_cfg );
	Event_para *pPara = &( pMainWnd->m_db_para );

	QTreeWidgetItem *pItem,*pSubItem;
	uint i, j;
	QString desc;
	QStringList desclist;
	uint types[ 1000 ];
	uint typenum;

// 	for( i = pPara->getEventSortCount(); i > 0 ; i -- )				//modify by chenkai 2013年5月3日 22:36:29
	for( i = 1; i <= pPara->get_event_sort_count() ; i++ )
	{
		if( !pPara->get_event_sort_desc( i, desc ) )
			continue;
		desclist.clear();
		desclist << desc;
		pItem = new QTreeWidgetItem( m_eventTypeListView, desclist );
		pItem->setCheckState(0, Qt::Unchecked );
		if( !pPara->get_event_types( i, types, typenum ) )
			continue;

		for( j = 0; j < typenum; j ++ )
		{
			if( !pPara->get_event_type_desc( types[ j ], desc ) )
				continue;
			desclist.clear();
			desclist << desc;
// 			pSubItem = new QCheckListItem( pItem, desc, QCheckListItem::CheckBox );		
			pSubItem = new QTreeWidgetItem( pItem, desclist );
			pSubItem->setCheckState(0, Qt::Unchecked );
			m_item_to_eventtype_map[ pSubItem ] = types[ j ];
		}
	}

// 	uint groupno;
// 	QStringList rtucodes;
// 	for( i = 0; i < pPara->getGroupCount(); i ++ )
// 	{
// 		if( !pPara->getGroupNo( i, groupno ) )
// 			continue;
// 
// 		if( !pPara->getStaCode( groupno, rtucodes ) )
// 			continue;
// 
// 		for( j = 0; j < rtucodes.count(); j ++ )
// 		{
// 			if( !pPara->getStaDesc( rtucodes[ j ], desc ) )
// 				continue;
// 
// 			desclist.clear();
// 			desclist << desc;
// 			pItem = new QTreeWidgetItem( m_rtuList, desclist );
// 			pItem->setCheckState( 0,Qt::Unchecked );
// 			m_item2RtuCodeMap[ pItem ] = rtucodes[ j ];
// 		}
// 	}


	QMap<int ,QTreeWidgetItem*> m_areaItemMap ;	//区域-qtreewidgetitem对应关系

	QStringList desc_group;
	desc_group.clear() ;
	desc_group << _C2Q("未分组厂站")  ;
	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( m_rtuList, desc_group ) ;
	firstStaItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块
	if (pMainWnd->m_cur_oper_man.type != 0)
	{
	firstStaItem->setDisabled(true);
	}
	//显示第一层
	for ( j = 0 ; j < m_station_area_para.count() ; j++ )
	{
		if (m_station_area_para[ j ][2].toInt() == 0)	//masterid=0 第一级目录
		{
			desc_group.clear() ;
			desc_group.append( m_station_area_para[ j ][1] ) ;
			pItem = new QTreeWidgetItem( m_rtuList, desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_areaItemMap[ m_station_area_para[ j ][0].toInt() ] = pItem ;
			if (pMainWnd->m_cur_oper_man.type != 0)
			{
			if (m_station_area_para[ j ][0].toInt() != pMainWnd->m_cur_oper_man.type)
			{
			   pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
               pItem->setDisabled(true);
			}
			}
		}
	}
	//显示第二层
	for ( j = 0 ; j < m_station_area_para.count() ; j++ )
	{
		if (m_station_area_para[ j ][2].toInt() != 0)	//masterid!=0 第二级目录
		{
			desc_group.clear() ;
			desc_group.append( m_station_area_para[ j ][1] ) ;
			pItem = new QTreeWidgetItem( m_areaItemMap[m_station_area_para[j][2].toInt()], desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_areaItemMap[ m_station_area_para[ j ][0].toInt() ] = pItem ;
		}
	}


	for( j = 0; j < m_station_para.count(); j ++ )
	{
		if( m_station_para[j][1].isEmpty())
			continue;

		QStringList desc_rtu;
		desc_rtu << m_station_para[j][1] ;

		QStringList tmp_station = m_station_para[j] ;

		if (m_station_area_map.contains(tmp_station[3].toInt()) && (tmp_station[3].toInt() !=0))
		{
			pItem = m_areaItemMap[ tmp_station.at(3).toInt()] ;
			pSubItem = new QTreeWidgetItem( pItem, desc_rtu);
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		}
		else
		{
			pSubItem = new QTreeWidgetItem( /*m_rtuList->topLevelItem( 0 )*/firstStaItem, desc_rtu);
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		}
		m_item_to_rtucode_map[ pSubItem ] = tmp_station[0];
	}

	if (firstStaItem->childCount() == 0)
	{
		int index_no = m_rtuList->indexOfTopLevelItem( firstStaItem ) ;
		m_rtuList->takeTopLevelItem(index_no);
	}



	char text[ 256 ];
	for( i = 0; i < 8; i ++ )
	{
		sprintf( text, "%d级", i );
		desclist.clear();
		desclist << tr(text);
// 		pItem = new QCheckListItem( m_eventLevelList, _C2Q( text ), QCheckListItem::CheckBox );
		pItem = new QTreeWidgetItem( m_eventLevelList, desclist );
		pItem->setCheckState( 0,Qt::Unchecked );
	}
}

void Event_ui_set_dlg::slot_event_type_listview_clicked( QTreeWidgetItem * item )
{

	if(Qt::PartiallyChecked!=item->checkState(0))  
		set_child_checkstate(item,item->checkState(0));  

	if(Qt::PartiallyChecked==item->checkState(0))  
		if(!is_topitem(item))  
			item->parent()->setCheckState(0,Qt::PartiallyChecked);      


/*
	if( pItem == NULL )
		return;
	
	bool checked;

	Qt::CheckState state = pItem->checkState(0);
	if( state == Qt::PartiallyChecked )
		return;
	else if( state == Qt::Checked )
		checked = true;
	else
		checked = false;

 	QTreeWidgetItem *pSubItem = pItem->child(0);
 	for( int i =0; i < pItem->childCount() ;i++ )
 	{
		pSubItem = pItem->child(i);
//  	pSubItem->setCheckState( checked );				//modify by chenkai
		pSubItem->setCheckState( 0,state );
	}

	if( !checked )
		return;

// 	QCheckListItem *pParentItem = (QCheckListItem*)pItem->parent();
	QTreeWidgetItem *pParentItem = pItem->parent();
	while( pParentItem != NULL )
	{
		pParentItem->setCheckState( 0,state );
		pParentItem = pParentItem->parent();
	}
	*/
}

void Event_ui_set_dlg::slot_save_filter()
{
	uint sort;
	uint level;
	QTreeWidgetItem * pItem, *pSubItem,*pSubSubItem;
	QTreeWidgetItem *pCheckItem;
	EVENTTYPEDEF event;

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_table *pTable;

	int item = m_eventPageCB->currentIndex();
	
	if( item < (int)pMainWnd->m_page_list.count() )
	{
		pTable = pMainWnd->m_page_list.at( item );
	}
	else
	{
		pTable = pMainWnd->m_wnd_list.at( item - pMainWnd->m_page_list.count() );
	}

	pTable->reset_filter();
// 	pTable->clear();

// 	for( sort = 1, pItem = m_eventTypeListView->firstChild(); pItem != NULL; pItem = pItem->nextSibling(), sort ++ )
	for( sort = 1 ; sort < m_eventTypeListView->topLevelItemCount()+1 ; sort ++ )
	{
		pItem = m_eventTypeListView->topLevelItem( sort -1 );
// 		pCheckItem = (QCheckListItem*)pItem;
// 		if( !pCheckItem->isOn() )
		if( pItem->checkState(0) == Qt::Unchecked)
		continue;

		event.sort = sort;

		if( pItem->childCount()  == 0 )
		{
			event.type = 0;
			
			pTable->add_event_type( &event );
		}
		else
		{
// 			for( pSubItem = pItem->firstChild(); pSubItem != NULL;pSubItem = pSubItem->nextSibling() )
			for( int i = 0; i < pItem->childCount() ; i++ )
			{
				pSubItem = pItem->child(i);
// 				pCheckItem = (QCheckListItem*)pSubItem;
// 				if( !pCheckItem->isOn() )
				if( pSubItem->checkState(0)!= Qt::Checked)
					continue;

				if( m_item_to_eventtype_map.find( pSubItem ) == m_item_to_eventtype_map.end() )
					continue;

				event.type = m_item_to_eventtype_map[ pSubItem ];
				pTable->add_event_type( &event );
			}
		}
		
	}

// 	for( pItem = m_rtuList->firstChild(); pItem != NULL; pItem = pItem->nextSibling() )
	for( int i = 0; i < m_rtuList->topLevelItemCount() ;i++ )
	{
		pItem = m_rtuList->topLevelItem(i);
		if (pItem->isDisabled())
		{
			continue;
		}
		if( pItem->checkState(0) == Qt::Checked )				
			continue;
		if (pItem->childCount() == 0)
			continue;
		//第二层
		for( int j = 0; j < pItem->childCount() ; j++ )
		{
			pSubItem = pItem->child(j);
			if( pSubItem->checkState(0)== Qt::Checked)
				continue;
			if (pSubItem->childCount()>0)
			{
				//第三层
				for (int k=0;k<pSubItem->childCount();k++)
				{
					pSubSubItem = pSubItem->child(k);
					if( pSubSubItem->checkState(0)== Qt::Checked)
						continue;
					if( m_item_to_rtucode_map.find( pSubSubItem ) == m_item_to_rtucode_map.end() )
						continue;
					// 			QString cc = m_item2RtuCodeMap[ pSubItem ];
					pTable->add_rtu( m_item_to_rtucode_map[ pSubSubItem ] );

				}
			}
			if( m_item_to_rtucode_map.find( pSubItem ) == m_item_to_rtucode_map.end() )
				continue;
// 			QString cc = m_item2RtuCodeMap[ pSubItem ];
			pTable->add_rtu( m_item_to_rtucode_map[ pSubItem ] );
		}

		if( m_item_to_rtucode_map.find( pItem ) == m_item_to_rtucode_map.end() )
			continue;

		pTable->add_rtu( m_item_to_rtucode_map[ pItem ] );
	}

// 	for( level = 0, pItem = m_eventLevelList->firstChild(); pItem != NULL; pItem = pItem->nextSibling(), level ++ )
	for( level = 0 ; level < m_eventLevelList->topLevelItemCount() ; level ++ )
	{
		pItem = m_eventLevelList->topLevelItem(level);
// 		pCheckItem = (QCheckListItem*)pItem;
		if( pItem->checkState(0)!= Qt::Checked )
			continue;

		pTable->add_event_level( level );
	}
}

void Event_ui_set_dlg::slot_cancel_btn()
{
	reject();
}


void Event_ui_set_dlg::set_child_checkstate(QTreeWidgetItem *item, Qt::CheckState cs)  
{  
	if(!item) return;  
	for (int i=0;i<item->childCount();i++)  
	{  
		QTreeWidgetItem* child=item->child(i);  
		if(child->checkState(0)!=cs)  
		{  
			child->setCheckState(0, cs);  
		}  
	}  
	set_parent_checkstate(item->parent());  
}  

void Event_ui_set_dlg::set_parent_checkstate(QTreeWidgetItem *item)  
{  
	if(!item) return;  
	int selectedCount=0;  
	int childCount = item->childCount();  
	for (int i=0;i<childCount;i++)  
	{  
		QTreeWidgetItem* child= item->child(i);  
		if(child->checkState(0)==Qt::Checked)   
		{  
			selectedCount++;  
		}  
	}  

	if(selectedCount == 0) {  
		item->setCheckState(0,Qt::Unchecked);  
	} else if (selectedCount == childCount) {  
		item->setCheckState(0,Qt::Checked);  
	} else {  
		item->setCheckState(0,Qt::PartiallyChecked);  
	}  
}  

bool Event_ui_set_dlg::is_topitem(QTreeWidgetItem* item)  
{  
	if(!item) return false;  
	if(!item->parent()) return true;  
	return false;  
}  