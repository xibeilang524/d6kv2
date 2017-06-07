// rdbparadialog.cpp: implementation of the RdbParaDialog class.
//
//////////////////////////////////////////////////////////////////////
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QAbstractButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCompleter>

#include "appdef.h"
#include "mainwindow.h"
#include "rdbparadialog.h"
#include "ddisp/gstationselector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


static	char *szTableName[]={"analog","break","switch","status","prot","kwh","multistatus","terminal","channel","calculate","device","eventexpress","hoststatus","powline","server","station","userproc"};
DBExtension  dbex;


RdbParaDialog::RdbParaDialog( QWidget* parent, Qt::WFlags fl)
:QDialog( parent, fl )
{
	init(this);
	initRDBTableMode();
	initObjTypeTable();

	addListGroup();
	addListTable();
	addListStation();

}

//////////////////////////////////////////////////////////////////////////
void RdbParaDialog::boxTableIdxChanged( int index )
{
	qDebug()<<"RdbParaDialog::boxTableIdxChanged";
	if(index<0) return;
	updListObjType();
	updListDomain();
}

void RdbParaDialog::filterBoxIdxChanged( int index )
{
	qDebug()<<"RdbParaDialog::filterBoxIdxChanged";
	updListObject();
}

void RdbParaDialog::onBtnSelect()
{
	Gstation_selector station_selector;
	QString code = station_selector.get_code();
	QString desc = station_selector.get_desc();
	int hIndex = boxStation->findText( desc );
	if (hIndex==-1)
	{
		return;
	}
	boxStation->setCurrentIndex( hIndex );
}


void RdbParaDialog::buttonClicked( QAbstractButton* button )
{
	QDialogButtonBox::ButtonRole role=buttonBox->buttonRole(button);
	if(role==QDialogButtonBox::RejectRole)
		reject();
	else if(role==QDialogButtonBox::ApplyRole)
	{
		QString tabname,objname,domainname,rdbpara;
		if(boxTable->currentIndex()>=0)
			tabname=boxTable->itemData(boxTable->currentIndex()).toString();
		if(boxDomain->currentIndex()>=0)
			domainname=boxDomain->itemData(boxDomain->currentIndex()).toString();
		if(boxObject->currentIndex()>=0)
			objname=boxObject->itemData(boxObject->currentIndex()).toString();
		if(tabname.isNull() || objname.isNull() || domainname.isNull())
			return;
		MainWindow::mainWindow()->insertText(QString("%1.%2.%3").arg(tabname).arg(objname).arg(domainname));
	}
	else
		return;
}



//////////////////////////////////////////////////////////////////////////

