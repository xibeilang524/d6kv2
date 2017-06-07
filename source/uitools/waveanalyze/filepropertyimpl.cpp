#include <qfileinfo.h>
#include <qdatetime.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include "filepropertyimpl.h"
#include "globaldef.h"

File_property_impl::File_property_impl(Comtrade* c, QWidget* parent)
	: QDialog(parent)
{
	setupUi(this) ;

	m_pData = c;
	init();
	connect(m_btnOk, SIGNAL(clicked()), SLOT(accept()));
	connect(m_btnCancel, SIGNAL(clicked()), SLOT(reject()));
}

File_property_impl::~File_property_impl()
{
}

void File_property_impl::init()
{
	// 常规
	QFileInfo fl(m_pData->fault_file());
	m_edtFileName->setText(fl.fileName());
	m_edtFileType->setText(m_pData->file_fype());
	m_edtFilePath->setText(fl.path());
	QString str;
	QFile ff(fl.filePath());
	str.sprintf("%d bytes", ff.size());
	m_edtFileSize->setText(str);
	m_edtCreateTime->setText(fl.created().toString());
	m_edtModifyTime->setText(fl.lastModified().toString());
	m_edtReadTime->setText(fl.lastRead().toString());
	m_chkCfgfile->setChecked(true);
	m_chkDatfile->setChecked(true);

	//统计信息
	int samCount = m_pData->sample_count();
	m_lvConfig->setColumnCount(2);
	m_lvConfig->setRowCount(7+samCount);
	QTableWidgetItem* item = NULL;
	m_lvConfig->setItem(0,0,new QTableWidgetItem(_C2Q("模拟量通道数: ")));
	m_lvConfig->setItem(0,1,new QTableWidgetItem(QString::number(m_pData->Count(0))));
	m_lvConfig->setItem(1,0,new QTableWidgetItem(_C2Q("状态量通道数: ")));
	m_lvConfig->setItem(1,1,new QTableWidgetItem(QString::number(m_pData->Count(1))));
	m_lvConfig->setItem(2,0,new QTableWidgetItem(_C2Q("采样时间: ")));
	m_lvConfig->setItem(2,1,new QTableWidgetItem(m_pData->start_time()));
	m_lvConfig->setItem(3,0,new QTableWidgetItem(_C2Q("故障时间: ")));
	m_lvConfig->setItem(3,1,new QTableWidgetItem(m_pData->trigger_time()));
	m_lvConfig->setItem(4,0,new QTableWidgetItem(_C2Q("数据总点数: ")));
	m_lvConfig->setItem(4,1,new QTableWidgetItem(QString::number(m_pData->data_count())));
	m_lvConfig->setItem(5,0,new QTableWidgetItem(_C2Q("系统频率: ")));
	m_lvConfig->setItem(5,1,new QTableWidgetItem(QString::number(m_pData->sys_frequency())));
	m_lvConfig->setItem(6,0,new QTableWidgetItem(_C2Q("采样时段数量: ")));
	m_lvConfig->setItem(6,1,new QTableWidgetItem(QString::number(samCount)));

	SAMPLE* sam = new SAMPLE[samCount];
	m_pData->get_full_sample(sam);
	for (int i=0;i<samCount;i++)
	{
		str = QString(_C2Q("采样时段 %1：")).arg(i+1);
		m_lvConfig->setItem(7+i,0,new QTableWidgetItem(str));
		str = QString(_C2Q("采样频率: %1, 点数: %2")).arg( (int)sam[i].frequency).arg(sam[i].count);
		m_lvConfig->setItem(7+i,1,new QTableWidgetItem(str));
	}
	m_lvConfig->resizeColumnsToContents();
}