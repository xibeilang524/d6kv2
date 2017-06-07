#ifndef POWERCUTMNG_H
#define POWERCUTMNG_H
#include <QTreeWidgetItem>
#include <QDateTimeEdit>
#include <QToolButton>

#include <QEventPara.h>
#include "net/netapi.h"

#include "ui_outagequery.h"

class QPowerCutQuery : public QMainWindow ,public Ui::WindowBase 
{
	Q_OBJECT
public:
	QPowerCutQuery( QWidget* parent = 0 );	
	void iniAreaPage();
	void iniStPage();
	void iniFeederPage();
	void fillTable();
	bool timeChecked(uint time);

public slots:
	void onStQueryBtn() ;
	void onAreaQueryBtn();
	void onFeederQueryBtn();
	void onAllAreaBtn();
	void onAllStBtn();
	void onAllFeederBtn() ;
	void onRead();
	void onExit();
	void onQueryBtnClicked();

private:
	void findRecordByStation();
	void findRecordByArea() ;
	void findRecordByFeederLine() ;
	bool readPowerCutPara();

	bool getCutTypeDesc(uint &cutType, QString &breakName, uint &beginTime, QString &cutTypeDesc);
	bool readUserTypeFile();


private:
	//数据库读写类
	QEventPara		m_dbPara;
	QList<POWERCUTPARA *>		m_powerCutParaList;
	QList<POWERCUTPARA *>		m_initPowerCutParaList;

	//选中的厂站索引
	QMap<QTreeWidgetItem*,QString> m_item2StCodeMap;

	//选中的区域信息索引
	QMap<QTreeWidgetItem*,QString> m_item2AreaCodeMap;

	//选中的馈线设备索引
	QMap<QTreeWidgetItem*,QString> m_item2FeederLineMap;

	QStringList			m_AreaCodeList;
	QStringList			m_stCodeList;
	QStringList			m_feederNameList;

	//客户类型到客户描述索引
	QMap<uint, QString> m_userType2userDescMap;

	//查找开始时间
	QDateTimeEdit *m_beginDateTime;

	//查找结束时间
	QDateTimeEdit *m_endDateTime;

	//查询按钮
	QToolButton *m_queryBtn;

	SYS_TIME			m_beginTime;

	SYS_TIME			m_endTime;


};
#endif 