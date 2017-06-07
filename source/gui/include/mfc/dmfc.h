#ifndef _GMFC_H
#define _GMFC_H

#include <QtCore/Qt>
#include <QtCore/QtGlobal>
#include <QtCore/QRect>
#include <QtCore/QList>
#include <QtCore/QChildEvent>
#include <QtCore/QEvent>
#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QTabWidget>
#include <QtGui/QWorkspace>
#include <QtGui/QToolButton>
#include <QtGui/QMenuBar>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QToolTip>
#include <QtGui/QRubberBand>
#include <QtGui/QKeyEvent>
#include <QtGui/QPixmap>
#include <QtGui/QResizeEvent>
#include <QtGui/QMenu>
#include <QtGui/QFocusEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>

#include "mfc/ddefs.h"

class GView ;
class GFrameWnd ;
class GMDIFrameWnd ;
class GCmdUI;
class GTemplate ;
class QVBoxLayout ;
class QHBoxLayout ;
class GFile ;
class GDoc ;
class QSplashScreen;
class QImage;
class QCheckBox;
class GPatternPanel;
class GPopupMenu;

const static int MAX_TEMPLATE = 32;

class GDE_EXPORT_MFC GBlockSignals
{
	QObject* parent;

public:

	GBlockSignals(QObject* w);
	~GBlockSignals();
};

class GDE_EXPORT_MFC GTemplate
{
public:
	GDoc*  (*fnDoc)();
	GView* (*fnView)(QWidget *);
	QString docPrefix;
	QString docSuffix;
	QString docType  ;
	QPixmap docIcon  ;
};

class GAction
{
public:
	int 		m_id ;
	QString 	m_menuText ;
	QString		m_tipText  ;
	QString		m_statusText;
	QIcon	m_iconSet ;
	uint32	    m_flag	  ;
	int			m_accel	  ;
	bool		m_bEnabled;
};

class  GDE_EXPORT_MFC GSigV2I:public QObject
{
	Q_OBJECT

public:

	GSigV2I(int nSig,QObject * parent=0);

public slots:

	void slot_v();

	void destroyMe();

signals:

	void signal_i(int);

protected:

	int m_nSig;
};

class  GDE_EXPORT_MFC GIconButton:public QToolButton
{
	Q_OBJECT

public:

	GIconButton( QWidget * parent, \
			const char* name=0 );
	GIconButton( const QIcon& s, \
			const QString &textLabel,
		 	const QString & statusText,\
		 	QWidget * parent, \
		 	const char* name =0 );

	~GIconButton();

	void setIconSet( const QPixmap & s);
	void setIconSet( const QIcon & s);

	void setPushOffset(int x,int y);
	void setDropDown(bool b);

	void popup(bool b);

	void setDropDownWidget(QMenu * p,bool bHasArrow=true);
	void setFlatButton(bool b);
	void setID(int nID);
	int id(){return m_nID;};
	QSize sizeHint () const;

protected:

	void drawButton( QPainter * );
	void init();
	void mousePressEvent( QMouseEvent *e );
	void mouseReleaseEvent(QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent * e);

	void enterEvent(QEvent * e);
	void leaveEvent(QEvent * e);
	bool eventFilter( QObject *o, QEvent *e );

	QString m_statusText;
	int m_nID;
	QIcon m_icon;
	QIcon m_arrowIcon;
	int m_xPushOff;
	int m_yPushOff;

	bool m_bDropDown;
	bool m_bArrowDown;

	bool m_bPopup;
	bool m_bAutoPush;
	bool m_bOnButton;

	bool m_bDiscardMouseEvent;
	bool m_bHasArrow;

	QMenu * m_wndDropDown;

protected slots:

	void onHide();
	void onClicked();

signals:

	void setStatusText(const QString &);
	void clearStatusText();
	void onCMD(int);
};

class GDE_EXPORT_MFC GComboBox:public QComboBox
{
	Q_OBJECT

public:
	GComboBox( QWidget * parent=0,const char * name=0);

protected:

	void enterEvent(QEvent * e);

	void leaveEvent(QEvent * e);

	void paintEvent(QPaintEvent * e);

