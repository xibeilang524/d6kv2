#include "channelunit.h"

Channels::Channels()
{
// 	m_lstData = new  QList<IViewData>;
// 	m_lstCheck = new  QList<IViewData>;
}

Channels::~Channels()
{
// 	delete m_lstData;
// 	delete m_lstCheck;
}

void Channels::init_by_comtrade(Comtrade* obj, bool ana)
{
	m_lst_data.clear();
	if (ana)
	{
		QList<Analog*> pList = obj->AnaChannels();
		QList<Analog*>::iterator it;
		for (it=pList.begin(); it!=pList.end();++it)
		{
			Channel* channel = *it;
			m_lst_data.append(channel);
		}
	}
	else
	{
		QList<Digtal*> pList = obj->DigChannels();
		QList<Digtal*>::iterator it;
		for (it=pList.begin(); it!=pList.end();++it)
		{
			Channel* channel = *it;
			m_lst_data.append(channel);
		}
	}
	reset_check_list();
}

void Channels::clear()
{
	m_lst_data.clear();
	m_lst_check.clear();
}

int Channels::count()
{
	return m_lst_data.count();
}

int Channels::check_count()
{
	return m_lst_check.count();
}

View_data* Channels::channel(int index)
{
	if (index < 0 || (uint)index >= m_lst_data.count())
		return NULL;
	return m_lst_data.at(index);
}

bool Channels::checked(int index)
{
	return m_lst_check.indexOf(m_lst_data.at(index)) > -1;
}

void Channels::set_checked(int index, bool bCheck)
{
	if (index < 0 || (uint)index >= m_lst_data.count())
		return;

	View_data* pData = m_lst_data.at(index);
	int idx = m_lst_check.indexOf(pData);
	if (bCheck)
	{
		if (idx < 0)
			m_lst_check.append(pData);
	}
	else
	{
		if (idx > -1)
			m_lst_check.removeAt(idx);
	}
}

void Channels::check_all(bool bCheck)
{
	m_lst_check.clear();
	if (! bCheck)
	{
		return;
	}
	QList<View_data*>::iterator it;
	for (it=m_lst_data.begin();it!=m_lst_data.end();++it)
	{
		m_lst_check.append(*it);
	}
}

void Channels::add(Channel* obj)
{
	m_lst_data.append(obj);
}

void Channels::insert(int index, Channel* obj)
{
	m_lst_data.insert(index, obj);
}

void Channels::Delete(int index)
{
	m_lst_data.removeAt(index);
}

// 获得指定通道号的通道位置
int Channels::Indexof(int channelNo)
{
	int n = -1;

	QList<View_data*>::iterator it;
	for (it=m_lst_data.begin();it!=m_lst_data.end();++it)
	{
		if ((*it)->channel_no() == channelNo)
		{
			n = m_lst_data.indexOf(*it);
			break;
		}
	}
	return n;
}

// 检查CheckList中是否存在已经被删除的通道
void Channels::reset_check_list()
{
	QList<View_data*>::iterator it;
	for (it=m_lst_check.begin(); it!=m_lst_check.end(); ++it)
	{
		if (m_lst_data.indexOf(*it) < 0)
		{
			m_lst_check.removeOne(*it);
		}
	}
}