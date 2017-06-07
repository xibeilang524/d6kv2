#ifndef RAWVIEW_H_
#define RAWVIEW_H_

#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTabWidget>
#include <QMap>
#include "rawviewframe.h"
typedef struct _StationAreaPara
{
	short	id;						//id
	char	name[REASONINFO_LEN];	//name
	short	masterid;				//masterAreaId
}STATIONAREAPARA,*PSTATIONAREAPARA;


//站所线系参数表
typedef struct _ChannelPara
{
	short  channo;//
	short  terminalno;//终端序号
	short  areaid;
}CHANNELPARA,*PCHANNELPARA;

class QRawViewWnd : public QSplitter
{
	Q_OBJECT

public:
	QRawViewWnd( QWidget *parent = NULL );
	~QRawViewWnd();

private:
	QTreeWidget*			m_pChanTree;
	QRawViewFrame*		m_pRawFrame;
	QTimer*						m_pTimer;
	QIcon							m_rootIcon;	
	QIcon							m_rtuIcon;
	Fes_shm&					m_fes_shm;
	Fes_data_shm_accessor&	m_fes_data_shm ;
	int								m_curRtu;
	QMap<int,PSTATIONAREAPARA>	m_station_area;
	QMap<int,PCHANNELPARA>		m_channelpara;
    DBApi		m_db2;
private:
	void showMenu(  );
	void set_all_chan_rtu(  );
	int readstationarea_data();
	int read_channeldata();
	void initStyleSheet();

public slots:
	void onTreeItemChange(  );
	void onTimer(  );
	void reload(  );
	void itemClicked( QTreeWidgetItem *item, int column );

};


#endif