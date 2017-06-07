#include <QtGlobal>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPrinter>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QtAlgorithms>
#include "datacurveplot.h"
#include "datacurveview.h"

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include "qwt_legend_item.h"
#include "qwt_plot_marker.h"
#include "qwt_scale_engine.h"

#include <QMainWindow>
#include <QStatusBar>
#include <qwt_plot_picker.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


DataCurveView::DataCurveView( QWidget * parent /*= 0*/ )
:QAbstractItemView(parent),
currentSelector(static_cast<DataCurveApp*>(qApp)->selpara)
{
	plot=new DataCurvePlot(this);
	scaleDraw=new DateTimeScaleDraw(C2G("hh"));
	plot->setAxisScaleDraw(QwtPlot::xBottom,scaleDraw);


	statText=new QLineEdit(this);
	statText->setObjectName(QString::fromUtf8("statText"));
	statText->setFrame(false);
	statText->setVisible(false);
	statText->setReadOnly(true);
	statText->setMaximumHeight(20);  // only one line

	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(10);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
    mainLayout->addWidget(plot);
	mainLayout->addWidget(statText);
	setLayout(mainLayout);

	m_qwtPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPicker::PointSelection | QwtPicker::DragSelection, 
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOff, 
		plot->canvas());
	m_qwtPicker->setRubberBandPen(QColor(Qt::green));
	m_qwtPicker->setRubberBand(QwtPicker::CrossRubberBand);
	m_qwtPicker->setTrackerPen(QColor(Qt::black));

	connect(m_qwtPicker, SIGNAL(moved(const QPoint &)),
		SLOT(mouseMoved(const QPoint &)));

}

DataCurveView::~DataCurveView()
{
}

void DataCurveView::showInfo(QString strInfo)
{

	QMainWindow *mainWin = NULL;
	foreach (QWidget *widget, QApplication::allWidgets()){
		if ( widget->inherits("QMainWindow")) {
			mainWin = (QMainWindow *)widget;
			break;
		}
	}
	if(mainWin!=NULL) 
	{
#ifndef QT_NO_STATUSBAR
#if QT_VERSION >= 0x040000
		mainWin->statusBar()->showMessage(strInfo);
#else
		mainWin->statusBar()->message(strInfo);
#endif
#endif
	}

}

bool DataCurveView::findCurveVal(const DataCurve *pCurvData,double x,double &yVal)
{
	int count = pCurvData->dataSize();
	if (count <= 0)
	{
		return false;
	}

	int xMin = pCurvData->minXValue();
	int xMax = pCurvData->maxXValue();

	if ( (x > xMax) || (x< xMin) )
	{
		return false;
	}

	int x1,x2,y1,y2;

	for (int i=0;i<count;i++)
	{
		x2 = pCurvData->x(i);
		y2 = pCurvData->y(i);

		if (x2 >= x)
		{
			if (i>0)
			{
				x1 = pCurvData->x(i-1);
				y1 = pCurvData->y(i-1);
				yVal = y1 + ((y2-y1)/(x2-x1))*(x-x1);
			}
			else
			{
				yVal = pCurvData->y(i);	
			}
			return true;
		}
	}

/*
	if (count > 50)
	{
		int nBegin=0,nEnd=count-1;
		int nMid=(nBegin+nEnd)/2;
		while (nBegin<nEnd)
		{
			x1 = pCurvData->x(nMid);
			x2 = pCurvData->x(nMid+1);
			if (x >= x1 && x < x2)//找到了
			{
				if (x == x1)
				{
					yVal = pCurvData->y(nMid);
				}
				else
				{

					y1 = pCurvData->y(nMid);
					y2 = pCurvData->y(nMid+1);
					yVal = y1 + ((y2-y1)/(x2-x1))*(x-x1);
				}
				return true;
			}
			else
			{
				if (x > x1)
				{
					nBegin = nMid+1;
				}
				if (x < x1)
				{
					nEnd = nMid-1;
				}

				nMid = (nBegin+nEnd)/2;
			}
		}
	}
	else
	{
		for (int i=0;i<count;i++)
		{
			x2 = pCurvData->x(i);
			y2 = pCurvData->y(i);

			if (x2 >= x)
			{
				if (i>0)
				{
					x1 = pCurvData->x(i-1);
					y1 = pCurvData->y(i-1);
					yVal = y1 + ((y2-y1)/(x2-x1))*(x-x1);
				}
				else
				{
					yVal = pCurvData->y(i);	
				}
				return true;
			}
		}
	}
*/
	return true;
}

