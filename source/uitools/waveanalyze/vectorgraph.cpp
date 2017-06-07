#include <qpainter.h>
#include <qpixmap.h>
#include "vectorgraph.h"


QVectorGraph::QVectorGraph(QWidget* parent)
	: QWidget(parent)
{
	m_bShowIndex = false;
	m_nItemIndex = -1;
	m_Color = Qt::black;
	m_clrScale = Qt::white;
	m_nScale = 1;
	m_clrAngle = Qt::white;
	m_nAngle = 0;
	m_Items = new QVectorMgr;
// 	setBackgroundColor(m_Color);
	SetColor(m_Color);
}

QVectorGraph::~QVectorGraph()
{
	delete m_Items;
}

void QVectorGraph::Clear()
{
	m_nItemIndex = -1;
	m_Items->Clear();
	Change();
}

void QVectorGraph::SetItemIndex(int n)
{
	if (m_nItemIndex != n)
	{
		m_nItemIndex = n;
		Change();
	}
}

void QVectorGraph::SetColor(const QColor& clr)
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

void QVectorGraph::SetScaleColor(const QColor& clr)
{
	if (m_clrScale != clr)
	{
		m_clrScale = clr;
		Change();
	}
}

void QVectorGraph::SetScaleCount(int n)
{
	if (m_nScale != n)
	{
		m_nScale = n;
		Change();
	}
}

void QVectorGraph::SetAngleColor(const QColor& clr)
{
	if (m_clrAngle != clr)
	{
		m_clrAngle = clr;
		Change();
	}
}

void QVectorGraph::SetAngleCount(int n)
{
	if (m_nAngle != n)
	{
		m_nAngle = n;
		Change();
	}
}

void QVectorGraph::SetShowIndex(bool b)
{
	if (m_bShowIndex != b)
	{
		m_bShowIndex = b;
		Change();
	}
}

void QVectorGraph::Change()
{
// 	repaint();
	update();
}

QSize QVectorGraph::FontSize(const QString& str)
{
	QFontMetrics fm = fontMetrics();
	return QSize(fm.width(str), fm.height());
}

// Idx-索引 Radis-半径
void QVectorGraph::DrawVector(QPainter* paint, int idx, double radis)
{
	QVectorItem* item = m_Items->Item(idx);
	if (item == NULL || (! item->IsShow()))
		return;

	double fVal = radis * item->Re();
	fVal = fVal > radis ? radis : fVal;
	double fAng = item->Im() * PI / 180.0;

	// 根据基准进行相角偏移
	QVectorItem* baseItem = m_Items->Item(m_nItemIndex);
	if (baseItem != NULL)
		fAng = (item->Im() -  baseItem->Im()) * PI / 180.0;

	QPoint ptVect = QPoint(fVal * cos(fAng), fVal * sin(fAng));
	double fAng1 = fAng - 10.0 * PI / 180.0;
	QPoint pt1 = QPoint(m_ptZero.x() + ptVect.x() - ARROWLENGHT * cos(fAng1),
						m_ptZero.y() - (ptVect.y() - ARROWLENGHT * sin(fAng1)));

	fAng1 = fAng + 10.0 * PI / 180.0;
	QPoint pt2 = QPoint(m_ptZero.x() + ptVect.x() - ARROWLENGHT * cos(fAng1),
						m_ptZero.y() - (ptVect.y() - ARROWLENGHT * sin(fAng1)));

	ptVect.setX(ptVect.x() + m_ptZero.x());		// 坐标转换
	ptVect.setY(m_ptZero.y() - ptVect.y());	

	paint->setPen(item->Color());

	QPainterPath pa;
	pa.moveTo(m_ptZero.x(), m_ptZero.y());	// 矢量线
	pa.lineTo(ptVect.x(), ptVect.y());

	pa.moveTo(ptVect.x(), ptVect.y());		// 矢量箭头
	pa.lineTo(pt1.x(), pt1.y());
	pa.lineTo(pt2.x(), pt2.y());
	pa.lineTo(ptVect.x(), ptVect.y());
	paint->drawPath(pa);

	QString str;
	if (m_bShowIndex)
		str.sprintf("CH%d", item->Tag());
	else
		str = item->Caption();
	paint->drawText(ptVect.x() + 5, ptVect.y() + 5, str);
}

