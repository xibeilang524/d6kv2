#ifndef RTDB_API_H
#define RTDB_API_H

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DllExport
#ifdef WIN32
	#ifdef TRENDDB_EXPORT
		#define DllExport   __declspec( dllexport )
	#else
		#define DllExport   __declspec( dllimport )		
	#endif
#else
	#define DllExport
#endif
#endif

#if (UINT_MAX) == 4294967295U
	typedef unsigned int DB_UINT32;
#elif (ULONG_MAX) == 4294967295U
	typedef unsigned long DB_UINT32;
#else
	#error: unsupported int size, must be updated for this platform!
#endif

#if (UINT_MAX) == 65535U
	typedef unsigned int DB_UINT16;
#elif (USHRT_MAX) == 65535U
	typedef unsigned short DB_UINT16;
#else
	#error: unsupported short size, must be updated for this platform!
#endif

#ifdef WIN32
#if (ULLONG_MAX) == 0xffffffffffffffffui64
	typedef unsigned long long DB_UINT64;
#elif (ULONG_MAX) == 0xffffffffffffffffui64
	typedef unsigned long DB_UINT64;
#elif defined(__int64)
	typedef unsigned _int64 DB_UINT64;
#else
	#error: unsupported int64 size, must be updated for this platform!
#endif
#else
#if (ULONG_MAX) == 18446744073709551615UL
	typedef unsigned long DB_UINT64;
#elif (ULLONG_MAX) == 18446744073709551615UL
	typedef unsigned long long DB_UINT64;
#else
	#error: unsupported int64 size, must be updated for this platform!
#endif
#endif

typedef unsigned char DB_UINT8;

typedef char DB_NAME[64];
typedef char DB_ADDR[64];
typedef char DB_DESC[256];
typedef char DB_UNIT[16];
typedef char NET_ADDRESS[128];
typedef char SERVER_NAME[128];
typedef char DB_TIME_STRING[128];

struct DB_Time
{
	DB_UINT32 hours, usecs;
};

struct DB_Value
{
	DB_Time time;
	DB_UINT32 status;
	float value;
};

enum DB_TagType
{
	TAG_CONTINUOUS = 1,
	TAG_DISCRETE = 2,
	TAG_BLOB = 3
};

enum DB_AlarmRule
{
	ALARM_RULE_VALUE_CHNG = 0x01,
	ALARM_RULE_LIMIT_EXCD = 0x02,
	ALARM_RULE_STATUS_CHNG = 0x04,
	ALARM_RULE_LIMIT_TIMO = 0x08,
	ALARM_RULE_STATUS_TIMO = 0x10
};

enum DB_AlarmState
{
	ALARM_STATE_VALUE_NORM = 0x00,
	ALARM_STATE_VALUE_CHNG = 0x01,
	ALARM_STATE_HIHI_EXCD = 0x02,
	ALARM_STATE_HIGH_EXCD = 0x03,
	ALARM_STATE_LOLO_EXCD = 0x04,
	ALARM_STATE_LOW_EXCD = 0x05,
	ALARM_STATE_LIMIT_TIMO = 0x08,
	ALARM_STATE_STATUS_TIMO = 0x10
};

#define ALARM_STATE_MASK1 0x07
#define ALARM_STATE_MASK2 0x08
#define ALARM_STATE_MASK3 0x10

struct DB_AlarmInfo
{
	DB_UINT8 alarmState, alarmReason;
	DB_Value oldVal, newVal;
	float loloLim, lowLim, highLim, hihiLim;
	DB_UINT16 limitTimeout, statusTimeout;
	DB_UINT32 normalStatus;
};

typedef void (*DB_AlarmCallback)(const char* server, DB_UINT32 tagId, void* arg, const DB_AlarmInfo* pInfo);

