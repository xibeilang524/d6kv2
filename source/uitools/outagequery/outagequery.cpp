#include "outagequery.h"

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include "rdb/rdbdef.h"
#include "rdb/rdbapi.h"

#include "scd/devdef.h"
#include "uitools/dmessagebox.h"

#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

QPowerCutQuery::QPowerCutQuery( QWidget* parent ) : QMainWindow( parent )
{
	setupUi(this);
	m_powerCutTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_powerCutTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_powerCutTable->isSortingEnabled();

	QLabel *Label_Start = new QLabel( _C2Q( "起始时间：" ));
	m_beginDateTime = new QDateTimeEdit( timetoolbar );
	m_beginDateTime->setCalendarPopup(true);
	QLabel *Label_End = new QLabel( _C2Q( "终止时间：" ) );
	m_endDateTime = new QDateTimeEdit( timetoolbar );
	m_endDateTime->setCalendarPopup(true);
	timetoolbar->addWidget (Label_Start);
	timetoolbar->addWidget (m_beginDateTime);
	timetoolbar->addSeparator();
	timetoolbar->addWidget (Label_End);
	timetoolbar->addWidget (m_endDateTime);

	m_queryBtn = new QToolButton;
	m_queryBtn->setText( _C2Q( "查询" ) );
	m_queryBtn->setToolButtonStyle( Qt::ToolButtonTextOnly );
	timetoolbar->addWidget (m_queryBtn);

	QDate date;
	QTime time;
	time.setHMS( 0, 0, 0, 0 );
	date = QDate::currentDate();
	m_beginDateTime->setDateTime( QDateTime( date, time ) );
	m_endDateTime->setDateTime( QDateTime( date.addDays( 1 ), time ).addSecs( -1 ) );


	if( !m_dbPara.readPara() )
	{
		DMessageBox::warning( this,_C2Q("警告"), _C2Q( "读数据库失败!" ) );
		close();
	}

	iniStPage();
	iniAreaPage();
	iniFeederPage();

	connect( m_stQueryBtn ,	SIGNAL( clicked() ) , this , SLOT( onStQueryBtn() ) );
	connect( m_AreaQueryBtn ,	SIGNAL( clicked() ) , this , SLOT( onAreaQueryBtn() ) );
	connect( m_feederQueryBtn ,SIGNAL( clicked() ) , this , SLOT( onFeederQueryBtn() ) );
	connect( m_allStBtn ,	SIGNAL( clicked() ) , this , SLOT( onAllStBtn() ) );
	connect( m_allAreaBtn ,	SIGNAL( clicked() ) , this , SLOT( onAllAreaBtn() ) );
	connect( m_allfeederBtn ,	SIGNAL( clicked() ) , this , SLOT( onAllFeederBtn() ) );
	connect( action_read ,	 SIGNAL(triggered()) , this , SLOT(onRead()));
	connect( action_Exit ,	 SIGNAL(triggered()) , this , SLOT(onExit()));
	connect( m_queryBtn ,	 SIGNAL(clicked()) , this , SLOT(onQueryBtnClicked()));

	m_stTreeWidget->resizeColumnToContents(0);
	m_AreaTreeWidget->resizeColumnToContents(0);
	m_feederTreeWidget->resizeColumnToContents(0);
}

void QPowerCutQuery::iniAreaPage()
{
	QTreeWidgetItem *pItem ;
	uint i, j, groupno;
	QString code,desc;
	uint areaCount = m_dbPara.m_areaName2descMap.count();
	m_AreaTreeWidget->clear();
	for( i = 0; i < m_dbPara.getGroupCount(); i ++ )
	{
		if( !m_dbPara.getGroupNo( i, groupno ) )
			continue;

		for( j = 0; j < areaCount; j ++ )
		{
			code = m_dbPara.m_areaName2descMap.keys().at(j);
			desc = m_dbPara.m_areaName2descMap.values().at(j);
			QStringList desc_area;
			desc_area << desc ;
			pItem = new QTreeWidgetItem( m_AreaTreeWidget, desc_area);
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_item2AreaCodeMap[ pItem ] = code;
		}
	}
}

