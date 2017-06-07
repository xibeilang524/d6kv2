#include <QPolygon>
#include <QPolygonF>
#include <QPointF>

#include "drawchannel.h"
#include "globaldef.h"

/* QDrawChannel */

Draw_channel::Draw_channel(Wave_graph* owner)
{
	m_owner_ptr = owner;
	m_pdata = NULL;
	m_pcomtrade = NULL;
	m_max_value = 0;
	m_bshow_caption = true;
	m_bvisible = true;
	m_caption = "";
	m_bselected = false;
	// _font
	m_bshow_pointer = true;
	m_color = Qt::white;
	m_draw_rect = QRect(0, 0, 0, 0);
	m_position = 0;
	m_left_margin = AMARGIN;
	m_bauto_size = false;
	m_str_class_name = DRAWCLASS[0];
}

Draw_channel::~Draw_channel()
{
	m_pdata = NULL;
	m_pdata = NULL;
}

// 这个函数实现不正确，没有使用实际的font
QPoint Draw_channel::caption_size()
{
	QFontMetrics fm = m_owner_ptr->fontMetrics();
	return QPoint(fm.width(m_caption), fm.height());
}

void Draw_channel::copy_font(QFont& ft)
{
	m_font = ft;
}

void Draw_channel::change()
{
	if (!m_owner_ptr->locked())
		m_owner_ptr->render();
}

void Draw_channel::set_view_data(View_data* data, double maxData)
{
	if (m_pdata != data)
	{
		m_pdata = data;
		m_max_value = maxData;
	}
}

void Draw_channel::set_comtrade_data(Comtrade* c)
{
	if (m_pcomtrade !=c)
	{
		m_pcomtrade = c;
	}
}

void Draw_channel::draw_caption(QPainter* p, int rightPos,int index,int currentPos)
{
	if (!m_bshow_caption)
		return;

	QRect rct = draw_rect();
	rct.setRight(rightPos);
	//画两个曲线之间的虚线
	QPoint ptbegin;
	ptbegin.setX(0);
	ptbegin.setY(rct.bottom());
	QPen pen(Qt::darkMagenta,1,Qt::DashDotLine);
	p->setPen(pen);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	QPainterPath paLine;
	paLine.moveTo(ptbegin);
	paLine.lineTo((ptbegin.x()+rct.width()),ptbegin.y());
	p->drawPath(paLine);

	rct.setRight(rightPos - POINTERHEIGHT * 2);
	QRegion rgn(rct);	
	p->setClipRegion(rgn);		// <<<<<start clip region

	p->setPen(m_color);
	int fsize = m_font.pointSize();
	QFontMetrics fm = p->fontMetrics();
	int n = fm.height();
// 	QFont font = p->font();
// 	QString family = font.family();
// 	int ftsize = font.pointSize();
// 	font.setFamily(_C2Q("Microsoft YaHei"));
// 	p->setFont(font);
	QPoint pt(m_left_margin + 2, m_position + n / 2 -1 );
    // 左边: 从Margin开始画起; 右边: 从指针结束的地方开始画起
	if (rightPos == 0)
		pt.setX(m_left_margin + 2 * POINTERHEIGHT + 3);
	
	//显示坐标和值的位置
	QPoint ptValue(m_left_margin+2,rct.bottom()-1);
	double fTime=0;
	double currentValue=0;
	QString unit = "";
	if (m_pcomtrade)
	{
		fTime = m_pcomtrade->get_sample_time(currentPos);
		unit = m_pcomtrade->get_channel_unit(index);
	}
	if (m_pdata)
	{
		currentValue = m_pdata->Value(currentPos);
	}
	char message[256];
	sprintf(message,"游标: %5.2fms, 值: %5.2f", fTime, currentValue);		// 游标
	if (unit!="")
	{
		p->drawText(pt, m_caption+_C2Q(",单位:")+unit);
	}
	else
	{
		p->drawText(pt, m_caption);
	}

	p->drawText(ptValue, _C2Q(message));

	if (m_bselected)
	{
		rct = get_select_rect(rightPos);
//		rct.adjust(1, 1, -1, -1);
 		rct.adjust(1, 1, -1,-2);
		p->setBrush(Qt::NoBrush);
		p->drawRect(rct);
	}
	
	p->setClipping(false);		// >>>>>end clip region
}

