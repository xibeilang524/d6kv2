#ifndef DDBI_H_HEADER_INCLUDED_BCB03856
#define DDBI_H_HEADER_INCLUDED_BCB03856

#include "ddbi/ddbi_def.h"

class QLibrary;
class DdbiBase ;

//加载动态库回调函数
//symbol = dbiFactoryBuild
//

typedef DdbiBase* (*DBIFB_PROC)();

//##ModelId=434F8CBC034B
//##Documentation
//## 定义：
//##        数据库接口类
//##        继承类实现具体操作功能      
//## 功能：
//##        实现数据库无关类接口
//##        提供表数据结构查询
//##        提供数据库查询，删除，插入，更新操作
//##        支持直接使用SQL语言操作功能
//##       统一数据库编程时数据库操作定义    
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
//## 用例：
//##   const char*  PASSWDT="用户口令表";
//## 
//##   ddbi ddbi;
//##   if ( !ddbi.startdb() )
//##     return 0;
//##   
//##   if ( !ddbi.selectFrom(PASSWDT)
//##     return 0;
//##   
//##   GdbRecords dbrecs ;
//## 
//##   if ( !ddbi.selectRecords( dbrecs , 
//##            "where 操作员=sa") )
//##      return 0;
//##   
//##   ...... 
//##  
//##   return 1;
class GDE_EXPORT_DBI Ddbi
{
  public:
    //##ModelId=434F906A008C
    //##Documentation
    //## 数据库类型
    //## 用于createDBI
    enum  { 
        //##Documentation
        //## 历史参数库
        GD_DBI_HIS = 0, 
        //##Documentation
        //## 实时数据库
        GD_DBI_REAL = 1, 
        //##Documentation
        //## ODBC数据库
        GD_DBI_ODBC = 2 };
    
     //##ModelId=434FA17700DA
    enum  { 
        //##Documentation
        //## 只读方式打开
        G_DB_R = 0x01, 
        //##Documentation
        //## 读写方式打开
        G_DB_RW = 0x02 };

	//##ModelId=434FA17700DA
	enum{
		G_DBT_ODBC_UNKNOWN	=0x00,
		G_DBT_ODBC_ORACLE	=0x01,
		G_DBT_ODBC_SYBASE	=0x02,//default
		G_DBT_ODBC_SQLSERVER=0x03,
		G_DBT_ODBC_MYSQL	=0x04,
		G_DBT_ODBC_DB2		=0x05 };

    //##ModelId=434F8D17031C
    //##Documentation
    //## 功能：
    //##     启动数据库，默认为读取访问
    //## 参数：
    //##     uint32 opmode //打开方式（G_DB_R，G_DB_RW）
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    virtual uint8 startdb(
        //##Documentation
        //## 访问选项（G_DB_R，G_DB_RW）
        //## 默认为G_DB_R
        uint32 opmode = G_DB_R);

    //##ModelId=434F8D1D0109
    //##Documentation
    //## 功能：
    //##     关闭数据库
    //## 参数：
    //##     无
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    virtual uint8 closedb();

    //##ModelId=434F8D1F0290
    //##Documentation
    //## 功能：
    //##     设置内置数据库名称
    //## 参数：
    //##     const char* dbname //数据库名称
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    uint8 setDB(
        //##Documentation
        //## 指定数据库名
        const char* dbname);

    //##ModelId=434F8D210222
    //##Documentation
    //## 功能：
    //##     取得内置数据库名
    //## 参数：
    //##    无
    //## 返回：（const char* ）
    //##     if success return dbname
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    const char* getDB() const;

    //##ModelId=434F8D23031C
    //##Documentation
    //## 功能：
    //##     设置内置表名
    //##     用于读取数据库表信息
    //##     改变操作表，必须先调用此函数
    //## 参数：
    //##     const char* tblname //读取的表名
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    uint8 selectFrom(
        //##Documentation
        //## 数据库表名
        const char* tblname);

    //##ModelId=434F8D2600DA
    //##Documentation
    //## 功能：
    //##     取内置表名称
    //## 参数：
    //##     无
    //## 返回：（const char* ）
    //##     if success return tblname
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    const char* getTable() const;

    //##ModelId=434F8D660261
    //##Documentation
    //## 功能：
    //##     设置内置数据库登录用户名称
    //## 参数：
    //##     const char* user//登录用户名称
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    uint8 setUser(const char* user = NULL);

    //##ModelId=434F8D6D029F
    //##Documentation
    //## 功能：
    //##     设置内置数据库登录用户密码
    //## 参数：
    //##     const char* passwd//登录用户密码
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    uint8 setPass(const char* passwd = NULL);

    //##ModelId=434F8D7203D8
    //##Documentation
    //## 功能：
    //##     取内置登录用户名
    //## 参数：
    //##     无
    //## 返回：（const char* ）
    //##     if success return usrname
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    const char* getUser();