void QPowerCutQuery::iniStPage()
{
	QTreeWidgetItem *pItem ;
	uint i, j;
	uint groupno;
	QString desc;
	QStringList rtucodes;
	m_stTreeWidget->clear();
	for( i = 0; i < m_dbPara.getGroupCount(); i ++ )
	{
		if( !m_dbPara.getGroupNo( i, groupno ) )
			continue;

		if( !m_dbPara.getStaCode( groupno, rtucodes ) )
			continue;

		for( j = 0; j < rtucodes.count(); j ++ )
		{
			if( !m_dbPara.getStaDesc( rtucodes[ j ], desc ) )
				continue;
			QStringList desc_rtu;
			desc_rtu << desc ;
			pItem = new QTreeWidgetItem( m_stTreeWidget, desc_rtu);
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_item2StCodeMap[ pItem ] = rtucodes[ j ];
		}
	}
}

void QPowerCutQuery::iniFeederPage()
{
	QTreeWidgetItem *pItem ;
	uint i, j;
	uint groupno;
	QString desc;
	uint feederLineCnt = m_dbPara.m_feederLinePara.count();
	m_feederTreeWidget->clear();
	for( i = 0; i < m_dbPara.getGroupCount(); i ++ )
	{
		if( !m_dbPara.getGroupNo( i, groupno ) )
			continue;

		for( j = 0; j < feederLineCnt; j ++ )
		{
			desc = m_dbPara.m_feederLinePara[ j ]->desc ;
			QStringList desc_feeder;
			desc_feeder << desc ;
			pItem = new QTreeWidgetItem( m_feederTreeWidget, desc_feeder);
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_item2FeederLineMap[ pItem ] = m_dbPara.m_feederLinePara[ j ]->name;
		}
	}
}

bool QPowerCutQuery::timeChecked( uint time)
{
	RDB_DATETIME_MS_STRU    curclock;
	Rdb_Time       rdbtime;

	if( m_beginDateTime->dateTime() > m_endDateTime->dateTime() )
		return false;
	m_beginTime.year = m_beginDateTime->dateTime().date().year();
	m_beginTime.mon = m_beginDateTime->dateTime().date().month();
	m_beginTime.day = m_beginDateTime->dateTime().date().day();
	m_beginTime.hour = m_beginDateTime->dateTime().time().hour();
	m_beginTime.min = m_beginDateTime->dateTime().time().minute();
	m_beginTime.sec = m_beginDateTime->dateTime().time().second();
	m_beginTime.ms = 0;

	m_endTime.year = m_endDateTime->dateTime().date().year();
	m_endTime.mon = m_endDateTime->dateTime().date().month();
	m_endTime.day = m_endDateTime->dateTime().date().day();
	m_endTime.hour = m_endDateTime->dateTime().time().hour();
	m_endTime.min = m_endDateTime->dateTime().time().minute();
	m_endTime.sec = m_endDateTime->dateTime().time().second();
	m_endTime.ms = 999;

	ulong beginYmd , beginHmsms, endYmd , endHmsms, lYmd, lHmsms;


	beginYmd = m_beginTime.year * 10000l + m_beginTime.mon * 100l + m_beginTime.day;
	beginHmsms = m_beginTime.hour * 10000l + m_beginTime.min * 100l +m_beginTime.sec;

	endYmd = m_endTime.year * 10000l + m_endTime.mon * 100l + m_endTime.day;
	endHmsms = m_endTime.hour * 10000l + m_endTime.min * 100l +m_endTime.sec;
	
	rdbtime.RdbConvertTime(time,&curclock);
	lYmd = (ulong)curclock.year*10000 + (ulong)curclock.month * 100 + (ulong)curclock.day;
	lHmsms = (ulong)curclock.hour* 10000 + (ulong)curclock.minute * 100+ (ulong)curclock.second;
	if( lYmd < beginYmd || ( lYmd == beginYmd && lHmsms < beginHmsms ) ||
		endYmd < lYmd || ( endYmd == lYmd && endHmsms < lHmsms ) )
	{
		return false;
	}
	else
		return true;
}

void QPowerCutQuery::onStQueryBtn()
{
	uint i;

	QTreeWidgetItem *pItem ;
	m_stCodeList.clear();
	for ( i =0 ; i< m_stTreeWidget->topLevelItemCount() ; i++ )
	{
		pItem = m_stTreeWidget->topLevelItem(i) ;
		if( pItem->checkState(0) == Qt::Unchecked )
			continue;
		m_stCodeList.append( m_item2StCodeMap[ pItem ] );
	}
	findRecordByStation();
	fillTable();
}

