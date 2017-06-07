#ifndef _GXML_H

#include <QtCore/QString>
#include <QtXml/QtXml>

#include "ddef/ddes_comm.h"
#include "ddef/GRect.h"
#include "ddef/GPoint.h"
#include "GBaseObject.h"
#include "GVersion.h"
#include "GRange.h"
#include "GFillStyle.h"
#include "GLineStyle.h"
#include "GTextStyle.h"

GDE_EXPORT_DES void xmlVersion( QDomElement& dom,const GVersion& vers ) ;
GDE_EXPORT_DES void xmlRange( QDomElement& dom, const GRange& range ) ;
GDE_EXPORT_DES void xmlBound( QDomElement& dom,const GRect& rc )  ;
GDE_EXPORT_DES void xmlPoint( QDomElement& dom, const QString& name,const GPoint& pt);
GDE_EXPORT_DES QString xmlPoint( const GPoint& pt);
GDE_EXPORT_DES void xmlFillStyle( QDomElement& dom,const GFillStyle& style)  ;
GDE_EXPORT_DES void xmlLineStyle( QDomElement& dom,const GLineStyle& style)  ;
GDE_EXPORT_DES void xmlTextStyle( QDomElement& dom,const GTextStyle& style)  ;

GDE_EXPORT_DES GVersion   xmlVersion( QDomElement& dom);
GDE_EXPORT_DES GRange     xmlRange( QDomElement& dom  );
GDE_EXPORT_DES GRect	  xmlBound( QDomElement& dom  );
GDE_EXPORT_DES GPoint	  xmlPoint( QDomElement& dom, const QString& name);
GDE_EXPORT_DES GPoint	  xmlPoint( const QString& v );
GDE_EXPORT_DES GFillStyle xmlFillStyle( QDomElement& dom );
GDE_EXPORT_DES GLineStyle xmlLineStyle( QDomElement& dom );
GDE_EXPORT_DES GTextStyle xmlTextStyle( QDomElement& dom );

#endif
