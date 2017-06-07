#include <QMouseEvent>
#include <QCursor>
#include <QMenu>
#include <QPalette>

#include "wavetab.h"
#include "globaldef.h"

/*  CWaveTabbar  */

QWaveTabbar::QWaveTabbar(QWidget * parent)
    : QTabBar(parent)
{
	m_current_index = 0;
	create_action(this);
}

void QWaveTabbar::mouseDoubleClickEvent(QMouseEvent* e)
{
	m_current_index = tabAt( e->pos());
	if (m_current_index == -1)
		return;
	emit signal_item_close(m_current_index);
}

void QWaveTabbar::contextMenuEvent(QContextMenuEvent* e)
{
	menu_tab->clear();
	m_current_index = tabAt(e->pos());
	if (m_current_index == -1)
		return;

	menu_tab->addAction(action_close);
	menu_tab->addAction(action_property);
	menu_tab->exec(QCursor::pos());
	e->accept();
}

void QWaveTabbar::create_action( QWidget* parent )
{
	action_close = new QAction(_C2Q("关闭(&C)"),parent);
	action_close->setObjectName(QString::fromUtf8("actionClose"));
	action_close->setShortcut(_C2Q("Ctrl+C"));
	connect(action_close,SIGNAL(activated()),parent,SLOT(close_tab()));

	action_property = new QAction(_C2Q("文件属性(&P)"),parent);
	action_property->setObjectName(QString::fromUtf8("actionProperty"));
	action_property->setShortcut(_C2Q("Ctrl+P"));
	connect(action_property,SIGNAL(activated()),parent,SLOT(tab_property()));

	menu_tab = new QMenu();
}

void QWaveTabbar::close_tab( void )
{
	emit signal_item_close(m_current_index);
}

void QWaveTabbar::tab_property( void )
{
	emit signal_item_property(m_current_index);
}

/*  CWaveTabWdiget  */

QWaveTabWidget::QWaveTabWidget(QWidget * parent)
	: QTabWidget(parent)
{
	m_pTabbar = new QWaveTabbar(this);
	setTabBar(m_pTabbar);
}

QWaveTabWidget::~QWaveTabWidget()
{
	delete m_pTabbar;
}


/* QWaveBar */

QWaveBar::QWaveBar(QStatusBar* bar)
{
	m_pBar = bar;
	for (int i = 0; i < 4; i++)
		m_pEdit[i] = CreateEdit();
	m_pEdit[0]->setMaximumWidth(200);
}
	
QWaveBar::~QWaveBar() 
{
	for (int i = 0; i < 4; i++)
		delete m_pEdit[i];
}

void QWaveBar::Message(int n, const QString& message)
{
	if (n < 0 && n > 3)
		return;

	m_pEdit[n]->setText(message);
}

QLineEdit* QWaveBar::CreateEdit(int len)
{
	QLineEdit* pEdit = new QLineEdit(NULL);
// 	pEdit->setFrameShape(QFrame::MenuBarPanel);
	pEdit->setEnabled(false);
	m_pBar->addWidget(pEdit, 0);
	QFont ft = pEdit->font();
	ft.setBold(true);
	pEdit->setFont(ft);
	QPalette palette;
	palette.setColor(pEdit->foregroundRole(), Qt::black);
	pEdit->setPalette(palette);
	return pEdit;
}

void QWaveBar::Clear()
{
	for (int i = 0; i < 4; i++)
		m_pEdit[i]->setText(QString::null);
}
