#ifndef VECTORGRAPH_H
#define VECTORGRAPH_H


// #include "globaldef.h"
#include <QObject>
const double VECTORLIMIT = 0.1;		// 矢量最小门槛值
//const int V_BORDERSIZE = 5;
const int ARROWLENGHT = 10;
const int ANGLESIZE[] = {30, 45};
const QString ANGLECAPTION[] = {QObject::tr("+90°"), QObject::tr("0°"), QObject::tr("-90°"), QObject::tr("180°")};

#include <math.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include "vectoritem.h"
#include "mathutils.h"

// 矢量图控件
class QVectorGraph : public QWidget
{
	Q_OBJECT
public:
	QVectorGraph(QWidget* parent);
	~QVectorGraph();

	int ItemIndex()				{ return m_nItemIndex; }
	QColor Color()				{ return m_Color; }
	bool IsShowIndex()			{ return m_bShowIndex; }
	QColor ScaleColor()			{ return m_clrScale; }
	int ScaleCount()			{ return m_nScale; }
	QColor AngleColor()			{ return m_clrAngle; }
	int AngleCount()			{ return m_nAngle; }

	void SetItemIndex(int n);
	void SetColor(const QColor& clr);
	void SetScaleColor(const QColor& clr);
	void SetScaleCount(int n);
	void SetAngleColor(const QColor& clr);
	void SetAngleCount(int n);
	void SetShowIndex(bool b);

	void Clear();
	int Count()					{ return m_Items->Count(); }		
	QVectorMgr* Items()			{ return m_Items; }

	void Change();

signals:
// 	void VectorMenu();

protected:
	void DrawVector(QPainter* paint, int idx, double radis);
	QSize FontSize(const QString& str);
	void paintEvent(QPaintEvent* e);
	void contextMenuEvent(QContextMenuEvent* e);

private:
	int		m_nItemIndex;		// 矢量基准
	QColor	m_Color;
	bool	m_bShowIndex;		// 显示索引
	QColor	m_clrScale;			// 刻度颜色
	int		m_nScale;			// 刻度分格数
	QColor	m_clrAngle;			// 角度颜色
	int		m_nAngle;			// 角度分格数

	QVectorMgr* m_Items;// 要求: TVeItem的Re、Im是幅值+相角
	QPoint m_ptZero;

};


#endif