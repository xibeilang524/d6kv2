#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QIcon>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTextEdit>
#include <QStackedLayout>
#include <QItemDelegate>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QHeaderView>
#include <QClipboard>
#include <QUndoStack>
#include <QInputDialog>


#include <QPrintPreviewDialog>
// #include <QComboBox>
// #include <QDateEdit>
// #include <QTimeEdit>

// #include <QWidgetAction>

#include <QDateTime>
#include <QVariant>
#include <QDebug>

#include "mainwindow.h"
#include "selectorwidget.h"
#include "dataselectortoolbar.h"
#include "hisdatamodel.h"
#include "datacurveview.h"
#include "datatableview.h"
#include "datacurveapp.h"

#include "variantcurveform.h";
#include "trendcurveform.h"
#include "uitools/dmessagebox.h"


#if defined(EXCELFORMAT)
#include "./excelformat/ExcelFormat.h"
using namespace ExcelFormat;
#endif

#include <QFileDialog>

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags )
:QMainWindow(parent,flags),print_paper_size(QPrinter::A4),printer_orientation(QPrinter::Landscape)
{
	QString title(tr("历史曲线"));
#ifdef DBG_REGPROC
	netDbg.RegProc((char*)G2C(title));
#if defined(WIN32)
	WId wid = winId();
	procMan.StartProc("hiscurve",(char*)G2C(title),"",wid);
#else
	procMan.StartProc("hiscurve",(char*)G2C(title),"");
#endif
	procTimerID=startTimer(PROCINTERVAL);
#endif
	setWindowTitle(title);
	setWindowIcon( QIcon( ":/res/datacurve.png" ) ) ;
	init();
	mainTimerID=startTimer(MAININTERVAL);
}

MainWindow::~MainWindow()
{
#ifdef DBG_REGPROC
	netDbg.UnRegProc();
	procMan.EndProc();
	killTimer(procTimerID);
#endif
	killTimer(mainTimerID);
}



void MainWindow::init( bool factorySettings /*= false*/ )
{
	initCentralWidget();
	createActions(this);

	initMainMenu(this);
	initToolBars(this);
	initStatusBars(this);
	createDockWindows(this);
	initView();

	connect(selectorWidget,SIGNAL(selectorChanged(selectorpara&)),curveModel,SLOT(updateModelData(selectorpara&)));
	connect(selectorWidget,SIGNAL(selectorChanged(selectorpara&)),varCurveForm,SLOT(updateCurveData(selectorpara&)));
	connect(selectorWidget,SIGNAL(selectorChanged(selectorpara&)),trendCurveForm,SLOT(updateCurveData(selectorpara&)));
	connect(selectorWidget,SIGNAL(dataSrcChanged()),dataSelectorTools,SLOT(dataSrcChanged()));
	connect(dataSelectorTools,SIGNAL(selectorChanged(selectorpara&)),this,SLOT(selectorChanged(selectorpara&)));

	_undoStack = new QUndoStack(this);

	initStyleSheet() ;
}	

void MainWindow::initCentralWidget()
{
	centralwidget=new QWidget(this);
	centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
	
	viewLayerout=new QStackedLayout(centralwidget);
	curveView=new DataCurveView(this);
	curveView->setObjectName(QString::fromUtf8("curveView"));
	tableView=new DataTableView(this);
	tableView->setObjectName(QString::fromUtf8("tableView"));

	varCurveForm = new VariantCurveForm(this);
	varCurveForm->setObjectName(QString::fromUtf8("variantCurveForm"));
	trendCurveForm = new TrendCurveForm(this);
	trendCurveForm->setObjectName(QString::fromUtf8("trendCurveForm"));

	viewLayerout->addWidget(curveView);
	viewLayerout->addWidget(tableView);
	viewLayerout->addWidget(varCurveForm);
	viewLayerout->addWidget(trendCurveForm);

	centralwidget->setLayout(viewLayerout);
	setCentralWidget(centralwidget);
	connect(viewLayerout,SIGNAL(currentChanged(int)),this,SLOT(switchview(int)));
}



