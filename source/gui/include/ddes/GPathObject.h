#ifndef GPATHOBJECT_H_HEADER_INCLUDED_BB69AAF5
#define GPATHOBJECT_H_HEADER_INCLUDED_BB69AAF5

#include "gcommobjs.h"
#include "GPolylineObject.h"

class GScaleFactor;

//##ModelId=44960A13000F
//##Documentation
//## 定义：
//##        路径线
//## 功能：
//##        定义路径线
//## 设计：
//##        宋永生
//## 日期：
//##       2006-06-19
class GDE_EXPORT_DES GPathObject : public GPolylineObject
{
  public:
    //##ModelId=44960A7B00AB
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
    GPathObject();
    //##ModelId=44960A7B0119
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GPathObject&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GPathObject(const GPathObject& copy);
	
    //##ModelId=44960A7B01F4
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GPathObject&
    //## 返回：
    //##     const GPathObject&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GPathObject& operator=(const GPathObject& copy);

    //##ModelId=44960A7B0271
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

	 //##ModelId=4496268103B9
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
    //##ModelId=44962682009C
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
    //##ModelId=449626820109
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
	//##ModelId=4382D138007A
    //##Documentation
    //## 功能：
    //##      读取点集函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    uint8 readCodes(QDomElement& dom);
    //##ModelId=449626820177
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
	//##ModelId=4382D138007C
    //##Documentation
    //## 功能：
    //##      保存点集函数
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
	uint8 saveCodes(QDomElement& dom);
	//##ModelId=43A60320004E
    //##Documentation
    //## 功能：
    //##   绘制时需要多点控制码
	//##   用于插件图元绘制
    //## 参数：
    //##   无
    //## 返回：
    //##   if 多点 return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
	virtual void  setCodes(const GListUINT8& codes);
	
	GListUINT8	m_codes;
};



#endif /* GPATHOBJECT_H_HEADER_INCLUDED_BB69AAF5 */
