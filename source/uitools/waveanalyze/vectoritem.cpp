#include "vectoritem.h"

/* QVectorMgr */

QVectorMgr::QVectorMgr()
{
// 	m_pSig = new QSignal;
// 	m_lstVector = new  QList<QVectorItem>;
// 	m_lstVector.setAutoDelete(true);
}

QVectorMgr::~QVectorMgr()
{
// 	delete m_pSig;
// 	delete m_lstVector;
}

void QVectorMgr::Delete(int index)
{
	if (index < 0 || (uint)index >= m_lstVector.count())
		return;

	m_lstVector.removeAt(index);
	Change();
}

QVectorItem* QVectorMgr::Item(int index)
{
	if (index < 0 || (uint)index >= m_lstVector.count())
		return NULL;

	return m_lstVector.at(index);
}

QVectorItem* QVectorMgr::Add(QColor clr, QString str)
{
	QVectorItem* item = new QVectorItem;
	item->m_Color = clr;
	item->m_strCaption = str;
	item->m_nTag = Count();
	m_lstVector.append(item);
	return item;
}

void QVectorMgr::Connect(QObject* receiver, const char* member)
{
// 	m_pSig->connect(receiver, member);

// 	connect(this,SIGNAL(m_pSig()),receiver,SLOT());    //??????
}
