/**
@file list_view_color_item.h
@brief 事项表格单元格类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef LIST_VIEW_COLOR_ITEM_H
#define LIST_VIEW_COLOR_ITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QColor>
#include <QBrush>
#include "event_cfg.h"

class List_view_color_item : public QTreeWidgetItem  
{
public:
	List_view_color_item( Event_cfg *pCfg, QTreeWidget * parent, QColor &clr );
	virtual ~List_view_color_item();

// 	virtual void paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align );
	virtual void paintCell ( int column/*, const QBrush & brush*/ );

    virtual int compare( QTreeWidgetItem *i, int col, bool ) const;

	bool operator< ( const QTreeWidgetItem & other ) const ;

protected:
	QColor m_color;
	QBrush m_fore_brush ;
	QBrush m_back_brush ;

private:
	Event_cfg			*m_cfg;

};

// class QCompareStr : public QString
// {
// public:
// 	QCompareStr(){}
// 	QCompareStr( const QChar * unicode, int size ):QString( unicode , size ){}
// 	QCompareStr( const QChar * unicode ):QString( unicode ){}
// 	QCompareStr( QChar ch ):QString( ch ){}
// 	QCompareStr( int size, QChar ch ):QString( size , ch ){}
// 	QCompareStr( const QLatin1String & str ):QString( str ){}
// 	QCompareStr( const QString & other ):QString( other ){}
// 	QCompareStr( const char * str ):QString( str ){}
// 	QCompareStr( const QByteArray & ba ):QString( ba ){}
// 
// 	int compare ( const QCompareStr & s1, const QCompareStr & s2, Qt::CaseSensitivity cs )
// 	{
// 		int rt = s1.toInt() - s2.toInt();
// 		if( rt == 0 )
// 			return rt;
// 		else
// 			return rt > 0 ? 1 : -1;
// 	}
// };
#endif // !defined(AFX_QLISTVIEWCOLORITEM_H__6B5806D9_E2C5_46E5_BA98_9B33B8E16E8B__INCLUDED_)
