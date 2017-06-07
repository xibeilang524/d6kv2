#include <QPoint>
#include <QRegion>
#include <QFontMetrics> 
#include <QPolygon>
#include <QEvent>
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>

#include <math.h>

#include "wavegraph.h"
#include "funutils.h"
#include "wavepix.h"


/* QTimeLine */

Time_line::Time_line(Wave_graph* owner, QPaintDevice* d)
{
	m_pOwner = owner;
	m_nSize = 1;
	m_Color = Qt::white;
	m_Cursor = Qt::CrossCursor;
	m_nPosition = -1;
	m_bShowPointer = true;
	m_bVisible = true;
	m_nTag = 0;
	m_Paint = new QPainter;
	m_Device = d;
}

Time_line::~Time_line()
{
	delete m_Paint;
}

void Time_line::draw_device(QPaintDevice* dev)
{
	m_Paint->begin(dev);
	m_Paint->setPen(m_Color);
	m_Paint->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
// 	m_Paint->moveTo(m_nPosition, 0);
// 	m_Paint->lineTo(m_nPosition, m_pOwner->Footer());
	QPainterPath pa;
	pa.moveTo(m_nPosition,0);
	pa.lineTo(m_nPosition, m_pOwner->footer());
	m_Paint->drawPath(pa);
	m_Paint->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	m_Paint->end();
}

void Time_line::Draw()
{
	if (!m_bVisible) 
		return;
	draw_device(m_pOwner);
	draw_device(m_Device);
}

int Time_line::hit_test(QPoint pt)
{
	int n = 0;
	if ((pt.x() > m_nPosition - m_nSize - 1) &&
		(pt.x() < m_nPosition + m_nSize + 1))
		n = 1;
	return n;
}

void Time_line::set_color(QColor c)
{
	if (m_Color == c) return;
	Change();
	m_Color = c;
	Change();
}

void Time_line::set_position(int n)
{
	if (m_nPosition == n) return;
	if (n > m_nPosition && n >= m_pOwner->cursor_range()) return;
	if (n < m_nPosition && n < m_pOwner->cursor_range(true)) return;
	Change();
	m_nPosition = n;
	Change();
}

void Time_line::set_show_pointer(bool b)
{
	if (m_bShowPointer == b) return;
	Change();
	m_bShowPointer = b;
	Change();
}

void Time_line::set_visible(bool b)
{
	if (m_bVisible == b) return;
	Change();
	m_bVisible = b;
	Change();
}

void Time_line::set_size(int n)
{
	if (m_nSize == n) return;
	Change();
	m_nSize = n;
	Change();
}

void Time_line::Change()
{
	Draw();
}

void Time_line::set_caption(QString s)
{
	if (m_strCaption != s)
	{
		m_strCaption = s;
		m_strHint = s;
	}
}

void Time_line::set_tag(int n)
{
	if (n < 0 || n >= m_pOwner->data_count()) return;
	if (m_nTag != n)
	{
		m_nTag = n;
		emit OnCursor(m_nTag);
	}
}


/* QWaveGraph */

Wave_graph::Wave_graph(QWidget* parent, Qt::WindowFlags f) 
		:QFrame(parent, f )
{
	setMouseTracking(true);
	setFocusPolicy(Qt::WheelFocus);
	
	m_backgroud_color = Qt::black;
	m_auto_set_copy_flag = true;
	m_redraw_flag = true;
	m_show_count = 10;
	m_digtal_height = DIGTALHEIGHT;
	m_line_width = 1;
	m_last_show_count = m_show_count;
	m_lock_all_lines_flag = true;
	m_auto_set_flag = false;
	m_fzoom_Xstep = 1.1;
	m_digtal_kind = dkRect;

	QPixmap pixMap(2000,1000);
	m_pixmap = pixMap.copy(0,0,800,500);
	int width = m_pixmap.width();
	m_mouse_line = new Mouse_line(this);
	m_pleft_hand = new QCursor(QPixmap((const char**)lefthand), 0, 0);

//	m_lstChannel.setAutoDelete(true);
//	m_lstCursor.setAutoDelete(true);
// 	m_lstLockLines.setAutoDelete(false);

	create_scroll_bars();
	init_data();

// 	setBackgroundColor(m_Color);
	set_color(m_backgroud_color);

	connect(m_mouse_line, SIGNAL(signal_move(QRect)), SLOT(slot_elastic_move(QRect)));
	connect(m_mouse_line, SIGNAL(OnUp(QRect)), SLOT(slot_elastic_up(QRect)));
}

Wave_graph::~Wave_graph()
{
	Clear();
// 	delete m_lstCursor;
// 	delete m_lstLockLines;
// 	delete m_lstChannel;
// 	delete m_pixMap;
	delete m_mouse_line;
	delete m_pVbar;
	delete m_pHbar;
	delete m_pbutton;
	delete m_pleft_hand;
}

void Wave_graph::create_scroll_bars()
{
	m_pbutton = new QToolButton(this);
	m_pHbar = new QScrollBar(Qt::Horizontal, this);
	m_pVbar = new QScrollBar(Qt::Vertical, this);
	m_pHbar->hide();
	m_pVbar->hide();
	m_pHbar->setRange(0, 0);
	m_pVbar->setRange(0, 0);
// 	m_pHBar->setSteps(20, 1);
// 	m_pVBar->setSteps(20, 1);
	m_pHbar->setSingleStep(20);
	m_pHbar->setPageStep(1);
	m_pVbar->setSingleStep(20);
	m_pVbar->setPageStep(1);
	connect(m_pbutton, SIGNAL(clicked()), SLOT(slot_btn_clicked()));
	connect(m_pHbar, SIGNAL(valueChanged(int)), this, SLOT(slot_Hscroll(int)));
	connect(m_pVbar, SIGNAL(valueChanged(int)), this, SLOT(slot_Vscroll(int)));
	update_scroll_bars();
}

void Wave_graph::update_scroll_bars()
{
	m_pVbar->setShown(m_pVbar->maximum() > 0);
	m_pHbar->setShown(m_pHbar->maximum() > 0);
	m_pbutton->setShown(m_pHbar->isVisible() && m_pVbar->isVisible());

	int w = width();
	int h = height();
	if (m_pVbar->isVisible())
		w -= Vbar_width();
	if (m_pHbar->isVisible())
		h -= Hbar_height();

	m_pHbar->setGeometry(0, height() - Hbar_height(), w, Hbar_height());
	m_pVbar->setGeometry(width() - Vbar_width(), 0, Vbar_width(), h);
	m_pbutton->setGeometry(m_pVbar->x(), m_pHbar->y(), Vbar_width(), Hbar_height());
}
	
void Wave_graph::slot_btn_clicked()
{
	QMessageBox::information(this, 
					tr("提示"),	
					tr("波形图"));
}

