// QEventSysSetDlg.cpp: implementation of the QEventSysSetDlg class.
//
//////////////////////////////////////////////////////////////////////


#include <QApplication>
#include <QPixmap>
#include <QColorDialog>
#include <QtGui/QFileDialog>

#include "event_sys_set_dlg.h"
#include "event_real_wnd.h"
#include "event_cfg.h"
#include "event_para.h"
#include "globaldef.h"

#include "phone_no_edit.h"

extern QWidget* getMainWnd();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QEventSysSetDlg::QEventSysSetDlg( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl ):QDialog( parent ,fl)
{
	setupUi(this);
	setModal(modal);
	connect(m_eventColorCB2, SIGNAL(activated(int)), this, SLOT(onEventColor(int)));
	connect(m_colorList, SIGNAL(itemDoubleClicked(QListWidgetItem* )), this, SLOT(onDoubleClickedColorList(QListWidgetItem*)));
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOk()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
	connect(m_colUpBtn, SIGNAL(clicked()), this, SLOT(onColUp()));
	connect(m_colDownBtn, SIGNAL(clicked()), this, SLOT(onColDown()));
	connect( m_reportTree , SIGNAL( itemClicked ( QTreeWidgetItem * , int ) ) , this , SLOT( onReportTreeClicked( QTreeWidgetItem * , int ) ) ) ;
	connect( m_reportTree, SIGNAL( itemClicked(QTreeWidgetItem* ,int) ), this, SLOT( onReportTreeListViewClicked(QTreeWidgetItem* ,int) ) );		//2014-7-23

	connect( m_reportFileBtn , SIGNAL( clicked() ) , this , SLOT( onReportFileChoose() ) ) ;

// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
// 	m_reportChooseDlg = NULL ;

	if( pMainWnd->m_event_cfg.get_tone() )
		m_eventToneChkBox->setChecked( true );
	else
		m_eventToneChkBox->setChecked( false );

	m_toneDev->setText( pMainWnd->m_event_cfg.get_tone_dev() );

	if( pMainWnd->m_event_cfg.get_print() )
		m_eventPrintChkBox->setChecked( true );
	else
		m_eventPrintChkBox->setChecked( false );
	
	m_printer->setText(pMainWnd->m_event_cfg.get_printer());

	if( pMainWnd->m_event_cfg.get_keep_unconfirmed_event() )
		m_keepUnConfirmedEventChkBox->setChecked( true );
	else
		m_keepUnConfirmedEventChkBox->setChecked( false );

	if( pMainWnd->m_event_cfg.get_show_unconfirm() )
		m_waitforConfirmChkBox->setChecked( true );
	else
		m_waitforConfirmChkBox->setChecked( false );
	
	if( pMainWnd->m_event_cfg.get_auto_confirm_del() )
		m_AutoConfirmDelChkBox->setChecked( true );
	else
		m_AutoConfirmDelChkBox->setChecked( false );

	if( pMainWnd->m_event_cfg.get_auto_match() )
		m_chkNeedMatch->setChecked( true );
	else
		m_chkNeedMatch->setChecked( false );

//#ifdef _WIN32
	if( pMainWnd->m_event_cfg.get_send_msg() )
		m_msgChkBox->setChecked( true );
	else
		m_msgChkBox->setChecked( false );

	m_serial_no->setText( tr( "%1" ).arg( pMainWnd->m_event_cfg.get_serial_no() ) );
	m_rcvmsgTime->setText(tr( "%1" ).arg( pMainWnd->m_event_cfg.get_msg_confirm_time() ) );
// #else
// 	m_msgChkBox->setEnabled( false );
// 
// 	m_serial_no->setEnabled( false );
// #endif

	if( pMainWnd->m_event_cfg.get_rcv_msg() )
		m_rcvmsgChkBox->setChecked( true );
	else
		m_rcvmsgChkBox->setChecked( false );

	if( pMainWnd->m_event_cfg.get_dgb_out() )
		m_dbgChkBox->setChecked( true );
	else
		m_dbgChkBox->setChecked( false );

	if( pMainWnd->m_event_cfg.get_show_overhaul_event() )
		m_showJXEvent->setChecked( true );
	else
		m_showJXEvent->setChecked( false );

	m_eventFontSpinBox->setValue( pMainWnd->m_event_cfg.get_font_size() );

	m_eventNumSpinBox->setValue( pMainWnd->m_event_cfg.get_max_event_num() );

	m_eventColorCB1->setCurrentIndex( pMainWnd->m_event_cfg.get_color_type() );

	onEventColor( 0 );

	int i;
	QString desc;
	QStringList desclist;
	QTreeWidgetItem *pItem;
	QTableWidgetItem *pItem1 = NULL;
	QTableWidgetItem *pphone_item = NULL;

	uint index;
	m_colList->sortByColumn( -1 );
// 	for( i = LIST_COL_NUM - 1; i >= 0; i -- )
	for( i = 0; i < LIST_COL_NUM; i ++ )
	{
		desc = pMainWnd->m_event_cfg.get_col_name( i );

		if( !pMainWnd->m_event_cfg.get_col_id( i, index ) )
			continue;

// 		pItem = new QCheckListItem( m_colList, desc, QCheckListItem::CheckBox );
		desclist.clear();
		desclist << desc;
		pItem = new QTreeWidgetItem( m_colList, desclist );
		pItem->setCheckState( 0,Qt::Unchecked );

		if( pMainWnd->m_event_cfg.colIs_disp( index ) )
		{
// 			pItem->setOn( true );
			pItem->setCheckState( 0,Qt::Checked );
		}
		else
		{
// 			pItem->setOn( false );
			pItem->setCheckState( 0,Qt::Unchecked );
		}
	}

	PEVENTTYPEDEF pEventDef;

	int event_type_num = pMainWnd->m_db_para.get_event_type_count();

	m_soundTable->setRowCount( event_type_num );
	
	for( i = 0; i < event_type_num; i ++ )
	{
		pEventDef = pMainWnd->m_db_para.get_event_type( i );
// 		m_soundTable->setText( i, 0, QString().sprintf( "%d", pMainWnd->m_eventCfg.getEventSoundCount( pEventDef->type ) ) );
		m_soundTable->setItem( i, 0, new QTableWidgetItem( QString().sprintf( "%d", pMainWnd->m_event_cfg.get_event_sound_count( pEventDef->type ) )) );
		pMainWnd->m_db_para.get_event_type_desc( pEventDef->type, desc );
// 		m_soundTable->verticalHeader()->setLabel( i, desc );
		m_soundTable->setVerticalHeaderItem( i, new QTableWidgetItem( desc ) );

		//内容居中
		pItem1 = m_soundTable->item(i,0);
		if (pItem1)
		{
			pItem1->setTextAlignment(Qt::AlignCenter);
		}
	}
	
//#ifdef _WIN32
	m_phoneNoTable->setRowCount( event_type_num );
	
	for( i = 0; i < event_type_num; i ++ )
	{
		pEventDef = pMainWnd->m_db_para.get_event_type( i );
// 		m_phoneNoTable->setText( i, 0, pMainWnd->m_eventCfg.getPhoneNo( pEventDef->type ) );
		m_phoneNoTable->setItem( i, 0, new QTableWidgetItem( pMainWnd->m_event_cfg.get_type_phone_no( pEventDef->type ) ) );

		m_phoneNoTable->setItem( i, 1, new QTableWidgetItem( tr("点击查看详细配置") ) );

		pMainWnd->m_db_para.get_event_type_desc( pEventDef->type, desc );
// 		m_phoneNoTable->verticalHeader()->setLabel( i, desc );
		m_phoneNoTable->setVerticalHeaderItem( i, new QTableWidgetItem( desc ) );
		
		pphone_item = m_phoneNoTable->item(i,0);
		if (pphone_item)
		{
			pphone_item->setTextAlignment(Qt::AlignCenter);
		}
		pphone_item = m_phoneNoTable->item(i,1);
		if (pphone_item)
		{
			pphone_item->setTextAlignment(Qt::AlignCenter);
		}
	}

 	connect( m_phoneNoTable , SIGNAL( itemClicked(QTableWidgetItem *) ) , this , SLOT( onPhoneNoEdit(QTableWidgetItem *) ) ) ;

// #else
// 	tabWidget->page( 4 )->hide();
// #endif
	createReportTree() ;

	set_wnd_stylesheet();

}

