// QEventTable.cpp: implementation of the QEventTable class.
//
//////////////////////////////////////////////////////////////////////
#include "event_table.h"

#include <QFont>
#include <QDateTime>
#include <QHeaderView>
#include <QMenu>
#include <QCursor>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>

#include "event_para.h"
#include "event_cfg.h"
#include "globaldef.h"
#include "event_real_wnd.h"
//#include "login_dlg.h"
#include "uitools/login_widget.h"
#include "uitools/dmessagebox.h"
//  #include "xpm/confirm.xpm"
// #include "xpm/confirmall.xpm"
// #include "xpm/unconfirm.xpm"

// #include "xpm/eventalarm.xpm"
// #include "xpm/confirmed.xpm"
// #include "xpm/nonconfirm.xpm"
// #include "xpm/unconfirmed.xpm"
// #include "xpm/matching.xpm"
// #include "xpm/matched.xpm"
// #include "xpm/delete.xpm"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void saveFileName( SYS_EVENT *eve );
extern QWidget* getMainWnd();

Event_table::Event_table( Event_cfg *pCfg, Event_para *pEventPara, QWidget * parent, const char * name, Qt::WindowFlags f )
		:QTreeWidget( parent )
{	
	setWindowFlags(f);
	setIconSize(QSize(40,20));
	setSortingEnabled( true );
	m_null_pixmap = QIcon( ":/img/images/null.png" );		//空图标
	m_confirmed_pixmap = QIcon( ":/img/images/confirmed.png" );		//已确认
	m_unconfirm_pixmap = QIcon( ":/img/images/unconfirm.png" );		//未确认
	m_matching_pixmap = QIcon( ":/img/images/matching.png" );			//匹配中
	m_matched_pixmap = QIcon( ":/img/images/matched.png" );				//已匹配

	setIconSize(QSize(60,30));
// 	setIcon( QPixmap( eventalarm_xpm ) );
// 	setWindowIcon( QPixmap( eventalarm_xpm ) );
	setFrameShape(QFrame::NoFrame);
	setStyleSheet(QString::fromUtf8("background-image: url(:/img/images/bk.png);"));

// 	setPaletteBackgroundPixmap( qPixmapFromMimeSource( "bk.png" ) );
//	m_nullPixmap = QPixmap::fromMimeSource( "news.png" );
//	m_ConfirmedPixmap = QPixmap::fromMimeSource( "sign.png" );
//	m_unConfirmPixmap = QPixmap::fromMimeSource( "mail.png" );
//	m_MatchingPixmap = QPixmap::fromMimeSource( "person.png" );
//	m_matchedPixmap = QPixmap::fromMimeSource( "couple.png" );

//	setIcon( QPixmap::fromMimeSource( "ralarm.png" ) );


	m_lock = 0;
	
	m_cfg = pCfg;
	m_event_para = pEventPara;

	m_event_num = 0;

	m_bhave_new_event = false;

	refresh_table();

	m_right_pop_menu = new QMenu( this );
	m_right_pop_menu->addAction(  tr( "事项确认" ) );
	m_right_pop_menu->addAction(  tr( "确认所有" ) );
	m_right_pop_menu->addAction(  tr( "事项删除" ) );
	m_right_pop_menu->addAction(  tr( "曲线查看" ) );
	m_right_pop_menu->addAction(  tr( "接 线 图" ) );

	connect( m_right_pop_menu, SIGNAL( triggered( QAction* ) ), this, SLOT( slot_pop_menu( QAction* ) ) );

// 	connect( this, SIGNAL( rightButtonClicked ( QTreeWidgetItem *, const QPoint &, int ) ), this, SLOT( rightClicked( QTableWidgetItem *, const QPoint &, int ) ) );
	connect( this, SIGNAL( itemPressed ( QTreeWidgetItem * , int ) ), this, SLOT( right_clicked( QTreeWidgetItem *, int ) ) );
	connect( this, SIGNAL( itemClicked ( QTreeWidgetItem * , int ) ), this, SLOT( slot_current_page_changed( QTreeWidgetItem *, int ) ) );
	
	sortByColumn( 0 );
	
	setColumnWidth( 0, 120 );
// 	setShowSortIndicator( true );
	header()->setMovable( false );

// color:rgb(45, 48, 53);\

	QString styleSheet ;
	styleSheet = tr( "QHeaderView::section{ \
					 height:42px;\
					 font-size: 20px;\
					 color:rgb(79,79,79);\
					 border-radius: 1px;\
					 background-color: rgb(238,242,245);\
					}\
					QTreeView::item{\
					 height:36px; \
					 font-size: 18px;\
					 }\
					 QTreeView::branch{\
					 image:none;\
					 }\
					 QScrollBar{background:white;height:8px;}\
					 QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:4px;}\
					 QScrollBar::handle:hover{background:gray;}\
					 QScrollBar::sub-line{background:transparent;}\
					 QScrollBar::add-line{background:transparent;}\
					 ") ;
	setStyleSheet(styleSheet);
	setSelectionMode( QAbstractItemView::ContiguousSelection );
// 	setAllColumnsShowFocus( true );

	// add by chenkai 
// 	setSortingEnabled( false ); //不使能QT的自动排序
// 	header()->setSortIndicatorShown( true ); // 设置三角标识符可见
// 	header()->setClickable( true ); // 设置标题栏单击响应使能
// 	connect( header(), SIGNAL( sectionClicked(int) ), this, SLOT( onClickColumn(int) ) );
	m_fresh_timer = startTimer( 1000 );

	//设置tableview隔行显示不同颜色
	setAlternatingRowColors(true);
	QPalette palette = this->palette();
	palette.setColor(QPalette::AlternateBase,QColor("#D5D5D5"));
	setPalette(palette);
}

