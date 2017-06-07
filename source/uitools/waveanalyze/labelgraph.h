#ifndef LABELGRAPH_H
#define LABELGRAPH_H

#include <qsize.h>
#include <qwidget.h>
#include <qfontmetrics.h>

#include "vectoritem.h"

const int MINFONTSIZE  = 9;
const int LABEL_BORDERSIZE = 3;
const int DEFAULTFONTSIZE = 12;

// 需要抽象出一个可以执行doublebuffer的绘制基类，由子类来执行virtual的draw方法；

// 标识控件
class QLabelGraph : public QWidget
{
	Q_OBJECT
public:
	QLabelGraph(QWidget* parent);
	~QLabelGraph();

	void Clear(bool bRepaint = true);
	QColor BkColor()			{ return m_Color; }
	QVectorMgr* Items()			{ return m_Items; }
	void SetBkColor(const QColor& clr);

protected:
	int MaxSize();
	QSize FontSize(const QString& str);
	void paintEvent(QPaintEvent* e);

public slots:
	void Change();

private:
	QColor m_Color;
	QVectorMgr* m_Items;

};

#endif