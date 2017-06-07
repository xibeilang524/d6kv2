#include <QMouseEvent>
#include <QApplication>
#include <math.h>
#include <qfontmetrics.h>

#include "wavecaption.h"

Wave_caption::Wave_caption(QWidget* parent,  Qt::WindowFlags f  )
	:QWidget(parent, f)
{
	// active mouse move event, even if no mouse button is pressed.
	setMouseTracking(true);
	// 配合KeyPressEvent，将键盘输入信息传递给Graph
	setFocusPolicy(Qt::WheelFocus);

	m_pGraph = NULL;
	m_Paint = new QPainter(this);
	m_bLeftPressed = false;
	m_Color = Qt::black;
// 	m_lstSelection = new  QList<QDrawChannel>;
	m_pMouseLine = new Mouse_line(this);
	m_nLockCnt = 0;
	m_current_position = 0;
	resize(200,size().height()); //初始化宽度为200
	Clear();
	SetBackgroundColor(m_Color);
	connect(m_pMouseLine, SIGNAL(OnUp(QRect)), SLOT(slot_elastic_up(QRect)));
}

Wave_caption::~Wave_caption()
{
// 	delete m_lstSelection;
	delete m_pMouseLine;
	delete m_Paint;
}

void Wave_caption::Clear()
{
	m_current_position = 0;
	m_pChannel = NULL;
	m_OperateMode = omNone;
	m_lstSelection.clear();
}

Draw_channel* Wave_caption::get_selected_by_pos(int x, int y)
{
	if (m_pGraph == NULL) return NULL;
	for (int i = 0; i < m_pGraph->count(); i++)
	{
		Draw_channel* p = m_pGraph->channels(i);
		if (p->hit_test(QPoint(x, y)) == 1) return p;
	}
	return NULL;
}

void Wave_caption::mousePressEvent( QMouseEvent* e )
{
	m_nMousePointsMove = 0;
	int rx, ry;
//     rx = e->pos().x();
//     ry = e->pos().y();
	rx = e->x(); 
	ry = e->y();
	m_pChannel = get_selected_by_pos(rx, ry);
	m_bLeftPressed = e->button() == Qt::LeftButton;
	bool keyFlag = (e->modifiers() & Qt::ControlModifier) || (e->modifiers() & Qt::ShiftModifier);
	switch (m_OperateMode)
	{
	case omNone:
		if (!keyFlag) UnSelectAll();
		if (m_bLeftPressed)
		{
			SetOperateMove(omMouseLine);
			m_pMouseLine->DownMouse(rx, ry);
		}
		break;
	case omSelect:
		if (e->modifiers() & Qt::ControlModifier)
		{
			if (m_pChannel != NULL)
			{
				if (m_pChannel->selected())
					DeleteSelected(m_pChannel);
				else
					AddSelected(m_pChannel);
			}
		}
		else if (e->modifiers() & Qt::ShiftModifier)
		{
			
		}
		else
		{
			if (!(e->button() == Qt::RightButton && m_lstSelection.count() > 1))
			{
				UnSelectAll();
				if (m_pChannel != NULL) 
					AddSelected(m_pChannel);
			}
		}
		break;
	default:
		break;
	}
}

void Wave_caption::keyPressEvent ( QKeyEvent* e )
{
	qApp->sendEvent(m_pGraph, e);
}