Event_table::~Event_table()
{
	clear();
}

void Event_table::add_event( Rsys_event * eve)
{
	TRY_LOCK;
	m_event_list.append( eve );
// 	Rsys_event *subEvent = m_eventList.last();
// 	saveFileName( subEvent );
}

void Event_table::timerEvent( QTimerEvent * e )
{
	QTreeWidget::timerEvent( e ); 

	uint i;
	static bool showlast = false;

	if( e->timerId() != m_fresh_timer )
		return;

	killTimer( m_fresh_timer );

	blockSignals( true );
	uint nCount = m_event_list.count();
// 	if (!m_eventList.isEmpty())
// 	{
// 		saveFileName( m_eventList.last() );
// 	}
	if( nCount > m_event_num )
	{	
		TRY_LOCK;
		for( i = m_event_num; i < nCount; i ++ )
		{
			fill_table( i, m_event_list.at( i ) );
// 			saveFileName( m_eventList.at( i ) );
		}
		m_event_num = nCount;
		showlast = false;
	}
	
	if( !showlast )
	{
// 		if( sortOrder() == Qt::AscendingOrder )			//modify by chenkai
//  		if ( topLevelItemCount() > 1)
// 		{	
// 			if ( topLevelItem(0)->data(0 ,Qt::DisplayRole ).toInt() <= topLevelItem(1)->data(0 ,Qt::DisplayRole ).toInt() )
			if( header()->sortIndicatorOrder() == Qt::AscendingOrder )
			{
				QTreeWidgetItem * pItem = topLevelItem( topLevelItemCount() -1);
				if(pItem) scrollToItem( pItem );
			}
			else
			{
				QTreeWidgetItem * pItem = topLevelItem( 0 );
	 			if(pItem) scrollToItem( pItem );
			}
/*		}*/
// 		}
// 		else
// 		{
// 			QTreeWidgetItem * pItem = topLevelItem( 0 );
// 			if(pItem) scrollToItem( pItem );
// 		}

		showlast = true;
	}
	blockSignals( false );
	m_fresh_timer = startTimer( 1000 );
}

void Event_table::del_event( Sys_event_list & eventlist )
{
	if( eventlist.count() == 0 )
		return;

	uint i;
	Rsys_event *pEvent;
	List_view_color_item *pItem;

	TRY_LOCK;

	int num = 0;

	for( i = 0; i < eventlist.count(); i ++ )
	{
		pEvent = eventlist.at( i );

		if( m_event_to_item_map.find( pEvent ) != m_event_to_item_map.end() )
		{
			pItem = m_event_to_item_map[ pEvent ];
			takeTopLevelItem( indexOfTopLevelItem((QTreeWidgetItem*)pItem ));
			delete pItem;
			num ++;

			m_event_to_item_map.remove( pEvent );
		}

// 		QEventFilter::delEvent( pEvent ) ;
		m_event_list.removeOne( pEvent );
// 		int j = m_eventList.indexOf(pEvent);
// 		if (j != -1)
// 			delete m_eventList.takeAt(j);
	}

	//重新设置行标签
	if( num > 0 )
	{
		m_event_num -= num;

		QString label;
		for( i = 0; i < m_event_num; i ++ )
		{
			pEvent = m_event_list.at( i );
			if( m_event_to_item_map.find( pEvent ) == m_event_to_item_map.end() )
			{
				continue;
			}
			pItem = m_event_to_item_map[ pEvent ];

			label.sprintf( "%d  ", i + 1 );
			pItem->setText( 0, label );
// 			pItem->setData( 0 ,Qt::DisplayRole , i + 1);
		}
	}
}

