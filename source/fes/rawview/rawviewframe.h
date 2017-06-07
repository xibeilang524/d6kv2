#ifndef RAWVIEWFRAME_H_
#define RAWVIEWFRAME_H_

#include <QtGui/QFrame>
#include <QtGui/QTableWidget>
#include <QtCore/QTimer>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include "fes/fesshm.h"
#include "fes/fesdatashm_accessor.h"
#include "db/dbapi.h"

#ifdef WIN32
#ifndef strcasecmp
#define strcasecmp stricmp
#endif
#endif

#define _Q2C( str ) ( ( const char * )( str.local8Bit(  ).data(  ) ) )
#define _C2Q( str ) ( QString::fromLocal8Bit( str ) )

typedef struct rtdata
{
	int no;
	QString desc;
	int isnegate;
	int iscomputer;
}RTDATA, *PRTDATA;

class QRawViewFrame: public QFrame
{
	Q_OBJECT

public:
	QRawViewFrame( QWidget* parent = NULL );
	~QRawViewFrame(  );
	void setPara( int rtuNo , int chan_no);
	void fillData( );

public:
	QTabWidget*			m_pTabWidget;
	QTableWidget*		m_pYcTable;
	QTableWidget*		m_pYxTable;
	QTableWidget*		m_pKwhTable;

	PRTDATA				m_yc;
	PRTDATA				m_yx;
	PRTDATA				m_kwh;

	int					m_ycNum;
	int					m_yxNum;
	int					m_kwhNum;


	int					m_tableLock;
	int					m_rtuChanged;
    DBApi		m_db;

private:
	int					m_curRTU;
	int					m_curCHAN;


private slots:
	void				onReload(  );

private:
	void	 initTable(  );
	bool	 readYcPara(  );
	bool	 readYxPara(  );
	bool	 readKwhPara(  );
	void	 clearrdlist(  );

	bool	 readFile( char *filename, void ** buf );
	bool	 readFile( char *filename, void * buf, int len );

};
#endif