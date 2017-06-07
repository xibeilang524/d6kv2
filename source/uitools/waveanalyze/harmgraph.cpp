#include <math.h>
#include <qpainter.h>
#include <qpixmap.h>
#include "harmgraph.h"


/* QHarmGraph */

QHarmGraph::QHarmGraph(QWidget* parent)
	: QWidget(parent)
{
	m_fBaseValue = 1;
	m_Color = Qt::black;
	m_HarmStyle = hsHorz;
	m_clrCaption = Qt::yellow;
	m_Items = new QVectorMgr;
	SetColor(m_Color);
}

QHarmGraph::~QHarmGraph()
{
	Clear(false);
	delete m_Items;
}

void QHarmGraph::Clear(bool bRepaint)
{
	m_Items->Clear();
	m_strCaption = "谐波棒图";
	m_clrCaption = Qt::yellow;
	Change();
}

void QHarmGraph::SetColor(const QColor& clr)
{
	if (m_Color != clr)
	{
		m_Color = clr;
// 		setBackgroundColor(m_Color);
		QPalette palette;
		palette.setColor(backgroundRole(),m_Color);
		setPalette(palette);

		Change();
	}
}

void QHarmGraph::SetUnit(const QString& str)
{
	if (m_strUnit != str)
	{
		m_strUnit = str;
		Change();
	}
}

void QHarmGraph::SetCaption(const QString& str)
{
	if (m_strCaption != str)
	{
		m_strCaption = str;
		Change();
	}
}

void QHarmGraph::SetBaseValue(double val)
{
	if (m_fBaseValue != val)
	{
		m_fBaseValue = val;
		Change();
	}
}

void QHarmGraph::SetItemColor(const QColor& clr)
{
	if (m_clrItem != clr)
	{
		m_clrItem = clr;
		Change();
	}
}

void QHarmGraph::SetCaptionColor(const QColor& clr)
{
	if (m_clrCaption != clr)
	{
		m_clrCaption = clr;
		Change();
	}
}

void QHarmGraph::SetHarmStyle(HARMSTYLE hs)
{
	if (m_HarmStyle != hs)
	{
		m_HarmStyle = hs;
		Change();
	}
}

QSize QHarmGraph::FontSize(const QString& str)
{
	QFontMetrics fm = fontMetrics();
	return QSize(fm.width(str), fm.height());
}

int QHarmGraph::MaxSize()
{
	int nSize = 0;
	for (int i = 0; i < m_Items->Count(); i++)
	{
		int n =	FontSize(m_Items->Item(i)->m_strCaption).width();
		if (nSize < n)
			nSize = n;
	}
	return nSize;
}

void QHarmGraph::DrawLine(QPainter* paint, const QPoint& pt, int margin)
{
	paint->setPen(Qt::white);
// 	paint->moveTo(pt);
// 	paint->lineTo(pt.x(), rect().bottom() - margin);
	QPainterPath pa;
	pa.moveTo( pt );
	pa.lineTo( pt.x(), rect().bottom() - margin );
	paint->drawPath( pa );
}