Draw_channel* Wave_graph::add(bool b /* = true */)
{
	Draw_channel* p = NULL; 
	if (b)
	{
		m_analog_count++;
		p = new Draw_analog(this);
	}
	else
	{
		p = new Draw_digtal(this);
	}
	m_lst_channel.append(p);
	m_channel_count++;
	if (p) notify(p, lnAdded);
	return p;
}

void Wave_graph::Clear()
{
	m_lstZoom.clear();
	m_lst_lock_lines.clear();

	QList<Time_line*>::iterator it;
	for (it = m_lst_cursor.begin();it !=m_lst_cursor.end();it++)
	{
		delete *it;
	}
	m_lst_cursor.clear();

	QList<Draw_channel*>::iterator itchannel;
	for (itchannel = m_lst_channel.begin();itchannel !=m_lst_channel.end();itchannel++)
	{
		delete *itchannel;
	}
	m_lst_channel.clear();

	init_data();
	render();
	update_horz();
	update_vert();
}

int	Wave_graph::indexOf(Draw_channel* channel)
{
	return m_lst_channel.indexOf(channel);
}

void Wave_graph::delete_channel(int index)
{
	if (index < 0 || index >= (int)m_lst_channel.count()) return;
	Draw_channel* p = channels(index);
	p->set_view_data(NULL);
	notify(p, lnDeleted);
	m_lst_channel.removeOne(p);
	delete p;
	m_channel_count--;
	if (index < (int)m_analog_count) m_analog_count--;
	render();
	render_caption();
}

Draw_channel* Wave_graph::channels(int index)
{
	if (index < 0 || index >= (int)m_lst_channel.count()) return NULL;
	return m_lst_channel.at(index);
}

void Wave_graph::insert(int index, Draw_channel* channel)
{

}

Time_line* Wave_graph::cursors(int index)
{
	if (index < 0 || index >= (int)m_lst_cursor.count()) return NULL;
	return m_lst_cursor.at(index);
}

inline int	Wave_graph::get_delta()
{
	if (m_fzoom_Xdata >= 1) return 1;
	return (int)(1.0 / m_fzoom_Xdata);
}

// 用位或的方式，可能代码会更简单，但可能不好理解！
void Wave_graph::keyPressEvent ( QKeyEvent * e )
{
	if (e->key() == Qt::Key_Left)
	{
		if (e->modifiers() == Qt::NoModifier)
			cursor_left();
		else if (e->modifiers() & Qt::ControlModifier)
			cursor_left(get_delta());
		else if (e->modifiers() & Qt::ShiftModifier)
			set_zoom_Xdata(m_fzoom_Xdata / m_fzoom_Xstep);
		else if (e->modifiers() & Qt::ControlModifier & Qt::LeftButton)
			cursor_left(m_cursor_step);
	}
	else if (e->key() == Qt::Key_Right)
	{
		if (e->modifiers() == Qt::NoModifier)
			cursor_right();
		else if (e->modifiers() & Qt::ControlModifier)
			cursor_right(get_delta());
		else if (e->modifiers() & Qt::ShiftModifier)
			set_zoom_Xdata(m_fzoom_Xdata * m_fzoom_Xstep);
		else if (e->modifiers() & Qt::ControlModifier & Qt::LeftButton)
			cursor_right(m_cursor_step);
	}
	else if (e->key() == Qt::Key_Up)
	{
		if (e->modifiers() == Qt::NoModifier)
		{
			if (! move_selected_channels(-1 * analog_height()))
				cursor_up();
		}
		else if (e->modifiers() & Qt::ControlModifier)
		{
		}
		else if (e->modifiers() & Qt::ShiftModifier)
			set_show_count(m_show_count + 1);
	}
	else if (e->key() == Qt::Key_Down)
	{
		if (e->modifiers() == Qt::NoModifier)
		{
			if (! move_selected_channels(analog_height()))
				cursor_down();
		}
		else if (e->modifiers() & Qt::ControlModifier)
		{
		}
		else if (e->modifiers() & Qt::ShiftModifier)
			set_show_count(m_show_count - 1);
	}
	else if (e->key() == Qt::Key_Home) 
	{
		if (e->modifiers() == Qt::NoModifier)
			cursor_home();
		else if (e->modifiers() & Qt::ControlModifier)
			page_home();
	}
	else if (e->key() == Qt::Key_End)
	{
		if (e->modifiers() == Qt::NoModifier)
			cursor_end();
		else if (e->modifiers() & Qt::ControlModifier)
			page_end();
	}
	else if (e->key() == Qt::Key_PageUp)
		page_up();
	else if (e->key() == Qt::Key_PageDown)
		page_down();
}

void Wave_graph::contextMenuEvent(QContextMenuEvent* e)
{
	emit signal_wave_menu();
}

void Wave_graph::lock()
{
	m_lock_count++;
	if (m_lock_count == 1) set_locking(false);
}

bool Wave_graph::locked()
{
	return m_lock_count != 0;
}

void Wave_graph::unlock()
{
	m_lock_count--;
	if (m_lock_count == 0) set_locking(true);
}

void Wave_graph::set_locking(bool updateing)
{
	if (updateing)
	{
		render();
		render_caption();
	}
}

void Wave_graph::move_cursor_line(int x)
{
	if (!m_cursor_line) return;
	set_line_pos(m_cursor_line, x);
	move_lock_line();
}

void Wave_graph::move_lock_line()
{
	if (m_lock_line_flag)
	{
		m_pdouble_click_line->set_tag(m_cursor_line->tag() - m_lock_delta);
		update_line_pos(m_pdouble_click_line);
	}
}

void Wave_graph::mousePressEvent( QMouseEvent* e )
{
	if (m_data_count == 0)
		return;
	
	m_mouse_points_move = 0; 
	int rx, ry;
    rx = e->x();
    ry = e->y();
	m_pt_mouse = QPoint(rx, ry);
	if (e->button() == Qt::LeftButton)
	{
		m_bleft_pressed = true;
		if (e->modifiers() == Qt::ControlModifier)
			m_mouse_line->DownMouse(rx, ry);
		else if (m_selected_line == NULL)
			move_cursor_line(rx);
		update();
	}
	else
	{
		m_bright_pressed = true;
	}
}

// 获得鼠标所在的时间轴线
Time_line* Wave_graph::get_select_cursor(QPoint pt)
{
	for (uint i = 0; i < m_lst_cursor.count(); i++)
	{
		Time_line* p = m_lst_cursor.at(i);
		if (p->hit_test(pt) == 1)
			return p;
	}
	return NULL;
}

