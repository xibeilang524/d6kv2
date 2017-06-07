#include <QTableWidget>
#include <QTableWidgetItem>

#include <QTabWidget>
#include <QMenu>

// #include "constutils.h"
#include "globaldef.h"
#include "valueformimpl.h"
#include "selectdetailimpl.h"



Dlg_value_impl::Dlg_value_impl(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f) 
{ 
	setupUi(this);

	init(this); 
}
    
void Dlg_value_impl::init(QWidget *parent)
{
	m_tblAnalog->setContextMenuPolicy(Qt::CustomContextMenu);
	m_tblDigtal->setContextMenuPolicy(Qt::CustomContextMenu);
	menu_ana_pop = new QMenu(m_tblAnalog);
	menu_dig_pop = new QMenu(m_tblDigtal);

	action_name = new QAction(_C2Q("名称"),parent);
	action_name->setObjectName(QString::fromUtf8("action_name"));
	action_name->setChecked(true);
	action_name->setEnabled(false);
	connect(action_name,SIGNAL(activated()),this,SLOT(slot_ana_name));

	action_select_ana_column = new QAction(_C2Q("选择列..."),parent);
	action_select_ana_column->setObjectName(QString::fromUtf8("action_select_ana_column"));
	connect(action_select_ana_column,SIGNAL(activated()),this,SLOT(slot_ana_select_column));

	action_signal = new QAction(_C2Q("信号"),parent);
	action_signal->setObjectName(QString::fromUtf8("action_signal"));
	action_signal->setChecked(true);
	action_signal->setEnabled(false);
	connect(action_signal,SIGNAL(activated()),this,SLOT(slot_signal));

	action_cursor = new QAction(_C2Q("游标基准"),parent);
	action_cursor->setObjectName(QString::fromUtf8("action_cursor"));
	action_cursor->setChecked(true);
	connect(action_cursor,SIGNAL(activated()),this,SLOT(slot_cursor));

	action_hide = new QAction(_C2Q("隐藏无变位通道"),parent);
	action_hide->setObjectName(QString::fromUtf8("action_hide"));
	action_hide->setChecked(false);
	connect(action_hide,SIGNAL(activated()),this,SLOT(slot_hide));

	action_select_dig_column = new QAction(_C2Q("选择列..."),parent);
	action_select_dig_column->setObjectName(QString::fromUtf8("action_select_dig_column"));
	connect(action_select_dig_column,SIGNAL(activated()),this,SLOT(slot_select_dig_column));

	m_pAnalyze = NULL;
	connect(m_tblAnalog, SIGNAL(customContextMenuRequested(const QPoint&)), 
						 SLOT(slot_analog_context_menu(const QPoint&)));
	connect(m_tblDigtal, SIGNAL(customContextMenuRequested(const QPoint&)), 
						 SLOT(slot_digtal_context_menu(const QPoint&)));
}

Dlg_value_impl::~Dlg_value_impl()
{
}

void Dlg_value_impl::set_analyze(QAnalyze* fun)
{
	if (m_pAnalyze != NULL)
		m_pAnalyze->SetValuePage(m_tabInfo->currentIndex());

	if (m_pAnalyze != fun)
	{
		m_pAnalyze = fun;
		if (fun == NULL)
			return;

		m_tblAnalog->setColumnCount(0);
		m_tblAnalog->setRowCount(0);
		m_tblDigtal->setColumnCount(0);
		m_tblDigtal->setRowCount(0);

		update_table(m_tblAnalog, m_pAnalyze->Details(0));
		fresh_table(m_tblAnalog, m_pAnalyze->AnaChannels());

		update_table(m_tblDigtal, m_pAnalyze->Details(1));
		fresh_table(m_tblDigtal, m_pAnalyze->DigChannels());
	}
}

QTableWidget* Dlg_value_impl::get_table(int index)
{
	switch (index)
	{
	case 0:
		return m_tblAnalog;
		break;
	case 1:
		return m_tblDigtal;
		break;
	default:
		return NULL;
		break;
	}
}

