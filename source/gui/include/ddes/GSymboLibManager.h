#ifndef GSYMBOLIBMANAGER_H_HEADER_INCLUDED_BC5DE299
#define GSYMBOLIBMANAGER_H_HEADER_INCLUDED_BC5DE299
#include "GGraphLayer.h"
#include "GLibManager.h"
#include "ddef/ddes_comm.h"
class GFile;

//##ModelId=43A1052001E4
class GDE_EXPORT_DES GSymboLibManager : public GLibManager
{
  public:
    //##ModelId=43A105D70242
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      const char* strFile
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(const char* strFile);
    //##ModelId=43A105D70290
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      const char* strFile
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 readXML(const char* strFile);
    //##ModelId=43A1069E0148
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return write real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);
    //##ModelId=43A1069E0196
    //##Documentation
    //## 功能：
    //##      重载读取函数
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
    //##ModelId=43A107000128
    //##Documentation
    //## 功能：
    //##       构造函数   
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-12-16
    GSymboLibManager();
    //##ModelId=43A1072F003E
    //##Documentation
    //## 功能：
    //##       析构函数   
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-12-16
    virtual ~GSymboLibManager();

	//指定文件是否为图符文件
	static uint8 isGraphLib(const char* strPath) ;
};



#endif /* GGRAPHLIBMANAGER_H_HEADER_INCLUDED_BC5EE1DE */