void DataCurveView::mouseMoved( const QPoint &pos)
{

	/*
	if(currentSelector.cycle==DayCycle)
	scaleDraw->setXAsisFormat(C2G("hh:mm"));
	else if(currentSelector.cycle==MonthCycle)
	scaleDraw->setXAsisFormat(C2G("MM-dd hh:mm"));
	else if(currentSelector.cycle==YearCycle)
	scaleDraw->setXAsisFormat(C2G("yyyy-MM-dd"),timeSeq);
	*/

	static int xLastPos;
	int xPos;
	xPos = pos.x();

	if (abs(double(xPos - xLastPos)) < 1)
	{
		return;
	}

	xLastPos = xPos;

	QString strInfo,strVal;

	int xVal = plot->invTransform(QwtPlot::xBottom,xPos);

	strInfo = C2G("时间=%1     ")
		.arg(scaleDraw->xLable(xVal) );

	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);

	QDateTime dateTime = QDateTime::currentDateTime();
	QDate date = dateTime.date();
	QTime time = dateTime.time();
	QDateTime tmp = QDateTime(date,time,Qt::UTC);

	int xValMax = tmp.toTime_t();

	if (xVal < xValMax)
	{
		int curveNum = plotCurves.count();

		curveItemDef *pCurv=0;
		DataCurve *pCurvData=0;
		double dVal = 0.0;

		for (int i=0;i<curveNum;i++)
		{
			pCurv = plotCurves[i];
			pCurvData = pCurv->curveItem;
			if (findCurveVal(pCurvData,xVal,dVal))
			{
				strVal = C2G("曲线%1：测值=%2    ")
					.arg(i+1)
					.arg(dVal);

				strInfo += strVal;
			}
		}
	}

	showInfo(strInfo);

}

void DataCurveView::reset()
{
	resetPlot();
}

void DataCurveView::dataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
/*
	QAbstractItemView::dataChanged(topLeft, bottomRight);
	if(plotCurves.size()==0) 
		return;
	int mCurveColIdx=topLeft.column()/2;

	if(topLeft==bottomRight) // only one point
	{
		curveItemDef* item=plotCurves[mCurveColIdx-1];
		QVector<int>	xData(item->curveItem->dataSize());
		QVector<double> yData(item->curveItem->dataSize());
		for(int i=0;i<item->curveItem->dataSize();i++ )
		{
			if(i==topLeft.row())
			{
				xData[i] = item->curveItem->x(i);
				yData[i] = model()->data(topLeft).toDouble();
			}
			else
			{
				xData[i] = item->curveItem->x(i);
				yData[i] = item->curveItem->y(i);
			}
		}
		item->curveItem->setData(CurveItemData(xData,yData));
	}
	plot->replot();
*/
}


void DataCurveView::currentChanged( const QModelIndex & current, const QModelIndex & previous )
{
	QAbstractItemView::currentChanged(current, previous);
}


void DataCurveView::selectAll()
{
}

void DataCurveView::columnsInserted( const QModelIndex & parent, int start, int end )
{
	
}

void DataCurveView::columnsRemoved( const QModelIndex & parent, int start, int end )
{
	
}

//////////////////////////////////////////////////////////////////////////
//曲线更新操作
void DataCurveView::resetPlot()
{
	plot->setAutoReplot(false);
	plot->clear();
	//
// 	scaleDraw=new DateTimeScaleDraw(C2G("hh"));
// 	plot->setAxisScaleDraw(QwtPlot::xBottom,scaleDraw);

	qDeleteAll(plotCurves);
	plotCurves.clear();
	updateXAxis(currentSelector,currentTimeSeq);
	for(int col=1;col<model()->columnCount();col+=2)
	{
		insertCurve(col);
	}
	//??
	plot->setTitle(getCurveTitle(currentSelector));
	plot->setAutoReplot(true);
	plot->replot();
	showStatText("");

}

