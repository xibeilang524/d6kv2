#ifndef _GCOMMOBJS_H
#define _GCOMMOBJS_H

#include "ddef/GList.h"
#include "gcommfunc.h"
#include "gxml.h"

#define SO_SHOWOBJ	0x00
#define SO_SHOWPIN	0x01
#define SO_SHOWALL	0x02

class GDE_EXPORT_DES GListPTRS:public GList<void*>{};
class GDE_EXPORT_DES GListUINT8:public GList<uint8>{};
class GDE_EXPORT_DES GListINT32S:public GList<int32>{};
class GDE_EXPORT_DES GListLINES:public GList<GLineStyle*>{};
class GDE_EXPORT_DES GListFILLS:public GList<GFillStyle*>{};
class GDE_EXPORT_DES GListTEXTS:public GList<GTextStyle*>{};
class GDE_EXPORT_DES GListLAYERS:public GList<GGraphLayer *>{};
class GDE_EXPORT_DES GListOBJECTS:public GList<GBaseObject*>{};

GDE_EXPORT_DES GBaseObject*  newLineObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& style);

GDE_EXPORT_DES GBaseObject*  newBezierObj(\
		const GListPOINTS& pts,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle);

GDE_EXPORT_DES GBaseObject*  newPinObj(\
		const GPoint& ptc ,\
		const GLineStyle& style);

GDE_EXPORT_DES GBaseObject*  newDynTextObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GFillStyle& fs,\
		const GTextStyle& ts,\
		GBaseObject* pObj);

GDE_EXPORT_DES GBaseObject*  newTextObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GFillStyle& fs,\
		const GTextStyle& style,\
		const QString& strText);

GDE_EXPORT_DES GBaseObject* newImageObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& style,\
		GBaseObject* pObj);

GDE_EXPORT_DES GBaseObject* newImageObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& style,\
		const char* imgsrc,\
		uint32 imgmode=GFMD_STRETCH);

GDE_EXPORT_DES GBaseObject* newPluginObj(\
		const GListPOINTS& pts ,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		const GTextStyle& tstyle,\
		const QString& srcPath,\
		GFile& fFile);
GDE_EXPORT_DES GBaseObject* newPluginObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		const GTextStyle& tstyle,\
		const QString& srcPath,\
		GFile& fFile);

GDE_EXPORT_DES GBaseObject*  newComObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		const GTextStyle& tstyle,\
		GBaseObject* pObj,\
		uint8 link,\
		const char* linkPath,\
		uint8 chksize,\
		uint8 chkattr);

GDE_EXPORT_DES GBaseObject* newCustomObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		const GTextStyle& tstyle,\
		GBaseObject* pObj,\
		uint8 link,\
		const char* linkPath,\
		uint8 chksize,
		uint8 chkattr=1);

GDE_EXPORT_DES GBaseObject* newBayObj(\
		 const GPoint& pts,\
		 const GPoint& pte,\
		 const GLineStyle& lstyle,\
		 const GFillStyle& fstyle,\
		 const GTextStyle& tstyle,\
		 GBaseObject* pObj,\
		 uint8 link,\
		 const char* linkPath,\
		 uint8 chksize,
		 uint8 chkattr = 1);

GDE_EXPORT_DES GBaseObject*  newRectObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle);

GDE_EXPORT_DES GBaseObject*  newBusObj(\
		const GPoint& pts ,\
		const GPoint& pte,\
		const GFillStyle& fstyle);

GDE_EXPORT_DES GBaseObject*  newButtonObj(\
		const GRect& rc,\
		const QString& strCaption,\
		const QString& cmdString);

GDE_EXPORT_DES GBaseObject*  newButtonObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		const GTextStyle& tstyle,\
		GBaseObject* pObj );

GDE_EXPORT_DES GBaseObject*  newRoundRectObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		FLOAT8 RX=25,FLOAT8 RY=25);

GDE_EXPORT_DES GBaseObject*  newCArcObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		FLOAT8 AS=45,FLOAT8 AE=315);

GDE_EXPORT_DES GBaseObject*  newEllipseObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle);

GDE_EXPORT_DES GBaseObject*  newCircleObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		uint8 excircle=0);

GDE_EXPORT_DES GBaseObject*  newPathObj(\
		const GListPOINTS& pts ,\
		const GListUINT8& codes,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		uint8 polymode=GPM_WINDING);

GDE_EXPORT_DES GBaseObject*  newPolylineObj(\
		const GListPOINTS& pts ,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		uint8 polymode=GPM_WINDING);

GDE_EXPORT_DES GBaseObject*  newFlowlineObj(\
		const GListPOINTS& pts ,\
		const GLineStyle& lstyle);

GDE_EXPORT_DES GBaseObject*  newLinklineObj(\
		const GListPOINTS& pts ,\
		const GLineStyle& lstyle);

GDE_EXPORT_DES GBaseObject*  newArcObj(\
		const GListPOINTS& pts ,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle);

GDE_EXPORT_DES GBaseObject*  newPolygonObj(\
		const GPoint& pts,\
		const GPoint& pte,\
		const GLineStyle& lstyle,\
		const GFillStyle& fstyle,\
		uint8 excircle=0);


GDE_EXPORT_DES GRect getTR( const QFont&, const GTextStyle&,const QString&);
GDE_EXPORT_DES GRect getTR( QPainter*,const GTextStyle&,const QString&);
GDE_EXPORT_DES void	 setIS( GBaseObject* pObj , const GRect& rcRect);
GDE_EXPORT_DES GPoint*	SEP(const GPoint& ptc,uint32 v,FLOAT8 r,FLOAT8 a=0);

GDE_EXPORT_DES GBaseObject* createGO(uint32 type);
GDE_EXPORT_DES QString  convGOD(uint32 type);
GDE_EXPORT_DES const char* convGON(uint32 type);
GDE_EXPORT_DES uint32	convGOT(const char* name);
GDE_EXPORT_DES uint8	convLCO(GBaseObject** dstObj,GBaseObject* srcObj);
GDE_EXPORT_DES uint8	copyObject(GBaseObject** dstObj, GBaseObject* srcObj);

GDE_EXPORT_DES int32 saveGOL(GListOBJECTS& objs,GFile& fFile,uint32 ncount=(uint32)-1);
GDE_EXPORT_DES int32 loadGOL(GListOBJECTS& objs,GFile& fFile,GGraphLayer* pLayer);

GDE_EXPORT_DES void  showObject(GBaseObject* pObj,uint8 bPin);

#endif