void Draw_channel::draw_pointer(QPainter* p, int rightPos)
{
	if(!m_bshow_pointer) return;
	int flag = rightPos == 0 ? 1 : -1;
	QPoint pt(rightPos, m_position);
	QPointF ptArr[4];
	ptArr[0] = QPointF(pt.x() + flag * POINTERHEIGHT, pt.y());
	ptArr[1] = QPointF(ptArr[0].x() + flag * POINTERHEIGHT / 2,
		ptArr[0].y() - POINTERHEIGHT * 1.0 / 1.2 / 2);
	ptArr[2] = QPointF(ptArr[1].x(), ptArr[0].y() + POINTERHEIGHT * 1.0 / 1.2 / 2);
	ptArr[3] = QPointF(ptArr[0].x() + flag * POINTERHEIGHT, ptArr[0].y());
    
	QPen pen;
	pen.setWidth(0);
	pen.setColor(m_color);
	p->setPen(pen);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
//  	p->setBrush(m_Color);  //填充色

	QPolygonF  polygon;
	if (m_bselected)
	{
// 		polygon.setPoints(4, ptArr[0].x(), ptArr[0].y(),
// 			ptArr[1].x(), ptArr[1].y(),
// 			ptArr[3].x(), ptArr[3].y(),
// 			ptArr[2].x(), ptArr[2].y());
		polygon << QPointF(ptArr[0].x(),ptArr[0].y());
		polygon << QPointF(ptArr[1].x(),ptArr[1].y());
		polygon << QPointF(ptArr[3].x(),ptArr[3].y());
		polygon << QPointF(ptArr[2].x(),ptArr[2].y());
	}
	else
	{
// 		polygon.setPoints(3, ptArr[0].x(), ptArr[0].y(),
// 			ptArr[1].x(), ptArr[1].y(),
// 			ptArr[2].x(), ptArr[2].y());
		polygon << QPointF(ptArr[0].x(),ptArr[0].y());
		polygon << QPointF(ptArr[1].x(),ptArr[1].y());
		polygon << QPointF(ptArr[2].x(),ptArr[2].y());
	}
	p->drawPolygon(polygon,Qt::WindingFill);
// 	p->moveTo(ptArr[0]);
// 	p->lineTo(pt);
	QPainterPath pa;
	pa.moveTo(ptArr[0]);
	pa.lineTo( pt );
	p->drawPath( pa );

}

void Draw_channel::draw_line(QPainter* p,QPainterPath &pa, QPointF pt, bool print, bool move)
{
	
	QPointF ptDraw(pt);
	if (print)
	{
		ptDraw += m_owner_ptr->print_point();
	}
	if (move)
	{
		pa.moveTo(ptDraw.x(), ptDraw.y());
	}
	else
	{
		pa.lineTo(ptDraw.x(), ptDraw.y());
	}
}

int Draw_channel::hit_test(QPoint pt)
{
	if (!m_bvisible) return 0;
	return m_caption_rect.contains(pt) ? 1 : 0;
}

bool Draw_channel::selected()
{
	return m_bselected;
}

void Draw_channel::set_selected(bool b)
{
	if (b == m_bselected) return;
	m_bselected = b;
	m_owner_ptr->add_select_channel(this);
//	m_pOwner->RenderCaption(false);	// 重绘名称标识控件
	m_owner_ptr->render_caption();
	m_owner_ptr->select_channel();		// 通知控制者
}

QString Draw_channel::caption()
{
	return m_caption;
}

void Draw_channel::set_caption(const QString& s)
{
	if (s == m_caption) return;
	m_caption = s;
	m_owner_ptr->render_caption();
}

QRect Draw_channel::caption_rect()
{
	return m_caption_rect;
}

QFont& Draw_channel::font()
{
	return m_font;
}

void Draw_channel::set_font(QFont& f)
{
	if (f == m_font) return;
	m_font = f;
	m_owner_ptr->render_caption();
}


QColor Draw_channel::color()
{
	return m_color;
}

void Draw_channel::set_color(QColor c)
{
	if (c == m_color) return;
	m_color = c;
	// _font.color = m_Color? 如何设置颜色
	change();
}

