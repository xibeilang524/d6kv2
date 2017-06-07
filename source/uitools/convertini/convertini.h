/** 
@file convertini.h
@brief convertini

@vers v1.2.0
@author libinghong
@date 2017年1月16日 
*/
#ifndef CONVERINI_H
#define CONVERINI_H

#include "ui_convertini.h"

#include <QtCore/QDir>
#include <QtGui/QMainWindow>

class ConvertIni : public QMainWindow , public Ui::covertbase
{
	Q_OBJECT
public:
	ConvertIni( QWidget* parent = NULL, const char* name = NULL );
	~ConvertIni();

public slots:
	/**
	@brief 打开文件并显示
	*/
	void slot_open_file();
	void slot_ini_xml();
public:
	int getFileList(QStringList*  resultList,QDir director,QString  filter);

private:
	QStringList	m_fileList;	//所有ini的列表
	QString		m_syspath;
};

#endif