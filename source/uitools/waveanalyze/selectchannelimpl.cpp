#include <QListWidgetItem>

#include "selectchannelimpl.h"


Dlg_select_channel_impl::Dlg_select_channel_impl( QWidget* parent,Qt::WindowFlags f)
	: QDialog(parent,f)
{
	setupUi(this);

	m_multi_check_flag = true;

// 	m_lvChannel->addColumn("", 150);
// 	m_lvChannel->setColumnWidthMode(0, QListView::Maximum);
// 	m_lvChannel->header()->hide();
// 	m_lvChannel->setSortColumn(-1);

	connect(m_btnAnalog, SIGNAL(toggled(bool)), this,SLOT(slot_style_clicked(bool)));
	connect(m_btnSelect, SIGNAL(clicked()), SLOT(slot_select_all()));
	connect(m_btnUnSelect, SIGNAL(clicked()), SLOT(slot_unselect()));
	connect(m_btnOk, SIGNAL(clicked()), SLOT(accept()));
	connect(m_btnCancel, SIGNAL(clicked()), SLOT(reject()));
}

Dlg_select_channel_impl::~Dlg_select_channel_impl()
{
}

void Dlg_select_channel_impl::slot_select_all()
{
// 	m_lvChannel->selectAll();
	select_items(true);
}

void Dlg_select_channel_impl::slot_unselect()
{
// 	m_lvChannel->clearSelection();
	select_items(false);
}

void Dlg_select_channel_impl::select_items(bool b)
{
	QListWidgetItem* item=NULL;
	Qt::CheckState state = Qt::Unchecked;
	if (b)
	{
		state = Qt::Checked;
	}

	for (int i=0;i<m_lvChannel->count();i++)
	{
		item = m_lvChannel->item(i);
		item->setCheckState(state);
	}
// 	QListViewItemIterator it(m_lvChannel);
// 	QCheckListItem* item = NULL;
// 	while ((item = (QCheckListItem*)it.current()) != NULL)
// 	{
// 		item->setOn(b);
// 		++it;
// 	}
}

void Dlg_select_channel_impl::set_state(bool checked)
{
	QStringList* lst = NULL;
	if (checked)
		lst = &m_analog_list;
	else
		lst = &m_digtal_list;
	m_lvChannel->clear();
	QListWidgetItem* item = NULL;
	for (QStringList::Iterator it = lst->begin(); it != lst->end(); ++it)
	{
		item = new QListWidgetItem(*it,m_lvChannel );
		item->setCheckState(Qt::Unchecked);
		m_lvChannel->addItem(item);
	}
}

void Dlg_select_channel_impl::slot_style_clicked(bool Checked)
{
	set_state(Checked);
}

void Dlg_select_channel_impl::set_multi_check(bool b)
{
	m_multi_check_flag = b;
	if (! m_multi_check_flag)
	{
		m_btnSelect->setEnabled(false);
		m_btnUnSelect->setEnabled(false);

		connect(m_lvChannel, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this,
							 SLOT(slot_on_item(QListWidgetItem*,QListWidgetItem*)));
// 		connect(m_lvChannel, SIGNAL(pressed(QListViewItem*)), 
// 							 SLOT(OnItem(QListViewItem*)));
// 		connect(m_lvChannel, SIGNAL(spacePressed(QListViewItem*)), 
// 							 SLOT(OnItem(QListViewItem*)));
	}
}

void Dlg_select_channel_impl::slot_on_item(QListWidgetItem* current,QListWidgetItem* previous)
{
//     QListViewItemIterator it(m_lvChannel);
// 	QListWidgetItem* item1 = NULL;
// 	while ((item1 = (QListWidgetItem*)it.current()) != NULL)
// 	{
// 		item1->setOn(false);
// 		++it;
// 	}
// 	((QListWidgetItem*)item)->setOn(true);
	previous->setCheckState(Qt::Unchecked);
	current->setCheckState(Qt::Checked);
}

bool Dlg_select_channel_impl::Checked(int index)
{
// 	int n = -1;
//     QListViewItemIterator it(m_lvChannel);
// 	QListWidgetItem* item = NULL;
// 	while ((item = (QListWidgetItem*)it.current()) != NULL)
// 	{
// 		if (++n == index)
// 			break;
// 		++it;
// 	}
// 	if (n == -1 || n == m_lvChannel->childCount())
// 		return false;
// 	return item->isOn();
	QListWidgetItem* item = m_lvChannel->item(index);
	if (item != NULL)
	{
		item->setCheckState(Qt::Checked);
		return true;
	}
	return false;
}

// 唯一Checked，仅在MultiCheck=False时有效
int Dlg_select_channel_impl::only_check_index()
{
// 	if (m_bMultiCheck)
// 		return -1;
// 
// 	int index = 0;
//     QListViewItemIterator it(m_lvChannel);
// 	QListWidgetItem* item = NULL;
// 	while ((item = (QListWidgetItem*)it.current()) != NULL)
// 	{
// 		if (item->isOn())
// 			return index;
// 		index++; 
// 		++it;
// 	}
// 	return -1;

	if (m_multi_check_flag)
		return -1;

	int index = 0;
	QListWidgetItem* item = NULL;
	for (int i=0;i<m_lvChannel->count();i++)
	{
		item = m_lvChannel->item(i);
		if (item->checkState()==Qt::Checked)
		{
			return i;
		}
	}
	return -1;
}

void Dlg_select_channel_impl::set_only_analog(int val)
{
	bool b = val == 0 ? true : false;
	m_btnAnalog->setChecked(b);
	m_btnDigtal->setEnabled(! b);
}

int Dlg_select_channel_impl::Count()
{
// 	return m_lvChannel->childCount();
	return m_lvChannel->count();
}

int Dlg_select_channel_impl::check_count()
{
// 	int sum = 0;
//     QListViewItemIterator it(m_lvChannel);
// 	QListWidgetItem* item = NULL;
// 	while ((item = (QListWidgetItem*)it.current()) != NULL)
// 	{
// 		if (item->isOn())
// 			sum++;
// 		++it;
// 	}
// 	return sum;

	int sum=0;
	QListWidgetItem *item = NULL;
	for (int i=0;i<m_lvChannel->count();i++)
	{
		item = m_lvChannel->item(i);
		if (item->checkState()==Qt::Checked)
		{
			sum++;
		}
	}
	return sum;
}

void Dlg_select_channel_impl::add_item(const QString& str, bool b)
{
	if (b)
	{
		m_analog_list.append(str);
	}
	else
	{
		m_digtal_list.append(str);
	}
}