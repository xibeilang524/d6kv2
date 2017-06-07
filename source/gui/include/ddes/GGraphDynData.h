#ifndef GGRAPHDYNDATA_H_HEADER_INCLUDED_BCB8D865
#define GGRAPHDYNDATA_H_HEADER_INCLUDED_BCB8D865

#include <QtXml/QtXml>
#include <QtCore/QString>

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"
#include "GVersion.h"

class GFile;
class GBaseObject;
class GGraphDynData;
//加载动态库回调函数
//symbol = gddFactoryBuild
//typedef GGraphDynData* (*GDDFB_PROC)();

//##ModelId=4340F8B101B5
//##Documentation
//## 定义：
//##   图元连接动态数据
//## 功能：
//##   用于定义图元连接的动态数据
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-3
class GDE_EXPORT_DES GGraphDynData
{
  public:
	//##ModelId=43A63FE501C5
    enum  { 
        //##Documentation
        //## 数据库名称
        GDDT_DBNAME, 
        //##Documentation
        //## 表名
        GDDT_TBNAME, 
        //##Documentation
        //## 关键字域名
        GDDT_KEYNAME, 
        //##Documentation
        //## 关键字域值
        GDDT_KEYWORD, 
        //##Documentation
        //## 其他表名-GDDT_TBNAME 别名
        GDDT_RTABLE, 
        //##Documentation
        //## 其他表关键字名-GDDT_KEYNAME别名
        GDDT_RKEYNAME, 
        //##Documentation
        //## 选取的域名
        GDDT_RSFIELD,
        //##Documentation
        //## 设备类型
        GDDT_DEVTYPE,
        //##Documentation
        //## 起始厂站
		GDDT_HSTATION,
		//##Documentation
        //## 所属厂站
		GDDT_STATION=GDDT_HSTATION,
        //##Documentation
        //## 终端厂站
		GDDT_TSTATION,
        //##Documentation
        //## 设备编号
		GDDT_DEVID,
		//高侧电压等级
		GDDT_VOLH,
		//中侧电压等级
		GDDT_VOLM,
		//第侧电压等级
		GDDT_VOLL};

	//##ModelId=4341029B02AF
    //##Documentation
    //## 功能：
    //##     构造函数
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GGraphDynData();
	//##ModelId=43A62A7E01F4
    //##Documentation
    //## 功能：
    //##     析构函数
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    virtual ~GGraphDynData();

	//##ModelId=43422C970213
    //##Documentation
    //## 功能：
    //##   实现图元科隆技术
    //## 参数：
    //##   无
    //## 返回：
    //##   GGraphDynData*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual GGraphDynData* clone();
	 
    //##ModelId=435314BF02AF
    //##Documentation
    //## 功能：
    //##   取得图元动态尺寸，用于存储图元
    //## 参数：
    //##   无
    //## 返回：uint32
    //##   if success return realsize
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    uint32 getSize();