void DataCurveView::updateXAxis( selectorpara& selpara,QVector<uint>& timeSeq)
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);

	QDateTime tmpDatetime;

	QModelIndex index;
	uint        seqVal,minSeqVal,maxSeqVal;
	int setpVal;

	timeSeq.clear();
	minSeqVal=maxSeqVal=-1;
	for(int row=0;row<model()->rowCount();row++)
	{
		index = model()->index(row,0, rootIndex());
		if(model()->data(index,Qt::UserRole).isNull()) {
			qDebug() << "DataCurveView::updateXAxis" << "model()->data(index,Qt::UserRole).isNull()";
		} 
		if(model()->data(index,Qt::UserRole)!=curve_value)
			continue;
		tmpDatetime=model()->data(index,Qt::UserRole+1).toDateTime();
		tmpDatetime.setTimeSpec(appSettings->timeSpecValue);
		seqVal=tmpDatetime.toTime_t();

		timeSeq.append(seqVal);
		if(minSeqVal==-1) minSeqVal=seqVal;
		if(maxSeqVal==-1) maxSeqVal=seqVal;
		minSeqVal=qMin<int>(minSeqVal,seqVal);
		maxSeqVal=qMax<int>(maxSeqVal,seqVal);
	}

	CurveItemData::setTimeSequence(timeSeq);

	if(currentSelector.cycle==DayCycle)
		scaleDraw->setXAsisFormat(C2G("hh"));
	else if(currentSelector.cycle==MonthCycle)
		scaleDraw->setXAsisFormat(C2G("dd"));
	else if(currentSelector.cycle==YearCycle)
		scaleDraw->setXAsisFormat(C2G("yy-MM"),timeSeq);

	if(selpara.cycle==DayCycle)
	{
		setpVal=60*60; //hour setp
	}
	else if(selpara.cycle==MonthCycle)
	{
		setpVal=60*60*24; //day setp;
	}
	else if(selpara.cycle==YearCycle)
	{
		plot->setAxisScale(QwtPlot::xBottom,0,timeSeq.size()-1,1);
		//plot->setAxisAutoScale(QwtPlot::xBottom);
		return ;
	}
	plot->setAxisScale(QwtPlot::xBottom,minSeqVal,maxSeqVal,setpVal);
}


