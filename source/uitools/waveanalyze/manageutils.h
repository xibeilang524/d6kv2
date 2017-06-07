#ifndef MANAGEUTILS_H
#define MANAGEUTILS_H

#include <QList>

#include "comtrade.h"
#include "analyzeutils.h"
#include "wavegraph.h"
#include "wavecaption.h"

// 数据和功能管理者
class QComtradeMgr 
{
public:
	QComtradeMgr();
	~QComtradeMgr();

	Comtrade* comtrade(int index);
	QAnalyze* Analyze(int index);

	int Count();
	void Add(Comtrade* c);
	void Delete(int index);
	void Insert(int index, Comtrade* c);
	int IndexOf(Comtrade* c);

private:
	 QList<Comtrade*> m_lstData;
	 QList<QAnalyze*> m_lstFunc;
	
};


// 图形和标识管理者
class QGraphMgr
{
public:
	QGraphMgr();
	~QGraphMgr();

	Wave_graph* Graph(int index);
	Wave_caption* Caption(int index);

	int Count();
	void Add(Wave_graph* w, Wave_caption* c);
	void Delete(int index);
	int IndexOf(Wave_graph* w);
	void ClearIntf();

private:
	 QList<Wave_graph*> m_lstWave;
	 QList<Wave_caption*> m_lstCaption;

};

extern QComtradeMgr* g_ComtradeMgr;
extern QGraphMgr* g_GraphMgr;

#endif