void Wave_graph::mouseMoveEvent( QMouseEvent* e )
{
	int rx, ry;
    rx = e->x();
    ry = e->y();
	m_selected_channel = NULL;
	m_mouse_points_move++;
	if (m_bright_pressed && (m_mouse_points_move >= MINMOUSEPOINTSMOVE))
	{
	}
	else if (m_bleft_pressed)
	{
		m_str_hint = "";
		if (e->modifiers() & Qt::ControlModifier)
		{
			if (m_mouse_points_move >= MINMOUSEPOINTSMOVE)
				m_mouse_line->MoveMouse(rx, ry);
		}
		else
		{
			if (!cursor_count()) return;
			if (m_selected_line == m_pdouble_click_line)
				set_line_pos(m_pdouble_click_line, rx);
			else
				move_cursor_line(rx);
		}
	}
	else
	{
		m_selected_line = get_select_cursor(QPoint(rx, ry));
		if (m_selected_line)
		{
			set_mouse_mode(mmCursor);
			m_str_hint = m_selected_line->hint();
		}
		else
		{
			set_mouse_mode(mmNone);
			m_str_hint = "";
		}
	}
/*
	// 暂时先屏蔽tooltip功能，转移为工具栏提示
	if (ShowHint())
	{
		QRect r = QRect(rx - 5, ry - 5, 10, 10);
		m_pTip->Popup(r, tr(m_strHint));
	}
*/
	update();

	emit signal_auto_hint(m_str_hint);
}

void Wave_graph::mouseReleaseEvent( QMouseEvent* e )
{
	if (m_bright_pressed) 
	{
		m_bleft_pressed = false;
		m_bright_pressed = false;
	}
	else if (m_bleft_pressed)
	{
		m_bleft_pressed = false;
		if ((e->modifiers() & Qt::ControlModifier) && (m_mouse_points_move >= MINMOUSEPOINTSMOVE))
			m_mouse_line->UpMouse();
		update();
	}
}

void Wave_graph::mouseDoubleClickEvent( QMouseEvent* e )
{
	if (m_pdouble_click_line)
	{
		QPoint pt = QPoint(e->pos());
		set_line_pos(m_pdouble_click_line, pt.x());
	}
	update();

}

void Wave_graph::wheelEvent( QWheelEvent* e )
{

}

void Wave_graph::print(QPainter* p, int left, int ncount)
{
	for (uint i = 0; i < m_lst_channel.count(); i++)
	{
		Draw_channel* c = m_lst_channel.at(i);
		c->draw_wave(p, m_fzoom_Xdata, left, ncount, true);
	}
}

int Wave_graph::channel_position(int channel)
{
	int nResult(0);
	if (m_channel_count < 1) return nResult;
	int nAna = analog_height();
	for (int i = 0; i < m_channel_count; i++)
	{
		if (i >= channel) return nResult;
		if (i < m_analog_count) 
			nResult += nAna;
		else
			nResult += m_digtal_height;
	}
	nResult -= m_pVbar->value();
	return nResult;
}

void Wave_graph::auto_position(Draw_channel* channel)
{
	int n = indexOf(channel);
	if (n < 0) return;
	auto_position(n);
}

void Wave_graph::auto_position(int channel)
{
	if (m_channel_count < 1) return;
	int nHeight;
	QRect r(0, 0, 0, 0);
	int nAna = analog_height();
	int nPos = -1 * m_pVbar->value();
	for (int i = 0; i < m_channel_count; i++)
	{
		if (i < m_analog_count)
			nHeight = nAna;
		else
			nHeight = m_digtal_height;
		r = QRect(QPoint(AMARGIN, nPos), 
				  QPoint(width() - AMARGIN, nPos + nHeight));
		if (channel == i)
		{
			channels(i)->set_draw_rect(r);
			break;
		}
		nPos += nHeight;
	}
}

bool Wave_graph::can_zoom_in()
{
	bool bResult(true);
	//之前是开关量太多的情况下不允许放大太多
// 	int n = m_digtal_height * (m_channel_count - m_analog_count + 1);
// 	if (n > height() / 2) bResult = false;
	//模拟量最大也是一个屏幕显示3条波
	if(analog_height()>height()/3) 
	{
		bResult = false;
	}
	return bResult;
}

int Wave_graph::analog_height()
{
	if (m_analog_count < 1) 
		return 1;

	int anaHeight = height();
	int nCount = m_show_count;
	if (m_show_count > m_analog_count)
	{
		nCount = m_analog_count;
		anaHeight -= (m_channel_count - m_analog_count + 1) * m_digtal_height;
	}
	anaHeight = (anaHeight - m_pHbar->height()) / nCount;

	// 若开关量数目太多，则以每页显示10条模拟量曲线为基准
	if (anaHeight <= 0)
	{
		anaHeight = height() / 10;
	}
	else if (anaHeight<40)
	{
		anaHeight = 40;
	}
	else if (anaHeight >=200)
	{
		anaHeight = 200;
	}
	return anaHeight;
}

int Wave_graph::digtal_height()
{
	return m_digtal_height;
}

void Wave_graph::set_digtal_height(int n)
{
	if ((int)m_digtal_height == n) return;
	m_digtal_height = n;
	// 重新分配高度，刷新图形 
}

int Wave_graph::line_width()
{
	return m_line_width;
}

void Wave_graph::set_line_width(int n)
{
	if ((int)m_line_width == n) return;
	m_line_width = n;
	m_redraw_flag = true;
	// Invalidate
}

double Wave_graph::zoom_Xdata()
{
	return m_fzoom_Xdata;
}

void Wave_graph::set_zoom_Xdata(double f)
{
	int nLen = (int)(f * m_data_count);
	double fZoom = f;
	if (fZoom < m_fzoom_Xdata)					// zoom out缩小
	{
	    // 短数据，最小限制为以象素绘制
		int maxLen = width();
		if (m_pVbar->isVisible())
		{
			maxLen -= Vbar_width();
		}
		//这种情况应该不存在,m_nDataCount < nLen  指的是f>1
		if (m_data_count < nLen && fZoom < 1)	
		{
			return;
		}
		//m_nDataCount > nLen :f<1 缩小.  nLen<maxLen表示可以全部显示，不需要再缩小
// 		if (m_nDataCount > nLen && nLen < maxLen) 
// 		{
// 			return;
// 		}
	}
	if (fZoom > m_fzoom_Xdata && fZoom > 10)		// zoom in 放大
		fZoom = 10;
	if (fZoom != m_fzoom_Xdata)
	{
		m_fzoom_Xdata = fZoom;
		emit signal_zoom(m_fzoom_Xdata);

		update_horz(); 
		for (uint i = 0; i < m_lst_cursor.count(); i++)
			update_line_pos(cursors(i));
		m_redraw_flag = true;
// 		repaint();
		update();
	}
}

double Wave_graph::zoom_Xstep()
{
	return m_fzoom_Xstep;
}

