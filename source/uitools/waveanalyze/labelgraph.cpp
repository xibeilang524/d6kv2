#include <qpixmap.h>
#include <qpainter.h>
#include "labelgraph.h"


/* QLabelGraph */

QLabelGraph::QLabelGraph(QWidget* parent)
	: QWidget(parent)
{
	m_Color = Qt::black;
	m_Items = new QVectorMgr;
// 	setBackgroundColor(m_Color);
	SetBkColor(m_Color);
}

QLabelGraph::~QLabelGraph()
{
	Clear(false);
	delete m_Items;
}

void QLabelGraph::SetBkColor(const QColor& clr)
{
	if (m_Color != clr)
	{
		m_Color = clr;
// 		setBackgroundColor(m_Color);
		QPalette palette;
		palette.setColor(backgroundRole(),m_Color);
		setPalette(palette);
		repaint();
	}
}

void QLabelGraph::Clear(bool bRepaint)
{
	m_Items->Clear();
	if (bRepaint)
		repaint();
}

QSize QLabelGraph::FontSize(const QString& str)
{
	QFontMetrics fm = fontMetrics();
	return QSize(fm.width(str), fm.height());
}

int QLabelGraph::MaxSize()
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

// 支持多行显示
void QLabelGraph::paintEvent(QPaintEvent* e)
{
	QPainter paint;
	QPixmap pm(size());
	paint.begin(&pm);
	paint.fillRect(rect(), QBrush(m_Color));

	int i;
	if (m_Items->Count() > 1)
	{
		int count = 0;
		for (i = 0; i < m_Items->Count(); i++)
			if (m_Items->Item(i)->m_bShow)
				count++;
		int row = (rect().height() - LABEL_BORDERSIZE) / (MINFONTSIZE + LABEL_BORDERSIZE);
		int col = count / row;
		if (count % row > 0)
			col++;

		count = 0;
		int fontSize = MINFONTSIZE;
		QFont ft = font();
		ft.setPointSize(MINFONTSIZE);
		setFont(ft);
		int size = MaxSize() + 10 + 5;
		int nHeight = FontSize("A").height();
		for (i = 0; i < col; i++)
		{
			QRect rcCaption;
			rcCaption.setLeft(LABEL_BORDERSIZE + i * size);
			rcCaption.setTop(2 * LABEL_BORDERSIZE);
			rcCaption.setRight(10 + rcCaption.left());
			rcCaption.setBottom(fontSize + rcCaption.top());
			int j = 0;
			while (j < row)
			{
				if (count >= m_Items->Count())
					break;
				QVectorItem* item = m_Items->Item(count);
				count++;
				if (!item->IsShow())
					continue;
				if (j > 0)
					rcCaption.translate(0, LABEL_BORDERSIZE + fontSize);
				j++;
				paint.fillRect(rcCaption, QBrush(item->Color()));
				paint.setPen(item->Color());
				paint.drawText(QPoint(rcCaption.right() + 5, rcCaption.top() + nHeight / 2 + 3), 
					item->Caption());
			}
		}
	}
	paint.end();
// 	bitBlt(this, 0, 0, &pm);	
	const QPointF point(0,0);
	paint.drawPixmap(point,pm);
}

void QLabelGraph::Change()
{
	repaint();
}