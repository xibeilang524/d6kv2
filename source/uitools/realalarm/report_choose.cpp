
#include "report_choose.h"
#include "event_real_wnd.h"
#include <QtGui/QListWidgetItem>

extern QWidget* getMainWnd();

Report_choose::Report_choose( QWidget* parent ) : QDialog( parent )
{
	setupUi( this ) ;
	QListWidgetItem *pItem = NULL ;
	m_report_count = 0;

	connect( m_cancelBtn , SIGNAL( clicked() ), this, SLOT( reject() ) ) ;
	connect( m_okBtn , SIGNAL( clicked() ) , this , SLOT( accept() ) ) ;

	Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
	Event_para *pPara = &( pMainWnd->m_db_para );

	QStringList reportNameList = pPara->get_report_name() ;
	for ( int i = 0 ; i < pPara->get_report_count() ; i++ )
	{
		pItem = new QListWidgetItem( reportNameList[ i ] , m_reportList ) ;
	}

	QString style_sheet = tr("QListWidget{font-family: Microsoft YaHei;font-size: 12px;color:#3E464E;background: #FAFAFA;border: none;}"								
								"QScrollBar{background:white;height:8px;}"
								"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:4px;}"
								"QScrollBar::handle:hover{background:gray;}"
								"QScrollBar::sub-line{background:transparent;}"
								"QScrollBar::add-line{background:transparent;}"
								"QScrollBar:vertical{width:8px;background:white;padding-top:9px;padding-bottom:9px;}"
								"QScrollBar::handle:vertical{background:lightgray; width:8px;min-height:20;border:2px solid transparent; border-radius:4px;}"
								"QScrollBar::handle:vertical:hover{background:gray;}"
								"QScrollBar::add-line:vertical{{background:transparent;}"
								"QScrollBar::sub-line:vertical{background:transparent;}");
	m_reportList->setStyleSheet(style_sheet);

	style_sheet = tr("QPushButton{background:white;border-radius: 2px;border: 1px solid #4A90E2;\
					 color: #4A90E2;font-family: Microsoft YaHei;font-size: 12px;\
					 min-width:75px;min-height:23px;}"
					 "QPushButton:hover{background:#0C2543;border: 0px;color:white;}");
	m_cancelBtn->setStyleSheet(style_sheet);
}

Report_choose::~Report_choose()
{

}

