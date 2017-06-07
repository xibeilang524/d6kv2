#ifndef _GCOMMFUNC_H
#define _GCOMMFUNC_H

#include <Qt/qwindowdefs.h>
#include <QtGui/QColor>

#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddef/GRect.h"
#include "gcache.h"

class GObjectManage;
class GBaseObject;
class GGraphDynData;
class GLineStyle;
class GFillStyle;
class GTextStyle;
class QPainter;
class Ddbi;

#define GT_FAILED		0x00
#define GT_BINGRAPH		0x01
#define GT_XMLGRAPH		0x02

typedef struct
{
	uint32 actMask  ;
	uint8  flash    ;
	uint32 clrFore  ;
	uint32 clrBack  ;
	uint32 clrLine  ;
	uint32 clrText  ;
	uint32 clrTBack ;
	FLOAT8 rotate   ;
}DYNACTION , *LPDA ;

typedef struct 
{
	QString	srvType;
	QString	rtable;
	QString htable;
	QString rfieldDesc;
	QString rfieldCode;
	QString	fieldDesc;
	QString fieldCode;
	FLOAT8	value;
	QString action;
}DYNACTIONDATA , *LPDAD;

class GDE_EXPORT_DES GDAL:public GList<LPDA>{};
class GDE_EXPORT_DES GDADL:public GList<LPDAD>{};

GDE_EXPORT_DES  QRgb    convG2Q_RGB(uint32 color);
GDE_EXPORT_DES  uint32	convQ2G_RGB(QRgb color);
GDE_EXPORT_DES	QColor  convG2Q_CLR(uint32 color);
GDE_EXPORT_DES  uint32  convQ2G_GLR(const QColor& color);
GDE_EXPORT_DES  void	convGPL_QPA(const GListPOINTS& pl,QPolygon& pa);	// QPointArray->QPolygon [1/16/2013 ljs]
GDE_EXPORT_DES  void    convQPA_GPL(const QPolygon& pa,GListPOINTS& pl);

GDE_EXPORT_DES	void	setPLR( FLOAT8 plr );
GDE_EXPORT_DES	FLOAT8	getPLR();
GDE_EXPORT_DES	void	setPPR( FLOAT8 ppr );
GDE_EXPORT_DES	FLOAT8	getPPR();
GDE_EXPORT_DES  void	setCPH( FLOAT8 cph);
GDE_EXPORT_DES  FLOAT8  getCPH();
GDE_EXPORT_DES  void    setCPV( FLOAT8 cpv);
GDE_EXPORT_DES  FLOAT8  getCPV();
GDE_EXPORT_DES	void	setHZ( uint8 bHz );
GDE_EXPORT_DES	uint8	getHZ();
GDE_EXPORT_DES  void	setRunMode( uint8 bRun=1);
GDE_EXPORT_DES  uint8	getRunMode();
GDE_EXPORT_DES  void	setBackupMode(uint8 backup);
GDE_EXPORT_DES  uint8	isBackupMode();
GDE_EXPORT_DES uint8    DCTD(const char* str,uint8 *bin,int& len );
GDE_EXPORT_DES uint8*   DCTD(const char* str,int& len,uint8 *bok=0);
GDE_EXPORT_DES uint8    EDTC(const uint8 *bin,int len,char* str);
GDE_EXPORT_DES char*    EDTC(const uint8 *bin,int len,uint8* bok=0);
GDE_EXPORT_DES void		BCP(GPoint* lpt,const GPoint& pts,const GPoint& pte);  
GDE_EXPORT_DES GPoint*	BDB( GPoint *lpt,int ncount , int &rcnt );