	void focusInEvent ( QFocusEvent * );

	void focusOutEvent ( QFocusEvent * );

	void mousePressEvent(QMouseEvent *);

protected:

	QPixmap m_downarrow;
};

class GDE_EXPORT_MFC GPatternButton:public GIconButton
{
	Q_OBJECT

public:
	GPatternButton( QWidget* parent , const char* name=0);

	void setPattern( int nPattern , bool bRedraw=TRUE);
	void setDefaultPattern( int nPattern );
	void setPanelPattern( int nPattern);
	int	 pattern();

protected slots:
	void onPatternChanged( int);
	void onPatternChanged();

signals:

	void selectionChanged(int cr);

protected:

	int m_nPattern;
	GPatternPanel * m_panel;
};

class GDE_EXPORT_MFC GPatternPanel:public QWidget
{
	Q_OBJECT

public:

	GPatternPanel(QWidget * parent,const char * name=0);
	~GPatternPanel();

	void setDefaultPattern(int pattern);

	void setPattern( int pattern );
// 	void drawPanel(QPainter * pDC );
	void drawPanel(QPainter * pDC /*,QMouseEvent *e*/);
	//added by chenkai 2013年2月28日 14:03:20
	QPoint termination;
	QPoint origin ;
	QRubberBand *rubberBand ;

protected:

	void mouseMoveEvent ( QMouseEvent * ) ;
	void mousePressEvent ( QMouseEvent * ) ;
	void mouseReleaseEvent ( QMouseEvent * );

// 	void paintEvent ( QPaintEvent * pe);
	void paintEvent ( QPaintEvent * pe/* , QMouseEvent * e*/);

	void leaveEvent(QEvent * e);

	int	 patternHitTest(const QPoint& pt );

	QPixmap m_pix;

	int		m_nDefaultPattern;
	int		m_nHitPattern;
	int		m_nPattern;
	int		m_nRows ;
	int		m_nCols ;
	QPixmap* m_pixPattern;

signals:

	void selectionChanged(int pattern);

};

class GDE_EXPORT_MFC GColorPanel:public QWidget
{
	Q_OBJECT

public:

	GColorPanel(QWidget * parent,const char * name=0);

	void setDefaultColor(uint32 crColor);
	void setTrans(bool b);

	void setColor(uint32 crColor);
	void drawPanel(QPainter * pDC);

protected:

	void mouseMoveEvent ( QMouseEvent * ) ;
	void mousePressEvent ( QMouseEvent * ) ;
	void mouseReleaseEvent ( QMouseEvent * );

	void paintEvent ( QPaintEvent * );
	void leaveEvent(QEvent * e);

	QRect m_rc1;
	QRect m_rc2;

	QString m_strName1;

	QString m_strName2;
	QRect m_rcColor[64];

	QPixmap m_pix;

	uint32 m_crDefaultColor;

	int m_nIndex;
	int m_nHover;
	int m_nLine;
	
	int m_nmmoveHover;
	int m_nmpressHover;

	bool m_bTrans;

	uint32 m_crColor;

protected:

	void drawColorButton(QPainter * pDC,int nIndex,HoverStatus nStatus);
	void drawHover(QPainter * pDC,int nOldHover,int nHover,int nIndex);

signals:

	void colorChanged(uint32 cr);

};

class GDE_EXPORT_MFC GColorButton:public GIconButton
{
	Q_OBJECT

public:

	GColorButton(QWidget * parent,const char * name=0);

	void setColor(uint32 cr,bool bRedraw=TRUE);

	void setIconSet( const QPixmap & s);

	void setDefaultColor(uint32 cr);

	void setPanelColor(uint32 cr);

	void setTrans(bool b);

	uint32 color();

protected:

	int iconw;
	int iconh;

protected slots:

	void onColorChanged(uint32);

	void onColorChanged();

signals:

	void colorChanged(uint32 cr);

protected:

	QPixmap createIcon(uint32 cr,const QColor & bak,int w,int h);

	uint32 m_cr;

	GColorPanel * m_panel;

	QPixmap m_pix;
};

class GDE_EXPORT_MFC GFontPanel:public QWidget
{
	Q_OBJECT

public:

	GFontPanel(QWidget * parent,const char * name);

	void setFont(const char * fontName, int nFontSize,int nFontStyle);

	void getFont(char * fontName, int nMaxLen,int & nFontSize,int & nFontStyle);

protected:

	QComboBox *		m_pFontName;

	QComboBox *		m_pFontSize;

	QToolButton *	m_pFontBold;

	QToolButton *	m_pFontItalic;

	QToolButton *	m_pFontUnderLine;

	void resizeEvent ( QResizeEvent * );

	int m_nFontSize;

public slots:

	void onFontSize(const QString & s);
	void onFontFamily(const QString & s);
};

class  GRadioData
{
public:
	int		width	;
	QPixmap pixmap	;
	QString strText ;
	QString	strTip  ;
};

class GDE_EXPORT_MFC GRadioPanel:public QWidget
{
	Q_OBJECT
public:

	GRadioPanel(QWidget * parent=0,const char * name=0);

	~GRadioPanel();

	void	setLineHeight(int nHeight);

	void	insertItem(int width ,const \
		QPixmap & pixmap=QPixmap(),\
		const QString & strText=QString::null,\
		const QString & strTip=QString::null);

	int		getHotButton(int ,int);
	int		getChecked();
	void	setChecked(int n);
	void	updateToolTip();

protected:

	void paintEvent(QPaintEvent * );
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent * );

	void drawButton(QPainter *,int x,int y,\
		GRadioData * d,\
		bool bHot,\
		bool bChecked);

	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

protected:

	class GDE_EXPORT_MFC QPtrListRADIODATA:public QList<GRadioData*>{};
	QPtrListRADIODATA m_data;

	int m_lnHeight ;
	bool m_bDown;
	int  m_nHot;
	int  m_nChecked;

signals:

	void selectionChanged(int);
};

class GDE_EXPORT_MFC GTipView:public QWidget
{
	Q_OBJECT

public:

	GTipView(QWidget * parent);

	void paintEvent(QPaintEvent * e);

	void setString(const QString & str);

protected:

	QString m_str;
};

class GDE_EXPORT_MFC GTipWnd:public QDialog
{
	Q_OBJECT

public:

	GTipWnd();

	void setStringList(const QStringList & l);

	void setStartUp(bool bStart);

	bool startUp();

protected:

	QPushButton * m_btNext;
	QPushButton * m_btClose;

	QCheckBox * m_chStart;

	GTipView * m_tipView;

	QStringList m_strList;
	QStringList::Iterator m_it;

protected slots:

	void tipNext();
};

class GDE_EXPORT_MFC GToolTip:public QToolTip
{
public:

	GToolTip(QWidget *parent);

// 	virtual void maybeTip ( const QPoint & p );
// 
// 	virtual void tip ( const QPoint& pt, const QFont& font,\
// 		const QString & text );
// 
// 	virtual void tip ( const QRect & rect, const QString & text );
// 
// 	virtual void tip ( const QRect & rect, const QString & text,\
// 		const QString & groupText );
// 
// 	virtual void tip ( const QRect & rect, const QString & text, \
// 		const QRect & geometry );
// 
// 	virtual void tip ( const QRect & rect, const QString & text, \
// 		const QString & groupText, const QRect & geometry );
// 
// 	virtual void clear ();
//
//
};


/*#define GAddDocTemplate(doc,view,prefix,suffix,type,icon)\
	{\
		class G_TEMP##doc##view\
		{\
		public:\
			static GDoc* createObject(){ return new (##doc); }\
		};\
		class G_TEMP##view##doc\
		{\
		public:\
			static GView* createObject(QWidget* w){ return new (##view)(w);}\
		};\
		GTemplate t;\
		t.fnDoc = G_TEMP##doc##view::createObject;\
		t.fnView= G_TEMP##view##doc::createObject;\
		t.docPrefix=prefix;\
		t.docSuffix=suffix;\
		t.docType  =type  ;\
		t.docIcon  =icon  ;\
		addTemplate(&t);\
	}
*/

