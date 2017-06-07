#ifndef MSGVIEWWND_H_
#define MSGVIEWWND_H_

#include <QtGui>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <fstream>
#include <QList>
#include <QDir>
#include <QMap>
using namespace std;

#include "msgframe.h"
#include "CNetModula.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/fesdev.h"
#include "sysdef.h"


const int PORT_NO = 9005;			//想办法移动到更好的地方去
#define  RECV_LEN 1024

struct CHANNEL_LIST
{
	unsigned short channel_no; //通道号
	char describe[SHORT_DESC_LENTH]; //通道描述
	char protocol[CODE_LENTH];
	unsigned char valid;
};
typedef struct _StationAreaPara
{
	short	id;						//id
	char	name[REASONINFO_LEN];	//name
	short	masterid;				//masterAreaId
}STATIONAREAPARA,*PSTATIONAREAPARA;


typedef struct _ChannelPara
{
	short  channo;//
	short  terminalno;//终端序号
	short  areaid;
}CHANNELPARA,*PCHANNELPARA;
class QMsgViewWnd: public QSplitter
{
	Q_OBJECT

public:
	QMsgViewWnd(QWidget *parent = NULL);
	~QMsgViewWnd();

	void onPause(void);
	void onContinue(void);
	void showTree(void);

private:
	void getCurChanMsg(void);
	void closeEvent(QCloseEvent *event);
	
	void list_all_channels();

	int request_fep_msg();
	int read_stationareadata();
	int read_channeldata();
	void ini_stylesheet() ;
	//QTreeWidgetItem* FindTreeItem(QTreeWidgetItem* parent, QString strName);
	//QTreeWidgetItem* AddTreeItem(QTreeWidgetItem* parent, QString strName);

private:
	QTreeWidget*		m_pChanTree;
	QMsgFrame*			m_pMsgFrame;
	QIcon				m_rootIcon;
	QIcon				m_chanIcon;
	QIcon				m_chanPauseIcon;
	CNetModula*			m_Modula;
	QTimer*				m_pTimer;
	int					m_curModula;		//通道序号

	int					m_channel_num;

	Tcp_client*  tcp_recv;
// 	CHANNEL_LIST* db_channel;
	QMap	<int, QString>	channel_protocol;
	QMap<int,PSTATIONAREAPARA>	m_station_area;
	QMap<int,PCHANNELPARA>		m_channelpara;
public slots:
	void onTreeItemChange(/*QTreeWidgetItem* current*/);
	void onTimer();

};
#endif