void Event_table::fill_table( int row, Rsys_event * eve )
{
	QString	text;
	uint i;

// 	Rsys_event *subEvent = eve;
// 	saveFileName( subEvent );

	const char *col_des[ ] = {
		"否",
		"是"
	};

	QColor clr;

	if( m_cfg == NULL && m_event_para == NULL )
		return ;

	if(eve->m_match) 
	{
		Rsys_event * e = eve->m_match;
		if( m_event_to_item_map.find( e ) != m_event_to_item_map.end() &&  e->m_bneed_confirm )
		{
			List_view_color_item * pItem = m_event_to_item_map[e];
			if(e->m_event_status == eConfirmed)
				pItem->setIcon( 0, m_confirmed_pixmap );
			else {
				if(e->need_match()) {
					if(e->m_match) 
						pItem->setIcon( 0, m_matched_pixmap );
					else
						pItem->setIcon( 0, m_matching_pixmap );
				}
				else 
					pItem->setIcon( 0, m_unconfirm_pixmap );
			}
		}
	}

	get_event_color( eve, clr );
	List_view_color_item *pItem = new List_view_color_item(m_cfg, this, clr );

	m_event_to_item_map[ eve ] = pItem;

	uint disporder;

	QString label;
	label.sprintf( "%d", row + 1 );
	pItem->setText( 0, label );
// 	QVariant label( row+1 );
	if( eve->m_bneed_confirm )
	{
			if(eve->m_event_status == eConfirmed)
				pItem->setIcon( 0, m_confirmed_pixmap );
			else {
				if(eve->need_match()) {
					if(eve->m_match) 
						pItem->setIcon( 0, m_matched_pixmap );
					else
						pItem->setIcon( 0, m_matching_pixmap );
				}
				else 
					pItem->setIcon( 0, m_unconfirm_pixmap );
			}
	}
	else
		pItem->setIcon( 0, m_null_pixmap );

// 	pItem->setData( 0 ,Qt::DisplayRole ,row+1);

	//adjustColumn( 0 );

	for( i = 0 ; i < LIST_COL_NUM; i ++ )
	{
		if( !m_cfg->get_col_disp_order( i, disporder ) )
		{
			continue;
		}
		text = tr( "" );
		switch( i )
		{
		case LIST_COL_SORT:
			{
				if( !m_event_para->get_event_sort_desc( eve->sort, text ) )
				{
					continue;
				}
			}
			break;
		case LIST_COL_TYPE:
			{
				if( !m_event_para->get_event_type_desc( eve->type, text ) )
				{
					continue;
				}
			}
			break;
		case LIST_COL_TIME:
			{
				ulong lYmd = (ulong)(eve->ymd);
				int Year=(int)(lYmd/(100l*100l));
				int Month=(int)((lYmd%(100l*100l))/100);
				int Day=(int)((lYmd%(100l*100l))%100);

				ulong lTmp = (ulong)(eve->hmsms);
				int Hour = (int)(lTmp/(100l*100l*1000l));
				int Min = (int)((lTmp%(100l*100l*1000l))/(100l*1000l));
				int Sec = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))/1000l);
				int Ms = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))%1000l);
				text.sprintf( "%04d.%02d.%02d--%02d:%02d:%02d:%03d",Year,Month,Day,Hour, Min, Sec, Ms);
			}
			break;
		case LIST_COL_DES:
			{
				text = tr( eve->text );
			}
			break;
		case LIST_COL_FLOAT_VAL:
			{
				text.sprintf( "%.2f",  (float)eve->val );
			}
			break;
		case LIST_COL_STATUS_VAL:
			{
				text.sprintf( "%d",  (uint)eve->state ); //状态值
			}
			break;
		case LIST_COL_GROUP_NAME:
			{
				if( !m_event_para->get_station_desc( tr( eve->Object ),
					text ) )
				{
					break;
				}
			}
			break;
		case LIST_COL_MEMBER_NAME0:
			{
				if( !m_event_para->get_yc_desc( tr( eve->member0 ),text ) && 
					!m_event_para->get_yx_desc( tr( eve->member0 ),text ) )
				{
					text = tr( eve->member0 );
				}
			}
			break;
		case LIST_COL_MEMBER_NAME1:
			{
				text = tr( eve->member1 );
			}
			break;
		case LIST_COL_MEMBER_NAME2:
			{
				text = tr( eve->member2 );
			}
			break;
		case LIST_COL_MEMBER_NAME3:
			{
				text = tr( eve->member3 );
			}
			break;
		case LIST_COL_GRAPH_NAME:
			{
				text = tr( eve->graph );
			}
			break;
		case LIST_COL_TONE_INFO:
			{
				text = tr( eve->tone );
			}
			break;
		case LIST_COL_SECU_LEV:
			{
				text.sprintf( "%d",  (uint)eve->security );
			}
			break;
		case LIST_COL_PRINT_FLAG:
			{
				text = tr( col_des[ eve->PrintOut%2 ] );  		
			}
			break;
		case LIST_COL_SAVE_FLAG:
			{
				text = tr( col_des[ eve->DiskOut%2 ] );  		
			}
			break;
		case LIST_COL_GRAPH_FLAG:
			{
				text = tr( col_des[ eve->GraphOut%2 ] );  		
			}
			break;
		case LIST_COL_WAV_FLAG:
			{
				text = tr( col_des[ eve->ToneOut%2 ] );  		
			}
			break;
		default:
			;
		}

