#ifndef GLINESTYLE_H_HEADER_INCLUDED_BC93FEE6
#define GLINESTYLE_H_HEADER_INCLUDED_BC93FEE6
#include "ddef/ddes_comm.h"
class GFile;

//##ModelId=436C37E60000
//##Documentation
//## 定义：
//##        线风格
//## 功能：
//##        定义图元线型风格
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
class GDE_EXPORT_DES GLineStyle
{
  public:
    //##ModelId=436C3A17034B
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

    //##ModelId=436C3A1A0119
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
    //##ModelId=436C3F36005D
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
    //##   2005-11-5
    GLineStyle();

    //##ModelId=436C3F42037A
    //##Documentation
    //## 功能：
    //##   拷贝构造函数
    //## 参数：
    //##   const GLineStyle&
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GLineStyle(const GLineStyle& copy);

    //##ModelId=436C3F6402CE
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   const GLineStyle&
    //## 返回：
    //##   const GLineStyle&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    const GLineStyle& operator=(const GLineStyle& copy);

    //##ModelId=436C390F0290
    //##Documentation
    //## 线风格
    uint8 style;

    //##ModelId=436C395B00BB
    //##Documentation
    //## 线宽
    int16 width;

    //##ModelId=436C397E0242
    //##Documentation
    //## 线颜色
    uint32 color;

    //##ModelId=436C399A0290
    //##Documentation
    //## 左箭头－高字节为尺寸，低字节为风格
    uint32 arwLeft;

    //##ModelId=436C39C50167
    //##Documentation
    //## 右箭头－高字节为尺寸，低字节为风格
    uint32 arwRight;
  protected:

};

#endif /* GLINESTYLE_H_HEADER_INCLUDED_BC93FEE6 */
