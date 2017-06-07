#include "tpdata.h"
#include "streamdata.h"
#include "rdb/rdbapi.h"

///////////
//注意实时表指针的移动！
//需要改变时改动程序
////////////////////////////////

extern CStreamData g_stream;

CTpData::CTpData()
{
	m_rtDataNum = 0;
	m_rtData = NULL;
	m_topoRcd = NULL;
	m_topoRcdNum = 0;
	m_analogPData = NULL;
	m_analogPNum = 0;
}

CTpData::~CTpData()
{
	if(m_rtData != NULL) 
	{
		delete[] m_rtData;
		m_rtData = NULL;
	}
	if(m_analogPData != NULL) 
	{
		delete[] m_analogPData;
		m_analogPData = NULL;
	}

}

int CTpData::readTpData()
{
	Rdb_LocalTab rdb;
	RDB_BTBL_TAB_STRU tabInfo;

	m_topoRcdNum = 0;
	m_rtDataNum = 0;
	m_analogPNum = 0;
	void * pTmp = NULL;
	if(rdb.RdbOpenTab("scdtp") != 0)
		return FALSE;
	rdb.RdbGetTableInfo(&pTmp,&tabInfo);//取表的首地址，记录个数，记录长度
	rdb.RdbCloseTab();
	if(pTmp == NULL) 
		return FALSE;

	m_topoRcd = (TPRCD *)pTmp;
	m_topoRcdNum = tabInfo.rcdcount;

	m_rtData = new RTDATA[ m_topoRcdNum ];
	memset( m_rtData, 0, sizeof( RTDATA ) * m_topoRcdNum );

	//scdtp表中有实时数据代码的记录
	int i;
	for( i = 0; i < m_topoRcdNum; i ++ )
	{
		m_name2ID[ m_topoRcd[ i ].name ] = i;
		if( m_topoRcd[ i ].obligate[ 0 ] == '\0' )
			continue;
		strcpy( m_rtData[ m_rtDataNum ].devName, m_topoRcd[ i ].name );
		strcpy( m_rtData[ m_rtDataNum ].name, m_topoRcd[ i ].obligate );
		m_rtData[ m_rtDataNum ].devType = m_topoRcd[ i ].type;
		m_rtDataNum ++;
		
	}

	Rdb_SqlCli rl;
	rl.RdbSelectFrom("analog");
	rl.RdbOrderBy("ycno");
	rl.RdbSelectField("name");
	rl.RdbSelectField("describe");
	rl.RdbSelectField("devname");
	rl.RdbSelectField("baseobjtype");
	rl.RdbSelectField("value");
	int	ret = rl.RdbSelCmd(1,"scada","scada");
	if(ret != RDB_OK)	return FALSE;
	int	RecNumber=rl.rdbgetrcdcnt();
	if (RecNumber==0) return FALSE;
	int baseObjType;
	m_analogPData = new ANALOG_P_DATA[ RecNumber ];
	memset( m_analogPData, 0, sizeof( ANALOG_P_DATA ) * RecNumber );

	for (i = 0; i < RecNumber; i++)
	{
		baseObjType = rl.RdbGetVal_uint(i,"baseobjtype");
		if(baseObjType != BASEOBJTYPE_PWOER)
			continue;
		strcpy(m_analogPData[m_analogPNum].name, rl.RdbGetString(i,"name"));
		strcpy(m_analogPData[m_analogPNum].desc, rl.RdbGetString(i,"describe"));
		strcpy(m_analogPData[m_analogPNum].devName, rl.RdbGetString(i,"devname"));
		m_analogPData[m_analogPNum].baseobjtype = rl.RdbGetVal_uint(i,"baseobjtype");
		m_analogPData[m_analogPNum].value = rl.RdbGetVal_float(i,"value");		
		m_analogPNum++;			
	}

	return TRUE;
}
