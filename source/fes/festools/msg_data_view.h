/**
@file msg_data_view.h
@brief 报文展示界面

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef MSGDATAVIEW_H_
#define MSGDATAVIEW_H_

#include <QAbstractScrollArea>
#include "msg_modular.h"

class msg_data_view : public QAbstractScrollArea
{
	Q_OBJECT
public:
	msg_data_view( QWidget* parent = NULL ) ;
	void draw_modular_data( msg_modular_data* pModula_data, QPainter* pPainter ) ;
	void update_scroll();
	void set_up_down(int up_down) ;

protected:

private:
	void paintEvent( QPaintEvent* pe ) ;
	void resizeEvent( QResizeEvent *re ) ;
	bool match_keyword( unsigned char msg ) ;
	void scroll_contents_by( int dx, int dy ) ;
	int calc_total_line(); 

public:
	int m_total_line;					//当前页面总行数
	int m_char;								//m_char = 29  一行显示的报文数
	int m_scrollview_width;		//scrollview部件像素宽度
	int m_analysis_msg_state;	//复选框状态，1解析报文，0不解析
	int m_desc_char;					//未知变量
	int m_current_line ;				//当前行数
	msg_modular* m_pCurrent_msg_modular;
	QList<unsigned char> m_keyword_list;

private:
	int m_char_width;					//字符宽度，m_nCharWidth 为5，字符宽度
	int m_line_height;					//字符高度
	int m_up_down;						//0:上下行 1:下行 2:上行
};

#endif