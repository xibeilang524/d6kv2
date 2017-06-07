#include <QtCore/QList>
#include <QtGui/QMenu>
#include <QApplication>
#include "rawviewwnd.h"


QRawViewWnd::QRawViewWnd( QWidget *parent /* = NULL */ ): QSplitter( parent ) , m_fes_shm(  Fes_shm::get() ) , m_fes_data_shm( Fes_data_shm_accessor::get() )
{
	m_pChanTree = new QTreeWidget( this );
	m_pRawFrame = new QRawViewFrame( this );

	m_rtuIcon.addPixmap( QPixmap( ":res/unselect.png" ),QIcon::Normal, QIcon::Off );
	m_rtuIcon.addPixmap( QPixmap( ":res/select.png" ),QIcon::Selected, QIcon::Off );

	m_rootIcon.addPixmap( QPixmap( ":res/main_channel.png" ),QIcon::Normal, QIcon::Off );

	connect( m_pChanTree, SIGNAL( itemSelectionChanged(  ) ), this, SLOT( onTreeItemChange(  ) ) );

	m_pChanTree->setHeaderLabels( QStringList(  )<<_C2Q( "通道/终端号" )<<_C2Q( "名称" ) );
	m_pChanTree->setColumnWidth( 0 , 125 ) ;
    
	set_all_chan_rtu();

	m_pTimer = new QTimer(  );
	connect( m_pTimer, SIGNAL( timeout(  ) ), this, SLOT( onTimer(  ) ) );
	m_pTimer->start( 1000 );

	connect( m_pChanTree, SIGNAL( itemPressed( QTreeWidgetItem *, int ) ), this, SLOT( itemClicked( QTreeWidgetItem *, int ) ) );

	addWidget( m_pChanTree );
	addWidget( m_pRawFrame );
 	setStretchFactor( 0, 0 );
 	setStretchFactor( 1, 1 );
	setGeometry( 200, 100, 860, 500 );
	setWindowTitle( _C2Q( "生数据查看" ) );	

	initStyleSheet() ;
}

QRawViewWnd::~QRawViewWnd(  )
{

}


void QRawViewWnd::onTreeItemChange(  )
{
	QList<QTreeWidgetItem*> list = m_pChanTree->selectedItems(  );
	QTreeWidgetItem* rtu = list[0];
	bool ok = false;

	if (rtu->childCount(  ) > 0)
	{
		return;
	}
	else
	{
		if (rtu->data(0,Qt::UserRole).toInt()<=0)
		{
			return;
		}
	}


	QTreeWidgetItem* parent_channel = rtu->parent();

	int chan_no = parent_channel->text( 0 ).toInt( &ok, 10 );

	int rtuNo = rtu->text( 0 ).toInt( &ok, 10 );

	if ( true == ok )
	{
		m_pTimer->stop(  );
		m_pRawFrame->setPara( rtuNo , chan_no);
		m_pTimer->start(  1000  );
	}
}

void QRawViewWnd::onTimer(  )
{
	m_pRawFrame->fillData(  );
}

void QRawViewWnd::itemClicked( QTreeWidgetItem *item, int column )
{
	if ( qApp->mouseButtons(  ) == Qt::LeftButton )
	{
		return;
	}
	if ( qApp->mouseButtons(  ) == Qt::RightButton )
	{
		if ( item->childCount(  ) > 0 )
		{
			return;
		}
		showMenu(  );
	}
}

void QRawViewWnd::showMenu(  )
{
	QPoint pos;
	QMenu menu( m_pChanTree );
	QAction* reloadAction = menu.addAction( _C2Q( "重新载入" ) );
	connect( reloadAction, SIGNAL( triggered(  ) ), this, SLOT( reload(  ) ) );
	menu.exec( QCursor::pos(  ) );
}

void QRawViewWnd::reload(  )
{
	this->onTreeItemChange(  );
}


int QRawViewWnd::readstationarea_data()
{
   int ret =-1;
	m_station_area.clear();
    //DBApi db;
	if (m_db2.Open( DB_OP_R,FALSE,FALSE) != CS_SUCCEED)
	{
		printf("打开数据库失败，返回！\n");
		return ret;
	}

	CS_DATAFMT * datafmt = NULL;
	FUNC_STR  func;
	func.func = SEL_ISQL_RESULTS;
	strcpy(func.dbname, DB_NAME);
	strcpy( func.table, "STATIONAREA" );
	strcpy(func.isql, "select * from STATIONAREA");
	STATIONAREAPARA * db_stationarea = NULL;

	if (CS_SUCCEED == m_db2.SelectIsqlResults(&func, &datafmt, (void **)&db_stationarea))
	{
		ret = func.ret_roxnum;
	}

	if (ret > 0 )
	{
		for (int i = 0; i < ret; i++)
		{
			STATIONAREAPARA *tmp_db_stationarea = new STATIONAREAPARA;
			*tmp_db_stationarea = *(db_stationarea + i);
			m_station_area.insert(tmp_db_stationarea->id,tmp_db_stationarea);
		}
	}

	if (datafmt)
	{
		free(datafmt);
		datafmt = NULL;
	}

	if (db_stationarea)
	{
		free(db_stationarea);
	}
	m_db2.Close(  );
	return ret > 0 ? 0 : -1;
}

