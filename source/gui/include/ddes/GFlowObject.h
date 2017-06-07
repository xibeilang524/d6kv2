#ifndef GFLOWOBJECT_H_HEADER_INCLUDED_BC2BBC30
#define GFLOWOBJECT_H_HEADER_INCLUDED_BC2BBC30
#include "GPolylineObject.h"
class GScaleFactor;

//##ModelId=43D42CC40148
//##Documentation
//## 定义：
//##        图元潮流线
//## 功能：
//##        定义潮流线
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GFlowObject : public GPolylineObject
{
  public:
    //##ModelId=43D42D5E032C
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
    GFlowObject();

    //##ModelId=43D42D5E032D
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GFlowObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GFlowObject(const GFlowObject& copy);

    //##ModelId=43D42D5E032F
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GFlowObject&
    //## 返回：
    //##     const GFlowObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GFlowObject& operator=(const GFlowObject& copy);

    //##ModelId=43D42D5E033C
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
	//##ModelId=43D43D85036B
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

	//改变起始点或终止点
	GPoint  m_pts ;
	FLOAT8	m_lastValue;

};



#endif /* GFLOWOBJECT_H_HEADER_INCLUDED_BC2BBC30 */
