#ifndef GIMAGEOBJECT_H_HEADER_INCLUDED_BC7D576D
#define GIMAGEOBJECT_H_HEADER_INCLUDED_BC7D576D
#include "GBaseObject.h"
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "GRectObject.h"

class GScaleFactor;
class GFile;

//##ModelId=4382CF9F0261
//##Documentation
//## 定义：
//##        图元图像类
//## 功能：
//##        定义图像类
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GImageObject : public GRectObject
{
  public:
    //##ModelId=4382D154031C
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
    GImageObject();

    //##ModelId=4382D154031D
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GImageObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GImageObject(const GImageObject& copy);

    //##ModelId=4382D154031F
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GImageObject&
    //## 返回：
    //##     const GImageObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GImageObject& operator=(const GImageObject& copy);	

    //##ModelId=4382D1540321
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

    //##ModelId=4382D1540325
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

    //##ModelId=4382D1540327
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

    //##ModelId=4382D1540329
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

    //##ModelId=4382D154032B
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
    //##ModelId=438310310242
    //##Documentation
    //## 图像源
    QString m_imgsrc;
    //##ModelId=4383104B01B5
    //##Documentation
    //## 图像模式
    uint32 m_imgmode;
    //##ModelId=4383106701C5
    //##Documentation
    //## 是否透明
    uint8 m_trans;
    //##ModelId=4383108C0222
    //##Documentation
    //## 颜色掩码
    uint32 m_clrmask;
  private:





};



#endif /* GIMAGEOBJECT_H_HEADER_INCLUDED_BC7D576D */