struct DB_TagConfig
{
	DB_NAME name;
	DB_ADDR addr;
	DB_DESC desc;
	DB_UNIT unit;
	DB_UINT32 tagId;
	float comDev, expDev, loloLim, hihiLim, lowLim, highLim;
	DB_UINT32 histDays, normalStatus;
	DB_UINT16 histDepth, limitTimeout, statusTimeout;
	DB_TagType type;
	DB_UINT8 alarmRule;
	DB_UINT8 mirror:1, archive:1, statistics:1, precision:5;
	DB_UINT8 grpRD:1, grpWR:1, otherRD:1, otherWR:1;
};

struct DB_TagInfo
{
	DB_TagConfig config;
	char owner[64];
	char group[64];
	DB_Time createTime, modifiedTime;
	DB_UINT32 ownerId;
};

struct DB_ConfigBlock
{
	DB_UINT16 count;
	DB_TagConfig tags[100];
};
typedef int DB_ConfigResults[100];

struct DB_ConfigPosition
{
	char pos_data[64];
};

struct DB_HistoryStatus
{
	DB_Time start, end;
};

struct DB_HistoryPosition
{
	DB_Time start, end, current;
};

struct DB_Profile
{
	DB_Value min, max;
};

struct DB_HistValueBlock
{
	DB_Value values[96*1024/sizeof(DB_Value)];
	DB_UINT8 calculated[96*1024/sizeof(DB_Value)];
	size_t count;
};


struct DB_HistProfileBlock
{
	DB_Profile profiles[128*1024/sizeof(DB_Profile)];
	size_t count;
};

struct DB_ClientStatus
{
	char address[32], user[32], command[128];
	double requests, replies;
	DB_Time start;
};
typedef DB_ClientStatus DB_ClientStatusBlock[500];

struct DB_ServerStatus
{
	DB_Time start;
	DB_UINT32 tagCount;
	DB_UINT64 rt, arch, insert, dataSize, indexSize;
	DB_UINT64 readCount, writeCount;
};

struct DB_BlobValue
{
	DB_Time time;
	char data[96*1024];
	size_t size;
};

struct DB_TagScript
{
	char data[96*1024];
	size_t size;
};

//connection name format: connection-name@server-name

DllExport int DB_SetEnvironment(const char* env);
DllExport const char* DB_GetEnvironment();

DllExport int DB_StartupAPI();
DllExport int DB_ShutdownAPI();
DllExport int DB_InitNameServers(NET_ADDRESS* name_servers, size_t count, int argc, char** argv);
DllExport int DB_SetServerAddress(NET_ADDRESS* servers, size_t count, int argc, char** argv);
DllExport int DB_ListServers(SERVER_NAME* servers, size_t count);
DllExport int DB_CreateConnection(const char* connection, int bFireWall, int bCacheEnabled, const char* user, const char* passwd);
DllExport int DB_CloseConnection(const char* connection);
DllExport int DB_GetConnectionStatus(const char* connection);
DllExport int DB_SetCallbackEnabled(const char* connection, int bDataEnabled, DB_AlarmCallback callback, void* callbackArg);

DllExport int DB_GetConnectionID(const char* connection);
DllExport int DB_FlushConnectionBuffer(const char* connection);

DllExport int DBC_CloseConnection(int connId);
DllExport int DBC_FlushConnectionBuffer(int connId);

DllExport int DB_SetDataImporting(const char* connection, int bDataImporting);
DllExport int DBC_SetDataImporting(int connId, int bDataImporting);

DllExport int DBC_GetConnectionStatus(int connId);
DllExport int DBC_SetCallbackEnabled(int connId, int bDataEnabled, DB_AlarmCallback callback, void* callbackArg);

DllExport int DB_GetTagInfoByName(const char* connection, const char* name, DB_TagInfo* pInfo, DB_UINT8 bComplete);
DllExport int DB_GetTagInfoByAddr(const char* connection, const char* addr, DB_TagInfo* pInfo, DB_UINT8 bComplete);
DllExport int DB_GetTagInfoByID(const char* connection, DB_UINT32 tagId, DB_TagInfo* pInfo, DB_UINT8 bComplete);