void RdbParaDialog::init( QWidget* parent)
{	
	parent->setWindowTitle(tr("插入实时库对象"));
	mainLayout=new QGridLayout(parent);
	
	boxGroup=new QComboBox(parent);
	boxGroup->setObjectName(QString::fromUtf8("boxGroup"));
	boxGroup->setEditable(false);
	labelGroup=new QLabel(tr("组号(&G)"),parent);
	labelGroup->setObjectName(QString::fromUtf8("labelGroup"));
	labelGroup->setBuddy(boxGroup);
	mainLayout->addWidget(labelGroup,0,0);
	mainLayout->addWidget(boxGroup,0,1);

	
	boxTable=new QComboBox(parent);
	boxTable->setObjectName(QString::fromUtf8("boxTable"));
	boxTable->setEditable(false);
	labelTable=new QLabel(tr("表名(&B)"),parent);
	labelTable->setObjectName(QString::fromUtf8("labelTable"));
	labelTable->setBuddy(boxTable);
	mainLayout->addWidget(labelTable,1,0);
	mainLayout->addWidget(boxTable,1,1);
	connect(boxTable, SIGNAL(currentIndexChanged(int)), this, SLOT(boxTableIdxChanged(int)));

	
	boxDomain=new QComboBox(parent);
	boxDomain->setObjectName(QString::fromUtf8("boxDomain"));
	labelDomain=new QLabel(tr("域名(&D)"),parent);
	labelDomain->setObjectName(QString::fromUtf8("labelDomain"));
	labelDomain->setBuddy(boxDomain);
	mainLayout->addWidget(labelDomain,2,0);
	mainLayout->addWidget(boxDomain,2,1);
	
	boxObject=new QComboBox(parent);
	boxObject->setObjectName(QString::fromUtf8("boxObject"));
	labelObject=new QLabel(tr("对象名(&L)"),parent);
	labelObject->setObjectName(QString::fromUtf8("labelObject"));
	labelObject->setBuddy(boxObject);
	mainLayout->addWidget(labelObject,3,0);
	mainLayout->addWidget(boxObject,3,1);
	
	labelFilter=new QLabel(tr("检索过滤方式"),parent);
	labelFilter->setObjectName(QString::fromUtf8("labelFilter"));
	mainLayout->addWidget(labelFilter,4,0);
	
	boxStation=new QComboBox(parent);
	boxStation->setObjectName(QString::fromUtf8("boxStation"));
	labelStation=new QLabel(tr("按厂站名(&R)"),parent);
	labelStation->setObjectName(QString::fromUtf8("labelStation"));
	labelStation->setBuddy(boxStation);

	mainLayout->addWidget(labelStation,5,0);

	m_btn_select = new QPushButton(QWidget::tr("选择"), parent);
	m_btn_select->setObjectName(QString::fromUtf8("m_btn_select"));
	m_btn_select->setFixedWidth(40); 
	//mainLayout->addWidget(m_btn_select,5,2);
	connect(boxStation, SIGNAL(currentIndexChanged(int)), this, SLOT(filterBoxIdxChanged(int)));
	btnLayout = new QHBoxLayout(parent);
	btnLayout->addWidget(boxStation); 
	btnLayout->addWidget(m_btn_select); 
	mainLayout->addLayout(btnLayout,5,1);
    connect(m_btn_select, SIGNAL(clicked()), this, SLOT(onBtnSelect()));
	boxType=new QComboBox(parent);
	boxType->setObjectName(QString::fromUtf8("boxType"));
	labelType=new QLabel(tr("按对象类型(&T)"),parent);
	labelType->setObjectName(QString::fromUtf8("labelType"));
	labelType->setBuddy(boxType);
	mainLayout->addWidget(labelType,6,0);
	mainLayout->addWidget(boxType,6,1);
	connect(boxType, SIGNAL(currentIndexChanged(int)), this, SLOT(filterBoxIdxChanged(int)));
	
	buttonBox=new QDialogButtonBox(QDialogButtonBox::Apply | QDialogButtonBox::Cancel,Qt::Horizontal,parent);
	connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
	mainLayout->addWidget(buttonBox,7,1);

	parent->setLayout(mainLayout);
}


void RdbParaDialog::initRDBTableMode()
{
	int				colcnt,i,j,k;
	QString			sqlword;
	QStringList		content;
	
	rdbTableModes.clear();
#ifdef APP_VER_1_0
	sqlword=C2G("select name,describe,key1,key2,key3,key4,key5,key6 from rtdb_table");
#else ifdef APP_VER_2_0
#endif
	
	colcnt=dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(j=0;j<sizeof(szTableName)/sizeof(char*);j++)
	{
		for(i=0;i<content.size();i+=colcnt)
		{
			if(content[i].compare(szTableName[j],Qt::CaseInsensitive)==0)
			{
				rdbtablemode mode;
				mode.name=content[i];
				mode.describe=content[i+1];
				for(k=0;k<MAXKEY_NUM;k++)
				{
					mode.keyname[k]=content[i+2+k];
				}
				rdbTableModes.append(mode);
			}
		}
	}
}

