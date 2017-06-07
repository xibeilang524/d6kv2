#ifndef TRENDCURVEFORM_H
#define TRENDCURVEFORM_H
#include <QWidget>



#include "trenddatacurveplot.h"
#include "datacurveview.h"			//reference some declaration 
#include "datacurveapp.h"

class QLineEdit;
class DataCurvePlot;
class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class selectorpara;
class selectorcurvepara;
class DateTimeScaleDraw;
class DataCurve;
class TrendDataCurvePlot;

struct TrendHisDataValue{
	QString		name;
	uint		datetime;
	uint		ms;
	float       ycdata;
};
typedef QVector<TrendHisDataValue> DbTrendOneCurveData;




class TrendCurveForm : public QWidget
{
	Q_OBJECT
public:
	TrendCurveForm(QWidget* parent = NULL);
	~TrendCurveForm();
	void print(QPrinter *printer);
public slots:
	void updateCurveData(selectorpara& selPara);			 //main slot,selector changed emit
	void selectorChanged(selectorcurvepara& selpara);		 //ui curve selector changed slot
private slots:
	void mouseSelected(const QwtDoublePoint&);
	void moved(const QPoint &);
	void selected(const QwtPolygon &);
protected:
	void resetPlot(const DbTrendOneCurveData& varOneCurveData);
	bool readTrendDayHisData(QString paraName,QDate curveDate,DbTrendOneCurveData& varOneCurveData) const;		//read variant curve data from one yc table
	void enableZoomMode(bool on	);
	void showInfo(QString text = QString::null);
	QString getCurveTitle(selectorpara& selpara) const;
	bool findWaveCurveVal(const QwtPlotCurve* pCurveData, double x,double &yVal);

protected:
	TrendDataCurvePlot    *plot;
	
	trendTimeScaleDraw        *varScaleDraw;
	//zoom and move 
	QwtPlotZoomer			*zoomer;
	QwtPlotPicker			*picker;
	QwtPlotPanner			*panner;
	//
	selectorpara			lastSelector;					    //独立保留了最后的选择器

	QLineEdit                *statText;
};

#endif
