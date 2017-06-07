/**
@file wavecaption.h
@brief 绘制标题画板头文件
负责绘制曲线名称和当前游标位置、数据
@author lujiashun 
@version 1.0.0
@date 2016-11-01
*/
#ifndef WAVECAPTON_H
#define WAVECAPTON_H

#include <QWidget>
#include <QList>

#include "mouselinerect.h"
#include "wavegraph.h"

typedef enum Operate_Mode{omNone, omSelect, omDrag, omMouseLine};

class Wave_caption : public QWidget
{
	Q_OBJECT
public:
	Wave_caption(QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~Wave_caption();

private:
	Wave_graph* m_pGraph;
	QPainter*	m_Paint;
	Operate_Mode m_OperateMode;
	Draw_channel* m_pChannel;
	Mouse_line*	m_pMouseLine;
	QList<Draw_channel*>	m_lstSelection;	// 曲线数组

	bool	m_bAutoSize;
	int		m_nLockCnt;
	QColor	m_backgroundColor;
	QColor	m_Color;
	QColor	m_clrOld;
	bool	m_bLeftPressed;
	int		m_nMousePointsMove;
	int m_current_position;	//当前x轴位置

signals:
	void OnSelect(int idx);					// 选择一条通道
	void signal_auto_hint(const QString& hint);	// 自动提示

	void CaptionMenu();

protected slots:
	// elastic，橡皮筋
	void	slot_elastic_move(QRect rct);
	void	slot_elastic_up(QRect rct);

	// QWaveGraph
	void	slot_graph_move(int action);
	void	slot_graph_unselect();
	void	slot_graph_add_select(void* ptr);
	void	slot_graph_delete(void* ptr);

	//Cursors move
	//在caption旁边显示当前游标的时间和值
	void slot_cursor_move(int pos);

protected:
	void	Lock();		// Lock Design Pattern
	void	UnLock();
	bool	Locked();
	void	SetLocking(bool updating);

	void	Change()				{ repaint(); }
	int		Lefter();
	bool	litter_rect(QRect r);
	bool	get_selected(int index);
	QSize	FontSize(const QString& str);
	Draw_channel*	get_selected_by_pos(int x, int y);

	void	UpdateOperateMode();
	void	SetOperateMove(Operate_Mode om);

protected:
	// mouse
	void	mousePressEvent( QMouseEvent* );
	void	mouseMoveEvent ( QMouseEvent* );
	void	mouseReleaseEvent ( QMouseEvent* );
	void	mouseDoubleClickEvent ( QMouseEvent* );

	// keyboard
	void	keyPressEvent ( QKeyEvent* );

	// draw
	void	paintEvent ( QPaintEvent* );
	void	resizeEvent ( QResizeEvent* );

	// menu
	void contextMenuEvent ( QContextMenuEvent* e );

public:
	void	Clear();
	void	UnSelectAll();
	void	BeSamePosition();
	void	BeSameScale();
	void	BeAutoPosition();
	int		MaxCaptionSize();
	void	BeAutoSize();
	int		SelCount();
	void	AddSelected(Draw_channel* channel);
	virtual	void DeleteSelected(int channel);
	virtual void DeleteSelected(Draw_channel* channel);
	void	CopyGraph(bool b = true);

	bool	AutoSize()				{ return m_bAutoSize; }
	void	SetAutoSize(bool b);
	Wave_graph* Graph()				{ return m_pGraph; }
	void	SetGraph(Wave_graph* graph);
	QColor	Color()					{ return m_Color; }
	void	SetColor(QColor c);
	QColor	BackgroundColor()					{ return m_backgroundColor; }
	void	SetBackgroundColor(QColor c);
	Draw_channel* Channel()			{ return m_pChannel; }
	void	SetChannel(Draw_channel* p);
	bool	HasDigtal();
	void set_current_position(int pos);
	int get_current_position()	{return m_current_position;}
	
};

#endif