void RdbParaDialog::initObjTypeTable()
{
	int				colcnt;
	QString			sqlword;
	QStringList		content;
	scdObjTypeDefs.clear();
#ifdef APP_VER_1_0
	sqlword=C2G("select 类型值,描述,表名 from scd对象类型表 order by 表名,类型值");
#else ifdef APP_VER_2_0
#endif
	colcnt=dbex.sqlReadRecorderDir(DB_NAME,sqlword,content);
	for(int i=0;i<content.size();i+=colcnt)
	{
		scada_objtype objtype;
		objtype.typeval=content[i].toUInt();
		objtype.describe=content[i+1];
		objtype.tablename=content[i+2];
		scdObjTypeDefs.append(objtype);
	}
}

void RdbParaDialog::addListGroup()
{
	int* host_group_no=netstat.GetHostGrpEnm();
	int  grpnum=netstat.GetHostGrpNum();
	for(int i=0;i<grpnum;i++)
	{
		boxGroup->addItem(QString::number(host_group_no[i]),QVariant(host_group_no[i]));
	}
}


void RdbParaDialog::addListTable()
{
	for(int i=0;i<rdbTableModes.size();i++)
	{
		boxTable->addItem(rdbTableModes[i].describe,QVariant(rdbTableModes[i].name));
	}
	boxTable->setCurrentIndex(0);
}


void RdbParaDialog::addListStation()
{
	int			i,nRet,nRcdCount;
	QString		szTemp;

	boxStation->clear();
	
#ifdef USERDBCLI
	Rdb_SqlCli  rdbSQL;
#else
	Rdb_SQL		rdbSQL;
#endif
	//厂站
	rdbSQL.RdbSelectFrom("station");
	rdbSQL.RdbSelectField("name");
	rdbSQL.RdbSelectField("describe");
	rdbSQL.RdbSelectField("status");
	
	//only get this group station 
	int groupno=boxGroup->itemData(boxGroup->currentIndex()).toInt();
	//nRet=rdbSQL.RdbWhereCond("groupno",RDB_DATATYPE_SHORT,&groupno);
#ifdef USERDBCLI
	nRet=rdbSQL.RdbSelCmd(groupno,MyUserName,MyPassWord);
#else
	nRet=rdbSQL.RdbSelCmd(MyUserName,MyPassWord);
#endif
	//boxStation->addItem(tr("所有厂站列表"),QVariant(-1));
	if (nRet==RDB_OK)
	{
		nRcdCount=rdbSQL.RdbGetRcdCnt();
		qDebug()<<"addListStation ok rdbcnt="<<nRcdCount;
		for (i=0;i<nRcdCount;i++) {
			int status=rdbSQL.RdbGetVal_int(i,"status");
			boxStation->addItem(C2G(rdbSQL.RdbGetString(i,"describe")),QVariant(C2G(rdbSQL.RdbGetString(i,"name"))));
		}
	}
	else
	{
		char chsTmp[512];
		RdbPrintErr(nRet,chsTmp);
		RdbPrintErr(const_cast<char*>(G2C(tr("从实时库中获取厂站列表失败"))),nRet);
		qDebug()<<"addListStation error:"<<C2G(chsTmp);
	}
	boxStation->setCurrentIndex(0);
}

void RdbParaDialog::updListObjType()
{
	boxType->clear();
	boxType->addItem(tr("所有类型列表"),QVariant(0));
	QString selTab;

	if(boxTable->currentIndex()<0) return;
	selTab=boxTable->currentText();
	foreach(scada_objtype objtype,scdObjTypeDefs)
	{
		if(objtype.tablename.compare(selTab,Qt::CaseInsensitive)==0)
		{
			boxType->addItem(objtype.describe,QVariant(objtype.typeval));
		}
	}
	boxType->setCurrentIndex(0);
}

