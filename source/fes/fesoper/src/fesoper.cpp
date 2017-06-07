#include "fesoper.h"
#include "fesoper_string.h"
#include "fes/platformwrapper.h"


QTerminalTreeItem::QTerminalTreeItem(int rtuNo, QStringList & strings)
:QTreeWidgetItem(strings)
{
	m_rtuNo = rtuNo;
}

QChanTreeItem::QChanTreeItem(int chanNo, QStringList & channel_name)
:QTreeWidgetItem(channel_name)
{
	m_chanNo = chanNo;
}


QFesOper::QFesOper(QWidget* parent /* = NULL */)
:QMainWindow(parent),m_fes_shm(Fes_shm::get())
{
	m_pChanTree = new QTreeWidget(this);
	m_pChanTree->setHeaderLabel(_C2Q("通道列表"));

	m_pRtuTree = new QTreeWidget(this);
	m_pRtuTree->setHeaderLabel(_C2Q("终端列表"));

	m_pRtuOperGroup = new QGroupBox(_C2Q("终端操作"), this);
	m_pChanOperGroup = new QGroupBox(_C2Q("通道操作"), this);

	m_wholeHSplitter = new QSplitter( Qt::Horizontal ) ;
	m_pOperSplitter = new QSplitter( Qt::Vertical ) ;

	m_pVRtuTreeLayout = new QVBoxLayout(NULL);
	m_pRtuOperLayout = new QGridLayout(NULL);
	m_pHChanOperLayout = new QHBoxLayout(NULL);

	m_pRtuResetButton = new QPushButton(QIcon(":/res/reset.png"), _C2Q("终端初始化"), m_pRtuOperGroup);
	m_pCallDataButton = new QPushButton(QIcon(":/res/callall.png"), _C2Q("召唤全数据"), m_pRtuOperGroup);
	m_pSendTimeButton = new QPushButton(QIcon(":/res/time.png"), _C2Q("对时"), m_pRtuOperGroup);
	m_pCallKwhButton = new QPushButton(QIcon(":/res/callkwh.png"), _C2Q("召唤电度"), m_pRtuOperGroup);

	m_pChanResetButton = new QPushButton(QIcon(":/res/reset.png"), _C2Q("通道重启"), m_pChanOperGroup);
// 	m_pSwitchToMain = new QPushButton(QIcon(":/res/setmain.png"), _C2Q("切主运行"), m_pChanOperGroup);
	m_pMannel_Load_Para = new QPushButton(QIcon(":/res/load_para.png"), _C2Q("手动加载参数"), m_pChanOperGroup);

	m_pRtuOperLayout->addWidget(m_pRtuResetButton, 0, 0);
	m_pRtuOperLayout->addWidget(m_pCallDataButton, 0, 1);
	m_pRtuOperLayout->addWidget(m_pSendTimeButton, 1, 0);
	m_pRtuOperLayout->addWidget(m_pCallKwhButton, 1, 1);
	m_pVRtuTreeLayout->addWidget(m_pRtuTree);
	m_pVRtuTreeLayout->addLayout(m_pRtuOperLayout);
	m_pRtuOperGroup->setLayout(m_pVRtuTreeLayout);

// 	m_pVChanOperLayout->addWidget(m_pSwitchToMain);
	m_pHChanOperLayout->addWidget(m_pMannel_Load_Para);
	m_pHChanOperLayout->addWidget(m_pChanResetButton);
	m_pChanOperGroup->setLayout(m_pHChanOperLayout);

	m_pOperSplitter->addWidget(m_pRtuOperGroup);
	m_pOperSplitter->addWidget(m_pChanOperGroup);

	m_wholeHSplitter->addWidget( m_pChanTree ) ;
	m_wholeHSplitter->addWidget( m_pOperSplitter ) ;

	m_pTimer = new QTimer(this);

	setCentralWidget( m_wholeHSplitter ) ;
	setGeometry(300, 200, 700, 800);
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	setWindowTitle(_C2Q("前置控制台"));	
 
	init_mail_buf();
	set_CHAN_Tree();
// 	set_RTU_Tree();
// 	connect(m_pRtuTree, SIGNAL(itemClicked(QTreeWidgetItem *, int )), this, SLOT(onRTUSelect(QTreeWidgetItem *, int)));
	connect(m_pChanTree, SIGNAL(itemClicked(QTreeWidgetItem *, int )), this, SLOT(onChanSelect(QTreeWidgetItem *, int)));
	connect(m_pCallDataButton, SIGNAL(clicked()), this, SLOT(onCallAllData()));
	connect(m_pCallKwhButton, SIGNAL(clicked()), this, SLOT(onCallKwh()));
	connect(m_pSendTimeButton, SIGNAL(clicked()), this, SLOT(onSendTime()));
	connect(m_pRtuResetButton, SIGNAL(clicked()), this, SLOT(onResetRtu()));
	connect(m_pChanResetButton, SIGNAL(clicked()), this, SLOT(onResetChan()));
	connect(m_pMannel_Load_Para, SIGNAL(clicked()), this, SLOT(onMannal_Load_Para()));
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));

	m_pTimer->start(REFLESHTIME);
	m_main_chanNo = -1;
	m_pChanResetButton->setEnabled(false);

	m_pRtuResetButton->setEnabled(false);
	m_pSendTimeButton->setEnabled(false);
	m_pCallDataButton->setEnabled(false);
	m_pCallKwhButton->setEnabled(false);

	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QPushButton\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font-family: Microsoft YaHei;\
						   font-size: 16px;\
						   color:#FFFFFF;\
						   min-height: 40px;\
						   max-height: 40px;\
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
						   QGroupBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font: 16px;\
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
						   QFrame\
						   {\
						   background: #FAFAFA;\
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
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}