// 		text = text.stripWhiteSpace();
		text = text.trimmed();
		pItem->setText( disporder + 1 , text );
		pItem->paintCell(disporder+1);
	}
}

void Event_table::refresh_table()
{
	TRY_LOCK;

	uint  i; 

	uint dispno, disporder;

	m_cur_font = font();
// 	m_cur_font.setFamily( tr( "华文细黑") ) ;
	m_cur_font.setFamily( tr( "微软雅黑") ) ;
	QColor clr;

	m_cur_font.setPixelSize( m_cfg->get_font_size() );

	setFont( m_cur_font );

	QString label;

	QTreeWidget::clear();

	uint addcolnum;

	if( m_cfg->get_disp_col_num() + 1 > (uint)columnCount() )
	{
		addcolnum = m_cfg->get_disp_col_num() + 1 - columnCount();
		columnCountChanged( columnCount() ,addcolnum+columnCount());
// 		for( i = 0; i < addcolnum; i ++ )
// 		{
// 			addColumn( _C2Q( "" ) );
// 		}
	}
	else if( m_cfg->get_disp_col_num() + 1 < (uint)columnCount() )
	{
		addcolnum = columnCount() - m_cfg->get_disp_col_num() - 1;
// 		for( i = 0; i < addcolnum; i ++ )
// 		{
// 			removeColumn( 0 );
// 		}
		columnCountChanged( columnCount() ,columnCount()-addcolnum);
	}

	headerItem()->setText( 0, tr( "序号" ) );

	setColumnWidth( 0, 120 );
// 	setColumnWidthMode( 0, QListView::Manual );

	for( i = 0 ; i < LIST_COL_NUM; i ++ )
	{
		if( !m_cfg->get_col_disp_order( i, disporder ) )
		{
			continue;
		}
		m_cfg->get_col_disp_no( i , dispno );
		label = m_cfg->get_col_name( dispno );
		headerItem()->setText( disporder + 1, label );
		if( i == LIST_COL_SORT || i == LIST_COL_TYPE )
		{
			setColumnWidth( disporder + 1, 150 );
		}
		else if( i == LIST_COL_TIME )
		{
			setColumnWidth( disporder + 1, 270 );
		}
		else if( i == LIST_COL_DES )
		{
			setColumnWidth( disporder + 1, 500 );
		}
		else
		{
			setColumnWidth( disporder + 1, 100 );
		}
		
// 		setColumnWidthMode( disporder + 1, QListView::Manual );
	}

// 	setSelectionMode( Extended );
}

