#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>

#include <QString>
#include <QStringList>

#include <QVariant>
#include <QIcon>
#include <QtAlgorithms>
#include <QVBoxLayout>
#include <QDebug>


#include <QMenu>
#include <QAbstractButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialogButtonBox>

#include <QMessageBox>
#include <QFile>
#include <QDomDocument>


#include "mainwindow.h"
#include "functionwidget.h"
#include "appdef.h"
#include "customFunctionDialog.h"
#include "uitools/dmessagebox.h"
//////////////////////////////////////////////////////////////////////////
#include "upctrl.h"
//extern FUNCTIONTAB  interfunctab[63-5];		//5-->遥控函数个数
extern FUNCTIONTAB  interfunctab[79];

//////////////////////////////////////////////////////////////////////////

FunctionWidget::FunctionWidget( QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/ )
:QWidget(parent,f),m_domDoc("scriptedit")
{
	funcTree=new QTreeWidget(this);
	funcTree->setObjectName(QString::fromUtf8("funcTree"));
	//devTree->setFrameStyle(QFrame::NoFrame);
	funcTree->setIndentation(funcTree->indentation() * 7/10);
    funcTree->setUniformRowHeights(true);
    funcTree->setTextElideMode(Qt::ElideNone);
    funcTree->setAttribute(Qt::WA_MacShowFocusRect, false);
	funcTree->setHeaderHidden(true);
	// show horizontal scrollbar
	funcTree->header()->setResizeMode(QHeaderView::ResizeToContents);
	connect(funcTree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(funcTreeItemDClicked(QTreeWidgetItem*,int)));


	funcTree->setContextMenuPolicy(Qt::CustomContextMenu);
	popMenu=new QMenu(funcTree);
	
	

	actionInsertCustomFunc=new QAction(tr("插入自定义函数"),this);
	actionInsertCustomFunc->setObjectName(QString::fromUtf8("actionInsertCustomFunc"));
	connect(actionInsertCustomFunc, SIGNAL(activated()), this, SLOT(insertFunc()));
	
	actionEditCustomFunc=new QAction(tr("编辑自定义函数"),this);
	actionEditCustomFunc->setObjectName(QString::fromUtf8("actionEditCustomFunc"));
	connect(actionEditCustomFunc, SIGNAL(activated()), this, SLOT(editFunc()));

	actionDeleteCustomFunc=new QAction(tr("删除自定义函数"),this);
	actionDeleteCustomFunc->setObjectName(QString::fromUtf8("actionDeleteCustomFunc"));
	connect(actionDeleteCustomFunc, SIGNAL(activated()), this, SLOT(deleteFunc()));

	popMenu->addAction(actionInsertCustomFunc);
	popMenu->addAction(actionEditCustomFunc);
	popMenu->addAction(actionDeleteCustomFunc);
	connect(funcTree,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(customContextMenuRequested(const QPoint&)));


	QVBoxLayout *layout=new QVBoxLayout(this);
	layout->setObjectName(QString::fromUtf8("layout"));
	layout->setSpacing(3);
	layout->setMargin(0);
	setLayout(layout);
	layout->addWidget(funcTree);

	initFromXmlIni();
}

FunctionWidget::~FunctionWidget()
{

}

