#include <QPixmap>
#include <QCursor>
#include <QListWidget>
#include <QColorDialog>

#include "wavepix.h"
#include "manageutils.h"
#include "channelpropertyimpl.h"
#include "globaldef.h"

Channel_property_impl::Channel_property_impl(Wave_graph* g, Comtrade* d, QWidget* parent)
	: QDialog(parent)
{
	setupUi(this);
	m_pGraph = g;
	m_pData = d;
	m_lbxChannel->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_lbxChannel->setContextMenuPolicy(Qt::CustomContextMenu);

	create_actions();
	fresh_list();
	connect(m_btnClose, SIGNAL(clicked()), SLOT(accept()));
	connect(m_lbxChannel, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), 
						  SLOT(slot_select(QListWidgetItem*)));	 
	connect(m_lbxChannel, SIGNAL(customContextMenuRequested(const QPoint&)), 
						  SLOT(slot_channel_menu(const QPoint&)));
	connect(m_lbxChannel, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
						  SLOT(slot_locate_channel(QListWidgetItem*)));
	connect(m_lbxChannel, SIGNAL(itemSelectionChanged()), 
						  SLOT(slot_check_select()));
	connect(m_lbxChannel, SIGNAL(itemChanged()), 
		SLOT(slot_check_select()));

	connect(m_edtChannelName, SIGNAL(returnPressed()), SLOT(slot_caption_change()));
	connect(m_edtChannelName, SIGNAL(lostFocus()), SLOT(slot_caption_change()));
	connect(m_edtChannelScale, SIGNAL(returnPressed()), SLOT(slot_set_position()));
	connect(m_edtChannelScale, SIGNAL(lostFocus()), SLOT(slot_set_position()));
	connect(m_chbChannelVisible, SIGNAL(clicked()), SLOT(slot_check_visible()));
	connect(m_btnAutoScale, SIGNAL(clicked()), SLOT(slot_auto_size()));
	connect(m_btnZoomOut, SIGNAL(clicked()), SLOT(slot_zoom_out()));
	connect(m_btnZoomIn, SIGNAL(clicked()), SLOT(slot_zoom_in()));
	connect(m_btnAutoPosition, SIGNAL(clicked()), SLOT(slot_auto_position()));
	connect(m_btnChannelColor, SIGNAL(clicked()), SLOT(slot_channel_color()));
	connect(m_btnDefault, SIGNAL(clicked()), SLOT(slot_resume_defualt()));
	update_list_checkstate();
}

Channel_property_impl::~Channel_property_impl()
{
}

void Channel_property_impl::create_actions()
{
	m_pop_menu = new QMenu(m_lbxChannel);

	action_select_all = new QAction(_C2Q("全选"),this);
	action_select_all->setObjectName(QString::fromUtf8("action_select_all"));
	connect(action_select_all,SIGNAL(triggered(bool)),this,SLOT(slot_select_all(bool)));

	action_analog_chennel = new QAction(_C2Q("模拟量通道"),this);
	action_analog_chennel->setObjectName(QString::fromUtf8("action_analog_chennel"));
	connect(action_analog_chennel,SIGNAL(triggered(bool)),this,SLOT(slot_select_analog_channel()));

	action_digtal_channel = new QAction(_C2Q("开关量通道"),this);
	action_digtal_channel->setObjectName(QString::fromUtf8("action_digtal_channel"));
	connect(action_digtal_channel,SIGNAL(triggered(bool)),this,SLOT(slot_select_digtal_channel()));

	action_channel_A = new QAction(_C2Q("A相通道"),this);
	action_channel_A->setObjectName(QString::fromUtf8("action_channel_A"));
	connect(action_channel_A,SIGNAL(triggered(bool)),this,SLOT(slot_select_A_channel()));

	action_channel_B = new QAction(_C2Q("B相通道"),this);
	action_channel_B->setObjectName(QString::fromUtf8("action_channel_B"));
	connect(action_channel_B,SIGNAL(triggered(bool)),this,SLOT(slot_select_B_channel()));

	action_channel_C = new QAction(_C2Q("C相通道"),this);
	action_channel_C->setObjectName(QString::fromUtf8("action_channel_C"));
	connect(action_channel_C,SIGNAL(triggered(bool)),this,SLOT(slot_select_C_channel()));

}

void Channel_property_impl::slot_channel_menu(const QPoint& pos)
{
	m_pop_menu->clear();
	m_pop_menu->addAction(action_select_all);
	m_pop_menu->addSeparator();
	m_pop_menu->addAction(action_analog_chennel);
	m_pop_menu->addAction(action_digtal_channel);
	m_pop_menu->addSeparator();
	m_pop_menu->addAction(action_channel_A);
	m_pop_menu->addAction(action_channel_B);
	m_pop_menu->addAction(action_channel_C);
	m_pop_menu->exec(QCursor::pos());
}

void Channel_property_impl::unselect_digtal()
{
// 	for (uint i = m_pGraph->AnaCount(); i < m_lbxChannel->count(); i++)
// 		m_lbxChannel->setSelected(i, false);
	m_lbxChannel->clearSelection();
}