void QVectorGraph::paintEvent(QPaintEvent* e)
{
	QPixmap pm(size());
	QPainter paint;
	paint.begin(&pm);

	paint.fillRect(rect(), QBrush(m_Color));
	QRect rcGraph = rect();
	int nDelta =  FontSize(ANGLECAPTION[3]).width() / 2 + 8;
	rcGraph.adjust(nDelta, nDelta, -1 * nDelta, -1 * nDelta);
	if (rcGraph.right() > rcGraph.bottom())
		rcGraph.setRight(rcGraph.bottom());
	else
		rcGraph.setBottom(rcGraph.right());
	
	// 背景圆框
	m_ptZero = QPoint((rcGraph.right() + rcGraph.left()) / 2, 
					  (rcGraph.bottom() + rcGraph.top()) / 2);
	paint.setPen(Qt::white);
	paint.drawEllipse(rcGraph);

	// 刻度分格
	if (m_nScale > 0)
	{
		paint.setPen(m_clrScale);
//		paint.setBrush();
		paint.setBrush(Qt::NoBrush);
		QRect rcScale = rcGraph;
		int nLen = rcGraph.width() / m_nScale / 2;
		for (int x = 1; x < m_nScale; x++)
		{
 			rcScale.adjust(nLen, nLen, -1 * nLen, -1 * nLen);
			paint.drawEllipse(rcScale);
		}
	}
	
	// 坐标轴
	QPoint ptCross[4];
	ptCross[0] = QPoint(m_ptZero.x(), rcGraph.top());		// +90
	ptCross[1] = QPoint(rcGraph.right(), m_ptZero.y());		// 0
	ptCross[2] = QPoint(ptCross[0].x(), rcGraph.bottom());	// -90
	ptCross[3] = QPoint(rcGraph.left(), m_ptZero.y());		// 180
	paint.setPen(m_clrAngle);
// 	paint.moveTo(ptCross[0].x(), ptCross[0].y());
// 	paint.lineTo(ptCross[2].x(), ptCross[2].y());
// 	paint.moveTo(ptCross[1].x(), ptCross[1].y());
// 	paint.lineTo(ptCross[3].x(), ptCross[3].y());
	
	QPainterPath pa;
	pa.moveTo(ptCross[0].x(), ptCross[0].y());
	pa.lineTo(ptCross[2].x(), ptCross[2].y());
	pa.moveTo(ptCross[1].x(), ptCross[1].y());
	pa.lineTo(ptCross[3].x(), ptCross[3].y());
	paint.drawPath(pa);

	int x = FontSize(ANGLECAPTION[0]).width();
	int y = FontSize(ANGLECAPTION[0]).height();
	ptCross[0].setX(ptCross[0].x() - x/2 + 3);	// +90
	ptCross[0].setY(ptCross[0].y() - 3);
	x =  FontSize(ANGLECAPTION[1]).width();
	ptCross[1].setX(ptCross[1].x() + 3);		// 0
	ptCross[1].setY(ptCross[1].y() + y/2);
	x =  FontSize(ANGLECAPTION[2]).width();
	ptCross[2].setX(ptCross[0].x());			// -90
	ptCross[2].setY(ptCross[2].y() + y);
	ptCross[3].setX(ptCross[3].x() - nDelta);	// -180
	ptCross[3].setY(ptCross[1].y());

	double Radis = rcGraph.height() / 2;

	// 角度分格
	double fAng = 0.0;
	if (m_nAngle > 0)
	{
		int nCount = 360 / ANGLESIZE[m_nAngle - 1];
		for (x = 0; x < nCount; x++)
		{
			fAng += ANGLESIZE[m_nAngle - 1] * PI / 180.0;
			double fx = m_ptZero.x() + Radis * cos(fAng);
			double fy = m_ptZero.y() - Radis * sin(fAng);
/*
			QPoint ptAng = QPoint(m_ptZero.x() + Radis * cos(fAng),
								  m_ptZero.y() - Radis * sin(fAng));
*/
// 			paint.moveTo(m_ptZero.x(), m_ptZero.y());
// 			paint.lineTo(fx, fy);
			pa.moveTo(m_ptZero.x(), m_ptZero.y());
			pa.lineTo(fx, fy);
			paint.drawPath(pa);
//			paint.lineTo(ptAng.x(), ptAng.y());
		}
	}

	// 角度标识
	for (x = 0; x < 4; x++)
		paint.drawText(ptCross[x], ANGLECAPTION[x]);

	// 矢量线
	for (x = 0; x < m_Items->Count(); x++)
		DrawVector(&paint, x, Radis);

	paint.end();
// 	bitBlt(this, 0, 0, &pm);	
	const QPointF point(0,0);
	paint.drawPixmap(point,pm);
}

void QVectorGraph::contextMenuEvent(QContextMenuEvent* e)
{
// 	emit VectorMenu();
}
