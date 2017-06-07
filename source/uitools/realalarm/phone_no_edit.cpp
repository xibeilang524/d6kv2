
#include "phone_no_edit.h"
#include "event_real_wnd.h"
#include "uitools/dmessagebox.h"
#include <QtGui/QListWidgetItem>
#include <QMessageBox>
#include <QTextCodec>

extern QWidget* getMainWnd();

//////////////////////////////////////////////////////////////////////////
Staion_tree_item::Staion_tree_item(const QStringList & strings, int station_no):QTreeWidgetItem(strings),m_station_no(station_no)
{

}

Staion_tree_item::~Staion_tree_item()
{

}

//////////////////////////////////////////////////////////////////////////
Phone_no_edit::Phone_no_edit(Event_cfg* pcfg, QString phoneNo_list, uint type ,QWidget* parent ) : QDialog( parent ),m_event_type(type),m_event_cfg(pcfg)
{
	setupUi( this ) ;
	setWindowTitle(tr("短信设置"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	m_bload = true;

	if(m_db.Open() != CS_SUCCEED)
	{
		printf("DB opend failed\n");
	}

	QStringList phonelist = phoneNo_list.split(",");
	m_phoneNo->addItems(phonelist);

	QStringList head_lable;
	head_lable << tr("描述") << tr("代码");
	m_selected_tree->setHeaderLabels(head_lable);

//  buttonBox->button( QDialogButtonBox::Ok )->setText( tr( "确定" ) );
// 	buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "取消" ) );

	show_all_staion();

	connect( m_desc_tree , SIGNAL( itemClicked( QTreeWidgetItem *, int) ) , this , SLOT( slot_refresh_desc_tree( QTreeWidgetItem *, int ) ) ) ;
	connect( m_addbutton , SIGNAL( clicked(  ) ) , this , SLOT( slot_add_desc( ) ) );
	connect( m_delete_button , SIGNAL( clicked(  ) ) , this , SLOT( slot_delete_desc( ) ) );
	connect( m_load_button , SIGNAL( clicked(  ) ) , this , SLOT( slot_load_desc( ) ) );
	connect( m_phoneNo , SIGNAL( currentIndexChanged ( const QString & )  ) , this , SLOT( slot_refresh_selected_desc_tree(const QString &  ) ) );
	connect( m_okBtn , SIGNAL( clicked(  ) ) , this , SLOT( accept( ) ) );
	connect( m_cancelBtn , SIGNAL( clicked(  ) ) , this , SLOT( reject( ) ) );

	m_phoneNo->setCurrentIndex(0);
	slot_refresh_selected_desc_tree(m_phoneNo->currentText());

	QString style_sheet = tr("QPushButton{background:white;border-radius: 2px;border: 1px solid #4A90E2;\
					 color: #4A90E2;font-family: Microsoft YaHei;font-size: 12px;\
					 min-width:75px;min-height:23px;}"
					 "QPushButton:hover{background:#0C2543;border: 0px;color:white;}");
	m_cancelBtn->setStyleSheet(style_sheet);
}

Phone_no_edit::~Phone_no_edit()
{
	m_db.Close();
}

void Phone_no_edit::show_all_staion()
{
	struct RTU_INFO_
	{
		short rtu_no;
		char desc[64];
	};
	char sql[4096];
	RTU_INFO_* result;
	sprintf(sql,"select 终端序号,描述 from 终端参数表 order by 终端序号");
	int rtu_num = select(sql,(void**)&result);
	QStringList strname;
	Staion_tree_item* pStItem;
	for(int i = 0; i< rtu_num;i++)
	{
		strname.clear();
		strname.append(_C2Q(result->desc));
		pStItem = new Staion_tree_item(strname,result->rtu_no);
		m_desc_tree->addTopLevelItem(pStItem);
		result++;
	}

}

void Phone_no_edit::slot_refresh_desc_tree( QTreeWidgetItem * item, int column )		//根据类型刷新所选设备列表
{

	struct DESC_INFO_
	{
		char desc_name[24];
		char desc[40];
	};


	if (item->parent() || item->childCount())
	{
		return;
	}

	Staion_tree_item* pitem = (Staion_tree_item*)item;
	int rtuno = pitem->get_station_no();

	char sql[4096];
	DESC_INFO_* result;
	int yx_type = 0;
	char table_name[256];
	if (m_event_type == 2001)//遥测越限
	{
		sprintf(sql,"select 代码,描述 from 遥测参数表 where 终端序号= %d order by 遥测序号 asc",rtuno);
	}
	else
	{
		switch(m_event_type)
		{
		case 2002:		//状态量类型
		case 2003:
		case 2008:
			yx_type = 400;
			break;
		case 2004:		//开关量类型
		case 2005:		
		case 2009:		
			yx_type = 300;
			break;
		case 2006:		//刀闸量类型
		case 2010:		
			yx_type = 500;
			break;
		case 2007:		//保护量类型
		case 2011:		
			yx_type = 600;
			break;
		default:
			yx_type = 0;
		}
		sprintf(sql,"select 代码,描述 from 遥信参数表 where 终端序号= %d and 类型 = %d order by 遥信序号 asc",rtuno, yx_type);
	}

	int desc_num = select(sql,(void**)&result);
	QStringList strname;
	QTreeWidgetItem* pSubItem;
	for(int i = 0; i< desc_num;i++)
	{
		strname.clear();
		strname.append(_C2Q(result->desc));
		strname.append(_C2Q(result->desc_name));
		pSubItem = new QTreeWidgetItem( item, strname );
		result++;
	}

}

void Phone_no_edit::slot_refresh_selected_desc_tree(const QString &phoneNo)
{
	if (!m_bload)
	{
		DMessageBox::warning(this, _C2Q("警告"), _C2Q( "当前手机号提醒设置已做修改，未保存，已进行自动保存!" ) );
		slot_load_desc();
	}
	m_selected_tree->clear();
	if (m_event_cfg)
	{
		QMap<int,Event_cfg::PhoneDescMap> typeNomap = m_event_cfg->get_type_to_no_map();
		Event_cfg::PhoneDescMap tempmap = typeNomap[m_event_type];
		QString name_str = tempmap[phoneNo];
		if (name_str.isEmpty())
		{
			return;
		}
		QStringList name_list = name_str.split(",");
		for(int i = 0; i<name_list.size();i++)
		{
			QStringList strlist;
			strlist = name_list.at(i).split("_");
			m_selected_tree->addTopLevelItem(new QTreeWidgetItem(strlist));
		}
	}
}

QString Phone_no_edit::get_descname()
{
	QString descname;
	QTreeWidgetItem * pitem;
	for (int i = 0;i < m_selected_tree->topLevelItemCount(); i++)
	{
		pitem = m_selected_tree->topLevelItem(i);
		if(!descname.isEmpty())
			descname.append(",");
		descname.append(pitem->text(0));
		descname.append("_");		
		descname.append(pitem->text(1));
	}

	return descname;
}

void Phone_no_edit::slot_add_desc()
{
	QTreeWidgetItem* pselectedItem = m_desc_tree->currentItem();


	if (!pselectedItem || !pselectedItem->parent())
	{
		printf("no item selected\n");
	}
	else
	{
		QString strdesc = get_descname();

		if (strdesc.contains(pselectedItem->text(0)) || strdesc.contains(pselectedItem->text(1)) )
		{
			return;
		}
		QStringList descstr;
		descstr << pselectedItem->text(0) << pselectedItem->text(1);

 		m_selected_tree->addTopLevelItem(new QTreeWidgetItem(descstr));
	}

	m_bload = false;
}

void Phone_no_edit::slot_delete_desc()
{
	QTreeWidgetItem* pitem = m_selected_tree->currentItem();

	int row_no = m_selected_tree->indexOfTopLevelItem(pitem);
	if (row_no != -1)
	{
		m_selected_tree->takeTopLevelItem(row_no);
	}

	m_bload = false;

}

void Phone_no_edit::slot_load_desc()
{
	m_bload = true;

	if (!m_event_cfg)
	{
		printf("Config Pointer Error!\n");
	}
	else
	{
		QString phoneno = m_phoneNo->currentText();
		m_event_cfg->set_phone_no_desc(m_event_type,phoneno,get_descname());

		QMap<int,Event_cfg::PhoneDescMap> typeNomap = m_event_cfg->get_type_to_no_map();
		Event_cfg::PhoneDescMap phonedescmap = typeNomap[m_event_type];

		QString str = phonedescmap[phoneno];

	}
}

int Phone_no_edit::select(const char* sql, void**result_pt)
{
	int ret = -1;

	//只有数据库正常时才真正从数据库select
	if (sql != NULL)
	{
		CS_DATAFMT *datafmt = NULL;
		FUNC_STR func;
		func.func = SEL_ISQL_RESULTS;
		strcpy(func.dbname, DB_NAME);
		strcpy(func.isql, sql);

		if (CS_SUCCEED == m_db.SelectIsqlResults(&func, &datafmt, result_pt))
		{
			ret = func.ret_roxnum;
		}

		if (datafmt)
		{
			free(datafmt);
			datafmt = NULL;
		}
	}//if (!s_db_stat && sql != NULL)

	return ret;
}