void Wave_graph::set_zoom_Xstep(double f)
{
	if (f <= 1 || f > 10 || m_fzoom_Xstep == f) return;
	m_fzoom_Xstep = f;
}

void Wave_graph::line_zoom()
{
	if (m_channel_count == 0)
		return;

	QRect r = m_mouse_line->LastRoom();
	int nLen = r.right() - r.left();
	if (fabs((double)nLen) < MINMOUSEZOOM)
		return;

	double r1 = nLen < 0 ? 1.0 / 2.0 : width() / (nLen * 1.0);
	double lastData = m_fzoom_Xdata;
	set_zoom_Xdata(m_fzoom_Xdata * r1);
	
	if (lastData < m_fzoom_Xdata)		// zoom in
	{
		r1 = r1 * r.left() / m_fzoom_Xdata * (nLen / fabs((double)nLen));
		update_horz(r1);
	}
	else if (lastData > m_fzoom_Xdata)	// zoom out
	{
		if (m_pHbar->value() > m_pHbar->maximum())
			m_pHbar->setValue(m_pHbar->maximum());
	}
}

void Wave_graph::update_horz(int pos /* = 0 */)
{
	int nDraw = draw_count();
	int nLen = m_data_count - nDraw;
	if (nLen > 0)
	{
		m_pHbar->setRange(0, m_data_count - nDraw);		// 是否要减1 ???
// 		m_pHBar->setSteps(m_pHBar->singleStep(), nDraw);
		m_pHbar->setSingleStep(m_pHbar->singleStep());
		m_pHbar->setPageStep(nDraw);
		if (pos != 0)
		{
			pos += m_pHbar->value();
			if (pos > m_pHbar->maximum())
				pos = m_pHbar->maximum();
			m_pHbar->setValue(pos);
		}
	}
	else
	{
		m_pHbar->setRange(0, 0);
// 		m_pHBar->setSteps(m_pVBar->singleStep(), 0);
		m_pHbar->setSingleStep(m_pVbar->singleStep());
		m_pHbar->setPageStep(0);
	}
	update_scroll_bars();
}

// 游标至尾部
void Wave_graph::cursor_end()
{
	if (m_pHbar->isVisible())
		m_pHbar->setValue(m_pHbar->maximum());
	
	move_cursor_line(cursor_range() - 1);
}

// 游标至首部
void Wave_graph::cursor_home()
{
	if (m_pHbar->isVisible())
		m_pHbar->setValue(0);
	
	move_cursor_line(0);
}

// 游标左移(一个单位), 移至页首则PageUp
void Wave_graph::cursor_left(int delta /* = 1 */)
{
	if (m_cursor_line == NULL || m_cursor_line->tag() <= 0)
		return;

	m_cursor_line->set_tag(m_cursor_line->tag() - delta);
	update_line_pos(m_cursor_line);
	move_lock_line();

	if (m_cursor_line->position() <= 1)
	{
		int nMove = m_pHbar->value();
		page_up();
		if (m_pHbar->value() != 0)
			nMove /= 3;
		move_cursor_line(nMove * m_fzoom_Xdata);
	}
	update();
}

// 游标右移(一个单位), 移至页尾则PageDown
void Wave_graph::cursor_right(int delta /* = 1 */)
{
	if (m_cursor_line == NULL || 
		m_cursor_line->tag() >= m_data_count - 1)
		return;

	int maxLen = width();
	if (m_pVbar->isVisible())
		maxLen -= Vbar_width();
	m_cursor_line->set_tag(m_cursor_line->tag() + delta);
	update_line_pos(m_cursor_line);
	move_lock_line();
	if (m_cursor_line->position() > maxLen)
	{
		int nMove = m_pHbar->value();
		page_down();
		if (m_pHbar->value() < m_pHbar->maximum())
			nMove = draw_count() * 2 / 3;
		move_cursor_line(nMove * m_fzoom_Xdata);
	}
	update();
}  

// 向左翻页(1/3页)
void Wave_graph::page_up()
{
	int nMove = draw_count() / 3;
	if ((m_pHbar->value() - nMove) < 0)
		m_pHbar->setValue(0);
	else
		m_pHbar->setValue(m_pHbar->value() - nMove);
}

// 向右翻页
void Wave_graph::page_down()
{
	int nEnd = m_pHbar->maximum();
	int nMove = draw_count() / 3;
	if ((m_pHbar->value() + nMove) >= nEnd)
		m_pHbar->setValue(nEnd);
	else
		m_pHbar->setValue(m_pHbar->value() + nMove);
}

// 曲线上移(单位模拟量高度)
void Wave_graph::cursor_up()
{
	if (m_pVbar->isVisible())
	{
		int nPos = analog_height();
		if (m_pVbar->value() < nPos)
			nPos = m_pVbar->value();
		m_pVbar->setValue(m_pVbar->value() - nPos);
	}
}

// 曲线下移
void Wave_graph::cursor_down()
{  
	if (m_pVbar->isVisible())
	{
		int nPos = m_pVbar->value() + analog_height();
		if (nPos < m_pVbar->maximum())
			m_pVbar->setValue(nPos);
		else
			m_pVbar->setValue(m_pVbar->maximum());
	}
}

// 曲线至底部
void Wave_graph::page_end()
{
	if (m_pVbar->isVisible())
		m_pVbar->setValue(m_pVbar->maximum());
}

// 曲线至顶部
void Wave_graph::page_home()
{
	if (m_pVbar->isVisible())
		m_pVbar->setValue(0);
}

// 全展开曲线
void Wave_graph::expand()
{
	set_zoom_Xdata(1);
}

// 配置缩放下拉列表
void Wave_graph::zoom_config()
{
	int nLen = width();
	if (m_pVbar->isVisible())
		nLen -= Vbar_width();
	double fZoom = (double)nLen / m_data_count;
	m_lstZoom.clear();
	bool b = false;
	int nMax = 1 / fZoom;
	if (nMax < 1)
	{
		b = true;
		nMax = 1;
	}

	int i;
	int m_zoom=fZoom*100;
	for (i = 1; i <= nMax; i++)
		m_lstZoom.append(QString::number(i *  m_zoom) + "%");
	if (b)
		m_lstZoom.append(QString::number(100) + "%");
	for (i = 2; i <= 9; i++)
		m_lstZoom.append(QString::number(i * 100) + "%");
}

void Wave_graph::fill_in()
{
	int nLen = width();
	if (m_pVbar->isVisible())
		nLen -= Vbar_width();
	set_zoom_Xdata((double)nLen / (double)m_data_count);
	zoom_config();
}

void Wave_graph::move_channel(Draw_channel* channel, int pos)
{
	if (channel == NULL) 
		return;
	int pos1 = (channel->draw_rect().bottom() + channel->draw_rect().top()) / 2;
	pos1 = pos - pos1;
	channel_move(channel, pos1);
}

