#include "msgviewwnd.h"
//#include "vld.h"
#include <QtGui>
#include <QIcon>
#include <QTreeWidgetItem>
#include <QTreeWidget> 
#include <QString>
#include <QDebug>
#include <iostream>

#include <ace/Log_Msg.h>
#include <sys/stat.h>

using namespace std;
extern bool g_autoSave;
extern int g_saveDays;

QMsgViewWnd::QMsgViewWnd(QWidget *parent /* = NULL */)
: QSplitter(parent)
{
	m_pMsgFrame = new QMsgFrame(this);
	m_pTimer = NULL;
	//通道是否选中图标
	m_rootIcon.addPixmap(QPixmap(":res/unselect.png"), QIcon::Normal, QIcon::Off);
	m_rootIcon.addPixmap(QPixmap(":res/select.png"), QIcon::Selected, QIcon::Off);

	m_pChanTree = new QTreeWidget(this);
	m_pChanTree->setHeaderLabels(QStringList()<<_C2Q("通道序号")<<_C2Q("名称"));
	m_pChanTree->setColumnWidth( 0 , 125 ) ;
	connect(m_pChanTree, SIGNAL(itemSelectionChanged()), this, SLOT(onTreeItemChange()));
	
	m_Modula = new CNetModula();
	m_curModula = -1;
	m_channel_num = 0;
	list_all_channels();

	addWidget(m_pChanTree);  //左边树形框
	addWidget(m_pMsgFrame);  //右边解析框
	//QSplitter空间中QTreeWidget和QFrame占据大小的分配
	setStretchFactor(0, 0);
	setStretchFactor(1, 1);
	setGeometry(200,100,1000,500);
	setWindowTitle(_C2Q("通道报文查看"));
	
	//定时器
	m_pTimer = new QTimer();
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_pTimer->start(150);  //timeout时间为150毫秒


	////自动存储
	//m_autosave_checkbox_state = 0;  //初始默认不自动存储
	tcp_recv = NULL;

	ini_stylesheet() ;
	
}

//切换通道序号
void QMsgViewWnd::onTreeItemChange(void)
{
	//Returns a list of all selected non-hidden items.
	QList<QTreeWidgetItem*> list = m_pChanTree->selectedItems();
	QTreeWidgetItem* chan = list[0];
	if (chan->data(0,Qt::UserRole).toInt()<=0)
	{
		return;
	}
	bool ok = false;
	//QString QTreeWidgetItem::text ( int column ) const
	//Returns the text in the specified column.
	int chanNo = chan->text(0).toInt(&ok, 10);  //chanNo为选中的通道序号，从0开始

	if ( true == ok )
	{
		m_curModula = chanNo;
		//qDebug() << "chanNo = " << chanNo;
		m_Modula->ClearListData();
		m_Modula->SetName(chan->text(1));
		m_pMsgFrame->OnCurrentModulaChange(m_Modula);
	}

	if(request_fep_msg()<0)
	{
		ACE_ERROR((LM_ERROR, "请求通道，无值班机值班！\n"));
	}

}

QMsgViewWnd::~QMsgViewWnd()
{
	printf("\n delete QMsgViewWnd");
	delete m_pChanTree;
	m_pChanTree = NULL;
	delete m_pMsgFrame;
	m_pMsgFrame = NULL;
	delete m_Modula;
	m_Modula = NULL;
	delete m_pTimer;
	m_pTimer = NULL;
}

void QMsgViewWnd::onTimer()
{
	if (tcp_recv)
		getCurChanMsg();
}