GDE_EXPORT_DES uint8  getEnableActiveFill();
GDE_EXPORT_DES void   setEnableActiveFill(uint8 bFill);
GDE_EXPORT_DES uint8  getEnableActiveLine();
GDE_EXPORT_DES void   setEnableActiveLine(uint8 bLine);
GDE_EXPORT_DES uint32 getLostColor();
GDE_EXPORT_DES void   setLostColor(uint32 color);
GDE_EXPORT_DES uint8 getTopoColor( int32 index, uint32 &color);
GDE_EXPORT_DES void	  setTopoColor(int32 index,uint32 color);
GDE_EXPORT_DES void	  set3DFaceCLR( uint32 color);
GDE_EXPORT_DES void	  set3DLightCLR( uint32 color);
GDE_EXPORT_DES void   set3DShadowCLR(uint32 color);
GDE_EXPORT_DES void	  set3DDKShadowCLR(uint32 color);
GDE_EXPORT_DES void	  set3DGrayCLR(uint32 color);
GDE_EXPORT_DES void	  set3DHotLightCLR( uint32 color);
GDE_EXPORT_DES void	  set3DHighLightCLR( uint32 color);
GDE_EXPORT_DES void	  set3DSelectCLR( uint32 color);
GDE_EXPORT_DES uint32 get3DFaceCLR();
GDE_EXPORT_DES uint32 get3DLightCLR();
GDE_EXPORT_DES uint32 get3DShadowCLR();
GDE_EXPORT_DES uint32 get3DDKShadowCLR();
GDE_EXPORT_DES uint32 get3DGrayCLR();
GDE_EXPORT_DES uint32 get3DHotLightCLR();
GDE_EXPORT_DES uint32 get3DHighLightCLR();
GDE_EXPORT_DES uint32 get3DSelectCLR();

GDE_EXPORT_DES FLOAT8 fixAngle(FLOAT8 angle);
GDE_EXPORT_DES void	  FDAL(GDAL& dal);
GDE_EXPORT_DES void	  FDADL(GDADL& dadl);
GDE_EXPORT_DES uint8  RDAL(const QString& strPath,GDAL& dal,QStringList& daIds,Ddbi* ddbi=NULL);
GDE_EXPORT_DES uint8  RDADL(const QString& strPath,GDADL& dadl,QStringList& daNames,Ddbi* ddbi=NULL);
GDE_EXPORT_DES uint8  SDAL(const QString& strPath,GDAL& dal,QStringList& daNames,Ddbi* ddbi=NULL);
GDE_EXPORT_DES uint8  SDADL(const QString& strPath,GDADL& dadl,QStringList& daIds,Ddbi* ddbi=NULL);

GDE_EXPORT_DES void   clearDAMask(LPDA da);
GDE_EXPORT_DES void	  setDAMask(LPDA da,uint32 mask);
GDE_EXPORT_DES void   clrDAMask(LPDA da,uint32 mask);
GDE_EXPORT_DES uint8  isDAMask(LPDA da,uint32 mask);
GDE_EXPORT_DES uint8  isDANone(LPDA da);
GDE_EXPORT_DES void   setDAFlashNone(LPDA da);
GDE_EXPORT_DES void	  setDAFlashVis(LPDA da);
GDE_EXPORT_DES void   setDAFlashInvert(LPDA da);
GDE_EXPORT_DES void	  setDAFlashFrame(LPDA da);
GDE_EXPORT_DES void   setDAFlashFill(LPDA da);
GDE_EXPORT_DES uint8  isDAFlash(LPDA da);
GDE_EXPORT_DES uint8  isDAFlashNone(LPDA da);
GDE_EXPORT_DES uint8  isDAFlashVis(LPDA da);
GDE_EXPORT_DES uint8  isDAFlashInvert(LPDA da);
GDE_EXPORT_DES uint8  isDAFlashFrame(LPDA da);
GDE_EXPORT_DES uint8  isDAFlashFill(LPDA da);
GDE_EXPORT_DES uint8  isDAFore(LPDA da);
GDE_EXPORT_DES void   clrDAFore(LPDA da);
GDE_EXPORT_DES void   setDAFore(LPDA da,uint32 clr);
GDE_EXPORT_DES uint32 getDAFore(LPDA da);
GDE_EXPORT_DES uint8  isDABack(LPDA da);
GDE_EXPORT_DES void   clrDABack(LPDA da);
GDE_EXPORT_DES void   setDABack(LPDA da,uint32 clr);
GDE_EXPORT_DES uint32 getDABack(LPDA da);
GDE_EXPORT_DES uint8  isDAFrame(LPDA da);
GDE_EXPORT_DES void   clrDAFrame(LPDA da);
GDE_EXPORT_DES void   setDAFrame(LPDA da,uint32 clr);
GDE_EXPORT_DES uint32 getDAFrame(LPDA da);
GDE_EXPORT_DES uint8  isDAText(LPDA da);
GDE_EXPORT_DES void   clrDAText(LPDA da);
GDE_EXPORT_DES void   setDAText(LPDA da,uint32 clr);
GDE_EXPORT_DES uint32 getDAText(LPDA da);
GDE_EXPORT_DES uint8  isDATextBack(LPDA da);
GDE_EXPORT_DES void   clrDATextBack(LPDA da);
GDE_EXPORT_DES void   setDATextBack(LPDA da,uint32 clr);
GDE_EXPORT_DES uint32 getDATextBack(LPDA da);
GDE_EXPORT_DES uint8  isDARotate(LPDA da);
GDE_EXPORT_DES void   clrDARotate(LPDA da);
GDE_EXPORT_DES void   setDARotate(LPDA da,FLOAT8 rotate);
GDE_EXPORT_DES FLOAT8 getDARotate(LPDA da);
GDE_EXPORT_DES uint8  comDynAct(LPDA dstDA,LPDA srcDA);