    //##ModelId=434F8D7B0242
    //##Documentation
    //## 功能：
    //##     取内置登录用户密码
    //## 参数：
    //##     无
    //## 返回：（const char* ）
    //##     if success return passwd
    //##     else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    const char* getPass();

    //##ModelId=434F8D820177
    //##Documentation
    //## 功能：
    //##   获取错误信息
    //## 参数：
    //##   无
    //## 返回：
    //##   const char*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    const char* getError();


    //##ModelId=434F8D9401D4
    //##Documentation
    //## 功能：
    //##     读取全部表记录集信息
    //## 参数：
    //##     GdbRecords& records//存储表中所有记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-9
    virtual uint8 readRecords(
        //##Documentation
        //## 记录集信息
        GdbRecords& records);

    //##ModelId=434F8D9C00BB
    //##Documentation
    //## 功能：
    //##     写入全部表记录集信息
    //## 参数：
    //##     GdbRecords& records//存储表中所有记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-8
    virtual uint8 writeRecords(GdbRecords& records);

    //##ModelId=434F8DE000FA
    //##Documentation
    //## 功能：
    //##     根据指定条件，选取一条记录中某一字段
    //## 参数：
    //##     GdbField& value//存取域值
    //##     const char* keyfield//关键字段域
    //##     const char* keyword //检索关键字
    //##     const char* field   //选取字段域
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 selectAField(GdbField& value, const char* keyfield, const char* keyword, const char* field);

    //##ModelId=434F8DE6005D
    //##Documentation
    //## 功能：
    //##     有指定where条件从句读取选取记录集信息
    //## 参数：
    //##     GdbRecords& records//存储选中的记录
    //##     const char* where  //条件从句，选取记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    virtual uint8 selectRecords(GdbRecords& records, const char* where = NULL);

    //##ModelId=434F8DEB006D
    //##Documentation
    //## 功能：
    //##     插入单条记录
    //## 参数：
    //##     const GdbRecord& rec//单条记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 insertARecord(const GdbRecord& rec);

    //##ModelId=434F8DF00261
    //##Documentation
    //## 功能：
    //##     将老记录更新为新记录
    //## 参数：
    //##     const GdbRecord& oldRec //库中记录 
    //##     const GdbRecord& newRec //新记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 updateARecord(const GdbRecord& oldRec, const GdbRecord& newRec);

    //##ModelId=434F8DF902DE
    //##Documentation
    //## 功能：
    //##     删除一条记录
    //## 参数：
    //##     const GdbRecord& rec//删除的记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 removeARecord(const GdbRecord& rec);

    //##ModelId=434F8DFF0399
    //##Documentation
    //## 功能：
    //##     执行无返回SQL语句
    //## 参数：
    //##     const char* sql//欲执行的SQL语句
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 execSQL(const char* sql);

    //##ModelId=434F8E060128
    //##Documentation
    //## 功能：
    //##     取表列信息，如果成功列信息存储在colfmt中
    //## 参数：
    //##     GdbDataFmt& colfmt //列信息
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    virtual uint8 getFmt(
        //##Documentation
        //## 表列信息
        GdbDataFmt& colfmt);

    //##ModelId=434F8E0F00CB
    //##Documentation
    //## 功能：
    //##      取得表信息
    //## 参数：
    //##      GdbTable& tbl //表信息
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-10-10
    virtual uint8 getTbl(GdbTable& tbl);

    //##ModelId=434F8E9B0157
    //##Documentation
    //## 功能：
    //##   创建DBI接口
    //##   目前支持GD_DBI_HIS
    //##           GD_DBI_REAL
    //##           GD_DBI_ODBC
	//##   调用前必须设置接口路径
    //## 参数：
    //##    uint8 dbiType//数据库接口类型
    //## 返回：
    //##    if success return 1;
    //##    else return 0;
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-14
    virtual uint8 createDBI(uint8 dbi);
	//##ModelId=434F8E9B0157
    //##Documentation
    //## 功能：
    //##   创建DBI接口
    //## 参数：
    //##    const char* libpath//数据库接口文件路径
    //## 返回：
    //##    if success return 1;
    //##    else return 0;
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-14
	virtual uint8 createDBI(const QString& libpath);
	//##ModelId=435C9AD90138
    //##Documentation
    //## 功能：
    //##     执行有返回select-SQL语句
    //## 参数：
    //##     GdbRecords& recs//存放数据缓冲区
    //##     const char* sql//欲执行的SQL语句
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-24
    virtual uint8 readSQL(GdbRecords& recs, const char* sql);

    //##ModelId=434F8F5902CE
	//##Documentation
    //## 功能：
    //##     构造函数
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-10-24
    Ddbi();

    //##ModelId=434F8F610213
	//##Documentation
    //## 功能：
    //##     析构函数
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-10-24
    ~Ddbi();

	//##ModelId=434F8F610213
	//##Documentation
    //## 功能：
    //##    判断数据库类型
    //## 参数：
    //##    int32 dbType;//数据库类型
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-10-24
	virtual uint8 isDBType ( int32 dbType ) const;

