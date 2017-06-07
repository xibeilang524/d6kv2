#include "variantcurveform.h"

#include <QPrinter>
#include <QPainter>
#include <QMainWindow>
#include <QStatusBar>
#include <QLineEdit>
#include <QVBoxLayout>


#include <qwt_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

#include "variantdatacurveplot.h"

class Zoomer: public QwtPlotZoomer
{
public:
	Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
	  QwtPlotZoomer(xAxis, yAxis, canvas)
	  {
		  setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
		  setTrackerMode(QwtPicker::AlwaysOff);
		  setRubberBand(QwtPicker::NoRubberBand);
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



VariantCurveForm::VariantCurveForm( QWidget* parent /*= NULL*/ )
:QWidget(parent),zoomer(NULL),picker(NULL),panner(NULL)
{
	plot = new VariantDataCurvePlot(this);
	//make scale draw 
	varScaleDraw=new varTimeScaleDraw(C2G("hh:mm:ss:zzz"));
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

VariantCurveForm::~VariantCurveForm()
{
	if(zoomer!=NULL) delete zoomer;
	if(panner!=NULL) delete panner;
	if(picker!=NULL) delete picker;
}

void VariantCurveForm::print( QPrinter *printer )
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

void VariantCurveForm::updateCurveData( selectorpara& selPara )
{
	if(selPara.cycle != VariantDayCycle || selPara.paras.count() == 0) return;
	DataCurveDefine &curveDef = static_cast<DataCurveApp*>(qApp)->dataCurveDef;
// 	DbVariantOneCurveData varCurveData;
	//read data from db
	if (!varCurveData.isEmpty())
	{
		varCurveData.clear();
	}
	
	bool readit=readVariantDayHisData(selPara.paras[0],selPara.beginDate,varCurveData);

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
	zoomer->setTrackerMode(QwtPicker::AlwaysOff);
	zoomer->setTrackerPen(QColor(Qt::white));

	panner = new QwtPlotPanner(plot->canvas());
	panner->setMouseButton(Qt::MidButton);

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
		SLOT(mouseMoved(const QwtDoublePoint &)));

	enableZoomMode(true);
	showInfo();

}

void VariantCurveForm::selectorChanged( selectorcurvepara& selpara ) {
	qDebug() << "";
}

void VariantCurveForm::resetPlot( const DbVariantOneCurveData& varOneCurveData )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);

	const bool doReplot = plot->autoReplot();
	plot->setAutoReplot(false);
	
	//updateXAxis day
	uint seqVal,minSeqVal=-1,maxSeqVal=-1;
	int setpVal;
	QDateTime tmpDatetimeLocal;// 本地时区处理 [3/6/2014 qianjin]
	QDate date;
	QTime time;
	tmpDatetimeLocal.setTimeSpec(Qt::LocalTime);

	for(int i=0;i<varOneCurveData.size();i++) {

		tmpDatetimeLocal.setTime_t(varOneCurveData[i].datetime);
		date = tmpDatetimeLocal.date();
		time = tmpDatetimeLocal.time();
		QTime zerotime(0,0,0);
		seqVal = zerotime.msecsTo(time)+varOneCurveData[i].ms;

		if(minSeqVal==-1) minSeqVal=seqVal;
		if(maxSeqVal==-1) maxSeqVal=seqVal;
		minSeqVal=qMin<int>(minSeqVal,seqVal);
		maxSeqVal=qMax<int>(maxSeqVal,seqVal);
	}
	setpVal=60; 
	plot->setAxisScale(QwtPlot::xBottom,minSeqVal,maxSeqVal/*,setpVal*/);
	//update value
	int arraySize = varOneCurveData.count();
	double *timeSeq = new double[arraySize];
	double *ycData =new double[arraySize];


	for(int i=0;i<arraySize;i++) {

		tmpDatetimeLocal.setTime_t(varOneCurveData[i].datetime);
		date = tmpDatetimeLocal.date();
		time = tmpDatetimeLocal.time();
		//为了精确到毫秒。舍弃日期信息，数据存储时都是按日期存储的。
		QTime zerotime(0,0,0);
		timeSeq[i] = zerotime.msecsTo(time)+varOneCurveData[i].ms;
		ycData[i] = varOneCurveData[i].ycdata;
	}
	
