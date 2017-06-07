#ifndef GBUTTONOBJECT_H_HEADER_INCLUDED_BC7D0C57
#define GBUTTONOBJECT_H_HEADER_INCLUDED_BC7D0C57

#include "gcommobjs.h"
#include "GRectObject.h"

class GScaleFactor;
class GFile;

//##ModelId=4382CF8202EE
//##Documentation
//## 定义：
//##        图元按钮类
//## 功能：
//##        定义图元按钮
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GButtonObject : public GRectObject
{
  public:
    //##ModelId=4382D17200DA
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
    GButtonObject();

    //##ModelId=4382D17200DB
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GButtonObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GButtonObject(const GButtonObject& copy);

    //##ModelId=4382D17200DD
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GButtonObject&
    //## 返回：
    //##     const GButtonObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GButtonObject& operator=(const GButtonObject& copy);
    
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

    //##ModelId=4382D17200E3
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

    //##ModelId=4382D17200E5
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

    //##ModelId=4382D17200EA
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

	//##ModelId=4382D17200EA
    //##Documentation
    //## 功能：
    //##      读取显示层函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
	uint8 readLayers(QDomElement& dom);

    //##ModelId=4382D17200EC
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

	//##ModelId=4382D17200EC
    //##Documentation
    //## 功能：
    //##      保存显示层函数
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
	uint8 saveLayers(QDomElement& dom);
	//##ModelId=43A0F670035B
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
    //##ModelId=43A0F671004E
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
    //##ModelId=438313AD02DE
    //##Documentation
    //## 命令掩码
    uint32 m_cmdMask;
    //##ModelId=438313F00119
    //##Documentation
    //## 命令串
    QString m_cmdString;
	//##ModelId=43A21F0B009C
    //##Documentation
    //## 按钮标签
    QString m_lblString;
	//##ModelId=43A21F0B009C
    //##Documentation
	//## 允许显示的图层列表
	GListINT32S m_layers;
  protected:
    //##ModelId=4383140F0128
    //##Documentation
    //## 按钮风格
    uint32 m_style;

};



#endif /* GBUTTONOBJECT_H_HEADER_INCLUDED_BC7D0C57 */
