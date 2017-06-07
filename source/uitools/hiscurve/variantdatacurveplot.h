#ifndef VARIANTDATACURVEPLOT_H
#define VARIANTDATACURVEPLOT_H

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

class varTimeScaleDraw : public QwtScaleDraw
{
public:
	varTimeScaleDraw(QString xAsisFormat) {
		setLabelAlignment (Qt::AlignVCenter);
		_xAsisFormat = xAsisFormat;
	}
	~varTimeScaleDraw(){;}
	virtual QwtText label(double v) const{
// 		DataCurveApp *thisApp=static_cast<DataCurveApp*>(qApp);
// 		QDateTime datetimeVal;
// 		datetimeVal.setTimeSpec(thisApp->timeSpecValue);
// 		datetimeVal.setTime_t(static_cast<uint>(v));
// 		datetimeVal = datetimeVal.addMSecs((v-static_cast<uint>(v))*1000);
// 		return datetimeVal.toString(_xAsisFormat);
		QTime zerotime(0,0,0);
		QTime time = zerotime.addMSecs(static_cast<uint>(v));
		return time.toString(_xAsisFormat);
	}
	QString xLable(double v) const{
// 		DataCurveApp *thisApp=static_cast<DataCurveApp*>(qApp);
// 		QDateTime datetimeVal;
// 		datetimeVal.setTimeSpec(thisApp->timeSpecValue);
// 		datetimeVal.setTime_t(static_cast<uint>(v));
// 		datetimeVal = datetimeVal.addMSecs((v-static_cast<uint>(v))*1000);
// 		return datetimeVal.toString(_xAsisFormat);
		QTime zerotime(0,0,0);
		QTime time = zerotime.addMSecs(static_cast<uint>(v));
		return time.toString(_xAsisFormat);
	}
private:
	QString _xAsisFormat;
};


class VariantDataCurvePlot : public QwtPlot  
{
	Q_OBJECT
public:
	VariantDataCurvePlot(QWidget * parent = NULL);
	~VariantDataCurvePlot();
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