void Dlg_value_impl::slot_analog_context_menu(const QPoint& pos)
{
	menu_ana_pop->clear();
	menu_ana_pop->addAction(action_name);
	menu_ana_pop->addSeparator();
	menu_ana_pop->addAction(action_select_ana_column);
	menu_ana_pop->exec(QCursor::pos());
}

void Dlg_value_impl::slot_digtal_context_menu(const QPoint& pos)
{
	menu_dig_pop->clear();
	menu_dig_pop->addAction(action_signal);
	menu_dig_pop->addAction(action_cursor);
	menu_dig_pop->addAction(action_hide);
	menu_dig_pop->addSeparator();
	menu_dig_pop->addAction(action_select_dig_column);
	menu_dig_pop->exec(QCursor::pos());
}

// 设置无变位通道的显隐
void Dlg_value_impl::update_dig_table()
{
	m_pAnalyze->SetShowNoSwitch(! m_pAnalyze->ShowNoSwitch());
	int n = m_pAnalyze->comtrade()->DigChannelNum();
	bool* args = new bool[n];

	if (m_pAnalyze->ShowNoSwitch())
		memset(args, 1, n * sizeof(bool));
	else
		m_pAnalyze->GetSwitchInfo(n, args);

	for (int i = 0; i < n; i++)
	{
		if (args[i])
			m_tblDigtal->showRow(i);
		else
			m_tblDigtal->hideRow(i);
	}
}

void Dlg_value_impl::update_table(QTableWidget* tab, QDetails* dets)
{
	tab->setColumnCount(dets->CheckCount());
	QTableWidgetItem* header = tab->horizontalHeaderItem(0);
	if (header != NULL)
	{
		header->setText(dets->Item(0)->Caption());
	}
	else
	{
		header = new QTableWidgetItem(dets->Item(0)->Caption());
		tab->setHorizontalHeaderItem(0,header);
	}

	int col = 1;
	for (int i = 1; i < dets->Count(); i++)
	{
		if (! dets->Item(i)->Check())
			continue;
		header = tab->horizontalHeaderItem(col);
		if (header != NULL)
		{
			header->setText(dets->Item(i)->Caption());
		}
		else
		{
			header = new QTableWidgetItem(dets->Item(i)->Caption());
			tab->setHorizontalHeaderItem(col,header);
		}

		int idx = 0;
		if (tab == m_tblAnalog)
		{
			switch (dets->Item(i)->Data())
			{
			case 1:
			case 4:
			case 5:
			case 6:
				idx = 0;
				break;
			case 3:
				idx = 2;
				break;
			default:
				idx = 1;
				break;
			}
		}
		col++;
		// 需要表格自动调整width
	}
	if (tab->columnCount() < 2)
	{
		tab->setColumnCount(2);
	}
}

void Dlg_value_impl::fresh_table(QTableWidget* tab, Channels* channels)
{
	tab->setRowCount(channels->count());
	
	for (int i = 0; i < channels->count(); i++)
	{
		QTableWidgetItem* header = tab->verticalHeaderItem(i);
		if (header != NULL)
		{
			header->setText(channels->channel(i)->channel_name());
		}
		else
		{
			header = new QTableWidgetItem(channels->channel(i)->channel_name());
			tab->setVerticalHeaderItem(i,header);
		}
	}
	if (tab = m_tblAnalog)
		tab->resizeColumnsToContents();
// 	if (tab->rowCount() < 2)
// 		tab->setRowCount(2);
}

void Dlg_value_impl::Fresh(int pos)
{
	if (m_pAnalyze == NULL)
		return;

	switch (m_tabInfo->currentIndex())
	{
	case 0:
		fresh_analog(pos);
		break;
	case 1:
		fresh_digtal(pos);
		break;
	default:
		break;
	}
}