void FunctionWidget::customContextMenuRequested( const QPoint & pos )
{
	QTreeWidgetItem* curItem=funcTree->itemAt(pos);  //获取当前被点击的节点  
    if(curItem==NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
	if(curItem->data(0,Qt::UserRole).isNull()) return;

	
	if(curItem->data(0,Qt::UserRole).toInt()==FuncClassNode)
	{
		qDebug()<<curItem->text(0) << "==" << CUSTOM_CLASS_NAME;
		if(curItem->text(0)==CUSTOM_CLASS_NAME)
		{
			actionDeleteCustomFunc->setEnabled(false);
			actionEditCustomFunc->setEnabled(false);
			actionInsertCustomFunc->setEnabled(true);
			popMenu->exec(QCursor::pos());
		}
		else
			return;
	}
	else if(curItem->data(0,Qt::UserRole).toInt()==FuncItemNode)
	{
		QTreeWidgetItem *parentItem=curItem->parent();

		qDebug()<<parentItem->text(0);
		if(parentItem->text(0)!=CUSTOM_CLASS_NAME)
		{
			return;
		}
		if(curItem->data(0,Qt::UserRole+1).isNull()) return;
		actionDeleteCustomFunc->setEnabled(true);
		actionEditCustomFunc->setEnabled(true);
		actionInsertCustomFunc->setEnabled(true);
		popMenu->exec(QCursor::pos());
	}
    
}

//////////////////////////////////////////////////////////////////////////
////Implemention


void FunctionWidget::initFromXmlIni()
{
	QString xmlini;
	//获得环境变量
	QString nbenv=MainWindow::getEnvVar(); 
	xmlini = QString("%1/%2").arg(nbenv).arg(XML_INI_FILE_NAME);
	QFile file(xmlini);
	if(file.exists() && file.open(QIODevice::ReadOnly))
	{
		if(!m_domDoc.setContent(&file))
		{
			file.close();
			qDebug()<<"dom doc setContext error!";
		}
		//append root tree node 
		QTreeWidgetItem *funcNode,*classNode;
		funcTree->clear();
		funcNode=new QTreeWidgetItem(QStringList(tr("插入函数功能")));
		funcNode->setIcon(0, QIcon(":/res/funcnode.png"));
		funcNode->setData(0,Qt::UserRole,QVariant(FuncNode));
		funcTree->addTopLevelItem(funcNode);

	
		QDomElement docElem=m_domDoc.documentElement();
		QDomNode docNode=docElem.firstChild();
		while(!docNode.isNull())
		{
			QDomElement e=docNode.toElement();
			if(!e.isNull())
			{
				classNode=appendClassTreeNode(e);
				QString functionclass=e.attribute(DOM_CLASS_CLASS_ATTR);
				QDomNode nFuncNode=e.firstChild();
				while(!nFuncNode.isNull())
				{
					QDomElement eFunc=nFuncNode.toElement();

					appendFuncTreeNode(classNode,eFunc);
					nFuncNode=nFuncNode.nextSibling();
				}
			}
			docNode=docNode.nextSibling();
		}
		file.close();
	}
	else
	{
		DMessageBox::warning(this, tr("警告"),tr("配置文件[%1]不存在,程序将初始化默认列表").arg(xmlini));
	}
	return;
}

QTreeWidgetItem* FunctionWidget::appendClassTreeNode( const QDomElement& classElem )
{
	QTreeWidgetItem *classItem;
	QTreeWidgetItem *root=funcTree->topLevelItem(0);

	classItem=new QTreeWidgetItem(QStringList(classElem.attribute("functionclass")));
	classItem->setIcon(0, QIcon(":/res/functionclassnode.png"));
	classItem->setData(0,Qt::UserRole,QVariant(FuncClassNode));
	root->addChild(classItem);
	return classItem;
}

void FunctionWidget::appendFuncTreeNode( QTreeWidgetItem* classNode,const QDomElement& funcElem )
{
	QTreeWidgetItem *functionItem;
	QVariant var;
	qVariantSetValue<QDomElement>(var,funcElem);
	functionItem=new QTreeWidgetItem(QStringList(funcElem.attribute(DOM_FUNCTION_NODE_DESC_ATTR)));
	functionItem->setIcon(0, QIcon(":/res/functionnode.png"));
	functionItem->setData(0,Qt::UserRole,QVariant(FuncItemNode));
	functionItem->setData(0,Qt::UserRole+1,var);
	classNode->addChild(functionItem);
}


void FunctionWidget::initRdbNode( QTreeWidgetItem* parent )
{
	
}

void FunctionWidget::funcTreeItemDClicked( QTreeWidgetItem* item,int column )
{
	
	if(item->data(0,Qt::UserRole).isNull()) return;
	if(item->data(0,Qt::UserRole).toInt()==FuncItemNode)
	{
		if(item->data(0,Qt::UserRole+1).isNull()) return;
		QDomElement e=qVariantValue<QDomElement>(item->data(0,Qt::UserRole+1));
		QString insText=QString("%1")
			.arg(e.attribute(DOM_FUNCTION_NODE_DEF_ATTR));
		MainWindow::mainWindow()->insertText(insText);
	}
}

void FunctionWidget::insertFunc()
{
	CustomFunctionDialog funcDialog(this);
	if(funcDialog.exec()==QDialog::Accepted)
	{
		//find custom node;
		QDomElement docElem=m_domDoc.documentElement();
		QDomNode docNode=docElem.firstChild();
		QDomElement findEleme;
		bool bfind=false;
		while(!docNode.isNull())
		{
			findEleme=docNode.toElement();
			if(!findEleme.isNull())
			{
				QString functionclass=findEleme.attribute("functionclass");
				if(functionclass==CUSTOM_CLASS_NAME)
				{
					bfind=true;
					break;
				}
			}
			docNode=docNode.nextSibling();
		}
		if(bfind)
		{
			QDomElement funcNode = m_domDoc.createElement(DOM_FUNCTION_NODE_TAG);
			QDomAttr funcDescAttr=m_domDoc.createAttribute(DOM_FUNCTION_NODE_DESC_ATTR);
			QDomAttr funcdefAttr=m_domDoc.createAttribute(DOM_FUNCTION_NODE_DEF_ATTR);
			QDomAttr funchelpAttr=m_domDoc.createAttribute(DOM_FUNCTION_NODE_HELP_ATTR);
			
			funcDescAttr.setValue(funcDialog.funcNameEdit->text().trimmed());
			funcdefAttr.setValue(funcDialog.funcDefEdit->toPlainText().trimmed());
			funchelpAttr.setValue(funcDialog.funcHelpEdit->toPlainText().trimmed());
			
			funcNode.setAttributeNode(funcDescAttr);
			funcNode.setAttributeNode(funcdefAttr);
			funcNode.setAttributeNode(funchelpAttr);
			findEleme.appendChild(funcNode);
			//save to xml ini file
			QString xmlini;
			//获得环境变量
			QString nbenv=MainWindow::getEnvVar(); 
			xmlini = QString("%1/%2").arg(nbenv).arg(XML_INI_FILE_NAME);
			QFile file(xmlini);
			if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly))
			{
				qDebug()<<"error open xml ini file!";
				return;
			}
			QTextStream out(&file);
			m_domDoc.save(out,4);
			out.flush();
			file.close();
			
			QTreeWidgetItem *curItem=funcTree->currentItem();
			QTreeWidgetItem *classItem=curItem->parent();

			appendFuncTreeNode(classItem,funcNode);
		}
		else
		{
			qDebug()<<"error find customfunction node";
		}
	}
}

