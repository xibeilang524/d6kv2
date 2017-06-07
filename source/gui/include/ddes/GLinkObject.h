#ifndef GLINKOBJECT_H_HEADER_INCLUDED_BC2BC6A2
#define GLINKOBJECT_H_HEADER_INCLUDED_BC2BC6A2
#include "GPolylineObject.h"
class GScaleFactor;

//##ModelId=43D42CE70128
//##Documentation
//## 定义：
//##        图元连接线
//## 功能：
//##        定义连接线
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GLinkObject : public GPolylineObject
{
  public:
    //##ModelId=43D42DB50186
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
    GLinkObject();

    //##ModelId=43D42DB50196
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GLinkObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GLinkObject(const GLinkObject& copy);

    //##ModelId=43D42DB50198
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GLinkObject&
    //## 返回：
    //##     const GLinkObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GLinkObject& operator=(const GLinkObject& copy);

    //##ModelId=43D42DB5019A
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

    //##ModelId=43D42F4000BB
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

    //##ModelId=43D42F4000BC
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
	//##ModelId=43D43D7A01E4
    //##Documentation
    //## 功能：
    //##   设置图元线风格
    //## 参数：  
    //##   const GLineStyle& style //线风格
    //## 返回：
    //##   void
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setLineStyle(const GLineStyle& style);

};



#endif /* GLINKOBJECT_H_HEADER_INCLUDED_BC2BC6A2 */
