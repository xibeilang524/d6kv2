#include "trenddatacurveplot.h"

#include <qwt_plot_grid.h>
#include <qwt_legend_item.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>
#include "qwt_scale_widget.h"

TrendDataCurvePlot::TrendDataCurvePlot( QWidget * parent /*= NULL*/ )
:QwtPlot(parent)
{
	setAutoReplot(false);

	//make one curve 
	varCurve = new QwtPlotCurve();
#if QT_VERSION >= 0x040000
	varCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
// 	varCurve->setPen(QPen(Qt::black));
	varCurve->setPen(QPen(Qt::red));
	varCurve->setYAxis(QwtPlot::yLeft);
	varCurve->attach(this);

	//make grid
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	grid->enableXMin(true);
	grid->enableYMin(true);
	grid->attach(this);
	
	//make legend
	QwtLegend *legend = new QwtLegend(this);
	legend->setItemMode(QwtLegend::CheckableItem);
	legend->setDisplayPolicy(QwtLegend::FixedIdentifier,QwtLegendItem::ShowLine | QwtLegendItem::ShowText);
	this->insertLegend(legend, QwtPlot::BottomLegend);
	connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),SLOT(showCurve(QwtPlotItem *, bool)));

	//init curves env
	QwtPlotCanvas* plCanvas = canvas();
	plCanvas->setFocusPolicy(Qt::StrongFocus);
	plCanvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
	plCanvas->setFocus();
	plCanvas->setFrameShadow(QwtPlot::Plain);
	plCanvas->setPaintAttribute(QwtPlotCanvas::PaintCached, true);
	plCanvas->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
	plCanvas->setFrameStyle(QFrame::NoFrame);

	//init plot env
 	QColor background = QColor(Qt::white);
//	QColor background = QColor(Qt::black);
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

	setAutoReplot(true);
}

TrendDataCurvePlot::~TrendDataCurvePlot()
{

}

void TrendDataCurvePlot::showCurve( QwtPlotItem* item, bool on )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	this->setAutoReplot(false);
	const bool doReplot = this->autoReplot();
	item->setVisible(on);
	QWidget *w = this->legend()->find(item);
	if ( w && w->inherits("QwtLegendItem"))
		((QwtLegendItem*)w)->setChecked(on);

	this->setAutoReplot(doReplot);
	this->replot();
}


void TrendDataCurvePlot::clear()
{
	
}

void TrendDataCurvePlot::alignScales()
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