void Channel_property_impl::select_channel(int idx)
{
	QListWidgetItem* item ;
	if (idx == 0)			// 全选
	{
		m_lbxChannel->selectAll();
	}
	else if (idx == 1)		// 反选
	{
/*
		for (uint i = 0; i < m_lbxChannel->count(); i++)
			m_lbxChannel->setSelected(i, m_lbxChannel->isSelected(i));
*/
	}
	else if (idx == 2)		// 模拟量
	{
		for (uint i = 0; i < m_lbxChannel->count(); i++)
		{
			item = m_lbxChannel->item(i);
			item->setSelected(i<m_pGraph->analog_count());
		}
	}
	else if (idx == 3)		// 开关量
	{
		for (uint i = 0; i < m_lbxChannel->count(); i++)
		{
			item = m_lbxChannel->item(i);
			item->setSelected(i>=m_pGraph->analog_count());
		}
	}
	else if (idx > 3 && idx < 7)		// A、B、C
	{
		unselect_digtal();
		for (int i = 0; i < m_pGraph->analog_count(); i++)
		{
			bool b = (int)m_pData->channel_phase(i) == idx - 4;
// 			m_lbxChannel->setSelected(i, b);
			item = m_lbxChannel->item(i);
			item->setSelected(b);
		}
	}
}

void Channel_property_impl::select_item(int idx)
{
	if (idx < 0 || (uint)idx >= m_lbxChannel->count())
		return;
	
	Draw_channel* channel = m_pGraph->channels(idx);
	int analog_count = m_pGraph->analog_count();

	View_data* view = m_pData->channel(idx);
	if (idx<analog_count)
	{
		m_cbbChannelPhase->setCurrentIndex(view->channel_phase());
		m_cbbChannelUnit->setText(view->get_channel_unit());
		m_cbbChannelProperty->setCurrentIndex(0);
	}
	else
	{
		m_cbbChannelPhase->setCurrentIndex(-1);
		m_cbbChannelUnit->setText("");
		m_cbbChannelProperty->setCurrentIndex(1);
	}
	m_edtChannelName->setText(channel->caption());
	m_edtChannelPosition->setText(QString::number(channel->position()));
	if (idx < m_pGraph->analog_count())
		m_edtChannelScale->setText(QString::number(((Draw_analog*)channel)->Yref()));
	else
		m_edtChannelScale->setText("1");
	m_btnChannelColor->setStyleSheet(QString("background-color:%1").arg(channel->color().name()));

	m_chbChannelVisible->setChecked(channel->visible());
	update_details();
}

void Channel_property_impl::slot_select(QListWidgetItem* item)
{
	if (item == NULL)
		return;

	int channelIndex = m_lbxChannel->row(item);
	select_item(channelIndex);
}

int Channel_property_impl::selected_count()
{
	int col = 0;
	QListWidgetItem* item ;
	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		item = m_lbxChannel->item(i);
		if (item->isSelected())
		{
			col++;
		}
	}
	return col;
}

// 更新详细信息控件
void Channel_property_impl::update_details()
{
	bool anaSelected = false;
	bool digSelected = false;

	if (selected_count() > 1)
		m_edtChannelName->setEnabled(false);// 多个通道被选择时，名称禁止使用

	int i;
	for (i = 0; i < m_pGraph->analog_count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (item->isSelected())
		{
			anaSelected = true;
			break;
		}
	}
	for (i = m_pGraph->analog_count(); i < m_pGraph->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (item->isSelected())
		{
			digSelected = true;
			break;
		}
	}

	if (digSelected)
	{
		set_controls(false);					// 开关量选择，比例禁止使用
	}
}

void Channel_property_impl::set_controls(bool enable)
{
	m_edtChannelScale->setEnabled(enable);
	m_btnZoomIn->setEnabled(enable);
	m_btnZoomOut->setEnabled(enable);
	m_btnAutoScale->setEnabled(enable);
}

void Channel_property_impl::fresh_list()
{
	m_lbxChannel->clear();

	for (int i = 0; i < m_pData->Count(); i++)
	{
		View_data* view = m_pData->channel(i);
		Draw_channel* channel = m_pGraph->channels(i);
		QListWidgetItem* item;
		if (channel->visible())
		{
			item = new QListWidgetItem(view->channel_name());
			item->setCheckState(Qt::Checked);
		}
		else
		{
			item = new QListWidgetItem(view->channel_name());
			item->setCheckState(Qt::Unchecked);
		}
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
		m_lbxChannel->addItem(item);
	}
}

// 设置显隐不进行位置重排 
void Channel_property_impl::slot_check_visible()
{
	m_pGraph->lock();

	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (!item->isSelected())
			continue;
		m_pGraph->channels(i)->set_visible(m_chbChannelVisible->isChecked());
		item->setCheckState(m_chbChannelVisible->isChecked()?Qt::Checked:Qt::Unchecked);
	}

	m_pGraph->unlock();
	m_pGraph->show_channel();
}

