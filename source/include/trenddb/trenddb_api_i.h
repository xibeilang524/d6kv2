#ifndef INTERNAL_API_H
#define INTERNAL_API_H

#include "trenddb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DB_BufferStatus
{
	char name[128];
	DB_UINT64 size, limit;
};

struct DB_BufferStates
{
	DB_BufferStatus states[32];
	DB_UINT32 count;
	DB_UINT64 total_size, total_limit, total_capacity;
};

DllExport int DB_CreateConnectionTimeout(const char* connection, int bFireWall, int bCacheEnabled, const char* user, const char* passwd, int timeout);
DllExport int DB_ConnectServer();
DllExport int DB_DisconnectServer();
DllExport int DB_LogonServer(const char* name, const char* passwd);
DllExport int DB_CreateGroup(const char* name);
DllExport int DB_CreateUser(const char* name, const char* grp, const char* passwd, bool pntWR);
DllExport int DB_SetUserPasswd(const char* name, const char* oldPass, const char* passwd);
DllExport int DB_DeleteGroup(const char* name);
DllExport int DB_DeleteUser(const char* name);
DllExport int DB_GetUserByID(int userId, DB_NAME userName, DB_NAME grpName);
DllExport int DB_GetGroupByID(int grpId, DB_NAME grpName);
DllExport int DB_GetUserGroupCount(int* pGrpCnt, int* pUserCnt);
DllExport int DB_CheckPassword(const char* passwd, const char** message);

DllExport int DB_ListRDFServers(SERVER_NAME* servers, size_t count);
DllExport int DB_ListBlockServers(SERVER_NAME* servers, size_t count);

typedef int (*DB_FileInfoCallback)(DB_UINT32 fileno, DB_UINT32 filelen, const char* filename, const char* filedir, void* arg);
typedef int (*DB_FileProgCallback)(DB_UINT32 fileno, DB_UINT32 offset, DB_UINT32 length, void* arg);

DllExport int DB_CreateBackupConn(const char* connection, const char* user, const char* passwd);
DllExport int DB_GetBackupList(const char* connection, DB_FileInfoCallback callback, void* arg);
DllExport int DB_GetBackupFile(const char* connection, DB_UINT32 fileno, const char* localFilePath, DB_FileProgCallback callback, void* arg);
DllExport int DB_SyncBackupFile(const char* connection, DB_UINT32 fileno, const char* localFilePath, DB_FileProgCallback callback, void* arg);

DllExport int DB_GetBufferStates(const char* connection, DB_BufferStates* pStates);
DllExport int DBC_GetBufferStates(int connId, DB_BufferStates* pStates);

struct DB_ServerLog
{
	char data[96*1024];
	size_t size;
};

DllExport int DB_GetServerLog(const char* connection, DB_ServerLog* pLog);
DllExport int DBC_GetServerLog(int connId, DB_ServerLog* pLog);

struct DB_AttrData
{
	char data[96*1024];
	size_t size;
};

struct DataHead
{
	enum Type
	{
		TAG_NAME_ITEM = 0xE5E6E7E8,
		TAG_DATA_ITEM = 0x8E7E6E5E
	};
	DB_UINT32 type;
	DB_UINT32 length;
};

DllExport int DB_LocateDataByName(const char* connection, const char* name, DB_Time* start);
DllExport int DB_GetNextDataByName(const char* connection, const char* name, DB_Time* start, DB_HistValueBlock* block);
DllExport int DB_GetPrevDataByName(const char* connection, const char* name, DB_Time* start, DB_HistValueBlock* block);
DllExport int DB_LocateDataByID(const char* connection, DB_UINT32 tagId, DB_Time* start);
DllExport int DB_GetNextDataByID(const char* connection, DB_UINT32 tagId, DB_Time* start, DB_HistValueBlock* block);
DllExport int DB_GetPrevDataByID(const char* connection, DB_UINT32 tagId, DB_Time* start, DB_HistValueBlock* block);

DllExport int DBC_LocateDataByName(int connId, const char* name, DB_Time* start);
DllExport int DBC_GetNextDataByName(int connId, const char* name, DB_Time* start, DB_HistValueBlock* block);
DllExport int DBC_GetPrevDataByName(int connId, const char* name, DB_Time* start, DB_HistValueBlock* block);
DllExport int DBC_LocateDataByID(int connId, DB_UINT32 tagId, DB_Time* start);
DllExport int DBC_GetNextDataByID(int connId, DB_UINT32 tagId, DB_Time* start, DB_HistValueBlock* block);
DllExport int DBC_GetPrevDataByID(int connId, DB_UINT32 tagId, DB_Time* start, DB_HistValueBlock* block);

DllExport int DB_GetAttrDataByName(const char* connection, const char* name, DB_AttrData* pData);
DllExport int DB_GetAttrDataByAddr(const char* connection, const char* addr, DB_AttrData* pData);
DllExport int DB_GetAttrDataByID(const char* connection, DB_UINT32 tagId, DB_AttrData* pData);

DllExport int DB_SetAttrDataByName(const char* connection, const char* name, const DB_AttrData* pData);
DllExport int DB_SetAttrDataByAddr(const char* connection, const char* addr, const DB_AttrData* pData);
DllExport int DB_SetAttrDataByID(const char* connection, DB_UINT32 tagId, const DB_AttrData* pData);

DllExport int DB_AsyncRequestTagInfoByID(int connId, DB_UINT32 tagId, DB_UINT8 bComplete, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveTagInfoByID(int connId, DB_UINT32 requestId, DB_TagInfo* pInfo);

DllExport int DB_AsyncRequestLatestValueByName(int connId, const DB_NAME* names, size_t count, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveLatestValueByName(int connId, DB_UINT32 requestId, DB_Value* values, size_t count);

DllExport int DB_AsyncRequestLatestValueByAddr(int connId, const DB_NAME* addrs, size_t count, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveLatestValueByAddr(int connId, DB_UINT32 requestId, DB_Value* values, size_t count);

DllExport int DB_AsyncRequestLatestValueByID(int connId, const DB_UINT32* ids, size_t count, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveLatestValueByID(int connId, DB_UINT32 requestId, const DB_UINT32* ids, DB_Value* values, size_t count);

DllExport int DB_AsyncRequestSnapshotsByName(int connId, const DB_Time* start, const DB_ADDR* names, size_t count, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveSnapshotsByName(int connId, DB_UINT32 requestId, DB_Value* values, size_t count);

DllExport int DB_AsyncRequestSnapshotsByAddr(int connId, const DB_Time* start, const DB_ADDR* addrs, size_t count, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveSnapshotsByAddr(int connId, DB_UINT32 requestId, DB_Value* values, size_t count);

DllExport int DB_AsyncRequestSnapshotsByID(int connId, const DB_Time* start, const DB_UINT32* ids, size_t count, DB_UINT32* pRequestId);
DllExport int DB_AsyncRetrieveSnapshotsByID(int connId, DB_UINT32 requestId, DB_Value* values, size_t count);


#ifdef __cplusplus
}
#endif

#endif
