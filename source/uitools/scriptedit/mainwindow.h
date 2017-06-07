#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPrinter>
#include <QMainWindow>
#include "functionwidget.h"

#include "upexec.h"
//QT_BEGIN_NAMESPACE
class QString;
class UserScriptEdit;
class QAction;
class QDockWidget;
class QTreeWidgetItem;
class QTextEdit;
class QTimerEvent;
//QT_END_NAMESPACE

class FunctionWidget;
class RdbParaDialog;

#define MAINTIMERINTERVAL 1000
      
#define MAINWINDOWTITLE   tr("用户过程编辑生成工具")


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
	void init(bool factorySettings = false);
	void createActions(QWidget* parent);
	void initMainMenu(QWidget * parent);
	void initToolBars(QWidget * parent);
	void initStatusBars(QWidget * parent);
	void initCentralWidget();
	void initializeRecentActions(QWidget* parent);
	void CreateDockWidget(QMainWindow* mainWin);
	//继承
	virtual void closeEvent(QCloseEvent* event);
	void timerEvent(QTimerEvent *event);
private slots:
	void newFile();
	void open();
	void openRecentFile();
	bool save();
	bool saveAs();
	bool saveAll();
	void setPrintPreviewOptions(QPrinter* printer);
	void print(QPrinter* printer=NULL);
	void printPreview();
	void exit();

	void undo();
	void redo();
	void cut();
	void copy();
	void paste();

	void insertFunc();
	void insertSCDFunc();
	void insertCustomFunc();  //open edit custom function dialog be edit;
	void deleteCustomFunc();
	void editCustomFunc();

	void viewToolBar(bool checked);
	void viewStatusBar(bool checked);
	void viewOutput(bool checked);
	void compile();
	void execProc();
	void about();
	
private:
	QString					m_curFile;
	bool maybeSave();
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void readSettings();
    void writeSettings();

	void		setCurrentFile(const QString &fileName);
	void		updateRecentFileActions();
	QString		strippedName(const QString &fullFileName);
	MainWindow *findMainWindow(const QString &fileName);

	//////////////////////////////////////////////////////////////////////////
	uint proc_formula_exec(QString &execfile);
	void proc_hand_exec(const QString &execfile);
	void proc_hand_stop();
	int	 ExecOneProc(int groupno,USERPROCPARA_STRU *calp);
	int  IsMainServer(int groupno);
	void initStyleSheet() ;
	//////////////////////////////////////////////////////////////////////////
public:
	static      QString     getEnvVar();
	static MainWindow*   mainWindow();	
	void        insertText(const QString& text);
	void        appendOutput(QString outText,bool additionDate=true);
private:
	QAction *actionNewFile,*actionOpenFile,*actionSave,*actionSaveAs/*,*actionSaveAll*/,*actionPrint,*actionPrintpreview,*actionExitApp;
	QAction *actionUndo,*actionRedo,*actionCopy,*actionPaste,*actionCut;
	QAction *actionToolBar,*actionStatusBar;
	QAction *actionInsertFunc,*actionInsertSCDFunc,*actionViewOutput;
	
	QAction *actionCompile,*actionExecproc;
	QAction *actionAbout;

	
	QMenuBar *menubar;	
	QMenu *fileMenu;
    QMenu *editMenu;
	QMenu *viewMenu;
    QMenu *funcMenu;
    
	QMenu *compileMenu;
    QMenu *helpMenu;

	QToolBar *fileTools,*editTools,/**viewTools,*/*funcTools,*compileTools;
	QStatusBar *statusbar;
	
	QDockWidget		*dockFunctionWidget,*dockInfoTip;
	UserScriptEdit  *m_editor;
	FunctionWidget  *funcWidget;
	QTextEdit       *compileOut;
	
	//recentFile
	enum			{ MaxRecentFiles = 5 };
	QAction*		m_recentFileActs[MaxRecentFiles];
	QAction*		m_separatorAct;
	
	int             mainTImerID;
	
	RdbParaDialog   *rdbParaDlg;
	
	int             removeUserProcDelayID;
	QStringList     m_existUserProcRec;
	int             m_insRecSerial;
#ifdef DBG_REGPROC
	NetDbg       netDbg;
	ProcMan		 procMan;
#define			 PROCINTERVAL 1000
	int          procimerID;
#endif
	

	//printer
	QPrinter::PageSize		print_paper_size;
	QPrinter::Orientation	printer_orientation;

	//xml ini 
	//void initFuncClassesFromXml();
};

#endif 