void DataCurveView::insertCurve( int column )
{
	uint timeSeqVal;
	int  curveRowBegin;
	double curveVal;
	QModelIndex index,dateIndex;
	QVector<uint>			XCurveVal;
	QVector<double>			YCurveVal;
	QVector<DataState>		CurveValState;
	QString nameOfKey;
	
	//init curve data
	curveRowBegin=-1;
	for(int row=0;row<model()->rowCount();row++)
	{
		index = model()->index(row,column, rootIndex());
		dateIndex = model()->index(row,0, rootIndex());
		if(model()->data(dateIndex,Qt::UserRole).toInt()!=curve_value)
			continue;
		if(curveRowBegin<0)
			curveRowBegin=row;
		DataState state=static_cast<DataState>(model()->data(index,Qt::UserRole).toInt());
		QDateTime tmpDatetime = model()->data(dateIndex,Qt::UserRole+1).toDateTime();
		tmpDatetime.setTimeSpec(((DataCurveApp*)qApp)->timeSpecValue);
		timeSeqVal=tmpDatetime.toTime_t();
		curveVal=model()->data(index,Qt::UserRole+1).toDouble();
		//current version not process empty value,use model set value;
		//and inspect current datetime 
		if((state & original_data))
		{
			XCurveVal.append(timeSeqVal);
			YCurveVal.append(curveVal);
		}
		else if(state & empty_data)
		{
			XCurveVal.append(timeSeqVal);
			YCurveVal.append(curveVal);
		} else {
			qDebug() << "Untreated model state :" << state;
		}

		//LiCG 20130318 get time sequence use datetime column value
		//get actual date value 
		QDateTime dtTmp,dtModel;
		Qt::TimeSpec dtSpec;
		dtModel = model()->data(model()->index(row,column-1, rootIndex()),Qt::UserRole+1).toDateTime();
		dtSpec = dtModel.timeSpec();
		dtTmp = QDateTime(QDate::currentDate(),QTime::currentTime(),dtSpec);
		if( dtModel < dtTmp)
		{
			CurveValState.append(state);
		}
		else
		{
			CurveValState.append(illegal_data);
		}
	}
	nameOfKey=model()->headerData(column,Qt::Horizontal,Qt::UserRole).toString();
	curveItemDef *curveDef=new curveItemDef(nameOfKey);
	CurveItemData curveData(XCurveVal,YCurveVal,CurveValState);
	//common set
	curveDef->curveItem->setRenderHint(QwtPlotItem::RenderAntialiased,currentCurveSelector.attrAntialiased);
	curveDef->curveItem->setCurveAttribute(QwtPlotCurve::Fitted,currentCurveSelector.attrFitted);
	//update max and min value;
	//
	curveDef->curveItem->setStyle((QwtPlotCurve::CurveStyle)currentCurveSelector.curvestyle);

	if(currentSelector.isCompType())
	{
		QString title;
		if(currentSelector.cycle==DayCycle)
		{
			title=QString("[%1]%2")
			.arg(model()->data(model()->index(curveRowBegin,column-1, rootIndex()),Qt::UserRole+1).toDateTime().toString("yyyy-MM-dd"))
			.arg(curveDef->describe());
			curveDef->curveItem->setTitle(title);
		}
	}
	else
	{
		QString strTitle = model()->headerData(column,Qt::Horizontal).toString();
		//qDebug()<<strTitle << "   " << curveDef->describe();
		if(strTitle.compare(curveDef->describe(),Qt::CaseInsensitive)==0)
			curveDef->curveItem->setTitle(curveDef->describe());
		else
			curveDef->curveItem->setTitle(tr("计划值曲线"));
	}
	

	if(currentSelector.isCompType())
	{
		QPen pen=curveDef->linePen();
		int  lineidx=column/2;
		pen.setColor(curveClrTab[lineidx]);
		curveDef->curveItem->setPen(pen);
	}
	else
	{
		if(model()->columnCount()/2==1)
		{
			curveDef->curveItem->setPen(curveDef->linePen());
		}
		else
		{
			QPen pen=curveDef->linePen();
			int  lineidx=column/2;
			pen.setColor(curveClrTab[lineidx]);
			curveDef->curveItem->setPen(pen);
		}
	}

	curveDef->curveItem->setData(curveData);
	//(curveDef->curveItem)->attach(plot);
	plot->appendCurveItemDef(curveDef);

	if(currentCurveSelector.showsymbol)
	{
		curveDef->curveItem->setSymbol(QwtSymbol(curveDef->symbolStyle(),curveDef->symbolBrush(),
		curveDef->symbolPen(),curveDef->symbolSize()));
		curveDef->showNullMarker();
	}
	

	QWidget *w=curveDef->curveItem->legendItem();
	if (w && w->inherits("QwtLegendItem"))
        ((QwtLegendItem*)w)->setChecked(true);  //default curve show in plot;

	

	//single curve has x,y title
	if(currentSelector.selType==SingleSelector)
	{
		QwtText xAxisTitle(curveDef->xAxisUnit());
		xAxisTitle.setRenderFlags(Qt::AlignRight);
		plot->setAxisTitle(QwtPlot::xBottom,xAxisTitle);
		
		QwtText yAxisTitle(curveDef->dataUnit());
		yAxisTitle.setRenderFlags(Qt::AlignRight);
		plot->setAxisTitle(QwtPlot::yLeft,yAxisTitle);
	}

	plotCurves.append(curveDef);
}


void DataCurveView::deleteCurve( int column )
{
	
}
void DataCurveView::updatePlot( int row,int col )
{
	
}


