#ifndef DATACURVEPLOT_H
#define DATACURVEPLOT_H

#include <QDateTime>
#include <QDebug>

#include "datacurveapp.h"

#include "qwt_plot.h"
#include "qwt_data.h"
#include "qwt_scale_draw.h"
#include "qwt_plot_curve.h"
struct curveItemDef;



class DataCurve  : public QwtPlotCurve
{
protected:
	virtual void draw(QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, int from, int to) const;	
};

class DateTimeScaleDraw : public QwtScaleDraw
{
public:
	DateTimeScaleDraw(QString xAsisFormat);
//	DateTimeScaleDraw(QString xAsisFormat);
	~DateTimeScaleDraw(){;}
	void    setXAsisFormat(QString xAsisFormat,QVector<uint> timeSeq = QVector<uint>(0));
	virtual QwtText label(double v) const;
	QString xLable(double v) const;
private:
	QString			_xAsisFormat;
	QVector<uint>   _timeSeq;
//	QVector<int>	_timeSeq;
};

class CurveItemData : public QwtData
{
public:
	CurveItemData(QVector<uint> timeSeq,QVector<double> curveData,QVector<DataState> curveDataState)
	{
		_dataTimeSeq=timeSeq;
		_data=curveData;
		_curveDataState=curveDataState;
		_size=_timeSeq.size();
	}
	virtual QwtData *copy() const {
		return new CurveItemData(_dataTimeSeq,_data,_curveDataState);
	}
	virtual size_t size() const {
		return _size;
	}
	virtual double x(size_t i) const 
	{
		DataCurveApp *thisApp = static_cast<DataCurveApp*>(qApp);
		for(int j=0;j<_dataTimeSeq.size();j++)
		{
			if(_timeSeq[i]==_dataTimeSeq[j])
			{
				if( thisApp->selpara.cycle == YearCycle )
				{
					return i;
				}
				else 
				{
					return _dataTimeSeq[j];
				}
			}
		}
		//return _timeSeq[i];
		return 0;
	}
	virtual double y(size_t i) const
	{
		//DataCurveApp *appSettings=static_cast<DataCurveApp*>qApp;
		for(int j=0;j<_dataTimeSeq.size();j++)
		{
			if(_timeSeq[i]==_dataTimeSeq[j])
			{
				return _data[j];
			}
		}
		return 0;
	}

	DataState state(size_t i) const
	{
		for(int j=0;j<_dataTimeSeq.size();j++)
		{
			if(_timeSeq[i]==_dataTimeSeq[j])
			{
				return _curveDataState[j];
			}
		}
		return empty_data;
	}
	static void setTimeSequence(QVector<uint> timeSeq){_timeSeq=timeSeq;}
private:
	size_t _size;
	QVector<double>		_data;
	QVector<uint>        _dataTimeSeq;
	QVector<DataState>  _curveDataState;
	static QVector<uint> _timeSeq;
};


class DataCurvePlot : public QwtPlot  
{
	Q_OBJECT
public:
	DataCurvePlot(QWidget * parent = NULL);
	~DataCurvePlot();
private slots:
	void showCurve(QwtPlotItem* item, bool on);
public:
	virtual void clear();
	void appendCurveItemDef(const curveItemDef* curveItem);
	void alignScales();
protected:
	QVector<curveItemDef*> plotCurves;
};

#endif