QFesOper::~QFesOper()
{
	delete []m_mail_struct;
	m_mail_struct = NULL;
}

void QFesOper::init_mail_buf(void)
{
	//建立邮件内存
	m_mail_struct = (FE_MAIL_STRUCT*)malloc(sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));
}

void QFesOper::set_CHAN_Tree(void)
{
	for (int chan_no = 0; chan_no < MAX_CHAN_NUM; chan_no++)
	{
		CHANPARA* chan_para = m_fes_shm.chan_para_ptr + chan_no;

		if (chan_para->valid == 0)
		{
			continue;
		}
		m_pChanTree->addTopLevelItem(new QChanTreeItem(chan_para->order, QStringList()<<_C2Q(chan_para->desdribe)));
	}
}

void QFesOper::set_RTU_Tree(void)
{
}

void QFesOper::onRTUSelect(QTreeWidgetItem *item, int order)
{

}

void QFesOper::onChanSelect(QTreeWidgetItem *item, int order)
{
	QChanTreeItem * selected_item = (QChanTreeItem*)item;
	int chan_no = selected_item->getChanNo();

	if( chan_no < 0 )
	{
		m_pChanResetButton->setEnabled(false);

		m_pRtuResetButton->setEnabled(false);
		m_pSendTimeButton->setEnabled(false);
		m_pCallDataButton->setEnabled(false);
		m_pCallKwhButton->setEnabled(false);
		return;
	}
	else
		m_pChanResetButton->setEnabled(true);


	int child_rtu = m_fes_shm.chan_para_ptr[chan_no].child_rtu;		//终端所带终端号
	m_pRtuTree->clear();

	if (child_rtu > -1)
	{
		RTUPARA* rtu_para = m_fes_shm.rtu_para_ptr + child_rtu;
		QTerminalTreeItem *item = new QTerminalTreeItem(rtu_para->order, QStringList()<<_C2Q(rtu_para->desdribe));
		m_pRtuTree->addTopLevelItem(item);
 		m_pRtuTree->setCurrentItem(item);
		m_pRtuResetButton->setEnabled(true);
		m_pSendTimeButton->setEnabled(true);
		m_pCallDataButton->setEnabled(true);
		m_pCallKwhButton->setEnabled(true);
	
	}
	onTimer();
}

void QFesOper::onMannal_Load_Para()
{
	memset(m_mail_struct, 0, sizeof(FE_MAIL_STRUCT));	//初始化

	m_mail_struct->mail_type = LOAD_PARA;
	m_mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT);
	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);
}

void QFesOper::onSendTime(void)		//对时
{
	memset(m_mail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_mail_struct->mail_type = CALLING;
	m_mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_mail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CHECK_TIME;
	call_mail->channel_no = current_channelno();
	call_mail->rtu_no = current_rtuno();
	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);
}

void QFesOper::onCallAllData(void)	//总召
{
	memset(m_mail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_mail_struct->mail_type = CALLING;
	m_mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_mail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CALL_ALL;
	call_mail->channel_no = current_channelno();
	call_mail->rtu_no = current_rtuno();
	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);

}

void QFesOper::onCallKwh(void)		//召唤电度
{
	memset(m_mail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_mail_struct->mail_type = CALLING;
	m_mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_mail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CALL_KWH;
	call_mail->channel_no = current_channelno();
	call_mail->rtu_no = current_rtuno();
	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);

}

void QFesOper::onResetRtu(void)		//终端初始化
{
	memset(m_mail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_mail_struct->mail_type = CALLING;
	m_mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_mail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::RTU_INIT;
	call_mail->channel_no = current_channelno();
	call_mail->rtu_no = current_rtuno();
	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);

}


void QFesOper::onResetChan(void)		//通道初始化
{
	memset(m_mail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_mail_struct->mail_type = CALLING;
	m_mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_mail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CHANNEL_INIT;
	call_mail->channel_no = current_channelno();
	call_mail->rtu_no = current_rtuno();
	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);

}

void QFesOper::onAuto(void)
{

}

void QFesOper::onTimer(void)
{
	QChanTreeItem* chan_tree;
	int rtu_chan_num = m_pChanTree->columnCount();

	if ((-1 == m_main_chanNo) ||  rtu_chan_num == 0)
	{
		return;
	}
	for (int i = 0; i < rtu_chan_num; i++)
	{
		chan_tree = (QChanTreeItem*)m_pChanTree->topLevelItem(i);
		if (chan_tree->getChanNo() == m_main_chanNo)
		{
			chan_tree->setIcon(0, QIcon(":/res/main_channel.png"));
		}
	}
}


int QFesOper::current_rtuno(void)
{
	QTerminalTreeItem* current_rtu_item =  (QTerminalTreeItem*)m_pRtuTree->currentItem();

	return current_rtu_item->getRtuNo();
}

int QFesOper::current_channelno(void)
{
	QChanTreeItem* current_chan_item = (QChanTreeItem*)m_pChanTree->currentItem();

	return current_chan_item->getChanNo();

}
