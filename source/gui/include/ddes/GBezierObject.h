#ifndef GBEZIEROBJECT_H_HEADER_INCLUDED_BC0A2377
#define GBEZIEROBJECT_H_HEADER_INCLUDED_BC0A2377

#include "GPolylineObject.h"
class GScaleFactor;

//##ModelId=43F5E9A5002C
//##Documentation
//## 定义：
//##        四点控制曲线
//## 功能：
//##        控制曲线
//## 设计：
//##        宋永生
//## 日期：
//##       2006-2－17
class GDE_EXPORT_DES GBezierObject : public GPolylineObject
{
  public:
    //##ModelId=43F5EA2C002C
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GBezierObject();

    //##ModelId=43F5EA2C004B
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GBezierObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GBezierObject(const GBezierObject& copy);

  	//##ModelId=43F674F90213
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC //绘制环境
    //##   uint32 nFlags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual void drawFocus(GDC* pDC, uint32 nFlags);

	//##ModelId=4340A54901C5
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC //绘制环境
    //##   const GFillStyle& fs//填充属性
	//##   const GLineStyle& ls//边框属性
	//##   uint8 bFill=1//是否填充
	//##   uint8 bFrame=1//是否绘制边框
	//##   GScaleFactor* pSF//缩放因子
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
	virtual void drawNormal(GDC *pDC ,\
		const GFillStyle& fs,\
		const GLineStyle& ls ,\
		uint8 bFill=1, uint8 bFrame=1 ,\
		GScaleFactor *pSF=NULL );

    //##ModelId=43F5EA2C0050
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GBezierObject&
    //## 返回：
    //##     const GBezierObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GBezierObject& operator=(const GBezierObject& copy);

};



#endif /* GBEZIEROBJECT_H_HEADER_INCLUDED_BC0A2377 */
