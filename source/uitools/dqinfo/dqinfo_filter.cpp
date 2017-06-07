#include "dqinfo_filter.h"
#include "dqinfo_dbgpara.h"
#include "uitools/dmessagebox.h"

extern CDbgPara g_dbgPara;

QDbgCheck::QDbgCheck(int type, QString desc, QWidget * parent /* = NULL */)
:QCheckBox(desc, parent)
{
	m_type = type;
	m_desc = desc;
}

QFilter::QFilter(QWidget* parent /* = NULL */)
:QDialog(parent)
{
	QDbgCheck* dbgTypeLeft;
	QDbgCheck* dbgTypeRight;
	char typedes[DEBUG_MAX_TYPE][DESC_LEN];

	m_pVLayout = new QVBoxLayout(NULL);
	m_pHLayout = new QHBoxLayout(NULL);
	m_pGridlayout = new QGridLayout(NULL);

	m_pOK = new QPushButton(_C2Q("确定"), this);
	m_pCancel = new QPushButton(_C2Q("取消"), this);
	m_pSelectAll = new QPushButton(_C2Q("全选"), this);
	m_pSelectNone = new QPushButton(_C2Q("全不选"), this);

	connect(m_pSelectAll, SIGNAL(clicked()), this, SLOT(onSelectAll()));
	connect(m_pSelectNone, SIGNAL(clicked()), this, SLOT(onSelectNone()));
	connect(m_pOK, SIGNAL(clicked()), this, SLOT(onOk()));
	connect(m_pCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

	m_pGridlayout->addWidget(m_pSelectAll, 0, 0);
	m_pGridlayout->addWidget(m_pSelectNone, 0, 1);
	m_pGridlayout->addWidget(m_pOK, 1, 0);
	m_pGridlayout->addWidget(m_pCancel, 1, 1);
	
	if( g_dbgPara.m_remoteFlag )
	{
		for (int i = 0; i < g_dbgPara.m_remoteDbg.proc_num; i++)
		{
			if( g_dbgPara.m_dbgID != g_dbgPara.m_remoteDbg.proc[i].regidx )
			{
				continue;
			}
			
			DbgProcInfo*	proc = g_dbgPara.m_remoteDbg.proc;
			
			int typenum = proc[i].typenum;
			
			for (int j=0; j < ( typenum + 1 ) / 2; j++)
			{
				dbgTypeLeft = dbgTypeRight = NULL;
				dbgTypeLeft = new QDbgCheck(proc[i].type[j * 2].type, _C2Q(proc[i].type[j * 2].desc), this);
				//dbgTypeLeft->setCheckState(Qt::Checked);
				m_checkGroup.push_back(dbgTypeLeft);
				
				if (typenum > (j * 2 + 1))
				{
					dbgTypeRight = new QDbgCheck(proc[i].type[j * 2 + 1].type, _C2Q(proc[i].type[j * 2 + 1].desc), this);
					//dbgTypeRight->setCheckState(Qt::Checked);
					m_checkGroup.push_back(dbgTypeRight);
				}
				
				QHBoxLayout* line = new QHBoxLayout(NULL);
				if (dbgTypeLeft != NULL)
				{
					line->addWidget(dbgTypeLeft);
				}
				
				if (dbgTypeRight != NULL)
				{
					line->addWidget(dbgTypeRight);
				}
				
				m_pVLayout->addLayout(line);
			}
			
			break;
		}
	}
	else
	{
		int typeNum = m_dbg.GetTypeEnm(g_dbgPara.m_dbgID, typedes);
		int* index = m_dbg.GetTypeIndex(); 
		for (int i = 0; i < (typeNum + 1) / 2; i++)
		{
			dbgTypeLeft = dbgTypeRight = NULL;

			dbgTypeLeft = new QDbgCheck(index[i * 2], _C2Q(typedes[i * 2]), this);
			//dbgTypeLeft->setCheckState(Qt::Checked);
			m_checkGroup.push_back(dbgTypeLeft);

			if (typeNum > (i * 2 + 1))
			{
				dbgTypeRight = new QDbgCheck(index[i * 2 + 1], _C2Q(typedes[i * 2 + 1]), this);
				//dbgTypeRight->setCheckState(Qt::Checked);
				m_checkGroup.push_back(dbgTypeRight);
			}
			
			QHBoxLayout* line = new QHBoxLayout(NULL);

			if (dbgTypeLeft != NULL)
			{
				line->addWidget(dbgTypeLeft);
			}

			if (dbgTypeRight != NULL)
			{
				line->addWidget(dbgTypeRight);
			}
		
			m_pVLayout->addLayout(line);
		}
	}

	m_pVLayout->addLayout(m_pGridlayout);
	setLayout(m_pVLayout);
	setWindowTitle(_C2Q("信息过滤"));
	setWindowIcon(QIcon(":/res/view-filter.png"));

	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QToolButton,QPushButton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font: 14px;\
						   color:#FFFFFF;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width: 58px ;\
						   }\
						   QToolButton:hover,QPushButton:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:pressed,QPushButton:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:checked,QPushButton:checked\
						   {\
						   background: #4A90E2;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:disabled,QPushButton:disabled\
						   {\
						   background: #E5E5E5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}


void QFilter::onSelectAll(void)
{
	for (int i = 0; i < m_checkGroup.size(); i++)
	{
		m_checkGroup[i]->setCheckState(Qt::Checked);
	}
}


void QFilter::onSelectNone(void)
{
	for (int i = 0; i < m_checkGroup.size(); i++)
	{
		m_checkGroup[i]->setCheckState(Qt::Unchecked);
	}
}

void QFilter::onOk(void)
{
	m_filter.clear();

	for (int i = 0; i < m_checkGroup.size(); i++)
	{
		if (Qt::Checked == m_checkGroup[i]->checkState())
		{
			m_filter.push_back(m_checkGroup[i]->m_desc);
		}
	}

	if (0 == m_filter.size())
	{
		DMessageBox::information( this ,tr("提示") , tr( "请选择至少一种调试信息类型" ) ) ;
	}
	else
	{
		accept();
	}
	
}

void QFilter::onCancel(void)
{
	reject();
}


QVector<QString> QFilter::getFilter(void)
{
	return m_filter;
}

void QFilter::setFilter(QVector<QString> filter)
{
	int i;
	//第一次打开过滤界面时，所有全部勾选
	if (0 == filter.size())
	{
		for (i = 0; i < m_checkGroup.size(); i++)
		{
			m_checkGroup[i]->setCheckState(Qt::Checked);
		}

		return;
	}

	for (i = 0; i < m_checkGroup.size(); i++)
	{
		if (filter.contains(m_checkGroup[i]->text()))
		{
			m_checkGroup[i]->setCheckState(Qt::Checked);
		}
	}
}
