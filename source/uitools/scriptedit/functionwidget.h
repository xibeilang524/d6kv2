#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QVector>
#include <QAction>
#include <QTreeWidget>
#include <QDomDocument>

QT_BEGIN_NAMESPACE
class QToolBar;
class QTreeWidget;
class QTreeWidgetItem;
class QDomDocument;
class QDomElement;
QT_END_NAMESPACE




// typedef struct _func_instruction
// {
// 	_func_instruction(){}
// 	_func_instruction(QString _desc,QString _funcdef,QString _funchelp="")
// 	{desc=_desc;funcdef=_funcdef;funchelp=_funchelp;}
// 
// 	QString desc;
// 	QString funcdef;
// 	QString funchelp;
// }FUNC_INSTRUCTION;
// typedef QVector<FUNC_INSTRUCTION> FUNC_INSTRUCTIONES;
// 
// typedef struct _function_class
// {
// 	_function_class(){}
// 	_function_class(QString _functionclass,FUNC_INSTRUCTIONES& _func_instructiones)
// 	{functionclass=_functionclass,func_instructiones=_func_instructiones;}
// 
// 	QString				functionclass;
// 	FUNC_INSTRUCTIONES	func_instructiones;
// }FUNCTION_CLASS;
// typedef QVector<FUNCTION_CLASS>					FUNCTION_CLASSES;
// 
// Q_DECLARE_METATYPE(FUNC_INSTRUCTION);


class FunctionWidget : public QWidget  
{
	Q_OBJECT
public:
	explicit FunctionWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~FunctionWidget();
signals:
	void insertFunctionTip(QString functionTip);
	void insertRdbWord(QString RdbWord);
private slots:
	void funcTreeItemDClicked(QTreeWidgetItem* item,int column);
	void customContextMenuRequested ( const QPoint & pos );

 	void insertFunc();
 	void deleteFunc();
 	void editFunc();
	
private:
	enum TreeNodeType{FuncNode=0,FuncClassNode,FuncItemNode,FuncRdbRtuNode,YcNode,YxNode};
//	void initFuncClasses();
	void initFromXmlIni();
	
	//void initTreeFuncNode();
	void initRdbNode(QTreeWidgetItem* parent);
	
    //tree node operator
	QTreeWidgetItem* appendClassTreeNode(const QDomElement& classElem);
	void appendFuncTreeNode(QTreeWidgetItem* classNode,const QDomElement& funcElem);
private:
	QTreeWidget *funcTree;  
	QMenu   *popMenu;
	QAction *actionInsertCustomFunc,*actionDeleteCustomFunc,*actionEditCustomFunc;
	//FUNCTION_CLASSES funcClasses;
	QDomDocument     m_domDoc;
public:
	QTreeWidget* getFuncTree() const{return funcTree;}
};
Q_DECLARE_METATYPE(QDomElement);
#endif
