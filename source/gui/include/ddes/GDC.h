#ifndef GDC_H_HEADER_INCLUDED_BCB8BDBA
#define GDC_H_HEADER_INCLUDED_BCB8BDBA

#include <time.h>
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddef/GRect.h"
#include "GObjectCommon.h"
#include "GFillStyle.h"
#include "GLineStyle.h"
#include "GTextStyle.h"
#include "gcommfunc.h"
#include <QtGui/QPainter>
#include <QtGui/QColorGroup>
#include <QtGui/QtGui>


class GCoorTrans ;
// class GDE_EXPORT_DES GDC:public QPainter
class GDE_EXPORT_DES GDC:public QPainter
{
	static  QPixmap *m_pixPattern  ;
	static  uint8	 m_pixAutoFree ;
	GCoorTrans*		 m_coorTrans   ;
	uint8			 m_bPrinting   ;
	uint8			 m_bPenScale   ;
	uint8			 m_bFontScale  ;

public:

	static int32 getMaxPattern();
	static void allocPattern();
	static void freePattern();
	static QPixmap *getPixPattern();
	static QPixmap getPattern( int nPattern );
	static QPixmap getMaskPattern(int nPattern);
	static Qt::PenStyle GPS2QPS( int32 style );

	static QBitmap createMask( const QPixmap& pix , uint32 clr );
	static QPixmap createReservePixmap( const QPixmap& pixmap , uint32 clrReserve);

public:

	enum
	{
		GOS_POINT  =0x01,
		GOS_CROSS  =0x02,
		GOS_ECROSS =0x04,
		GOS_CIRCLE =0x10,
		GOS_DIAMOND=0x20,
		GOS_RECT   =0x40,
		GOS_FILL   =0x80
	};

	enum {G_HEX,G_DEC,G_OCT,G_BIN};
	enum {G_QUAD1,G_QUAD2,G_QUAD3,G_QUAD4,G_QUAD};

	QRegion saveClipRegion(const QRegion& region,bool& bClipping);
	void    restoreClipRegion(const QRegion& oldRegion,bool bClipping);
	QPainterPath saveClipPath(const QPainterPath& path,bool& bClipping);
	void    restoreClipPath(const QPainterPath& oldPath,bool bClipping);


	void	saveG();
	void	restoreG();

	void	setPrinting(uint8 bPrinting=1);
	uint8   isPrinting() ;

	void	setCoorTrans( GCoorTrans* trans );
	GCoorTrans*  getCoorTrans();

	void	setPenScale( uint8 bScale=1);
	uint8	isPenScale();

	void	setFontScale( uint8 bScale=1);
	uint8	isFontScale();

	QFont	chgGFont ( uint8& chgFont );
	void	setGFont ( const QFont & font );

	int		setROP2( int rop );

	QPen	chgGPen( uint8& chgPen );
	void	setGPen( uint32 c , int32 w=-1 , int32 style=-1);

	void	setGBrush( uint32 c , Qt::BrushStyle=Qt::SolidPattern);//  [1/16/2013 ljs]

	uint8	setGTextStyle(const GTextStyle& style);
	int32   getGTextFlag(const GTextStyle& style);

public:

	void drawHPointArcR(const GPoint& pts,\
		const GPoint& ptc,\
		const GPoint& pte,\
		uint8 pie,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);
	void drawHArcO(const GPoint& pto,\
		FLOAT8 al,FLOAT8 bl,\
		FLOAT8 sa,FLOAT8 alen,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHArcL(const GPoint& pto,\
		FLOAT8 al,FLOAT8 bl,\
		FLOAT8 sa,FLOAT8 alen,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHArcR(const GPoint& pto,\
		FLOAT8 al,FLOAT8 bl,\
		FLOAT8 sa,FLOAT8 alen,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHRectR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHRoundRectR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		FLOAT8 rndX ,FLOAT8 rndY ,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHCircleR(const GPoint& pto,\
		FLOAT8 radius,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHEllipseR(const GPoint& pto,\
		FLOAT8 al,FLOAT8 bl,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHImageR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle,\
		uint8  trans,\
		uint32 clrmask);

	void drawHPolygonR(const GPoint& pto,\
		FLOAT8  radius,\
		GPoint* lppoints, int32 npoints,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHPolylineR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		uint8  polymode,\
		GPoint* lppoints, int32 npoints,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		const GLineStyle& ls,\
		FLOAT8 angle);

	void drawHButtonR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		uint8 bfill,\
		const GFillStyle& fs,\
		uint8 bframe,\
		uint32 ls,\
		uint8 btxt,\
		const GTextStyle& ts,\
		const char* label,\
		FLOAT8 angle);

	void drawHTextR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		uint8 btxt,\
		const GTextStyle& ts,\
		const char* label,\
		FLOAT8 angle);

	void drawHDynTextR(const GPoint& pto,\
		FLOAT8 halfw,FLOAT8 halfh,\
		uint8 dismode,\
		uint8 btxt,\
		const GTextStyle& ts,\
		const char* label,\
		FLOAT8 angle);
public:

	GDC();
	GDC(QPaintDevice *p );
	~GDC();

	/*
	void	_moveGTo( FLOAT8 x , FLOAT8 y );
	void	_lineGTo( FLOAT8 x , FLOAT8 y );
	void	_moveGTo( const GPoint& pt );
	void	_lineGTo( const GPoint& pt );
	*/

	void	_drawGArrow( const GPoint& pt,const GLineStyle& style,uint32 arrowsize ,FLOAT8 angle=0);
	void	drawGArrow( const GPoint& pt,const GLineStyle& style,uint32 arrowsize ,FLOAT8 angle=0);

	void	_drawGLine( const GPoint& pt1 ,const GPoint& pt2 ,const GLineStyle& style);
	void	_drawGLine( const GPoint& pt1 ,const GPoint& pt2 ,const GLineStyle& style,FLOAT8 angle );
	void	drawGLine(  const GPoint& pt1 ,const GPoint& pt2 ,const GLineStyle& style );
	void	drawGLine(  const GPoint& pt1 ,const GPoint& pt2 ,const GLineStyle& style,FLOAT8 angle );

	void	_drawGLine( FLOAT8 x1,FLOAT8 y1,FLOAT8 x2,FLOAT8 y2);
	void	_drawGLine( FLOAT8 x1,FLOAT8 y1,FLOAT8 x2,FLOAT8 y2, uint32 c);

	void	_drawGPoint(FLOAT8 x, FLOAT8 y);

	//简单填充
	void	_fillGSimpleRect(const GRect& rc, uint32 f, uint32 b);
	//光栅填充之双色渐变和复变及立体填充处理
	void	_fillGShadeRect(const GRect& rc, uint32 f, uint32 b, uint32 m);
	//光栅填充之辐射渐变
	void	_fillGRadialRect(const GRect& rc, uint32 f, uint32 b, uint32 m);
	//光栅填充之角度渐变
	void	_fillGConicalRect(const GRect& rc, uint32 f, uint32 b, uint32 m);
	void	_fillGPattern(const GRect& rc,uint32 f,uint32 b,uint32 m);
	void	_fillGImage(const GRect& rc,const char* f,uint32 m,uint8 trans,uint32 mask);
	void	_drawGBorder( const GRect& rcRect , uint32 uflags );
	void	drawGBorder( const GRect& rcRect , uint32 uflags );
	void	_drawGRect(FLOAT8 x,FLOAT8 y,FLOAT8 w,FLOAT8 h);	
	void	_drawGRect( const GRect& rcRect , uint32 c );
	void	_drawGRect( const GRect& rcRect , const GLineStyle& style);
	void	drawGRect( const GRect& rcRect );	
	void	drawGRect( const GRect& rcRect , uint32 c );	
	void	drawGRect( const GRect& rcRect , const GLineStyle& style );
	void	_fillGRect( const GRect& rcRect , uint32 b );	
	void	_fillGRect( const GRect& rcRect , const GFillStyle& style,uint8 trans=0,uint32 clrmask=(uint32)-1);
	void	fillGRect( const GRect& rcRect , uint32 b );
	void	fillGRect( const GRect& rcRect , const GFillStyle& style,uint8 trans=0,uint32 clrmask=(uint32)-1);

	void	_drawGEllipse(FLOAT8 x,FLOAT8 y,FLOAT8 w,FLOAT8 h);
	void	_drawGEllipse2( FLOAT8 x,FLOAT8 y,FLOAT8 a,FLOAT8 b);

	void	fillGEllipse( const GRect& rcRect ,const GFillStyle& style );
	void	fillGEllipse( const GPoint& ptc ,FLOAT8 a,FLOAT8 b, const GFillStyle& style );

	void	_drawGArc( FLOAT8 x , FLOAT8 y,FLOAT8 w,FLOAT8 h,FLOAT8 a,FLOAT8 alen);
	void	_drawGArc( FLOAT8 x,FLOAT8 y,FLOAT8 r0,FLOAT8 r1,FLOAT8 as,FLOAT8 ae,uint32 color);
	void	_drawGArc( const GPoint& pts,const GPoint& ptc,const GPoint& pte,uint8 pie=GPAM_ARC);
	void	drawGArc( const GPoint& pts,const GPoint& ptc,const GPoint& pte,const GLineStyle& style,uint8 pie=GPAM_ARC);

	//  [1/29/2013 ljs]
// 	QPolygon makearc(const GPoint& pts,\
// 		const GPoint& ptc,\
// 		const GPoint& pte,\
// 		uint8 pie=GPAM_ARC);

	void	fillGArc( const GPoint& pts,const GPoint& ptc,const GPoint& pte,const GFillStyle& style,uint8 pie=GPAM_ARC);

	void	drawGArc( FLOAT8 x , FLOAT8 y,FLOAT8 w,FLOAT8 h,FLOAT8 a,FLOAT8 alen);
	void	drawGPie ( FLOAT8 x, FLOAT8 y, FLOAT8 w, FLOAT8 h,	FLOAT8 a, FLOAT8 alen );
	void	drawGPie( const GRect& rc ,FLOAT8 a, FLOAT8 alen );
	void	drawGPolyline(const QPolygonF& pa , const GLineStyle& style);
	void	drawGPolyline( GPoint* pts,int32 npoints, const GLineStyle& style );

	void	_drawGPolygon( GPoint* pts,  int32 npoints, uint8 winding=0 );
	void	drawGPolygon( const QPolygonF& pa , const GLineStyle& style , uint8 winding=0);

	void	drawGFocusRect( const GRect& rcRect );

	void	_drawGText ( const GRect & r,int32 tf,const QString & str);
	void	_drawGText( const GRect& ,const QString& strText,const GTextStyle& style );
	void	drawGText( const GRect& ,const QString& strText ,const GTextStyle& style );
	void	drawGText ( const GRect & r,int32 tf,const QString & str);

	void	_drawGTiledPixmap( const GRect& rc , const QPixmap& pixmap );
	void	_drawGPixmap ( FLOAT8 x, FLOAT8 y,const QPixmap & pixmap,\
					FLOAT8 sx = 0, FLOAT8 sy = 0,FLOAT8 sw = -1, FLOAT8 sh = -1 );

	void	drawGO( const GPoint& pt ,uint32 lc , uint32 fc , uint32 uflags,FLOAT8 range=3);
	void	drawGS( const GPoint& pt );
	void	drawGOS( const GPoint& pt );
	void	drawGHotOS(const GPoint& pt );

	void	drawGOE( const GPoint& pt );
	void	drawGHotOE(const GPoint& pt );

	void	drawGOC( const GPoint& pt);
	void	drawGHotOC(const GPoint& pt );

	void	drawGOR( const GPoint& pt);
	void	drawGHotOR(const GPoint& pt );

	void	drawGPin( const GPoint& pt,uint8 sno=0);
	void	drawGHotPin( const GPoint& pt);
	void	drawGFocusPin( const GPoint& pt);
	void    drawGLinkPin(const GPoint& pt);

	void	drawLCDString(const GRect& rc ,const QString &s ,uint32 fgColor=get3DFaceCLR());

	void	drawTextDate(const GRect& rc , const GTextStyle& style , time_t t );
	void	drawLCDDate(const GRect& rc , time_t t);
	void	drawTextTime(const GRect& rc , const GTextStyle& style , time_t t );
	void	drawLCDTime(const GRect& rc , time_t t);
	void	drawDateTime( const GRect& rc , const GTextStyle& style , time_t t );
	void	drawClock( const GRect& rc , time_t t);
	void	drawDial(const GRect& rc,uint32 style ,int nDial,\
				FLOAT8	start, FLOAT8 end , uint8 scale,\
				FLOAT8  value,\
				const GFillStyle& fs,const GLineStyle& ls,\
				const QString& label=QString::null,\
				uint32 clrLabel=c_RGB(0x00,0x00,0x00),\
				uint8 cw =0 , uint8 df = 1 , uint8 db = 1);
};

#endif