GDE_EXPORT_DES uint8  isLink(uint16 flag);
GDE_EXPORT_DES uint8  isLinkHI(uint32 flag);
GDE_EXPORT_DES uint8  isLinkLO(uint32 flag);
GDE_EXPORT_DES uint8  isLinkOK(uint32 flag);
GDE_EXPORT_DES void   addLink(uint16& flag);
GDE_EXPORT_DES void   addLinkHI(uint32& flag);
GDE_EXPORT_DES void   addLinkLO(uint32& flag);
GDE_EXPORT_DES void   delLink(uint16& flag);
GDE_EXPORT_DES void   delLinkHI(uint32& flag);
GDE_EXPORT_DES void   delLinkLO(uint32& flag);
GDE_EXPORT_DES uint8  cntLink(uint16 flag);
GDE_EXPORT_DES uint8  cntLinkHI(uint32 flag);
GDE_EXPORT_DES uint8  cntLinkLO(uint32 flag);
GDE_EXPORT_DES void   addObjLink(GBaseObject* pObj,uint8 usf);
GDE_EXPORT_DES void   clearObjLink(GBaseObject* pObj,uint8 usf);

GDE_EXPORT_DES int32  convWEEK(int y,int m,int d);
GDE_EXPORT_DES void	  conv2YMD(uint32 ymd , int& y,int& m,int& d);
GDE_EXPORT_DES uint32 convYMD2(int y,int m,int d);
GDE_EXPORT_DES uint32 convYMD2();
GDE_EXPORT_DES void   currYMD(int& y,int& m,int& d);
GDE_EXPORT_DES void	  conv2HMSM(uint32 hmsm , int& h,int& m,int& s,int& ms);
GDE_EXPORT_DES uint32 convHMSM2(int h,int m,int s,int ms);
GDE_EXPORT_DES uint32 convHMSM2();
GDE_EXPORT_DES void   currHMSM(int& h,int& m,int& s,int& ms);
GDE_EXPORT_DES void	  conv2HMS(uint32 hmsm , int& h,int& m,int& s);
GDE_EXPORT_DES uint32 convHMS2(int h,int m,int s);
GDE_EXPORT_DES uint32 convHMS2();
GDE_EXPORT_DES void   currHMS(int& h,int& m,int& s);
GDE_EXPORT_DES uint32 diffHMSM(uint32 src,uint32 dst);
GDE_EXPORT_DES void	  lastDYMD(int& y,int& m,int& d);  
GDE_EXPORT_DES void   lastMYMD(int& y,int& m,int& d);
GDE_EXPORT_DES void   lastYYMD(int& y,int& m,int& d);
GDE_EXPORT_DES uint32 makeFLAG(uint32 a,uint32 b,uint32 c,uint32 d);
GDE_EXPORT_DES uint8  isFS(uint32 imgmode);
GDE_EXPORT_DES uint8  isGT( const char* strFile,const QString& type);
GDE_EXPORT_DES void   sfPT( const GPoint& fisrtPt ,GPoint& lastPt, uint8 bshift=0 );
GDE_EXPORT_DES uint8  ignoreObj(GBaseObject* pObj);
GDE_EXPORT_DES GGraphDynData* getDynData(GBaseObject* pObj,uint32 type=100);

