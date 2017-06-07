#ifndef GTEXTSTYLE_H_HEADER_INCLUDED_BC93F287
#define GTEXTSTYLE_H_HEADER_INCLUDED_BC93F287

#include <QtCore/QString>
#include "ddef/ddes_comm.h"

class GFile;

//##ModelId=436C3BF4002E
//##Documentation
//## 定义：
//##        文本风格
//## 功能：
//##        定义图元文本风格
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
class GDE_EXPORT_DES GTextStyle
{
  public:
    //##ModelId=436C3D3C0251
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

    //##ModelId=436C3D3E01B5
    //##ModelId=436C3D3E01B5
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
    
    //##ModelId=436C3FFE02CE
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
    GTextStyle();
    //##ModelId=436C400801C5
    //##Documentation
    //## 功能：
    //##   拷贝构造函数
    //## 参数：
    //##   const GTextStyle&
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GTextStyle(const GTextStyle& copy);
    //##ModelId=436C401E0222
    //##Documentation
    //## 功能：
    //##   赋值函数
    //## 参数：
    //##   const GTextStyle&
    //## 返回：
    //##   const GTextStyle&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    const GTextStyle& operator=(const GTextStyle& copy);



    //##ModelId=436C3C2E007D
    //##Documentation
    //## 文本颜色
    uint32 color;

    //##ModelId=436C3C40005D
    //##Documentation
    //## 背景色
    uint32 back;

    //##ModelId=436C3C620148
    //##Documentation
    //## 透明
    uint8 trans;

    //##ModelId=436C3C7C038A
    //##Documentation
    //## 字体风格 正常，粗体，斜体，阴影
    uint32 style;

    //##ModelId=436C3CFE0109
    //##Documentation
    //## 字体
    QString family;

    //##ModelId=436C3D0E0261
    //##Documentation
    //## 大小
    int16 size;
    //##ModelId=436DB28702FD
    //##Documentation
    //## 文本对齐格式
    uint32 align;
};


#endif /* GTEXTSTYLE_H_HEADER_INCLUDED_BC93F287 */