DllExport int DBC_GetTagInfoByName(int connId, const char* name, DB_TagInfo* pInfo, DB_UINT8 bComplete);
DllExport int DBC_GetTagInfoByAddr(int connId, const char* addr, DB_TagInfo* pInfo, DB_UINT8 bComplete);
DllExport int DBC_GetTagInfoByID(int connId, DB_UINT32 tagId, DB_TagInfo* pInfo, DB_UINT8 bComplete);

DllExport int DB_GetTagAttrByName(const char* connection, const char* name, void** handle);
DllExport int DB_GetTagAttrByAddr(const char* connection, const char* addr, void** handle);
DllExport int DB_GetTagAttrByID(const char* connection, DB_UINT32 tagId, void** handle);

DllExport int DBC_GetTagAttrByName(int connId, const char* name, void** handle);
DllExport int DBC_GetTagAttrByAddr(int connId, const char* addr, void** handle);
DllExport int DBC_GetTagAttrByID(int connId, DB_UINT32 tagId, void** handle);

DllExport int DB_GetTagScriptByName(const char* connection, const char* name, DB_TagScript* pScript, DB_UINT32* pCycle);
DllExport int DB_GetTagScriptByAddr(const char* connection, const char* addr, DB_TagScript* pScript, DB_UINT32* pCycle);
DllExport int DB_GetTagScriptByID(const char* connection, DB_UINT32 tagId, DB_TagScript* pScript, DB_UINT32* pCycle);

DllExport int DBC_GetTagScriptByName(int connId, const char* name, DB_TagScript* pScript, DB_UINT32* pCycle);
DllExport int DBC_GetTagScriptByAddr(int connId, const char* addr, DB_TagScript* pScript, DB_UINT32* pCycle);
DllExport int DBC_GetTagScriptByID(int connId, DB_UINT32 tagId, DB_TagScript* pScript, DB_UINT32* pCycle);

DllExport int DB_SetTagAttrByName(const char* connection, const char* name, void* handle);
DllExport int DB_SetTagAttrByAddr(const char* connection, const char* addr, void* handle);
DllExport int DB_SetTagAttrByID(const char* connection, DB_UINT32 tagId, void* handle);

DllExport int DBC_SetTagAttrByName(int connId, const char* name, void* handle);
DllExport int DBC_SetTagAttrByAddr(int connId, const char* addr, void* handle);
DllExport int DBC_SetTagAttrByID(int connId, DB_UINT32 tagId, void* handle);

DllExport int DB_SetTagScriptByName(const char* connection, const char* name, const DB_TagScript* pScript, DB_UINT32 cycle);
DllExport int DB_SetTagScriptByAddr(const char* connection, const char* addr, const DB_TagScript* pScript, DB_UINT32 cycle);
DllExport int DB_SetTagScriptByID(const char* connection, DB_UINT32 tagId, const DB_TagScript* pScript, DB_UINT32 cycle);

DllExport int DBC_SetTagScriptByName(int connId, const char* name, const DB_TagScript* pScript, DB_UINT32 cycle);
DllExport int DBC_SetTagScriptByAddr(int connId, const char* addr, const DB_TagScript* pScript, DB_UINT32 cycle);
DllExport int DBC_SetTagScriptByID(int connId, DB_UINT32 tagId, const DB_TagScript* pScript, DB_UINT32 cycle);

DllExport int DB_GetLibraryScript(const char* connection, DB_TagScript* pScript);
DllExport int DB_SetLibraryScript(const char* connection, const DB_TagScript* pScript);

DllExport int DBC_GetLibraryScript(int connId, DB_TagScript* pScript);
DllExport int DBC_SetLibraryScript(int connId, const DB_TagScript* pScript);

