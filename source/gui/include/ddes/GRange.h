#ifndef GRANGE_H_HEADER_INCLUDED_BC93C755
#define GRANGE_H_HEADER_INCLUDED_BC93C755
#include "ddef/ddes_comm.h"
class GFile;

//##ModelId=436C3DB1004E
//##Documentation
//## 定义：
//##        视野范围
//## 功能：
//##        定义图元视野范围
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
class GDE_EXPORT_DES GRange
{
  public:
    //##ModelId=436C3E2401E4
    //##Documentation
    //## 功能：
    //##   判断是否在范围内
    //## 参数：
    //##   FLOAT8 fRange//判断范围
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    uint8 in(
        //##Documentation
        //## 是否在范围内
        FLOAT8 fRange) const ;

    //##ModelId=436C3E5F0213
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
    GRange();

    //##ModelId=436C3E7C03D8
    //##Documentation
    //## 功能：
    //##   赋值函数
    //## 参数：
    //##   const GRange&
    //## 返回：
    //##   const GRange&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    const GRange& operator=(const GRange& copy);

    //##ModelId=436C3EC201C5
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   FLOAT8 fMin
    //##   FLOAT8 fMax
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    void setRange(FLOAT8 fMin, FLOAT8 fMax);

    //##ModelId=436C3EF90271
    //##Documentation
    //## 功能：
    //##   取得最大范围函数
    //## 参数：
    //##   无
    //## 返回：
    //##   FLOAT8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    FLOAT8 getMax() const;

    //##ModelId=436C3EFE0271
    //##Documentation
    //## 功能：
    //##   取最小函数
    //## 参数：
    //##   无
    //## 返回：
    //##   FLOAT8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    FLOAT8 getMin() const;

    //##ModelId=436C3F940232
    //##Documentation
    //## 功能：
    //##   拷贝构造函数
    //## 参数：
    //##   const GRange&
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GRange(const GRange& copy);
    //##ModelId=436C40700148
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


    //##ModelId=436C407201C5
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

    //##ModelId=436C3DDE02EE
    //## 最大视野范围
    //##Documentation
    //## 最大视野范围
    FLOAT8 maxRange;

    //##ModelId=436C3E0100FA
    //##Documentation
    //## 最小视野
    FLOAT8 minRange;

};



#endif /* GRANGE_H_HEADER_INCLUDED_BC93C755 */
