#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QSplitter>
#include <QTimer>
#include <QLabel>
#include "protocolthread.h"
#include "listenthread.h"

class mainWindow: public QMainWindow
{
	Q_OBJECT

public:
	mainWindow();

protected:
	void closeEvent(QCloseEvent *event);

signals:
	void stormCommand(void);

private slots:
	void onListen(void);
	void onStorm(void);
	void onSetting(void);
	void onExit(void);
	void onShowTimer(void);
	void onStormTimer(void);

private:
	void defaultSetting(void);
	void refleshStatTable(void);
	void refleshProtocol(void);
	void initStyleSheet( void ) ;

private:
	QProtocolThread** m_ppProtocol;
	QPushButton*	  m_pStartListen;
	QPushButton*	  m_pStartStorm;
	QPushButton*      m_pSetting;
	QPushButton*	  m_pExit;
	QTableWidget*	  m_pStatTable;
	QSplitter*		  m_pHSplitter;
	QSplitter*		  m_pVSplitter;
	QTimer*			  m_pShowTimer;
	QTimer*			  m_pStormTimer;
	int				  m_stormCount;
	QListenThread	  m_listemThread;
	QLabel*			  m_pStatusLabel;
	QLabel*			  m_pStormNumLabel;
};
#endif