void MainWindow::initMainMenu( QWidget * parent )
{
	menubar = new QMenuBar(parent);
	
	menubar->setObjectName(QString::fromUtf8("menubar"));

	fileMenu = new QMenu(menubar);
	fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
	fileMenu->setTitle(tr("文件(&F)"));

	editMenu = new QMenu(menubar);
    editMenu->setObjectName(QString::fromUtf8("editMenu"));
	editMenu->setTitle(tr("编辑(&E)"));

    viewMenu = new QMenu(menubar);
    viewMenu->setObjectName(QString::fromUtf8("viewMenu"));
	viewMenu->setTitle(tr("视图(&V)"));

    helpMenu = new QMenu(menubar);
    helpMenu->setObjectName(QString::fromUtf8("helpMenu"));
	helpMenu->setTitle(tr("帮助(&H)"));


	menubar->addAction(fileMenu->menuAction());
    menubar->addAction(editMenu->menuAction());
    menubar->addAction(viewMenu->menuAction());
    menubar->addAction(helpMenu->menuAction());


	fileMenu->addAction(actionPrintpreview);
    fileMenu->addAction(actionPrint);
    fileMenu->addSeparator();
	fileMenu->addAction(actionExport->menuAction());
	{
	#if defined(EXCELFORMAT)
		actionExport->addAction(actionExportXls);
	#endif
		actionExport->addAction(m_actExportCsv);
		actionExport->addAction(actionExportImage);
	}
	fileMenu->addSeparator();
	fileMenu->addAction(actionExecProc);
	fileMenu->addAction(actionUpdateDb);
    fileMenu->addSeparator();
    fileMenu->addAction(actionExit);
	
	editMenu->addAction(actionUndo);
	editMenu->addAction(actionRedo);
	editMenu->addSeparator();
	editMenu->addAction(actionCopy);
	editMenu->addAction(actionPaste);
	editMenu->addSeparator();
	editMenu->addAction(actionRandonPaste);


	viewMenu->addAction(actionCurveView);
    viewMenu->addAction(actionDataView);
	viewMenu->addSeparator();
	viewMenu->addAction(actionViewSDate);


    helpMenu->addAction(actionAbout);

	setMenuBar(menubar);
}

void MainWindow::createDockWindows( QWidget* parent )
{
	   dockWidget = new QDockWidget(tr("设备对象树"),parent);
	   dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
	   dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::NoDockWidgetFeatures);
	   dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	   selectorWidget = new SelectorWidget(dockWidget);
	   selectorWidget->setObjectName(QString::fromUtf8("selectorWidget"));		
	   dockWidget->setWidget(selectorWidget);
	   dockWidget->setMinimumWidth( 300 ) ;
		
       ((QMainWindow*)parent)->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

