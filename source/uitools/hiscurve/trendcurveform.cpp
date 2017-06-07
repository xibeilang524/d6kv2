#include "trendcurveform.h"

#include <QPrinter>
#include <QPainter>
#include <QMainWindow>
#include <QStatusBar>
#include <QLineEdit>
#include <QVBoxLayout>


#include <qwt_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

#include "trenddatacurveplot.h"

class Zoomer: public QwtPlotZoomer
{
public:
	Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
	  QwtPlotZoomer(xAxis, yAxis, canvas)
	  {
		  setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
// 		  setTrackerMode(QwtPicker::AlwaysOff);
// 		  setRubberBand(QwtPicker::NoRubberBand);
		  setTrackerMode(QwtPicker::AlwaysOff);
		  setRubberBand(QwtPicker::CrossRubberBand);
		  // RightButton: zoom out by 1
		  // Ctrl+RightButton: zoom out to full size
#if QT_VERSION < 0x040000
		  setMousePattern(QwtEventPattern::MouseSelect2,
			  Qt::RightButton, Qt::ControlButton);
#else
		  setMousePattern(QwtEventPattern::MouseSelect2,
			  Qt::RightButton, Qt::ControlModifier);
#endif
		  setMousePattern(QwtEventPattern::MouseSelect3,
			  Qt::RightButton);
	  }
};



TrendCurveForm::TrendCurveForm( QWidget* parent /*= NULL*/ )
:QWidget(parent),zoomer(NULL),picker(NULL),panner(NULL)
{
	plot = new TrendDataCurvePlot(this);
	//make scale draw 
	varScaleDraw=new trendTimeScaleDraw(C2G("hh:mm:ss.zzz"));
	plot->setAxisScaleDraw(QwtPlot::xBottom,varScaleDraw);
	plot->setAxisTitle(QwtPlot::xBottom, C2G("小时"));
	plot->setAxisLabelAlignment(QwtPlot::xBottom,Qt::AlignRight);
	
	QPalette palette;
	palette.setColor(this->backgroundRole(), QColor(Qt::white));
	this->setPalette(palette);

	//make stat component
	statText=new QLineEdit(this);
	statText->setObjectName(QString::fromUtf8("statText"));
	statText->setFrame(false);
	statText->setVisible(false);
	statText->setReadOnly(true);
	statText->setMaximumHeight(20);  // only one line

	//build layout
	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(10);
	mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
	mainLayout->addWidget(plot);
	mainLayout->addWidget(statText);
	setLayout(mainLayout);
	
}

TrendCurveForm::~TrendCurveForm()
{
	if(zoomer!=NULL) delete zoomer;
	if(panner!=NULL) delete panner;
	if(picker!=NULL) delete picker;
}

void TrendCurveForm::print( QPrinter *printer )
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

void TrendCurveForm::updateCurveData( selectorpara& selPara )
{
	if(selPara.cycle != TrendDayCycle || selPara.paras.count() == 0) return;
	DataCurveDefine &curveDef = static_cast<DataCurveApp*>(qApp)->dataCurveDef;
	DbTrendOneCurveData varCurveData;
	//read data from db
	bool readit=readTrendDayHisData(selPara.paras[0],selPara.beginDate,varCurveData);

	//construct curve data array
	resetPlot(varCurveData);
	plot->setTitle(getCurveTitle(selPara));
	//save last ui paraselector
	lastSelector=selPara;

	//////////////////////////////////////////////////////////////////////////
	//direct init have problem , init them after update curve data
	//reinit zoom and move function
	if(zoomer!=NULL) delete zoomer;
	if(panner!=NULL) delete panner;
	if(picker!=NULL) delete picker;
	
	zoomer = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft, 
		plot->canvas());
	zoomer->setRubberBand(QwtPicker::RectRubberBand);
	zoomer->setRubberBandPen(QColor(Qt::green));
	// 设置鼠标显示坐标值
	zoomer->setTrackerMode(QwtPicker::AlwaysOff);
	zoomer->setTrackerPen(QColor(Qt::black));

	panner = new QwtPlotPanner(plot->canvas());
	panner->setMouseButton(Qt::MidButton);

	// 滚轮放大缩小功能。y轴在放大的时候，坐标不变化  
	(new QwtPlotMagnifier(plot->canvas()))->setAxisEnabled(QwtPlot::yLeft,false); 

	picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPicker::PointSelection | QwtPicker::DragSelection, 
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOff, 
		plot->canvas());
	picker->setRubberBandPen(QColor(Qt::green));
	picker->setRubberBand(QwtPicker::CrossRubberBand);
	picker->setTrackerPen(QColor(Qt::black));