bool Event_table::get_selected_event( Sys_event_list & eventlist )
{
	uint i;
	List_view_color_item *pItem;
	Rsys_event *pEvent;

	TRY_LOCK;
	for( i = 0; i < m_event_num; i ++ )
	{
		pEvent = m_event_list.at( i );
		if( m_event_to_item_map.find( pEvent ) == 
			m_event_to_item_map.end() )
			continue;

		pItem = m_event_to_item_map[ pEvent ];
// 		if( isSelected( (QTreeWidgetItem*)pItem ) )
		if( (QTreeWidgetItem*)pItem -> isSelected())
			eventlist.append( m_event_list.at( i ) );
	}

	if( eventlist.count() == 0 )
		return false;

	return true;
}

void Event_table::confirm_event( Rsys_event * eve )
{
	TRY_LOCK;
	if( m_event_to_item_map.find( eve ) != m_event_to_item_map.end() )
	{
		List_view_color_item *pItem = m_event_to_item_map[ eve ];
		
		if( eve->m_bneed_confirm )
		{
			if(eve->m_event_status == eConfirmed)
				pItem->setIcon( 0, m_confirmed_pixmap );
			else 
			{
				if(eve->need_match()) {
					if(eve->m_match) 
						pItem->setIcon( 0, m_matched_pixmap );
					else
						pItem->setIcon( 0, m_matching_pixmap );
				}
				else 
					pItem->setIcon( 0, m_unconfirm_pixmap );
			}
		}
		else
			pItem->setIcon( 0, m_null_pixmap );

// 		update() ;
// 		repaintItem( (QTreeWidgetItem*)pItem );			//modify by chenkai
	}
}

void Event_table::get_event_color( Rsys_event *eve, QColor &clr )
{
	uint type =  m_cfg->get_color_type();

	switch( type )
	{
	case EVENTSORTCOLOR:
		{
			if( !m_cfg->get_sort_color( eve->sort, clr ) )
			{
				clr = Qt::black;
			}
		}
		break;
	case EVENTTYPECOLOR:
		{
			if( !m_cfg->get_type_color( eve->type, clr ) )
			{
				clr = Qt::black;
			}
		}
		break;
	case EVENTRTUCOLOR:
		{
			if( eve->sort == ENTCLASS_SCADA && 
				!m_cfg->get_rtu_color( tr( eve->Object ), clr ) )
			{
				clr = Qt::black;
			}
		}
		break;
	case EVENTLEVELCOLOR:
		{
			uint level;
		#ifdef EventLevelL
			level = eve->EventLevelL + 2 * eve->EventLevelM + 4 * eve->EventLevelH;
		#else
			level = eve->testflag;
		#endif
			m_cfg->get_level_color( level, clr );
		}
		break;
	default:
		clr = Qt::black;
	}
}

void Event_table::clear()
{
	TRY_LOCK;

	m_event_list.clear();
	m_event_to_item_map.clear();
	QTreeWidget::clear();

	m_event_num = 0;
	
}

void Event_table::closeEvent( QCloseEvent * e )
{
// 	if( testAttribute( Qt::Window ) )
// 	{
// 		hide();
// 		return;
// 	}
	Qt::WindowFlags f;
	f = windowFlags();
	if( f == Qt::Window )
	{
		hide();
		return;
	}

	QTreeWidget::closeEvent( e );
}