int QRawViewWnd::read_channeldata()
{
	int ret = -1;
	m_channelpara.clear();
	//DBApi db_api;

	if (m_db2.Open( DB_OP_R,FALSE,FALSE) != CS_SUCCEED)
	{
		printf("打开数据库失败，返回！\n");
		return ret;
	}

	CS_DATAFMT * datafmt = NULL;
	FUNC_STR  func;
	func.func = SEL_ISQL_RESULTS;
	strcpy(func.dbname, DB_NAME);
	strcpy( func.table, "subareachannelview" );
	strcpy(func.isql, "select 通道序号,终端序号,id from subareachannelview");
	CHANNELPARA * db_channel = NULL;

	if (CS_SUCCEED == m_db2.SelectIsqlResults(&func, &datafmt, (void **)&db_channel))
	{
		ret = func.ret_roxnum;
	}

	if (ret > 0 )
	{
		for (int i = 0; i < ret; i++)
		{
			CHANNELPARA *tmp_db_station = new CHANNELPARA;
			*tmp_db_station = *(db_channel + i);
			m_channelpara.insert(tmp_db_station->channo,tmp_db_station);
		}
	}

	if (datafmt)
	{
		free(datafmt);
		datafmt = NULL;
	}

	if (db_channel)
	{
		free(db_channel);
	}

	m_db2.Close(  );

	return ret > 0 ? 0 : -1;
}


void QRawViewWnd::set_all_chan_rtu(  )
{
	int channel_no;
	CHANPARA* para_ptr = NULL;
	CHANDATA* data_ptr = NULL;
	RTUPARA*  rtu_ptr = NULL;
	readstationarea_data();
    read_channeldata();

	QTreeWidgetItem *pItem,*pSubItem;
	QMap<int ,QTreeWidgetItem*> m_groupItemMap ;

	QStringList desc_group;
	desc_group.clear() ;
	desc_group << _C2Q("未分组厂站")  ;
	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( m_pChanTree, desc_group ) ;
	// 	firstStaItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
	m_groupItemMap[0 ] = firstStaItem ;
	//QIcon iconArea,iconStation;
	//iconArea.addFile(QString::fromUtf8(":/res/res/house.png"), QSize(), QIcon::Normal, QIcon::On);
	//iconStation.addFile(QString::fromUtf8(":/res/res/one.png"), QSize(), QIcon::Normal, QIcon::On);


	//先显示第一层
	QMapIterator<int , PSTATIONAREAPARA> area(m_station_area);
	while (area.hasNext()) 
	{
		area.next();
		if (area.value()->masterid == NULL)
		{
			desc_group.clear() ;
			desc_group.append( _C2Q(area.value()->name) ) ;
			pItem = new QTreeWidgetItem( m_pChanTree, desc_group ) ;
			// 			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_groupItemMap[area.value()->id ] = pItem ;
			//pItem->setIcon(0,iconArea);
		}
	}
	//显示第二层,区域表只可以表示到2层
	QMapIterator<int , PSTATIONAREAPARA> area2(m_station_area);
	while (area2.hasNext()) 
	{
		area2.next();
		if (area2.value()->masterid != NULL)
		{
			desc_group.clear() ;
			desc_group.append( _C2Q(area2.value()->name) ) ;
			pItem = new QTreeWidgetItem( m_groupItemMap[area2.value()->masterid ] , desc_group ) ;
			// 			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_groupItemMap[area2.value()->id ] = pItem ;
			//pItem->setIcon(0,iconArea);
		}
	}

	for ( int i = 0; i < MAX_CHAN_NUM; i++ )
	{
		para_ptr = m_fes_shm.chan_para_ptr + i;
		data_ptr = m_fes_shm.chan_data_ptr + i;

		if ( !strcmp( para_ptr->desdribe, "" ) ||  !(para_ptr->valid) )
		{
			continue;
		}

		rtu_ptr = m_fes_shm.rtu_para_ptr + para_ptr->child_rtu;

		QStringList colum, rtu_colum;
		QTreeWidgetItem* rtu_tmp = NULL;
		colum<<QString::number( para_ptr->order )<<_C2Q( para_ptr->desdribe );	//根节点为通道,通道下为其所挂终端,加入主通道标志
		short channelareain=0;
		if (m_channelpara.contains(para_ptr->order))
		{
			channelareain  = m_channelpara[para_ptr->order]->areaid;
		}
		if (m_groupItemMap.contains(channelareain))
		{
			pSubItem = new QTreeWidgetItem(m_groupItemMap[channelareain] , colum );
		}

		printf("FFFFFFFFFFFFFFF  ok!\n");
		fflush(stdout);

		if (rtu_ptr->valid)
		{
			rtu_colum<<QString::number( rtu_ptr->order )<<_C2Q( rtu_ptr->desdribe );
			rtu_tmp = new QTreeWidgetItem( rtu_colum );
			pSubItem->addChild(rtu_tmp);
			rtu_tmp->setIcon( 0, m_rtuIcon );
			rtu_tmp->setData(0,Qt::UserRole,1);
		}

		if (data_ptr->master_flag)		//主通道
		{
			pSubItem->setIcon( 0, m_rootIcon );			
		}
		//m_pChanTree->addTopLevelItem( tmp );
	}

}

void QRawViewWnd::initStyleSheet()
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
						   QListView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QListView::indicator:checked\
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