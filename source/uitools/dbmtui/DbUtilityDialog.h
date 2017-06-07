#ifndef DBUTILITYDIALOG_H
#define DBUTILITYDIALOG_H

#include <QDialog>

#define DD_DB_STYLE	0
#define PB_DB_STYLE	1
#define DL_DB_STYLE	2

namespace Ui {class pb_Dialog;}
namespace Ui {class dd_Dialog;}
namespace Ui {class dl_Dialog;}

class QString;
class QStringList;
class QProcess;
class QDir;

class DbUtilityDlg : public QDialog 
{
	Q_OBJECT

public:
	DbUtilityDlg(int DlgStyle,QWidget *parent = 0);
	~DbUtilityDlg();

public:
	int		m_nDbStyle;		//当前的数据库界面类型
	int		m_dbtype ;
	QString m_fileName;
	QString m_filePath;
	QString m_userName;
	QString m_password;
	QStringList m_pwTablesList ;

private:
	Ui::pb_Dialog *pb_ui;
	Ui::dd_Dialog *dd_ui;
	Ui::dl_Dialog *dl_ui;

public slots:
	void begin();
	void exit();
	void about();
 	void tab_selChanged(int index);
	void importDb();
	void exportDb();
	void userSelChanged();
	void slotChekmdlChanged(int state);

protected:
	int  GetDLTables(QStringList& tables);
	int  GetPWTables(QStringList& tables);
	//int  GetexportTables(QStringList& tables);   //自动导出时读取表
	void exportDL();
	void exportPWTables();
	void importDL();
    void importPWTables();
	void cleanDL();
	void cleanPWTables();
	QProcess *m_pProcess;

	bool copyGraphFullDir(QDir from,QDir dest);

private:
	QString getDLFilename();
	QString getPWFilename();
	int readPWTables( QStringList& tables ) ;
	void ini_style_sheet( void ) ;

public:
//	void exportTable();							//自动导出表
};
#endif