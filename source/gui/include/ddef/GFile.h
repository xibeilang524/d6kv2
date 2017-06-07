#ifndef GFILE_H_HEADER_INCLUDED_BC8A4290
#define GFILE_H_HEADER_INCLUDED_BC8A4290
#include "ddes_comm.h"
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QByteArray>

class GRect;
class GPoint;

//##ModelId=4375A13B02AF
//##Documentation
//## 定义：
//##   图形文件存储基类类
//## 功能：  
//##   对文件句柄及内存句柄进行封装
//##   用于存储二进制格式文件
//##   存储字节序为LittleEndian
//##   如果为BigEndian的，则进行自动转换
//##   提供静态判断字节序的函数
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-2
class GDE_EXPORT_DEF GFile
{
  public:
    //##ModelId=4375A53D0399
    //##Documentation
    //## 随机读写搜索方式定义
    enum  { 
        //##Documentation
        //## 相对首位置寻址
        begin = SEEK_SET, 
        //##Documentation
        //## 相对当前位置寻址
        current = SEEK_CUR, 
        //##Documentation
        //## 相对尾位置寻址
        end = SEEK_END };

    //##ModelId=4375A5960531
    //##Documentation
    //## 文件打开方式
    enum  { 
        //##Documentation
        //## 只读方式
        modeRead = 0x01, 
        //##Documentation
        //## 只写方式
        modeWrite = 0x02, 
        //##Documentation
        //## 附加方式
        modeAppend = 0x04, 
        //##Documentation
        //## 读写方式
        modeReadWrite = 0x08, 
        //##Documentation
        //## 创建打开用于modeReadWrite组合使用
        modeCreate = 0x10, 
        //##Documentation
        //## 创建打开用于modeReadWrite组合使用
        modeTruncate = 0x20 };

    //##ModelId=4375A5A70186
    //##Documentation
    //## 文件打开类型
    enum  { 
        //##Documentation
        //## 文本方式
        typeText = 0x40, 
        //##Documentation
        //## 二进制方式
        typeBinary = 0x80 };

    //##ModelId=433FD67A000F
    //##Documentation
    //## 功能：
    //##      检测计算机内存字节序是否为BigEndian
    //## 参数：
    //##      无
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    static uint8 isBig();

    //##ModelId=434236C9036B
    //##Documentation
    //## 功能：
    //##   更改文件名称
    //## 参数：
    //##   const char* oldName //老名字
    //##   const char* newName //新名字
    //## 返回：（uint8）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    static uint8 rename(const char* oldName, const char* newName);

    //##ModelId=4375A67400BB
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

    //##ModelId=4375A6740157
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

    //##ModelId=4375A6740203
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

    //##ModelId=4375A67402AF
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

    //##ModelId=4375A674036B
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

    //##ModelId=4375A675005D
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

    //##ModelId=4375A6750128
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
    virtual uint8 isEOF() const = 0;

    //##ModelId=4375A67501F4
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
    virtual uint8 isOpen() const = 0;

    //##ModelId=4375A67502DE
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
    virtual uint8 seekBegin() = 0;

    //##ModelId=4375A67503A9
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
    virtual uint8 seekEnd() = 0;

    //##ModelId=4375A676009C
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
    virtual int32 getLength() = 0;

    //##ModelId=4375A6760167
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
    virtual uint8 setPos(int32 iPos, uint8 iFlag = GFile::begin) = 0;

    //##ModelId=4375A6760271
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
    virtual int32 getPos() = 0;

    //##ModelId=4375A676033C
    //##Documentation
    //## 功能：
    //##   保存当前文件读取位置
    //## 参数：
    //##   无
    //## 返回：（int32）
    //##   if success return file position
    //##   else return -1
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual int32 save();

    //##ModelId=4375A677002E
    //##Documentation
    //## 功能：
    //##   恢复文件记录位置
    //## 参数：
    //##   int32 savePos
    //## 返回：（uint8）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-4
    virtual uint8 restore(int32 savePos);

