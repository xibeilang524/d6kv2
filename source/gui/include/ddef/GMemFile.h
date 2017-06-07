#ifndef GMEMFILE_H_HEADER_INCLUDED_BC8A5FAC
#define GMEMFILE_H_HEADER_INCLUDED_BC8A5FAC
#include "ddes_comm.h"
#include "GFile.h"
class GString;

//##ModelId=437571BD00CB
//##Documentation
//## 定义：
//##   图形内存存储类
//## 功能：  
//##   对内存句柄进行封装
//##   用于存储二进制格式文件
//##   存储字节序为LittleEndian
//##   如果为BigEndian的，则进行自动转换
//##   提供静态判断字节序的函数
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-2
class GDE_EXPORT_DEF GMemFile : public GFile
{
  public:
    //##ModelId=43757309036B
    //##Documentation
    //## 功能：
    //##      内存文件构造
    //## 参数：
    //##      uint32 growSize//增长尺寸
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##     2005-11-11
    GMemFile(uint32 growSize = 1024);

    //##ModelId=437573730157
    //##Documentation
    //## 功能：
    //##      内存文件构造
    //## 参数：
    //##      char *dataBuffer;
    //##      uint32 dataSize ;
    //##      uint32 growSize ;//增长尺寸
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##     2005-11-11
    GMemFile(char* dataBuffer, uint32 dataSize, uint32 growSize = 0);
	//##ModelId=437575390109
    //##Documentation
    //## 功能：
    //##      内存文件析构
    //## 参数：
    //##      无
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##     2005-11-11
    virtual ~GMemFile();


    //##ModelId=437573E60290
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
    uint8 attach(char* dataBuffer, uint32 dataSize, uint32 growSize);
	 //##ModelId=4379C8F80232
    //##Documentation
    //## 功能：
    //##      返回缓存区域
    //## 参数：
    //##      无
    //## 返回：
    //##      if success return databuf
    //##      else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    void* detach();
	//##ModelId=437B211A037A
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

    
    //##ModelId=43757636032C
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

    //##ModelId=4375763E0119
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

    //##ModelId=4375764502AF
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

    //##ModelId=4375764B0271
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

    //##ModelId=437576630138
    //##Documentation
    //## 功能：
    //##   设置当前文件位置
    //## 参数： 
    //##   int32 iPos //位置
    //##   char  iFlag//标志 
    //## 返回：（char）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual uint8 setPos(int32 iPos, uint8 iFlag = GFile::begin);

    //##ModelId=4375766E0251
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

    //##ModelId=4375767C01A5
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

    //##ModelId=4375768801B5
    //##Documentation
    //## 功能：
    //##      移动文件到文件末尾位置
    //## 参数：
    //##      无
    //## 返回：（char）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 seekEnd();

    //##ModelId=4375768E0261
    //##Documentation
    //## 功能：
    //##      移动文件到文件起始位置
    //## 参数：
    //##      无
    //## 返回：（char）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 seekBegin();

    //##ModelId=4375769502EE
    //##Documentation
    //## 功能：
    //##      检测文件是否打开
    //## 参数：
    //##      无
    //## 返回：（char）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isOpen() const;

    //##ModelId=437576A5033C
    //##Documentation
    //## 功能：
    //##      检测文件是否结尾
    //## 参数：
    //##      无
    //## 返回：（char）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 isEOF() const;

    //##ModelId=437576AF02BF
    //##Documentation
    //## 功能：
    //##   关闭文件
    //## 参数：
    //##   无
    //## 返回：（char）
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    virtual uint8 close();



  protected:
    //##ModelId=437575340000
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

    //##ModelId=43758DCE0290
    //##Documentation
    //## 功能：
    //##   申请内存
    //## 参数：
    //##   uint32 size
    //## 返回：
    //##   if success return char*
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    char* gmalloc(uint32 size);

    //##ModelId=43758E210242
    //##Documentation
    //## 功能：
    //##   调整内存
    //## 参数：
    //##   char*  ptr
    //##   uint32 size
    //## 返回：
    //##   if success return char*
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    char* grealloc(char* ptr, uint32 size);

    //##ModelId=43758E550148
    //##Documentation
    //## 功能：
    //##   赋值内存块
    //## 参数：
    //##   char*  memDst
    //##   const char*  memSrc
    //##   uint32 size
    //## 返回：
    //##   if success return char*
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    void gmemcpy(char* memDst, const char* memSrc, uint32 size);

    //##ModelId=43758F1D01C5
    //##Documentation
    //## 功能：
    //##   是否内存
    //## 参数：
    //##   char* ptr
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    void gfree(char* ptr);

    //##ModelId=43758F7A0290
    //##Documentation
    //## 功能：
    //##   增长文件
    //## 参数：
    //##   uint32 newsize
    //## 返回：
    //##   if  success return 1
    //##   else return 0;
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    uint8 growFile(uint32 newsize);

    //##ModelId=43758FD4034B
    //##Documentation
    //## 功能：
    //##   填充内存为空
    //## 参数：
    //##   char*  ptr
    //##   uint32 size
    //## 返回：
    //##   if success return char*
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    void gmemset(char* ptr, uint32 size);

    //##ModelId=437593200138
    //##Documentation
    //## 功能：
    //##   安全取字符串长度
    //## 参数：
    //##   const char*  ptr
    //## 返回：
    //##   if success return length
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    uint32 gstrlen(const char* ptr);

    //##ModelId=437597A8036B
    //##Documentation
    //## 功能：
    //##   判断是否可以增长文件
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-2
    uint8 canGrow();

    //##ModelId=437572220177
    //##Documentation
    //## 数据缓冲句柄
    char* m_hData;

    //##ModelId=4375725F02BF
    //##Documentation
    //## 数据自动增长尺寸
    uint32 m_growSize;

    //##ModelId=437572F10177
    //##Documentation
    //## 当前读写位置指针
    uint32 m_curPos;

    //##ModelId=437574C7037A
    //##Documentation
    //## 内存数据长度
    uint32 m_dataLength;

    //##ModelId=4375729E0000
    //##Documentation
    //## 数据缓冲最大长度
    uint32 m_maxLength;

    //##ModelId=4375756002AF
    //##Documentation
    //## 是否连接外部数据
    uint8 m_uAttach;

};

#endif /* GMEMFILE_H_HEADER_INCLUDED_BC8A5FAC */