void FunctionWidget::deleteFunc()
{
	QTreeWidgetItem *curItem=funcTree->currentItem();
	if(curItem->data(0,Qt::UserRole).isNull()) return;
	if(curItem->data(0,Qt::UserRole).toInt()==FuncItemNode)
	{
		QTreeWidgetItem *parentItem=curItem->parent();
		if(parentItem->text(0)!=CUSTOM_CLASS_NAME)
		{
			return;
		}
		if(curItem->data(0,Qt::UserRole+1).isNull()) return;
		QDomElement e=qVariantValue<QDomElement>(curItem->data(0,Qt::UserRole+1));

		//find custom node;
		QDomNode parentNode=e.parentNode();
		if(!parentNode.isNull())
		{
			QDomNode oldNode=parentNode.removeChild(e);
		}

		//remove node from tree
		parentItem->removeChild(curItem);

		//save to xml ini file
		QString xmlini;
		//获得环境变量
		QString nbenv=MainWindow::getEnvVar(); 
		xmlini = QString("%1/%2").arg(nbenv).arg(XML_INI_FILE_NAME);
		QFile file(xmlini);
		if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly))
		{
			qDebug()<<"error open xml ini file!";
			return;
		}
		QTextStream out(&file);
		m_domDoc.save(out,4);
		out.flush();
		file.close();
	}
}

void FunctionWidget::editFunc()
{
	QTreeWidgetItem *curItem=funcTree->currentItem();
	if(curItem->data(0,Qt::UserRole).isNull()) return;
	if(curItem->data(0,Qt::UserRole).toInt()==FuncItemNode)
	{
		QTreeWidgetItem *parentItem=curItem->parent();
		if(parentItem->text(0)!=CUSTOM_CLASS_NAME)
		{
			return;
		}
		if(curItem->data(0,Qt::UserRole+1).isNull()) return;
		QDomElement e=qVariantValue<QDomElement>(curItem->data(0,Qt::UserRole+1));
		
		CustomFunctionDialog funcDialog(this);
		funcDialog.funcNameEdit->setText(e.attribute(DOM_FUNCTION_NODE_DESC_ATTR));
		funcDialog.funcDefEdit->setText(e.attribute(DOM_FUNCTION_NODE_DEF_ATTR));
		funcDialog.funcHelpEdit->setText(e.attribute(DOM_FUNCTION_NODE_HELP_ATTR));
		if(funcDialog.exec()==QDialog::Accepted)
		{
			QVariant var;
			e.setAttribute(DOM_FUNCTION_NODE_DESC_ATTR,funcDialog.funcNameEdit->text().trimmed());
			e.setAttribute(DOM_FUNCTION_NODE_DEF_ATTR,funcDialog.funcDefEdit->toPlainText().trimmed());
			e.setAttribute(DOM_FUNCTION_NODE_HELP_ATTR,funcDialog.funcHelpEdit->toPlainText().trimmed());
			qVariantSetValue<QDomElement>(var,e);
			curItem->setData(0,Qt::UserRole+1,var);
			curItem->setText(0,e.attribute(DOM_FUNCTION_NODE_DESC_ATTR));
			//save to xml ini file
			QString xmlini;
			//获得环境变量
			QString nbenv=MainWindow::getEnvVar(); 
			xmlini = QString("%1/%2").arg(nbenv).arg(XML_INI_FILE_NAME);
			QFile file(xmlini);
			if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly))
			{
				qDebug()<<"error open xml ini file!";
				return;
			}
			QTextStream out(&file);
			m_domDoc.save(out,4);
			out.flush();
			file.close();
		}
		
	}
	return;
}














