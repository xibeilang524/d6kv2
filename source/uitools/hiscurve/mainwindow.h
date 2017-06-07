#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>

#include "appdef.h"


#ifdef DBG_REGPROC
#include "net/netapi.h"
#endif

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QDockWidget;
class QVBoxLayout;
class QStackedLayout;
class QItemDelegate;
class QLabel;
class QUndoStack;
QT_END_NAMESPACE

class selectorpara;
class SelectorWidget;
class DataSelectorToolBar;
class CurveSelectorToolBar;

class HisDataModel;
class DataCurveView;
class DataTableView;
class VariantCurveForm;
class TrendCurveForm;

#define CURVEVIEW_LAYEROUT_INDEX 0
#define CURVETABLE_LAYEROUT_INDEX 1
#define VARIANTCURVEFORM_LAYEROUT_INDEX 2
#define TRENDCURVEFORM_LAYEROUT_INDEX 3


class MainWindow : public QMainWindow
{
    Q_OBJECT
		
public:
    MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();
public:
	QUndoStack    *undoStack() {return this->_undoStack;}
	DataCurveView *dataCurveView(){return this->curveView;}
	DataTableView *dataTableView(){return this->tableView;}
	HisDataModel  *hisDataModel(){return this->curveModel;}
	void setTreeItemSelect(QString ycCode,QString date);

private slots:
	void setPrintPreviewOptions(QPrinter* printer);
	void printpreview();
	void print();
	
#if defined(EXCELFORMAT)	
	void exportXls();
#endif
	void slotExportCsv();//导出到CSV文件

	void exportImage();
	void exit();
	
	void copy();
	void paste();
	void randomPaste(bool checked);
	void undo();
	void redo();

	void curveview(bool checked);
	void dataview(bool checked);
	void showSDate(bool checked);

	void execstatic();
	void updatedb();
	void about();

	void switchview(int index);
	void curveModelReset(); //curve model to be reset;
	void selectorWidgetChanged(selectorpara & selpara);   //proc main ui update
	void selectorChanged(selectorpara& selpara);
private:
	void init(bool factorySettings = false);
	void createDockWindows(QWidget* parent);
	void createActions(QWidget* parent);
	void initMainMenu(QWidget * parent);
	void initToolBars(QWidget * parent);
	void initStatusBars(QWidget * parent);
	void initCentralWidget();

	void initView();
	void initStyleSheet();

private:
	int   randomPercent;

	QWidget *centralwidget;

    QAction *actionPrintpreview,*actionPrint,*actionExportImage,*actionExit;
#if defined(EXCELFORMAT)
	QAction *actionExportXls;
#endif
	QAction *m_actExportCsv;//导出到csv文件

#define MAININTERVAL  500
int     mainTimerID;
#ifdef DBG_REGPROC
	NetDbg       netDbg;
	ProcMan		 procMan;
#define			 PROCINTERVAL 1000
	int          procTimerID;
#endif
	QAction *actionUndo,*actionRedo,*actionCopy,*actionPaste,*actionRandonPaste;
    QAction *actionCurveView,*actionDataView,*actionViewSDate;
	QAction *actionExecProc,*actionUpdateDb;
	//QAction *actionYAutoScale;//?
    QAction *actionAbout;
	
	QMenuBar *menubar;	
	QMenu *fileMenu;
    QMenu *actionExport;
    QMenu *editMenu;
    QMenu *viewMenu;
	//QMenu *sysToolMenu; 
    QMenu *helpMenu;
	QLabel     *userStyleLbl,*userDescLbl,*datetimeLbl;
	QStatusBar *statusbar;
    QToolBar *fileTools,*viewTools,*tableTools;
	DataSelectorToolBar  *dataSelectorTools;
	CurveSelectorToolBar *curveSelectorTools;

	QDockWidget *dockWidget;
	SelectorWidget *selectorWidget;
	QStackedLayout *viewLayerout;
	
	HisDataModel   *curveModel;
	QItemDelegate  *delegate;
	DataCurveView  *curveView;
	DataTableView  *tableView;
	VariantCurveForm	    *varCurveForm;
	TrendCurveForm	    *trendCurveForm;

	QPrinter::PageSize		print_paper_size;
	QPrinter::Orientation	printer_orientation;

    //redo undo frame
	QUndoStack *_undoStack;
protected:
	virtual void timerEvent(QTimerEvent *event);
	virtual void closeEvent(QCloseEvent *event);
};

#endif