void RdbParaDialog::updListDomain()
{
	boxDomain->clear();
	int i,nRet;
	QString strTmp;
#ifdef USERDBCLI
	Rdb_SqlCli  rdbSQL;
#else
	Rdb_SQL		rdbSQL;
#endif
	//获取选定表的所有域的描述
	if(boxTable->currentIndex()<0) 
		return;
	strTmp=boxTable->itemData(boxTable->currentIndex()).toString();
	rdbSQL.RdbSelectFrom(const_cast<char*>(G2C(strTmp)));
	//rdbSQL.RdbWhereCond("",RDB_DATATYPE_STRING,"");
	int groupno=boxGroup->itemData(boxGroup->currentIndex()).toInt();
	//nRet=rdbSQL.RdbSelCmd(groupno,MyUserName,MyPassWord);
#ifdef USERDBCLI
	nRet=rdbSQL.RdbSelCmd(groupno,MyUserName,MyPassWord);
#else
	nRet=rdbSQL.RdbSelCmd(MyUserName,MyPassWord);
#endif
	if(nRet==RDB_OK) {
		void *pTableAddr=NULL;
		RDB_BTBL_TAB_STRU TableInfo;
		RDB_BTBL_COL_STRU	ColumnInfo;
		int nSetFlag=0;
		rdbSQL.RdbGetTableInfo(&pTableAddr,&TableInfo);
		for(i=0;i<TableInfo.columnnums;i++) {
			rdbSQL.RdbGetColumnInfo(i,&ColumnInfo);
			strTmp=C2G(ColumnInfo.full_name);
			boxDomain->addItem(strTmp,QVariant(C2G(ColumnInfo.name)));
			if((strTmp.compare(tr("值"),Qt::CaseInsensitive)==0) && nSetFlag==0)
			{
				nSetFlag=1;
				nRet=i;
			}
		}
		if(nSetFlag){
			boxDomain->setCurrentIndex(nRet);
		}
		else{
			boxDomain->setCurrentIndex(0);
		}
	}
	else{
		//RdbPrintErr("\n",nRet);
		return;
	}
	return;
}

