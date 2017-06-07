#include "masinfo_nodeinfo.h"


QNodeInfo::QNodeInfo(QWidget *parent /* = NULL */)
:QDialog(parent)
{
	setWindowTitle(_C2Q("全网节点信息"));
	setWindowIcon(QIcon(":/res/computer-4.png"));
	m_pNodeTable = new QTableWidget(this);
	showNode();
}

void QNodeInfo::showNode(void)
{

}