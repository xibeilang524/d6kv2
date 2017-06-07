#ifndef FESOPER_H_
#define FESOPER_H_

#include <QtGui>
#include <QMainWindow>
#include <QListWidget>
#include <QString>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QTimer>
#include "fes/fesshm.h"
#include "fes/femailstruct.h"

const int REFLESHTIME = 2000;


class QTerminalTreeItem :public QTreeWidgetItem
{
public:
	QTerminalTreeItem(int rtuNo, QStringList & strings);
	int getRtuNo(void){return m_rtuNo;};
private:
	int m_rtuNo;
};

class QChanTreeItem :public QTreeWidgetItem
{
public:
	QChanTreeItem(int chanNo, QStringList & channel_name);
	int getChanNo(void){return m_chanNo;};

private:
	int m_chanNo;
};


class QFesOper: public QMainWindow
{
	Q_OBJECT
public:
	QFesOper(QWidget* parent = NULL);
	~QFesOper();

private slots:
	void onCallAllData(void);
	void onSendTime(void);
	void onCallKwh(void);
	void onResetRtu(void);
	void onResetChan(void);
	void onRTUSelect(QTreeWidgetItem *, int);
	void onTimer(void);
	void onAuto(void);
	void onChanSelect(QTreeWidgetItem *item, int order);
	void onMannal_Load_Para();
private:
	void init_mail_buf(void);

	/**
	@brief 显示所有终端
	*/
	void set_RTU_Tree(void);

	void set_CHAN_Tree(void);

	int current_rtuno(void);

	int current_channelno(void);

private:
	QTreeWidget*		 m_pChanTree;
	QTreeWidget*		 m_pRtuTree;

	QGroupBox*			 m_pRtuOperGroup;
	QGroupBox*			 m_pChanOperGroup;

	
	QGridLayout*		 m_pRtuOperLayout;
	QHBoxLayout*		 m_pHChanOperLayout;
	QVBoxLayout*		 m_pVRtuTreeLayout;
	QSplitter*				 m_wholeHSplitter;
	QSplitter*				 m_pOperSplitter;

	QPushButton*		 m_pCallDataButton;
	QPushButton*		 m_pSendTimeButton;
	QPushButton*		 m_pCallKwhButton;
	QPushButton*		 m_pRtuResetButton;

	QPushButton*		 m_pChanResetButton;
	QPushButton*		 m_pSwitchToMain;
	QPushButton*		 m_pMannel_Load_Para;

	QTimer*				 m_pTimer;

	int					 m_main_chanNo;			//当前终端的主运行通道

	Fes_shm&			 m_fes_shm;
	FE_MAIL_STRUCT*		 m_mail_struct;		//邮件指针

};


#endif