QEventSysSetDlg::~QEventSysSetDlg()
{

}

void 	QEventSysSetDlg::set_wnd_stylesheet()
{
	QFont font ;
	font.setFamily( tr( "微软雅黑") ) ;	
	setFont( font ) ;

	tabWidget->setStyleSheet("QTabWidget::pane{border:none;}""QTabWidget::tab-bar{alignment:left;}"
		"QTabBar::tab{font-family: Microsoft YaHei;color:#0F1923; font-size: 12px;min-width:60px; min-height:30px;border:0px;}"
		"QTabBar::tab:hover{background:rgb(255,255, 255, 100);}"
		"QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}");

	QString  style_sheet = tr("QDialog{background:white;}"
		"QPushButton{background:#4A90E2;border-radius: 2px;\
			font-family: Microsoft YaHei;font-size: 12px;color: white;\
			min-width:75px;min-height:23px;}"
		"QPushButton:hover{background:#0C2543;}"
		"QLabel{font-family: Microsoft YaHei;font-size: 12px;color: #3E464E;}"
		"QLineEdit {font-family:Microsoft YaHei;font-size: 12px;color: #3E464E;border: 1px solid #C6CFD9;border-radius: 2px;min-height:22px;}"
		"QSpinBox{font-family:Microsoft YaHei;font-size: 12px;color: #4A4A4A;background: #FFFFFF;\
			border: 1px solid #C6CFD9;border-radius: 2px;min-height:23px;}"
		"QSpinBox::up-button{subcontrol-origin: padding;subcontrol-position: right;width: 24px;height: 24px;border-width: 3px;}"
		"QSpinBox::up-arrow{image: url(:/img/images/spinbox_add.png);width: 18px;height: 18px;}"
		"QSpinBox::down-button{subcontrol-origin: border;subcontrol-position: left;width: 24px;height: 24px;border-width: 3px;}"
		"QSpinBox::down-arrow{image: url(:/img/images/spinbox_remove.png);width: 18px;height: 18px;}"
		"QComboBox {font-family: Microsoft YaHei;font-size: 12px;color: #3E464E;\
			min-width:60px;min-height:22px;border: 1px solid #C6CFD9;border-radius: 2px;}"
		"QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 28px;\
			border-top-right-radius: 2px;border-bottom-right-radius: 2px;}"
		"QComboBox::down-arrow{image: url(:/img/images/dropdown.png);}"
		"QComboBox::down-arrow:pressed{image: url(:/img/images/dropdown_pressed.png);}"
		"QHeaderView::section {font-family:Microsoft YaHei;font-size:12px;color: #0F1923;width:100px;height:30px;\
			border: 0px solid #567dbc;border-left:0px;	border-right:0px;background:#F1F1F1}"
		"QCheckBox{font-family: Microsoft YaHei;font-size: 12px;color:#4A4A4A;}"
		"QCheckBox::indicator:unchecked{image:url(:/img/images/checkbox_unchecked.png);}"
		"QCheckBox::indicator:checked{image:url(:/img/images/checkbox_checked.png);}"
		"QRadioButton::indicator:unchecked{image:url(:/img/images/radio.png);}"
		"QRadioButton::indicator:checked{image:url(:/img/images/radio_checked.png);}"
		);
		setStyleSheet(style_sheet);

// 		m_colList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
// 		m_colList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_reportTree->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
		m_reportTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		//HorizontalScrollBarPolicy设置为needed和alwayson均可，不要设置成off
		//设置table的水平滚动条

		QHeaderView *pHeader=m_reportTree->header();  
		pHeader->setResizeMode(QHeaderView::ResizeToContents);  
		pHeader->setStretchLastSection(false);

// 		pHeader=m_colList->header();  
// 		pHeader->setResizeMode(QHeaderView::ResizeToContents);  
// 		pHeader->setStretchLastSection(false);

// 		QTreeWidget::branch:closed:has-children{ image: url(:img/images/drop_down_nomal.png);}\
// 			QTreeWidget::branch:open:has-children:has-siblings { image: url(:img/images/drop_down_click.png); }\
// 			QTreeWidget::branch{image:none;}\
		//有兄弟节点才有图标 "QTreeWidget::branch:closed:has-children:has-siblings{ image: url(:/res/drop_down_nomal.png);}"
		style_sheet = tr("QTreeWidget{font-family: Microsoft YaHei;font-size: 12px;color:#3E464E;\
							background: #FAFAFA;border: none;}"
							"QTreeWidget::item{height:22px;}\
							QTreeWidget::item:hover{background-color:wheat}\
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
		m_colList->setStyleSheet(style_sheet);
		m_reportTree->setStyleSheet(style_sheet);

		style_sheet = tr("QTableWidget{font-family: Microsoft YaHei;font-size: 12px;color:#3E464E;background: #FAFAFA;border: none;}"								
								"QScrollBar{background:white;height:8px;}"
								"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:4px;}"
								"QScrollBar::handle:hover{background:gray;}"
								"QScrollBar::sub-line{background:transparent;}"
								"QScrollBar::add-line{background:transparent;}"
								"QScrollBar:vertical{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
								"QScrollBar::handle:vertical{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
								"QScrollBar::handle:vertical:hover{background:gray;}"
								"QScrollBar::add-line:vertical{{background:transparent;}"
								"QScrollBar::sub-line:vertical{background:transparent;}");
		m_soundTable->setStyleSheet(style_sheet);
		m_phoneNoTable->setStyleSheet(style_sheet);

		m_soundTable->setAlternatingRowColors(true);	//隔行变色
		m_soundTable->setShowGrid(false); //设置不显示格子线
		m_phoneNoTable->setAlternatingRowColors(true);	//隔行变色
		m_phoneNoTable->setShowGrid(false); //设置不显示格子线

		style_sheet = tr("QListWidget{font-family: Microsoft YaHei;font-size: 12px;color:#3E464E;background: #FAFAFA;border: none;}"								
			"QScrollBar{background:white;height:8px;}"
			"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:4px;}"
			"QScrollBar::handle:hover{background:gray;}"
			"QScrollBar::sub-line{background:transparent;}"
			"QScrollBar::add-line{background:transparent;}"
			"QScrollBar:vertical{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
			"QScrollBar::handle:vertical{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
			"QScrollBar::handle:vertical:hover{background:gray;}"
			"QScrollBar::add-line:vertical{{background:transparent;}"
			"QScrollBar::sub-line:vertical{background:transparent;}");
		m_colorList->setStyleSheet(style_sheet);

		style_sheet = tr("QLabel{background:#F1F1F1;min-height:25px;}");
		label1->setStyleSheet(style_sheet);
		label2->setStyleSheet(style_sheet);

		style_sheet = tr("QPushButton{background:white;border-radius: 2px;border: 1px solid #4A90E2;\
						 color: #4A90E2;font-family: Microsoft YaHei;font-size: 12px;\
						 min-width:75px;min-height:23px;}"
						 "QPushButton:hover{background:#0C2543;border: 0px;color:white;}");
		buttonCancel->setStyleSheet(style_sheet);

		QHeaderView *pTableHeaderView = m_soundTable->verticalHeader();
		if (pTableHeaderView)
		{
			pTableHeaderView->setDefaultAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
		}
		pTableHeaderView = m_phoneNoTable->verticalHeader();
		if (pTableHeaderView)
		{
			pTableHeaderView->setDefaultAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
		}
		m_eventFontSpinBox->setAlignment(Qt::AlignCenter);
		m_eventNumSpinBox->setAlignment(Qt::AlignCenter);
}

void QEventSysSetDlg::onPhoneNoEdit(QTableWidgetItem * item)
{
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();

	PEVENTTYPEDEF pEventDef;

	int row_no = item->row();

	QTableWidgetItem* pphoneNo_item = m_phoneNoTable->item(row_no, 0);
	pEventDef = pMainWnd->m_db_para.get_event_type( row_no );

	if (item->column() == 1)
	{
		Phone_no_edit* pdlg = new Phone_no_edit(&(pMainWnd->m_event_cfg),pphoneNo_item->text(),pEventDef->type,this);
		pdlg->exec();
	}
}

void QEventSysSetDlg::onEventColor( int item )
{
	uint i, j;
// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();

	m_colorList->clear();
	m_itemColorList.clear();

	QPixmap pixmap( 16,16 );

	QString desc;
	QColor	clr;

	switch( item )
	{
	case EVENTSORTCOLOR:
		{
			for( i = 1; i < pMainWnd->m_db_para.get_event_sort_count(); i ++ )
			{
				if( !pMainWnd->m_db_para.get_event_sort_desc( i, desc ) )
					continue;

				if( pMainWnd->m_event_cfg.get_sort_color( i, clr ) )
				{
					pixmap.fill( clr );
				}
				else
				{
					pixmap.fill( Qt::black );
				}
				m_colorList->addItem( new QListWidgetItem( pixmap, desc ));
				m_itemColorList.append( clr ) ;
			}
		}
		break;
	case EVENTTYPECOLOR:
		{
			uint types[ 2000 ], typenum;
			uint curitem = 0;
			for( i = 1; i < pMainWnd->m_db_para.get_event_sort_count(); i ++ )
			{
				if( !pMainWnd->m_db_para.get_event_types( i, types, typenum ) )
					continue;

				for( j = 0; j < typenum; j ++ )
				{
					if( !pMainWnd->m_db_para.get_event_type_desc( types[ j ], desc ) )
						continue;
					if( pMainWnd->m_event_cfg.get_type_color( types[ j ], clr ) )
					{
						pixmap.fill( clr );
					}
					else
					{
						pixmap.fill( Qt::black );
					}
// 					m_colorList->insertItem( pixmap, desc );
					m_colorList->addItem( new QListWidgetItem( pixmap, desc ));
					m_itemColorList.append( clr ) ;
					m_item2EventTypeMap[ curitem ++ ] = types[ j ];
				}
			}
		}
		break;
	case EVENTRTUCOLOR:
		{
			uint groupno;
			QStringList rtucodes;
			uint curitem = 0;
			for( i = 0; i < pMainWnd->m_db_para.get_group_count(); i ++ )
			{
				if( !pMainWnd->m_db_para.get_group_no( i, groupno ) )
					continue;

				rtucodes.clear();
				if( !pMainWnd->m_db_para.get_station_code( groupno, rtucodes ) )
					continue;

				for( j = 0; j < rtucodes.count(); j ++ )
				{
					if( !pMainWnd->m_db_para.get_station_desc( rtucodes[ j ], desc ) )
						continue;

					if( pMainWnd->m_event_cfg.get_rtu_color( rtucodes[ j ], clr ) )
					{
						pixmap.fill( clr );
					}
					else
					{
						pixmap.fill( Qt::black );
					}

// 					m_colorList->insertItem( pixmap, desc );
					m_colorList->addItem( new QListWidgetItem( pixmap, desc ));
					m_itemColorList.append( clr ) ;
					m_item2RtuCodeMap[ curitem ++ ] = rtucodes[ j ];
				}
			}
		}
		break;
	case EVENTLEVELCOLOR:
		{
			for( i = 0; i < 8; i ++ )
			{
				if( pMainWnd->m_event_cfg.get_level_color( i, clr ) )
				{
					pixmap.fill( clr );
				}
				else
				{
					pixmap.fill( Qt::black );
				}
				desc = tr( "级别%1").arg( i );
// 				m_colorList->insertItem( pixmap, desc );
				m_colorList->addItem( new QListWidgetItem( pixmap, desc ));
				m_itemColorList.append( clr ) ;
			}
		}
		break;
	default:
		;
	}
}

void QEventSysSetDlg::onDoubleClickedColorList( QListWidgetItem * item )
{
	int curindex = m_colorList->currentRow();
	QListWidgetItem *curitem = m_colorList->currentItem();
	if( curitem < 0 )
		return;

	QColor retColor = QColorDialog::getColor( m_itemColorList[ curindex ], this );

	QPixmap pixmap( 16,16 );

	if( retColor.isValid() )
	{
		pixmap.fill( retColor );
// 		m_colorList->changeItem( pixmap, item->text(), curitem );
		curitem->setText(item->text());
		curitem->setIcon( pixmap );
		m_itemColorList[ curindex ] = retColor;
	}
}

void QEventSysSetDlg::onOk()
{
// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	uint i;

	//保存系统配置
	bool ret = m_eventToneChkBox->isChecked();
	pMainWnd->m_event_cfg.set_tone( ret );
	pMainWnd->m_event_cfg.set_tone_dev( m_toneDev->text() );

	ret = m_eventPrintChkBox->isChecked();
	pMainWnd->m_event_cfg.set_print( ret );
	pMainWnd->m_event_cfg.set_printer(m_printer->text());

	ret = m_keepUnConfirmedEventChkBox->isChecked();
	pMainWnd->m_event_cfg.set_keep_unconfirmed_event( ret );

//#ifdef _WIN32
	ret = m_msgChkBox->isChecked();
	pMainWnd->m_event_cfg.set_send_msg( ret );

	ret = m_rcvmsgChkBox->isChecked();
	pMainWnd->m_event_cfg.set_rcv_msg( ret );

	pMainWnd->m_event_cfg.set_serial_no( m_serial_no->text());
	pMainWnd->m_event_cfg.set_msg_confirm_time( (m_rcvmsgTime->text()).toUInt());
//	pMainWnd->m_eventCfg.setSerialNo( m_serial_no->text());
//#endif

	pMainWnd->m_event_cfg.set_font_size( m_eventFontSpinBox->value() );

	pMainWnd->m_event_cfg.set_color_type( (EmColoring)m_eventColorCB1->currentIndex() );

	pMainWnd->m_event_cfg.set_show_unconfirm( m_waitforConfirmChkBox->isChecked() );

	pMainWnd->m_event_cfg.set_auto_confirm_del( m_AutoConfirmDelChkBox->isChecked() );

	pMainWnd->m_event_cfg.set_auto_match( m_chkNeedMatch->isChecked() );
	Rsys_event::g_cfg_need_match=m_chkNeedMatch->isChecked();

	pMainWnd->m_event_cfg.set_max_event_num( m_eventNumSpinBox->value() );

	pMainWnd->m_event_cfg.set_dbg_out( m_dbgChkBox->isChecked() );

	pMainWnd->m_event_cfg.set_show_overhaul_event( m_showJXEvent->isChecked() );

	//保存颜色设置
	int item = m_eventColorCB2->currentIndex();

	switch( item )
	{
	case EVENTSORTCOLOR:
		{
			for( i = 0; i < m_colorList->count(); i ++ )
			{
				pMainWnd->m_event_cfg.add_sort_color( i + 1 , 
					m_itemColorList[ i ] );
			}
		}
		break;
	case EVENTTYPECOLOR:
		{
			for( i = 0; i < m_colorList->count(); i ++ )
			{
				pMainWnd->m_event_cfg.add_type_color( m_item2EventTypeMap[ i ], 
					m_itemColorList[ i ] );
			}
			
		}
		break;
	case EVENTRTUCOLOR:
		{
			for( i = 0; i < m_colorList->count(); i ++ )
			{
				pMainWnd->m_event_cfg.add_rtu_color( m_item2RtuCodeMap[ i ], 
					m_itemColorList[ i ] );
			}
			
		}
		break;
	case EVENTLEVELCOLOR:
		{
			for( i = 0; i < m_colorList->count(); i ++ )
			{
				pMainWnd->m_event_cfg.set_level_color( i, 
					m_itemColorList[ i ] );
			}
	
		}
		break;
	default:
		;
	}

	//保存列设置
	QTreeWidgetItem *pItem;
	for( i = 0; i < m_colList->topLevelItemCount() ; i ++ )
	{
		pItem = m_colList->topLevelItem(i);
		pMainWnd->m_event_cfg.set_col_disp_no( pMainWnd->m_event_cfg.get_colid_by_colname( pItem->text( 0 ) ), i );
		pMainWnd->m_event_cfg.set_col_disp( pItem->text( 0 ), pItem->checkState(0) );
	}

	pMainWnd->refresh_table();

	PEVENTTYPEDEF pEventDef;
	
	for( i = 0; i < pMainWnd->m_db_para.get_event_type_count(); i ++ )
	{
		pEventDef = pMainWnd->m_db_para.get_event_type( i );

// 		pMainWnd->m_eventCfg.setEventSoundCount( pEventDef->type, m_soundTable->text( i, 0 ).toInt() );
		pMainWnd->m_event_cfg.set_event_sound_count( pEventDef->type, m_soundTable->item( i, 0 )->text().toInt() );
	}

//#ifdef _WIN32

	for( i = 0; i < pMainWnd->m_db_para.get_event_type_count(); i ++ )
	{
		pEventDef = pMainWnd->m_db_para.get_event_type( i );

// 		pMainWnd->m_eventCfg.setPhoneNo( pEventDef->type, m_phoneNoTable->text( i, 0 ) );
		pMainWnd->m_event_cfg.set_phone_no( pEventDef->type, m_phoneNoTable->item( i, 0 )->text() );
	}
//#endif

	QTreeWidgetItem* pSubItem = NULL ;
	QTreeWidgetItem* pSubSubItem = NULL ;
	QString station ;
	ret = m_isReport->isChecked() ;
	pMainWnd->m_event_cfg.set_report_print( ret ) ;
	ret = m_vBtn->isChecked() ;
	pMainWnd->m_event_cfg.set_report_print_type( ret ) ;
	for( i = 0 ; i < m_reportTree->topLevelItemCount() ; i ++ )
	{
		QStringList reportEventTypeList , reportNameStrList ;
		pItem = m_reportTree->topLevelItem( i ) ;
		if ( pItem->checkState( 0 ) == Qt::Checked )
		{
			station = pItem->text( 0 ) ;
			for ( int j = 0 ; j < pItem->childCount() ; j++ )
			{
				pSubItem = pItem->child( j ) ;
				if ( pSubItem->checkState( 0 ) == Qt::Checked )
				{
					for ( int k = 0 ; k < pSubItem->childCount() ; k++ )
					{
						pSubSubItem = pSubItem->child( k ) ;
						if ( pSubSubItem->checkState( 0 ) == Qt::Checked )
						{
							reportEventTypeList << pSubSubItem->text( 1 ) ;
							reportNameStrList << pSubSubItem->text( 2 ) ;
						}
					}
				}
			}
			pMainWnd->m_event_cfg.set_report_event_type( m_rtuDesc2RtuNameMap[ station ], reportEventTypeList );
			pMainWnd->m_event_cfg.set_report_name( m_rtuDesc2RtuNameMap[ station ], reportNameStrList );
		}
	}

	accept();
}


void QEventSysSetDlg::onCancel()
{
	reject();
}

void QEventSysSetDlg::onColUp()
{
	QTreeWidgetItem *pItem = m_colList->currentItem();

// 	if( pItem == NULL || pItem == m_colList->firstChild() )
	if( pItem == NULL || pItem == m_colList->topLevelItem(0) )
		return;

// 	QTreeWidgetItem *pCheckItem;
// 
// 	QString aboveItemText = pItem->itemAbove()->text( 0 );
// 	pCheckItem = ( QCheckListItem* )( pItem->itemAbove() );
// 	bool checked = pCheckItem->isOn();
// 	pItem->itemAbove()->setText( 0, pItem->text( 0 ) );
// 	pCheckItem->setOn( ( (QCheckListItem*)pItem )->isOn() );
// 	pItem->setText( 0, aboveItemText );
// 	((QCheckListItem*)pItem)->setOn( checked );
// 
// 	m_colList->setCurrentItem( pItem->itemAbove() );
	QTreeWidgetItem *pUpItem;
 	QString aboveItemText = m_colList->itemAbove( pItem )->text( 0 );
 	pUpItem = m_colList->itemAbove( pItem );
	Qt::CheckState checked = pUpItem->checkState(0);
 	pUpItem->setText( 0, pItem->text( 0 ) );
 	pUpItem->setCheckState( 0,pItem->checkState(0) );
 	pItem->setText( 0, aboveItemText );
 	pItem->setCheckState( 0,checked );
 
 	m_colList->setCurrentItem( pUpItem );
}


void QEventSysSetDlg::onColDown()
{
// 	QListViewItem *pItem = m_colList->currentItem();
	QTreeWidgetItem *pItem = m_colList->currentItem();

if (pItem && m_colList->indexOfTopLevelItem(pItem) == (m_colList->topLevelItemCount() - 1)) //mdy 解决下移到最后一项程序退出的问题
{
	return;
}

// 	QCheckListItem *pCheckItem;
// 
// 	QString belowItemText = pItem->itemBelow()->text( 0 );
// 	pCheckItem = ( QCheckListItem* )( pItem->itemBelow() );
// 	bool checked = pCheckItem->isOn();
// 	pItem->itemBelow()->setText( 0, pItem->text( 0 ) );
// 	pCheckItem->setOn( ( (QCheckListItem*)pItem )->isOn() );
// 	pItem->setText( 0, belowItemText );
// 	((QCheckListItem*)pItem)->setOn( checked );
	QTreeWidgetItem *pDownItem;

	QString belowItemText = m_colList->itemBelow(pItem)->text( 0 );
	pDownItem = m_colList->itemBelow(pItem);
	Qt::CheckState checked = pDownItem->checkState(0);
	pDownItem->setText( 0, pItem->text( 0 ) );
	pDownItem->setCheckState( 0,pItem->checkState(0) );
	pItem->setText( 0, belowItemText );
	pItem->setCheckState( 0,checked );
	m_colList->setCurrentItem( pDownItem );
}

void QEventSysSetDlg::createReportTree()
{
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_para *pPara = &( pMainWnd->m_db_para );
	uint groupno ;
	QStringList rtuName ;
	QString desc ;
	QStringList descList , eventSortDescList ;
	uint types[ 1000 ] ;
	uint typenum ;
	QTreeWidgetItem* pItem = NULL ;
// 	QTreeWidgetItem* pTopItem = NULL ;
// 	QTreeWidgetItem* pSubItem = NULL ;
	QMap<QString , QStringList> eventTypeMap = pMainWnd->m_event_cfg.get_report_map() ;
	m_isReport->setChecked( pMainWnd->m_event_cfg.get_report_print() ) ;
// 	QMap<QString , QStringList> reportNameMap = pMainWnd->m_eventCfg.getReportMap() ;
	QStringList stationList = eventTypeMap.keys() ;
	m_rtuDesc2RtuNameMap.clear() ;

	for( int i = 0 ; i < pPara->get_group_count() ; i ++ )
	{
		if( !pPara->get_group_no( i, groupno ) )
			continue;

		if( !pPara->get_station_code( groupno, rtuName ) )
			continue;

		for( int j = 0 ; j < rtuName.count() ; j ++ )
		{
			if( !pPara->get_station_desc( rtuName[ j ], desc ) )
				continue;
			m_rtuDesc2RtuNameMap.insert( desc , rtuName[ j ] ) ;
			descList.clear();
			descList << desc;
			pItem = new QTreeWidgetItem( m_reportTree , descList ) ;
			if ( stationList.indexOf( rtuName[ j ] ) != -1 )
			{
				pItem->setCheckState(0, Qt::Checked );
			}
			else
			{
				pItem->setCheckState(0, Qt::Unchecked );
			}
		}
	}

	pItem = m_reportTree->topLevelItem( 0 ) ;
	pItem->setSelected( true ) ;
	m_reportTree->resizeColumnToContents( 0 ) ;
}

void QEventSysSetDlg::onReportTreeClicked( QTreeWidgetItem * item , int col )
{
	if ( col == 0 )
	{
		item->setSelected( true ) ;
		QTreeWidgetItem* pTopItem = NULL ;
		QTreeWidgetItem* pSubItem = NULL ;
		QTreeWidgetItem* pSubSubItem = NULL ;

		for ( int i = 0 ; i < item->childCount() ; i++ )
		{
			pSubItem = item->child( i ) ;
			pSubItem->setCheckState( 0 , item->checkState( 0 ) ) ;
			for ( int j =0 ; j < pSubItem->childCount() ; j++ )
			{
				pSubSubItem = pSubItem -> child( j ) ;
				pSubSubItem->setCheckState( 0 , item->checkState( 0 ) ) ;
			}
		}

		bool checked = false ;
		uint state = item->checkState( 0 );
		if( state == Qt::PartiallyChecked ||  state == Qt::Checked )
		{
			checked = true ;
		}
		else
		{
			checked = false;
		}
		if ( !checked )
		{
			return ;
		}

		pTopItem = item->parent();
		while( pTopItem != NULL )
		{
			pTopItem->setCheckState( 0 , item->checkState( 0 ) ) ;
			pTopItem = pTopItem->parent() ;
		}
	}
}

void QEventSysSetDlg::onReportTreeListViewClicked( QTreeWidgetItem * item , int col )
{
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_para *pPara = &( pMainWnd->m_db_para );
	uint groupno ;
	QStringList rtuName ;
	QString desc ;
	QStringList descList , eventSortDescList ;
	uint types[ 1000 ] ;
	uint typenum ;

	QTreeWidgetItem* pItem = NULL ;
	QTreeWidgetItem* pTopItem = NULL ;
	QTreeWidgetItem* pSubItem = NULL ;
	QMap<QString , QStringList> eventTypeMap = pMainWnd->m_event_cfg.get_report_map() ;

//toplevel的item才添加子item,已增加则不增加
	if (item->parent() || item->childCount())
	{
		return;
	}

	for( int i = 1 ; i <= pPara->get_event_sort_count() ; i++ )
	{
		if( !pPara->get_event_sort_desc( i, desc ) )
			continue;
		eventSortDescList.clear();
		eventSortDescList << desc ;
		pTopItem = item ;
		QString desc = m_rtuDesc2RtuNameMap[ pTopItem->text( 0 )];
		QStringList eventTypeList = eventTypeMap[ m_rtuDesc2RtuNameMap[ pTopItem->text( 0 ) ] ] ;
		QStringList reportNameList = pMainWnd->m_event_cfg.get_report_name( m_rtuDesc2RtuNameMap[ pTopItem->text( 0 ) ] ) ;
		pItem = new QTreeWidgetItem( pTopItem , eventSortDescList ) ;
		if( !pPara->get_event_types( i , types , typenum ) )
		{
			pItem->setCheckState(0, Qt::Unchecked ) ;
			continue;
		}

		for( int k = 0 ; k < typenum ; k ++ )
		{
			if( !pPara->get_event_type_desc( types[ k ], desc ) )
				continue;
			descList.clear();
			desc.replace( _C2Q( "/") , _C2Q( "-" ) ) ;
			descList << desc << QString::number(types[ k ]) ;
			pSubItem = new QTreeWidgetItem( pItem, descList );
			if ( eventTypeList.indexOf( QString::number( types[ k ] ) ) != -1 )
			{
				pItem->setCheckState(0, Qt::Checked ) ;
				pSubItem->setCheckState(0, Qt::Checked ) ;
				pSubItem->setText( 2 , reportNameList[ eventTypeList.indexOf( QString::number( types[ k ] ) ) ] ) ;
			}
			else
			{
				pItem->setCheckState(0, Qt::Unchecked ) ;
				pSubItem->setCheckState(0, Qt::Unchecked ) ;
			}
		}
	}
}

void QEventSysSetDlg::onReportFileChoose()
{
// 	if ( m_reportChooseDlg == NULL )
// 	{
// 		m_reportChooseDlg = new Report_choose( this ) ;
// 	}
	Report_choose m_reportChooseDlg(this) ;

	if ( m_reportChooseDlg.exec() == QDialog::Accepted )
	{
		if ( m_reportChooseDlg.m_reportList->currentItem() )
		{
			QListWidgetItem* pListItem = m_reportChooseDlg.m_reportList->currentItem() ;
			QString preportname = pListItem->text() ;
			QTreeWidgetItem* pItem = NULL ;
		 	QTreeWidgetItem* pSubItem = NULL ;
		 	QTreeWidgetItem* pSubSubItem = NULL ;
		 	if ( !preportname.isEmpty() )
		 	{
		 		pItem = m_reportTree->currentItem() ;
		 		if ( !pItem)
		 		{
		 			pItem = m_reportTree->topLevelItem( 0 ) ;
		 		}
				if ( pItem->childCount() == 0 )
				{
					pItem->setText( 2 , preportname ) ;
				}
		 
				else
				{
					for ( int i = 0 ; i < pItem->childCount() ; i++ )
					{
						pSubItem = pItem->child( i ) ;
						if ( pSubItem->childCount() == 0 )
						{
							pSubItem->setText( 2 , preportname ) ;
						}
						else
						{
							for ( int j = 0 ; j < pSubItem->childCount() ; j++ )
							{
								pSubSubItem = pSubItem->child( j ) ;
								pSubSubItem->setText( 2 , preportname ) ;
							}
						}
					}
				}
		 	}
		}
		else
		{
			QTreeWidgetItem* pItem = NULL ;
			QTreeWidgetItem* pSubItem = NULL ;
			QTreeWidgetItem* pSubSubItem = NULL ;
			QString preportname="";
			pItem = m_reportTree->currentItem() ;
			if ( !pItem)
			{
				pItem = m_reportTree->topLevelItem( 0 ) ;
			}
			if ( pItem->childCount() == 0 )
			{
				pItem->setText( 2 , preportname ) ;
			}
			else
			{
				for ( int i = 0 ; i < pItem->childCount() ; i++ )
				{
					pSubItem = pItem->child( i ) ;
					if ( pSubItem->childCount() == 0 )
					{
						pSubItem->setText( 2 , preportname ) ;
					}
					else
					{
						for ( int j = 0 ; j < pSubItem->childCount() ; j++ )
						{
							pSubSubItem = pSubItem->child( j ) ;
							pSubSubItem->setText( 2 , preportname ) ;
						}
					}
				}
			}
		}
	}

// 	QString filePath = QString( getenv( NBENV ) ) + _C2Q( "/report_file" ) ;
// 	QString strPath = QFileDialog::getOpenFileName( this , _C2Q( "选择报表" ) , filePath , _C2Q( "*.vts" ) ) ;
// 	QTreeWidgetItem* pItem = NULL ;
// 	QTreeWidgetItem* pSubItem = NULL ;
// 	QTreeWidgetItem* pSubSubItem = NULL ;
// 	if ( !strPath.isEmpty() )
// 	{
// 		pItem = m_reportTree->currentItem() ;
// 		if ( !pItem)
// 		{
// 			pItem = m_reportTree->topLevelItem( 0 ) ;
// 		}
// 		pItem->setText( 2 , strPath ) ;
// 
// 		for ( int i = 0 ; i < pItem->childCount() ; i++ )
// 		{
// 			pSubItem = pItem->child( i ) ;
// 			pSubItem->setText( 2 , strPath ) ;
// 			for ( int j = 0 ; j < pSubItem->childCount() ; j++ )
// 			{
// 				pSubSubItem = pSubItem->child( j ) ;
// 				pSubSubItem->setText( 2 , strPath ) ;
// 			}
// 		}
// 	}
	m_reportTree->resizeColumnToContents( 0 ) ;
}

