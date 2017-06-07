#ifndef GFILLSTYLE_H_HEADER_INCLUDED_BC93CAEB
#define GFILLSTYLE_H_HEADER_INCLUDED_BC93CAEB

#include <QtCore/QString>

#include "ddef/ddes_comm.h"

class GFile;

//##ModelId=436C3A4A000F
//##Documentation
//## 定义：
//##        填充风格
//## 功能：
//##        定义图元填充风格
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
class GDE_EXPORT_DES GFillStyle
{
  public:
    //##ModelId=436C3BCE0213
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

    //##ModelId=436C3BD20148
    //##ModelId=436C3BD20148
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

    //##ModelId=436C3FBC00FA
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
    GFillStyle();
    //##ModelId=436C3FC200BB
    //##Documentation
    //## 功能：
    //##   拷贝构造函数
    //## 参数：
    //##   const GFillStyle&
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GFillStyle(const GFillStyle& copy);
    //##ModelId=436C3FD50148
    //##Documentation
    //## 功能：
    //##   赋值函数
    //## 参数：
    //##   const GFillStyle&
    //## 返回：
    //##   const GFillStyle&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    const GFillStyle& operator=(const GFillStyle& copy);



    //##ModelId=436C3A9C00AB
    //##Documentation
    //## 填充掩码
    //##     0x00＝透明显示
    //##     0x01＝填充颜色，存储颜色
    //##     0x02＝填充模式，存储模式
    //##     0x03＝填充图像，存储图像
	//##	 0x04= 光栅填充，存储光栅

    uint8 mask;

    //##ModelId=436C3AC30399
    //##Documentation
    //## 背景色
    uint32 back;

    //##ModelId=436C3AF0029F
    //##Documentation
    //## 前景色
    uint32 fore;

    //##ModelId=436C3B390109
    //##Documentation
    //## 填充模式
    uint8 pattern;

    //##ModelId=436C3B630119
    //##Documentation
    //## 背景图像源
    QString imgSrc;
    //##ModelId=436C621C01A5
    //##Documentation
    //## 背景模式
    //##    0x00=锁定
    //##    0x01=居中
    //##    0x02=平铺
    //##    0x03=拉伸
    uint32 imgMode;
	 //##ModelId=436D782F031C
    //##Documentation
    //## 光栅填充模式
    //##   低字
    //##    0x00=简单填充
    //##    0x01=双色渐变
    //##    0x02=双色复变
    //##    0x03=立体填充
    //##   高字
    //##    0x000000=横向
    //##    0x010000=纵向
	//##	0x020000=自动
    uint32 raster;
  private:

};

#endif /* GFILLSTYLE_H_HEADER_INCLUDED_BC93CAEB */