void Wave_caption::mouseMoveEvent ( QMouseEvent* e )
{
	if (m_bLeftPressed) m_nMousePointsMove++;
	int rx, ry;
//     rx = e->pos().x();
//     ry = e->pos().y();
	rx = e->x();
	ry = e->y();

	switch (m_OperateMode)
	{
	case omNone:
		m_pChannel = get_selected_by_pos(rx, ry);
		if (m_bLeftPressed)
		{
		}
		else
		{
			if (m_pChannel != NULL)
				SetOperateMove(omSelect);
		}
		break;
	case omSelect:
		if (m_bLeftPressed)
		{
			if (m_nMousePointsMove > MINMOUSEPOINTSMOVE)
				SetOperateMove(omDrag);
		}
		else
		{
			m_pChannel = get_selected_by_pos(rx, ry);
			if (m_pChannel == NULL)
			{
				SetOperateMove(omNone);
			}
			else
			{
				// 检查是否需要弹出通道名称提示
			}
		}
		break;
	case omDrag:
		if (m_bLeftPressed)
			m_pGraph->move_channel(m_pChannel, ry);
		break;
	case omMouseLine:
		if (m_bLeftPressed)
			m_pMouseLine->MoveMouse(rx, ry);
		else
			SetOperateMove(omNone);
		break;
	default:
		break;
	}
}

void Wave_caption::mouseReleaseEvent ( QMouseEvent* )
{
	if (m_OperateMode == omMouseLine)
	{
		m_pMouseLine->UpMouse();
		// 重绘图形
	}
	SetOperateMove(omNone);
	m_bLeftPressed = false;
}

void Wave_caption::mouseDoubleClickEvent ( QMouseEvent* )
{
	if (m_pChannel == NULL) return;
	emit OnSelect(m_pGraph->indexOf(m_pChannel));
}

void Wave_caption::paintEvent( QPaintEvent* e)
{
	QPixmap pm(size());
	if (m_Paint->isActive())
		m_Paint->end();
	QPainter painter(this);
	pm.fill(this,rect().topLeft());
	pm.fill(m_Color);
	painter.begin(&pm);

	painter.fillRect(rect(), QBrush(m_Color));
	if (m_pGraph != NULL)
	{
		for (int i = 0; i < m_pGraph->count(); i++)
		{
			Draw_channel* channel = m_pGraph->channels(i);
			if (!channel->visible()) continue;
			channel->draw_caption(&painter, Lefter(),i,get_current_position());
			channel->draw_pointer(&painter, Lefter());
		}
	}
   painter.end();
//    bitBlt(this, 0, 0, &pm);
   const QPointF point(0,0);
   painter.drawPixmap(point,pm);
}

int	Wave_caption::Lefter()
{
	return width();
}

void Wave_caption::resizeEvent( QResizeEvent* e)
{
	m_pMouseLine->SetRange(QPoint(width(), height()));
	QWidget::resizeEvent(e);
}

void Wave_caption::SetGraph(Wave_graph* graph)
{
	if (m_pGraph != graph)
	{
		m_pGraph = graph;
		Change();
		if (m_pGraph != NULL)
		{
			connect(m_pGraph, SIGNAL(signal_move(int)), SLOT(slot_graph_move(int)));
			connect(m_pGraph, SIGNAL(signal_unselect()), SLOT(slot_graph_unselect()));
			connect(m_pGraph, SIGNAL(signal_add_select(void*)), SLOT(slot_graph_add_select(void*)));
			connect(m_pGraph, SIGNAL(signal_delete(void*)), SLOT(slot_graph_delete(void*)));
		}
	}
}

void Wave_caption::SetOperateMove(Operate_Mode om)
{
	if (m_OperateMode != om)
	{
		m_OperateMode = om;
		UpdateOperateMode();
	}
}

void Wave_caption::UpdateOperateMode()
{
	switch (m_OperateMode)
	{
	case omNone:
		setCursor(Qt::ArrowCursor);
		break;
	case omSelect:
		setCursor(Qt::PointingHandCursor);
		break;
	case omDrag:
		setCursor(Qt::SizeAllCursor);
		break;
	default:
		break;
	}
}

// Lock Design Pattern
void Wave_caption::Lock()
{
	m_nLockCnt++;
	if (m_nLockCnt == 1) SetLocking(false);
}

void Wave_caption::UnLock()
{
	m_nLockCnt--;
	if (m_nLockCnt == 0) SetLocking(true);
}

bool Wave_caption::Locked()
{
	return m_nLockCnt != 0;
}

void Wave_caption::SetLocking(bool updating)
{
	if (updating) Change();
}

