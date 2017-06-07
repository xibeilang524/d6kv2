#ifndef TRENDDATACURVEPLOT_H
#define TRENDDATACURVEPLOT_H

#include <QDateTime>
#include <QDebug>

#include "datacurveplot.h"   //reference some declaration

#include <QDateTime>
#include <QDebug>

#include "datacurveapp.h"

#include "qwt_plot.h"
#include "qwt_data.h"
#include "qwt_scale_draw.h"
#include "qwt_plot_curve.h"

class trendTimeScaleDraw : public QwtScaleDraw
{
public:
	trendTimeScaleDraw(QString xAsisFormat) {
		setLabelAlignment (Qt::AlignVCenter);
		_xAsisFormat = xAsisFormat;
	}
	~trendTimeScaleDraw(){;}
	virtual QwtText label(double v) const{
		DataCurveApp *thisApp=static_cast<DataCurveApp*>(qApp);
		QTime datetimeVal(0,0,0);
		int m_msec = static_cast<int>(v);
		datetimeVal = datetimeVal.addMSecs(m_msec);
		QString sss = datetimeVal.toString(_xAsisFormat);
		return datetimeVal.toString(_xAsisFormat);
	}
	QString xLable(double v) const{
		DataCurveApp *thisApp=static_cast<DataCurveApp*>(qApp);
		QTime datetimeVal(0,0,0);
		int m_msec = static_cast<int>(v);
		datetimeVal = datetimeVal.addMSecs(m_msec);
		QString sss = datetimeVal.toString(_xAsisFormat);
		return datetimeVal.toString(_xAsisFormat);
}
private:
	QString _xAsisFormat;
};


class TrendDataCurvePlot : public QwtPlot  
{
	Q_OBJECT
public:
	TrendDataCurvePlot(QWidget * parent = NULL);
	~TrendDataCurvePlot();
private slots:
	void showCurve(QwtPlotItem* item, bool on);
public:
	virtual void clear();
	void alignScales();
	QwtPlotCurve* plotCurve() const {return varCurve;}
private:
	//curveItemDef			 *plotCurve; //only single curve
	
	QwtPlotCurve			 *varCurve;
};

#endif

