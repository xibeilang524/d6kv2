//#include "sybase.h"

#include "db/dbapi.h"
#include "verapi.h"

#include <io.h>

//#include <sys\stat.h>

#include <conio.h>
#include <direct.h>

//#define m_startRtu 40
#define YXNUM	10
#define YCNUM	5
class CDbOperate
{
private:
	int m_StationNum;
	int m_PointNum;
	int m_startChan;
	int m_startRtu;
	DBSingle dbsingle;
private:
	int DBStart();
	int InsertStaData();
	int InsertRtuData( );
	int InsertChanData( );
	int InsertDevData( );
	int InsertYcData( );
	int InsertYxData( );

public:
	CDbOperate(int startChan, int startRtu, int stanum, int pointnum );
	~CDbOperate();
	int InsertData();
};
CDbOperate::CDbOperate(int startChan, int startRtu, int stanum, int pointnum )
{
	m_startChan = startChan;
	m_startRtu = startRtu;
	m_StationNum = stanum;
	m_PointNum = pointnum;
}

CDbOperate::~CDbOperate()
{
	dbsingle.Close();
}

int CDbOperate::InsertRtuData( )
{
	int i, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "终端参数表" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_StationNum;
	pt = ( char* )malloc( func.recorderlen * m_StationNum );
	int ptcnt = 0;
	int ptstart = 0;
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		col = 0;
		ptcnt = ptstart;
		//终端序号
		tmpint = i + m_startRtu;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//代码
		sprintf( tmpstr, "rtest%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//描述
		sprintf( tmpstr, "测试终端%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//安全级
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//站所线系代码
		sprintf( tmpstr, "test%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//主通道号
// 		tmpint = i + m_startChan;
// 		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
// 		col ++;
		//遥测个数
		tmpint = m_PointNum / YXNUM * YCNUM;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//遥信个数
		tmpint = m_PointNum;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//电度个数
		tmpint = 10;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//状态个数
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//使用标志
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		
		ptstart += func.recorderlen;
	}
	
	retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertChanData( )
{
	int i, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "通道参数表" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_StationNum;
	pt = ( char* )malloc( func.recorderlen * m_StationNum );
	int ptcnt = 0;
	int ptstart = 0;
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		col = 0;
		ptcnt = ptstart;
		//通道序号
		tmpint = i + m_startChan;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//代码
		sprintf( tmpstr, "ctest%03d", i + m_startChan );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//描述
		sprintf( tmpstr, "测试终端%03d通道", i + m_startChan );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//安全级
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//规约类型
		sprintf( tmpstr, "iec104");
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//终端序号
		tmpint = i + m_startRtu;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//组内优先级
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//公共地址
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//主站地址
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//是否检查
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//是否GPS
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//起始状态
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//通道停止时间限
		tmpint = 300;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//误码率标准
		tmpint = 20;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//默认值班机
		sprintf( tmpstr, "FEP1");
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//设备类型
		tmpint = 2;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//本地设备
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//本地设备端口
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//设备地址
		sprintf( tmpstr, "172.27.27.170" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//设备端口
		tmpint = 10001+i;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//备设备地址
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//备设备端口
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//波特率
		tmpint = 1200;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//校验位
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//停止位
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//数据位
		tmpint = 8;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//流控制
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//是否推图
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//推图图名
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
// 		//起始运行年月日
// 		tmpint = 0;
// 		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
// 		col ++;
		//通道使用
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;


		ptstart += func.recorderlen;
	}
	
	retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertDevData( )
{
	int i, j, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "SCD设备参数表" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_PointNum / YXNUM;
	pt = ( char* )malloc( func.recorderlen * func.roxnum );
	int ptcnt = 0;
	int ptstart = 0;
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		ptcnt = 0;
		ptstart = 0;
		memset( pt, 0, func.recorderlen * func.roxnum );
		for( j = 0; j < m_PointNum / YXNUM ; j ++ )
		{
			col = 0;
			ptcnt = ptstart ;
			//SUBID
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			//DTYPEID
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			//DEVID
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;    
			// RCFGID         
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;    
			// 代码		
			sprintf( tmpstr, "Ktest%03d%03d", i + m_startRtu, j + 1 );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// 描述	
			sprintf( tmpstr, "测试站%03d开关%03d", i + m_startRtu, j + 1 );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// 站所线系代码	
			sprintf( tmpstr, "test%03d", i + m_startRtu );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// 类型      
			tmpint = 1;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// 高电压等级 
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// 中电压等级 
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// 低电压等级  
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// 虚设备标志 
			tmpint = 0;
			dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
			col ++;
			// 末端站所线系代码 
			sprintf( tmpstr, ""  );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// 型号 	
			sprintf( tmpstr, ""  );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;
			// 备注 		
			sprintf( tmpstr, ""  );
			dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
			col ++;

			ptstart += func.recorderlen;
		}
		retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	}
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertYcData( )
{
	int i, j, k, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];
	double tmpdouble;

	int ycno;
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "遥测参数表" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = m_PointNum / YXNUM * YCNUM;
	pt = ( char* )malloc( func.recorderlen * func.roxnum );
	int ptcnt;
	int ptstart = 0;
	
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		ycno = 0;
		ptstart = 0;
		memset( pt, 0, func.recorderlen * func.roxnum );
		for( j = 0; j < m_PointNum / YXNUM ; j ++ )
		{
			for( k = 0; k < YCNUM; k ++ )
			{
				col = 0;
				ptcnt = ptstart;
				//SUBID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DTYPEID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DEVID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;    
				switch( k )
				{
				case 0:
					{
						// 代码
						sprintf( tmpstr, "Ktest%03d%03dP", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						//描述
						sprintf( tmpstr, "测试站%03d开关%03d有功", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;	
					}
					break;
				case 1:
					{
						sprintf( tmpstr, "Ktest%03d%03dQ", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03d无功", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				case 2:
					{
						sprintf( tmpstr, "Ktest%03d%03dIa", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03dA电流", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				case 3:
					{
						sprintf( tmpstr, "Ktest%03d%03dIb", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03dB电流", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				case 4:
					{
						sprintf( tmpstr, "Ktest%03d%03dIc", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03dC电流", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				default:
					break;
				}
				// 安全级
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 设备代码
				sprintf( tmpstr, "Ktest%03d%03d", i + m_startRtu, j + 1 );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 类型
				switch( k )
				{
				case 0:
					{
						tmpint = 20;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 1:
					{
						tmpint = 40;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 2:
					{
						tmpint = 2;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 3:
					{
						tmpint = 3;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				case 4:
					{
						tmpint = 4;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				default:
					break;
				}
				// 采集端子
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 终端序号
				tmpint =  i + m_startRtu;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥测序号
				tmpint =  ycno ++ ;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 斜率
				tmpdouble = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 截距
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 零漂
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 上溢值
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 下溢值
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 最大增量
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 存盘周期
				tmpint = 5;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 计划间隔
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 死数时间限
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 事项处理方式
				sprintf( tmpstr, "event" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 处理越限级别
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 警告上限
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 警告下限
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 警报上限
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 警报下限
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 追忆模式
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 恢复系数
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++; 
				// 事项句代码
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 推图名
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 计算量标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 倍率
				tmpint = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 显示标志
				tmpint = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发终端号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发序号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发系数
				tmpdouble = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpdouble ) );
				col ++;
				// 转发遥控
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 上级转发
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// VQC转发
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 模拟屏转发
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;

				ptstart +=func.recorderlen;
			}
		}
		retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	}
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}

int CDbOperate::InsertYxData( )
{
	int i, j, k, col;
	
	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;
	
	int tmpint;
	char tmpstr[ 100 ];

	int yxno;
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "遥信参数表" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}
	
	func.func = INS_RECORDERS;
	func.roxnum = YXNUM;
	pt = ( char* )malloc( func.recorderlen * func.roxnum );
	int ptcnt = 0;
	int ptstart = 0;
	
	
	for( i = 0; i < m_StationNum; i ++ )
	{
		yxno = 0;
		for( j = 0; j < m_PointNum / YXNUM ; j ++ )
		{
			ptstart = 0;
			memset( pt, 0, func.recorderlen * func.roxnum );
			for( k = 0; k < YXNUM; k ++ )
			{
				col = 0;
				ptcnt = ptstart;
				//SUBID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DTYPEID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				//DEVID
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;    
				// 代码
				switch( k )
				{
				case 0:
					{
						sprintf( tmpstr, "Ktest%03d%03dKg", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03d", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;	
					}
					break;
				case 1:
					{
						sprintf( tmpstr, "Ktest%03d%03dDz", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03d刀闸", i + m_startRtu, j + 1 );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				default:
					{
						sprintf( tmpstr, "Ktest%03d%03d%04dBh", i + m_startRtu, j + 1, yxno );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
						sprintf( tmpstr, "测试站%03d开关%03d保护%04d", i + m_startRtu, j + 1, yxno );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				}
				// 安全级
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;    
				// 设备代码
				sprintf( tmpstr, "Ktest%03d%03d", i + m_startRtu, j + 1 );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 旁路开关代码
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 类型
				switch( k )
				{
				case 0:
					{
						tmpint = 300;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++;  
					}
					break;
				case 1:
					{
						tmpint = 500;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				default:
					{
						tmpint = 600;
						dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
						col ++; 
					}
					break;
				}
				// 是否追忆
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 是否遥控
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 终端序号
				tmpint = i + m_startRtu;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥信序号
				tmpint = yxno ++ ;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 次遥信序号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 双遥信标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 主遥信标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 双遥信计算方式
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥控序号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 调度模式
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥控方式
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 事项处理方式
				sprintf( tmpstr, "event" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 推图名
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// SOE句
				sprintf( tmpstr, "SOE" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 事项句
				switch( k )
				{
				case 0:
				case 1:
					{
						sprintf( tmpstr, "KgChg" );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				default:
					{
						sprintf( tmpstr, "ProtChg" );
						dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
						col ++;
					}
					break;
				}
				// 事故限值
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 判事故方式
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 判事故死区
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 追忆组号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 事故过程名
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 控合过程名
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 控分过程名
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 遥控返校时间限
				tmpint = 30;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥控发令时间限
				tmpint = 30;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥控执行时间限
				tmpint = 30;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++; 
				// 遥控事项句
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 计算量标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 取反标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 是否统计
				tmpint = 1;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发标志
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 扩展遥控
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 遥控校验码
				sprintf( tmpstr, "" );
				dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
				col ++;
				// 转发终端号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发序号
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 转发遥控
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 上级转发
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// VQC转发
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 模拟屏转发
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;
				// 防抖处理
				tmpint = 0;
				dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
				col ++;

				ptstart += func.recorderlen;
			}
			retcode = dbsingle.InsertRcds(&func,datafmt,pt);
		}
	}
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );
	
	return retcode;
}


int CDbOperate::DBStart()
{
	LOG_INFO *loginfo;
	int retcode;
	
	loginfo=(LOG_INFO *)malloc (sizeof(LOG_INFO));
	memset(loginfo,0,sizeof(LOG_INFO));
	
	strcpy(loginfo->servername,"nbdb");
	
	strcpy(loginfo->username,"modeldb");
	strcpy(loginfo->password,"supernb");
	
	loginfo->useflag=1;
	
	retcode=dbsingle.Open(0,loginfo,0,1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n错误：服务器%s打开失败!\n",loginfo->servername);
		fprintf(stderr,"请检查 1、服务器%s是否运行\n",loginfo->servername);
		fprintf(stderr,"       2、客户端配置是否正确\n");
		
		if (loginfo) free (loginfo);
		return retcode;
	}
	
	
	if (loginfo) free(loginfo);
	return retcode;
}

int CDbOperate::InsertStaData()
{
	int i, col;

	FUNC_STR func ;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL ;

	int tmpint;
	char tmpstr[ 100 ];
	
	
	CS_RETCODE  retcode = CS_SUCCEED ;
	
	memset(&func,0,sizeof(FUNC_STR));
	
	func.func = GET_TABLE_INFO;
	
	strcpy( func.dbname, DB_NAME );
	strcpy( func.table , "站所线系参数表" );
	
	retcode = dbsingle.GetTableInfo(&func,&datafmt);
	if ( retcode == CS_FAIL)
	{
		if ( datafmt ) free ( datafmt );
		return 0;
	}

	func.func = INS_RECORDERS;
	func.roxnum = m_StationNum;
	pt = ( char* )malloc( func.recorderlen * m_StationNum );
	int ptcnt = 0;
	int ptstart = 0;

	for( i = 0; i < m_StationNum; i ++ )
	{
		col = 0;
		ptcnt = ptstart;

		//SUBID
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//代码
		sprintf( tmpstr, "test%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//描述
		sprintf( tmpstr, "测试站%03d", i + m_startRtu );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//事故总标志
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//事故总死区
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//安全起始年
		tmpint = 2012;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//安全起始月
		tmpint = 2;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//安全起始日
		tmpint = 10;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//接线图名
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//分组序号
		tmpint = 1;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//备注
		sprintf( tmpstr, "" );
		dbsingle.FillData( datafmt + col, pt, ptcnt, tmpstr );
		col ++;
		//区域序号
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//所属态
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//区域序号
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;
		//区域序号
		tmpint = 0;
		dbsingle.FillData( datafmt + col, pt, ptcnt, (char*)( &tmpint ) );
		col ++;

		ptstart += func.recorderlen;
	}

	retcode = dbsingle.InsertRcds(&func,datafmt,pt);
	
	if( pt ) free( pt );
	
	if ( datafmt ) free( datafmt );

	return retcode;
}

int CDbOperate::InsertData()
{
	int retcode = DBStart();
	if( retcode != CS_SUCCEED )
		return retcode;

	retcode = InsertStaData();
	if( retcode != CS_SUCCEED )
		return retcode;
	
	retcode = InsertRtuData();
	if( retcode != CS_SUCCEED )
		return retcode;

	retcode = InsertChanData();
	if( retcode != CS_SUCCEED )
		return retcode;

	retcode = InsertDevData();
	if( retcode != CS_SUCCEED )
		return retcode;


	retcode = InsertYcData();
	if( retcode != CS_SUCCEED )
		return retcode;
	retcode = InsertYxData();

	return retcode;
}

int main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("datagen,1.1");
				printf("datagen, 1.1");
				return 0;
			}
		}
	}
	NetDbg dbg;
	NetStatus cfg;

	if( argc < 5 )
	{
		printf( "datagen 批量产生测试数据工具\n" );
		printf( "参数说明：\n" );
		printf( "datagen [起始通道号] [起始终端号] [厂站个数] [数据点数]\n" );
		printf( "实例：\n" );
		printf( "datagen 10 20 30 40\n" );
		printf( "产生的通道号为：10 到 39 共30个通道\n" );
		printf( "产生的终端号为：20 到 49 共30个终端\n" );
		printf( "产生的设备数为：30 * (40 / 10) = 120个设备\n" );
		printf( "产生的遥测数为：30 * (40 /  2) = 600个遥测\n" );
		printf( "产生的遥信数为：30 * 40 = 1200个遥信\n" );
		exit(0);
	}

	int startchan = atoi(argv[1]);
	int startRTU = atoi(argv[2]);
	int stanum = atoi(argv[3]);
	int pointnum = atoi(argv[4]);

	if( stanum == 0 || pointnum == 0 )
	{
		return 1;
	}

	CDbOperate dbOperate(startchan, startRTU, stanum, pointnum );

	dbg.RegProc("数据库工具");

	dbOperate.InsertData();

	dbg.UnRegProc();

	return 1;
}
