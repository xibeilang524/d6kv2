#ifndef GLIBMANAGER_H_HEADER_INCLUDED_BC052778
#define GLIBMANAGER_H_HEADER_INCLUDED_BC052778
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
class GFile;
class GBaseObject;

//##ModelId=43FACD920251
class GDE_EXPORT_DES GLibManager : public GGraphLayer
{
  public:
    //##ModelId=43FACDBA0232
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
    GLibManager();

    //##ModelId=43FACDBA0242
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
    virtual ~GLibManager();

    //##ModelId=43FACDBA0243
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

    //##ModelId=43FACDBA0245
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

    //##ModelId=43FACDBA0251
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

    //##ModelId=43FACDBA0253
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

    //##ModelId=43FACDBA0255
    //##Documentation
    //## 功能：
    //##      获取指定位置图元组件
    //## 参数：
    //##      uint nindex
    //## 返回：
    //##      GBaseObject*
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-16
    GBaseObject* findIndex(uint32 nindex = 0) const;

	//##ModelId=43FACDBA0255
    //##Documentation
    //## 功能：
    //##      获取指名称图元组件
    //## 参数：
    //##      const char* name
    //## 返回：
    //##      GBaseObject*
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-16
    GBaseObject* findName(const char* name = 0) const;

    //##ModelId=43FACEF002CE
    //##Documentation
    //## 功能：
    //##      获取是否为符号库
    //## 参数：
    //##      无
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-16
    uint8 isSymboLib() const;
	//##ModelId=4372FEA00167
    //##Documentation
    //## 功能：
    //##   生成最大标识
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-10
    virtual uint32 maxID();
	//##ModelId=4372FEA00167
    //##Documentation
    //## 功能：
    //##   重新设置标识
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-10
	void	resetID();

    //##ModelId=43FACDDF0119
    //##Documentation
    //## 图形文件标志{GDL}
    char m_gFlag[4];
	//##ModelId=4372FE6F0138
    //##Documentation
    //## 当前最大标识
    uint32 m_maxID;
};



#endif /* GLIBMANAGER_H_HEADER_INCLUDED_BC052778 */