// 	connect(picker, SIGNAL(moved(const QPoint &)),
// 		SLOT(moved(const QPoint &)));
// 	connect(picker, SIGNAL(selected(const QwtPolygon &)),
// 		SLOT(selected(const QwtPolygon &)));

	connect(picker, SIGNAL(selected(const QwtDoublePoint &)),
		SLOT(mouseSelected(const QwtDoublePoint &)));

	enableZoomMode(true);
	showInfo();

}

void TrendCurveForm::selectorChanged( selectorcurvepara& selpara ) {
	qDebug() << "";
}

void TrendCurveForm::resetPlot( const DbTrendOneCurveData& varOneCurveData )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);

	const bool doReplot = plot->autoReplot();
	plot->setAutoReplot(false);
	
	//updateXAxis day
	double seqVal,msecs,minSeqVal=-1,maxSeqVal=-1;
	int setpVal;
//	QDateTime tmpDatetime;
	QDateTime tmpDatetimeLocal;// 本地时区处理 [3/6/2014 qianjin]
	QDate date;
	QTime time;
//	tmpDatetime.setTimeSpec(appSettings->timeSpecValue);
	tmpDatetimeLocal.setTimeSpec(Qt::LocalTime);

	for(int i=0;i<varOneCurveData.size();i++) {

		tmpDatetimeLocal.setTime_t(varOneCurveData[i].datetime);
		date = tmpDatetimeLocal.date();
		time = tmpDatetimeLocal.time();
		msecs = varOneCurveData[i].ms;
		time = time.addMSecs(msecs);
		qDebug() << time.toString("hh:mm:ss:zzz");
// 		QDateTime tmp = QDateTime(date,time,Qt::UTC);		
// 		seqVal = tmp.toTime_t();
		seqVal = (time.hour()*60*60 + time.minute()*60 + time.second())*1000 + time.msec();
		// 获取到毫秒的值
		seqVal = seqVal+static_cast<double>(msecs)/1000;
		if(minSeqVal==-1) minSeqVal=seqVal;
		if(maxSeqVal==-1) maxSeqVal=seqVal;
		minSeqVal=qMin<int>(minSeqVal,seqVal);
		maxSeqVal=qMax<int>(maxSeqVal,seqVal);
		
		//qDebug() << tmpDatetime.toString("yyyy-MM-dd hh:mm:ss:zzz");
		tmpDatetimeLocal = tmpDatetimeLocal.addMSecs(varOneCurveData[i].ms);
		qDebug() << tmpDatetimeLocal.toString("yyyy-MM-dd hh:mm:ss:zzz");
	}
	setpVal=60; //hour setp : 60 minute * 60 second
//	plot->setAxisScale(QwtPlot::xBottom,minSeqVal,maxSeqVal,setpVal);
	plot->setAxisScale(QwtPlot::xBottom,minSeqVal,maxSeqVal/*,setpVal*/);
	//plot->setAxisMaxMinor(QwtPlot::xBottom,);
	//plot->setAxisMaxMinor(QwtPlot::xBottom,);
	
	//update value
	int arraySize = varOneCurveData.count();
	double *timeSeq = new double[arraySize];
	double *ycData =new double[arraySize];


	for(int i=0;i<arraySize;i++) {

		tmpDatetimeLocal.setTime_t(varOneCurveData[i].datetime);
		date = tmpDatetimeLocal.date();
		time = tmpDatetimeLocal.time();
		time = time.addMSecs(varOneCurveData[i].ms);

//     	QDateTime tmp = QDateTime(date,time,Qt::UTC);
		timeSeq[i] = (time.hour()*60*60 + time.minute()*60 + time.second())*1000 + time.msec();

// 		timeSeq[i] = tmp.toTime_t()+static_cast<double>(varOneCurveData[i].ms)/1000;
		ycData[i] = varOneCurveData[i].ycdata;
	}
	
	plot->plotCurve()->setStyle(QwtPlotCurve::Steps);
	plot->plotCurve()->setData(timeSeq,ycData,arraySize);
	plot->setAxisAutoScale(QwtPlot::yLeft);
	delete []timeSeq;
	delete []ycData;
	//showStatText("");
	plot->setAutoReplot(doReplot);
	plot->replot();
}