void Dlg_value_impl::fresh_analog(int pos)
{
	m_tblAnalog->setUpdatesEnabled(false);
	int row = 0;
	for (int i = 0; i < m_pAnalyze->AnaChannels()->count(); i++)
	{
		int col = 0;
		QDetails* dets = m_pAnalyze->Details(0);
		for (int j = 0; j < dets->Count(); j++)
		{
			QDetail* item = dets->Item(j);
			if (! item->Check())
				continue;
			QString str = m_pAnalyze->CalAnaData(i, pos, item->Data());
			QTableWidgetItem* tableItem = m_tblAnalog->item(row,col);
			if (tableItem == NULL)
			{
				tableItem = new QTableWidgetItem(str);
				m_tblAnalog->setItem(row,col,tableItem);
			}
			else
			{
				tableItem->setText( str);
			}
			col++;
		}
		row++;
	}
	m_tblAnalog->setUpdatesEnabled(true);
	m_tblAnalog->resizeColumnsToContents();
	m_tblAnalog->repaint();
}

/*
  刷新开关量的变位情况
  开关量显示：
  游标位置之前，根据设置来确定是否显示
  显示全部变位信息时，时标以数据起始时间为基准;过滤时，时标以当前游标位置为起始
*/
void Dlg_value_impl::fresh_digtal(int pos)
{
	int nDig = m_pAnalyze->comtrade()->DigChannelNum();
	bool* args = new bool[nDig];
	QTableWidgetItem *item;

	if (m_pAnalyze->ShowNoSwitch())
		memset(args, 1, nDig * sizeof(bool));
	else
		m_pAnalyze->GetSwitchInfo(nDig, args);

	int row = -1;
	for (int i = 0; i < m_pAnalyze->DigChannels()->count(); i++)
	{
		QString str;
		int col = 0;
		if (! args[i]) continue;
		row++;
		QDetails* dets = m_pAnalyze->Details(1);
		for (int j = 0; j < dets->Count(); j++)
		{
			if (! dets->Item(j)->Check())
				continue;
			str = m_pAnalyze->CalDigData(i, pos, j);
			item = new QTableWidgetItem(str);
			m_tblDigtal->setItem(row,col,item);
// 			item = m_tblDigtal->item(row,col);
// 			item->setText( str);
// 			m_tblDigtal->setText(row, col, str);
			col++;
		}
	}
	m_tblDigtal->resizeColumnsToContents();

	delete args;
}

// 显示所有的开关量变位信息
void Dlg_value_impl::show_all_switch()
{
	m_pAnalyze->SetShowAllSwitch(! m_pAnalyze->ShowAllSwitch());
	fresh_digtal(m_pAnalyze->CursorPos());
}

// 暂不实现被选择通道的高亮度显示功能
void Dlg_value_impl::update_grid_color(QColor* args)
{
/*
	QTable* tab = m_tblAnalog;
	for (int i = 0; i < tab->rowCount(); i++)
	{
	}
*/
}

void Dlg_value_impl::slot_ana_name( void )
{

}

void Dlg_value_impl::slot_ana_select_column( void )
{
	QDlgDetailsImpl dlg(m_pAnalyze->Details(0), NULL, true);
	dlg.SetItemValue(m_pAnalyze->AverCount());
	dlg.SetValueCaption(AVERAGESTR);
	if (dlg.exec() == QDialog::Accepted)
	{
		update_table(get_table(0), m_pAnalyze->Details(0));
		m_pAnalyze->SetAverCount(dlg.ItemValue());// 平均值点数
	}
	Fresh(m_pAnalyze->CursorPos());

}

void Dlg_value_impl::slot_signal( void )
{

}

void Dlg_value_impl::slot_cursor( void )
{
	show_all_switch();
}

void Dlg_value_impl::slot_hide( void )
{
	update_dig_table();
}

void Dlg_value_impl::slot_select_dig_column( void )
{
	QDlgDetailsImpl dlg(m_pAnalyze->Details(1), NULL);
	if (dlg.exec() == QDialog::Accepted)
	{
		m_tblDigtal->setColumnCount(0);
		m_tblDigtal->setRowCount(0);
		update_table(m_tblDigtal, m_pAnalyze->Details(1));
		fresh_table(m_tblDigtal, m_pAnalyze->DigChannels());
	}
	Fresh(m_pAnalyze->CursorPos());

}