void MainWindow::createActions( QWidget* parent )
{
	

	actionPrintpreview = new QAction(QIcon(":/res/printpreview.png"),tr("打印预览"),parent);
    actionPrintpreview->setObjectName(QString::fromUtf8("actionPrintpreview"));
	actionPrintpreview->setToolTip(tr("显示打印预览窗口进行打印前的文档预览"));
	actionPrintpreview->setStatusTip(actionPrintpreview->toolTip());
	actionPrintpreview->setShortcut( tr("Ctrl+Alt+P") );
	actionPrintpreview->setVisible(false);
	connect(actionPrintpreview, SIGNAL(activated()), parent, SLOT(printpreview()));

	actionPrint = new QAction(QIcon(":/res/print.png"),tr("打印"),parent);
    actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
	actionPrint->setShortcut( tr("Ctrl+P") );
	actionPrint->setVisible(false);
	connect(actionPrint, SIGNAL(activated()), parent, SLOT(print()));
	
	actionExport = new QMenu(tr("导出"),parent);
    actionExport->setObjectName(QString::fromUtf8("actionExport"));
#if defined(EXCELFORMAT)
	actionExportXls = new QAction(QIcon(":/res/exportxls.png"),tr("导出到Excel"),actionExport);
    actionExportXls->setObjectName(QString::fromUtf8("actionExportXls"));
	actionExportXls->setToolTip(tr("将当前曲线数据导出到XLS文件中，供EXCEL处理。"));
	actionExportXls->setStatusTip(tr("将当前曲线数据导出到XLS文件中，供EXCEL处理。"));
	connect(actionExportXls, SIGNAL(activated()), parent, SLOT(exportXls()));
#endif
	m_actExportCsv = new QAction(QIcon(":/res/exportxls.png"),tr("导出到CSV"),actionExport);
	m_actExportCsv->setObjectName(QString::fromUtf8("m_actExportCsv"));
	m_actExportCsv->setToolTip(tr("将当前曲线数据导出到CSV文件中。"));
	m_actExportCsv->setStatusTip(tr("将当前曲线数据导出到CSV文件中。"));
	connect(m_actExportCsv, SIGNAL(activated()), parent, SLOT(slotExportCsv()));


	actionExportImage = new QAction(QIcon(":/res/exportimage.png"),tr("导出到图片"),actionExport);
    actionExportImage->setObjectName(QString::fromUtf8("actionExportImage"));
	actionExportImage->setVisible(false);
	connect(actionExportImage, SIGNAL(activated()), parent, SLOT(exportImage()));

	actionExit = new QAction(QIcon(":/res/exit.png"),tr("退出"),parent);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
	actionPrint->setShortcut( tr("Alt+X") );
	connect(actionExit, SIGNAL(activated()), parent, SLOT(exit()));

	
	actionCopy = new QAction(QIcon(":/res/copy.png"),tr("复制"),parent);
    actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
	actionCopy->setShortcut( tr("Ctrl+C"));
	connect(actionCopy, SIGNAL(activated()), parent, SLOT(copy()));
	
	actionPaste = new QAction(QIcon(":/res/paste.png"),tr("粘贴"),parent);
    actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
	actionPaste->setShortcut( tr("Ctrl+V"));
	connect(actionPaste, SIGNAL(activated()), parent, SLOT(paste()));
	//
	actionRandonPaste = new QAction(tr("随机变化范围粘贴"),parent);
    actionRandonPaste->setObjectName(QString::fromUtf8("actionRandonPaste"));
	//actionRandonPaste->setShortcut( tr("Ctrl+V"));
	actionRandonPaste->setCheckable(true);
	actionRandonPaste->setChecked(false);
	connect(actionRandonPaste, SIGNAL(toggled(bool)), parent, SLOT(randomPaste(bool)));


	actionUndo = new QAction(QIcon(":/res/undo.png"),tr("撤销"),parent);
    actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
	actionUndo->setShortcut( tr("Ctrl+Z"));
	connect(actionUndo, SIGNAL(activated()), parent, SLOT(undo()));

	actionRedo = new QAction(QIcon(":/res/redo.png"),tr("重做"),parent);
    actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
	actionRedo->setShortcut( tr("Ctrl+Alt+Z"));
	connect(actionRedo, SIGNAL(activated()), parent, SLOT(redo()));


	actionCurveView = new QAction(QIcon(":/res/curveview.png"),tr("曲线"),parent);
    actionCurveView->setObjectName(QString::fromUtf8("actionCurveView"));
	actionCurveView->setToolTip(tr("切换视图到曲线视图浏览模式"));
	actionCurveView->setStatusTip(actionCurveView->toolTip());
	actionCurveView->setShortcut(tr("Alt+1"));
	actionCurveView->setCheckable(true);
	actionCurveView->setChecked(true);
	connect(actionCurveView, SIGNAL(toggled(bool)), parent, SLOT(curveview(bool)));

	actionDataView = new QAction(QIcon(":/res/dataview.png"),tr("表格"),parent);
    actionDataView->setObjectName(QString::fromUtf8("actionDataView"));
	actionDataView->setToolTip(tr("切换视图到数据表格视图浏览模式"));
	actionDataView->setStatusTip(actionDataView->toolTip());
	actionDataView->setShortcut( tr("Alt+2"));
	actionDataView->setCheckable(true);
	connect(actionDataView, SIGNAL(toggled(bool)), parent, SLOT(dataview(bool)));
	
	
	actionViewSDate = new QAction(tr("显示日期SDATE值"),parent);
    actionViewSDate->setObjectName(QString::fromUtf8("actionViewSDate"));
	actionViewSDate->setCheckable(true);
	actionViewSDate->setChecked(false);
	connect(actionViewSDate, SIGNAL(toggled(bool)), parent, SLOT(showSDate(bool)));

	QActionGroup *viewActionGroup=new QActionGroup(parent);
	viewActionGroup->addAction(actionCurveView);
	viewActionGroup->addAction(actionDataView);
	
	actionExecProc=new QAction(QIcon(":/res/execstatic.png"),tr("执行统计过程"),parent);
	actionExecProc->setObjectName(QString::fromUtf8("actionExecProc"));
	actionExecProc->setToolTip(tr("执行数据库中历史数据的统计过程，这通常在修改了历史库中数据后进行重新统计。"));
	actionExecProc->setStatusTip(actionExecProc->toolTip());
	actionExecProc->setEnabled(false);
	connect(actionExecProc, SIGNAL(activated()), parent, SLOT(execstatic()));
	
	actionUpdateDb=new QAction(QIcon(":/res/updatedb.png"),tr("保存修改到数据库"),parent);
	actionUpdateDb->setObjectName(QString::fromUtf8("actionUpdateDb"));
	actionUpdateDb->setToolTip(tr("将当前用户修改的历史数据库数据保存到数据库中。"));
	actionUpdateDb->setStatusTip(actionUpdateDb->toolTip());
	actionUpdateDb->setEnabled(false);
	connect(actionUpdateDb, SIGNAL(activated()), parent, SLOT(updatedb()));

	Q_ASSERT(curveView!=NULL);

	actionAbout = new QAction(tr("关于"),parent);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
	actionAbout->setToolTip(tr("显示关于本软件版权和帮助的信息。"));
	actionAbout->setStatusTip(actionAbout->toolTip());
	connect(actionAbout, SIGNAL(activated()), parent, SLOT(about()));
}

void MainWindow::initToolBars(QWidget * parent)
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	fileTools = new QToolBar(tr("文件工具"),parent);
    fileTools->setObjectName(QString::fromUtf8("file"));
	fileTools->setIconSize(appSettings->toolsIconSize);
    ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, fileTools);
#if defined(EXCELFORMAT)
 	fileTools->addAction(actionExportXls);