DllExport int DB_CloseTagAttrHandle(void* handle);
DllExport int DB_SetTagAttr(void* handle, const char* attrName, const char* attrVal);
DllExport int DB_DelTagAttr(void* handle, const char* attrName);
DllExport int DB_ClearTagAttr(void* handle);
DllExport int DB_GetTagAttr(void* handle, const char* attrName, const char** attrVal);
DllExport int DB_GetTagAttrCount(void* handle, int* pCount);
DllExport int DB_GetTagAttrAt(void* handle, int index, const char** attrName, const char** attrVal);

DllExport int DB_AddTag(const char* connection, const DB_TagConfig* pConfig);
DllExport int DBC_AddTag(int connId, const DB_TagConfig* pConfig);

DllExport int DB_UpdateTagByID(const char* connection, const DB_TagConfig* pConfig);
DllExport int DB_UpdateTagByName(const char* connection, const DB_TagConfig* pConfig);

DllExport int DBC_UpdateTagByID(int connId, const DB_TagConfig* pConfig);
DllExport int DBC_UpdateTagByName(int connId, const DB_TagConfig* pConfig);

DllExport int DB_RenameTag(const char* connection, const char* oldName, const char* newName);
DllExport int DBC_RenameTag(int connId, const char* oldName, const char* newName);

DllExport int DB_DeleteTagByName(const char* connection, const char* name);
DllExport int DB_DeleteTagByAddr(const char* connection, const char* addr);
DllExport int DB_DeleteTagByID(const char* connection, DB_UINT32 tagId);

DllExport int DBC_DeleteTagByName(int connId, const char* name);
DllExport int DBC_DeleteTagByAddr(int connId, const char* addr);
DllExport int DBC_DeleteTagByID(int connId, DB_UINT32 tagId);

DllExport int DB_AddTagWithID(const char* connection, const DB_TagConfig* pConfig, DB_UINT32* pTagID);
DllExport int DBC_AddTagWithID(int connId, const DB_TagConfig* pConfig, DB_UINT32* pTagID);

DllExport int DB_AddTagList(const char* connection, const DB_ConfigBlock* block, DB_ConfigResults* results); 
DllExport int DBC_AddTagList(int connId, const DB_ConfigBlock* block, DB_ConfigResults* results); 

DllExport int DB_UpdateTagListByName(const char* connection, const DB_ConfigBlock* block, DB_ConfigResults* results); 
DllExport int DBC_UpdateTagListByName(int connId, const DB_ConfigBlock* block, DB_ConfigResults* results); 

DllExport int DB_TagCount(const char* connection, DB_ConfigPosition* pos);
DllExport int DBC_TagCount(int connId, DB_ConfigPosition* pos);

DllExport int DB_SetTagFilter(const char* connection, const char* type, const char* value);
DllExport int DBC_SetTagFilter(int connId, const char* type, const char* value);

DllExport int DB_ListTags(const char* connection, DB_ConfigPosition* pos, DB_ConfigBlock* block);
DllExport int DBC_ListTags(int connId, DB_ConfigPosition* pos, DB_ConfigBlock* block);

DllExport int DB_GetLatestValueByName(const char* connection, const DB_NAME* names, DB_Value* values, size_t count);
DllExport int DB_GetLatestValueByAddr(const char* connection, const DB_ADDR* addrs, DB_Value* values, size_t count);
DllExport int DB_GetLatestValueByID(const char* connection, const DB_UINT32* ids, DB_Value* values, size_t count);

DllExport int DBC_GetLatestValueByName(int connId, const DB_NAME* names, DB_Value* values, size_t count);
DllExport int DBC_GetLatestValueByAddr(int connId, const DB_ADDR* addrs, DB_Value* values, size_t count);
DllExport int DBC_GetLatestValueByID(int connId, const DB_UINT32* ids, DB_Value* values, size_t count);

DllExport int DB_InsertBlockValueByName(const char* connection, const char* name, const DB_Value* values, size_t count);
DllExport int DB_InsertBlockValueByAddr(const char* connection, const char* addr, const DB_Value* values, size_t count);
DllExport int DB_InsertBlockValueByID(const char* connection, DB_UINT32 tagId, const DB_Value* values, size_t count);

