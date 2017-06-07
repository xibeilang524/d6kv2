#ifndef GPROFILE_H_HEADER_INCLUDED_BCB8E460
#define GPROFILE_H_HEADER_INCLUDED_BCB8E460
// #include <qstring.h>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"

//##ModelId=4345C2FF0109
//##Documentation
//## 定义：
//##   配置文件关键字及其值
//## 功能：
//##   用于配置文件读取链表
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-7
struct GDE_EXPORT_CONF GPFS
{
    //##ModelId=434E11F8025C
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   const GPFS& gpfs
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    GPFS(const GPFS& gpfs)
	{
		key = gpfs.key ;
		value = gpfs.value ;
	}
    //##ModelId=434E122E0365
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
    //##   2005-10-13
    GPFS()
	{
	}
    //##ModelId=434E123A00B6
    //##Documentation
    //## 功能：
    //##   赋值
    //## 参数：
    //##   const GPFS&
    //## 返回：
    //##   const GPFS&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    const GPFS& operator=(const GPFS& gpfs)
	{
		if ( this == &gpfs )
			return *this ;

		key = gpfs.key ;
		value = gpfs.value ;

		return *this ;
	}
	//##ModelId=435D8257004D
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   const char* k//关键字
    //##   const char* v//值
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    GPFS(const QString& k, const QString& v)
	{
		key   = k ;
		value = v ;
	}


    //##ModelId=4345C356030D
    //##Documentation
    //## 关键字
    QString key;

    //##ModelId=4345C37002AF
    //##Documentation
    //## 值
    QString value;

};

class GDE_EXPORT_CONF GListPFS: public GList<GPFS *>{};

//##ModelId=43422ED200CB
//##Documentation
//## 定义：
//##   配置文件读写
//## 功能：
//##   进行系统配置文件读写功能
//##   配置文件采用如下格式进行存储
//##   #------注视语句
//##   [func_block1]
//##   keyword1=value
//##   keyword2=value
//##   [func_block2]
//##   keyword1=value
//##   keyword2=value
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-06
class GDE_EXPORT_CONF GProfile
{
  public:
    //##ModelId=434E050B03A7
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
    //##   2005-10-13
    GProfile();
	//##ModelId=434E0502029D
    //##Documentation
    //## 功能：
    //##   析构函数
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    ~GProfile();
	//##ModelId=434E0AE102FA
    //##Documentation
    //## 功能：
    //##   创建配置文件
    //## 参数：
    //##   const char* profile //配置文件
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    //##  
    virtual uint8 create(const char* profile);
    //##ModelId=434E0AE102FA
    //##Documentation
    //## 功能：
    //##   打开配置文件
    //## 参数：
    //##   const char* profile //配置文件
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    //##  
    virtual uint8 open(const char* profile);
	//##ModelId=434E0D8B029B
    //##Documentation
    //## 功能：
    //##   关闭配置文件
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
	virtual void  close();
    //##ModelId=434E0D8B029B
    //##Documentation
    //## 功能：
    //##   配置文件是否可以使用
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual uint8 isValid();
	//##ModelId=4345C41C0177
    //##Documentation
    //## 功能：
    //##   添加注释
    //## 参数：
    //##   const char* comment //注释
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    //##   
	virtual uint8 addComment(const char* comment);
    //##ModelId=4345C41C0177
    //##Documentation
    //## 功能：
    //##   读取配置文件
    //## 参数：
    //##   const char* entry //入口段名
    //##   const char* key   //关键字
    //##   char* value       //值
    //##   const char* def   //缺省值
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    //##   
    virtual uint8 getString(const char* entry, const char* key, char* value, const char* def = NULL);

    //##ModelId=4345C42502FD
    //##Documentation
    //## 功能：
    //##   写入配置文件
    //## 参数：
    //##   const char* entry //入口段名
    //##   const char* key   //关键字
    //##   char* value       //值
    //##   const char* def   //缺省值
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    virtual uint8 writeString(const char* entry, const char* key, const char* value);

	//##ModelId=4345C42502FD
    //##Documentation
    //## 功能：
    //##   翻译配置文件
    //## 参数：
    //##   GString& strDst    //结果
    //##   const char* strSrc //带环境变量的字符串
	//##   环境变量:SYSHOME=c:
	//##   例如:
	//##	strSrc=$(SYSHOME)/bin
	//##	strDst=c:/bin
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
	static uint8 parseString(QString& strDst,const char* strSrc);