	//##ModelId=434F8F610213
	//##Documentation
    //## 功能：
    //##    设置数据库类型
    //## 参数：
    //##    int32 dbType;//数据库类型
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-10-24
	void	setDBType( int32 dbType );

	//##ModelId=434F8F610213
	//##Documentation
    //## 功能：
    //##    获取数据库类型
    //## 参数：
    //##    无
    //## 返回：int32
    //##    数据库类型
    //## 设计：
    //##    宋永生
    //## 日期：
    //##     2005-10-24
	int32   getDBType() const;

	//##ModelId=434F8F610213
	//##Documentation
    //## 功能：
    //##    数据库是否打开
    //## 参数：
    //##    无
    //## 返回：(uint8)
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##    宋永生
    //## 日期：
    //##     2005-10-24
	uint8   isOpen() const;

	//##ModelId=434F8F610213
	//##Documentation
    //## 功能：
    //##    设置数据库插件路径
    //## 参数：
    //##    const char* dbiPath//插件路径
    //## 返回：
    //##    无
    //## 设计：
    //##    宋永生
    //## 日期：
    //##     2005-10-24
	void	setDBIPath(const QString& dbiPath);

  protected:
    //##ModelId=434F8E5B02FD
    //##Documentation
    //## 数据库操作内部接口
    DdbiBase* m_ddbi;

	//##ModelId=434F8E5B02FD
    //##Documentation
    //## 数据库插件加载接口
	QLibrary* m_hLibrary;

	//##ModelId=434F8E5B02FD
    //##Documentation
    //## 数据库插件路径
	QString	  m_dbiPath;

};


//##ModelId=433B75FE0399
//##Documentation
//## 定义：
//##        取历史库中某一列域值
//## 功能：
//##       实现数据库记录定义无关性
//##       统一数据库编程时数据类型定义    
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GdbFieldData
{
  public:
    //##ModelId=433CE0EC0224
    //##Documentation
    //## 功能：
    //##     初始化构造函数
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-1
    GdbFieldData()
	{
	}

    //##ModelId=433B772F00FA
    //##Documentation
    //## 功能：
    //##    取数据构造，构造函数中调用get
    //## 参数：
    //##    ddbi* ddbi    //数据库
    //##    const char* tblname //表名
    //##    const char* keyfield//关键字域名
    //##    const char* keyword //关键字
    //##    const char* field   //选取域
    //## 返回：
    //##    无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GdbFieldData(
        //##Documentation
        //## 数据库接口
        Ddbi* ddbi, 
        //##Documentation
        //## 读取表名
        const char* tblname, 
        //##Documentation
        //## 关键字域名
        const char* keyfield, 
        //##Documentation
        //## 关键字域值
        const char* keyword, 
        //##Documentation
        //## 数据域名
        const char* field)
	{
		get( ddbi , tblname , keyfield , keyword , field );
	}

    //##ModelId=433B785100CB
    //##Documentation
    //## 功能：
    //##     执行取数据过程
    //## 参数：
    //##    ddbi* ddbi    //数据库
    //##    const char* tblname //表名
    //##    const char* keyfield//关键字域名
    //##    const char* keyword //关键字
    //##    const char* field   //选取域
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    //##   
    uint8 get(Ddbi* ddbi, 
        //##Documentation
        //## 读取表名
        const char* tblname, 
        //##Documentation
        //## 关键字域名
        const char* keyfield, 
        //##Documentation
        //## 关键字域值
        const char* keyword, 
        //##Documentation
        //## 数据域名
        const char* field)
	{
		if ( ddbi == NULL ) 
			return 0;
		
		if ( !ddbi->selectFrom( tblname ) )
			return 0 ;
		
		return ddbi->selectAField( value , keyfield , keyword , field );
	}

    //##ModelId=433D2F2F0148
    //##Documentation
    //## 功能：
    //##    析构函数，调用value.clear清空数据
    //## 参数：
    //##    无
    //## 返回：
    //##    无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    ~GdbFieldData()
	{
	}

    //##ModelId=433F8106031C
    //##Documentation
    //## 功能：
    //##      赋值重载函数
    //## 参数：
    //##      const GdbFieldData& field
    //## 返回：
    //##     const GdbFieldData& 
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
	const GdbFieldData& operator=(const GdbFieldData& field)
	{
		if ( this == &field )
			return *this ;
			
		value = field.value ;
		
		return *this ;
	}

    //##ModelId=433F816700DA
    //##Documentation
    //## 功能：
    //##     初始化构造函数
    //## 参数：
    //##     const GdbFieldData& field
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-1
	GdbFieldData(const GdbFieldData& field )
	{
		if ( this == &field )
			return ;
			
		value = field.value ;
	}

    //##ModelId=433B763F0186
    GdbField value;
};

#endif /* DDBI_H_HEADER_INCLUDED_BCB03856 */

