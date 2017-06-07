#ifndef _GMDOC_H
#define _GMDOC_H

#include "mfc/dmfc.h"
#include "ddes/GDC.h"
#include "ddes/GGraphFile.h"
#include "ddes/GXYCoorTrans.h"
#include <QtGui/QPixmap>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QPaintEvent>

class GDE_EXPORT_MFCDOC GMDoc:public GDoc, public GGraphFile
{
	Q_OBJECT

public:

	GMDoc();
	virtual ~GMDoc();

public:
	virtual const QString getOwner() const;
	virtual void releaseDoc();
	virtual void deleteContents();

	virtual void onDraw(GDC *pDC ,\
		const GRect& rcRect ,\
		GScaleFactor* pSF=NULL);

	virtual bool readFile(GFile& fFile);
	virtual bool writeFile(GFile& fFile);
	virtual bool loadXML(const QString& strPath);
	virtual bool writeXML(const QString& strPath);
};


class GDE_EXPORT_MFCDOC GMView: public GView, public GXYCoorTrans
{
	Q_OBJECT

public:
	GMView(QWidget * parent);

	virtual ~GMView();

	GMDoc* getDoc() const {return (GMDoc *)m_pDoc;}

	GDC* getGDC() const {return (GDC   *)&m_dcView;}

	QPixmap* getBuffer() const{return (QPixmap*)&m_bufView;}

	QPixmap* getDoubleBuffer() const{return (QPixmap*)&m_bufDoc;}

	void updateRect(const GRect& rcRect);

	void updateDoubleRect(const GRect& rcRect);

public:

	GRect GetClientRect() const ;
	GPoint GetCursorPos() const ;

	virtual void OnDraw(GDC* pDC, const GRect& rcRect){};

	virtual void OnLButtonDown(const GPoint& point,uint32 nFlags){};

	virtual void OnLButtonUp(const GPoint& point,uint32 nFlags){};

	virtual void OnRButtonDown(const GPoint& point,uint32 nFlags){};

	virtual void OnRButtonUp(const GPoint& point,uint32 nFlags){};

	virtual void OnMouseMove(const GPoint& point, uint32 nFlags){};

	virtual void OnLDoubleClick(const GPoint& point,uint32 nFlags){};

	virtual void OnRDoubleClick(const GPoint& point,uint32 nFlags){};

	virtual void OnKeyDown(QKeyEvent* e,int nChar,uint32 nFlags){};

	virtual void OnKeyUp(QKeyEvent* e,int nChar,uint32 nFlags){};

	virtual void OnMouseWheel(const GPoint& point,short zDelta , uint32 nFlags){};

	virtual void OnClose(){};

protected:

	void paintEvent(QPaintEvent* e);

	void mousePressEvent(QMouseEvent* e);

	void mouseReleaseEvent(QMouseEvent*e);

	//右键弹出菜单的事件，如果不捕捉
	//会导致这个事件被主窗口捕捉，弹出主窗口的右键菜单
	void contextMenuEvent(QContextMenuEvent* e);

	void mouseDoubleClickEvent(QMouseEvent* e);

	void mouseMoveEvent(QMouseEvent *e);

	void keyPressEvent(QKeyEvent *e);

	void keyReleaseEvent(QKeyEvent *e);

	void wheelEvent(QWheelEvent *e);

	void closeEvent(QCloseEvent* e);

protected:

	void grabMouse(){};
	void releaseMouse(){};

protected:

	GDC			m_dcView;
	QPixmap		m_bufView;
	QPixmap		m_bufDoc;
};

#endif
