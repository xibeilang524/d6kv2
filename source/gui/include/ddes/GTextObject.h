#ifndef GTEXTOBJECT_H_HEADER_INCLUDED_BC7D2513
#define GTEXTOBJECT_H_HEADER_INCLUDED_BC7D2513
#include "GRectObject.h"
class GScaleFactor;
class GFile;

//##ModelId=4382CFAB00BB
//##Documentation
//## 定义：
//##        图元文本类
//## 功能：
//##        定义图元文本类
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GTextObject : public GRectObject
{
  public:
    //##ModelId=4382D14300CB
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
    GTextObject();

    //##ModelId=4382D14300CC
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GTextObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GTextObject(const GTextObject& copy);

    //##ModelId=4382D14300CE
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GTextObject&
    //## 返回：
    //##     const GTextObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GTextObject& operator=(const GTextObject& copy);

	//##ModelId=43A0C35201B5
    //##Documentation
    //## 功能：
    //##   设置内部实时值
    //## 参数：
    //##   const char* v
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-15
    virtual void setValue(const char* value);

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

    //##ModelId=4382D14300DE
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

    //##ModelId=4382D14300E0
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

    //##ModelId=4382D14300E2
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

    //##ModelId=4382D14300E4
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
    //##ModelId=4383112B0138
    //##Documentation
    //## 文本文字
    QString m_text;
  private:


};



#endif /* GTEXTOBJECT_H_HEADER_INCLUDED_BC7D2513 */
