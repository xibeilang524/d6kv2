#ifndef REPORT_CHOOSE_H_
#define REPORT_CHOOSE_H_

#include "ui_qreportchoosebase.h"
#include "db/dbapi.h"
#include "globaldef.h"

class Report_choose : public QDialog, public Ui::reportChooseDlg  
{
	Q_OBJECT
public:
	Report_choose( QWidget* parent = 0 );
	virtual ~Report_choose();

public:
// 	bool readReportName() ;
	int m_report_count ;
// 	PREPORTNAMEPARA m_reportPara ;
// 	QMap<QString , uint > m_reportName2webId ;
};

#endif
