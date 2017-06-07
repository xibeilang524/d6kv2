// #include "stdafx.h"
#include "sysstr.h"
#include "globaldef.h"
#include "streamdata.h"
#include "tpdata.h"
#include "manageevent.h"
#include "initialize.h"

#include "scd/devdef.h"
#include "db/dbapi.h"
#include "net/netapi.h"
#include "math.h"

#include "../../gui/include/ddisp/TopoApi.h"

#include <QString>

NetDbg	   dbg;
extern CTpData g_topoData;
extern CManageEvent g_manageEvent;
extern CStreamData g_stream;

#if defined (__unix)
void StreamZF_CallBack(void *buf,int ord,int len,int no)
#elif defined (WIN32)
void STDCALL StreamZF_CallBack(void *buf,int ord,int len,int no)
#endif
{
	g_stream.readStreamData();
}

CStreamData::CStreamData()
{
	for(int i = 0 ; i < REALDATA_BUF_SIZE ; i++)
		bUsing[i] = TRUE;
	m_firstFlag = TRUE;
	m_pStreamData = NULL;

	if( !m_dbApi.Open( DB_OP_WR, FALSE, FALSE ) )
	{
		return;
	}
}

CStreamData::~CStreamData()
{
	m_dbApi.Close();
}

int CStreamData::initStreamPara()
{	
	//流参数，开关状态，刀闸状态，设备带电状态
	int paranums = g_topoData.m_rtDataNum + g_topoData.m_topoRcdNum + g_topoData.m_analogPNum;	

	if (paranums == 0)
		return 0;

	int i;
	for( i = 0 ; i < REALDATA_BUF_SIZE ; i ++)
	{
		m_rtDataState[ i ] = new unsigned char[ g_topoData.m_rtDataNum ];
		memset( m_rtDataState[ i ], 0, g_topoData.m_rtDataNum );
		m_tpDevActive[ i ] = new unsigned char[ g_topoData.m_topoRcdNum ];
		memset( m_tpDevActive[ i ], 0, g_topoData.m_topoRcdNum );
		
		m_analogPValue[ i ] = new float[ g_topoData.m_analogPNum ];
		memset( m_analogPValue[ i ], 0, g_topoData.m_analogPNum );
	}

	memset( m_saveRdataTime, 0, sizeof( int ) * REALDATA_BUF_SIZE ); 

	m_pStreamData = new RDB_FIELD_STRU[paranums];
	memset( m_pStreamData, 0, sizeof( RDB_FIELD_STRU ) * paranums );

	memset(bUsing,0,REALDATA_BUF_SIZE*sizeof(int));

	//流参数，开关状态，刀闸状态
	for ( i = 0; i < g_topoData.m_rtDataNum; i++)
	{
		( m_pStreamData + i) -> groupno = 1;
		if( g_topoData.m_rtData[ i ].devType == DEVTYPE_BREAKER )
			strcpy( ( m_pStreamData + i )->tabname, "break" );
		if( g_topoData.m_rtData[ i ].devType == DEVTYPE_SWITCH )
			strcpy( ( m_pStreamData + i )->tabname, "switch" );
		strcpy( ( m_pStreamData + i )->objname, ( g_topoData.m_rtData + i )->name );
		strcpy( ( m_pStreamData + i )->fldname, "value" );
	}

	//流参数，设备带电状态
	for( i = g_topoData.m_rtDataNum; i < g_topoData.m_topoRcdNum + 
		g_topoData.m_rtDataNum; i ++ )
	{
		( m_pStreamData + i) -> groupno = 1;
		strcpy( ( m_pStreamData + i )->tabname, "scdtp" );
		strcpy( ( m_pStreamData + i )->objname, 
			( g_topoData.m_topoRcd + i - g_topoData.m_rtDataNum )->name );
		strcpy( ( m_pStreamData + i )->fldname, "active" );
	}

	//流参数，analog 功率实时值
	for( i = g_topoData.m_rtDataNum + g_topoData.m_topoRcdNum; 
		i < g_topoData.m_analogPNum + g_topoData.m_topoRcdNum + g_topoData.m_rtDataNum ; i ++ )
	{
		( m_pStreamData + i) -> groupno = 1;
		strcpy( ( m_pStreamData + i )->tabname, "analog" );
		strcpy( ( m_pStreamData + i )->objname, 
			( g_topoData.m_analogPData + i - g_topoData.m_rtDataNum - g_topoData.m_topoRcdNum )->name );
		strcpy( ( m_pStreamData + i )->fldname, "value" );
	}

	m_curWritePointer = 0;

	//每隔2秒回调读取数据函数
	if( m_rdbStream.RdbSetStreamPara( "scada", "scada", 2, paranums, 
		m_pStreamData, 1, 
#ifdef WIN32
		(HWND)20000, 
#endif	
		StreamZF_CallBack ) != RDB_OK )
		return 1;

	return 0;
}