void QPowerCutQuery::onAreaQueryBtn()
{
	uint i;

	QTreeWidgetItem *pItem ;
	m_AreaCodeList.clear();

	for ( i =0 ; i< m_AreaTreeWidget->topLevelItemCount() ; i++ )
	{
		pItem = m_AreaTreeWidget->topLevelItem(i) ;
		if( pItem->checkState(0) == Qt::Unchecked )
			continue;
		m_AreaCodeList.append( m_item2AreaCodeMap[ pItem ] );
	}
	findRecordByArea() ;
	fillTable();
}

void QPowerCutQuery::onFeederQueryBtn()
{
	uint i;

	QTreeWidgetItem *pItem ;
	m_feederNameList.clear();

	for ( i =0 ; i< m_feederTreeWidget->topLevelItemCount() ; i++ )
	{
		pItem = m_feederTreeWidget->topLevelItem(i) ;
		if( pItem->checkState(0) == Qt::Unchecked )
			continue;
		m_feederNameList.append( m_item2FeederLineMap[ pItem ] );
	}

	findRecordByFeederLine() ;
	fillTable();
}

void QPowerCutQuery::fillTable()
{
	PPOWERCUTPARA pPowerCutPara;
	RDB_DATETIME_MS_STRU    curclock;
	Rdb_Time       rdbtime;

	uint i;
	uint powercutCount = m_powerCutParaList.count();
	m_powerCutTable->clearContents();
	m_powerCutTable->setRowCount(0);
	m_powerCutTable->setRowCount( powercutCount );
	QColor mcolor;
// 	mcolor.setRgb(252,222,156);
	mcolor.setRgb(224,238,225);
	QPalette palt= m_powerCutTable->palette();
	palt.setColor(QPalette::AlternateBase,mcolor);
	m_powerCutTable->setPalette(palt);
	m_powerCutTable->setAlternatingRowColors(true);	//隔行变色

	readUserTypeFile();

	PUSERINFOPARA pUserInfo;
	for ( i = 0; i < powercutCount; i++)
	{
		pPowerCutPara = m_powerCutParaList.at(i);
		pUserInfo = m_dbPara.m_belongsLoad2userInfoMap[pPowerCutPara->code];
		if(!pUserInfo)
			continue;
		m_powerCutTable->setItem(i, 0, new QTableWidgetItem(pUserInfo->desc));

		QString typeDesc ;
			typeDesc = m_userType2userDescMap[pUserInfo->type];
		m_powerCutTable->setItem(i, 1, new QTableWidgetItem(typeDesc));

		QString stationDesc;
		if (!m_dbPara.getStaDesc(pPowerCutPara->stationcode, stationDesc))
			continue;
		m_powerCutTable->setItem(i, 2, new QTableWidgetItem(stationDesc));

		QString mdate;
		if ( pPowerCutPara->starttime == 0)
		{
			mdate = QString::number(pPowerCutPara->starttime);
		}
		else
		{		
			rdbtime.RdbConvertTime(pPowerCutPara->starttime,&curclock);
			mdate.sprintf("%d/%d/%d %d:%d:%d",curclock.year,curclock.month,curclock.day,curclock.hour,curclock.minute,curclock.second);
		}
		m_powerCutTable->setItem(i, 3, new QTableWidgetItem(mdate));

		if ( pPowerCutPara->endtime == 0)
		{
			mdate = QString::number(pPowerCutPara->endtime);
			m_powerCutTable->setItem(i, 4, new QTableWidgetItem(_C2Q("停电未结束")));
		}
		else
		{
			rdbtime.RdbConvertTime(pPowerCutPara->endtime,&curclock);
			mdate.sprintf("%d/%d/%d %d:%d:%d",curclock.year,curclock.month,curclock.day,curclock.hour,curclock.minute,curclock.second);
			m_powerCutTable->setItem(i, 4, new QTableWidgetItem(mdate));
		}

		//结束时间为0 如何处理
		if ( pPowerCutPara->endtime == 0 )
		{
			pPowerCutPara->endtime = QDateTime::currentDateTime().toTime_t();
		}
		int mytime = pPowerCutPara->endtime - pPowerCutPara->starttime ;
		int myday = 0,myhour = 0,myminute = 0, mysecond  = 0;
		QString strday,strhour, strmin, strsec,strtext;
		myday = mytime/(24*60*60);
		myhour = (mytime - myday*24*60*60)/(60*60);
		myminute = (mytime - myday*24*60*60 - myhour*60*60)/60;
		mysecond = mytime - myday*24*60*60 - myhour*60*60 - myminute*60;
		strday = QString::number(myday) + _C2Q("天");
		if (myhour < 10 )
		{
			strhour = _C2Q("0") + QString::number(myhour) + _C2Q("时");
		}
		else strhour = QString::number(myhour) + _C2Q("时");

		if ( myminute < 10 )
		{
			strmin = _C2Q("0") + QString::number(myminute) + _C2Q("分");
		}
		else strmin = QString::number(myminute) + _C2Q("分");

		if ( mysecond < 10)
		{
			strsec = _C2Q("0") + QString::number(mysecond) + _C2Q("秒");
		}
		else strsec = QString::number(mysecond) + _C2Q("秒");

		strtext = strday + strhour + strmin + strsec;

		QString strCutTime = QString::number(mytime);
		m_powerCutTable->setItem(i, 5, new QTableWidgetItem(strtext));

		QString breakDesc;
		if(!m_dbPara.getDevDesc(pPowerCutPara->breakname, breakDesc))
			continue;
		m_powerCutTable->setItem(i, 6, new QTableWidgetItem(breakDesc));

		QString cutTypeDesc;
		if(!getCutTypeDesc(pPowerCutPara->cutType, pPowerCutPara->breakname, pPowerCutPara->starttime, cutTypeDesc))
			continue;
		m_powerCutTable->setItem(i, 7, new QTableWidgetItem(cutTypeDesc));

		QString feederLineDesc;
		if (!m_dbPara.getFeederLineDesc(pPowerCutPara->feeder, feederLineDesc))
			continue;
		m_powerCutTable->setItem(i, 8, new QTableWidgetItem(feederLineDesc));

		QString areaDesc = m_dbPara.m_areaName2descMap[pUserInfo->belongsArea];
		m_powerCutTable->setItem(i, 9, new QTableWidgetItem(areaDesc));

		m_powerCutTable->setItem(i, 10, new QTableWidgetItem(QString::number(pPowerCutPara->loadValue)));


	}
	m_powerCutTable->resizeColumnsToContents();
}
void QPowerCutQuery::onAllStBtn()
{
	QTreeWidgetItem *pItem;
	uint i;
	if( m_allStBtn->isChecked() )
	{	
		for( i = 0 ; i < m_stTreeWidget->topLevelItemCount() ; i++ )
		{
			pItem = m_stTreeWidget->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Checked );
		}
	}
	else
	{
		for( i = 0 ; i < m_stTreeWidget->topLevelItemCount() ; i++ )
		{
			pItem = m_stTreeWidget->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Unchecked );
		}
	}	
}