// 返回：是否可以移动
bool Wave_graph::channel_move(Draw_channel* channel, int delta, bool bLock)
{
	if (channel == NULL) 
		return false;
	QRect r = channel->draw_rect();
	int pos;
	if (delta > 0)
	{
		pos = r.bottom() + delta;
		if (pos > rect().height())
			return false;
	}
	else
	{
		pos = r.top() + delta;
		if (pos < 0) 
			return false;
	}
	r.translate(0, delta);
	channel->set_draw_rect(r);
	if (! bLock)
	{
		m_redraw_flag = true;
// 		repaint();
		update();
		render_caption();
	}
	return true;
}

bool Wave_graph::move_selected_channels(int delta)
{
	int b = false;
	for (int i = 0; i < m_channel_count; i++)
	{
		Draw_channel* p = channels(i);
		if (p->selected())
		{
			channel_move(p, delta, true);
			b = true;
		}
	}
	if (b)		// 一次性刷新
	{
		m_redraw_flag = true;
// 		repaint();
		update();
		render_caption();
	}
	return b;
}

Time_line* Wave_graph::add_cursor(bool key /* = false */)
{
	Time_line* p = new Time_line(this, &m_pixmap);
	if (!m_cursor_line && key)
		m_cursor_line = p;
	m_lst_cursor.append(p);
	return p;
}

void Wave_graph::delete_cursor(int index)
{
	if (index < 0 || index >= (int)m_lst_cursor.count()) return;
	Time_line* line = m_lst_cursor.at(index);
	m_lst_cursor.removeAt(index);
	delete line;
}

int Wave_graph::cursor_index(Time_line* line)
{
	return m_lst_cursor.indexOf(line);
}

void Wave_graph::update_vert(bool all /* = true */, int h /* = 0 */)
{
	int bottomHeight = h != 0 ? h : max_footer(all);
	int nSize = height();
	bool bShow = bottomHeight > nSize;
	if (m_pHbar->maximum() > 0)
	{
		bottomHeight += Hbar_height();
		nSize -= Hbar_height();
	}

	if (bShow)
	{
		m_pVbar->setRange(0, bottomHeight - nSize);
// 		m_pVBar->setSteps(m_pVBar->singleStep(), nSize);
		m_pVbar->setSingleStep(m_pVbar->singleStep());
		m_pVbar->setPageStep(nSize);
	}
	else
	{
		m_pVbar->setRange(0, 0);
// 		m_pVBar->setSteps(m_pVBar->singleStep(), 0);
		m_pVbar->setSingleStep(m_pVbar->singleStep());
		m_pVbar->setPageStep(0);
	}
	m_pVbar->setShown(m_pVbar->maximum() != 0);
}

int Wave_graph::draw_count()
{
	int nLen(0);
	if (m_pVbar->isVisible())
		nLen += Vbar_width();
	double f = (width() - nLen - lefter()) / m_fzoom_Xdata;
	return (int)f;
}

int Wave_graph::cursor_range(bool h /* = false */)
{
	int nRange(0);
	if (h)
	{
		if (m_pHbar->isVisible())
			nRange = -1 * (int)(m_pHbar->value() * m_fzoom_Xdata);
	}
	else
	{
		nRange = m_data_count - 1;
		if (m_pHbar->isVisible())
			nRange -= m_pHbar->value();
		nRange *= m_fzoom_Xdata;
	}
	return nRange;
}

Digtal_Kind Wave_graph::digtal_kind()
{
	return m_digtal_kind;
}

void Wave_graph::set_digtal_kind(Digtal_Kind dk)
{
	if (m_digtal_kind == dk) return;
	m_digtal_kind = dk;
	lock();

	int counter = 0;
// 	QDrawChannel* channel = NULL;
// 	 QListIterator<QDrawChannel> it(*m_lstChannel);
// 	while ((channel = it.current()) != 0)
// 	{
// 		++it;
// 		if (++counter < m_nAnaCount)
// 			continue;
// 
// 		QDrawDigtal* p = (QDrawDigtal*)channel;
// 		p->SetBeRect(m_DigtalKind == dkRect);
// 	}
	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		if (++counter < m_analog_count)
			continue;
		Draw_digtal* p = (Draw_digtal*)(*it);
		p->set_is_rect(m_digtal_kind == dkRect);
	}
	unlock();
}

bool Wave_graph::is_selected(int index)
{
	if (index < 0 || index >= (int)m_lst_channel.count()) return false;
	return channels(index)->selected();
}

void Wave_graph::do_selected(int index, bool b)
{
	if (index < 0 || index >= (int)m_lst_channel.count()) return;
	channels(index)->set_selected(b);
}

void Wave_graph::render_caption(bool full /* = true */)
{
	int action = full;
	emit signal_move(action);
}

void Wave_graph::add_lock_line(Time_line* line)
{
	if (line) m_lst_lock_lines.append(line);
}

void Wave_graph::delete_lock_line(Time_line* line)
{
	int n = m_lst_lock_lines.indexOf(line);
	if (n > -1)	m_lst_lock_lines.removeAt(n);
}

Mouse_Mode Wave_graph::mouse_mode()
{
	return m_mouse_mode;
}

void Wave_graph::set_mouse_mode(Mouse_Mode mm)
{
	if (m_mouse_mode == mm) return;
	m_mouse_mode = mm;
	update_mouse_mode();
}

void Wave_graph::update_mouse_mode()
{
	switch (m_mouse_mode)
	{
	case mmNone:
		setCursor(Qt::ArrowCursor);
		break;
	case mmCopy:
		setCursor(Qt::PointingHandCursor);
		break;
	case mmCursor:
		setCursor(*m_pleft_hand);
		break;
	case mmHint:
		break;
	}
}

Time_line* Wave_graph::lock_cursor(int index)
{
	if (index < 0 || index >= (int)m_lst_lock_lines.count()) return NULL;
	return m_lst_lock_lines.at(index);
}

int Wave_graph::lefter()
{
	return 0;
}

int Wave_graph::footer()
{
	return height();
}

void Wave_graph::lock_lines()
{
	if (cursor_count() < 3) return;
	m_lock_line_flag = !m_lock_line_flag;
	Time_line *p1, *p2;
	p1 = cursors(0); 
	p2 = cursors(2);
	if (m_lock_line_flag)
	{
		add_lock_line(p1);
		add_lock_line(p2);
		m_lock_delta = p1->tag() - p2->tag();
	}
	else
	{
		delete_lock_line(p1);
		delete_lock_line(p2);
		m_lock_delta = 0;
	}
}