DllExport int DBC_InsertBlockValueByName(int connId, const char* name, const DB_Value* values, size_t count);
DllExport int DBC_InsertBlockValueByAddr(int connId, const char* addr, const DB_Value* values, size_t count);
DllExport int DBC_InsertBlockValueByID(int connId, DB_UINT32 tagId, const DB_Value* values, size_t count);

DllExport int DB_DeleteHistoryByName(const char* connection, const char* name, const DB_Time* before);
DllExport int DB_DeleteHistoryByAddr(const char* connection, const char* addr, const DB_Time* before);
DllExport int DB_DeleteHistoryByID(const char* connection, DB_UINT32 tagId, const DB_Time* before);

DllExport int DBC_DeleteHistoryByName(int connId, const char* name, const DB_Time* before);
DllExport int DBC_DeleteHistoryByAddr(int connId, const char* addr, const DB_Time* before);
DllExport int DBC_DeleteHistoryByID(int connId, DB_UINT32 tagId, const DB_Time* before);

DllExport int DB_UpdateValueByName(const char* connection, const DB_NAME* names, const DB_Value* values, size_t count);
DllExport int DB_UpdateValueByAddr(const char* connection, const DB_ADDR* addrs, const DB_Value* values, size_t count);
DllExport int DB_UpdateValueByID(const char* connection, const DB_UINT32* ids, const DB_Value* values, size_t count);

DllExport int DBC_UpdateValueByName(int connId, const DB_NAME* names, const DB_Value* values, size_t count);
DllExport int DBC_UpdateValueByAddr(int connId, const DB_ADDR* addrs, const DB_Value* values, size_t count);
DllExport int DBC_UpdateValueByID(int connId, const DB_UINT32* ids, const DB_Value* values, size_t count);

//blob point type
DllExport int DB_SetBlobValueByName(const char* connection, const char* name, const DB_BlobValue* blob);
DllExport int DB_SetBlobValueByAddr(const char* connection, const char* addr, const DB_BlobValue* blob);
DllExport int DB_SetBlobValueByID(const char* connection, DB_UINT32 tagId, const DB_BlobValue* blob);

DllExport int DBC_SetBlobValueByName(int connId, const char* name, const DB_BlobValue* blob);
DllExport int DBC_SetBlobValueByAddr(int connId, const char* addr, const DB_BlobValue* blob);
DllExport int DBC_SetBlobValueByID(int connId, DB_UINT32 tagId, const DB_BlobValue* blob);

DllExport int DB_QueryHistStatusByName(const char* connection, const char* name, DB_HistoryStatus* status);
DllExport int DB_QueryHistStatusByAddr(const char* connection, const char* addr, DB_HistoryStatus* status);
DllExport int DB_QueryHistStatusByID(const char* connection, DB_UINT32 tagId, DB_HistoryStatus* status);

DllExport int DBC_QueryHistStatusByName(int connId, const char* name, DB_HistoryStatus* status);
DllExport int DBC_QueryHistStatusByAddr(int connId, const char* addr, DB_HistoryStatus* status);
DllExport int DBC_QueryHistStatusByID(int connId, DB_UINT32 tagId, DB_HistoryStatus* status);

DllExport int DB_QueryHistPositionByName(const char* connection, const char* name, DB_HistoryPosition* position);
DllExport int DB_QueryHistPositionByAddr(const char* connection, const char* addr, DB_HistoryPosition* position);
DllExport int DB_QueryHistPositionByID(const char* connection, DB_UINT32 tagId, DB_HistoryPosition* position);

DllExport int DBC_QueryHistPositionByName(int connId, const char* name, DB_HistoryPosition* position);
DllExport int DBC_QueryHistPositionByAddr(int connId, const char* addr, DB_HistoryPosition* position);
DllExport int DBC_QueryHistPositionByID(int connId, DB_UINT32 tagId, DB_HistoryPosition* position);