QRect Draw_channel::draw_rect()
{
	return m_draw_rect;
}

void Draw_channel::set_draw_rect(QRect& r)
{
	if (r == m_draw_rect) return;
	m_draw_rect = r;
	rect_update();
	change_caption_rect();
}

int Draw_channel::left_margin()
{
	return m_left_margin;
}

void Draw_channel::set_left_margin(int n)
{
	if (n >= 0 && n < 10)
	{
		m_left_margin = n;
		change();
	}
}

int Draw_channel::position()
{
	return m_position;
}

void Draw_channel::set_position(int n)
{
	if (n == m_position) return;
	QRect r(m_draw_rect);
	int nDelta = n - m_position;
	r.translate(0, nDelta);
	set_draw_rect(r);
}

bool Draw_channel::show_border()
{
	return m_bshow_border;
}

void Draw_channel::set_show_border(bool b)
{
	if (b == m_bshow_border) return;
	m_bshow_border = b;
	change();
}

bool Draw_channel::show_caption()
{
	return m_bshow_caption;
}

void Draw_channel::set_show_caption(bool b)
{
	if (b == m_bshow_caption) return;
	m_bshow_caption = b;
	change();
}

bool Draw_channel::show_pointer()
{
	return m_bshow_pointer;
}

void Draw_channel::set_show_pointer(bool b)
{
	if (b == m_bshow_pointer) return;
	m_bshow_pointer = b;
	change();
}


bool Draw_channel::visible()
{
	return m_bvisible;
}

void Draw_channel::set_visible(bool b)
{
	if (b == m_bvisible) return;
	m_bvisible = b;
	if (m_owner_ptr->locked()) return;
	m_owner_ptr->show_channel(m_owner_ptr->indexOf(this), m_bvisible);
}

void Draw_channel::change_caption_rect()
{
	int len = POINTERHEIGHT / 2;
	m_caption_rect = QRect(QPoint(m_draw_rect.left(), m_position - len), 
							 QPoint(m_draw_rect.right(), m_position + len));
}

void Draw_channel::set_max_value( double f )
{
	m_max_value = f;
}

QString Draw_channel::class_name()
{
	return m_str_class_name;
}

QString Draw_channel::hint()
{
	return m_str_hint;
}

void Draw_channel::set_hint( QString s )
{
	m_str_hint = s;
}

bool Draw_channel::auto_size()
{
	return m_bauto_size;
}

void Draw_channel::set_auto_size( bool b )
{
	m_bauto_size = b;
}




/* QDrawAnalog */

Draw_analog::Draw_analog( Wave_graph* owner ) : Draw_channel(owner)
{
	m_fmin_Yref = 0.01;
	m_fmax_Yref = 100;
	m_str_class_name = DRAWCLASS[1];
}

// XData-数据指针 Max-最大值 Min-最小值 Result-指针偏移位移
int Draw_analog::get_margin(double* pp, double& fmax, double& fmin, 
						   int&counter, int nunit, int count)
{
	int nResult = nunit - 1;
	int n(0);
	fmax = pp[0];
	fmin = fmax;
	for (int i = 0; i < nunit; i++)
	{
		n++;
		counter++;
		if (counter >= count)
		{
			nResult = i;
			break;
		}
		if (fmax < pp[n]) fmax = pp[n];
		if (fmin > pp[n]) fmin = pp[n];
	}
	return 0;
}

