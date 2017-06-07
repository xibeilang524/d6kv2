#include "dqinfo_getdbginfo.h"
#include "dqinfo_dbgpara.h"

extern CDbgPara g_dbgPara;



QGetDbgInfo::QGetDbgInfo(QWidget *o)
:QThread()
{
	m_pDbgModel = new QStandardItemModel(0, 2, this);
	m_pDbgModel->setHeaderData(0, Qt::Horizontal, QObject::tr("信息类型"));
	m_pDbgModel->setHeaderData(1, Qt::Horizontal, QObject::tr("信息描述"));
}

void QGetDbgInfo::run()
{
	while(true)
	{
		char type[40];
		char desc[255];
		int  typeNo = 0;

		if (g_dbgPara.getDbg(&typeNo, type, desc) == NULL)
		{
			_delay_time(10 * 1000);
			continue;
		}
		
		emit sendDbgInfo(typeNo, QString::fromLocal8Bit(type), QString::fromLocal8Bit(desc));
		_delay_time(10 * 1000);

	}
}