void CStreamData::stopStreamData()
{
	if( g_topoData.m_rtDataNum != 0)
	{
		m_rdbStream.RdbStopFresh();
		if( m_pStreamData != NULL )
		{
			delete[] m_pStreamData;
			m_pStreamData = NULL;
		}

		for(int i = 0 ; i < REALDATA_BUF_SIZE; i++)
		{
			while(bUsing[i] != FALSE)
				;
			bUsing[i] = TRUE;
			if( m_rtDataState[ i ] != NULL )
			{
				delete[] m_rtDataState[ i ];
				m_rtDataState[ i ] = NULL;
			}
			if( m_tpDevActive[ i ] != NULL )
			{
				delete[] m_tpDevActive[ i ];
				m_tpDevActive[ i ] = NULL;
			}
			if( m_analogPValue[ i ] != NULL )
			{
				delete[] m_analogPValue[ i ];
				m_analogPValue[ i ] = NULL;
			}

			bUsing[i] = FALSE;
		}
	}
}
void CStreamData::readStreamData()
{
	if( g_topoData.m_rtDataNum == 0 )
		return;

	int timeval;

	//如果不是第一次流数据需要把前一个断面状态复制到当前状态
	if( !m_firstFlag )
	{
		while(bUsing[m_curWritePointer] != FALSE)
			;
		bUsing[m_curWritePointer] = TRUE;//开始写入实时数据	

		memcpy(*( m_rtDataState + ( m_curWritePointer + 1 ) % REALDATA_BUF_SIZE ),
			*( m_rtDataState + m_curWritePointer ),
			g_topoData.m_rtDataNum * sizeof( unsigned char ) );
		memcpy(*( m_tpDevActive + ( m_curWritePointer + 1 ) % REALDATA_BUF_SIZE ),
			*( m_tpDevActive + m_curWritePointer ),
			g_topoData.m_topoRcdNum * sizeof( unsigned char ) );
		memcpy(*( m_analogPValue + ( m_curWritePointer + 1 ) % REALDATA_BUF_SIZE ),
			*( m_analogPValue + m_curWritePointer ),
			g_topoData.m_analogPNum * sizeof( float ) );

		bUsing[ m_curWritePointer ] = FALSE;
		m_curWritePointer = ( m_curWritePointer + 1 ) % REALDATA_BUF_SIZE;
	}

	while(bUsing[m_curWritePointer] != FALSE)
		;
	bUsing[ m_curWritePointer ] = TRUE;

	//读取实时数据和设备带电状态
	Rdb_MultiTypeValue value;
	unsigned char tmpCharValue;
	int tmpIntValue;
	float tmpFloatValue;
	int i;
	if( m_rdbStream.RdbGetFieldValue( 1, &value ) >= RDB_OK )
	{
		*(m_saveRdataTime + m_curWritePointer) = time( NULL );
		int valnum = value.RdbGetValNums();
		for( i = 0; i < valnum; i++ ) 
		{
			int no = value.RdbGetValOrderno(i);
			if( no < 0 || no >= g_topoData.m_rtDataNum + g_topoData.m_topoRcdNum + g_topoData.m_analogPNum )
				continue;
			if( no >= g_topoData.m_rtDataNum  && no < g_topoData.m_rtDataNum + g_topoData.m_topoRcdNum)
			{
				tmpIntValue = value.RdbGetVal_int( i );
				m_tpDevActive[ m_curWritePointer ][ no - g_topoData.m_rtDataNum ] = tmpIntValue;
			}
			if( no >= 0 && no < g_topoData.m_rtDataNum )
			{
				value.RdbGetVal( i, RDB_DATATYPE_UCHAR, &tmpCharValue );
				 m_rtDataState[ m_curWritePointer ][ no ] = tmpCharValue;
			}
			if (no >= g_topoData.m_rtDataNum + g_topoData.m_topoRcdNum)
			{
				tmpFloatValue = value.RdbGetVal_float(i);
				m_analogPValue[ m_curWritePointer ][no- g_topoData.m_rtDataNum - g_topoData.m_topoRcdNum] = tmpFloatValue;
			}
		}
	}
	else
	{
#ifdef _WINDOWS
		TRACE("实时流数据刷新错误！\n");
#else
		printf("--------------实时流数据刷新错误！-----------\n");
#endif
	}

// 	for( i = 0; i < g_topoData.m_topoRcdNum; i ++ )
// 	{
// 		m_tpDevActive[ m_curWritePointer ][ i ] = g_topoData.m_topoRcd[ i ].active;
// 	}
	bUsing[m_curWritePointer] = FALSE;

	printf( "-------收到流数据开始-------" );
	SYS_TIME currentTime;
	TimeProc timeProc;
	timeProc.GetTime(currentTime);//实时数据存储时间
	printf( "时间: %04d-%02d-%02d %02d:%02d:%02d.%03d\n", 
		(int)currentTime.year, currentTime.mon, currentTime.day, currentTime.hour,
		currentTime.min, currentTime.sec, (int)currentTime.ms );
	int j;
	for( j = 0; j < g_topoData.m_rtDataNum; j ++ )
	{
		if( m_rtDataState[ m_curWritePointer ][ j ] == 0 )
		{
			printf( "%s： 分\n", g_topoData.m_rtData[ j ].name );
		}
		else
		{
			printf( "%s： 合\n", g_topoData.m_rtData[ j ].name );
		}
	}

	if( !m_firstFlag )
	{
		saveEndData();
		removeEndData();
	}

	if( m_firstFlag )
	{
		m_firstFlag = FALSE;
	}
}