QString DataCurveView::getCurveTitle( selectorpara& selpara ) const
{
	QString title,descript;
	if(plotCurves.size()==0)
		return C2G("");
	
	if(selpara.selType==SingleSelector)
		descript=plotCurves[0]->describe();
	else
		descript=C2G("");
	if(selpara.cycle==DayCycle)
	{
		if(selpara.segment==GeneralDate)
			if(selpara.isCompType())
				title=QString(tr("%1%2"))
				.arg(descript)
				.arg(tr("日同比数据曲线"));
			else		
				title=QString(tr("%1%2%3"))
				.arg(selpara.beginDate.toString(tr("yyyy年MM月dd日")))
				.arg(descript)
				.arg(tr("日数据曲线"));
		else
			title=QString(tr("%1%2至%3%4\r\n%5%6"))
			.arg(selpara.beginDate.toString(tr("yyyy年MM月dd日")))
			.arg(selpara.beginTime.toString(tr("hh时mm分")))
			.arg(selpara.endDate.toString(tr("yyyy年MM月dd日")))
			.arg(selpara.endTime.toString(tr("hh时mm分")))	
			.arg(descript)
			.arg(tr("日数据曲线"));
	}
	else if(selpara.cycle==MonthCycle)
	{
		if(selpara.segment==GeneralDate)
			title=QString(tr("%1%2%3"))
			.arg(selpara.beginDate.toString(tr("yyyy年MM月")))
			.arg(descript)
			.arg(tr("月统计数据曲线"));
		else
			title=QString(tr("%1至%2\r\n%3%4"))
			.arg(selpara.beginDate.toString(tr("yyyy年MM月dd日")))
			.arg(selpara.endDate.toString(tr("yyyy年MM月dd日")))
			.arg(descript)
			.arg(tr("月统计数据曲线"));
	}
	else if(selpara.cycle==YearCycle)
	{
		if(selpara.segment==GeneralDate)
			title=QString(tr("%1%2%3"))
			.arg(selpara.beginDate.toString(tr("yyyy年")))
			.arg(descript)
			.arg(tr("年统计数据曲线"));
		else
			title=QString(tr("%1至%2\r\n%3%4"))
			.arg(selpara.beginDate.toString(tr("yyyy年MM月")))
			.arg(selpara.endDate.toString(tr("yyyy年MM月")))
			.arg(descript)
			.arg(tr("年统计数据曲线"));
	}
	
	return title;
}


//////////////////////////////////////////////////////////////////////////
//public plot
void DataCurveView::print( QPrinter *printer )
{
	if (!printer)
		return;

	QPainter p;
	if ( !p.begin(printer))
		return; // paint on printer

	printer->setColorMode (QPrinter::Color);
	printer->setFullPage(true);
	
	double aspect = double(width())/double(height());
	if (aspect < 1)
		printer->setOrientation(QPrinter::Portrait);
	else
		printer->setOrientation(QPrinter::Landscape);
	
	plot->print(*printer);

	p.end();
}