#define MENUITEM getCmdUI()->insertItem
#define CREATEMENU_A(a) CREATEMENU(a,sizeof(a)/sizeof(a[0]))
#define ADDTOOLBAR_A(a,b) ADDTOOLBAR(a,b,sizeof(b)/sizeof(b[0]))
#define CREATEMENU(a,n) getCmdUI()->createMenu( a,n )
#define ADDTOOLBAR(a,b,n) getCmdUI()->addToolBar(a,b,n)
#define ADDTOOLWIDGET(a,w,id,pos) getCmdUI()->addToolWidget(a,w,id,pos)

class  GDE_EXPORT_MFC GCmdUI:public QObject
{
	Q_OBJECT

public:

	GCmdUI(GFrameWnd * parent);

	~GCmdUI();

	void insertItem(int id,const QString & menuText, \
			const QIcon & iconSet=QIcon(),\
			uint32 flag=0,	\
			const QString & tipText=QString::null,	\
			const QString & statusText=QString::null,	\
			int accel=0\
		);

	void insertItem(int id,const QString & menuText, \
			const QPixmap & pixmap,\
			uint32 flag=0,	\
			const QString & tipText=QString::null,	\
			const QString & statusText=QString::null,\
			int accel=0\
		);

	void createMenu(const int ids[][256],int nNum );

	void addToolBar(QToolBar * pToolBar,const int ids[],int nNum);

	void setEnabled(int id,bool bEnable);

	void setChecked(int id,bool bCheck);

	void setIconset(int id,const QPixmap & pixmap );

	void setChecked( int nParentMenuID, int nSubID, bool bCheck  );

	void chgItem ( int id, const QString & text );

	GIconButton* getItem( int id );

	bool sendKeyEvent(QKeyEvent * e);

	QMenu * getPopupMenu(int menuid);

	bool loadMenuBar(QMenuBar * pBar,int barid);

	void addToolWidget(QToolBar * pToolBar, QWidget * pW , int nID,int nIndex);

public slots:

	void onCMD(int);
	void onStatusMsg(int);
	void onUDI();

	void destroyMe();

	void clearStatusText();
	void setStatusText(const QString &);

protected:

	class GDE_EXPORT_MFC QMapINT2POPUMMENU:public QMap<int32,GPopupMenu* >{};
	class GDE_EXPORT_MFC QMapINT2ACTION:public QMap<int32,GAction* >{};
	class GDE_EXPORT_MFC QPtrListICONBUTTON:public QList<GIconButton*>{};
	class GDE_EXPORT_MFC QPtrListMENUBAR:public QList<QMenuBar*>{};
	class GDE_EXPORT_MFC QPtrListSIGV2I:public QList<GSigV2I*>{};

	QMapINT2POPUMMENU		m_mapPopupMenu;
	QMapINT2ACTION 			m_mapAction;
	QPtrListICONBUTTON		m_aButton;
	QPtrListMENUBAR			m_lstMenuBar;
	QPtrListSIGV2I			m_aSig;
	GFrameWnd *				m_pFrameWnd;
	QMap<int32,QAction*>	m_mapQAction;
};

class  GDE_EXPORT_MFC GStatusBarMan:public QObject
{
	Q_OBJECT

public:

	GStatusBarMan();
	~GStatusBarMan();

	void addStatusWidget(QWidget * w , int nWidth);

	void set(GFrameWnd * p,QStatusBar * s);

protected:

	class GDE_EXPORT_MFC QPtrListWIDGET:public QList<QWidget* >{};
	class GDE_EXPORT_MFC QPtrListINTS:public QList<int *>{};

	//todo 析构的时候要手动释放
	QPtrListWIDGET		m_wList;
	QPtrListINTS 		m_nList;

	bool eventFilter(QObject *,QEvent *);

	GFrameWnd*  m_pFrameWnd ;
	QStatusBar* m_pStatusBar;
};

class  GDE_EXPORT_MFC GDoc:public QObject
{
	Q_OBJECT
public:
	GDoc();
	virtual ~GDoc();
	bool isModified();
	void setModified( bool bModified=true );
	void setPathName( const QString& strPath );
	void setTitle( const QString& strTitle );
	const QString& getPathName() const;
	const QString& getTitle() const ;
	GView* getView() const ;
	void   setView(GView* );
public:

	virtual const QString getOwner() const;
	virtual bool readFile( GFile& fFile );
	virtual bool writeFile( GFile& fFile );
	virtual bool loadXML(const QString& strPath);
	virtual bool writeXML(const QString& strPath);
	virtual void releaseDoc(){};
	virtual bool cmdSave();
	virtual bool cmdSave( const QString& pathname, bool bOverlap=true);
	virtual bool saveModified();
	virtual bool newDoc();
	virtual bool openDoc(const QString &strPathName);
	virtual void closeDoc();
	virtual bool saveDoc( const QString& pathname );

	virtual void deleteContents(){};
	virtual bool onOpenFailed();
	virtual bool onOpenSuccess( const QString& pathname );
	virtual bool onSaved( bool bModified );
	virtual bool onSaveFailed();
	virtual bool onSaveSuccess( const QString& pathname );
	virtual void onCMD( int nID ){};
public:
	GTemplate* m_pTemplate ;

protected:
	GView 	*m_pView ;
	bool	 m_bModified ;
	QString	   m_strPathName ;
	QString    m_strTitle  ;

signals:
	void docChanged();
};

class  GDE_EXPORT_MFC QPtrListDOCS:public QList<GDoc*>{};

class  GDE_EXPORT_MFC GView:public QWidget
{
	Q_OBJECT
public:
	GView( QWidget *p=0 ,Qt::WFlags f=0);
	virtual ~GView();
	GDoc*  getDoc() const ;
	void   setDoc(GDoc* );
	virtual void onCMD( int ){}
	virtual void onUDI(GCmdUI *cmd ){}
	virtual void deleteContents(){};
	virtual void onInitialUpdate(){};

protected:
	GDoc*	m_pDoc ;
};

class  GDE_EXPORT_MFC GFrameWnd:public QMainWindow
{
	Q_OBJECT

public:

	GFrameWnd( QWidget* parent = 0,Qt::WindowFlags f = Qt::Window );	// 20130415
	virtual ~GFrameWnd();

	virtual void  onCreateView(GDoc* pDoc);
	virtual GDoc * openDoc(const QString & strPathName);
	virtual GDoc * createDoc(int nTemplate=-1);
	virtual GView * createView(QWidget * w,GDoc * pDoc);

	virtual void setTitle(GView * pView,const QString & strTitle);

	virtual void setCmdUI( QWidget * ){};

	virtual void setCaption(const QString & str);

	virtual void addTemplate(GTemplate * pTemplate);

	GView*   getView () const ;
	GDoc*	 getDoc  () const ;
	GCmdUI*  getCmdUI() const ;

	QVBoxLayout * getBoxLayout() const{ return m_layoutBox; }
	QVBoxLayout* getViewLayout() const{ return m_viewLayoutBox; }
	virtual bool mdiDoc() const {return false;};

	QSplashScreen*  getSplash() const;
	QSplashScreen*  showSplash() const;

	void  closeSplash(QWidget* qw);

	void  setSplashEnable(bool );
	bool  getSplashEnable() const ;

	void  setSplashPath(const QString& strPath );
	const QString& getSplashPath() const ;

	void  setSplashMessage( const QString& strMessage );
	const QString& getSplashMessage() const ;

	virtual void popupMenu( int menuid , const QPoint & point);

	virtual void onUDI();
	virtual void onCMD(int nID);
	virtual void loadMenuBar(int barid);

	virtual void addStatusWidget(QWidget * w,int width);

	virtual void onDestroy(){};

	virtual void onPrepareDestroy(){};

protected:

	void paintEvent( QPaintEvent* );
	void closeEvent( QCloseEvent* );

	GDoc * 			m_pDoc;
	QString 		m_topCaption;
	GTemplate 		m_template[MAX_TEMPLATE];
	int 			m_nTempNum;
	GCmdUI * 		m_cmdUI;
	QVBoxLayout * 	m_layoutBox;
	QVBoxLayout*		m_viewLayoutBox ;
	GStatusBarMan 	m_statusMan;

