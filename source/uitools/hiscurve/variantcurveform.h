#ifndef VARIANTCURVEFORM_H
#define VARIANTCURVEFORM_H
#include <QWidget>



#include "variantdatacurveplot.h"
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
class VariantDataCurvePlot;

struct VariantHisDataValue{
	QString		name;
	uint		datetime;
	uint		ms;
	float       ycdata;
};
typedef QVector<VariantHisDataValue> DbVariantOneCurveData;




class VariantCurveForm : public QWidget
{
	Q_OBJECT
public:
	VariantCurveForm(QWidget* parent = NULL);
	~VariantCurveForm();
	void print(QPrinter *printer);
public slots:
	void updateCurveData(selectorpara& selPara);			 //main slot,selector changed emit
	void selectorChanged(selectorcurvepara& selpara);		 //ui curve selector changed slot
private slots:
// 	void moved(const QPoint &);
	void mouseMoved(const QwtDoublePoint&);
	void selected(const QwtPolygon &);
protected:
	void resetPlot(const DbVariantOneCurveData& varOneCurveData);
	bool readVariantDayHisData(QString paraName,QDate curveDate,DbVariantOneCurveData& varOneCurveData) const;		//read variant curve data from one yc table
	void enableZoomMode(bool on	);
	void showInfo(QString text = QString::null);
	QString getCurveTitle(selectorpara& selpara) const;
	bool findWaveCurveVal( double x,double &yVal);
protected:
	VariantDataCurvePlot    *plot;
	
	varTimeScaleDraw        *varScaleDraw;
	//zoom and move 
	QwtPlotZoomer			*zoomer;
	QwtPlotPicker			*picker;
	QwtPlotPanner			*panner;
	//
	selectorpara			lastSelector;					    //独立保留了最后的选择器

	DbVariantOneCurveData varCurveData;

	QLineEdit                *statText;
};

#endif