#endif
	fileTools->addAction(m_actExportCsv);

//  fileTools->addAction(actionExportImage);
	fileTools->addAction(actionExecProc);
	fileTools->addAction(actionUpdateDb);
    fileTools->addAction(actionExit);
	QWidget* pWidget = new QWidget( this ) ;
	fileTools->addWidget( pWidget ) ;
	
	viewTools = new QToolBar(tr("视图工具"),parent);
	viewTools->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    viewTools->setObjectName(QString::fromUtf8("file"));
	viewTools->setIconSize( appSettings->toolsIconSize);
    ((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, viewTools);
	
	viewTools->addAction(actionCurveView);
    viewTools->addAction(actionDataView);
	pWidget = new QWidget( this ) ;
	viewTools->addWidget( pWidget ) ;

	curveSelectorTools=new CurveSelectorToolBar(this);
	curveSelectorTools->setObjectName(QString::fromUtf8("curveSelectorTools"));
	curveSelectorTools->setIconSize( appSettings->toolsIconSize );
	((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, curveSelectorTools);

	dataSelectorTools=new DataSelectorToolBar(this);
	dataSelectorTools->setObjectName(QString::fromUtf8("dataSelectorTools"));
	dataSelectorTools->setIconSize(appSettings->toolsIconSize );
	((QMainWindow*)parent)->addToolBarBreak(Qt::TopToolBarArea);
	((QMainWindow*)parent)->addToolBar(Qt::TopToolBarArea, dataSelectorTools);	
}

void MainWindow::initView()
{
	tableView->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    tableView->setSelectionMode(QAbstractItemView::ContiguousSelection);// only one contiguous selection supported
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    tableView->setFocusPolicy(Qt::StrongFocus);
	//tableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    tableView->setFocus();

	// set header properties
	QHeaderView* hHeader = (QHeaderView*)tableView->horizontalHeader();
	hHeader->setMovable(false);
	hHeader->setResizeMode(QHeaderView::Interactive);
	hHeader->setDefaultSectionSize(100);

	delegate=new QItemDelegate(this);
	curveModel = new HisDataModel(this);
	curveModel->setObjectName(QString::fromUtf8("curveModel"));
	curveView->setModel(curveModel);
	tableView->setModel(curveModel);
	tableView->setItemDelegate(delegate);
	
	connect(dataSelectorTools,SIGNAL(selectorChanged(selectorpara&)),
		curveModel,SLOT(selectorChanged(selectorpara&)));
	connect(selectorWidget,SIGNAL(selectorChanged(selectorpara&)),this,SLOT(selectorWidgetChanged(selectorpara&)));
	connect(curveSelectorTools,SIGNAL(selectorChanged(selectorcurvepara&)),
		curveView,SLOT(selectorChanged(selectorcurvepara&)));
	connect(curveSelectorTools,SIGNAL(selectorChanged(selectorcurvepara&)),
		curveModel,SLOT(selectorCurveChanged(selectorcurvepara&)));
	//variant form signals
	connect(dataSelectorTools,SIGNAL(selectorChanged(selectorpara&)),varCurveForm,SLOT(updateCurveData(selectorpara&)));
	connect(curveSelectorTools,SIGNAL(selectorChanged(selectorcurvepara&)),varCurveForm,SLOT(selectorChanged(selectorcurvepara&)));
	//trend form signals
	connect(dataSelectorTools,SIGNAL(selectorChanged(selectorpara&)),trendCurveForm,SLOT(updateCurveData(selectorpara&)));
	connect(curveSelectorTools,SIGNAL(selectorChanged(selectorcurvepara&)),trendCurveForm,SLOT(selectorChanged(selectorcurvepara&)));

	//model reset curve value be recalc;
	connect(curveModel, SIGNAL(modelReset()), this, SLOT(curveModelReset()));

}

void MainWindow::initStatusBars(QWidget * parent)
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
// 	mainMsgLbl=new QLabel(parent);
// 	mainMsgLbl->setFrameStyle(QFrame::NoFrame);
// 	mainMsgLbl->setObjectName(QString::fromUtf8("mainMsgLbl"));
// 	mainMsgLbl->setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
// 	mainMsgLbl->setMinimumSize(mainMsgLbl->sizeHint());
	userStyleLbl=new QLabel(parent);
	userStyleLbl->setObjectName(QString::fromUtf8("userStyleLbl"));
	userStyleLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	QSize tmpSize = userStyleLbl->sizeHint() ;
	userStyleLbl->setMinimumSize( QSize( tmpSize.width() + 10 , tmpSize.height() ) );
	userStyleLbl->setText(QString(tr("用户类别[%1]")).arg(myApp->loginUserStyle()));

	userDescLbl=new QLabel(parent);
	userDescLbl->setObjectName(QString::fromUtf8("userDescLbl"));
	userDescLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	tmpSize = userDescLbl->sizeHint() ;
	userDescLbl->setMinimumSize( QSize( tmpSize.width() + 10 , tmpSize.height() ) );
	userDescLbl->setText(QString(tr("操作员描述[%1]")).arg(myApp->loginUser()));

// 	dbstatusLbl=new QLabel(parent);
// 	dbstatusLbl->setObjectName(QString::fromUtf8("dbstatusLbl"));
// 	dbstatusLbl->setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
// 	dbstatusLbl->setMinimumSize(userStyleLbl->sizeHint());

	datetimeLbl=new QLabel(parent);
	datetimeLbl->setObjectName(QString::fromUtf8("datetimeLbl"));
	datetimeLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	tmpSize = datetimeLbl->sizeHint() ;
	datetimeLbl->setMinimumSize( QSize( tmpSize.width() + 10 , tmpSize.height() ) );	

	statusbar = new QStatusBar(parent);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    setStatusBar(statusbar);

	statusbar->addPermanentWidget(userStyleLbl);
	statusbar->addPermanentWidget(userDescLbl);
	statusbar->addPermanentWidget(datetimeLbl);

	statusBar()->showMessage(tr("准备"));
}