	//##ModelId=4345C42502FD
    //##Documentation
    //## 功能：
    //##   翻译配置文件
    //## 参数：
    //##   GString& strDst    //结果
    //##   const char* strSrc //带环境变量的字符串
	//##   const char* env	  //环境变量
	//##   环境变量:SYSHOME=c:
	//##   例如:
	//##	env="SYSHOME"
	//##	strSrc=c:/bin
	//##	strDst=$(SYSHOME)/bin
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
	static uint8 makeString(QString& strDst,const char* strSrc,const char* env);
	
    //##ModelId=4345C59D02CE
    //##Documentation
    //## 功能：
    //##   读取配置文件段所有域
    //## 参数：
    //##   const char* entry //入口段名 
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    virtual uint8 readBlock(const char* entry);
	//##ModelId=4345C59D02CE
    //##Documentation
    //## 功能：
    //##   读取配置文件所有段
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    virtual uint8 readEntries();
	//##ModelId=4345C74E02FD
    //##Documentation
    //## 功能：
    //##     取首节点位置
    //## 参数：
    //##    无
    //## 返回：（GPOS）
    //##     if success return pos
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
	GListPFS& getPFS();
	//##ModelId=4345C74E02FD
    //##Documentation
    //## 功能：
    //##     取首节点位置
    //## 参数：
    //##    无
    //## 返回：（GPOS）
    //##     if success return pos
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
	const GListPFS& getPFS() const;
    //##ModelId=4345C74E02FD
    //##Documentation
    //## 功能：
    //##     取首节点位置
    //## 参数：
    //##    无
    //## 返回：（GPOS）
    //##     if success return pos
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GPOS firstPos() const;

    //##ModelId=4345C7820119
    //##Documentation
    //## 功能：
    //##     取下一个节点位置
    //## 参数：
    //##    GPOS& pos //获取下一个位置
    //## 返回：（GPFS&）
    //##     if success return GPFS&
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GPFS& nextPos(GPOS& pos);
	//##ModelId=4345C7820119
    //##Documentation
    //## 功能：
    //##     取下一个节点位置
    //## 参数：
    //##    GPOS& pos //获取下一个位置
    //## 返回：（GPFS&）
    //##     if success return GPFS&
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPFS& nextPos(GPOS& pos) const;
    //##ModelId=434E04A101D3
    //##Documentation
    //## 功能：
    //##   返回m_block中配置条件个数
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    uint32 count() const;
    //##ModelId=43508BB70157
    //##Documentation
    //## 功能：
    //##   写入配置文件段所有域
    //## 参数：
    //##   const char* entry //入口段名 
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    uint8 writeBlock(const char* entry);
    //##ModelId=435325E001B5
    //##Documentation
    //## 功能：
    //##   读取配置文件
    //## 参数：
    //##   const char* entry //入口段名
    //##   const char* key   //关键字
    //##   int32 def         //缺省值
    //## 返回：
    //##   if exists return get
    //##   else return def
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    int32 getInt(const char* entry, const char* key, int32 def);
    //##ModelId=4353273F03D8
    //##Documentation
    //## 功能：
    //##   读取配置文件
    //## 参数：
    //##   const char* entry //入口段名
    //##   const char* key   //关键字
    //##   FLOAT8 def         //缺省值
    //## 返回：
    //##   if exists return get
    //##   else return def
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-17
    FLOAT8 getDouble(const char* entry, const char* key, FLOAT8 def);
    //##ModelId=435327A80232
    //##Documentation
    //## 功能：
    //##   写入配置文件
    //## 参数：
    //##   const char* entry //入口段名
    //##   const char* key   //关键字
    //##   int32 value       //值
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    uint8 writeInt(const char* entry, const char* key, int32 value);
    //##ModelId=4353283403D8
    //##Documentation
    //## 功能：
    //##   写入配置文件
    //## 参数：
    //##   const char* entry //入口段名
    //##   const char* key   //关键字
    //##   FLOAT8 value       //值
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-7
    uint8 writeDouble(const char* entry, const char* key, FLOAT8   value);
	//##ModelId=434E0D8B029B
    //##Documentation
    //## 功能：
    //##   获取配置文件路径名
    //## 参数：
    //##   无
    //## 返回：
    //##   const QString&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
	const QString& getFilePath() const;
  protected:
    //##ModelId=435D835F01D3
    //##Documentation
    //## 功能：
    //##   删除配置文件中block内容
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    void remove();
  protected:
    //##ModelId=43508C58005E
    //##Documentation
    //## 块配置属性	
	GListPFS m_block;
	//##ModelId=43508C58005E
    //##Documentation
    //##存储文件路径
	QString	 m_filePath;
    //##ModelId=434E0B4B0377
    //##Documentation
    //## 配置文件句柄
    QFile* m_fFile;
	QDomDocument*	m_pDoc ;
  private:


};

#endif /* GPROFILE_H_HEADER_INCLUDED_BCB8E460 */
