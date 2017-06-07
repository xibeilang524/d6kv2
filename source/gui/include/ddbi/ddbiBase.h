#ifndef DDBIBASE_H_HEADER_INCLUDED_BCB071E7
#define DDBIBASE_H_HEADER_INCLUDED_BCB071E7

#include "ddef/ddes_comm.h"
#include "ddbi_def.h"

//##ModelId=433CBE8C0174
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
//##   ddbiBase ddbi;
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
//##  
class GDE_EXPORT DdbiBase
{
  public:
    //##ModelId=434734500157
    enum  { 
        //##Documentation
        //## 只读方式打开
        G_DB_R = 0x01, 
        //##Documentation
        //## 读写方式打开
        G_DB_RW = 0x02 };

	enum{
		G_DBT_ODBC_UNKNOWN	=0x00,
		G_DBT_ODBC_ORACLE	=0x01,
		G_DBT_ODBC_SYBASE	=0x02,
		G_DBT_ODBC_SQLSERVER=0x03,
		G_DBT_ODBC_MYSQL	=0x04,
		G_DBT_ODBC_DB2		=0x05 };

    //##ModelId=433CC3B801EF
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

    //##ModelId=433CC5550068
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

    //##ModelId=433CC913028A
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

    //##ModelId=433CC98D00C5
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

    //##ModelId=433CCE450304
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

    //##ModelId=433CCF2201BC
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

    //##ModelId=433CC77701FD
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
        GdbDataFmt& colfmt) = 0;

    //##ModelId=433CE2100188
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
    //##       2005-9-30
    virtual uint8 selectAField(
        //##Documentation
        //## 返回值
        GdbField& value, 
        //##Documentation
        //## 关键字域名
        const char* keyfield, 
        //##Documentation
        //## 关键字值
        const char* keyword, 
        //##Documentation
        //## 欲选取的字段域名
        const char* field) = 0;

    //##ModelId=433CD809011D
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
    virtual uint8 selectRecords(
        //##Documentation
        //## 记录集
        GdbRecords& recs, 
        //##Documentation
        //## 条件从句，进行记录过滤，if where=NULL 其结果等价于readRecords
        const char* where = NULL) = 0;

    //##ModelId=433CD2140286
    //##Documentation
    //## 功能：
    //##     插入单条记录
    //## 参数：
    //##      const GdbRecord& rec//单条记录
    //## 返回：（uint8）
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-30
    virtual uint8 insertARecord(
        //##Documentation
        //## 记录信息
        const GdbRecord& rec) = 0;

    //##ModelId=433CD2F500EF
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
    //##       2005-9-30
    virtual uint8 updateARecord(
        //##Documentation
        //## 老记录
        const GdbRecord& oldRec, 
        //##Documentation
        //## 新记录
        const GdbRecord& newRec) = 0;

    //##ModelId=433CD3000351
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
    //##       2005-9-30
    virtual uint8 removeARecord(
        //##Documentation
        //## 表记录
        const GdbRecord& rec) = 0;

    //##ModelId=433CD7690218
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
    //##       2005-9-30
    virtual uint8 execSQL(
        //##Documentation
        //## SQL语句
        const char* sql) = 0;

    //##ModelId=433E9EF300DA
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
    DdbiBase();

    //##ModelId=433E9EF90271
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
    //##       2005-10-1
    virtual ~DdbiBase();

    //##ModelId=4347285101D4
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

    //##ModelId=4347285801C5
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

    //##ModelId=434728FE0232
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

    //##ModelId=43472925030D
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

    //##ModelId=4347618F0196
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

    //##ModelId=434762A20186
    //##Documentation
    //## 功能：
    //##   输出错误信息到m_strerr
    //## 参数：
    //##   const char* format//格式
    //##   ...//可变参数
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    void errFormat(const char* format, ...);

    //##ModelId=433CCD62018E
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

    //##ModelId=4349293F0167
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

    //##ModelId=4349D93F034B
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
    virtual uint8 getTbl(GdbTable& tbl) = 0;
	//##ModelId=435C99FB0177
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
    virtual uint8 readSQL(GdbRecords& recs, const char* sql) = 0;

	//##ModelId=435C99FB0177
    //##Documentation
    //## 功能：
    //##     判断数据库类型
    //## 参数：
    //##     int32 dbType
    //## 返回：(uint8)
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-24
	virtual uint8 isDBType ( int32 dbType );

	//##ModelId=435C99FB0177
    //##Documentation
    //## 功能：
    //##     设置数据库类型
    //## 参数：
    //##     int32 dbType
    //## 返回：(uint8)
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-24
	void	setDBType( int32 dbType );
	//##ModelId=435C99FB0177
    //##Documentation
    //## 功能：
    //##     获取数据库类型
    //## 参数：
    //##     
    //## 返回：(int32)
    //##     
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-24
	int32   getDBType() const;
	
	virtual uint8 isOpen() const;

  protected:
    //##ModelId=433CC8A903E1
    //##Documentation
    //## 内置数据库名称
    //## 用于多数据库系统如sybase
    char m_dbname[GDB_FMT_LEN];

    //##ModelId=433CCE050391
    //##Documentation
    //## 内置数据库表名，保存上次选取表的表名
    char m_tblname[GDB_FMT_LEN];

    //##ModelId=434727D80128
    //##Documentation
    //## 登录口令
    char m_passwd[GDB_FMT_LEN];

    //##ModelId=434727DE0242
    //##Documentation
    //## 登录用户名称
    char m_usrname[GDB_FMT_LEN];

    //##ModelId=4347615102EE
    //##Documentation
    //## 错误信息描述
    char m_strerr[GDB_STR_LEN];

	int32  m_dbType ;

};

#endif /* DDBIBASE_H_HEADER_INCLUDED_BCB071E7 */