void QPowerCutQuery::onAllAreaBtn()
{
	QTreeWidgetItem *pItem;
	uint i;
	if( m_allAreaBtn->isChecked() )
	{	
		for( i = 0 ; i < m_AreaTreeWidget->topLevelItemCount() ; i++ )
		{
			pItem = m_AreaTreeWidget->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Checked );
		}
	}
	else
	{
		for( i = 0 ; i < m_AreaTreeWidget->topLevelItemCount() ; i++ )
		{
			pItem = m_AreaTreeWidget->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Unchecked );
		}
	}	
}
void QPowerCutQuery::onAllFeederBtn()
{
	QTreeWidgetItem *pItem;
	uint i;
	if( m_allfeederBtn->isChecked() )
	{	
		for( i = 0 ; i < m_feederTreeWidget->topLevelItemCount() ; i++ )
		{
			pItem = m_feederTreeWidget->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Checked );
		}
	}
	else
	{
		for( i = 0 ; i < m_feederTreeWidget->topLevelItemCount() ; i++ )
		{
			pItem = m_feederTreeWidget->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Unchecked );
		}
	}	
}

void QPowerCutQuery::onRead()
{
	action_read->setChecked(false);
	if( !m_dbPara.readPara() )
	{
		DMessageBox::warning( this,_C2Q("警告"), _C2Q( "读数据库失败!" ) );
		close();
	}
	iniAreaPage();
	iniStPage();
	iniFeederPage();
}

void QPowerCutQuery::onExit()
{
	qApp->quit();
}