void DataCurveView::selectorChanged(selectorcurvepara& selpara )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	if(currentCurveSelector.curvestyle!=selpara.curvestyle)  //curve style changed
	{
		foreach(curveItemDef *item,plotCurves)
		{	
			item->curveItem->setStyle(static_cast<QwtPlotCurve::CurveStyle>(selpara.curvestyle));
			if(static_cast<QwtPlotCurve::CurveStyle>(selpara.curvestyle)==QwtPlotCurve::Dots && currentCurveSelector.showsymbol == true)
			{
				item->curveItem->setSymbol(QwtSymbol(item->symbolStyle(),item->symbolBrush(),
					item->symbolPen(),item->symbolSize()));
			}
		}
		
		plot->replot();
	}
	else if(currentCurveSelector.showgrid!=selpara.showgrid)
	{
		foreach(QwtPlotItem *item,plot->itemList())
		{
			if(item->rtti()==QwtPlotItem::Rtti_PlotGrid)
			{
				QwtPlotGrid *gridItem=static_cast<QwtPlotGrid*>(item);
				gridItem->setVisible(selpara.showgrid);
				plot->replot();
			}
		}
	}
	if(currentCurveSelector.showsymbol!=selpara.showsymbol)
	{
		plot->setAutoReplot(false);
		foreach(curveItemDef *item,plotCurves)
		{
			if(selpara.showsymbol)
			{
				item->curveItem->setSymbol(QwtSymbol(item->symbolStyle(),item->symbolBrush(),
					item->symbolPen(),item->symbolSize()));
				item->showNullMarker();
			}
			else
			{
				item->curveItem->setSymbol(QwtSymbol());
				item->hideNullMark();
			}
		}
		plot->setAutoReplot();
		plot->replot();
	}
	if(currentCurveSelector.yAxisAutoScale!=selpara.yAxisAutoScale )
	{
		if(selpara.yAxisAutoScale)
		{
			plot->setAxisAutoScale(QwtPlot::yLeft);
		}
		else
		{
			plot->setAxisScale(QwtPlot::yLeft,selpara.yAxisScaleMin,selpara.yAxisScaleMax);
		}
		plot->replot();
	}
	else if(selpara.yAxisAutoScale==false && 
		(currentCurveSelector.yAxisScaleMax!=selpara.yAxisScaleMax ||
		 currentCurveSelector.yAxisScaleMin!=selpara.yAxisScaleMin ))
	{
		plot->setAxisScale(QwtPlot::yLeft,selpara.yAxisScaleMin,selpara.yAxisScaleMax);
		plot->replot();
	}
	if(currentCurveSelector.attrAntialiased!=selpara.attrAntialiased)
	{
		foreach(curveItemDef *item,plotCurves)
		{
			item->curveItem->setRenderHint(QwtPlotItem::RenderAntialiased,selpara.attrAntialiased);
		}
		plot->replot();
	}
	if(currentCurveSelector.attrFitted!=selpara.attrFitted )
	{
		foreach(curveItemDef *item,plotCurves)
		{
			if(selpara.curvestyle==static_cast<int>(QwtPlotCurve::Lines))
				item->curveItem->setCurveAttribute(QwtPlotCurve::Fitted,selpara.attrFitted);
			else
				item->curveItem->setCurveAttribute(QwtPlotCurve::Fitted,false);
		}
		plot->replot();
	}
	
	else if(currentCurveSelector.nullValueStyle!=selpara.nullValueStyle)
	{
		//curveModel process nullvalue;
	}
	currentCurveSelector=selpara;
}

void DataCurveView::showStatText(QString text )
{
	QModelIndex index,indexnextcol;
	QString     info,tmpstr;
	if(model()->columnCount() > ONE_CURVE_TABLE_COLUMN && !currentSelector.isPlanType())
	{
		statText->setVisible(false);
		return;
	}
	statText->setVisible(true);

	for(int row=0;row<model()->rowCount();row++)
	{
		index = model()->index(row,0, rootIndex());
		if(model()->data(index,Qt::UserRole).isNull()) {
			//qDebug()<<"DataCurveView::showStatText" << "model()->data(index,Qt::UserRole).isNull()";
		}
		if(model()->data(index,Qt::UserRole)!=stat_value)
			continue;
		indexnextcol=model()->index(row,1,rootIndex());
		tmpstr=QString("%1:%2")
			.arg(model()->data(index,Qt::DisplayRole).toString())
			.arg(model()->data(indexnextcol,Qt::DisplayRole).toString());
		info.append(tmpstr);
		info.append("   ");
	}
	statText->setText(info);
}



//////////////////////////////////////////////////////////////////////////
///CurveItemDef
curveItemDef::curveItemDef()
:curveItem(NULL),_nameOfKey(""),_dataType(0)
{
	
}

curveItemDef::curveItemDef(QString nameOfKey )
:curveItem(NULL),_dataType(0),_nameOfKey(nameOfKey)
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	_nameOfKey=nameOfKey;
	curveItem=new DataCurve();
	if(myApp->selpara.dataSrc==YcDataSrc)
	{
		YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,_nameOfKey);
		if(itFind!=myApp->ycparas.constEnd())
		{
			_dataType = (*itFind)->type;
		}
	}
	else if(myApp->selpara.dataSrc==DdDataSrc)
	{
		KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,_nameOfKey);
		if(itFind!=myApp->kwhparas.constEnd())
		{
			_dataType = (*itFind)->type;
		}
	}
}