GDE_EXPORT_DES void	  setCIS(uint32 size);
GDE_EXPORT_DES uint32 getCIS(); 
GDE_EXPORT_DES uint32 getCI(); 
GDE_EXPORT_DES void	  addCI(const char* strPath,QPixmap* qpixmap);
GDE_EXPORT_DES LPIMAGECACHE  findCI(const char* strPath);
GDE_EXPORT_DES void	  freeCI();
GDE_EXPORT_DES void	  setCSS(uint32 size);
GDE_EXPORT_DES uint32 getCSS(); 
GDE_EXPORT_DES uint32 getCS(); 
GDE_EXPORT_DES void	  addCS(const char* strPath,uint32 nID,GBaseObject* symbol);
GDE_EXPORT_DES LPSYMBOLCACHE  findCS(const char* strPath,uint32 nID);
GDE_EXPORT_DES void	  freeCS();
GDE_EXPORT_DES GBaseObject* loadPO(const char* strPath);
GDE_EXPORT_DES void	  setCPS(uint32 size);
GDE_EXPORT_DES uint32 getCPS(); 
GDE_EXPORT_DES uint32 getCP(); 
GDE_EXPORT_DES LPPLUGINCACHE  findCP(const char* strPath);
GDE_EXPORT_DES void	  freeCP();
GDE_EXPORT_DES GGraphDynData* loadDDO(const char* strPath);
GDE_EXPORT_DES void	  setCDDS(uint32 size);
GDE_EXPORT_DES uint32 getCDDS(); 
GDE_EXPORT_DES uint32 getCDD(); 
GDE_EXPORT_DES LPDYNDATACACHE  findCDD(const char* strPath);
GDE_EXPORT_DES void	  freeCDD();

GDE_EXPORT_DES void   AOLL(GListPOINTS& ptList,const GPoint& destPt,uint8 order=0);
GDE_EXPORT_DES void   AOLL(GBaseObject* adjObj,GBaseObject* obObj,uint8 flag1,uint8 flag2);
GDE_EXPORT_DES void   AOPP(GBaseObject* adjObj,GBaseObject* obObj);
GDE_EXPORT_DES void   AOPB(GBaseObject* adjObj,GBaseObject* obObj);
GDE_EXPORT_DES void   AOPL(GBaseObject* adjObj,GBaseObject* obObj,uint8 flag);
GDE_EXPORT_DES void   AOLP(GBaseObject* adjObj,GBaseObject* obObj,uint8 flag);
GDE_EXPORT_DES void   AOLB(GBaseObject* adjObj,GBaseObject* obObj,uint8 flag);

GDE_EXPORT_DES GBaseObject*  getSO(GBaseObject *pObj,uint8 sno=0);
GDE_EXPORT_DES GBaseObject*  getSOP(GBaseObject *pObj,uint8 sno=0);
GDE_EXPORT_DES uint32		 getSOS(GBaseObject* pObj,GObjectManage& objs);
GDE_EXPORT_DES int			 getSC(GBaseObject *pObj);
GDE_EXPORT_DES uint32		 getSI(GBaseObject* pObj);

#endif
