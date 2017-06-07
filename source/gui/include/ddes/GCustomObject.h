#ifndef GCUSTOMOBJECT_H_HEADER_INCLUDED_BC5DBFB4
#define GCUSTOMOBJECT_H_HEADER_INCLUDED_BC5DBFB4
#include "GComObject.h"

class GScaleFactor;
class GFile;
class GBaseObject;

//##ModelId=43A2103A0109
//##Documentation
//## 定义：
//##        自定义图元类
//## 功能：
//##        具有图元基本属性图元
//##        类型为GD_TYPE_CUSTOM
//##        有（组合图元）组成
//## 设计：
//##       宋永生
//## 日期：
//##       2005-12-16
class GDE_EXPORT_DES GCustomObject : public GComObject
{
  public:
    //##ModelId=43A211790261
    //##Documentation
    //## 功能：
    //##       构造/析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-3
    GCustomObject();

    //##ModelId=43A211790271
    //##Documentation
    //## 功能：
    //##       构造/析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-3
    virtual ~GCustomObject();

    //##ModelId=43A211790272
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      GFile& fFile //文件
    //## 返回：（int32 ）
    //##      if success return write real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=43A211790274
    //##Documentation
    //## 功能：
    //##      重载读取函数，读取所有数据包含父类
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return read real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadFile(GFile& fFile);

	//##ModelId=43A0C35201B5
    //##Documentation
    //## 功能：
    //##   设置内部实时值
    //## 参数：
    //##   FLOAT8 v
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-15
    virtual void setValue(FLOAT8 value);

    //##ModelId=43A211790280
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
	
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   设置行为
    //## 参数：
    //##   LPDA dynAct
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual void setAct(LPDA dynAct);
	virtual void addAct( LPDA dynAct );
	virtual void rmvAct( LPDA dynAct );
	virtual void clrAct();

    //##ModelId=43A211790283
    //##Documentation
    //## 功能：
    //##   计算图元矩形
    //## 参数：  
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void calcRect();

    //##ModelId=43A211790284
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 readXML(QDomElement& dom);

    //##ModelId=43A211790290
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);

    //##ModelId=43A2117902CE
    //##Documentation
    //## 功能：
    //##   X,Y偏移量
    //## 参数：
    //##   FLOAT8 x
    //##   FLOAT8 y
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void offsetXY(FLOAT8 x, FLOAT8 y, uint32 flags = 0);

    //##ModelId=43A2117902D2
    //##Documentation
    //## 功能：
    //##   X,Y调整偏移量
    //## 参数：
    //##   const GPoint& pt
    //##   FLOAT8 x
    //##   FLOAT8 y
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void resizeXY(const GPoint& pt, FLOAT8 x, FLOAT8 y, uint32 flags = 0);

    //##ModelId=43A2117902D7
    //##Documentation
    //## 功能：
    //##   对图元进行镜像处理
    //## 参数：
    //##   uint32 op
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-15
    virtual void mirrorXY(uint32 op, uint32 flags = 0);

    //##ModelId=43A2117902DE
    //##Documentation
    //## 功能：
    //##   以指定原点对图元进行镜像处理
    //## 参数：
    //##   const GPoint& pto
    //##   uint32 op
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-15
    virtual void mirrorXY(const GPoint& pto, uint32 op, uint32 flags = 0);

    //##ModelId=43A2117902E2
    //##Documentation
    //## 功能：
    //##   按图元内部旋转中心点旋转th度
    //## 参数：
    //##   FLOAT8 th
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void rotateXY(FLOAT8 th, uint32 flags = 0);

    //##ModelId=43A2117902EE
    //##Documentation
    //## 功能：
    //##   按指定旋转中心点ptc旋转th度
    //## 参数：
    //##   const GPoint& ptc
    //##   FLOAT8 th
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void rotateXY(const GPoint& ptc, FLOAT8 th, uint32 flags = 0);

    //##ModelId=43A2117902F2
    //##Documentation
    //## 功能：
    //##   鼠标进入操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual int32 onMouseEnter(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A2117902FD
    //##Documentation
    //## 功能：
    //##   鼠标在对象中移动操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onMouseMove(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179030D
    //##Documentation
    //## 功能：
    //##   鼠标离开操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    virtual int32 onMouseLeave(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A211790311
    //##Documentation
    //## 功能：
    //##   鼠标点击左键弹起操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onLButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179031C
    //##Documentation
    //## 功能：
    //##   鼠标点击左键操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onLButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A211790320
    //##Documentation
    //## 功能：
    //##   鼠标点击右键弹起操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onRButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179032C
    //##Documentation
    //## 功能：
    //##   鼠标点击右键操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onRButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);

    //##ModelId=43A21179033C
    //##Documentation
    //## 功能：
    //##   提供对象缩放计算，无返回，直接改变绘制对象
    //##   默认忽略，不要在该函数中调用pSF->scaleObject
    //## 参数： 
    //##   GScaleFactor *pSF //缩放因子
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    virtual void scaleFactor(GScaleFactor* pSF);
	//##ModelId=43A6064003B9
    //##Documentation
    //## 功能：
    //##   重新设置端子属主ID
    //## 参数：
    //##   reset 
	//##	 if =0 重置OwnerID
	//##	 else  清除OwnerID
	//##   nID  
	//##	 if =-1 当前ID为子端子的属主
	//##	 else	指定ID为属主
	//##   pObj
	//##	 if =NULL 当前对象为子端子的属主
	//##	 else	  指定对象为属主	
    //## 返回：
    //##   if is return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
	virtual void setPortOwner(uint8 reset=0,uint32 nID=(uint32)-1,\
		GBaseObject* pObj=NULL);

    //##ModelId=43A21179033E
    //##Documentation
    //## 功能：
    //##   图元对齐操作,根据指定功能对齐图元
    //## 参数：
    //##   uint32 op
    //##   FLOAT8 v
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void alignPos(uint32 op, FLOAT8 v);

    //##ModelId=43A21179034B
    //##Documentation
    //## 功能：
    //##   图元等大小操作,根据指定功能等大小图元
    //## 参数：
    //##   uint32 op
    //##   FLOAT8 w
    //##   FLOAT8 h
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void sameSize(uint32 op, FLOAT8 w, FLOAT8 h);   
    //##ModelId=43A2122E008C
    //##Documentation
    //## 功能：
    //##      获取指定状态图元组件
    //## 参数：
    //##      FLOAT8 vstatus
    //## 返回：
    //##      GBaseObject*
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-16
    GBaseObject* findObj(FLOAT8 vstatus = 0) const;
	    //##ModelId=43D314070196
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
    //##ModelId=43D31407037A
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
    //##ModelId=43D31408005D
    //##Documentation
    //## 功能：
    //##      设置图元文本风格信息
    //## 参数：  
    //##     const GTextStyle&
    //## 返回：
    //##    无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    virtual void setTextStyle(const GTextStyle& style);
    //##ModelId=43D431F6035B
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
    //##ModelId=43D431F7001F
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

	//##ModelId=43A211A4033C
    //##Documentation
    //## 功能：
    //##   获取链接图元序列
    //## 参数：
    //##   libPath 库名
	//##   libID   标识
    //## 返回：
    //##   if success return 1
	//##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-17
	uint8 loadElems(const char* libPath,uint32 libID);
};



#endif /* GCUSTOMOBJECT_H_HEADER_INCLUDED_BC5DBFB4 */