//////////////////////////////////////////////////////////////////////////
//solt

void MainWindow::printpreview()
{
	
	QPrinter p;
	p.setPaperSize(print_paper_size);
	p.setOrientation(printer_orientation);
	
	QPrintPreviewDialog *preview = new QPrintPreviewDialog(&p, this, Qt::Window);
	QWidget *w=viewLayerout->currentWidget();
	preview->setWindowTitle(tr("曲线查看器") + " - " + tr("打印预览窗口: ")+ w->windowIconText());
	connect(preview, SIGNAL(paintRequested(QPrinter *)), tableView, SLOT(print(QPrinter *)));
	connect(preview, SIGNAL(paintRequested(QPrinter *)), curveView, SLOT(print(QPrinter *)));
	connect(preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(setPrintPreviewOptions(QPrinter *)));
	
	preview->exec();
}


void MainWindow::setPrintPreviewOptions( QPrinter* printer )
{
	if (!printer)
		return;
	print_paper_size = printer->paperSize();
	printer_orientation = printer->orientation();
}

void MainWindow::print()
{
	QPrinter p;
	p.setPaperSize(print_paper_size);
	p.setOrientation(printer_orientation);

	if(viewLayerout->currentIndex()==0)
	{
		tableView->print(&p);
	}
	else
	{
		curveView->print(&p);
	}
}

#if defined(EXCELFORMAT)
void MainWindow::exportXls()
{
	QString filepath;
	int row,col;
	QModelIndex index;
	BasicExcel xls;
	XLSFormatManager fmt_mgr(xls);

	xls.New(1);
	BasicExcelWorksheet* sheet = xls.GetWorksheet(0);
	BasicExcelCell* cell = sheet->Cell(0, 0);
	
	//init title from curvemodel
	for(col=0;col<curveModel->columnCount();col++)
	{
		cell=sheet->Cell(0,col);  
		QString headStr=(curveModel->headerData(col,Qt::Horizontal)).toString();
#ifdef Q_OS_WIN32
		wchar_t wchars[255];
		memset(wchars,0,sizeof(wchars));
		headStr.toWCharArray(wchars);
		cell->Set(wchars);
#else
		//china font may be problem ???
		cell->Set(G2C(headStr));
#endif
	}
	//fill timeseq and curvedata from curvemodel
	for(row=0;row<curveModel->rowCount();row++)
	{
		for(col=0;col<curveModel->columnCount();col++)
		{
			index=curveModel->index(row,col);
			cell=sheet->Cell(row+1,col);
			if(col==0)
				cell->Set(G2C((curveModel->data(index)).toString()));
			else
				cell->Set((curveModel->data(index)).toDouble());
		}
	}
	xls.SaveAs("lcg.xls");
}
#endif 

