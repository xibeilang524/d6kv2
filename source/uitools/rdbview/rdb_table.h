/**
@file rdb_table.h
@brief 

@author chenkai
@version 1.2.0
@date 2016-11-3
*/
#ifndef RDBTABLE_H
#define RDBTABLE_H

#include <QTableWidget>

class Rdb_table : public QTableWidget
{
	Q_OBJECT
public:
	Rdb_table( QWidget* parent = 0 ) ;
protected:
private:
	virtual void paintCell( int row , int col , const QBrush & brush ) ;
};

#endif