// void QEventTable::onPopMenu( int id )
// {
// // 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)QGetMainWnd();
// 	QSysEventList	eventlist;
// 
// 	switch( id )
// 	{
// 	case CONFIRM_MENU:
// 		{
// 			if( !pMainWnd->m_curUser.entconfirm )
// 			{
// // 				QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ),QString::null, this );
// 				QMessageBox::information( this ,tr( "提示" ), tr( "当前用户无事项确认权限!" ) );
// 				return;
// 			}
// 			getSelectedEvent( eventlist );
// 
// 			pMainWnd->sendMsg( eventlist );
// 			pMainWnd->confirmEvent( eventlist );
// 			
// 		}
// 		break;
// 	case CONFIRMALL_MENU:
// 		{
// 			if( !pMainWnd->m_curUser.entconfirm )
// 			{
// // 				QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ),QString::null, this );
// 				QMessageBox::information( this ,tr( "提示" ), tr( "当前用户无事项确认权限!" ) );
// 				return;
// 			}
// 			eventlist = CollectUnConfirmed(m_eventList);
// 
// 			pMainWnd->sendMsg( eventlist );
// 			pMainWnd->confirmEvent( eventlist );
// 		}
// 		break;
// 	case DELETE_MENU:
// 		{
// 			if( !pMainWnd->m_curUser.modifydbpara )
// 			{
// // 				QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无删除事项权限!" ),QString::null, this );
// 				QMessageBox::information( this ,tr( "提示" ), tr( "当前用户无删除事项权限!" ) );
// 				return;
// 			}
// 			getSelectedEvent( eventlist );
// 			pMainWnd->deleteEvent( eventlist );
// 		}
// 		break;
// 	case CURVE_MENU:
// 		{
// 			getSelectedEvent( eventlist );
// 			if( eventlist.count() == 0 )
// 				return;
// 
// 			Rsys_event *pEvent = eventlist.at( 0 );
// 
// 			if( pEvent->type == ENTTYPE_POWERSYS_OVERLIMI )
// 			{
// 				if( !pMainWnd->m_curUser.viewhiscurve )
// 				{
// // 					QMessageBox::message( tr( "提示" ), tr( "当前用户无浏览历史曲线权限!" ),QString::null, this );
// 					QMessageBox::information( this ,tr( "提示" ), tr( "当前用户无浏览历史曲线权限!" ) );
// 					return;
// 				}
// 
// 				ulong lYmd = (ulong)(pEvent->ymd);
// 				int nYear=(int)(lYmd/(100l*100l));
// 				int nMonth=(int)((lYmd%(100l*100l))/100);
// 				int nDay=(int)((lYmd%(100l*100l))%100);
// 
// 				char strdate[ 256 ];
// 				sprintf( strdate, "%04d%02d%02d", nYear,nMonth,nDay );
// 				QString strfilename = /*tr( "hiscurve ") +*/ tr( "-U" ) + 
// 					tr(  pMainWnd->m_curUser.username ) + tr( " -P" ) +
// 					tr( pMainWnd->m_curOperMan.password ) + tr( " -R" ) + 
// 					tr( pEvent->member0 ) + tr( " -D" ) + tr( strdate );
// // 				QStringList processArgs = QStringList::split(tr(" "),strfilename);
// 				QStringList processArgs = strfilename.split( tr(" ") );
// 				
// 				printf("%s\n",_Q2C(strfilename));
// 
// // 				QProcess proc(processArgs);			//modify by chenkai 
// // 				proc.start();
// 				QProcess proc(this);
// 				proc.start("hiscurve",processArgs);
// 			}
// 		}
// 		break;
// 	case GRAPH_MENU:
// 		{
// 			if( !pMainWnd->m_curUser.viewgraph )
// 			{
// // 				QMessageBox::message( tr( "提示" ), tr( "当前用户无浏览图形权限!" ),QString::null, this );
// 				QMessageBox::information( this ,tr( "提示" ), tr( "当前用户无浏览图形权限!" ) );
// 				return;
// 			}
// 			getSelectedEvent( eventlist );
// 
// 			if( eventlist.count() == 0 )
// 				return;
// 
// 			Rsys_event *pEvent = eventlist.at( 0 );
// 
// 			if( strlen( pEvent->graph ) > 0 )
// 			{
// 				QString strfilename = /*tr( "gmmi ") +*/ tr( "$(NBENV)/graph/" ) + tr( pEvent->graph );
// // 				QStringList processArgs = QStringList::split(_C2Q(" "),strfilename);
// 				QStringList processArgs = strfilename.split( tr(" ") );
// 				
// 				
// // 				QProcess proc(processArgs);				//modify by chenkai
// // 				proc.start();
// 				QProcess proc(this);
// 				proc.start("gmmi",processArgs);
// 			}
// 		}
// 	}
// 
// }

void Event_table::right_clicked( QTreeWidgetItem *, int )
{
	if (qApp->mouseButtons() == Qt::LeftButton)
	{ 
		return; 
	}
	if (qApp->mouseButtons() == Qt::RightButton)
	{ 
		m_right_pop_menu->popup( QCursor::pos() );
	}
}

void Event_table::slot_current_page_changed(QTreeWidgetItem* ,int )
{
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	pMainWnd->m_current_table_index = m_tabel_index;

}