void QPowerCutQuery::onQueryBtnClicked()
{
	if(m_beginDateTime->dateTime() > m_endDateTime->dateTime()) 
	{
		DMessageBox::information(this, _C2Q( "错误" ), _C2Q( "结束时间早于起始时间!" ));
		return;
	}
	
	QDateTime mytime = QDateTime::currentDateTime();
	if( mytime.date().month() < m_endDateTime->dateTime().date().month() - 1)
	{
		DMessageBox::information(this, _C2Q( "错误" ), _C2Q( "结束时间最远到当前月份!" ));
		return;
	}
	int curTab = m_tabWidget->currentIndex();
	switch( curTab )
	{
	case 0:
		{
			onStQueryBtn();
		}
		break;

	case 1:
		{
			onAreaQueryBtn();
		}
		break;

	case 2:
		{
			onFeederQueryBtn();
		}
		break;
	}
}

void QPowerCutQuery::findRecordByArea()
{
	uint i ;
	m_powerCutParaList.clear();
	if (!readPowerCutPara())
		return;
	if(m_AreaCodeList.count() <= 0)
		return;
	PPOWERCUTPARA pPowerCut;
	PUSERINFOPARA pUserInfo;
	for (i = 0; i < m_initPowerCutParaList.count(); i++)
	{
		pPowerCut = m_initPowerCutParaList.at(i);
		pUserInfo = m_dbPara.m_belongsLoad2userInfoMap[pPowerCut->code];
		if(!pUserInfo)
			continue;
		QString areaCode = pUserInfo->belongsArea;
		if(m_AreaCodeList.contains(areaCode))
		{
			m_powerCutParaList.append(pPowerCut);
		}
	}
}

void QPowerCutQuery::findRecordByStation()
{
	uint i ;
	m_powerCutParaList.clear();
	if (!readPowerCutPara())
		return;
	if(m_stCodeList.count() <= 0)
		return;
	PPOWERCUTPARA pPowerCut;
	PUSERINFOPARA pUserInfo;
	for (i = 0; i < m_initPowerCutParaList.count(); i++)
	{
		pPowerCut = m_initPowerCutParaList.at(i);
		if(m_stCodeList.contains(pPowerCut->stationcode))
		{
			pUserInfo = m_dbPara.m_belongsLoad2userInfoMap[pPowerCut->code];
			if(!pUserInfo)
				continue;
			m_powerCutParaList.append(pPowerCut);
		}
	}		
}

void QPowerCutQuery::findRecordByFeederLine()
{
	uint i ;
	m_powerCutParaList.clear();
	if (!readPowerCutPara())
		return;
	if(m_feederNameList.count() <= 0)
		return;
	PPOWERCUTPARA pPowerCut;
	PFEEDERLINEPARA pFeederLine;
	PUSERINFOPARA pUserInfo;
	for (i = 0; i < m_initPowerCutParaList.count(); i++)
	{
		pPowerCut = m_initPowerCutParaList.at(i);
		pFeederLine = m_dbPara.m_feederLineNo2FeederLineMap[pPowerCut->feeder];
		if(!pFeederLine) 
			continue;
		if(m_feederNameList.contains(pFeederLine->name))
		{
			pUserInfo = m_dbPara.m_belongsLoad2userInfoMap[pPowerCut->code];
			if(!pUserInfo)
				continue;
			m_powerCutParaList.append(pPowerCut);
		}
	}
}

