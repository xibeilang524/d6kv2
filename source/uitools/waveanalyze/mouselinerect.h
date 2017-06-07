///////////////////////////////////////////////////////////////
//	文 件 名 :MouseLineRect.h
//	文件功能 : 橡皮筋
//	作    者 : 邱俊宏
//	创建时间 : 2005年8月11日 11::22
//	项目名称 ：
//	操作系统 : 
//	备    注 :
//	历史记录： : 
///////////////////////////////////////////////////////////////
#ifndef MOUSELINERECT_H
#define MOUSELINERECT_H

#include <qobject.h>
#include <qcolor.h>
#include <qrect.h> 
#include <qpoint.h>
#include <qnamespace.h> 
#include <qpainter.h> 
#include <qpaintdevice.h> 


class Mouse_line : public QObject
{
	Q_OBJECT

public:
	Mouse_line(QPaintDevice* dev)
	{
		m_Widget = dev;
		m_ptRange = QPoint(10, 10);
		m_Pen = new QPen(Qt::white, 1, Qt::SolidLine);
	}
	~Mouse_line() 
	{
		delete m_Pen;
	}

private:
	QPoint		m_ptRange;
	QColor		m_Color;
	QRect		m_rctRoom;
	QPen*		m_Pen;
	QPaintDevice* m_Widget;

signals:
	void signal_move(QRect rct);
	void signal_up(QRect rct);

protected:
	QRect RoomRect()
	{
		return m_rctRoom.normalized();
	}
	void LineRect(int x1, int y1, int x2, int y2)
	{
		QPainter paint(m_Widget);
		paint.setBrush(Qt::NoBrush);
// 		paint.setROP2(QPainter::RasterOp_SourceXorDestination);
		paint.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		paint.setPen(*m_Pen);
		int a;
		if (x1 > x2)
		{
			a = x1; x1 = x2; x2 = a;
		}
		if (y1 > y2)
		{
			a = y1; y1 = y2; y2 = a;
		}
		paint.drawRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
		paint.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
	}

public:
	void DownMouse(int x, int y) 
	{ 
		m_rctRoom = QRect(QPoint(x, y), QPoint(x, y)); 
	}
	void MoveMouse(int x, int y)
	{
		LineRect(m_rctRoom.left(), m_rctRoom.top(), m_rctRoom.right(), m_rctRoom.bottom());
		int x2 = x;
		int y2 = y;
		if (x < 1) x2 = 1;
		if (x > m_ptRange.x()) x2 = m_ptRange.x() - 2;
		if (y < 1) y2 = 1;
		if (y > m_ptRange.y()) y2 = m_ptRange.y();
		m_rctRoom.setRight(x2);
		m_rctRoom.setBottom(y2);
		LineRect(m_rctRoom.left(), m_rctRoom.top(), x2, y2);
		emit signal_move(RoomRect());
	}
	void UpMouse()
	{
		LineRect(m_rctRoom.left(), m_rctRoom.top(), m_rctRoom.right(), m_rctRoom.bottom());
		emit signal_up(RoomRect());
		m_rctRoom = QRect(0, 0, 0, 0);
	}
	void	ReDraw() {}
	QRect	LastRoom()				{ return m_rctRoom; }
	void	SetColor(QColor c)		{ m_Pen->setColor(c); }
	void	SetRange(QPoint r)		{ m_ptRange = r; }	

}; 

#endif