void RdbParaDialog::updListObject()
{
	boxObject->clear();
	QString     selstationname,strtmp;
	uint		 groupno,seltypeval;
	int i,nRet,rcdcount;
#ifdef USERDBCLI
	Rdb_SqlCli  rdbSQL;
#else
	Rdb_SQL		rdbSQL;
#endif

	foreach(QAbstractButton* button,buttonBox->buttons())
	{
		QDialogButtonBox::ButtonRole role=buttonBox->buttonRole(button);
		if(role==QDialogButtonBox::ApplyRole)
			button->setEnabled(false);
	}

	if(boxGroup->currentIndex()<0) return;
	groupno=boxGroup->itemData(boxGroup->currentIndex()).toUInt();
	
	if(boxType->currentIndex()<0) return;
	
	if(boxTable->currentIndex()<0) return;
	
	if(boxTable->currentIndex()<=6){			//"模拟量表","电压表","开关量表","刀闸量表","状态量表","保护量表","电度量表"
		if(boxType->currentIndex()){  //select one objtype
			seltypeval=boxType->itemData(boxType->currentIndex()).toUInt();
		}
		else			//select all objtype currently unused; 
		{
			seltypeval=boxType->itemData(boxType->currentIndex()).toUInt();  //all type = 0;
		}
		rdbSQL.RdbSelectFrom(const_cast<char*>(G2C(boxTable->itemData(boxTable->currentIndex()).toString())));
		rdbSQL.RdbSelectField("name");
		rdbSQL.RdbSelectField("describe");
		rdbSQL.RdbSelectField("baseobjtype");
		rdbSQL.RdbSelectField("substation");
		//只取本组内的数据            
//		rdbSQL.RdbWhereCond("groupno",RDB_DATATYPE_SHORT,&groupno);
#ifdef USERDBCLI
		nRet=rdbSQL.RdbSelCmd(groupno,MyUserName,MyPassWord);
#else
		nRet=rdbSQL.RdbSelCmd(MyUserName,MyPassWord);
#endif
		if (nRet==RDB_OK) {
			rcdcount=rdbSQL.RdbGetRcdCnt();
			selstationname=boxStation->itemData(boxStation->currentIndex()).toString();  //get select station name field value;
			for (i=0;i<rcdcount;i++)
			{
				uint baseobjtype=rdbSQL.RdbGetVal_uint(i,"baseobjtype");
				QString substation=C2G(rdbSQL.RdbGetString(i,"substation"));
				if(seltypeval==0 && selstationname.compare(substation,Qt::CaseInsensitive)==0)  //all obj type
				{
					boxObject->addItem(C2G(rdbSQL.RdbGetString(i,"describe")),QVariant(C2G(rdbSQL.RdbGetString(i,"name"))));
				}
				else if(baseobjtype==seltypeval && selstationname.compare(substation,Qt::CaseInsensitive)==0)
				{
					boxObject->addItem(C2G(rdbSQL.RdbGetString(i,"describe")),QVariant(C2G(rdbSQL.RdbGetString(i,"name"))));
				}
			}
		}
	}
	else//其他实时库表
	{	
		QString rdbname(boxTable->itemData(boxTable->currentIndex()).toString());
		rdbSQL.RdbSelectFrom(const_cast<char*>(G2C(rdbname)));
		foreach(rdbtablemode rdbtabmode,rdbTableModes)
		{
			if(rdbname.compare(rdbtabmode.name,Qt::CaseInsensitive)==0)
			{
				qDebug()<<rdbtabmode.keyname[0];
				char  szKey[CODE_LEN];
				char  szTabname[CODE_LEN];
				strcpy(szKey,G2C(rdbtabmode.keyname[0]));
				strcpy(szTabname,G2C(rdbtabmode.name));
				rdbSQL.RdbSelectField(szKey);
				rdbSQL.RdbSelectField("describe");
#ifdef USERDBCLI
				nRet=rdbSQL.RdbSelCmd(groupno,MyUserName,MyPassWord);
#else 
				nRet=rdbSQL.RdbSelCmd(MyUserName,MyPassWord);
#endif
				if (nRet==RDB_OK) {
					rcdcount=rdbSQL.RdbGetRcdCnt();
					for (i=0;i<rcdcount;i++) 
					{
						boxObject->addItem(C2G(rdbSQL.RdbGetString(i,"describe")),QVariant(C2G(rdbSQL.RdbGetString(i,szKey))));
					}
				}
				else// open not describe field
				{
					rdbSQL.RdbSelectField(szKey);
#ifdef USERDBCLI
					nRet=rdbSQL.RdbSelCmd(groupno,MyUserName,MyPassWord);
#else 
					nRet=rdbSQL.RdbSelCmd(MyUserName,MyPassWord);
#endif
					if(nRet==RDB_OK)
					{
						rcdcount=rdbSQL.RdbGetRcdCnt();
						RDB_BTBL_COL_STRU cinfo;
						for (i=0;i<rcdcount;i++) 
						{
#ifdef USERDBCLI
							nRet=rdbSQL.RdbGetColInfo(groupno,szTabname,szKey,&cinfo);
#else
							//nRet=rdbSQL.RdbGetColInfo(sztabname,szKey,&cinfo);
#endif
							if(nRet!=RDB_OK)	
								return;
							if(cinfo.datatype>RDB_DATATYPE_LASTDEF)	
								return ;
							if(cinfo.datatype==RDB_DATATYPE_STRING)
								strtmp=QString(C2G(rdbSQL.RdbGetString(i,szKey)));
							else{
								int nTemp=rdbSQL.RdbGetVal_uint(i,szKey);
								strtmp=QString("%1").arg(nTemp);
							}
							boxObject->addItem(strtmp,QVariant(strtmp));
						}// end for each rcd record
					}
				}//end if rdb success
			}
		}
	}
	if(boxObject->currentIndex()>=0)
	foreach(QAbstractButton* button,buttonBox->buttons())
	{
		QDialogButtonBox::ButtonRole role=buttonBox->buttonRole(button);
		if(role==QDialogButtonBox::ApplyRole)
			button->setEnabled(true);
	}
}