void Wave_graph::set_item_color(QColor c, int index /* = 1 */)
{
	uint n1(0), n2(0);
	switch (index)
	{
	case 0:
		n1 = 0; n2 = m_analog_count;
		break;
	case 1:
		n1 = m_analog_count; n2 = m_channel_count;
		break;
	case 2:
		n1 = 0; n2 = m_channel_count;
		break;
	}
	lock();
	for (uint i = n1; i < n2; i++)
	{
		Draw_channel* p = m_lst_channel.at(i);
		p->set_color(c);
	}
	unlock();
}

void Wave_graph::scrollY(int delta)
{
// 	QDrawChannel* channel = NULL;
// 	 QListIterator<QDrawChannel> it(*m_lstChannel);
// 	while ((channel = it.current()) != 0)
// 	{
// 		++it;
// 		channel->SetPosition(channel->Position() + delta);
// 	}

	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		(*it)->set_position((*it)->position() + delta);
	}
}

void Wave_graph::load_cursors()
{

}

void Wave_graph::clear_intf()
{
// 	QDrawChannel* channel = NULL;
// 	 QListIterator<QDrawChannel> it(*m_lstChannel);
// 	while ((channel = it.current()) != 0)
// 	{
// 		++it;
// 		channel->set_view_data(NULL);
// 	}
	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		(*it)->set_view_data(NULL);
	}
}

int Wave_graph::get_default_analog_height()
{
	return 0;
}

void Wave_graph::init_data()
{
	m_bPrinting = false;
	m_clr_old = -1;
	m_channel_count = 0;
	m_analog_count = 0;
	m_data_count = 0;
	m_selected_channel = NULL;
	m_fzoom_Xdata = 1;
	m_copying_flog = false;
	m_selected_line = NULL;
	m_cursor_line = NULL;
	m_pdouble_click_line = NULL;
	m_lock_line_flag = false;
	m_cursor_step = 10;
	m_lock_delta = 0;
	m_region_select_flag = false;
	m_lock_count = 0;
	m_pt_selected_region = QPoint(0, 0);
	m_last_vert_pos = 0;
	m_last_horz_pos = 0;
}

void Wave_graph::set_line_pos(Time_line* line, int x)
{
	line->set_position(x);
	line->set_tag(position_to_dot(line->position()));
}

void Wave_graph::update_line_pos(Time_line* line)
{
	int nPos = line->tag();
	if (m_pHbar->isVisible())
		nPos -= m_pHbar->value();

	nPos *= m_fzoom_Xdata;
	line->set_position(nPos);
}

int Wave_graph::position_to_dot(int pos)
{
	int n = pos / m_fzoom_Xdata;
	if (m_pHbar->isVisible())
		n += m_pHbar->value();

	return n;
}

// 最底部的位置
int Wave_graph::max_footer(bool all /* = true */)
{
	int Max = 0;
	int Min = 0;
	for (int i = 0; i < m_channel_count; i++)
	{
		Draw_channel* channel = channels(i);
		if ((!all) && (! channel->visible()))
			continue;
		if (Min > channel->draw_rect().top())
			Min = channel->draw_rect().top();
		if (Max < channel->draw_rect().bottom())
			Max = channel->draw_rect().bottom();
	}
	// 保留一个开关量的空白高度
	return Max - Min + m_digtal_height;
}

bool Wave_graph::locate_channel(int index)
{
	return locate_channel(channels(index));
}

bool Wave_graph::locate_channel(Draw_channel* channel)
{
	return true;
}

bool Wave_graph::region_select()
{
	return m_region_select_flag;
}

void Wave_graph::set_region_select(bool b)
{
	if (m_region_select_flag == b) return;
	m_region_select_flag = b;
	if (m_region_select_flag)
	{
		int m1 = cursors(0)->tag();
		int m2 = cursors(2)->tag();
		get_range(m1, m2);
		m_pt_selected_region = QPoint(m1, m2);
	}
	else
		m_pt_selected_region = QPoint(0, 0);
	render();
}

bool Wave_graph::checked(int index)
{
	if (index < 0 || index >= (int)m_channel_count) return false;
	return channels(index)->selected();
}

void Wave_graph::set_checked(int index, bool b)
{
	if (index < 0 || index >= (int)m_channel_count) return;
	channels(index)->set_selected(b);
}

void Wave_graph::unselect_all()
{
	for (int i = 0; i < m_channel_count; i++)
		channels(i)->set_selected(false);
	emit signal_unselect();
}

void Wave_graph::select_channel()
{
/*	if (m_nAnaCount < 1)
		return;

	QColor* clrs = new QColor[m_nAnaCount];

	QDrawChannel* p = NULL;
	 QListIterator<QDrawChannel> it(*m_lstChannel);
	while ((p = it.current()) != 0)
	{
		++it;
		if (p->Selected())
			clrs[n] = p->Color();
		else
			clrs[n] = Qt::lightGray;

		if (++n >= m_nAnaCount)
			break;
	}
	emit OnSelect(m_nAnaCount, clrs);
	delete []clrs;

	if (m_nAnaCount < 1)
		return;
*/
	QColor* clrs = new QColor[m_analog_count];

	int n = 0;
	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		if((*it)->selected())
		{
			clrs[n] = (*it)->color();
		}
		else
		{
			clrs[n] = Qt::lightGray;
		}
		if (++n >= m_analog_count)
			break;
	}
	emit signal_select(m_analog_count, clrs);
	delete []clrs;
}

void Wave_graph::auto_set_rect()
{
	lock();
	alloc_rect();
	unlock();
}

int Wave_graph::cursor_step()
{
	return m_cursor_step;
}

void Wave_graph::set_cursor_step(int n)
{
	if (n < 2 || n > (int)m_data_count / 10) return;
	if ((int)m_cursor_step != n) 
		m_cursor_step = n;
}

double Wave_graph::get_scale(int index)
{
	double ran[4];
	int i;
	for (i = 0; i < 3; i++)
		ran[i] = 1;
	for (i = 0; i < m_analog_count; i++)
	{
		Draw_analog* p = (Draw_analog*)channels(index);
		if (!p->selected()) continue;
		if (ran[0] > p->Yref())
			ran[0] = p->Yref();
		if (ran[1] < p->Yref())
			ran[1] = p->Yref();
		if (ran[2] > p->min_Yref())
			ran[2] = p->min_Yref();
		if (ran[3] < p->max_Yref())
			ran[3] = p->max_Yref();
	}
	double f(1);
	if (index >= 0 && index <=3)
		f = ran[index];
	return f;
}