// 自动设置位置
void Channel_property_impl::slot_auto_position()
{
	m_pGraph->lock();

	int count = 0;
	Draw_channel* channel = NULL;
	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (! item->isSelected())
			continue;

		m_pGraph->auto_position(i);
		channel = m_pGraph->channels(i);
		count++;
	}
	if (count == 1 && channel != NULL)
		m_edtChannelPosition->setText(QString::number(channel->position()));
	
	m_pGraph->unlock();
}

void Channel_property_impl::slot_zoom_in()
{
	zoom(true);
}

void Channel_property_impl::slot_zoom_out()
{
	zoom(false);
}

void Channel_property_impl::zoom(bool enlarge)
{
	double f = enlarge ? 2.0 : 0.5;
	int count = 0;

	m_pGraph->lock();

	Draw_analog* ana = NULL;
	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (! item->isSelected() || (int)i >= m_pGraph->analog_count())
			continue;

		ana = (Draw_analog*)m_pGraph->channels(i);
		ana->set_Yref(ana->Yref() * f);
		count++;
	}

	if (count == 1 && ana != NULL)
		m_edtChannelScale->setText(QString::number(ana->Yref()));

	m_pGraph->unlock();
}

// 自动设置比例
void Channel_property_impl::slot_auto_size()
{
	m_pGraph->lock();

	int count = 0;
	Draw_analog* ana = NULL;
	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (! item->isSelected() || (int)i >= m_pGraph->analog_count())
			continue;

		ana = (Draw_analog*)m_pGraph->channels(i);
		ana->set_auto_size(true);
		ana->set_auto_size(false);
		count++;
	}
	if (count == 1 && ana != NULL)
		m_edtChannelScale->setText(QString::number(ana->Yref()));

	m_pGraph->unlock();
}

void Channel_property_impl::slot_channel_color()
{
// 	QColor clr = m_btnChannelColor->paletteBackgroundColor();
	QPalette pal = m_btnChannelColor->palette();
	QColor clr = pal.color(m_btnChannelColor->backgroundRole());
	clr = QColorDialog::getColor(clr, this, "hey!");
	if (clr.isValid())
	{
		m_pGraph->lock();
		for (uint i = 0; i < m_lbxChannel->count(); i++)
		{
			QListWidgetItem* item = m_lbxChannel->item(i);
			if (! item->isSelected())
				continue;
			m_pGraph->channels(i)->set_color(clr);
		}
		m_pGraph->unlock();
// 		QPalette palette;
// 		palette.setColor(m_btnChannelColor->backgroundRole(),clr);
// 		m_btnChannelColor->setPalette(palette);
		m_btnChannelColor->setStyleSheet(QString("background-color:%1").arg(clr.name()));
	}
}

void Channel_property_impl::slot_caption_change()
{
	if (m_edtChannelName->text().isEmpty())
		return;

	m_pGraph->channels(m_lbxChannel->currentRow())->set_caption(m_edtChannelName->text());
}
	
// 若执行TWaveGraph.AllocRect,则位置会重新排列 
void Channel_property_impl::slot_set_position()
{
	m_pGraph->lock();

	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (! item->isSelected())
			continue;

		m_pGraph->channels(i)->set_position(m_edtChannelPosition->text().toInt());
	}
	m_pGraph->render();

	m_pGraph->unlock();
}

// 恢复原始比例和位置
void Channel_property_impl::slot_resume_defualt()
{
	m_pGraph->alloc_rect();
	m_pGraph->render();
}

// 双击某通道自动定位到该通道
void Channel_property_impl::slot_locate_channel(QListWidgetItem* item)
{
	if (item == NULL)
		return;

	int channelIndex = m_lbxChannel->row(item);
	if (m_pGraph->locate_channel(channelIndex))
		m_edtChannelPosition->setText(
			QString::number(m_pGraph->channels(channelIndex)->position()));
	else
		m_chbChannelVisible->setChecked(! m_chbChannelVisible->isChecked());
}

Wave_caption* Channel_property_impl::wave_captions()
{
	return g_GraphMgr->Caption(g_GraphMgr->IndexOf(m_pGraph));
}

// 根据通道的选择刷新ListBox的选择
void Channel_property_impl::update_list_checkstate()
{
	if (m_pGraph == NULL)
		return;
	for (int i = 0; i < m_pGraph->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		item->setSelected(m_pGraph->checked(i));
	}
}

void Channel_property_impl::slot_check_select()
{
	Wave_caption* c = wave_captions();
	for (uint i = 0; i < m_lbxChannel->count(); i++)
	{
		QListWidgetItem* item = m_lbxChannel->item(i);
		if (item->isSelected())
		{
			c->AddSelected(m_pGraph->channels(i));
		}
		else
		{
			c->DeleteSelected(m_pGraph->channels(i));
		}
	}
}

void Channel_property_impl::slot_select_all(bool b)
{
	select_channel(0);
}

void Channel_property_impl::slot_select_analog_channel()
{
	select_channel(2);
}

void Channel_property_impl::slot_select_digtal_channel()
{
	select_channel(3);
}

void Channel_property_impl::slot_select_A_channel()
{
	select_channel(4);
}

void Channel_property_impl::slot_select_B_channel()
{
	select_channel(5);
}

void Channel_property_impl::slot_select_C_channel()
{
	select_channel(6);
}
