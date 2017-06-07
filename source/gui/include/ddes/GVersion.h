#ifndef GVERSION_H_HEADER_INCLUDED_BCACDF1F
#define GVERSION_H_HEADER_INCLUDED_BCACDF1F

#include <QtCore/QString>
#include <QtCore/QObject>
#include "ddef/ddes_comm.h"

class GFile;

//##ModelId=43530F370167
//##Documentation
//## 定义：
//##   图形版本管理类
//## 功能：
//##   实现图形版本管理功能
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-17
class GDE_EXPORT_DES GVersion
{
  public:
    //##ModelId=43530FDF02AF
    //##Documentation
    //## 功能：
    //##   设置主版本号
    //## 参数：
    //##   uint8   h //高字节号
    //##   uint8   l//低字节号
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    void setMajor(uint8 h, uint8 l);
    //##ModelId=4353107D02BF
    //##Documentation
    //## 功能：
    //##   设置次版本号
    //## 参数：
    //##   uint8   h //高字节号
    //##   uint8   l //低字节号
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    void setMinor(uint8 h, uint8 l);
    //##ModelId=435310BE03B9
    //##Documentation
    //## 功能：
    //##   是否指定主版本号
    //## 参数：
    //##   uint8   h //高字节号
    //##   uint8   l //低字节号
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 isMajor(uint8 h, uint8 l) const ;
    //##ModelId=4353111F02DE
    //##Documentation
    //## 功能：
    //##   是否指定次版本号
    //## 参数：
    //##   uint8   h //高字节号
    //##   uint8   l //低字节号
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 isMinor(uint8 h, uint8 l) const ;
	//##ModelId=4353111F02DE
    //##Documentation
    //## 功能：
    //##   是否指版本号
    //## 参数：
    //##   uint8   mh //主版本高字节号
    //##   uint8   ml //主版本低字节号
	//##   uint8   nh //次版本高字节号
    //##   uint8   nl //次版本低字节号
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 isVers(uint8 mh, uint8 ml,uint8 nh,uint8 nl) const ;
    //##ModelId=43531154000F
    //##Documentation
    //## 功能：
    //##   取主版本号
    //## 参数：
    //##   uint8&   h //高字节号
    //##   uint8&   l //低字节号
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    void getMajor(uint8& h, uint8& l);
    //##ModelId=4353122502DE
    //##Documentation
    //## 功能：
    //##   取次版本号
    //## 参数：
    //##   uint8&   h //高字节号
    //##   uint8&   l //低字节号
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    void getMinor(uint8& h, uint8& l);
    //##ModelId=4353125402FD
    //##Documentation
    //## 功能：
    //##   判断版本号是否一致
    //## 参数：
    //##   const  GVersion& ver //版本号
    //## 返回：
    //##   if equal return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 operator==(const GVersion& ver) const;
    //##ModelId=4353125D03B9
    //##Documentation
    //## 功能：
    //##   判断版本号是否一致
    //## 参数：
    //##   const  GVersion& ver //版本号
    //## 返回：
    //##   if equal return 0
    //##   else return 1
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 operator!=(const  GVersion& ver) const;
    //##ModelId=435312CF02FD
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    GVersion();
    //##ModelId=435312F701F4
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   uint8 mh // major version h
    //##   uint8 ml // major version l
    //##   uint8 nh // minor version h 
    //##   uint8 nl // minor version l
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    GVersion(uint8 mh, uint8 ml, uint8 nh, uint8 nl);
    //##ModelId=4353139C00EA
    //##Documentation
    //## 功能：
    //##   取主版本号高字节
    //## 参数：
    //##   无
    //## 返回：
    //##   uint8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 getMH() const ;
    //##ModelId=435313BD00BB
    //##Documentation
    //## 功能：
    //##   取主版本号低字节
    //## 参数：
    //##   无
    //## 返回：
    //##   uint8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 getML() const;
    //##ModelId=435313D00232
    //##Documentation
    //## 功能：
    //##   取次版本号高字节
    //## 参数：
    //##   无
    //## 返回：
    //##   uint8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 getNH() const;
    //##ModelId=435313E00242
    //##Documentation
    //## 功能：
    //##   取次版本号低字节
    //## 参数：
    //##   无
    //## 返回：
    //##   uint8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint8 getNL() const;
    //##ModelId=43531437035B
    //##Documentation
    //## 功能：
    //##      写入函数
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return write real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-17
    int32 saveFile(GFile& fFile);


    //##ModelId=4353143902BF
    //##Documentation
    //## 功能：
    //##      读取函数
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return read real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-17
    int32 loadFile(GFile& fFile);
    //##ModelId=435315CA01E4
    //##Documentation
    //## 功能：
    //##   判断版本号是否一致
    //## 参数：
    //##   const GVersion& ver //版本号
    //## 返回：
    //##   const GVersion&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    const GVersion& operator=(const GVersion& ver);
    //##ModelId=435315F30157
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   onst GVersion& ver //版本号
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    GVersion(const GVersion& ver);
    //##ModelId=4353182702BF
    //##Documentation
    //## 功能：
    //##   取版本号
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    uint32 getVers() const;

	//##ModelId=4353182702BF
    //##Documentation
    //## 功能：
    //##   设置版本号
    //## 参数：
    //##   uint32 ver
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    void setVers(uint32 ver);
	//##ModelId=438C096C0251
    //##Documentation
    //## 功能：
    //##      转换函数
    //## 参数：
    //##      无
    //## 返回：
    //##      字符串
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-29
    QString toString();

  protected:
    //##ModelId=43530F9A02CE
    //##Documentation
    //## 主版本号高字节
    uint8 mvh;
    //##ModelId=43530FC60157
    //##Documentation
    //## 主版本号低字节
    uint8 mvl;
    //##ModelId=435311C1004E
    //##Documentation
    //## 次版本高字节
    uint8 nvh;
    //##ModelId=43531201031C
    //##Documentation
    //## 次版本低字节
    uint8 nvl;
  private:
};



#endif /* GVERSION_H_HEADER_INCLUDED_BCACDF1F */