	plot->plotCurve()->setData(timeSeq,ycData,arraySize);
	plot->setAxisAutoScale(QwtPlot::yLeft);
	delete []timeSeq;
	delete []ycData;
	//showStatText("");
	plot->setAutoReplot(doReplot);
	plot->replot();
}

bool VariantCurveForm::readVariantDayHisData( QString paraName,QDate curveDate,DbVariantOneCurveData& varOneCurveData ) const
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	QString sqlTable,sqlWord,sqlWhere;
	QVector<QVariant> tmpContent;
	int i,j,colCnt;
	QDate tmpDate = curveDate;

	sqlTable=QString("%1%2%3%4")
		.arg(VARIANTCURVETABLEPREFIX)
		.arg(tmpDate.year(),4,10,QLatin1Char('0'))
		.arg(tmpDate.month(),2,10,QLatin1Char('0'))
		.arg(tmpDate.day(),2,10,QLatin1Char('0'));
	sqlWhere = C2G("代码='%1'").arg(paraName);
	sqlWord=C2G("select 代码,处理时间秒,处理时间毫秒,值 from %1 where %2 order by 处理时间秒,处理时间毫秒").arg(sqlTable).arg(sqlWhere);
	colCnt=myApp->dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
	if(colCnt>0) {
		varOneCurveData.clear();
		for( i=0;i<tmpContent.size();i+=colCnt)
		{
			j=i;
			VariantHisDataValue tmpData;
			tmpData.name=tmpContent[j++].toString();
			tmpData.datetime=tmpContent[j++].toUInt();
			tmpData.ms = tmpContent[j++].toUInt();
			tmpData.ycdata=tmpContent[j++].toFloat();
			varOneCurveData.append(tmpData);
		}
	}
	return varOneCurveData.size();
}

void VariantCurveForm::enableZoomMode( bool on )
{
	panner->setEnabled(on);
	zoomer->setEnabled(on);
	zoomer->zoom(0);
	picker->setEnabled(on);
	showInfo();
}

void VariantCurveForm::showInfo( QString text /*= QString::null*/ )
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

void VariantCurveForm::mouseMoved( const QwtDoublePoint &pos )
{
 	static int xLastPos;
 	int xPos;
 	xPos = pos.x();
 
 	if (abs(double(xPos - xLastPos)) < 1)
 	{
 		return;
 	}
 
 	xLastPos = xPos;
 
 	QString strInfo,strVal;
	double dVal = 0.0;

 	int xVal = xPos;
 	char msec[10];
 	strInfo = C2G("时间=%1     ")
 		.arg(varScaleDraw->xLable(xVal) );

	if (findWaveCurveVal(xVal,dVal))
	{
		strVal = C2G( "测值=%1 ").arg(dVal);

		strInfo += strVal;
	}
	showInfo(strInfo);
}

void VariantCurveForm::selected( const QwtPolygon & )
{
	showInfo();
}

QString VariantCurveForm::getCurveTitle( selectorpara& selpara ) const
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
	title=QString(tr("%1%2%3")).arg(selpara.beginDate.toString(tr("yyyy年MM月dd日"))).arg(descript).arg(tr("日数据曲线"));
	return title;
}

bool VariantCurveForm::findWaveCurveVal(double x,double &yVal)
{
	QwtPlotCurve* pCurveData = plot->plotCurve();
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

	int x1,x2;
	double y1,y2;

	for (int i=0;i<count;i++)
	{
		x2 = pCurveData->x(i);
		y2 = pCurveData->y(i);

		if (x2 >= x)
		{
			if (i>0)
			{
				x1 = pCurveData->x(i-1);
				y1 = pCurveData->y(i-1);
				yVal = y1 + ((y2-y1)/(x2-x1))*(x-x1);
			}
			else
			{
				yVal = pCurveData->y(i);	
			}
			return true;
		}
	}
	return true;
}

