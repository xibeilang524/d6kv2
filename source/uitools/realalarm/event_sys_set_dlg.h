// event_sys_set_dlg.h: interface for the QEventSysSetDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QEVENTSYSSETDLG_H__B77E779C_49D1_41C1_A3B6_563FB57CB9A7__INCLUDED_)
#define AFX_QEVENTSYSSETDLG_H__B77E779C_49D1_41C1_A3B6_563FB57CB9A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QColor>

#include "ui_qeventsyssetbase.h"
#include "report_choose.h"

class QEventSysSetDlg : public QDialog, public Ui::QEventSysSetBase  
{
	Q_OBJECT
public:
	QEventSysSetDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
	virtual ~QEventSysSetDlg();
	void createReportTree() ;
// 	Report_choose* m_reportChooseDlg ;

public slots:
	void onDoubleClickedColorList( QListWidgetItem * item );
	void onEventColor( int item );
	void onOk();
	void onCancel();
	void onColUp();
	void onColDown();
	void onReportTreeClicked( QTreeWidgetItem * , int ) ;
	void onReportTreeListViewClicked( QTreeWidgetItem * , int ) ;
	void onReportFileChoose() ;
	void onPhoneNoEdit(QTableWidgetItem *item);

private:
	void set_wnd_stylesheet();

	QMap<uint,uint>								m_item2EventTypeMap;
	QMap<uint,QString>						m_item2RtuCodeMap;
	QMap<QString,QString>					m_rtuDesc2RtuNameMap;

	//m_colorList中对应项设置颜色列表
	QList<QColor>	m_itemColorList;
};

#endif // !defined(AFX_QEVENTSYSSETDLG_H__B77E779C_49D1_41C1_A3B6_563FB57CB9A7__INCLUDED_)
