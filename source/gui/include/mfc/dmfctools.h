#ifndef _MFC_TOOLS_H
#define _MFC_TOOLS_H

#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QStyle>
#include <QtCore/QMutex>
#include <QtGui/QFrame>
#include <QtCore/QObject>
#include <QtGui/QCursor>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtGui/QSpinBox>
#include <QtCore/QtGlobal>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QClipboard>
#include <QtGui/QValidator>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>
#include <QtGui/QListWidget>
#include <QtGui/QColorGroup>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QScrollArea>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QApplication>
#include <QtGui/QColorDialog>
#include <QtGui/QResizeEvent>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QListWidgetItem>

#include "ddefs.h"
#include "dconfig/GProfile.h"
#include "ddes/GLibManager.h"
#include "ddes/GXYCoorTrans.h"
#include "ddes/gcommobjs.h"
#include "ddes/GBaseObject.h"
#include "ddes/GComObject.h"
#include "ddes/GLineObject.h"
#include "ddes/GRectObject.h"
#include "ddes/GEllipseObject.h"
#include "ddes/GCircleObject.h"
#include "ddes/GRoundRectObject.h"
#include "ddes/GPolylineObject.h"
#include "ddes/GPolygonObject.h"
#include "ddes/GButtonObject.h"
#include "ddes/GPointArcObject.h"
#include "ddes/GPinObject.h"
#include "ddes/GBusObject.h"
#include "ddes/GLinkObject.h"
#include "ddes/GFlowObject.h"
#include "ddes/GTextObject.h"
#include "ddes/GDynTextObject.h"
#include "ddes/GImageObject.h"
#include "ddes/GCArcObject.h"
#include "ddes/GGraphPlugin.h"
#include "ddes/GPathObject.h"
#include "ddes/GGraphFile.h"
#include "ddes/GCustomObject.h"
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddef/GList.h"
#include "ddes/gcommobjs.h"

class QRadioButton;
class QCheckBox;
class QPushButton;
class QHBoxLayout;
class QFrame;
class QSpinBox;
class QComboBox;
class QLabel;
class QMutex;
class QWidget;
class QMainWindow;

class GBaseObject;
class GComObject;
class GCustomObject;
class GLibManager;
class GRect;
class GFillStyle;
class GLineStyle;
class GTextStyle;
class GVersion;
class GRange;

GDE_EXPORT_MFCTOOLS uint8 QEqualFilePath( const QString& strDst ,const QString& strSrc ,uint8 casesense = 0 );
GDE_EXPORT_MFCTOOLS void  QBeginWaitCursor( );
GDE_EXPORT_MFCTOOLS void  QEndWaitCursor( );

GDE_EXPORT_MFCTOOLS void QEnterCriticalSection(  );
GDE_EXPORT_MFCTOOLS void QLeaveCriticalSection(  );

GDE_EXPORT_MFCTOOLS void QEnterCriticalSection( QMutex& mutex );
GDE_EXPORT_MFCTOOLS void QLeaveCriticalSection( QMutex& mutex );

GDE_EXPORT_MFCTOOLS uint8 IsSHIFTpressed( uint32 flags );
GDE_EXPORT_MFCTOOLS uint8 IsCTRLpressed( uint32 flags );

GDE_EXPORT_MFCTOOLS int32	QGetArgc( );
GDE_EXPORT_MFCTOOLS char**	QGetArgv( );
GDE_EXPORT_MFCTOOLS QWidget* QGetMainWnd();
GDE_EXPORT_MFCTOOLS QApplication* QGetApp( );
GDE_EXPORT_MFCTOOLS QString QGetAppName( );
GDE_EXPORT_MFCTOOLS void QAppSetting( );

GDE_EXPORT_MFCTOOLS int QDesktopWidth( );
GDE_EXPORT_MFCTOOLS int QDesktopHeight( );

