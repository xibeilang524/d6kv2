#ifndef GBUSOBJECT_H_HEADER_INCLUDED_BC2B9C4C
#define GBUSOBJECT_H_HEADER_INCLUDED_BC2B9C4C

#include "GRectObject.h"
class GScaleFactor;

//##ModelId=43D42C8B03B9
//##Documentation
//## 定义：
//##        图元母线
//## 功能：
//##        定义母线
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GBusObject : public GRectObject
{
  public:
    //##ModelId=43D42F460148
    //##Documentation
    //## 功能：
    //##   判断是否允许粘帖
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
    virtual uint8 canSnap() const;

    //##ModelId=43D42F460157
    //##Documentation
    //## 功能：
    //##   判断指定点是否位于粘帖范围
    //## 参数：  
    //##   const GPoint& pt
    //## 返回：
    //##   uint8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    virtual uint8 hitSnap(const GPoint& pt);

    //##ModelId=43D42F610109
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
    GBusObject();

    //##ModelId=43D42F61010A
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GBusObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GBusObject(const GBusObject& copy);

    //##ModelId=43D42F61010C
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GBusObject&
    //## 返回：
    //##     const GBusObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GBusObject& operator=(const GBusObject& copy);
	//##ModelId=43D435480148
    //##Documentation
    //## 功能：
    //##   设置图元填充风格操作
    //## 参数：  
    //##   const GFillStyle& //填充风格
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setFillStyle(const GFillStyle& style);

    //##ModelId=43D42F610119
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC //绘制环境
    //##   GScaleFactor* pSF//缩放因子
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual uint8 drawObj(GDC* pDC, GScaleFactor* pSF = NULL);

};



#endif /* GBUSOBJECT_H_HEADER_INCLUDED_BC2B9C4C */
