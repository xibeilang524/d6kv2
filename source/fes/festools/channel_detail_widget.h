/**
@file channel_detail_widget.h
@brief Í¨µÀ×´Ì¬´°¿Ú

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef CHANNELDETAILWIDGET_H_
#define CHANNELDETAILWIDGET_H_

#include <QTableWidget>
#include <QMap>
#include <QList>
#include "fes/fesshm.h"

class channel_detail_widget : public QTableWidget
{
	Q_OBJECT
public:
	channel_detail_widget( QWidget* parent = NULL ) ;
	void refresh_channel_table() ;

protected:

private:
	void ini_channel_stat_table() ;
	void ini_status_list() ;
	bool read_protocol_para() ;
	int calc_real_channel_cnt() ;

private:
	int								m_host_no ;
	Fes_shm&					m_fes_shm ;
	QList<QString>		m_status_list ;
	QMap<QString, QString> m_protocol_map ;
};

#endif