//导出到csv
void MainWindow::slotExportCsv()
{
	QList<QString>strCsvRow;
	QString filepath;
	int row,col;
	QModelIndex index;

	QString strTemp;

	//init title from curvemodel
	for(col=0;col<curveModel->columnCount();col++)
	{
		if (col)
		{
			strTemp += ",";
		}
		QString headStr=(curveModel->headerData(col,Qt::Horizontal)).toString();
		strTemp += headStr;
	}
	strCsvRow.append(strTemp);
	qDebug()<<strTemp<<"\n";

	//fill timeseq and curvedata from curvemodel
	for(row=0;row<curveModel->rowCount();row++)
	{
		strTemp.clear();
		for(col=0;col<curveModel->columnCount();col++)
		{
			if (col)
			{
				strTemp += ",";
			}
			index=curveModel->index(row,col);
			strTemp += curveModel->data(index).toString();
		}
		strCsvRow.append(strTemp);
	}

	qDebug()<<strCsvRow.count()<<"\n";

	QDir dirPath;
//	QString fileName;
	QString m_filePath;
	QString dirString = QFileDialog::getSaveFileName(this,tr("导出文件"),tr("未命名"),"*.csv");
	
	dirPath.setPath(dirString);
	m_filePath=dirPath.absolutePath();
	m_filePath=dirPath.toNativeSeparators(m_filePath);
	if(dirPath.isRoot())		//处理一下根目录情况，去掉最后的"/"符号
		m_filePath=m_filePath.mid(0,m_filePath.size()-1);
	
	m_filePath += ".csv";
	qDebug()<<m_filePath;

	QFile csvFile(m_filePath);

	if (csvFile.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QString temp;
		for (int k=0;k<strCsvRow.count();k++)
		{	
			temp = strCsvRow[k];
			csvFile.write(G2C(temp));
			csvFile.write("\n");
		}
		csvFile.close();
	}

//	bool bres = xls.SaveAs(G2C(m_filePath));
}

void MainWindow::exportImage()
{
	
}

void MainWindow::exit()
{
	qApp->closeAllWindows();
}


void MainWindow::copy()
{
	QClipboard *clipboard=qApp->clipboard();
	QStringList dataList;
	QItemSelectionModel *itemSelModel=tableView->selectionModel();
	QModelIndexList indexes=itemSelModel->selectedIndexes();
	QModelIndex index;
	foreach(index,indexes)
	{
		dataList.append(curveModel->data(index).toString());
	}
	qDebug()<< "table view select list" << dataList;
	clipboard->setText(dataList.join(C2G(",")),QClipboard::Clipboard);
	qDebug()<< "table view select text" << clipboard->text();
}

void MainWindow::paste()
{
	QClipboard *clipboard=qApp->clipboard();
	QString    dataText=clipboard->text(QClipboard::Clipboard);
	QStringList dataList = dataText.split(C2G(","));

	QItemSelectionModel *itemSelModel=tableView->selectionModel();
	QModelIndexList indexes=itemSelModel->selectedIndexes();
	QModelIndex index;
	
	int listIdx=0;
	foreach(index,indexes)
	{
		if(listIdx>=dataList.count())
			return;
		bool ok;
		double val = dataList[listIdx].toDouble(&ok);
		if(actionRandonPaste->isChecked())
		{
			double coefficient = qrand() % randomPercent - randomPercent/2;
			coefficient /=100;
			val = val + val*coefficient;
		}
		curveModel->setData(index,QVariant(val));
		listIdx++;
	}
}

void MainWindow::randomPaste(bool checked)
{
	if(actionRandonPaste->isChecked())
	{
		actionRandonPaste->setChecked(true);
		bool ok;
		randomPercent = QInputDialog::getInteger(this,tr("输入"),tr("输入粘贴数据随机变化范围的百分比"),0,0,100,1,&ok);
		if(!ok)
		{
			randomPercent = 0;
		}
	}
}


void MainWindow::undo()
{
	_undoStack->undo();
	qDebug()<<"MainWindow::undo()";
}

void MainWindow::redo()
{
	_undoStack->redo();
	qDebug()<<"MainWindow::redo()";
}



void MainWindow::curveview(bool checked)
{
	if(checked)
		viewLayerout->setCurrentIndex(CURVEVIEW_LAYEROUT_INDEX);
}

void MainWindow::dataview(bool checked)
{
	if(checked)
		viewLayerout->setCurrentIndex(CURVETABLE_LAYEROUT_INDEX);
}

void MainWindow::showSDate( bool checked )
{
	dataSelectorTools->showSDateText(checked);
}


void MainWindow::execstatic()
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
// 	QMessageBox msgBox;
// 	QString     msgDetails;
// 	msgDetails=QString(tr("重新统计对象:%1\r\n统计日期:%2"))
// 		.arg(myApp->selpara.getDescript())
// 		.arg(myApp->selpara.beginDate.toString(tr("yyyy年MM月dd日")));
// 	msgBox.setTextFormat(Qt::RichText);
// 	msgBox.setText(tr("<b>将执行数据库的日统计过程.</b>"));
// 	msgBox.setInformativeText(tr("执行过程中请耐心等待，继续吗？"));
// 	msgBox.setDetailedText(msgDetails);
// 	msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
// 	msgBox.setDefaultButton(QMessageBox::No);
// 	msgBox.setIcon(QMessageBox::Information);
// 	msgBox.setWindowTitle(tr("操作提示"));
// 	int ret=msgBox.exec();
	if(DMessageBox::question(NULL,tr("操作提示"),tr("执行过程中请耐心等待，继续吗?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
	{
		myApp->dataCurveDef.staticsProc(myApp->selpara.beginDate,myApp->selpara.paras[0],myApp->selpara.statType);
	}
	return;
}

void MainWindow::selectorWidgetChanged(selectorpara & selpara)
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);	
	if(!myApp->checkLoginUserRight(MODIFY_HISTORY_CURVE))
	{
		actionExecProc->setEnabled(false);
		actionUpdateDb->setEnabled(false);
		return;
	}
	else
		actionUpdateDb->setEnabled(true);
	selpara.cycle==DayCycle && selpara.segment==GeneralDate 
		&& selpara.selType==SingleSelector ? 
		actionExecProc->setEnabled(true) : actionExecProc->setEnabled(false);
}

