#ifndef GDEVFILE_H_HEADER_INCLUDED_BC8A1093
#define GDEVFILE_H_HEADER_INCLUDED_BC8A1093
#include <QtCore/QString>
#include <QtCore/QObject>
#include "ddes_comm.h"
#include "GFile.h"

//##ModelId=433FC90C00BB
//##Documentation
//## 定义：
//##   图形文件存储类
//## 功能：  
//##   对文件句柄进行封装
//##   用于存储二进制格式文件
//##   存储字节序为LittleEndian
//##   如果为BigEndian的，则进行自动转换
//##   提供静态判断字节序的函数
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-2
class GDE_EXPORT_DEF GDevFile : public GFile
{
  public:
    //##ModelId=433FCF3F03B9
    //##Documentation
    //## 功能：
    //##       构造空文件
    //## 参数：
    //##      无
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##    2005-10-2
    GDevFile();

    //##ModelId=433FCF45008C
    //##Documentation
    //## 功能：
    //##       构造文件
    //## 参数：
    //##      const char* fName//文件名称：可以包含路径
    //##      uint32 uOpenMode //打开模式
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##    2005-10-2
    GDevFile(const char* fName, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=433FD00F0000
    //##Documentation
    //## 功能：
    //##      构造文件
    //## 参数：
    //##      int32  fDesc //文件描述符
    //##      uint32 uOpenMode//打开方式
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##    2005-10-2
    GDevFile(int32 fDesc, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=433FD11F0157
    //##Documentation
    //## 功能：
    //##       构造文件
    //## 参数：
    //##      FILE* fFile//已经打开的文件
    //## 返回：
    //##      无
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
    GDevFile(FILE* fFile);

    //##ModelId=433FD19702AF
    //##Documentation
    //## 功能：
    //##       构造文件
    //## 参数：
    //##      int32 fDesc //文件描述符
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##    2005-10-2
    GDevFile(int32 fDesc);

    //##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    析构文件
    //## 参数：
    //##    无
    //## 返回：
    //##    无
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
    virtual ~GDevFile();

    //##ModelId=433FD24C0213
    //##Documentation
    //## 功能：
    //##   打开文件
    //## 参数：
    //##   const char* fName//文件名称：可以包含路径
    //##   uint32 uOpenMode //打开模式
    //## 返回：（uint8）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual uint8 open(const char* fName, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=433FD32C008C
    //##Documentation
    //## 功能：
    //##   打开文件
    //## 参数：
    //##   int32 fDesc//文件描述符
    //##   uint32 uOpenMode //打开模式
    //## 返回：（uint8）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual uint8 open(int32 fDesc, uint32 uOpenMode = GFile::modeRead|GFile::typeBinary);

    //##ModelId=433FD3B800FA
    //##Documentation
    //## 功能：
    //##   关闭文件
    //## 参数：
    //##   无
    //## 返回：（uint8）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual uint8 close();

    //##ModelId=433FD4BB02FD
    //##Documentation
    //## 功能：
    //##      连接文件句柄
    //## 参数：
    //##      FILE* fFile //文件句柄
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 attach(FILE* fFile);

    //##ModelId=433FD567037A
    //##Documentation
    //## 功能：
    //##      连接文件描述符
    //## 参数：
    //##      int32 fDesc //文件描述符
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 attach(int32 fDesc);

    //##ModelId=433FD5C002BF
    //##Documentation
    //## 功能：
    //##      返回文件句柄
    //## 参数：
    //##      无
    //## 返回：
    //##      if success return m_fFile
    //##      else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual void* detach();

    //##ModelId=433FD60E006D
    //##Documentation
    //## 功能：
    //##      检测文件是否结尾
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isEOF() const;

    //##ModelId=433FD614038A
    //##Documentation
    //## 功能：
    //##      检测文件是否打开
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isOpen() const;

    //##ModelId=433FD700033C
    //##Documentation
    //## 功能：
    //##      移动文件到文件起始位置
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 seekBegin();

    //##ModelId=433FD736029F
    //##Documentation
    //## 功能：
    //##      移动文件到文件末尾位置
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 seekEnd();

    //##ModelId=433FD77002CE
    //##Documentation
    //## 功能：
    //##      计算文件长度
    //## 参数：
    //##      无
    //## 返回：（int32）
    //##      if success return filesize
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 getLength();

    //##ModelId=4342485D03A9
    //##Documentation
    //## 功能：
    //##   设置当前文件位置
    //## 参数： 
    //##   int32 iPos //位置
    //##   uint8  iFlag//标志 
    //## 返回：（uint8）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual uint8 setPos(int32 iPos, uint8 iFlag = GFile::begin);

    //##ModelId=433FD7B702EE
    //##Documentation
    //## 功能：
    //##      取得文件当前位置
    //## 参数：
    //##      无
    //## 返回：（int32）
    //##      if success return curr position
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 getPos();

    //##ModelId=433FD871033C
    //##Documentation
    //## 功能：
    //##     读取文件字符串
    //## 参数：
    //##      char* chBuffer//接收字符串
    //##      int32 chNum   //字符串个数
    //## 返回：（int32）
    //##      if success return 实际读取数
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual char* readString(char* chBuffer, int32 chNum);

    //##ModelId=433FD9590157
    //##Documentation
    //## 功能：
    //##      写入字符串
    //## 参数：
    //##      const char* chBuffer//字符串缓冲区
    //## 返回：（int32）
    //##      if success return real write size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 writeString(const char* chBuffer);

    //##ModelId=433FDAB201B5
    //##Documentation
    //## 功能：
    //##      二进制分块读取内存区域
    //## 参数：
    //##      void* buffer //读取缓冲区
    //##      int32 size   //每块尺寸
    //##      int32 size_num//块数
    //## 返回：（int32）
    //##      if success return 实际读入块数
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 read(void* buffer, int32 size, int32 size_num = 1);

    //##ModelId=433FDABB0000
    //##Documentation
    //## 功能：
    //##      二进制分块写入内存区域
    //## 参数：
    //##      void* buffer //写入缓冲区
    //##      int32 size   //每块尺寸
    //##      int32 size_num//块数
    //## 返回：（int32）
    //##      if success return 实际写入块数
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 write(void* buffer, int32 size, int32 size_num = 1);

    //##ModelId=434BD3E60049
    //##Documentation
    //## 功能：
    //##      检测当前文件是否可以读
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isRead();

    //##ModelId=434BD4150123
    //##Documentation
    //## 功能：
    //##      检测当前文件是否可以写
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isWrite();

    //##ModelId=434BD42901DF
    //##Documentation
    //## 功能：
    //##      检测当前文件为链接
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isAttach();

    //##ModelId=434BD46E01CF
    //##Documentation
    //## 功能：
    //##      检测当前文件是否可以读写
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isReadWrite();

    //##ModelId=434BD4810152
    //##Documentation
    //## 功能：
    //##      检测当前文件模式是否为文本方式
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-12
    virtual uint8 isText();

    //##ModelId=434BD4A503A4
    //##Documentation
    //## 功能：
    //##      检测当前文件模式是否为二进制方式
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-12
    virtual uint8 isBinary();

    //##ModelId=435C615802CE
    //##Documentation
    //## 功能：
    //##    获取文件路径名称
    //## 参数：
    //##    无
    //## 返回：
    //##    返回文件名称
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-24
    virtual const QString& getFileName() const;
	//##ModelId=437B21430222
    //##Documentation
    //## 功能：
    //##      取得内部数据，但不断开连接
    //##      如果是该对象打开或申请的内存
    //##      该对象会自动释放，这一点和detach不同
    //##      detach后，该对象失去联系，不对对象
    //##      内部数据进行释放，需要用户自己释放
    //##      否则有内存泄漏
    //## 参数：
    //##      无
    //## 返回：
    //##      void*
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-12
    virtual void* getData();

  protected:
    //##ModelId=434BC9FB009B
    //##Documentation
    //## 功能：
    //##   初始化成员变量
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-11
    virtual void init();

    //##ModelId=433FCE4702AF
    //##Documentation
    //## 文件句柄
    FILE* m_fFile;

    //##ModelId=433FCE730000
    //##Documentation
    //## 文件打开标志，打开为1，否则为0
    uint8 m_uFileOpen;

    //##ModelId=433FCEDE0109
    //##Documentation
    //## 打开模式
    uint32 m_uOpenMode;

    //##ModelId=434BC95A0261
    //##Documentation
    //## 链接标志，仅仅是链接，析构时不释放句柄
    uint8 m_uAttach;

    //##ModelId=435C613603B9
    //##Documentation
    //## 文件名称，保留Open文件名称
    QString m_fileName;

};

#endif /* GDEVFILE_H_HEADER_INCLUDED_BC8A1093 */