    //##ModelId=4375A6770109
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
    virtual char* readString(char* chBuffer, int32 chNum) = 0;

    //##ModelId=4375A67701D4
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
    virtual int32 writeString(const char* chBuffer) = 0;

    //##ModelId=4375A677029F
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
    virtual int32 read(void* buffer, int32 size, int32 size_num = 1) = 0;

    //##ModelId=4375A677037A
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
    virtual int32 write(void* buffer, int32 size, int32 size_num = 1) = 0;

    //##ModelId=4375A678006D
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      uint8 v 
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(uint8 v);

    //##ModelId=4375A6780148
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      char v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(char v);

    //##ModelId=4375A6780222
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      uint16 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(uint16 v);

    //##ModelId=4375A67802FD
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      int16 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(int16 v);

    //##ModelId=4375A67803D8
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      uint32 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(uint32 v);

    //##ModelId=4375A67900CB
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      int32 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(int32 v);

    //##ModelId=4375A67901B5
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      uint64 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(uint64 v);

    //##ModelId=4375A6790290
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      int64 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(int64 v);

    //##ModelId=4375A679036B
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      FLOAT4 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(FLOAT4 v);

    //##ModelId=4375A67A006D
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      FLOAT8 v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(FLOAT8 v);

    //##ModelId=4375A67A0157
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##       const GPoint& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(const GPoint& v);

    //##ModelId=4375A67A0232
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      const GRect& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(const GRect& v);

    //##ModelId=4375A67A030D
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      uint8& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(uint8& v);

    //##ModelId=4375A67B000F
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      char& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(char& v);

    //##ModelId=4375A67B00EA
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      uint16& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(uint16& v);

    //##ModelId=4375A67B01D4
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      int16& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(int16& v);

    //##ModelId=4375A67B02BF
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      uint32& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(uint32& v);

    //##ModelId=4375A67B03A9
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      int32& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(int32& v);

    //##ModelId=4375A67C00AB
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      uint64& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(uint64& v);

    //##ModelId=4375A67C0196
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      int64& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(int64& v);

    //##ModelId=4375A67C0271
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      FLOAT4& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(FLOAT4& v);

    //##ModelId=4375A67C036B
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      FLOAT8& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(FLOAT8& v);

    //##ModelId=4375A67D005D
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      GPoint& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(GPoint& v);

    //##ModelId=4375A67D0157
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      GRect& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(GRect& v);

    //##ModelId=4375A67D0242
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

    //##ModelId=4375A67D032C
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

    //##ModelId=4375A67E003E
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

    //##ModelId=4375A67E0128
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

    //##ModelId=4375A67E0222
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

    //##ModelId=4375A67E030D
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

    //##ModelId=4375A67F001F
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      QString& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator>>(QString& v);

    //##ModelId=4375A69E0148
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      const QString& v
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    GFile& operator<<(const QString& v);
    //##ModelId=4375A8B902DE
    //##Documentation
    //## 功能：
    //##      连接数据缓冲，该缓冲由用户自己释放
    //## 参数：
    //##      nt8 *dataBuffer;
    //##      uint32 dataSize ;
    //##      uint32 growSize//增长尺寸
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##     2005-11-11
    virtual uint8 attach(char* dataBuffer, uint32 dataSize, uint32 growSize);
	//##ModelId=4375BF66035B
    //##Documentation
    //## 功能：
    //##      写值到文件
    //## 参数：
    //##      void* value
    //##      int32 size
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-12
    GFile& writeV2F(void* value, int32 size);
    //##ModelId=4375BF7201D4
    //##Documentation
    //## 功能：
    //##      读值到文件
    //## 参数：
    //##      void* value
    //##      int32 size
    //## 返回：（GFile&）
    //##      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-12
    GFile& readF2V(void* value, int32 size);
	//##ModelId=437B202E02DE
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
    virtual void* getData() = 0;

};

#endif /* GFILE_H_HEADER_INCLUDED_BC8A4290 */