//update variantDayCycle Env
void MainWindow::selectorChanged( selectorpara& selpara )
{
	if ( selpara.cycle == VariantDayCycle ) {
		viewLayerout->setCurrentIndex(VARIANTCURVEFORM_LAYEROUT_INDEX);
		viewTools->setEnabled(false);
		curveSelectorTools->setEnabled(false);
	} else if ( selpara.cycle == TrendDayCycle ) {
		viewLayerout->setCurrentIndex(TRENDCURVEFORM_LAYEROUT_INDEX);
		viewTools->setEnabled(false);
		curveSelectorTools->setEnabled(false);
	}	else if ( actionCurveView->isChecked()) {
		viewLayerout->setCurrentIndex(CURVEVIEW_LAYEROUT_INDEX);
		viewTools->setEnabled(true);
		curveSelectorTools->setEnabled(true);
	} else if (actionDataView->isChecked()) {
		viewLayerout->setCurrentIndex(CURVETABLE_LAYEROUT_INDEX);
		viewTools->setEnabled(true);
		curveSelectorTools->setEnabled(true);
	}
	
}

void MainWindow::updatedb()
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
// 	QMessageBox msgBox;
// 	
// 	msgBox.setText(tr("<b>将修改的数据保存到数据库中.</b>"));
// 	msgBox.setInformativeText(tr("执行过程中请耐心等待，继续吗？"));
// 	msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
// 	msgBox.setDefaultButton(QMessageBox::No);
// 	msgBox.setIcon(QMessageBox::Information);
// 	msgBox.setWindowTitle(tr("操作提示"));
// 	int ret=msgBox.exec();
// 	if(ret==QMessageBox::Yes)
	if(DMessageBox::question(NULL,tr("操作提示"),tr("执行过程中请耐心等待，继续吗?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
	{
		curveModel->updateModelDataToDB();
	}
	return;
	
}

void MainWindow::about()
{
//	QMessageBox msgBox;
	QString     msgText;
#ifdef Q_OS_WIN32
	msgText=QString(tr("<b style=color:#00F; font-size: large; >历史曲线数据浏览修改工具</b><br> \
		<code style=color:#00F; font-size: 24px;>版本:V2.0 2016年 版权所有<br>\
		<a>南京大全自动化科技有限公司</a><br>\
		<a href=\"http://www.daqo.com/\">http://www.daqo.com/</a></code>"));
#else
	msgText=QString(tr("<b style=color:#00F; >历史曲线数据浏览修改工具</b><br> \
		<code style=color:#00F;>版本:V2.0 2016年 版权所有<br>\
		<a>南京大全自动化科技有限公司</a><br>\
		<a href=\"http://www.daqo.com/\">http://www.daqo.com/</a></code>"));
#endif
	DMessageBox::about( this , tr( "关于本软件" ) , msgText , tr( ":/res/datacurve.png" ) ) ;
// 	msgBox.setText(msgText);
// 	msgBox.setStandardButtons(QMessageBox::Ok);
// 	msgBox.setDefaultButton(QMessageBox::No);
// 	msgBox.setIconPixmap(QPixmap(":/res/datacurve.png"));
// 	msgBox.setWindowTitle(tr("关于本软件"));
// 	msgBox.exec();
	return;
}

void MainWindow::switchview( int index )
{
	if(index<0) return;
	if(viewLayerout->widget(index)->inherits("DataCurveView"))
	{
		curveSelectorTools->setEnabled(true);
		actionCopy->setEnabled(false);
		actionPaste->setEnabled(false);
	}
	else if(viewLayerout->widget(index)->inherits("DataTableView"))
	{
		curveSelectorTools->setEnabled(false);
		actionCopy->setEnabled(true);
		actionPaste->setEnabled(true);
	} else if(viewLayerout->widget(index)->inherits("VariantCurveForm")) {  
		curveSelectorTools->setEnabled(true);
		actionCopy->setEnabled(false);
		actionPaste->setEnabled(false);
	}
}

void MainWindow::curveModelReset()
{
	double min,max;
	if(curveModel->curveDataBound(min,max))
	{
		curveSelectorTools->setYAsisBoxRange(min,max);
	}
}


void MainWindow::timerEvent( QTimerEvent *event )
{
#ifdef DBG_REGPROC
	if(event->timerId()==procTimerID)
	{
		procMan.CheckParentProc();
	}
#endif
	if(event->timerId()==mainTimerID)
	{
		datetimeLbl->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	}
}


void MainWindow::closeEvent( QCloseEvent *event )
{
	if(DMessageBox::question(this, tr("提示"),tr("是否退出历史曲线?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::No)
	{
		event->ignore();
		return;
	}
	event->accept();
}

void MainWindow::setTreeItemSelect( QString ycCode,QString date )
{
	if (!selectorWidget || !dataSelectorTools)
	{
		return;
	}
	dataSelectorTools->setDate(date);
	selectorWidget->setCurrentTreeItem(ycCode);

}

void MainWindow::initStyleSheet()
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QToolBar\
						   {\
						   spacing:10px;\
						   min-height:35px;\
						   background:#FAFAFA;\
						   }\
						   QToolButton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   min-height:25px;\
						   }\
						   QToolButton:checked\
						   {\
						   background: #FFFFFF;\
						   border: 1px solid #4A90E2;\
						   }\
						   QTableWidget \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 12px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QTableWidget::item\
						   {\
						   text-align:center;\
						   }\
						   QScrollBar\
						   {\
						   background:white;\
						   height:8px;\
						   }\
						   QScrollBar::handle\
						   {\
						   background:lightgray;\
						   border:2px solid transparent;\
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar:vertical\
						   {\
						   width:8px;\
						   background:white;\
						   padding-top:9px;\
						   padding-bottom:9px;\
						   }\
						   QScrollBar::handle:vertical\
						   {\
						   background:lightgray; \
						   width:8px;\
						   min-height:20;\
						   border:2px solid transparent; \
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:vertical:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QPushButton\
						   {\
						   background:#4A90E2;\
						   border-radius: 2px;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: white;\
						   min-width:75px;\
						   min-height:25px;\
						   }\
						   QPushButton:hover\
						   {\
						   background:#0C2543;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QCheckBox::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QCheckBox::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QComboBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down\
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QComboBox::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QListView,QTreeView\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QListView::item:hover,QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QListView::item:selected,QTreeView::item:selected \
						   {\
						   background-color: lightblue;\
						   }\
						   QListView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QListView::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QSpinBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QSpinBox::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QSpinBox::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QDateTimeEdit::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QDateTimeEdit::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton\
						   {\
						   border:0px solid #000000;\
						   border-radius:3px;\
						   margin:3px 3px 3px 3px;\
						   padding:3px;\
						   background-color:rgba(0,0,0,0);\
						   color:#FFFFFF;\
						   }\
						   QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolButton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed\
						   {\
						   border:1px solid #D8D8D8;\
						   }\
						   QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   margin:3px 3px 3px 3px;\
						   padding:0px 0px 0px 0px;\
						   font-family: Microsoft YaHei;\
						   font-size: 9px;\
						   background-color:rgba(0,0,0,0);\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width:70px;\
						   max-width:70px;\
						   }\
						   QCalendarWidget QToolButton::menu-indicator\
						   {\
						   image:None;\
						   }\
						   QCalendarWidget QTableView\
						   {\
						   border-width:0px;\
						   }\
						   QCalendarWidget QWidget#qt_calendar_navigationbar\
						   {\
						   border:1px solid #575757;\
						   border-width:1px 1px 0px 1px;\
						   background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #4D4D4D,stop:1 #292929);\
						   }\
						   QTabBar::tab\
						   {\
						   color:#0F1923;\
						   font-family:Microsoft YaHei;\
						   font-size: 13px;\
						   min-width:100px;\
						   min-height:30px;\
						   border:0px;\
						   }\
						   QTabBar::tab:hover\
						   {\
						   background:rgb(255,255, 255, 100);\
						   }\
						   QTabBar::tab:selected\
						   {\
						   border-color:white;\
						   background:white;\
						   color:#5D9CE5;\
						   border-bottom: 2px solid #4A90E2;\
						   }\
						   QLineEdit\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QRadioButton::indicator:unchecked\
						   {\
						   image:url(:/res/radio.png);\
						   }\
						   QRadioButton::indicator:checked\
						   {\
						   image:url(:/res/radio_checked.png);\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QGroupBox,QRadioButton,QDockWidget\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   }\
						   QMenuBar\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:14px;\
						   color:#3E464E;\
						   background: #FAFAFA;\
						   min-width:60px;\
						   min-height:32px;\
						   border:1px solid #D5D5D5;\
						   }\
						   QMenuBar::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QMenu\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:black;\
						   background-color:#FAFAFA;\
						   }\
						   QMenu::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTimeEdit::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QTimeEdit::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QHeaderView::section \
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:#0F1923;\
						   min-width:50px;\
						   height:30px;\
						   background:#F1F1F1;\
						   border: 0px solid #567dbc;\
						   border-right:1px solid #B5B5B5;\
						   border-bottom:1px solid #B5B5B5;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}