int CStreamData::saveStartData()
{
	CUTEVENT event;
	if( !g_manageEvent.getEvent( event ) )
		return FALSE;

	//找到事故前段面
	int i;
	int cur, last;
	int rightPoint = -1;
	for( i = 0; i < REALDATA_BUF_SIZE; i ++ )
	{
		cur = ( m_curWritePointer - i + REALDATA_BUF_SIZE ) % 
			REALDATA_BUF_SIZE ;
		if( event.starttime > m_saveRdataTime[ cur ] &&
			m_saveRdataTime[ cur ] > 0 )
		{
			rightPoint = cur;
			break;
		}
	}

	if( rightPoint < 0 )
		return FALSE;

	//事故前断面的设备代码到带电状态、设备代码到实时数据
	QMap< QString, int > activemap;
	QMap< QString, int > statusmap;
	QMap<QString, float> devname2valuePMap;
	devname2valuePMap.clear();
	for( i = 0; i < g_topoData.m_topoRcdNum; i ++ )
	{
		activemap[ g_topoData.m_topoRcd[ i ].name ] = m_tpDevActive[ rightPoint ][ i ];
	}
	for( i = 0; i < g_topoData.m_rtDataNum; i ++ )
	{
		statusmap[ g_topoData.m_rtData[ i ].devName ] = m_rtDataState[ rightPoint ][ i ];
	}
	for( i = 0; i < g_topoData.m_analogPNum; i ++ )
	{
		devname2valuePMap[ g_topoData.m_analogPData[ i ].devName ] = m_analogPValue[ rightPoint ][ i ];
	}
	
	

	//获得本次停电设备
	TopoApi scdtp;
	char **devlist;
	int devNum = 0;
	devNum = scdtp.GetLoadPathFromBefDev( event.devName, event.type, &devlist,
		activemap, statusmap );

	dbg.PutDbg(0,"%s开关停电设备数：%d",event.devName,devNum);
	if( devNum <= 0 )
		return FALSE;

	TimeProc timeProc;
	SYS_TIME currentTime;
	timeProc.GetTime(currentTime);//实时数据存储时间
	int year = currentTime.year;
	int month = currentTime.mon;

	//停电设备存盘
	char strSQL[1024];
	int index;
	float valueP;
	FUNC_STR func;
	func.func=ISQL_COMMAND;
	strcpy(func.dbname,"modeldb");
	int ret = 0;
	for( i = 0; i < devNum; i ++ )
	{
		if( !g_topoData.m_name2ID.contains( devlist[ i ] ) )
			continue;
		index = g_topoData.m_name2ID[ devlist[ i ] ];
		m_name2StartTime[ devlist[ i ] ] = event.starttime;
		if (devname2valuePMap.keys().contains(devlist[i]))
		{
			valueP = devname2valuePMap[devlist[i]];
		}
		else
		{
			valueP = 0.00;
		}
		if(event.cutType == 1)
		{
			sprintf(strSQL, "insert into 停电管理事项表%.4d%.2d  values('%s',%d,'%s',%d,%d,%d,'%s',%d,%.2f)",
				year, month, devlist[ i ],g_topoData.m_topoRcd[ index ].type,
				g_topoData.m_topoRcd[ index ].hstation,g_topoData.m_topoRcd[ index ].feederno,
				event.starttime,0,event.devName, event.cutType, valueP );
			ret  = m_dbApi.IsqlCommand( &func, strSQL ); 
			if (!ret)
			{
				sprintf(strSQL, "update 停电管理事项表%.4d%.2d set 停电类型 = %d \
								where 代码 = '%s' and  停电开始时间 = %d",
								year, month, event.cutType, devlist[ i ], event.starttime);
				ret  = m_dbApi.IsqlCommand( &func, strSQL );				
			}
			g_topoData.m_topoRcd[ index ].lossreason = FAULT_POWERCUT;
		}
		else
		{
			sprintf(strSQL, "insert into 停电管理事项表%.4d%.2d  values('%s',%d,'%s',%d,%d,%d,'%s',%d,%.2f)",
				year, month, devlist[ i ],g_topoData.m_topoRcd[ index ].type,
				g_topoData.m_topoRcd[ index ].hstation,g_topoData.m_topoRcd[ index ].feederno,
				event.starttime,0,event.devName, event.cutType ,valueP);
			ret  = m_dbApi.IsqlCommand( &func, strSQL ); 
			g_topoData.m_topoRcd[ index ].lossreason = NORMAL_POWERCUT;
		}
	}

	//停电设备状态更新
}