void Wave_caption::SetColor(QColor c)
{
	if (m_Color != c)
	{
		m_Color = c;
		Change();
	}
}

void Wave_caption::SetBackgroundColor(QColor c)
{
	if (m_backgroundColor != c)
	{
		m_backgroundColor = c;
		QPalette palette;
		palette.setColor(backgroundRole(),m_backgroundColor);
		setPalette(palette);
		Change();
	}
}

QSize Wave_caption::FontSize(const QString& str)
{
	QFontMetrics fm = fontMetrics();
	return QSize(fm.width(str), fm.height());
}

// 获得合适的名称标识尺寸
int Wave_caption::MaxCaptionSize()
{
	int nSize = 0;
	for (int i = 0; i < m_pGraph->count(); i++)
	{
		Draw_channel* channel = m_pGraph->channels(i);
		if (! channel->visible())
			continue;
		int nLen = FontSize(channel->caption()).width();
		nSize = nSize < nLen ? nLen : nSize;
	}
	nSize += 2 * POINTERHEIGHT + 5;
	return nSize;
}

void Wave_caption::SetAutoSize(bool b)
{
	m_bAutoSize = b;
	if (!m_bAutoSize || m_pGraph == NULL) 
		return;

	resize(QSize(MaxCaptionSize(), height()));
}

void Wave_caption::BeSameScale()
{
	if (m_lstSelection.count() < 2) 
		return;

	Draw_channel* p = m_lstSelection.first();
	if (p->class_name() != DRAWCLASS[1])
		return;

	m_pGraph->lock();

	double yref = ((Draw_analog*)p)->Yref();

	QList<Draw_channel*>::iterator it;
	for (it=m_lstSelection.begin(); it!=m_lstSelection.end();++it)
	{
		if ((*it)->class_name() == DRAWCLASS[1])
		{
			Draw_analog* ana = (Draw_analog*)(*it);
			ana->set_Yref(yref);
		}
	}
	m_pGraph->unlock();
}

void Wave_caption::BeSamePosition()
{
	if (m_lstSelection.count() < 2) 
		return;
	
	int pos = m_lstSelection.first()->position();
	m_pGraph->lock();

	QList<Draw_channel*>::iterator it;
	for (it=m_lstSelection.begin(); it!=m_lstSelection.end();++it)
	{
		if ((*it)->class_name() == DRAWCLASS[1])
		{
			(*it)->set_position(pos);
		}
	}

	m_pGraph->unlock();
}

void Wave_caption::BeAutoPosition()
{
	m_pGraph->lock();

// 	QDrawChannel* p = NULL;
// 	 QListIterator<QDrawChannel> it(*m_lstSelection);
// 	while ((p = it.current()) != 0)
// 	{
// 		++it;
// 		m_pGraph->AutoPosition(p);
// 	}

	QList<Draw_channel*>::iterator it;
	for (it=m_lstSelection.begin(); it!=m_lstSelection.end();++it)
	{
		m_pGraph->auto_position(*it);
	}

	m_pGraph->unlock();
}

void Wave_caption::BeAutoSize()
{
	m_pGraph->lock();

// 	QDrawChannel* p = NULL;
// 	 QListIterator<QDrawChannel> it(*m_lstSelection);
// 	while ((p = it.current()) != 0)
// 	{
// 		++it;
// 		QDrawAnalog* ana = (QDrawAnalog*)p;
// 		ana->SetAutoSize(true);
// 		ana->SetAutoSize(false);
// 	}

	QList<Draw_channel*>::iterator it;
	for (it=m_lstSelection.begin(); it!=m_lstSelection.end();++it)
	{
// 		QDrawAnalog* ana = *it;
		(*it)->set_auto_size(true);
		(*it)->set_auto_size(false);
	}


	m_pGraph->unlock();
}

int	Wave_caption::SelCount()
{
	return m_lstSelection.count();
}

void Wave_caption::UnSelectAll()
{
	Lock();
	for (int i = m_lstSelection.count() - 1; i >= 0; i--)
		DeleteSelected(i);
	m_lstSelection.clear();
	UnLock();
}

