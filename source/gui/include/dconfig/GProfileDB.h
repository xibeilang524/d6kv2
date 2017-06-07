#ifndef _GPROFILEDB_H
#define _GPROFILEDB_H

#include "GProfile.h"

class Ddbi;

//##ModelId=43422ED200CB
//##Documentation
//## 定义：
//##   配置数据库读写
//## 功能：
//##   进行系统配置数据库读写功能
//##   配置数据库采用如下格式进行存储
//##   #------注视语句
//##   func_block1 keyword1 value
//##   func_block1 keyword2 value
//##   func_block2 keyword1 value
//##   func_block2 keyword2 value
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-06
class GDE_EXPORT_CONF GProfileDB:public GProfile
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
    GProfileDB(Ddbi* dbi);    
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
    ~GProfileDB();
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
    
protected:
		//##ModelId=43508BB70157
    //##Documentation
    //##数据库访问插件
		Ddbi*		m_ddbi;
};

#endif
