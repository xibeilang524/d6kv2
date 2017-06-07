#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trenddb/trenddb_api.h"
#include "trenddb/trenddb_api_i.h"
#include "trenddb/trenddb_err.h"
#include "trenddb/trenddbapi.h"




void TrendDBAPI::DBStartupAPI(void)
{
	DB_StartupAPI();
}

int TrendDBAPI::DBInitNameServers(char* name_servers, size_t count, int argc, char** argv)
{
    NET_ADDRESS naddr;
	strcpy(naddr, name_servers);
	int nRet = DB_InitNameServers(&naddr, 1, NULL, NULL);
	if(nRet)
	{
		printf("DB_InitializeConnection error: %d\n", nRet);
	}
	return nRet;
}

int TrendDBAPI::DBCreateConnection(const char* connection, int bFireWall, int bCacheEnabled, const char* user, const char* passwd)
{
	int nRet = DB_CreateConnection(connection, bFireWall, bCacheEnabled, user, passwd);
	if(nRet < 0)
	{
		printf("DB_CreateConnection error: %d\n", nRet);
		printf("%s, %s\n", connection, DB_GetErrorMsg(nRet));
		return nRet;
	}
	else
	{
		return nRet;
	}
}


int TrendDBAPI::DBCreateYcPoints(const char* connection, char* name,char* desc,char* unit,int histDays,short histDepth)
{
	int nConnId = DB_GetConnectionID(connection);
	if(nConnId < 0)
	{
		return -1;
	}
	char temp_str[1024];
	DB_TagConfig point;
	memset(&point, 0, sizeof(point));
	strcpy(point.name,name);
	strcpy(point.desc,desc);
	strcpy(point.unit,unit);
	strcpy(point.addr,name);

	point.comDev = 0;//0.05f;
	point.precision = 6;
	point.type = TAG_CONTINUOUS;
	point.histDays = histDays;
	point.archive = 1;
	point.statistics = 1;
	point.mirror = 0;
	point.histDepth = histDepth;
	int nRet = DBC_AddTag(nConnId, &point);
	return nRet;
}


int TrendDBAPI::DBCreateYxPoints(const char* connection, char* name,char* desc,int histDays)
{
	int nConnId = DB_GetConnectionID(connection);
	if(nConnId < 0)
	{
		return -1;
	}
	char temp_str[1024];
	DB_TagConfig point;
	memset(&point, 0, sizeof(point));
	strcpy(point.name,name);
	strcpy(point.desc,desc);
	strcpy(point.addr,name);

	//point.comDev = 0;//0.05f;
	point.precision = 1;
	point.type = TAG_DISCRETE;
	point.histDays = histDays;
	point.archive = 1;
	point.statistics = 0;
	point.mirror = 0;
	//point.histDepth = histDepth;
	int nRet = DBC_AddTag(nConnId, &point);
	return nRet;
}

int TrendDBAPI::DBSaveYcData(char* connection,float value,char* ycname)
{
	DB_NAME name;
	DB_Value val;
	val.status = 1;
	val.time = DB_TimeNow();
	val.value = value;
	sprintf(name, "%s",ycname );
	int nRet = DB_UpdateValueByName(connection, &name, &val, 1);
	if(0 != nRet || nRet != ERROR_WAIT_QUEUE)
	{
		char temp_str[2048];
		sprintf(temp_str,"%s DB_UpdateValueByName failed due to %s", name, DB_GetErrorMsg(nRet));
		DB_LogMsg(temp_str);
	}
	return nRet;
}


int TrendDBAPI::DBSaveYxData(char* connection,unsigned char value,char* yxname)
{
	DB_NAME name;
	DB_Value val;
	val.status = 1;
	val.time = DB_TimeNow();
	val.value = value;
	sprintf(name, "%s",yxname );
	int nRet = DB_UpdateValueByName(connection, &name, &val, 1);
	if(0 != nRet || nRet != ERROR_WAIT_QUEUE)
	{
		char temp_str[2048];
		sprintf(temp_str,"%s DB_UpdateValueByName failed due to %s", name, DB_GetErrorMsg(nRet));
		DB_LogMsg(temp_str);
	}
	return nRet;
}



void TrendDBAPI::DBClose(char* connection)
{
	DB_CloseConnection(connection);
	DB_ShutdownAPI();
}

void TrendDBAPI::DBFlushConnectionBuffer(char* connection)
{
	DB_FlushConnectionBuffer(connection);
}