// 该函数的代码需要优化!
void QHarmGraph::paintEvent(QPaintEvent* e)
{
	QPainter paint;
	QPainterPath pa;
	QPixmap pm(size());
	paint.begin(&pm);
	
	const int Coors[] = {0, 20, 40, 60, 80, 100};
	int margin = 10;
	paint.fillRect(rect(), QBrush(m_Color));
	int capx = margin * 2;	// 名称标识
	int capy = margin;
	paint.setPen(m_clrCaption);
	QFont ft = font();
	ft.setPointSize(MINHARMSIZE);
	setFont(ft);
	paint.drawText(QPoint(capx, capy + 3), m_strCaption);
	
	if (m_Items->Count() < 1)
	{
		paint.end();
// 		bitBlt(this, 0, 0, &pm);	
		const QPointF point(0,0);
		paint.drawPixmap(point,pm);
		return;
	}

	int nDelta = FontSize(m_strCaption).height();// 名称，棒图分割线
	capx = MaxSize() + margin + 3;
	capy += nDelta;
	QString str;
	QRect rcHarm;
	QVectorItem* item;
	QPoint ptLine = QPoint(capx, capy);
	QPoint PtA, PtO, PtB;
	int i, baseWidth, nHeight, nx, ny;
	switch (m_HarmStyle)
	{
	case hsVert:	// 垂直方式
		// 坐标		
		nDelta = FontSize("100").width() + margin;
		PtA = QPoint(nDelta, ptLine.y());
		nDelta = rect().bottom() - margin - FontSize("100").height();
		PtO = QPoint(PtA.x(), nDelta);
		PtB = QPoint(rect().right() - margin, PtO.y());
		paint.setPen(Qt::white);
// 		paint.moveTo(PtA.x() - 2, PtA.y());
// 		paint.lineTo(PtA.x() + 3, PtA.y());
// 		paint.moveTo(PtA.x(), PtA.y());
// 		paint.lineTo(PtO.x(), PtO.y());
// 		paint.lineTo(PtB.x(), PtB.y());
// 		paint.moveTo(PtB.x(), PtB.y() - 2);
// 		paint.lineTo(PtB.x(), PtB.y() + 3);
		
		pa.moveTo(PtA.x() - 2, PtA.y());
		pa.lineTo(PtA.x() + 3, PtA.y());
		pa.moveTo(PtA.x(), PtA.y());
		pa.lineTo(PtO.x(), PtO.y());
		pa.lineTo(PtB.x(), PtB.y());
		pa.moveTo(PtB.x(), PtB.y() - 2);
		pa.lineTo(PtB.x(), PtB.y() + 3);
		// 绘制标尺 - 5等份
		baseWidth = rect().width() - 2 * margin - PtA.x();// 棒长极限
		nHeight = baseWidth / 5;
		for (i = 0; i < 6; i++)
		{
			PtB = QPoint(PtO.x() + i * nHeight, PtO.y());
			if (i > 0)
			{
// 				paint.moveTo(PtB.x(), PtB.y());
// 				paint.lineTo(PtB.x(), PtB.y() + 5);
				pa.moveTo(PtB.x(), PtB.y());
				pa.lineTo(PtB.x(), PtB.y() + 5);
			}
			str = QString::number(Coors[i]);
			nx = PtB.x() - FontSize(str).width() / 2;
			ny = PtB.y() + FontSize("A").height() + 3;
			paint.drawText(nx, ny, str);
		}
		paint.drawPath( pa );

		// 绘制棒图
		nHeight = PtO.y() - PtA.y() - margin;	// 棒图总高度
		nHeight /= m_Items->Count();			// 每个棒图宽度
		if (nHeight < MINHARMSIZE)
			nHeight = MINHARMSIZE;
		capx = PtA.y() + margin;
		for (i = 0; i < m_Items->Count(); i++)
		{
			item = m_Items->Item(i);
			if (m_fBaseValue < HARMLIMIT)
				rcHarm = QRect(PtA.x(), capx, 1, nHeight);
			else
			{
				rcHarm.setTop(capx);
				rcHarm.setHeight(nHeight);
				rcHarm.setLeft(PtA.x());
				rcHarm.setWidth(fabs(baseWidth * item->Re() / m_fBaseValue));
			}
			if (rcHarm.width() > baseWidth)
				rcHarm.setWidth(baseWidth);
			rcHarm.translate(0, nHeight * i);
			paint.fillRect(rcHarm, QBrush(item->Color()));
			paint.drawText(rcHarm.left() - FontSize("10").width() - 1,
				rcHarm.top() + FontSize("10").height(), item->Caption());
		}
		break;
	case hsHorz:// 水平方式
		// 坐标
		nDelta = FontSize("100").width() + margin;
		PtA = QPoint(nDelta, ptLine.y());
		nDelta = rect().bottom() - margin - FontSize("100").height();
		PtO = QPoint(PtA.x(), nDelta);
		PtB = QPoint(rect().right() - margin, PtO.y());
		paint.setPen(Qt::white);
// 		paint.moveTo(PtA.x() - 2, PtA.y());
// 		paint.lineTo(PtA.x() + 3, PtA.y());
// 		paint.moveTo(PtA.x(), PtA.y());
// 		paint.lineTo(PtO.x(), PtO.y());
// 		paint.lineTo(PtB.x(), PtB.y());
// 		paint.moveTo(PtB.x(), PtB.y() - 2);
// 		paint.lineTo(PtB.x(), PtB.y() + 5);

		pa.moveTo(PtA.x() - 2, PtA.y());
		pa.lineTo(PtA.x() + 3, PtA.y());
		pa.moveTo(PtA.x(), PtA.y());
		pa.lineTo(PtO.x(), PtO.y());
		pa.lineTo(PtB.x(), PtB.y());
		pa.moveTo(PtB.x(), PtB.y() - 2);
		pa.lineTo(PtB.x(), PtB.y() + 5);
		// 绘制标尺 - 5等份
		baseWidth = rect().bottom() - ptLine.y() - 3 * margin;
		nHeight = baseWidth / 5;
		
		for (i = 0; i < 6; i++)
		{
			PtA = QPoint(PtO.x(), PtO.y() - i * nHeight);
			if (i > 0)
			{
// 				paint.moveTo(PtA.x(), PtA.y());
// 				paint.lineTo(PtA.x() - 5, PtA.y());
				pa.moveTo(PtA.x(), PtA.y());
				pa.lineTo(PtA.x() - 5, PtA.y());
			}
			str = QString::number(Coors[i]);
			nx = PtA.x() - FontSize(str).width() - 6;
			ny = PtA.y() + FontSize("100").height() / 2 - 2;
			paint.drawText(nx, ny, str);
		}
		paint.drawPath(pa);
		// 绘制棒图
		nHeight = PtB.x() - PtO.x() - margin;	// 棒图总宽度
		nHeight /= m_Items->Count();			// 每个棒图宽度
		if (nHeight < MINHARMSIZE)
			nHeight = MINHARMSIZE;
		capx = PtO.x() + margin / 2;
		for (i = 0; i < m_Items->Count(); i++)
		{
			item = m_Items->Item(i);
			if (m_fBaseValue < HARMLIMIT)
				rcHarm = QRect(0, 0, 0, 0);
			else
			{
				rcHarm.setLeft(capx);
				rcHarm.setWidth(nHeight);
				rcHarm.setBottom(PtO.y());
				rcHarm.setTop(PtO.y() - fabs(baseWidth * item->Re() / m_fBaseValue));
				if (rcHarm.height() > baseWidth)
					rcHarm.setTop(rcHarm.bottom() - baseWidth);
				rcHarm.translate(nHeight * i, 0);
			}
			paint.fillRect(rcHarm, QBrush(item->Color()));
			int n1 = (rcHarm.width() - FontSize(item->Caption()).width()) / 2;
			paint.drawText(rcHarm.left() + n1, PtO.y() + FontSize("A").height() + 1, item->Caption());
		}
		break;
	default:
		break;
	}

	paint.end();
// 	bitBlt(this, 0, 0, &pm);	
	const QPointF point(0,0);
	paint.drawPixmap(point,pm);
}

void QHarmGraph::contextMenuEvent(QContextMenuEvent* e)
{
// 	emit HarmMenu();
}