DllExport int DB_RetrieveBlobByName(const char* connection, const char* name, DB_HistoryPosition* position, DB_BlobValue* blob);
DllExport int DB_RetrieveBlobByAddr(const char* connection, const char* addr, DB_HistoryPosition* position, DB_BlobValue* blob);
DllExport int DB_RetrieveBlobByID(const char* connection, DB_UINT32 tagId, DB_HistoryPosition* position, DB_BlobValue* blob);

DllExport int DBC_RetrieveBlobByName(int connId, const char* name, DB_HistoryPosition* position, DB_BlobValue* blob);
DllExport int DBC_RetrieveBlobByAddr(int connId, const char* addr, DB_HistoryPosition* position, DB_BlobValue* blob);
DllExport int DBC_RetrieveBlobByID(int connId, DB_UINT32 tagId, DB_HistoryPosition* position, DB_BlobValue* blob);

DllExport int DB_RetrieveTrendByName(const char* connection, const char* name, DB_HistoryPosition* position, DB_HistValueBlock* block);
DllExport int DB_RetrieveTrendByAddr(const char* connection, const char* addr, DB_HistoryPosition* position, DB_HistValueBlock* block);
DllExport int DB_RetrieveTrendByID(const char* connection, DB_UINT32 tagId, DB_HistoryPosition* position, DB_HistValueBlock* block);

DllExport int DBC_RetrieveTrendByName(int connId, const char* name, DB_HistoryPosition* position, DB_HistValueBlock* block);
DllExport int DBC_RetrieveTrendByAddr(int connId, const char* addr, DB_HistoryPosition* position, DB_HistValueBlock* block);
DllExport int DBC_RetrieveTrendByID(int connId, DB_UINT32 tagId, DB_HistoryPosition* position, DB_HistValueBlock* block);

//trend max and min profile
DllExport int DB_GetProfileByName(const char* connection, const char* name, const DB_Time* startTime, const DB_Time* endTime, DB_Profile* profile);
DllExport int DB_GetProfileByAddr(const char* connection, const char* addr, const DB_Time* startTime, const DB_Time* endTime, DB_Profile* profile);
DllExport int DB_GetProfileByID(const char* connection, DB_UINT32 tagId, const DB_Time* startTime, const DB_Time* endTime, DB_Profile* profile);

DllExport int DBC_GetProfileByName(int connId, const char* name, const DB_Time* startTime, const DB_Time* endTime, DB_Profile* profile);
DllExport int DBC_GetProfileByAddr(int connId, const char* addr, const DB_Time* startTime, const DB_Time* endTime, DB_Profile* profile);
DllExport int DBC_GetProfileByID(int connId, DB_UINT32 tagId, const DB_Time* startTime, const DB_Time* endTime, DB_Profile* profile);

DllExport int DB_RetrieveProfilesByName(const char* connection, const char* name, DB_HistoryPosition* position, const DB_Time* period, DB_HistProfileBlock* block);
DllExport int DB_RetrieveProfilesByAddr(const char* connection, const char* addr, DB_HistoryPosition* position, const DB_Time* period, DB_HistProfileBlock* block);
DllExport int DB_RetrieveProfilesByID(const char* connection, DB_UINT32 tagId, DB_HistoryPosition* position, const DB_Time* period, DB_HistProfileBlock* block);

DllExport int DBC_RetrieveProfilesByName(int connId, const char* name, DB_HistoryPosition* position, const DB_Time* period, DB_HistProfileBlock* block);
DllExport int DBC_RetrieveProfilesByAddr(int connId, const char* addr, DB_HistoryPosition* position, const DB_Time* period, DB_HistProfileBlock* block);
DllExport int DBC_RetrieveProfilesByID(int connId, DB_UINT32 tagId, DB_HistoryPosition* position, const DB_Time* period, DB_HistProfileBlock* block);