void Draw_analog::draw_wave(QPainter* p, double xref, int lefter, 
						   int drawCount, bool print /* = FALSE */)
{
	if (!m_pdata) return;
	int nCount(0);
	QRect drawRect = draw_rect();
	//画两个曲线之间的虚线
	QPoint ptbegin;
	ptbegin.setX(0);
	ptbegin.setY(drawRect.bottom());
	QPen pen(Qt::darkMagenta,1,Qt::DashDotLine);
	p->setPen(pen);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	QPainterPath paLine;
	paLine.moveTo(ptbegin);
// 	paLine.lineTo((ptbegin.x()+drawRect.width())*xref,ptbegin.y());
	paLine.lineTo((ptbegin.x()+drawRect.width()),ptbegin.y());
	p->drawPath(paLine);
	//画中间的零的白色横线
	QPoint ptcentre;
	ptcentre.setX(0);
	ptcentre.setY((drawRect.top()+drawRect.bottom())/2+1);		//中间的横线向上移动一个像素,因为值全为0时可能会显示不出线
	QPen penCentre(Qt::white,1,Qt::SolidLine);
	p->setPen(penCentre);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	QPainterPath paLineCentre;
	paLineCentre.moveTo(ptcentre);
// 	paLineCentre.lineTo((ptcentre.x()+drawRect.width())*xref,ptcentre.y());
	paLineCentre.lineTo((ptcentre.x()+drawRect.width()),ptcentre.y());
	p->drawPath(paLineCentre);
	//画曲线
	QPainterPath pa;
	double* buffer = m_pdata->get_analog_data(lefter, lefter + drawCount, nCount);
	if (!buffer) return;
	int nunit = (int) 1.0 / xref;
	p->setPen(m_color);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	QPointF pt;
	pt.setX(0); 
	pt.setY(m_position - (buffer[0] * m_fYref));
 	draw_line(p,pa, pt, print, true);
	int counter(0);
	int n(0);
	double* fpoints = buffer;
	double ymin, ymax;
	while (counter < nCount - 1)
	{
// 		if (nunit > 1)
// 		{
// 			n = GetMargin(fpoints, ymax, ymin, counter, nunit, nCount);
// 			pt.setX(counter * xref);
// 			pt.setY(m_nPosition - (int)(ymin * m_fYref));
// 			DrawLine(p, pa,pt, print, false);
// 
// 			pt.setY(m_nPosition - (int)(ymax * m_fYref));
// 			DrawLine(p, pa,pt, print, false);
// 			fpoints += n;
// 		}
// 		else
		{
			counter++;
			pt.setX(counter * xref);
			pt.setY(m_position - (int)(buffer[++n] * m_fYref));
			draw_line(p, pa,pt, print, false);
		}
	}
	p->drawPath(pa);

}

double Draw_analog::Yref()
{
	return m_fYref;
}

void Draw_analog::set_Yref(double f)
{
	if (f < m_fmin_Yref || f > m_fmax_Yref || f == m_fYref) return;
	m_fYref = f;
	change();
}

double Draw_analog::min_Yref()
{
	return m_fmin_Yref;
}

double Draw_analog::max_Yref()
{
	return m_fmax_Yref;
}

// 绘制区域改变, 设置位置、纵向比例
void Draw_analog::rect_update()
{
	m_position = (m_draw_rect.top() + m_draw_rect.bottom()) / 2;
	if (m_bauto_size) auto_set_Yref();
}

// 自动根据分配高度和最大值来设置纵向比例；并设置比例极限值 
void Draw_analog::auto_set_Yref()
{
	m_fYref = (m_draw_rect.bottom() - m_draw_rect.top()) / m_max_value * 4.0 / 5.0 / 2.0;
	m_fmin_Yref = m_fYref / 10.0;
	m_fmax_Yref = m_fYref * 100;
}

void Draw_analog::set_auto_size(bool b)
{
	if (b == m_bauto_size) return;
	m_bauto_size = b;
	if (m_bauto_size) auto_set_Yref();
}

QRect Draw_analog::get_select_rect(int rightPos)
{
	QRect r(m_draw_rect);
	r.setTop(m_caption_rect.top());
	r.setBottom(m_caption_rect.bottom());
	r.setRight(rightPos - POINTERHEIGHT * 2);
// 	r.translate(0, 1);
	return r;
}



/* QDrawDigtal */

Draw_digtal::Draw_digtal( Wave_graph* owner ) : Draw_channel(owner)
{
	m_is_rect_flag = true;
	///< 		m_nHeight = (int)(DIGTALHEIGHT * 3.0 / 5.0);
	m_nheight = (int)(DIGTALHEIGHT * 3.0 / 10.0);		///<40高  1放12的位置
	m_str_class_name = DRAWCLASS[2];
}

QRectF Draw_digtal::print_rect(QRectF r, bool print)
{
	if (!print) return r;
	else return QRectF(QPoint(r.top(), r.left()), QPoint(r.bottom(), r.right()));
}

