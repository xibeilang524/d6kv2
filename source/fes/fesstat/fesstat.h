#ifndef FESSTAT_H_
#define FESSTAT_H_

#include <QtGui/QDialog>
#include <QtGui/QTableWidget>
#include <QtGui/QListWidget>
#include <QtCore/QTimer>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include "fes/fesshm.h"

const int REFLESHTIME = 1000;


class QFesStat: public QDialog
{
	Q_OBJECT
public:
	QFesStat( QWidget* parent = NULL );
	~QFesStat(  );

private slots:
	void onTimer( void );
	void onTableMode( void );
	void onListMode( void );
	void onExit( void );

private:
	bool readProtocol( void );
	void initStatusList( void );
	void initBKList( void );
	void initHostList( void );
	void setChanStat( void );
	void setChanTable( void );
	void setChanList( void );
	void refleshChanStat( void );
	void refleshChanTable( void );
	void refleshChanList( void );
	int	 calcRealChanNum( void );

private:
	QTableWidget*	m_pChanStatTable;
	QListWidget*	m_pChanStatList;
	QTimer*				m_pTimer;
	QPushButton*	m_pTableButton;
	QPushButton*	m_pListButton;
	QPushButton*	m_pExitButton;
	QVBoxLayout*	m_pVLayout;
	QHBoxLayout*	m_pHLayout;
	QMap<QString, QString> m_protocol;
	QList<QString>	   m_statusList;
	QList<QString>	   m_bkList;
	QList<QString>	   m_hostList;
	Fes_shm&			 m_fes_shm;
};

#endif