void Wave_graph::set_selected_item_scale(double y)
{
/*	Lock();
	int n = 0;
	QDrawChannel* p = NULL;
	 QListIterator<QDrawChannel> it(*m_lstChannel);
	while ((p = it.current()) != 0)
	{
		++it;
		if (++n >= m_nAnaCount)
			break;
		if (!p->Selected()) continue;
		((QDrawAnalog*)p)->SetYref(y);
	}
	UnLock();*/

	lock();
	int n = 0;
	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		if (++n >= m_analog_count)
			break;
		if (!(*it)->selected())
		{
			continue;
		}
		((Draw_analog*)(*it))->set_Yref(y);
	}
	unlock();
}

void Wave_graph::notify(void* ptr, ListNotify action)
{
	if (locked()) return;
	Draw_channel* p = (Draw_channel*)ptr;
	switch(action)
	{
	case lnAdded:
		unselect_all();
		p->set_selected(true);
		locate_channel(p);
		break;
	case lnDeleted:
		emit signal_delete(ptr);
		break;
	}
}

bool IsValid(QPoint pt, int count)
{
	return (pt.x() >= 0) && (pt.y() < count) && (pt.x() < pt.y());
}

void Wave_graph::do_select_region(QPoint pt)
{
	if (!IsValid(pt, m_data_count)) return;
	m_pt_selected_region = pt;
	m_region_select_flag = true;
	render();
}

void Wave_graph::show_all(bool b /* = true */)
{
	lock();
	for (int i = 0; i < m_channel_count; i++)
		channels(i)->set_visible(b);
	unlock();
}

void Wave_graph::copy_graph(bool b /* = true */)
{
	if (!m_auto_set_copy_flag) return;
	if (b)
	{
		m_clr_old = m_backgroud_color;
		m_backgroud_color = Qt::white;
		set_item_color(Qt::black, 1);
	}
	else
	{
//		if (m_clrOld != Qt::white) return;
		m_backgroud_color = m_clr_old;
		set_item_color(Qt::white, 1);
	}
}

void Wave_graph::add_select_channel(Draw_channel* channel)
{
	emit signal_add_select(channel);
}

void Wave_graph::show_channel(int channel, bool show)
{
	if (m_auto_set_flag) return;
	int pos = channel_position(channel);
	int delta;
	if (channel < (int)m_analog_count)
		delta = analog_height();
	else
		delta = DIGTALHEIGHT;
	QRect r;
	if(show)
	{
		r = channels(0)->draw_rect();
		r.setTop(pos);
		r.setBottom(r.top() + delta);
		channels(channel)->set_draw_rect(r);
	}
	if (!show) delta *= -1;
	for (int i = 0; i < m_channel_count; i++)
	{
		if ((int)i == channel) continue;
		r = channels(i)->draw_rect();
		if (r.top() >= pos)
		{
			r.translate(0, delta);
			channels(i)->set_draw_rect(r);
		}
	}
	update_vert(false);
}

void Wave_graph::show_channel()
{
	if (locked()) return;

	bool* arr = new bool[m_analog_count];
	for (int i = 0; i < m_analog_count; i++)
		arr[i] = channels(i)->visible();
	emit signal_show(m_analog_count, arr);
}

int Wave_graph::show_count()
{
	return m_show_count;
}

void Wave_graph::set_show_count(int n)
{
	if (n < m_show_count && n < 2) 
	{
		QMessageBox::about(this, tr("波形分析工具"),
			tr("波形不能再进行纵向放大操作了！"));

		return;	//最少也显示两条模拟量波形
	}
	if (n > m_show_count && n > (m_analog_count + 1))
	{
		QMessageBox::about(this, tr("波形分析工具"),
			tr("波形不能再进行纵向缩小操作了！"));
		return;
	}
//	if (n == (m_analog_count + 1) && (!can_zoom_in())) return; 
	m_show_count = n;
	realloc_rect();
	render();
	render_caption();
}

void Wave_graph::realloc_rect()
{
	if (m_channel_count < 1)
		return;

	int anaHeight = analog_height();
	int nPos = -1 * m_pVbar->value();
	int i=0;
	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		if (!(*it)->visible())
		{
			continue;
		}
		int channelHeight = i <m_analog_count ? anaHeight : m_digtal_height;
		QRect rct = QRect(AMARGIN, nPos, width(), channelHeight);
		(*it)->set_auto_size(true);
		(*it)->set_draw_rect(rct);
		(*it)->set_auto_size(false);
		nPos += channelHeight;
		i++;
	}
	update_vert(false);

}

void Wave_graph::alloc_rect()
{
	realloc_rect();
	fill_in();
}

int Wave_graph::wave_height()
{
	int n = analog_height();
	n *= m_analog_count;
	n += (m_channel_count - m_analog_count + 1) * m_digtal_height;
	return n;
}

void Wave_graph::render()
{
	m_redraw_flag = true;
// 	repaint();
	update();
}

void Wave_graph::draw_lines()
{
	QList<Time_line*>::iterator it;
	for (it=m_lst_cursor.begin();it!=m_lst_cursor.end();++it)
	{
		(*it)->Draw();
	}
}

void Wave_graph::resizeEvent( QResizeEvent* e)
{
	QFrame::resizeEvent(e);
	m_mouse_line->SetRange(QPoint(width(), height()));
	update_scroll_bars();
	render();
}

void Wave_graph::paintEvent( QPaintEvent* e)
{
	QPainter pp(this);
	int len = width() - lefter();
	if (m_pixmap.width() != len)
	{
		// 			m_pixMap.resize(len, height());
		m_pixmap = m_pixmap.copy(0,0,len,height());
	}
// 	QPixmap pixmap(len,height());
	m_pixmap.fill(this, rect().topLeft());
 	QColor color(Qt::black);
	m_pixmap.fill(color);
// 	if (m_bReDraw)
// 	{
// 		Render(0);
		m_redraw_flag = false;
		if (locked()) return;
// 		if (!pd)
		{
	/*		if (m_pixMap.width() != len)
			{
				// 			m_pixMap.resize(len, height());
				m_pixMap = m_pixMap.copy(0,0,len,height());
			}*/
			pp.begin(&m_pixmap);
			int a= m_pixmap.width();
			int b = m_pixmap.height();
		}
// 		else
// 		{
// 			pp.begin(pd);
// 		}

		QRect r(rect());
// 		r.setRight(CursorRange());
		pp.setClipRect(r);
		pp.fillRect(r,QBrush(m_backgroud_color));
// 		QPainterPath pa;
// 		pa.moveTo(10,10);
// 		pa.lineTo(20,20);
// 		pa.lineTo(50,100);
// 		pp.drawPath(pa);

		QList<Draw_channel*>::iterator it;
		for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
		{
			if (!(*it)->visible())
			{
				continue;
			}
			if ((*it)->draw_rect().bottom() <= 0 ||
				(*it)->draw_rect().top() >= r.bottom())
			{
				continue;
			}
			(*it)->draw_wave(&pp,m_fzoom_Xdata,m_pHbar->value(),
draw_count(),m_bPrinting);
		}

		draw_lines();
		pp.end();
		pp.drawPixmap(QPoint(0,0),m_pixmap);	
// 	}
// 	bitBlt(this, QPoint(0, 0), m_pixMap, rect(), QPainter::RasterOp_SourceAndNotDestination);


// 	m_pixMap =m_pixMap.copy(rect());

}

