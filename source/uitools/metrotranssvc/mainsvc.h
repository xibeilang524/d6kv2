#ifndef SANDDiag_H
#define SANDDiag_H

#include <QDialog>
#include <QTimer>
#include <QThread>
#include <QListWidget>

#include "globaldef.h"
#include "manageevent.h"


class Metro_trans_svc
{
public:
	Metro_trans_svc ();
	void Init();

public:
	QTimer *timer;

private:
	QThread *pMainThread; //主处理线程
	QThread *pEvtThread; //事项线程
	QThread *pControlThread;  //控制线程

};


class Main_thread : public QThread 
{    
	Q_OBJECT	
public:    
	Main_thread(Metro_trans_svc* dlg){
		m_dlg = dlg;
		m_pEvt = NULL;
		m_iEvtNum = 0;
	}; 
private:
	Metro_trans_svc* m_dlg;
protected:    
	void run();
	int deal_block_data(int no);					//判断闭锁条件是否瞒住
	int deal_da_file(QString filename);			//执行文件名为filename的文件
	void add_content(QString text);
signals:
	void update_signal(QString item);
public:
	EX_SYS_EVENT *m_pEvt;
	int m_iEvtNum;

// 	void run(){
// 		if (m_dlg)
// 		{
// 			m_dlg->DaProc();
// 		}
// 	}
};

class Evt_thread : public QThread 
{    
	Q_OBJECT	
public:    
	Evt_thread(Metro_trans_svc* dlg){
		m_dlg = dlg;
	}; 
private:
	Metro_trans_svc* m_dlg;
protected:
	void run();
// 	void run(){
// 
// 		if (m_dlg)
// 		{
// 			m_dlg->EvtProc();
// 		}
// 	}
};

class Action_thread : public QThread 
{    
	Q_OBJECT	
public:    
	Action_thread();    
	void add_content(QString text);
protected:    
	void run();
signals:
	void update_signal(QString item);
private:    
	Metro_trans_svc* m_dlg;
	bool stopped;
};

#endif