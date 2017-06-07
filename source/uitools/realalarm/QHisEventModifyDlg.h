// QHisEventModifyDlg.h: interface for the QHisEventModifyDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QHISEVENTMODIFYDLG_H__C2FCF051_937A_47A4_99D9_EE5095C794A6__INCLUDED_)
#define AFX_QHISEVENTMODIFYDLG_H__C2FCF051_937A_47A4_99D9_EE5095C794A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "qhiseventmodibase.h"

#include "sysstr.h"

#include "qvaluelist.h"

class QHisEventModifyDlg : public QHisEventModiBase  
{
public:
	QHisEventModifyDlg( HIS_EVENT *eve, QWidget* parent = 0, 
		const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	
	virtual ~QHisEventModifyDlg();

	void onEventSortChanged( int item );

	void onEventTypeChanged( int item );

	void onRtuChanged( int item );

	void onGrpChanged( int item );

	void onModifyEvent();

	void onDelEvent();

	void onAddEvent();
	
private:

	QValueList<uint>	m_grpNoList;
	QValueList<uint>	m_eventTypeList;
	QStringList			m_stationList;
	QStringList			m_objectList;
	
	HIS_EVENT			m_curHisEvent;
	HIS_EVENT			m_oldHisEvent;

private:

	void getEvent();

};

#endif // !defined(AFX_QHISEVENTMODIFYDLG_H__C2FCF051_937A_47A4_99D9_EE5095C794A6__INCLUDED_)
