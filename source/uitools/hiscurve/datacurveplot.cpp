#include <QWidget>
#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_layout.h"
#include "qwt_legend.h"
#include "qwt_legend_item.h"
#include "qwt_scale_widget.h"



#include "datacurveplot.h"
#include "datacurveview.h"

QVector<uint> CurveItemData::_timeSeq;

DataCurvePlot::DataCurvePlot( QWidget* parent)
:QwtPlot(parent)
{	
	setAutoReplot(false);

	QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	grid->enableXMin(true);
	grid->enableYMin(true);

    grid->attach(this);


	QwtPlotCanvas* plCanvas = canvas();
	plCanvas->setFocusPolicy(Qt::StrongFocus);
	plCanvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
	plCanvas->setFocus();
	plCanvas->setFrameShadow(QwtPlot::Plain);
	plCanvas->setPaintAttribute(QwtPlotCanvas::PaintCached, true);
	plCanvas->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
	plCanvas->setFrameStyle(QFrame::NoFrame);

    QColor background = QColor(Qt::white);
    background.setAlpha(255);
	
	QPalette palette;
    palette.setColor(QPalette::Window, background);
    setPalette(palette);
	
	setCanvasBackground (background);
	setFocusPolicy(Qt::StrongFocus);
	setFocusProxy(plCanvas);
	setFrameShape(QFrame::NoFrame);
	setLineWidth(2);
	setMouseTracking(true);

	QwtLegend *legend = new QwtLegend(this);
    legend->setItemMode(QwtLegend::CheckableItem);
	legend->setDisplayPolicy(QwtLegend::FixedIdentifier,QwtLegendItem::ShowLine | QwtLegendItem::ShowText);
    insertLegend(legend, QwtPlot::BottomLegend);
	connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),SLOT(showCurve(QwtPlotItem *, bool)));

	setAutoReplot(true);
}


DataCurvePlot::~DataCurvePlot()
{

}


void DataCurvePlot::showCurve(QwtPlotItem* item, bool on )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	setAutoReplot(false);
	item->setVisible(on);
	foreach(curveItemDef* itemDef,plotCurves)
	{
		if(itemDef->curveItem == item)
		{
			if(on && appSettings->selcurvepara.showsymbol)
			{
				itemDef->showNullMarker();
				break;
			}
			else
			{
				itemDef->hideNullMark();
				break;
			}
		}
	}

    QWidget *w = legend()->find(item);
    if ( w && w->inherits("QwtLegendItem"))
        ((QwtLegendItem*)w)->setChecked(on);
	setAutoReplot(true);
    replot();
}

void DataCurvePlot::alignScales()
{
    // The code below shows how to align the scales to
    // the canvas frame, but is also a good example demonstrating
    // why the spreaded API needs polishing.
	
    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
    canvas()->setLineWidth(1);
	
    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
    {
        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)axisWidget(i);
        if ( scaleWidget)
            scaleWidget->setMargin(0);
		
        QwtScaleDraw *scaleDraw = (QwtScaleDraw *)axisScaleDraw(i);
        if ( scaleDraw )
            scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
    }
}

void DataCurvePlot::appendCurveItemDef( const curveItemDef* curveItem )
{
	plotCurves.append(const_cast<curveItemDef*>(curveItem));

	QwtPlotCurve *curve=curveItem->curveItem;
	curve->attach(this);
	
	const bool cacheMode = canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);
#if QT_VERSION >= 0x040000 && defined(Q_WS_X11)
    // Even if not recommended by TrollTech, Qt::WA_PaintOutsidePaintEvent 
    // works on X11. This has an tremendous effect on the performance..
    canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif
	//	setAutoReplot(false);
//    curve.draw(1, curve.dataSize());
//	setAutoReplot(true);
    canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, cacheMode);
}

void DataCurvePlot::clear()
{
	QwtPlot::clear();
	plotCurves.clear();
	return;
}


//////////////////////////////////////////////////////////////////////////
////DateTimeScaleDraw implement
DateTimeScaleDraw::DateTimeScaleDraw( QString xAsisFormat )
{
	setLabelAlignment (Qt::AlignVCenter);
	_xAsisFormat=xAsisFormat;
}

QString DateTimeScaleDraw::xLable(double v) const
{
	DataCurveApp *thisApp=static_cast<DataCurveApp*>(qApp);
	if (YearCycle == thisApp->selpara.cycle)
	{
		uint timeSeqIdx = static_cast<uint>(v);
		QDateTime dtTmp ;
		dtTmp.setTimeSpec(thisApp->timeSpecValue);
		if(timeSeqIdx<_timeSeq.size()) 
		{
			dtTmp.setTime_t(_timeSeq[timeSeqIdx]);
			return dtTmp.toString("yyyy-MM-dd");
		}
		else
			return QString("");
	}
	else if (MonthCycle == thisApp->selpara.cycle)
	{
		QDateTime datetimeVal;
		datetimeVal.setTimeSpec(thisApp->timeSpecValue);
		datetimeVal.setTime_t(static_cast<uint>(v));
		return datetimeVal.toString("MM-dd hh:mm");
	} 
	else if (DayCycle == thisApp->selpara.cycle)
	{
		QDateTime datetimeVal;
		datetimeVal.setTimeSpec(thisApp->timeSpecValue);
		datetimeVal.setTime_t(static_cast<uint>(v));
		return datetimeVal.toString("hh:mm");
	}

}

QwtText DateTimeScaleDraw::label( double v ) const
{
	DataCurveApp *thisApp=static_cast<DataCurveApp*>(qApp);
	if (thisApp->selpara.cycle == YearCycle )
	{
		uint timeSeqIdx = static_cast<uint>(v);
		QDateTime dtTmp ;
		dtTmp.setTimeSpec(thisApp->timeSpecValue);
		if(timeSeqIdx<_timeSeq.size()) 
		{
			dtTmp.setTime_t(_timeSeq[timeSeqIdx]);
			return dtTmp.toString(_xAsisFormat);
		}
		else
			return QString("");
	}
	else
	{
		QDateTime datetimeVal;
		datetimeVal.setTimeSpec(thisApp->timeSpecValue);
		datetimeVal.setTime_t(static_cast<uint>(v));
		return datetimeVal.toString(_xAsisFormat);
	}
}

void DateTimeScaleDraw::setXAsisFormat( QString xAsisFormat ,QVector<uint> timeSeq )
{
	_xAsisFormat=xAsisFormat;
	_timeSeq=timeSeq;
}

void DataCurve::draw( QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, int from, int to ) const
{
	int nullBegin=to;
	CurveItemData *itemData = static_cast<CurveItemData*>(data().copy());
	
	for(int i=0;i<itemData->size();i++)
	{
		if( (( itemData->state(i) & illegal_data) == illegal_data))
		{
			nullBegin = i -1 ;
			if ( nullBegin < 0 ) nullBegin = 0;
			break;
		}
	}
	QwtPlotCurve::draw(painter,xMap,yMap,from,nullBegin);
}
