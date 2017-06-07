#include "QAddNode.h"
#include <QtGui>

#include "globaldef.h"

QAddNodeDlg::QAddNodeDlg( QWidget* parent)
	: QDialog( parent)
{
	setWindowTitle( _C2Q( "增加节点" ) ) ;
    setModal( TRUE );	
	m_Label = new QLabel(_C2Q("节点："),this);
    m_NodeName = new QComboBox(this);
	
    m_btnOK = new QPushButton(_C2Q("确定"),this);
    m_btnOK->setGeometry( QRect( 30, 100, 82, 25 ) );
	
    m_btnCancel = new QPushButton(_C2Q("取消"),this );
    m_btnCancel->setGeometry( QRect( 180, 100, 82, 25 ) );

	QGridLayout* pLayout = new QGridLayout(this);
	pLayout->addWidget(m_Label,0,0);
	pLayout->addWidget(m_NodeName,0,1);
	pLayout->addWidget(m_btnOK,1,0);
	pLayout->addWidget(m_btnCancel,1,1);
	
    // signals and slots connections
    connect( m_btnOK, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void QAddNodeDlg::OnSetNodename(QStringList& nodename)
{
	m_NodeName->addItems(nodename);
}

void QAddNodeDlg::OnGetNodename(QString &szname)
{
	szname = m_NodeName->currentText();
}