void Event_table::slot_pop_menu( QAction *popAction )
{
	// 	QEventRealWnd *pMainWnd = (QEventRealWnd *)qApp->mainWidget();
	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Sys_event_list	eventlist;

	if( popAction->text() == tr("事项确认"))
	{
		if( !pMainWnd->m_cur_user.entconfirm )
		{
			// 				QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ),QString::null, this );
			DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无事项确认权限!" ) );
			return;
		}
		get_selected_event( eventlist );

		pMainWnd->send_msg( eventlist );
		pMainWnd->confirm_event( eventlist );
	}
	else if( popAction->text() == tr("确认所有"))
	{
		if( !pMainWnd->m_cur_user.entconfirm )
		{
			// 				QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ),QString::null, this );
			DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无事项确认权限!" ) );
			return;
		}
		eventlist = collect_unconfirmed(m_event_list);

		pMainWnd->send_msg( eventlist );
		pMainWnd->confirm_event( eventlist );
	}
	else if( popAction->text() == tr("事项删除"))
	{
		if( !pMainWnd->m_cur_user.modifydbpara )
		{
			// 				QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无删除事项权限!" ),QString::null, this );
			DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无删除事项权限!" ) );
			return;
		}
		get_selected_event( eventlist );
		pMainWnd->delete_event( eventlist );
	}
	else if( popAction->text() == tr("曲线查看"))
	{
		get_selected_event( eventlist );
		if( eventlist.count() == 0 )
			return;

		Rsys_event *pEvent = eventlist.at( 0 );

		if( pEvent->type == ENTTYPE_POWERSYS_OVERLIMI )
		{
			if( !pMainWnd->m_cur_user.viewhiscurve )
			{
				// 					QMessageBox::message( tr( "提示" ), tr( "当前用户无浏览历史曲线权限!" ),QString::null, this );
				DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无浏览历史曲线权限!" ) );
				return;
			}

			ulong lYmd = (ulong)(pEvent->ymd);
			int nYear=(int)(lYmd/(100l*100l));
			int nMonth=(int)((lYmd%(100l*100l))/100);
			int nDay=(int)((lYmd%(100l*100l))%100);

			char strdate[ 256 ];
			sprintf( strdate, "%04d%02d%02d", nYear,nMonth,nDay );
			QString strfilename = /*tr( "hiscurve ") +*/ tr( "-U" ) + 
				tr(  pMainWnd->m_cur_user.username ) + tr( " -P" ) +
				tr( pMainWnd->m_cur_oper_man.password ) + tr( " -R" ) + 
				tr( pEvent->member0 ) + tr( " -D" ) + tr( strdate );
			// 				QStringList processArgs = QStringList::split(tr(" "),strfilename);
			QStringList processArgs = strfilename.split( tr(" ") );

			printf("%s\n",_Q2C(strfilename));

			// 				QProcess proc(processArgs);			//modify by chenkai 
			// 				proc.start();
			QProcess proc(this);
			proc.startDetached("hiscurve",processArgs);
		}
	}
	else if( popAction->text() == tr("接 线 图"))
	{
		if( !pMainWnd->m_cur_user.viewgraph )
		{
			// 				QMessageBox::message( tr( "提示" ), tr( "当前用户无浏览图形权限!" ),QString::null, this );
			DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无浏览图形权限!" ) );
			return;
		}
		get_selected_event( eventlist );

		if( eventlist.count() == 0 )
			return;

		Rsys_event *pEvent = eventlist.at( 0 );

		if( strlen( pEvent->graph ) > 0 )
		{
			QString strfilename = /*tr( "gmmi ") +*/ tr( "$(NBENV)/graph/" ) + tr( pEvent->graph );
			// 				QStringList processArgs = QStringList::split(_C2Q(" "),strfilename);
			QStringList processArgs = strfilename.split( tr(" ") );


			// 				QProcess proc(processArgs);				//modify by chenkai
			// 				proc.start();
			QProcess proc(this);
			proc.startDetached("hmi",processArgs);
		}
	}
}

void Event_table::set_page_name( QString name )
{
	m_page_name = name;

	setWindowTitle( m_page_name );
}

QString Event_table::get_page_name()
{
	return m_page_name;
}

void Event_table::set_page_index( int index )
{
	m_tabel_index = index;
}

int Event_table::get_page_index()
{
	return m_tabel_index;
}

// void QEventTable::onClickColumn(int i)
/*
{
	if ( i == 0)
	{
		if ( topLevelItemCount() > 1)
		{	
			if ( topLevelItem(0)->text(0).toInt() >= topLevelItem(1)->text(0).toInt())
			{
				sortItems( 0 ,Qt::AscendingOrder );
			}
			else
				sortItems( 0 ,Qt::DescendingOrder );
		}
	}
}*/

