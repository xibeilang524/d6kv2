#include "manageutils.h"

// 定义全局变量
QComtradeMgr* g_ComtradeMgr = NULL;
QGraphMgr* g_GraphMgr = NULL;


QComtradeMgr::QComtradeMgr()
{
// 	m_lstData = new  QList<QComtrade>;
//	m_lstData->setAutoDelete(true);
// 	m_lstFunc = new  QList<QAnalyze>;
//	m_lstFunc->setAutoDelete(true);
}

QComtradeMgr::~QComtradeMgr()
{
	if (g_GraphMgr != NULL)
		g_GraphMgr->ClearIntf();

// 	 QListIterator<QAnalyze> itA(*m_lstFunc);
//     QAnalyze* a;
//     while ((a = itA.current()) != 0) 
// 	{
//         ++itA;
// 		delete a;
//     }	
// 	delete m_lstFunc;
// 
// 	 QListIterator<QComtrade> itC(*m_lstData);
//     QComtrade* c;
//     while ((c = itC.current()) != 0) 
// 	{
//         ++itC;
// 		delete c;
//     }	
// 	delete m_lstData;

	QList<QAnalyze*>::iterator itFunc;
	for (itFunc=m_lstFunc.begin();itFunc!=m_lstFunc.end();++itFunc)
	{
		delete (*itFunc);
		(*itFunc) = NULL;
	}
	m_lstFunc.clear();

	QList<Comtrade*>::iterator itData;
	for (itData=m_lstData.begin();itData!=m_lstData.end();++itData)
	{
		delete (*itData);
		(*itData) = NULL;
	}
	m_lstData.clear();

}

Comtrade* QComtradeMgr::comtrade(int index)
{
	if (index < 0 || (uint)index > m_lstData.count())
		return NULL;
	return m_lstData.at(index);
}

QAnalyze* QComtradeMgr::Analyze(int index)
{
	if (index < 0 || (uint)index > m_lstFunc.count())
		return NULL;
	return m_lstFunc.at(index);
}

int QComtradeMgr::Count()
{
	return m_lstData.count();
}

void QComtradeMgr::Add(Comtrade* c)
{
	if (c != NULL)
	{
		m_lstData.append(c);
		QAnalyze* a = new QAnalyze(c);
		m_lstFunc.append(a);
	}
}

void QComtradeMgr::Delete(int index)
{
	if (index < 0 || (uint)index >= m_lstData.count())
		return;
	if (g_GraphMgr != NULL)
	{
		g_GraphMgr->Graph(index)->clear_intf();
		g_GraphMgr->Caption(index)->Clear();
	}
	Comtrade* c = m_lstData.at(index);
	m_lstData.removeAt(index);
	delete c;
	QAnalyze* a = m_lstFunc.at(index);
	m_lstFunc.removeAt(index);
	delete a;
}

void QComtradeMgr::Insert(int index, Comtrade* c)
{
	if (index < 0 || (uint)index > m_lstData.count())
		return;
	if (c != NULL)
	{
		m_lstData.insert(index, c);
		QAnalyze* a = new QAnalyze(c);
		m_lstFunc.insert(index, a);
	}
}

int QComtradeMgr::IndexOf(Comtrade* c)
{
	return m_lstData.indexOf(c);
}


// 图形和标识管理者

QGraphMgr::QGraphMgr()
{
// 	m_lstWave = new  QList<QWaveGraph>;
//	m_lstWave->setAutoDelete(true);
// 	m_lstCaption = new  QList<QWaveCaption>;
//	m_lstCaption->setAutoDelete(true);
}

QGraphMgr::~QGraphMgr()
{
	ClearIntf();
// 	 QListIterator<QWaveGraph> itW(*m_lstWave);
//     QWaveGraph* w;
//     while ((w = itW.current()) != 0) 
// 	{
//         ++itW;
// 		delete w;
//     }	
// 	delete m_lstWave;
// 
// 	 QListIterator<QWaveCaption> itC(*m_lstCaption);
//     QWaveCaption* c;
//     while ((c = itC.current()) != 0) 
// 	{
//         ++itC;
// 		delete c;
//     }	
// 	delete m_lstCaption;

	QList<Wave_graph*>::iterator itWave;
	for (itWave=m_lstWave.begin();itWave!=m_lstWave.end();++itWave)
	{
		delete *itWave;
		(*itWave)=NULL;
	}
	m_lstWave.clear();

	QList<Wave_caption*>::iterator itCaption;
	for (itCaption=m_lstCaption.begin();itCaption!=m_lstCaption.end();++itCaption)
	{
		delete (*itCaption);
		(*itCaption)=NULL;
	}
	m_lstCaption.clear();

}

Wave_graph* QGraphMgr::Graph(int index)
{
	if (index < 0 || (uint)index > m_lstWave.count())
		return NULL;
	return m_lstWave.at(index);
}

Wave_caption* QGraphMgr::Caption(int index)
{
	if (index < 0 || (uint)index > m_lstCaption.count())
		return NULL;
	return m_lstCaption.at(index);
}

int QGraphMgr::Count()
{
	return m_lstWave.count();
}

void QGraphMgr::Add(Wave_graph* w, Wave_caption* c)
{
	m_lstWave.append(w);
	m_lstCaption.append(c);
}

void QGraphMgr::Delete(int index)
{
	if (index < 0 || (uint)index >= m_lstWave.count())
		return;
	Wave_graph* w = m_lstWave.at(index);
	m_lstWave.removeAt(index);
	delete w;

	Wave_caption* c = m_lstCaption.at(index);
	m_lstCaption.removeAt(index);
	delete c;
}

int QGraphMgr::IndexOf(Wave_graph* w)
{
	return m_lstWave.indexOf(w);
}

void QGraphMgr::ClearIntf()
{
	for (uint i = 0; i < m_lstWave.count(); i++)
		m_lstWave.at(i)->clear_intf();
}