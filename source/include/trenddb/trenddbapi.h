#ifndef TRENDDBAPI_H_
#define TRENDDBAPI_H_

#ifdef WIN32
#ifdef TRENDDBAPI_DLL
#define	TRENDDBAPI_EXPORT __declspec(dllexport) 
#else
#define	TRENDDBAPI_EXPORT __declspec(dllimport) 
#endif
#else
#define	TRENDDBAPI_EXPORT
#endif


class TRENDDBAPI_EXPORT TrendDBAPI
{
public:
	static void DBStartupAPI(void);
	static int DBInitNameServers(char* name_servers, size_t count, int argc, char** argv);
	static int DBCreateConnection(const char* connection, int bFireWall, int bCacheEnabled, const char* user, const char* passwd);
	static int DBCreateYcPoints(const char* connection, char* name,char* desc,char* unit,int histDays,short histDepth);
	static int DBCreateYxPoints(const char* connection, char* name,char* desc,int histDays);
	static int DBSaveYcData(char* connection,float value,char* ycname);
	static int DBSaveYxData(char* connection,unsigned char value,char* yxname);
	static void DBClose(char* connection);
	static void DBFlushConnectionBuffer(char* connection);
};

#endif