void Wave_graph::render(QPaintDevice* pd)
{
	m_redraw_flag = false;
	if (locked()) return;
	QPainter pp(this);
	if (!pd)
	{
		int len = width() - lefter();
		if (m_pixmap.width() != len)
		{
// 			m_pixMap.resize(len, height());
			m_pixmap = m_pixmap.copy(0,0,len,height());
		}
		m_pixmap.fill(this, rect().topLeft());
		m_pixmap.fill(m_backgroud_color);
		pp.begin(&m_pixmap);
	}
	else
	{
		pp.begin(pd);
	}

	QRect r(rect());
	r.setRight(cursor_range());
	pp.setClipRect(r);

	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		if (!(*it)->visible())
		{
			continue;
		}
		if ((*it)->draw_rect().bottom() <= 0 ||
			(*it)->draw_rect().top() >= r.bottom())
		{
			continue;
		}
		(*it)->draw_wave(&pp,m_fzoom_Xdata,m_pHbar->value(),
				draw_count(),m_bPrinting);
	}

	draw_lines();
	pp.end();
}

void Wave_graph::slot_Hscroll(int pos)
{
	if (pos > m_pHbar->maximum())
		pos = m_pHbar->maximum();
	m_redraw_flag = true;
	m_last_horz_pos = pos;
	repaint();
	for (uint i = 0; i < m_lst_cursor.count(); i++)
		update_line_pos(m_lst_cursor.at(i));
}

void Wave_graph::slot_Vscroll(int pos)
{
	if (pos > m_pVbar->maximum())
		pos = m_pVbar->maximum();
	int nDelta = m_last_vert_pos - pos;

	QList<Draw_channel*>::iterator it;
	for (it=m_lst_channel.begin();it!=m_lst_channel.end();++it)
	{
		QRect r=(*it)->draw_rect();
		r.translate(0,nDelta);
		(*it)->set_draw_rect(r);
	}
	m_last_vert_pos = pos;

	m_redraw_flag = true;
	repaint();
	render_caption();
}

QRect Wave_graph::graph_rect()
{
	int hsbExt(0);
	if (m_pHbar->isVisible())
		hsbExt = Hbar_height();
	int vsbExt(0);
	if (m_pVbar->isVisible())
		vsbExt = Vbar_width();
	QRect r(contentsRect());
	r.setWidth(r.width() - vsbExt);
	r.setHeight(r.height() - hsbExt);
	return r;
}

void Wave_graph::slot_elastic_move(QRect rct)
{

}

// 橡皮筋弹起
void Wave_graph::slot_elastic_up(QRect rct)
{
	if (m_copying_flog)
	{
		m_copying_flog = false;
		copy_select_rect();
		emit signal_copy();
	}
	else
	{
		line_zoom();
	}
}

void Wave_graph::copy_select_rect()
{
	
}

void Wave_graph::set_color(QColor c)
{
	if (m_backgroud_color != c)
	{
		m_backgroud_color = c;
	}
	QPalette palette;
	palette.setColor(backgroundRole(),m_backgroud_color);
	setPalette(palette);
	render();
}

int Wave_graph::Hbar_height()
{
	return m_pHbar->sizeHint().height();
}

int Wave_graph::Vbar_width()
{
	return m_pVbar->sizeHint().width();
}

int Wave_graph::line_count()
{
	return m_lst_cursor.count();
}

Draw_channel* Wave_graph::selected_channel()
{
	return m_selected_channel;
}

void Wave_graph::set_selected_channel( Draw_channel* channel )
{
	m_selected_channel = channel;
}

int Wave_graph::count()
{
	return m_channel_count;
}

int Wave_graph::analog_count()
{
	return m_analog_count;
}

int Wave_graph::cursor_count()
{
	return m_lst_cursor.count();
}

int Wave_graph::lock_cursor_count()
{
	return m_lst_lock_lines.count();
}

bool Wave_graph::mouse_zoom()
{
	return m_mouse_zoom_flag;
}

bool Wave_graph::lock_line()
{
	return m_lock_line_flag;
}

QStringList* Wave_graph::zoom_list()
{
	return &m_lstZoom;
}

bool Wave_graph::copying()
{
	return m_copying_flog;
}

void Wave_graph::set_copying( bool b )
{
	m_copying_flog = b;
}

int Wave_graph::default_analog_height()
{
	return get_default_analog_height();
}

bool Wave_graph::lock_all_lines()
{
	return m_lock_all_lines_flag;
}

void Wave_graph::set_lock_all_lines( bool b )
{
	m_lock_all_lines_flag = b;
}

Time_line* Wave_graph::cursor_line()
{
	return m_cursor_line;
}

void Wave_graph::set_cursor_line( Time_line* line )
{
	m_cursor_line = line;
}

Time_line* Wave_graph::double_click_line()
{
	return m_pdouble_click_line;
}

void Wave_graph::set_double_click_line( Time_line* line )
{
	m_pdouble_click_line = line;
}

bool Wave_graph::auto_set()
{
	return m_auto_set_flag;
}

void Wave_graph::set_auto_set( bool b )
{
	m_auto_set_flag = b;
}

double Wave_graph::min_selected_scale()
{
	return get_scale(0);
}

double Wave_graph::max_selected_scale()
{
	return get_scale(1);
}

double Wave_graph::min_selected_scale1()
{
	return get_scale(2);
}

double Wave_graph::max_selected_scale1()
{
	return get_scale(3);
}

bool Wave_graph::auto_set_copy()
{
	return m_auto_set_copy_flag;
}

void Wave_graph::set_auto_set_copy( bool b )
{
	m_auto_set_copy_flag = b;
}

bool Wave_graph::printing()
{
	return m_bPrinting;
}

void Wave_graph::set_printing( bool b )
{
	m_bPrinting = b;
}

QPoint Wave_graph::print_point()
{
	return m_pt_print;
}

void Wave_graph::set_print_point( QPoint pt )
{
	m_pt_print = pt;
}

QPoint Wave_graph::selected_region()
{
	return m_pt_selected_region;
}

QColor Wave_graph::get_color()
{
	return m_backgroud_color;
}

bool Wave_graph::show_hint()
{
	return m_bshow_hint;
}

void Wave_graph::set_show_hint( bool b )
{
	m_bshow_hint = b;
}

int Wave_graph::data_count()
{
	return m_data_count;
}

void Wave_graph::set_data_count( int n )
{
	m_data_count = n;
}
