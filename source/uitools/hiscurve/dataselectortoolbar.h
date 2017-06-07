#ifndef DATASELECTORTOOLBAR_H
#define DATASELECTORTOOLBAR_H

#include <QToolBar>
#include "appdef.h"
#include "datacurveapp.h"

QT_BEGIN_NAMESPACE
class QAction;
class QDateEdit;
class QTimeEdit;
class QComboBox;
class QLabel;
class QWidgetAction;
class QSpinBox;
class QLineEdit;
class QToolButton;
QT_END_NAMESPACE


class DataSelectorToolBar : public QToolBar  
{
	Q_OBJECT
public:
	DataSelectorToolBar(QMainWindow *parent);
	~DataSelectorToolBar();
signals:
	void selectorChanged(selectorpara&);
public  slots:
	void dataSrcChanged();
private slots:
	void variantCycleDayTriggered(bool checked);
	void trendCycleDayTriggered(bool checked);
	void cycleDayTriggered(bool checked);
	void cycleMonthTriggered(bool checked);
	void cycleYearTriggered(bool checked);
	void generalDateTriggered(bool checked);
	void segmentDateTriggered(bool checked);

	
	void dateEdit_1Changed(const QDate & date);
	void dateEdit_2Changed(const QDate & date);
	void timeEdit_1Changed();
	void timeEdit_2Changed();
	void boxDataTypeIndexChanged(int index);
	void boxDayMinCycleIndexChanged(int index);
	void insComp();
	void delComp();
private:
	void init();
	void modifyCurveDataToolBars(CurveCycleType curveCycle,CurveSegmentType curveSeg);
	void updateSelPara(selectorpara& selpara,bool isset=false);
	void updCompPopmenu();
private:
	QAction *actionDayCycle,*actionMonCycle,*actionYearCycle,*actionSegmentDate,*actionGeneralDate,
		*actionSepComp,*actionInsComp,*actionDelComp,*actionVariantDayCycle,*activeTrendDayCycle;
	QToolButton *btnDelComp;
	QWidgetAction *actionDateEdit_1,*actionDateEdit_2,*actionTimeEdit_1,*actionTimeEdit_2,*actionBoxDataType;
	QWidgetAction *actionDateEdit_1Lbl,*actionDateEdit_2Lbl,*actionTimeEdit_1Lbl,*actionTimeEdit_2Lbl,*actionBoxDataTypeLbl,
		*actionsdateLel;
	QDateEdit *dateEdit_1,*dateEdit_2;
	QTimeEdit *timeEdit_1,*timeEdit_2;
	QLabel    *dateEdit_1Lbl,*dateEdit_2Lbl,*timeEdit_1Lbl,*timeEdit_2Lbl,*boxDataTypeLbl;
	QLineEdit *sdateEdit;
	QComboBox *boxDataType;

	QWidgetAction *actionDayMinCycleLbl,*actionBoxDayMinCycle;
	QLabel        *dayMinCycleLbl;
	QComboBox	  *boxDayMinCycle;

	QWidget *parent;

	selectorpara     &currentSelPara;              //ref app 
public:
	CurveCycleType   getCycle();
	CurveSegmentType getSegment();
	int              getDataType();
	QDateTime        getBeginDateTime();
	QDateTime        getEndDateTime();

	void             showSDateText(bool show);
	void	setDate(QString strDate);
	
};


class CurveSelectorToolBar : public QToolBar  
{
	Q_OBJECT
public:
	CurveSelectorToolBar(QMainWindow *parent);
	~CurveSelectorToolBar();
signals:
	void selectorChanged(selectorcurvepara&);
private slots:
	void attrAntialiased(bool checked);
	void attrFitted(bool checked);
	void showGrid(bool checked);
	void showSymbol(bool checked);

	void setLineStyle(bool checked);
	void setDotsStyle(bool checked);
	void setSetpStyle(bool checked);
	void SetSticksStyle(bool checked);

	void nullValZero(bool checked);
	void nullValAvg(bool checked);
	void nullValFix(bool checked);

	void yAxisAuto(bool checked);
	void yAxisRangeChanged(int val);
private:
	void init();
	QAction *actionShowGrid,*actionShowSymbol/*,*actionShowNullValue,*actionShowAlignScales*/;
	QAction *actionStyleLine,*actionStyleDots,*actionStyleSetp,*actionStyleSticks;
	QAction *actionAttrFitted,*actionAttrAntialiased;
	QAction *actionYAxisAutoScale;
	QAction *actionYAxisSparator;
	QWidgetAction *actionYMinSpinLbl,*actionYMaxSpinLbl,*actionYMinSpin,*actionYMaxSpin;
	QLabel  *yMinSpinLbl,*yMaxSpinLbl;
	QSpinBox *yMinSpin,*yMaxSpin;

	QAction  *actionNullValZero,*actionNullValAvg,*actionNullFix;


	selectorcurvepara     &currentSelPara;
	QWidget *parent;
public:
	void setYAsisBoxRange(double& min,double& max) const;
};
#endif 