	bool			m_splashEnable ;
	QString			m_splashPath   ;
	QString			m_splashMessage;

public slots:

	void fileNew();
	QString fileOpen();
	void fileSave();
	void fileSaveAs();
	void fileExit();
	
	/**
	@brief 载入进度
	@param  
	@return void 
	@author libinghong
	@date 2016年11月22日 
	*/	
	void progressChanged(int);

	void setSplashProgress(int value); //外部改变进度
};

class GMdiSubWindow:public QMdiSubWindow
{
	Q_OBJECT

public:
	GMdiSubWindow(QWidget *parent = 0);

	void setCanClose(bool canclose=true);
	bool canClose() ;

	GDoc* getDoc(){return m_pDoc;}
	GView* getView(){return m_pView;}

public:
	GDoc *m_pDoc;
	GView *m_pView;

protected:
	bool m_bCanClose;
	QMenu* m_pRightClickMemu;

protected:
	void createMenu(void);
	void closeEvent(QCloseEvent *closeEvent);
	void mouseReleaseEvent(QMouseEvent * mouseEvent);

protected slots:
	void saveDoc(void);
	void closeOther(void);


signals:
	void mdiChildClose(QMdiSubWindow *);
	void mdiChildDestroy();
};

class  GDE_EXPORT_MFC GMDIFrameWnd:public GFrameWnd
{
	Q_OBJECT

public:

	GMDIFrameWnd( QWidget* parent = 0, \
		const char* name = 0, Qt::WindowFlags f = Qt::Window );	// 20130415

	void   activeView(const QString& strPathName);
	void   activeView(GView* pView);
	void   activeView(int32 nPos  );
	void   activeDoc(const QString& strPathName);
	void   activeDoc(GDoc* pDoc);
	void   activeDoc(int nPos);

	GDoc*  findDoc(const QString& strPathName);
	GDoc*  findDoc(int nPos);
	GView* findView(const QString& strPathName);
	GView* findView(int nPos);

	GView* getActiveView() const;
	GDoc*  getActiveDoc()  const;

	void MDITileV();
	void MDITileH();
	void MDICascade();

	QPtrListDOCS* getDocList() const { return (QPtrListDOCS*)&m_docList; };

	GDoc * openDoc(const QString & strPathName);

	virtual bool mdiDoc() const {return true;};

	virtual void setTitle(GView * pView,const QString & strTitle);

	virtual bool saveAllModified();

	virtual void removeDoc(GDoc * );

	int	  isEnableTabBar();

	void  enableTabBar(int enable=0);

	void closeBut(QMdiSubWindow* subWindow);

protected:

	QMdiArea *m_mdiArea;
	QMdiSubWindow *m_pLastActiveWindow;

	QWidget		* m_vbox;		// 201304
	QVBoxLayout * m_vb;
	QHBoxLayout * m_hb;

	QPtrListDOCS m_docList;

	int m_nUntitledCount;

	int		 m_enableTabBar;

	void closeEvent( QCloseEvent* );

	virtual void mdiChildClose(QMdiSubWindow*);
	virtual void mdiChildDestroy();
	virtual void mdiChildActiveWindow(QMdiSubWindow*);
public slots:

	void onChildDestroy();
	void onChildClose(QMdiSubWindow*);
	void onActiveWindow(QMdiSubWindow *);
	void fileSave();

	void fileSaveAs();

	void fileSaveAll();

	void fileClose();

	bool fileCloseAll();

	void fileExit();
};

class GPopupMenu:public QMenu		//modify by lbh
{
public:

	GPopupMenu( QWidget * parent = 0, const char * name = 0 );

protected:

	void paintEvent(QPaintEvent *);

	void mouseMoveEvent(QMouseEvent *);
};



extern QBitmap GBitmapConvert( const QPixmap & m );		

extern void qDrawWinShades( QPainter *p,int x, int y, int w, int h,	const QColor &c1, const QColor &c2,	const QColor &c3, const QColor &c4,	const QBrush *fill );		//modify by lbh

extern void qDraw3DRect( QPainter *p,int x, int y, int w, int h,const QColor &c1, const QColor &c2,	const QBrush * fill);		//modify by lbh


#endif