bool TrendCurveForm::readTrendDayHisData( QString paraName,QDate curveDate,DbTrendOneCurveData& varOneCurveData ) const
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	QString sqlTable,sqlWord,sqlWhere;
	QVector<QVariant> tmpContent;
	int i,j,colCnt;
	QDate tmpDate = curveDate;

	sqlTable=QString("%1%2%3%4")
		.arg(TRENDCURVETABLEPREFIX)
		.arg(tmpDate.year(),4,10,QLatin1Char('0'))
		.arg(tmpDate.month(),2,10,QLatin1Char('0'))
		.arg(tmpDate.day(),2,10,QLatin1Char('0'));
	sqlWhere = C2G("代码='%1'").arg(paraName);
	sqlWord=C2G("select 代码,处理时间秒,处理时间毫秒,值,终端序号,遥测序号 from %1 where %2 order by 处理时间秒,处理时间毫秒").arg(sqlTable).arg(sqlWhere);
	colCnt=myApp->dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
	if(colCnt>0) {
		varOneCurveData.clear();
		for( i=0;i<tmpContent.size();i+=colCnt)
		{
			j=i;
			TrendHisDataValue tmpData;
			tmpData.name=tmpContent[j++].toString();
			tmpData.datetime=tmpContent[j++].toUInt();
			tmpData.ms = tmpContent[j++].toUInt();
			tmpData.ycdata=tmpContent[j++].toFloat();
			varOneCurveData.append(tmpData);
		}
	}
	return varOneCurveData.size();
}

void TrendCurveForm::enableZoomMode( bool on )
{
	panner->setEnabled(on);
	zoomer->setEnabled(on);
	zoomer->zoom(0);
	picker->setEnabled(on);
	showInfo();
}

void TrendCurveForm::showInfo( QString text /*= QString::null*/ )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	if ( text == QString::null )
	{
		if ( picker->rubberBand() )
			text = QObject::tr("光标位置 : 在曲线图形中按下鼠标左键选择放大区域，使用鼠标中键来拖动曲线，使用鼠标右键缩小");
		else
			text = tr("缩放 : 按下鼠标按钮并拖动");
	}
	QMainWindow *mainWin = NULL;
	foreach (QWidget *widget, QApplication::allWidgets()){
		if ( widget->inherits("QMainWindow")) {
			mainWin = (QMainWindow *)widget;
			break;
		}
	}
	if(mainWin!=NULL) {
#ifndef QT_NO_STATUSBAR
#if QT_VERSION >= 0x040000
	mainWin->statusBar()->showMessage(text);
#else
	mainWin->statusBar()->message(text);
#endif
	}
#endif
}

void TrendCurveForm::moved( const QPoint &pos)
{
	QString info;
	info = C2G("时间=%1, 遥测值=%2").arg(varScaleDraw->label(plot->invTransform(QwtPlot::xBottom, pos.x())).text())
	.arg(plot->invTransform(QwtPlot::yLeft, pos.y()));
	showInfo(info);
}

void TrendCurveForm::mouseSelected( const QwtDoublePoint &pos)
{
	static int xLastPos;
	double xPos;
	xPos = pos.x();

	if (abs(double(xPos - xLastPos)) < 1)
	{
		return;
	}

	xLastPos = xPos;

	QString strInfo,strVal;

	// 	int xVal = plot->invTransform(QwtPlot::xBottom,xPos);
	double xVal = xPos;
	char msec[10];
	strInfo = C2G("时间=%1     ")
		.arg(varScaleDraw->xLable(xVal) );

	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);

	QwtPlotCurve *pCurvData = plot->plotCurve();
	double dVal = 0.0;

	if (findWaveCurveVal(pCurvData,xVal,dVal))
	{
		QString curvename = pCurvData->title().text();

		strVal = C2G("%1：测值=%2    ")
			.arg(curvename)
			.arg(dVal);

		strInfo += strVal;
	}


	showInfo(strInfo);
}

void TrendCurveForm::selected( const QwtPolygon & )
{
	showInfo();
}

QString TrendCurveForm::getCurveTitle( selectorpara& selpara ) const
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	QString title,descript;
	if(selpara.paras.count()==0) return C2G("");
	//get descript
	if(myApp->selpara.dataSrc==YcDataSrc)
	{
		YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,selpara.paras[0]);
		if(itFind!=myApp->ycparas.constEnd())
		{
			descript = (*itFind)->describe;
		}
	}
	else if(myApp->selpara.dataSrc==DdDataSrc)
	{
		KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,selpara.paras[0]);
		if(itFind!=myApp->kwhparas.constEnd())
		{
			descript = (*itFind)->describe;
		}
	}
	title=QString(tr("%1%2%3")).arg(selpara.beginDate.toString(tr("yyyy年MM月dd日"))).arg(descript).arg(tr("日趋势曲线"));
	return title;
}

bool TrendCurveForm::findWaveCurveVal(const QwtPlotCurve* pCurveData, double x,double &yVal)
{
	int count = pCurveData->dataSize();
	if (count <= 0)
	{
		return false;
	}

	int xMin = pCurveData->minXValue();
	int xMax = pCurveData->maxXValue();

	if ( (x > xMax) || (x< xMin) )
	{
		return false;
	}

	double x1,x2;
	double y1,y2;

	for (int i=0;i<count;i++)
	{
		x2 = pCurveData->x(i);
		y2 = pCurveData->y(i);

		if (x2 >= x)
		{
			yVal = y2;
			return true;
		}
	}
	return true;
}