	//##ModelId=4340FD560167
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return write real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=4340FD5900AB
    //##Documentation
    //## 功能：
    //##      重载读取函数
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
	//##ModelId=43A9108A036B
    //##Documentation
    //## 功能：
    //##      从其他对象中读取数据
    //## 参数：
    //##      GGraphDynData *pObj
    //## 返回：（int32）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadObj(GGraphDynData* pObj);
    //##ModelId=43A9108A03C8
    //##Documentation
    //## 功能：
    //##      从其他对象中读取数据
    //## 参数：
    //##      const GGraphDynData& obj
    //## 返回：（int32）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadObj(const GGraphDynData& obj);
	//##ModelId=435DA0AE01E7
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 readXML(QDomElement& dom);
	//##ModelId=435DA0B1034E
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      QTextStream& stream
    //##      int32 indent
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);
    //##ModelId=43A61C2703D8
    //##Documentation
    //## 功能：
    //##   是否实时数据
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return  0 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual uint8 canReal() const;

	 //##ModelId=4354B56202F9
    //##Documentation
    //## 功能：
    //##   创建新的图元
    //## 参数：
    //##   const QString& src//数据源
    //## 返回：
    //##   if success return GGraphDynData* 
    //##   else return NULL
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    GGraphDynData* newImpObject(const QString& src);

    //##ModelId=43A61E180148
    //##Documentation
    //## 功能：
    //##   清除内部实例
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-13
    void clearImpObject();
    //##ModelId=43A61E790261
    //##Documentation
    //## 功能：
    //##   创建内部实例
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1 
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-13
    uint8 createImpObject();
	//##ModelId=43A631660119
    //##Documentation
    //## 功能：
    //##   取得数据类型
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32  
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    uint32 getType() const;
	//##ModelId=43A631660119
    //##Documentation
    //## 功能：
    //##   取得数据类型
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32  
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    uint32 getDevID() const;
	//##ModelId=43A631660119
    //##Documentation
    //## 功能：
    //##   取得数据类型
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32  
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    void setDevID(uint32 devID);
	//##ModelId=43A9365C0177
    //##Documentation
    //## 功能：
    //##   取得图元数据中的指定参数的名称
    //## 参数：
    //##   int32 pos
    //## 返回：
    //##   const char*   
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    virtual const char* getDataName(int32 pos) const;
	//##ModelId=43A63A78032C
    //##Documentation
    //## 功能：
    //##   取得图元数据中的参数个数
    //## 参数：
    //##   无
    //## 返回：uint32
    //##   if success return realsize
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    virtual int32 getDataCount() const;
    //##ModelId=43A63AC50251
    //##Documentation
    //## 功能：
    //##   取得图元数据中的指定参数的数据类型
    //## 参数：
    //##   int32 pos
    //## 返回：int32     
    //##     
    //##     GDB_FIELD_UINT8 = 0, //无符号字符型    
    //##     GDB_FIELD_INT8 = 1, //有符号字符型
    //##     GDB_FIELD_UINT16 = 2, //无符号短整数型  
    //##     GDB_FIELD_INT16 = 3, //有符号短整数型
    //##     GDB_FIELD_UINT32 = 4, //无符号整数型
    //##     GDB_FIELD_INT32 = 5, //有符号整数型
    //##     GDB_FIELD_UINT64 = 6, //无符号长整数型
    //##     GDB_FIELD_INT64 = 7, //有符号长整数型
    //##     GDB_FIELD_FLOAT4 = 8, //单精度浮点型
    //##     GDB_FIELD_FLOAT8 = 9, //双精度浮点型
    //##     GDB_FIELD_STRING = 10, //字符串型
    //##     GDB_FIELD_UNKNOWN=-1   //未知类型
    //##   if success return datatype
    //##   else return -1
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    virtual int32 getDataType(int32 pos) const;
    //##ModelId=43A63D320203
    //##Documentation
    //## 功能：
    //##   取得图元数据中的指定参数的数据
    //## 参数：
    //##   int32 pos
    //## 返回：
    //##   QString   
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    virtual QString getDataString(int32 pos);
	//##ModelId=443CF13900DA
    //##Documentation
    //## 功能：
    //##   设置图元数据中的指定参数的数据
    //## 参数：
    //##   int32 pos,const char* strValue
    //## 返回：
    //##   QString   
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
	virtual void  setDataString(int32 pos,const char* strValue);
    //##ModelId=43A63E2E02AF
    //##Documentation
    //## 功能：
    //##   取得图元数据中的指定特定参数位置
    //## 参数：
    //##   GDDT_DBNAME //数据库名称
    //##   GDDT_TBNAME //表名
    //##   GDDT_KEYNAME//关键字域名
    //##   GDDT_KEYWORD//关键字域值
    //##   GDDT_RTABLE //其他表名-GDDT_TBNAME 别名
    //##   GDDT_RKEYNAME//其他表关键字名-GDDT_KEYNAME别名
    //##   GDDT_RSFIELD //选取的域名
    //##   int32 type
    //## 返回： int32
    //##   if exist return pos
    //##   else return -1
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    virtual int32 getDataPos(int32 type) const;
	//##ModelId=43A6C9C402CE
    //##Documentation
    //## 功能：
    //##   获取数据名称
    //## 参数：
    //##   无
    //## 返回：
    //##   const char*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-17
    virtual QString getPluginName() const;

	//获取数据keyword
	virtual QString getKeyWord(void) const;

	//获取数据的类型，对于设备插件，返回设备类型，对于事实数据插件，返回数据的P、Q、I
	virtual int32 getKeyType(void) const;

    //##ModelId=4340F917032C
    //##Documentation
    //## 外挂数据源
    //##  
    QString m_impSrc;
    //##ModelId=43A61CBC02FD
    //##Documentation
    //## 数据源实现
    GGraphDynData* m_impObj;
	//##ModelId=43A6272002FD
    //##Documentation
    //## 数据类型
    uint32 m_nType;

	//##ModelId=43A6272002FD
    //##Documentation
    //## 数据版本号
	GVersion m_vers;
};

//##ModelId=4340FB28009C
class GDE_EXPORT_DES GGraphDynDataList:public GList<GGraphDynData*>{};

#endif /* GGRAPHDYNDATA_H_HEADER_INCLUDED_BCB8D865 */
