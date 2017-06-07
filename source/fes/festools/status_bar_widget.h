/**
@file status_bar_widget.h
@brief ×´Ì¬À¸

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef STATUSBARWIDGET_H_
#define STATUSBARWIDGET_H_

#include <QObject>

class status_bar_widget : public QObject
{
	Q_OBJECT
public:
	status_bar_widget( QWidget * parent = 0 ) ;
	void add_status_widget( QWidget* widget , int width ) ;

protected:
	bool eventFilter( QObject* , QEvent* );

private:
	QList<QWidget* >		m_widget_list;
	QList<int *> 		m_width_list;
};

#endif