void QMsgViewWnd::getCurChanMsg(void)		//新前置改为对所有前置轮询
{
	if (-1 == m_curModula || tcp_recv == NULL)  //chanNo
	{
		return;
	}

	FE_MSG *msg_buf;
	unsigned char buf[sizeof(FE_MSG)];
	memset(buf, 0, sizeof(FE_MSG));

	int ret = tcp_recv->read_dev(buf,sizeof(FE_MSG));
	if (ret <= 0)		//接收出错，或无报文
	{
		return;
	}

	msg_buf = (FE_MSG*)buf;
	if (msg_buf->channel_no != m_curModula || msg_buf->lenth == 0)
	{
		return;
	}
	int type = msg_buf->recv_flag ? 1 : 2;
	QString qprotocol = channel_protocol.value(msg_buf->channel_no);

	m_Modula->AddData((char*)msg_buf->msg, msg_buf->lenth, type, qprotocol,
		m_Modula->Get_autosave_begintime(), m_Modula->Get_autosave_endtime(),
		m_Modula->Get_autosave_checkbox_state(), m_Modula->GetName(), &(msg_buf->time));

}

void QMsgViewWnd::closeEvent(QCloseEvent *event)
{
	if (tcp_recv)
	{
		tcp_recv->close_dev();
		tcp_recv = NULL;
	}
	channel_protocol.clear();
	event->accept();
}

int QMsgViewWnd::read_stationareadata()
{
	int ret =-1;
	m_station_area.clear();

	STATIONAREAPARA * db_stationarea = NULL;
	Db_connector::open();
	int rcd_cnt = Db_connector::select("select * from STATIONAREA order by  id", (void **)&db_stationarea); 

    ret=rcd_cnt;

	if (ret > 0 )
	{
		for (int i = 0; i < ret; i++)
		{
			STATIONAREAPARA *tmp_db_stationarea = new STATIONAREAPARA;
			*tmp_db_stationarea = *(db_stationarea + i);
			m_station_area.insert(tmp_db_stationarea->id,tmp_db_stationarea);
		}
	}

	if (db_stationarea)
	{
		free(db_stationarea);
	}
	Db_connector::close();


	return ret > 0 ? 0 : -1;
}


int QMsgViewWnd::read_channeldata()
{
	int ret = -1;
	m_channelpara.clear();
	CHANNELPARA * db_channel = NULL;
	Db_connector::open();
	int rcd_cnt = Db_connector::select("select 通道序号,终端序号,id from subareachannelview order by  通道序号", (void **)&db_channel); 

	ret=rcd_cnt;

	if (ret > 0 )
	{
		for (int i = 0; i < ret; i++)
		{
			CHANNELPARA *tmp_db_station = new CHANNELPARA;
			*tmp_db_station = *(db_channel + i);
			m_channelpara.insert(tmp_db_station->channo,tmp_db_station);
		}
	}

	if (db_channel)
	{
		free(db_channel);
	}

	return ret > 0 ? 0 : -1;
}


