#ifndef GCARCOBJECT_H_HEADER_INCLUDED_BC02280A
#define GCARCOBJECT_H_HEADER_INCLUDED_BC02280A

#include "GEllipseObject.h"
class GScaleFactor;
class GFile;

//##ModelId=43FD9AFE02DE
//##Documentation
//## 定义：
//##        图元椭圆弧类
//## 功能：
//##        定义图元椭圆弧类
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GCArcObject : public GEllipseObject
{
  public:
    //##ModelId=43FD9B690148
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
    GCArcObject();

    //##ModelId=43FD9B690157
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GCArcObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GCArcObject(const GCArcObject& copy);

    //##ModelId=43FD9B690159
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GCArcObject&
    //## 返回：
    //##     const GCArcObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GCArcObject& operator=(const GCArcObject& copy);

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

    //##ModelId=43FD9B690177
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

    //##ModelId=43FD9B690179
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

    //##ModelId=43FD9B690187
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

    //##ModelId=43FD9B690196
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      QTextStream& stream
    //##      int32 indent
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);

	//##ModelId=4382D16401A0
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

    //##ModelId=4382D16401A4
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

	//##ModelId=4382D1640199
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

	//##ModelId=4382D164019C
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

    //##ModelId=43FD9BA9006D
    //##Documentation
    //## 起始角度
    FLOAT8 m_startAngle;

    //##ModelId=43FD9BDB0109
    //##Documentation
    //## 终止角度
    FLOAT8 m_endAngle;
};



#endif /* GCARCOBJECT_H_HEADER_INCLUDED_BC02280A */
