#ifndef MASINFO_MAINWINDOW_H_
#define MASINFO_MAINWINDOW_H_

#include <QtGui>
#include <QSet>
#include <QMainWindow>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QTreeView>
#include <QIcon>
#include <QToolBar>
#include <QVector>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QCheckBox>
#include <QTimer>
#include <QStandardItemModel>
#include <QList>
#include <QVector>
#include <QReadWriteLock>
#include <QFile>

#include "dqinfo_nettree.h"
#include "dqinfo_checktime.h"
#include "dqinfo_filter.h"
//#include "dqinfo_remotewatch.h"
#include "dqinfo_getdbginfo.h"
#include "net/netapi.h"

class mainWindow: public QMainWindow
{
	Q_OBJECT
public:
	mainWindow();
	~mainWindow();
private slots:
	void selectTaskChange(QModelIndex index);
	void taskRightClick(QModelIndex index);
	void showTaskMenu(void);
	void logOutTask(void);
	void killTask(void);
	void setWindowTop(void);
	void setPause(void);
	void setContinue(void);
	void setSave(void);
	void clearDbgInfo(void);
	void checkTime(void);
	void setFilter(void);
	void exitApp(void);
	void onRemote(void);
	void onRemoteModeChange(int state);
	void onTimer(void);
	void clearTaskList(void);
	void recvDbgInfo(int typeNo, QString type, QString desc);
	void saveDbgToFile(QString type, QString desc);
	void selectDbgMsgs(QModelIndex index);
	void copyInfo(void);

protected:
	void closeEvent(QCloseEvent *event);

private:
	void createActions(void);
	void createMenus(void);
	void initRemoteHost(void);
	void localTaskList(void);
	void remoteTaskList(void);
	void saveDbgToFile(void);
	void showMenu(void);
	
private:
	QSplitter*			m_pRemoteSplitter;
	QSplitter*			m_pHorSplitter;
	QSplitter*			m_pVerSplitter;
	QNetTree*			m_pNetTree;
	QTreeView*			m_pTaskList;
	QTreeView*			m_pdbgInfoList;
	QComboBox*			m_pRemoteHost;
	QCheckBox*			m_pRemoteCheck;
	QToolBar*			m_pToolBar;
	QStandardItemModel*	m_pTaskListSource;
	QStandardItemModel*	m_pDbgInfoSource;
	QMenu*				m_pFileMenu;
	QMenu*				m_pViewMenu;
	QMenu*				m_pOperMenu;
	QAction*			m_pTopWindow;
	QAction*			m_pClear;
	QAction*			m_pPause;
	QAction*			m_pContinue;
	QAction*			m_pSave;
	QAction*			m_pFilter;
	QAction*			m_pCheckTime;
	QAction*			m_pShowNode;
	QAction*			m_pRemote;
	QAction*			m_pExit;
	QAction*			m_pCopy;
	QTimer*				m_pTaskListTimer;
	NetDbg				m_dbg;
	QGetDbgInfo         m_getDbgInfo;
	QVector< QList<QStandardItem*> > m_tmpItems;
	QReadWriteLock		m_dbgInfoSrcMudex;
	QFile   m_file;
	bool m_pauseFlag;
	bool m_saveFlag;
	QVector<QString> m_StringFilter;
};


#endif