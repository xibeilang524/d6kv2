/**
@file convertools.h
@brief 转换工具头文件

@author ljh
@version 1.0
@date 2017-3-16
*/
#ifndef CONVERTOOLS_H
#define CONVERTOOLS_H

#include <QMainWindow>
#include "ddes/GGraphFile.h"
#include "ddef/GDevFile.h"
#include "ui_convertools.h"

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data())) 
#define _C2Q(str) (QString::fromLocal8Bit(str))

class Convertools : public QMainWindow , public Ui::convertbase
{
	Q_OBJECT
public:
	Convertools( QWidget* parent = NULL , const char* name = NULL);
	~Convertools();

public:
	bool gdf_to_xml( QString &gdfpath , QString &xmlpath ); //gdf转xml并修改xml
	bool xml_to_bgf( QString &xmlpath , QString &bgfpath );//xml转bgf
	int  get_file_list( QStringList &resultList , QDir director , QString filter );//获取文件列表

public slots:
	void slot_open_dir(); //打开文件夹
	void slot_open_file();//打开一个或多个文件
	void slot_del_filelist();//删除一个或多个文件
	void slot_search_file();
	void slot_gdf_to_bgf();//gdf转bgf
	void slot_about();
	void slot_exit();

private:
	QStringList m_file_list ; //文件列表
	QString m_file_path;//默认路径

};
#endif