int CStreamData::saveEndData()
{
	//停电结束信息存盘
	char strSQL[1024];
	FUNC_STR func;
	func.func=ISQL_COMMAND;
	strcpy(func.dbname,"modeldb");

	int i; 
	int curTime = time( NULL );

	TimeProc timeProc;
	SYS_TIME currentTime;
	timeProc.GetTime(currentTime);//实时数据存储时间
	int year = currentTime.year;
	int month = currentTime.mon;

	for( i = 0; i < g_topoData.m_topoRcdNum; i++)
	{
		if( !m_name2StartTime.contains( g_topoData.m_topoRcd[ i ].name ) )
			continue;

		if( m_tpDevActive[ m_curWritePointer ][ i ] > 0 )
		{
			sprintf(strSQL, "update 停电管理事项表%.4d%.2d set 停电结束时间 = %d \
							where 代码 = '%s' and  停电开始时间 = %d",
							year, month, curTime, g_topoData.m_topoRcd[ i ].name, 
							m_name2StartTime[ g_topoData.m_topoRcd[ i ].name ] );							
			m_dbApi.IsqlCommand( &func, strSQL );

			m_endDevList.append( g_topoData.m_topoRcd[ i ].name );
			g_topoData.m_topoRcd[ i ].lossreason = NORMAL_POWERCUT;
		}
	}

	return TRUE;
}

void CStreamData::removeEndData()
{
	int i;
	for( i = 0; i < m_endDevList.count(); i ++ )
	{
		m_name2StartTime.remove( m_endDevList[ i ] );
	}
	m_endDevList.clear();
}