GDE_EXPORT_MFCTOOLS void  mutexButton(  QRadioButton* rbtna[]  ,int32 tbtns  ,QRadioButton* cur  );
GDE_EXPORT_MFCTOOLS void  mutexButton( QCheckBox* rbtna[]  ,int32 tbtns  ,QCheckBox* cur );

GDE_EXPORT_MFCTOOLS uint32 getButtonColor(  QPushButton* btn );
GDE_EXPORT_MFCTOOLS void   setButtonColor(  QPushButton* btn  , uint32 clr );

GDE_EXPORT_MFCTOOLS QString QChoosePixmapOpenPath( QWidget *parent );
GDE_EXPORT_MFCTOOLS QString QChoosePixmapSavePath( QWidget *parent );

GDE_EXPORT_MFCTOOLS int	GMessageBox( QWidget * hWnd ,const QString & lpszText , const QString & lpszCaption =  "" , uint32 nType = MB_OK );

GDE_EXPORT_MFCTOOLS int GMessageBox( const QString & lpszText , uint32 nType = MB_OK );

GDE_EXPORT_MFCTOOLS QTreeWidgetItem* lastItem( QTreeWidget *parent );
GDE_EXPORT_MFCTOOLS QTreeWidgetItem* lastItem( QTreeWidgetItem *parent );
// GDE_EXPORT_MFCTOOLS Q3ListViewItem*	lastItem(Q3ListView *parent);
// GDE_EXPORT_MFCTOOLS Q3ListViewItem*	lastItem(Q3ListViewItem *parent);

GDE_EXPORT_MFCTOOLS void  drawObjPixmap( QPixmap& pixmap  , GBaseObject* pObj );

class GDE_EXPORT_MFCTOOLS GCustomViewItem : public QTreeWidgetItem
{
public:

	GCustomViewItem( int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( const QStringList & strings ,int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( QTreeWidget * parent, int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( QTreeWidget * parent, const QStringList & strings, int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( QTreeWidgetItem * parent, int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( QTreeWidgetItem * parent, const QStringList & strings, int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomViewItem( const QTreeWidgetItem & other ,GCustomObject *pObj = NULL ,GComObject *pSymboObj = NULL );
	GCustomObject* getObj() const ;

	void setObj(  GCustomObject* pObj  );
	GComObject*    getSymboObj() const ;
	void setSymboObj( GComObject* pObj );

private:

	GCustomObject* m_pObj ;
	GComObject*	   m_pSymboObj;
};

class GDE_EXPORT_MFCTOOLS GPluginViewItem : public QTreeWidgetItem
{
public:

	GPluginViewItem( int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( const QStringList & strings ,int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( QTreeWidget * parent, int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( QTreeWidget * parent, const QStringList & strings, int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( QTreeWidgetItem * parent, int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( QTreeWidgetItem * parent, const QStringList & strings, int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type ,const QString& pluginSrc = QString::null );
	GPluginViewItem( const QTreeWidgetItem & other ,const QString& pluginSrc = QString::null );

	const QString& getPluginSrc(  ) const;
	void  setPluginSrc(  const QString& pluginSrc  );

private:

	QString m_impSrc  ;
};

class GDE_EXPORT_MFCTOOLS GSymboViewItem:public QTreeWidgetItem
{
public:

	GSymboViewItem( int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( const QStringList & strings ,int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( QTreeWidget * parent, int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( QTreeWidget * parent, const QStringList & strings, int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( QTreeWidgetItem * parent, int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( QTreeWidgetItem * parent, const QStringList & strings, int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type ,GComObject* pObj = NULL );
	GSymboViewItem( const QTreeWidgetItem & other ,GComObject* pObj = NULL );

	GComObject* getObj(  ) const ;
	void setObj(  GComObject* pObj  );

private:

	GComObject* m_pObj ;
};

class GDE_EXPORT_MFCTOOLS GElementListViewItem:public QTreeWidgetItem
{
public:
	GElementListViewItem( int type = Type ,GBaseObject* pObj = NULL );
	GElementListViewItem( const QStringList & strings ,int type = Type ,GBaseObject* pObj = NULL );
	GElementListViewItem( QTreeWidget * parent, int type = Type ,GBaseObject* pObj = NULL );
	GElementListViewItem( QTreeWidget * parent, const QStringList & strings, int type = Type ,GBaseObject* pObj = NULL  );
	GElementListViewItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type ,GBaseObject* pObj = NULL );
	GElementListViewItem( QTreeWidgetItem * parent, int type = Type ,GBaseObject* pObj = NULL  );
	GElementListViewItem( QTreeWidgetItem * parent, const QStringList & strings, int type = Type ,GBaseObject* pObj = NULL  );
	GElementListViewItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type ,GBaseObject* pObj = NULL );
	GElementListViewItem( const QTreeWidgetItem & other ,GBaseObject* pObj = NULL );

	GBaseObject* getObj(  ) const ;
	void setObj(  GBaseObject* pObj  );

	uint32 getTextColor(  ) const;
	void setTextColor(int column, uint32 clrText );
	//Qt4中不需要
	//virtual void paintCell(int column );

private:
	GBaseObject* m_pObj ;	
	uint32		 m_clrText;
};

class GDE_EXPORT_MFCTOOLS GOutputListView:public QTreeWidget
{
	Q_OBJECT

public:

	GOutputListView( QWidget* parent = 0 );
	~GOutputListView();

protected slots:

	void slotRightPressed( const QPoint& );

	void slotCopy();
	void slotCut();
	void slotRemove();
};

class GDE_EXPORT_MFCTOOLS GCustomIconView:public QListWidget
{
	Q_OBJECT

public:

	GCustomIconView(  QWidget* parent = 0 , const char* name = 0 );
	~GCustomIconView(  );

public:

	enum SizeMode { Large ,Small };

	void setEnabledModified( uint8 bModified = 1 );
	uint8 getEnabledModified(  ) const;

	void setSizeMode(  SizeMode m  );
	SizeMode getSizeMode(  ) const;

	void setSymboManage(  GLibManager* pList  );
	GLibManager* getSymboManage(  ) const  ;

	QString getPath(  ) const ;
	void setPath(  const QString& libPath );

	GPOS firstPos(  ) const;
	GComObject* nextPos( GPOS& gPos ) const;
	GPOS lastPos(  ) const;
	GComObject* prevPos( GPOS& gPos ) const;

	void  removeAt( GComObject* pObj );

	void  freshItem(  );
	void  freshLabel(  );

signals:
	void deleteItem(  );
	void renameItem(  );
	void editItemAttribute( GLibManager*lib ,GBaseObject* pObj );

protected:

	void clearData(  );

	QString				m_libPath ;
	GLibManager*		m_symboLib;
	SizeMode			m_sizeMode;
	uint8				m_bModified;
	QListWidgetItem*	m_curEditItem;

	protected slots:
		void viewLarge(  );
		void viewSmall(  );
		void viewBottom(  );
		void viewRight(  );
		void flowEast(  );
		void flowSouth(  );
		void itemTextTruncate(  );
		void itemTextWordWrap(  );
		void sortAscending(  );
		void sortDescending(  );
 		void doubleClicked(  QListWidgetItem *  );
// 		void slotRightPressed(  QListWidgetItem *item  );
		void slotRightPressed(  const QPoint&  );
		void slotItemRenamed (  QListWidgetItem * );
		void arrangeItemsInGrid(  ) ;
};

class GDE_EXPORT_MFCTOOLS GCustomIconViewItem:public QListWidgetItem
{
public:

	GCustomIconViewItem(  QListWidget *parent ,GComObject* pObj );
	virtual ~GCustomIconViewItem(  );

public:

	virtual QPixmap *pixmap(  ) const;
	QString objectName(  ) const;
	void	setObjectName( const QString& str ) ;

	void    viewModeChanged( GCustomIconView::SizeMode vm );
	GComObject* getObj(  ) const;

protected:
	GComObject*	m_pObj;
	QPixmap	m_largePixmap ,m_smallPixmap;

	GCustomIconView::SizeMode m_sizeMode;
};

class GDE_EXPORT_MFCTOOLS GBaseIconView : public QListWidget
{
public:
	GBaseIconView(  QWidget* parent = 0 );
	void addViewItem( uint32 uID ,const QIcon& icon ,const QString& txt );
};

class GDE_EXPORT_MFCTOOLS GBaseIconViewItem:public QListWidgetItem
{
public:
// 	GBaseIconViewItem( GBaseIconView* parent ,uint32 uID ,const QPixmap& pix ,const QString& txt );
	GBaseIconViewItem( const QIcon & icon, const QString & txt, GBaseIconView * parent , uint32 uID ,int type = Type );

public:
	virtual QIcon* icon() const;
	void	setIcon( const QIcon& pixmap );

	uint32  getID() const;

protected:
	uint32 m_nID;
	QIcon  m_icon;
};

class GDE_EXPORT_MFCTOOLS GPropertyList : public QTreeWidget
{
public:

	GPropertyList( QWidget* parent = 0 );
	~GPropertyList(  );

	void setObjs(  GObjectManage* objs  );

public:

	void addObject( GBaseObject *pObj );
	void addObject( QTreeWidgetItem*  ,GBaseObject *pObj );
	void addBaseInfo( QTreeWidgetItem*  ,GBaseObject *pObj );
	void addComInfo( QTreeWidgetItem*  ,GBaseObject *pObj );
	void addSpecInfo( QTreeWidgetItem*  ,GBaseObject *pObj );
	void addDataInfo( QTreeWidgetItem*  ,GBaseObject *pObj );

	void addItem( QTreeWidgetItem* ,const char* ,const GListPOINTS& ,const GListUINT8& );
	void addItem( QTreeWidgetItem* ,const char* ,const GListPOINTS& );
	void addItem( QTreeWidgetItem* ,const char* ,const char*  );
	void addItem( QTreeWidgetItem* ,const char* ,uint32 ,uint8  = 0 );
	void addItem( QTreeWidgetItem* ,const char* ,FLOAT8 );
	void addItem( QTreeWidgetItem* ,const char* ,const GPoint&  );
	void addItem( QTreeWidgetItem* ,const char* ,const GRect&  );
	void addItem( QTreeWidgetItem* ,const char* ,const GFillStyle&  );
	void addItem( QTreeWidgetItem* ,const char* ,const GLineStyle&  );
	void addItem( QTreeWidgetItem* ,const char* ,const GTextStyle&  );
	void addItem( QTreeWidgetItem* ,const char* ,const GVersion&  );
	void addItem( QTreeWidgetItem* ,const char* ,const GRange& );

	void exec(  );

protected:

	GBaseObject* m_pObj;
	QStringList GProperyStrList;
};

class GDE_EXPORT_MFCTOOLS GPropertyItem:public QTreeWidgetItem
{
public:

// 	GPropertyItem(  QTreeWidget* parent ,int32 h = 0 );
// 	GPropertyItem(  QTreeWidget* parent ,int32 h = 0 );
// 	GPropertyItem(  QTreeWidgetItem* parent ,int32 h = 0 );
	GPropertyItem( int type = Type ,int32 h = 0 );
	GPropertyItem( const QStringList & strings ,int type = Type ,int32 h = 0 );
	GPropertyItem( QTreeWidget * parent, int type = Type ,int32 h = 0 );
	GPropertyItem( QTreeWidget * parent, const QStringList & strings, int type = Type ,int32 h = 0 );
	GPropertyItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type ,int32 h = 0 );
	GPropertyItem( QTreeWidgetItem * parent, int type = Type ,int32 h = 0 );
	GPropertyItem( QTreeWidgetItem * parent, const QStringList & strings, int type = Type ,int32 h = 0 );
	GPropertyItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type ,int32 h = 0 );
	GPropertyItem( const QTreeWidgetItem & other ,int32 h = 0 );

	virtual ~GPropertyItem(  );

	void paintCell(  QPainter * , const QPalette & cg ,int column , int width , int alignment  );
	void paintBranches(  QPainter * p , const QPalette & cg ,int w , int y , int h  );
    void paintFocus(  QPainter *p , const QPalette &cg , const QRect &r  );
	void  placeEditor( QWidget* w );
	uint8  isModified(  ) const{ return m_bModified;	}
	void   setModified(  uint8 bModified = 1 ){ m_bModified  =  bModified; }

	virtual bool hasCustomContents(  ) const;
    virtual void drawCustomContents(  QPainter *p , const QRect &r  );

	void setup(  );

	virtual void  showEditor(  );
	virtual void  hideEditor(  );

	void setData( void* data ){ m_data  =  data; }
	void* data(  ) const { return m_data;	}

	virtual int rtti(  ) const{ return 100;	}

protected:

	uint8	m_bModified;
	int32   m_height;
	void*	m_data ;
};
 
class GDE_EXPORT_MFCTOOLS GPropertyComboItem : public QObject ,public GPropertyItem
{
    Q_OBJECT

public:
	GPropertyComboItem( int type = Type ,int32 h = 0 );
	GPropertyComboItem( const QStringList & strings ,int type = Type ,int32 h = 0 );
	GPropertyComboItem( QTreeWidget * parent, int type = Type ,int32 h = 0 );
	GPropertyComboItem( QTreeWidget * parent, const QStringList & strings, int type = Type ,int32 h = 0 );
	GPropertyComboItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int type = Type ,int32 h = 0 );
	GPropertyComboItem( QTreeWidgetItem * parent, int type = Type ,int32 h = 0 );
	GPropertyComboItem( QTreeWidgetItem * parent, const QStringList & strings, int type = Type ,int32 h = 0 );
	GPropertyComboItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int type = Type ,int32 h = 0 );
	GPropertyComboItem( const QTreeWidgetItem & other ,int32 h = 0 );
    ~GPropertyComboItem(  );

    virtual void showEditor(  );
    virtual void hideEditor(  );

	void	setStringList( const QStringList& strs  );
    void	setValue(  int32 v  );
	void	setValue(  const QString& v  );

	int32	value(  ) const { return m_value; }

signals:
	void valueChanged( QTreeWidgetItem*  );

private slots:
    void editValue(  );

private:

	void createChildren(  );

    QComboBox *	combo;
	int32		m_value;
};

class GDE_EXPORT_MFCTOOLS GPropertyBoolItem : public QObject ,
	public GPropertyItem
{
	Q_OBJECT

public:
	//  GPropertyBoolItem( Q3ListView* parent ,uint8 v ,int32 h = 0  );
	// 	GPropertyBoolItem( Q3ListView* parent ,Q3ListViewItem* after ,uint8 v ,int32 h = 0  );
	// 	GPropertyBoolItem( Q3ListViewItem* parent ,Q3ListViewItem* after ,uint8 v ,int32 h = 0  );

	GPropertyBoolItem(uint8 v, int type = Type  ,int32 h = 0 );
	GPropertyBoolItem( const QStringList & strings ,uint8 v  ,int type = Type,int32 h = 0 );
	GPropertyBoolItem( QTreeWidget * parent ,uint8 v, int type = Type ,int32 h = 0 );
	GPropertyBoolItem( QTreeWidget * parent, const QStringList & strings ,uint8 v , int type = Type  ,int32 h = 0 );
	GPropertyBoolItem( QTreeWidget * parent, QTreeWidgetItem * preceding ,uint8 v, int type = Type  ,int32 h = 0 );
	GPropertyBoolItem( QTreeWidgetItem * parent,uint8 v , int type = Type ,int32 h = 0 );
	GPropertyBoolItem( QTreeWidgetItem * parent, const QStringList & strings ,uint8 v , int type = Type ,int32 h = 0 );
	GPropertyBoolItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding ,uint8 v , int type = Type,int32 h = 0 );
	GPropertyBoolItem( const QTreeWidgetItem & other ,uint8 v ,int32 h = 0 );


	~GPropertyBoolItem(  );

	virtual void showEditor(  );
	virtual void hideEditor(  );

	void setValue(  uint8 v  );
	uint8 value(  ) const{ return m_value; }

signals:
	void valueChanged( QTreeWidgetItem*  );

	private slots:
		void editValue(  );

private:

	void createChildren(  );

	QComboBox *	combo;
	uint8		m_value;
};

class GDE_EXPORT_MFCTOOLS GPropertyIntItem : public QObject ,public GPropertyItem
{
    Q_OBJECT

public:
	GPropertyIntItem( int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( const QStringList & strings ,int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( QTreeWidget * parent, int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( QTreeWidget * parent, const QStringList & strings, int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( QTreeWidget * parent, QTreeWidgetItem * preceding, int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( QTreeWidgetItem * parent, int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( QTreeWidgetItem * parent, const QStringList & strings, int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, int32 v ,int type = Type ,uint8 sign = 1 ,int32 h = 0 );
	GPropertyIntItem( const QTreeWidgetItem & other ,int32 v ,uint8 sign = 1 ,int32 h = 0 );
    ~GPropertyIntItem(  );

    virtual void showEditor(  );
    virtual void hideEditor(  );

    void setValue(  int32 v  );

	int32 value(  ) const{ return m_value; }
	uint8 sign(  ) const { return m_signed;}

signals:
	void valueChanged( QTreeWidgetItem*  );

private slots:
    void editValue(  );

private:

	void createChildren(  );

    QSpinBox*	spinBox;
    uint8	    m_signed;
	int32		m_value;

};

class GDE_EXPORT_MFCTOOLS GPropertyDoubleItem:public QObject ,public GPropertyItem
{
	Q_OBJECT

public:
	GPropertyDoubleItem(uint8 v, int type = Type  ,int32 h = 0 );
	GPropertyDoubleItem( const QStringList & strings ,uint8 v  ,int type = Type,int32 h = 0 );
	GPropertyDoubleItem( QTreeWidget * parent ,uint8 v, int type = Type ,int32 h = 0 );
	GPropertyDoubleItem( QTreeWidget * parent, const QStringList & strings ,uint8 v , int type = Type  ,int32 h = 0 );
	GPropertyDoubleItem( QTreeWidget * parent, QTreeWidgetItem * preceding ,uint8 v, int type = Type  ,int32 h = 0 );
	GPropertyDoubleItem( QTreeWidgetItem * parent,uint8 v , int type = Type ,int32 h = 0 );
	GPropertyDoubleItem( QTreeWidgetItem * parent, const QStringList & strings ,uint8 v , int type = Type ,int32 h = 0 );
	GPropertyDoubleItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding ,uint8 v , int type = Type,int32 h = 0 );
	GPropertyDoubleItem( const QTreeWidgetItem & other ,uint8 v ,int32 h = 0 );


	~GPropertyDoubleItem(  );

	virtual void showEditor(  );
	virtual void hideEditor(  );

	void   setValue(  FLOAT8 v  );
	FLOAT8 value(  ) const{ return m_value; }

signals:
	void valueChanged( QTreeWidgetItem*  );

	private slots:
		void editValue(  );

private:

	void createChildren(  );

	QLineEdit *	line;
	FLOAT8		m_value;
}; 

class GDE_EXPORT_MFCTOOLS GPropertyTextItem : public QObject ,public GPropertyItem
{
    Q_OBJECT

public:
	GPropertyTextItem( const QString &v , int type = Type , int32 h = 0 );
	GPropertyTextItem( const QStringList & strings ,const QString &v , int type = Type , int32 h = 0 );
	GPropertyTextItem( QTreeWidget * parent, const QString &v, int type = Type , int32 h = 0 );
	GPropertyTextItem( QTreeWidget * parent, const QStringList & strings, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyTextItem( QTreeWidget * parent, QTreeWidgetItem * preceding, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyTextItem( QTreeWidgetItem * parent, const QString &v, int type = Type , int32 h = 0 );
	GPropertyTextItem( QTreeWidgetItem * parent, const QStringList & strings, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyTextItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyTextItem( const QTreeWidgetItem & other ,const QString &v, int32 h = 0 );
	
    ~GPropertyTextItem();

    virtual void showEditor();
    virtual void hideEditor();

    void setValue( const QString &v );
    QString value() const { return m_value; }

signals:
	void valueChanged( QTreeWidgetItem* );
private slots:

    void editValue();

private:

	void createChildren();

    QLineEdit *line;
	QString	   m_value;

};
 
class GDE_EXPORT_MFCTOOLS GPropertyColorItem:public QObject ,
	public GPropertyItem
{
	Q_OBJECT

public:
	GPropertyColorItem( uint32 key ,uint32 v, int type = Type  ,int32 h = 0 );
	GPropertyColorItem( const QStringList & strings , uint32 key ,uint32 v  ,int type = Type,int32 h = 0 );
	GPropertyColorItem( QTreeWidget * parent , uint32 key ,uint32 v, int type = Type ,int32 h = 0 );
	GPropertyColorItem( QTreeWidget * parent, const QStringList & strings , uint32 key ,uint32 v , int type = Type  ,int32 h = 0 );
	GPropertyColorItem( QTreeWidget * parent, QTreeWidgetItem * preceding , uint32 key ,uint32 v, int type = Type  ,int32 h = 0 );
	GPropertyColorItem( QTreeWidgetItem * parent, uint32 key ,uint32 v , int type = Type ,int32 h = 0 );
	GPropertyColorItem( QTreeWidgetItem * parent, const QStringList & strings , uint32 key ,uint32 v , int type = Type ,int32 h = 0 );
	GPropertyColorItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding , uint32 key ,uint32 v , int type = Type,int32 h = 0 );
	GPropertyColorItem( const QTreeWidgetItem & other , uint32 key ,uint32 v ,int32 h = 0 );

	virtual ~GPropertyColorItem(  );

	uint32 key(  ) const{ return m_key; }
	uint32 value(  ) const{ return m_value;}

	void   setValue( uint32 vrgb );

	virtual void  showEditor(  );
	virtual void  hideEditor(  );

	virtual bool hasCustomContents(  ) const;
	virtual void drawCustomContents(  QPainter *p , const QRect &r  );

signals:
	void valueChanged( QTreeWidgetItem*  );

	private slots:
		void editValue(  );

private:
	void  createChildren(  );

	QFrame*		 box;
	QHBoxLayout  *layout;
	QFrame*		 colorPrev;
	QPushButton* button;

	uint32		 m_key;
	uint32		 m_value;
};
 
class GDE_EXPORT_MFCTOOLS GPropertyPixmapItem : public QObject ,public GPropertyItem
{
    Q_OBJECT

public:
	GPropertyPixmapItem( const QString &v , int type = Type , int32 h = 0 );
	GPropertyPixmapItem( const QStringList & strings ,const QString &v , int type = Type , int32 h = 0 );
	GPropertyPixmapItem( QTreeWidget * parent, const QString &v, int type = Type , int32 h = 0 );
	GPropertyPixmapItem( QTreeWidget * parent, const QStringList & strings, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyPixmapItem( QTreeWidget * parent, QTreeWidgetItem * preceding, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyPixmapItem( QTreeWidgetItem * parent, const QString &v, int type = Type , int32 h = 0 );
	GPropertyPixmapItem( QTreeWidgetItem * parent, const QStringList & strings, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyPixmapItem( QTreeWidgetItem * parent, QTreeWidgetItem * preceding, const QString &v, int type = Type ,int32 h = 0 );
	GPropertyPixmapItem( const QTreeWidgetItem & other ,const QString &v, int32 h = 0 );

	virtual ~GPropertyPixmapItem();

    virtual void showEditor();
    virtual void hideEditor();

    void setValue( const QString &v );
	QString value() const { return m_value;	}

	virtual bool hasCustomContents() const;
    virtual void drawCustomContents( QPainter *p , const QRect &r );

signals:
	void valueChanged( QTreeWidgetItem*  );

private slots:
	void editValue(  );

private:
	void  createChildren(  );

private:
	QHBoxLayout*	box;
	QFrame*			frame;
	QLabel*			pixPrev;
    QPushButton*	button;
	QString			m_value;

};

class GDE_EXPORT_MFCTOOLS GPropertyListEdit:public QTreeWidget
{
	Q_OBJECT
public:

	GPropertyListEdit( QWidget* parent = 0 , const char* name = 0 );
	~GPropertyListEdit(  );

	void setObj(  GBaseObject* pObj );

	void refetchData(  );

	virtual void setCurrentItem(  QTreeWidgetItem *i  );

public:
	typedef struct
	{
		GBaseObject*	obj;
		QString			stru;
		QString 		name;
		void*			ptr ;
	}*PDATA ,DATA;

	void copyData(  PDATA dst ,PDATA src );
	void setupProperties( GPropertyItem*  ,GBaseObject *pObj );
	void setupBaseInfo( GPropertyItem*  ,GBaseObject *pObj );
	void setupComInfo( GPropertyItem*  ,GBaseObject *pObj );
	void setupSpecInfo( GPropertyItem*  ,GBaseObject *pObj );
	void setupDataInfo( GPropertyItem*  ,GBaseObject *pObj );

	void setupData(  GPropertyItem* , PDATA  );

	GPropertyItem* newBoolItem( GPropertyItem* ,const char* ,uint8  );
	GPropertyItem* newComboItem(  GPropertyItem* ,const char* ,const QStringList&  ,int32  );
	GPropertyItem* newComboItem(  GPropertyItem* ,const char* ,const QStringList&  ,const QString&  );
	GPropertyItem* newIntItem( GPropertyItem* ,const char* ,int32  );
	GPropertyItem* newDoubleItem( GPropertyItem* ,const char* ,FLOAT8  );
	GPropertyItem* newTextItem( GPropertyItem* ,const char* ,const char*  );
	GPropertyItem* newColorItem( GPropertyItem* ,const char* ,uint32 );
	GPropertyItem* newPixmapItem( GPropertyItem* ,const char* ,const char* str  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const char*  = NULL );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GListPOINTS& ,const GListUINT8& );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GListPOINTS& );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,uint32 ,uint8  = 0 );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,FLOAT8 );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GPoint&  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GRect&  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GFillStyle&  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GLineStyle&  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GTextStyle&  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GVersion&  );
	GPropertyItem* setupItem( GPropertyItem* ,const char* ,const GRange& );

	void exec();
	void clear();

signals:
	void editBoolean( QTreeWidgetItem* );
	void editComboBox( QTreeWidgetItem* );
	void editInt( QTreeWidgetItem* );
	void editDouble( QTreeWidgetItem* );
	void editText( QTreeWidgetItem* );
	void editColor( QTreeWidgetItem* );
	void editPixmap( QTreeWidgetItem* );

protected:

	void resizeEvent( QResizeEvent *e );
protected slots:

	void updateEditorSize(  );
	void onListViewReturn( QTreeWidgetItem*  );

	void onEditObj_Boolean( QTreeWidgetItem*  );
	void onEditObj_ComboBox( QTreeWidgetItem*  );
	void onEditObj_Int( QTreeWidgetItem*  );
	void onEditObj_Double( QTreeWidgetItem*  );
	void onEditObj_Text( QTreeWidgetItem*  );
	void onEditObj_Color( QTreeWidgetItem*  );
	void onEditObj_Pixmap( QTreeWidgetItem*  );

protected:

	GBaseObject* m_pObj;
	GListPTRS    m_data;
};

#endif
