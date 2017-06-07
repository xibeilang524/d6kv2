#ifndef _GWSTYLE_H
#define _GWSTYLE_H
#include <QtCore/QSize>
#include "mfc/ddefs.h"
#include <QtGui/QWindowsStyle>

class GDE_EXPORT_MFC GWStyle:public QWindowsStyle
{
public:

	GWStyle();

// 	QSize sizeFromContents( ContentsType contents,
// 
// 				       const QWidget *widget,
// 
// 				       const QSize &contentsSize,
// 
// 				       const QStyleOption& opt ) const;
	QSize sizeFromContents(ContentsType contents,const QStyleOption *opt,const QSize &contentsSize,const QWidget *widget) const ;

//     void drawPrimitive( PrimitiveElement pe,
// 
// 			QPainter *p,
// 
// 			const QRect &r,
// 
// 			const QColorGroup &cg,
// 
// 			SFlags flags = State_None,
// 
// 			const QStyleOption& = QStyleOption::SO_Default ) const;
	void drawPrimitive( PrimitiveElement pe,const QStyleOption* opt,QPainter *p,const QWidget * widget = 0) const ;

//     void drawControl( ControlElement element,
// 
// 		      QPainter *p,
// 
// 		      const QWidget *widget,
// 
// 		      const QRect &r,
// 
// 		      const QColorGroup &cg,
// 
// 		      SFlags flags = State_None,
// 
// 		      const QStyleOption& = QStyleOption::SO_Default ) const;
	void drawControl( ControlElement element,const QStyleOption* opt,QPainter *p,const QWidget *widget = 0) const ;

//     void drawComplexControl( ComplexControl control,
// 
// 			     QPainter* p,
// 
// 			     const QWidget* widget,
// 
// 			     const QRect& r,
// 
// 			     const QColorGroup& cg,
// 
// 			     SFlags flags = State_None,
// 
// 			     SCFlags sub = SC_All,
// 
// 			     SCFlags subActive = SC_None,
// 
// 			     const QStyleOption& = QStyleOption::SO_Default ) const;
	void drawComplexControl(ComplexControl ctrl, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget = 0) const ;

    void polish(QWidget *widget);

};

#endif