void QMsgViewWnd::list_all_channels()
{
	CHANNEL_LIST* db_channel = NULL;
	Db_connector::open();
	int rcd_cnt = Db_connector::select("select 通道序号, 描述, 规约名, 使用标志 from 通道参数表 order by  通道序号 asc", (void **)&db_channel); 

// 	CHANPARA* p_chan_para = Fes_shm::get().chan_para_ptr;
// 	for (int i = 0; i < MAX_CHAN_NUM; i++)
// 	{
// 		CHANNEL_LIST* db_para = db_channel + i;
// 		int channel_no = db_para->channel_no; //获取通道号
// 	
// 		if ( !strcmp(db_para->describe, "") || !(db_para->valid))
// 		{
// // 			p_chan_para++;
// 			continue;
// 		}
// 
// 		QStringList colum;
// 		colum<<QString::number(channel_no)<<_C2Q(db_para->describe); //通道序号，通道名输入colum
// 		QTreeWidgetItem* tmp = new QTreeWidgetItem(colum);
// 		tmp->setIcon(0, m_rootIcon);  //设置通道序号前面图标
// 		m_pChanTree->addTopLevelItem(tmp);
// 		channel_protocol[channel_no] = _C2Q(db_para->protocol);
// // 		p_chan_para++;
// 		m_channel_num++;
// 	}
    bool dbflag =false;
	char path[256];
	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DDEFAULT_ENV),"log/dat/fesmsg.dat");

	if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "数据库中无通道参数记录！\n"));
		return;
	}
	else if (rcd_cnt < 0)	//黑库启动
	{
		ACE_DEBUG((LM_ERROR, "读取通道表参数失败！\n"));
		FILE* fp = ACE_OS::fopen(path, "rb");
		if (fp == NULL)
		{
			ACE_ERROR((LM_ERROR, "打开文件失败！\n"));
			return;
		}
		ACE_OS::fread(db_channel, sizeof(CHANNEL_LIST), MAX_CHAN_NUM, fp);
		ACE_OS::fclose(fp);

	}
	else
	{
		FILE* fp = ACE_OS::fopen(path, "w+b");
		if (fp == NULL)
		{
			ACE_ERROR((LM_ERROR, "打开文件失败！\n"));
		}
		else
		{
			ACE_OS::fwrite(db_channel, sizeof(CHANNEL_LIST), rcd_cnt, fp);
			ACE_OS::fclose(fp);
			ACE_DEBUG((LM_ERROR, "保存电度参数到本地文件成功！\n"));
		}
		dbflag =true;
	}
	if (dbflag)
	{
		if (read_stationareadata()<0||read_channeldata()<0)
		{
           dbflag=false;
		}
	}
	QTreeWidgetItem *pItem,*pSubItem;
	QMap<int ,QTreeWidgetItem*> m_groupItemMap ;
	if (dbflag==true)
	{
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

	}
	//将读出终端号的终端参数赋值给相应的终端参数结构体元素
	 for (int i = 0; i < rcd_cnt; i++)
	{
		CHANNEL_LIST* db_para = db_channel + i;
		int channel_no = db_channel[i].channel_no; //获取通道号

		if ( !strcmp(db_para->describe, "") || !(db_para->valid))
		{
			continue;
		}
        if (dbflag==true)
        {
			if (channel_no >= 0 && channel_no < MAX_CHAN_NUM)
			{
				QStringList colum;
				colum<<QString::number(channel_no)<<_C2Q(db_para->describe); //通道序号，通道名输入colum
				//QTreeWidgetItem* tmp = new QTreeWidgetItem(colum);

				short channelareain;
				channelareain  = m_channelpara[channel_no]->areaid;
                pSubItem= new QTreeWidgetItem(m_groupItemMap[channelareain] , colum );
				pSubItem->setIcon(0, m_rootIcon);  //设置通道序号前面图标
				pSubItem->setData(0,Qt::UserRole,1);
				//m_pChanTree->addTopLevelItem(tmp);
				channel_protocol[channel_no] = _C2Q(db_para->protocol);
				m_channel_num++;
			}
        }
		else
		{
		if (channel_no >= 0 && channel_no < MAX_CHAN_NUM)
		{
			QStringList colum;
			colum<<QString::number(channel_no)<<_C2Q(db_para->describe); //通道序号，通道名输入colum
			QTreeWidgetItem* tmp = new QTreeWidgetItem(colum);
			tmp->setIcon(0, m_rootIcon);  //设置通道序号前面图标
			tmp->setData(0,Qt::UserRole,1);
			m_pChanTree->addTopLevelItem(tmp);
			channel_protocol[channel_no] = _C2Q(db_para->protocol);

			m_channel_num++;
		}
		}
	}

	if (db_channel)
	{
		free(db_channel);
	}
	Db_connector::close();
}