curveItemDef::~curveItemDef()
{
	//if(curveItem!=NULL) delete curveItem;
	_nullValMarkers.clear();
}

void curveItemDef::showNullMarker()
{
	if(_nullValMarkers.count()>0)
		hideNullMark();
	if(curveItem != NULL)
	{
		QwtPlot *plot = curveItem->plot();
		CurveItemData *data = static_cast<CurveItemData*>(curveItem->data().copy());
		QwtPlotMarker *nullMark;
		for(int i=0;i<data->size();i++)
		{
			if( (data->state(i) & empty_data) == empty_data  && curveItem->isVisible()==true)
			{
				nullMark=new QwtPlotMarker;
				_nullValMarkers.append(nullMark);
				QSize markSize = symbolSize();
				markSize = QSize(markSize.width()-1,markSize.height()-1);
				nullMark->setSymbol(QwtSymbol(QwtSymbol::XCross,_symbolBursh,_symbolPen,markSize));
				nullMark->setValue(data->x(i),data->y(i));
				nullMark->attach(plot);
			}
		}
	}
	return;
}

void curveItemDef::hideNullMark()
{
 	foreach(QwtPlotMarker* item,_nullValMarkers)
 	{
 		item->detach();
 	}

}


QPen curveItemDef::linePen()
{
	QPen pen;
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	foreach(curvetypepara typepara,appSettings->curveTypeDefs)
	{
		if(typepara.Type()==_dataType)
		{
			pen=QPen(typepara.Color());
			pen.setWidth(appSettings->curveLineWidth);
			pen.setStyle(Qt::SolidLine);
			return pen;
		}
	}
	pen.setColor(appSettings->defaultColor);
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(appSettings->curveLineWidth);
	return pen;
}

QwtSymbol::Style curveItemDef::symbolStyle()
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	return (QwtSymbol::Style)appSettings->curveSymbolStyle;
}

QBrush curveItemDef::symbolBrush()
{
	QBrush brush;
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	foreach(curvetypepara typepara,appSettings->curveTypeDefs)
	{
		if(typepara.Type()==_dataType)
		{
			brush=QBrush(Qt::gray);
			return brush;
		}
	}
	brush=QBrush(Qt::NoBrush);
	return brush;
}

QPen curveItemDef::symbolPen()
{
	QPen pen;
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	foreach(curvetypepara typepara,appSettings->curveTypeDefs)
	{
		if(typepara.Type()==_dataType)
		{
			pen=QPen(typepara.Color());
			pen.setWidth(1);
			pen.setStyle(Qt::SolidLine);
			return pen;
		}
	}
	pen.setColor(appSettings->defaultColor);
	pen.setStyle(Qt::SolidLine);
	return pen;
}

QSize curveItemDef::symbolSize()
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	return appSettings->curveSymbolSize;
}

int curveItemDef::dataType()
{
	return _dataType;
}

QString curveItemDef::describe()
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	if(myApp->selpara.dataSrc==YcDataSrc)
	{
		YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,_nameOfKey);
		if(itFind!=myApp->ycparas.constEnd())
		{
			return (*itFind)->describe;
		}
	}
	else if(myApp->selpara.dataSrc==DdDataSrc)
	{
		KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,_nameOfKey);
		if(itFind!=myApp->kwhparas.constEnd())
		{
			return (*itFind)->describe;
		}
	}
	return C2G("");
}

QString curveItemDef::dataUnit()
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	foreach(curvetypepara typepara,appSettings->curveTypeDefs)
	{
		if(typepara.Type()==_dataType)
		{
			return typepara.Unit();
		}
	}
	return C2G("");
}

QString curveItemDef::xAxisUnit()
{
	selectorpara &selPara=(static_cast<DataCurveApp*>(qApp))->selpara;
	if(selPara.cycle==DayCycle)
		return QObject::tr("小时");
	else if(selPara.cycle==MonthCycle)
		return QObject::tr("日");
	else if(selPara.cycle==YearCycle)
		return QObject::tr("月");
	return C2G("");
}