bool QPowerCutQuery::readPowerCutPara()
{
	qDeleteAll( m_initPowerCutParaList ) ;
	m_initPowerCutParaList.clear();

	int i,j;
	QDate date = m_beginDateTime->dateTime().date();
	int year = date.year();
	int month = date.month();
	char tableName[DESC_LEN];
	sprintf(tableName,"停电管理事项表%.4d%.2d",year,month);
	DBApi	db;
	if( db.Open( DB_OP_R, FALSE, FALSE ) < 0 )
	{
		DMessageBox::information( this, _C2Q( "告警" ), _C2Q( "数据库打开失败!" ) );
		return FALSE ;
	}

	CS_RETCODE      retcode = 0; 
	FUNC_STR			func;
	CS_DATAFMT*		datafmt=NULL;
	int							ptcnt = 0;

	printf( "停电管理事项表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,tableName);
	strcpy(func.dbname,"modeldb");
	char * col_name[] = {"代码","设备类型","站所线系代码","所属馈线号","停电开始时间","停电结束时间","事故开关","停电类型","负荷容量"};

	datafmt = 0;
	void *pt=0;

	if(db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		datafmt = NULL;
		db.Close();	
		return FALSE;
	}

	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		datafmt = NULL;
		db.Close();	
		return FALSE;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for( i = 0 ; i < func.colnum ; i++ )
	{
		func.col[i].select = NO;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=NO;
		for( j = 0 ; j < col_num ;  j++ )
		{
			if(strcmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = YES;
			func.col[i].order_d = j;
		}
		if ( strcasecmp( datafmt[i].name,"停电开始时间") == 0 )
		{
			func.col[i].order = 0 ;
		}
	}
	uint begintime = m_beginDateTime->dateTime().toTime_t() ;
	uint endtime = m_endDateTime->dateTime().toTime_t() ;

	char where[2048];
	char tmpstr[1024];
	sprintf( where , "( 停电开始时间>= '%d' and 停电开始时间<='%d' and " , begintime , endtime ) ;
	sprintf( tmpstr, "(设备类型 = '%d' ",  DEVTYPE_LOAD );
	strcat( where, tmpstr );
	strcat( where, ") )" );

	func.func = SEL_ISQL_RECORDERS;
	strcpy( func.isql , where ) ;

	if( CS_SUCCEED != db.SelectRcds( &func , datafmt , &pt ) )  
	{
		if(datafmt)	free(datafmt);
		datafmt = NULL;
		if(pt)	free(pt);
		pt = NULL;
		db.Close();	
		return FALSE;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		datafmt = NULL;
		if(pt)	free(pt);
		pt = NULL;
		db.Close();	
		return FALSE;
	}

	int count;
	char tmpbuf[ 1024 ];

	POWERCUTPARA* powerCutPara ;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		powerCutPara = new POWERCUTPARA;
		db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		powerCutPara->code = _C2Q( tmpbuf );
		powerCutPara->type = (uint)db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		powerCutPara->stationcode = _C2Q( tmpbuf );
		powerCutPara->feeder = (uint)db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		powerCutPara->starttime = (uint)db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		powerCutPara->endtime = (uint)db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		powerCutPara->breakname = _C2Q( tmpbuf );
		powerCutPara->cutType = (uint)db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		powerCutPara->loadValue = (float)db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);

		m_initPowerCutParaList.append( powerCutPara );
	}
	if (pt) free(pt);
	if (datafmt) free(datafmt);

	db.Close();
	return TRUE;
}
bool QPowerCutQuery::getCutTypeDesc(uint &cutType, QString &breakName, uint &beginTime, QString &cutTypeDesc)
{
	bool ret = false;
	PPLANCUTPARA pPlanCut;
	if (cutType == 1)
	{
		cutTypeDesc = _C2Q("故障停电");
		ret = true;
	}
	else
	{
		RDB_DATETIME_MS_STRU    curclock;
		Rdb_Time       rdbtime;

		for (int i = 0; i < m_dbPara.m_planCutPara.count(); i++)
		{
			rdbtime.RdbConvertTime(beginTime,&curclock);
			int beginYmd = curclock.year * 10000 + curclock.month * 100 + curclock.day;
			int beginHms = curclock.hour * 10000 + curclock.minute * 100 + curclock.second;
			pPlanCut = m_dbPara.m_planCutPara.at(i);
			if( breakName != pPlanCut->code)
				continue;
			if (beginYmd >= pPlanCut->beginYmd && beginYmd <= pPlanCut->endYmd 
				&& beginHms >= pPlanCut->beginHms && beginHms <= pPlanCut->endHms)
			{
				cutTypeDesc = _C2Q("计划停电");
				return true;
			}				
		}
		cutTypeDesc = _C2Q("临时停电");
		ret = true;
	}
	return ret;
}

bool QPowerCutQuery::readUserTypeFile()
{
	m_userType2userDescMap.clear();
	bool ret = false;
	char fileName[256];
	sprintf(fileName,"%s/dboper/停电客户类型.txt",getenv("NBENV"));
	QFile file;
	file.setFileName(_C2Q(fileName));
	ret = file.open(QIODevice::ReadOnly);
	QTextStream textstream(&file);
	QString tmpText;
	tmpText = textstream.readLine();
	while (!tmpText.isNull())
	{
		QStringList textList;
		textList = tmpText.split("*");
		bool ok;
		uint type = textList[0].toInt(&ok, 10);
		m_userType2userDescMap[type] = textList[1];
		tmpText = textstream.readLine();
	}
	return ret;
}