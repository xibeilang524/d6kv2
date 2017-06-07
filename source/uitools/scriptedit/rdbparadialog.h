#ifndef RDBPARADIALOG_H
#define RDBPARADIALOG_H
#include <QtCore/QString>
#include <QtCore/QVector>


#include "net/netapi.h"
#include "rdb/rdbapi.h"
#include "../hiscurve/dbextension.h"


#include "sysdef.h"

#define MAXKEY_NUM				6
//#define USERDBCLI 

#include <QtGui/QDialog>

//QT_BEGIN_NAMESPACE
class QComboBox;
class QGridLayout;
class QLabel;
class QDialogButtonBox;
class QGroupBox;
class QAbstractButton;
class QPushButton;
class QHBoxLayout;
//QT_END_NAMESPACE




class RdbParaDialog : public QDialog  
{
	Q_OBJECT
public:
	RdbParaDialog(QWidget* parent = 0, Qt::WFlags fl = Qt::Dialog);
private slots:
	void boxTableIdxChanged(int index);
	void filterBoxIdxChanged(int index);
	void buttonClicked(QAbstractButton* button);
	virtual void onBtnSelect();
private:
	QGridLayout *mainLayout;
	QGroupBox *groupBox;
	QLabel    *labelGroup,*labelTable,*labelObject,*labelDomain,*labelFilter,*labelStation,*labelType;
	QComboBox *boxGroup,*boxTable,*boxObject,*boxDomain,*boxStation,*boxType;
	QDialogButtonBox *buttonBox;
	QPushButton *m_btn_select;
	QHBoxLayout *btnLayout;
private:
	typedef struct _rdbtabledef
	{
		_rdbtabledef(){;}
		_rdbtabledef(QString name,QString descript,QString keyname[]){this->name=name,this->describe=descript;}
		QString name;
		QString describe;
		QString keyname[MAXKEY_NUM];
	}rdbtablemode;
	
	typedef struct _scada_objtype
	{
		uint	typeval;
		QString describe;
		QString tablename;
	}scada_objtype;

	QVector<rdbtablemode>	rdbTableModes;
	QVector<scada_objtype>  scdObjTypeDefs;
	void init(QWidget* parent);
	void initRDBTableMode();
	void initObjTypeTable();

	void addListGroup();        //use netstatus init it
	void addListTable();        //use dbi init it;
	void addListStation();      //use rdb imp init it

	void updListDomain();		//use 
	void updListObject();
	void updListObjType();
private:
	NetStatus netstat;

	
public:
	
};

#endif