//retrieve historical snapshot in batch mode 
DllExport int DB_RetrieveSnapshotsByName(const char* connection, const DB_Time* start, const DB_NAME* names, DB_Value* values, size_t count);
DllExport int DB_RetrieveSnapshotsByAddr(const char* connection, const DB_Time* start, const DB_ADDR* addrs, DB_Value* values, size_t count);
DllExport int DB_RetrieveSnapshotsByID(const char* connection, const DB_Time* start, const DB_UINT32* ids, DB_Value* values, size_t count);

DllExport int DBC_RetrieveSnapshotsByName(int connId, const DB_Time* start, const DB_NAME* names, DB_Value* values, size_t count);
DllExport int DBC_RetrieveSnapshotsByAddr(int connId, const DB_Time* start, const DB_ADDR* addrs, DB_Value* values, size_t count);
DllExport int DBC_RetrieveSnapshotsByID(int connId, const DB_Time* start, const DB_UINT32* ids, DB_Value* values, size_t count);

//sampled values
DllExport int DB_RetrieveSamplesByName(const char* connection, const char* name, DB_HistoryPosition* position, const DB_Time* period, DB_HistValueBlock* block);
DllExport int DB_RetrieveSamplesByAddr(const char* connection, const char* addr, DB_HistoryPosition* position, const DB_Time* period, DB_HistValueBlock* block);
DllExport int DB_RetrieveSamplesByID(const char* connection, DB_UINT32 tagId, DB_HistoryPosition* position, const DB_Time* period, DB_HistValueBlock* block);

DllExport int DBC_RetrieveSamplesByName(int connId, const char* name, DB_HistoryPosition* position, const DB_Time* period, DB_HistValueBlock* block);
DllExport int DBC_RetrieveSamplesByAddr(int connId, const char* addr, DB_HistoryPosition* position, const DB_Time* period, DB_HistValueBlock* block);
DllExport int DBC_RetrieveSamplesByID(int connId, DB_UINT32 tagId, DB_HistoryPosition* position, const DB_Time* period, DB_HistValueBlock* block);

DllExport int DB_QueryServerRange(const char* connection, DB_HistoryStatus* status);
DllExport int DB_TestServer(const char* connection);
DllExport int DB_ListClients(const char* connection, DB_ClientStatusBlock clients, DB_UINT32* pCount);
DllExport int DB_QueryServerStatus(const char* connection, DB_ServerStatus* status);

DllExport int DBC_QueryServerRange(int connId, DB_HistoryStatus* status);
DllExport int DBC_TestServer(int connId);
DllExport int DBC_ListClients(int connId, DB_ClientStatusBlock clients, DB_UINT32* pCount);
DllExport int DBC_QueryServerStatus(int connId, DB_ServerStatus* status);

//time utilities
DllExport DB_Time DB_TimeNow();
DllExport DB_Time DB_SetTime(int year, int mon, int day, int hour, int min, int sec, int usec);
DllExport DB_Time DB_SetUTCTime(int year, int mon, int day, int hour, int min, int sec, int usec);
DllExport int DB_MakeTime(const DB_Time* now, int* year, int* mon, int* day, int* hour, int* min, int* sec, int* usec);
DllExport int DB_FormatTime(const DB_Time* now, DB_TIME_STRING strFmt, bool usec);
DllExport void DB_TimeAdd(DB_Time* t, DB_UINT32 secs, DB_UINT32 usecs);
DllExport void DB_TimeSub(DB_Time* t, DB_UINT32 secs, DB_UINT32 usecs);
DllExport void DB_TimeAddDay(DB_Time* t, int days);
DllExport void DB_TimeAddHour(DB_Time* t, int hours);
DllExport void DB_TimeAddMinute(DB_Time* t, int minutes);
DllExport void DB_TimeAddSeconds(DB_Time* t, int seconds);
DllExport const char* DB_GetErrorMsg(int nError);
DllExport void DB_LogMsg(const char* msg);
DllExport void SetAPILogger(void*);

#ifdef __cplusplus
}
#endif

#endif