int QMsgViewWnd::request_fep_msg()
{
	unsigned char buf[10];		//向报文服务请求通道号，地位在前，高位在后
	int nodes[MAX_FE_SRV_NUM];
	struct in_addr fep_addr[2];
	int ret = -1;
	
	buf[0] = m_curModula%256;
	buf[1] = m_curModula/256;


	int node_num = Net_stat::get_fe_nodes(nodes, MAX_FE_SRV_NUM);

	for (int i = 0; i< node_num; i++)
	{
		Net_stat::get_host_addr(nodes[i], fep_addr);
		if(Net_stat::get_host_stat(nodes[i]) != 0)	//若不在线,不进行轮询
		{
			continue;
		}
		if (tcp_recv)
		{
			tcp_recv->close_dev();
			delete tcp_recv;
			tcp_recv = NULL;
		}

// 		for(int num = 0; num < 2; num++)
// 		{
// 			if (strcasecmp("255.255.255.255", inet_ntoa(fep_addr[num])) == 0)
// 			{
//  				ACE_OS::memset(&fep_addr[num], 0, sizeof(in_addr));
// 			}
// 			printf("ip%d = %s \n",num, inet_ntoa(fep_addr[num]));
// 
// 		}
		char ip_addr0[DEV_NAME_LEN];
		ACE_OS::strcpy(ip_addr0, inet_ntoa(fep_addr[0]));
		printf("ip = %s \n",ip_addr0);
		char ip_addr1[DEV_NAME_LEN];
		ACE_OS::strcpy(ip_addr1, inet_ntoa(fep_addr[1]));
		printf("ip = %s \n",ip_addr1);
 		tcp_recv = new Tcp_client(ip_addr0, PORT_NO, ip_addr1, PORT_NO);
		
// 		tcp_recv = new Tcp_client(inet_ntoa(fep_addr[0]), PORT_NO, inet_ntoa(fep_addr[1]), PORT_NO);
		ret = tcp_recv->open_dev();
		if (ret < 0 || tcp_recv->get_stat() < 0)
		{
			tcp_recv->close_dev();
			delete tcp_recv;
			tcp_recv = NULL;
			continue;
		}
		tcp_recv->write_dev(buf, 2);
		while (tcp_recv->read_dev(buf,2) == 0)
		{
			continue;
		}
// 		if(buf[0] == 0xff && buf[1] == 0xff)
// 		{
// 			continue;
// 		}
		if(buf[0] == 0xcc &&  buf[1] == 0xcc)
		{
			ACE_DEBUG((LM_DEBUG, "请求主机host_no = %d！\n",nodes[i]));
			ret = 0;
			break;
		}

	}
	return ret;
}

void QMsgViewWnd::ini_stylesheet()
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QPushButton\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#FFFFFF;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width: 58px ;\
						   }\
						   QPushButton:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QPushButton:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QPushButton:disabled\
						   {\
						   background: #E5E5E5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QSplitter\
						   {\
						   background: #FAFAFA;\
						   }\
						   QFrame\
						   {\
						   background: #FAFAFA;\
						   }\
						   QLabel\
						   {\
						   background: #FAFAFA;\
						   }\
						   QHeaderView::section\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color: #0F1923;\
						   width:30px;\
						   height:30px;\
						   border: 0px solid #567dbc;\
						   border-left:0px;\
						   border-right:0px;\
						   background:#F1F1F1;\
						   }\
						   QSplitter\
						   {\
						   background: #FAFAFA;\
						   }\
						   QTabWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QTabWidget::pane\
						   {\
						   border:none;\
						   }\
						   QTableWidget \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 12px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: none;\
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
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
						   }\
						   QCheckBox::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QCheckBox::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QComboBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down \
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
						   QTreeView\
						   {\
						   border: none;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QTreeView::item:selected \
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
						   QLineEdit\
						   {\
						   background: #FFFFFF;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
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
						   QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton\
						   {\
						   border:0px solid #000000;\
						   border-radius:3px;\
						   margin:3px 3px 3px 3px;\
						   padding:3px;\
						   background-color:rgba(0,0,0,0);\
						   }\
						   QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolButton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed\
						   {\
						   border:1px solid #D8D8D8;\
						   }\
						   QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   margin:3px 3px 3px 3px;\
						   padding:0px -7px 0px 0px;\
						   }\
						   QDateEdit QCalendarWidget QSpinBox#qt_calendar_yearedit,QDateTimeEdit QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   padding:0px -2px 0px 0px;\
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
						   QToolBar\
						   {\
						   background: #FFFFFF;\
						   border: 0px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 30px;\
						   max-height: 30px;\
						   spacing:5px;\
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
						   QDateTimeEdit::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QDateTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QDateTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   ") ;
	setStyleSheet( buttonStyleSheet ) ;
}