void Wave_caption::AddSelected(Draw_channel* channel)
{
	if (m_lstSelection.indexOf(channel) >= 0)
		return;
	channel->set_selected(true);
	m_lstSelection.append(channel);
}

void Wave_caption::DeleteSelected(int channel)
{
	if (channel < 0 || (uint)channel >= m_lstSelection.count()) 
		return;
	Draw_channel* p = m_lstSelection.at(channel);
	if (p != NULL) 
		p->set_selected(false);

	m_lstSelection.removeAt(channel);
}

void Wave_caption::DeleteSelected(Draw_channel* channel)
{
	DeleteSelected(m_lstSelection.indexOf(channel));
}

void Wave_caption::SetChannel(Draw_channel* p)
{
	if (m_pChannel != NULL)
	{
		m_pChannel = p;
		if (m_pGraph != NULL) 
			m_pGraph->set_selected_channel(m_pChannel);
	}
}

void Wave_caption::set_current_position(int pos)
{
	m_current_position = pos;
}

bool Wave_caption::litter_rect(QRect r)
{
	return (fabs((double)(r.bottom() - r.top() < 3))) && 
			(fabs((double)(r.right() - r.left() < 3)));
}

void Wave_caption::CopyGraph(bool b)
{
	//
}

bool Wave_caption::get_selected(int index)
{
	return m_pGraph->is_selected(index);
}

void Wave_caption::slot_elastic_move(QRect rct)
{
}

void Wave_caption::slot_elastic_up(QRect rct)
{
	if (m_pGraph != NULL)
	{
		Lock();
		UnSelectAll();
		for (int i = 0; i < m_pGraph->count(); i++)
		{
			Draw_channel* channel = m_pGraph->channels(i);
			if (channel->caption_rect().top() > rct.top() &&
				channel->caption_rect().bottom() < rct.bottom())
				AddSelected(channel);
		}
		UnLock();
	}
}

void Wave_caption::slot_graph_move(int action)
{
	if (Locked()) return;
	switch (action)
	{
	case 0:
		if (litter_rect(m_pMouseLine->LastRoom()))
			repaint();
		else
		{
			for (int i = 0; i < m_pGraph->count(); i++)
			{
				Draw_channel* channel = m_pGraph->channels(i);
				if (! channel->visible())
					continue;
				QRect rct = channel->draw_rect();
				if (rct.top() >= 0 && rct.bottom() <= height())
				{
					if (channel->selected())
					{
						channel->draw_caption(m_Paint, Lefter(),i,get_current_position());
						channel->draw_pointer(m_Paint, Lefter());
					}
				}
			}
		}
		break;
	case 1:
		repaint();
		break;
	default:
		break;
	}
}

void Wave_caption::slot_graph_unselect()
{
	UnSelectAll();
}

void Wave_caption::slot_graph_add_select(void* ptr)
{
	Draw_channel* pChannel = (Draw_channel*)ptr;
	if (!pChannel->selected())
		AddSelected(pChannel);
	else
		DeleteSelected(pChannel);
}

void Wave_caption::slot_graph_delete(void* ptr)
{
	int n = m_lstSelection.indexOf((Draw_channel*)ptr);
	if (n >= 0) m_lstSelection.removeAt(n);
}

void Wave_caption::slot_cursor_move(int pos)
{
	set_current_position(pos);
	repaint();
}

void Wave_caption::contextMenuEvent(QContextMenuEvent* e)
{
	if (m_pChannel != NULL)	
		emit CaptionMenu();
}

// 被选择通道中是否含有开关量
bool Wave_caption::HasDigtal()
{
	bool b = false;

	QList<Draw_channel*>::iterator it;
	for (it=m_lstSelection.begin(); it!=m_lstSelection.end();++it)
	{
		if ((*it)->class_name() == DRAWCLASS[2])
		{
			b=true;
			break;
		}
	}

	return b;
}