// 初始状态 变位次数 变位指针, pos 返回 state, index, 
void Draw_digtal::switch_info(int pos, int nInit, const STATUS* sta, int count,int& state, int& index)
{
	state = nInit;
	index = 0;
	if (count < 1)
		return;		// 无变位

	const STATUS* cur = sta;
	
	for (int i = 0; i < count; i++)
	{
		if (pos >= cur[i].position && pos <= cur[i].endPosition)
		{
			state = (int)cur[i].change;
			index = i;
			return;
		}
	}
	if (pos > cur[count-1].endPosition)
	{
		state = (int)cur[count-1].change;
		index = count-1;
	}
}

void Draw_digtal::draw_wave(QPainter* p, double xref, int lefter, int drawCount, bool print)
{
	if (m_pdata == NULL)
		return;
	QPainterPath pa;
	QRect drawRect = draw_rect();
	//画两个曲线之间的虚线
	QPoint ptbegin;
	ptbegin.setX(0);
	ptbegin.setY(drawRect.bottom());
	QPen pen(Qt::darkMagenta,1,Qt::DashDotLine);
	p->setPen(pen);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	QPainterPath paLine;
	paLine.moveTo(ptbegin);
	paLine.lineTo((ptbegin.x()+drawRect.width()),ptbegin.y());
	p->drawPath(paLine);

	//画曲线
	int initData, count;
	STATUS* status = m_pdata->get_digtal_data(initData, count);
	int nFrom;	//起始点状态
	int iFrom;		//起始点序号
	int nTo;		//结束点状态
	int iTo;			//结束点序号
	switch_info(lefter, initData, status, count, nFrom, iFrom);
	switch_info(lefter + drawCount, initData, status, count, nTo, iTo);
	
	p->setPen(m_color);
	p->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	STATUS* pData = status;

	bool bflag = false;
// 	p->setBrush(m_Color);
	QRectF rct = QRect(0, m_position - m_nheight, 0, m_nheight + 1);
	if (nFrom == 1)
	{
		bflag = true;
		rct.setLeft(0);
	}

	QPointF pt = QPoint(0, m_position - nFrom * m_nheight);
	draw_line(p, pa,pt, print, true); // 移动到0点

	pData += iFrom;
	for (int i = iFrom; i <= iTo; i++)
	{
		pt.setX((pData->position-lefter)*xref);
		int s = pData->change;
		pt.setY(m_position - s * m_nheight);
		draw_line(p, pa,pt, print, false);	//绘制到变位起点

		pt.setX((pData->endPosition - lefter) * xref);
		pt.setY(m_position - s * m_nheight);
		draw_line(p, pa,pt, print, false);	//绘制到变位终点

		if (bflag)		// 绘制变位矩形
		{
			bflag = false;
			rct.setRight((pData->endPosition - lefter) * xref);
			if (m_is_rect_flag)
				p->fillRect(print_rect(rct, print), QBrush(m_color));
		}
		else
		{
			bflag = true;
			rct.setLeft((pData->endPosition - lefter) * xref);
		}
		pData++;
	}

// 	pt.setX(drawCount * xref);
// 	pt.setY(m_nPosition - nTo * m_nHeight);
// 	DrawLine(p,pa, pt, print, false);

	p->drawPath(pa);
// 	if (nTo == 1)
// 	{
// 		rct.setRight(drawCount * xref);
// 		if (m_bBeRect)
// 			p->fillRect(PrintRect(rct, print), QBrush(m_Color));
// 	}
}

QRect Draw_digtal::get_select_rect(int rightPos)
{
	QRect r(m_draw_rect);
	r.setTop(m_caption_rect.top());
	r.setBottom(m_caption_rect.bottom());
	r.setRight(rightPos - POINTERHEIGHT * 2);
// 	r.translate(0, 1);
	return r;
}

void Draw_digtal::rect_update()
{
	m_position = (m_draw_rect.top()+m_draw_rect.bottom())/2;
}

void Draw_digtal::set_is_rect(bool b)
{
	if (b != m_is_rect_flag)
	{
		m_is_rect_flag = b;
		change();
	}
}

int Draw_digtal::height()
{
	return m_nheight;
}

bool Draw_digtal::is_rect()
{
	return m_is_rect_flag;
}
