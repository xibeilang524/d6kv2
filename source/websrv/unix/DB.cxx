// DB.cpp: implementation of the CDB class.
//
//////////////////////////////////////////////////////////////////////


#ifdef WIN32
#include "stdafx.h"
#endif


#include "db.h"

#ifdef WIN32
	#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
	#endif
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CRITICAL_SECTION CDB::m_DbapiLock;
extern char DBNAME[64];
extern char HISDBNAME[64];
extern char DBO[64];

CDB::CDB()
{
	#ifdef WIN32
		InitializeCriticalSection(&m_DbapiLock);
	#else
		pthread_mutex_init(&m_DbapiLock,NULL);
	#endif
		if(DBTYPE_CURRENT == DBTYPE_SYBASE)
		{
			strcpy(DBNAME,"modeldb");
			strcpy(HISDBNAME,"modeldb");
			strcpy(DBO,"dbo.");
		}
		else if(DBTYPE_CURRENT == DBTYPE_ORACLE)
		{
			strcpy(DBNAME,"modeldb");
			strcpy(HISDBNAME,"modeldb");
			strcpy(DBO,"");
		}
		else if(DBTYPE_CURRENT == DBTYPE_DMSQL)
		{
			strcpy(DBNAME,"modeldb");
			strcpy(HISDBNAME,"modeldb");
			strcpy(DBO,"");
		}

// 		else if(DBTYPE_CURRENT == DBTYPE_MYSQL)
// 		{
// 			strcpy(DBNAME,"paradb");
// 			strcpy(HISDBNAME,"paradb");
// 			strcpy(DBO,"");
// 		}
}

CDB::~CDB()
{
	#ifdef WIN32
		DeleteCriticalSection(&m_DbapiLock);
	#else
		pthread_mutex_destroy(&m_DbapiLock);
	#endif
}

void CDB::requestDBApi()
{
	#ifdef WIN32
		EnterCriticalSection(&m_DbapiLock);
	#else
		pthread_mutex_lock(&m_DbapiLock);
	#endif

	return;
}

void CDB::releaseDBApi()
{
	#ifdef WIN32
		LeaveCriticalSection(&m_DbapiLock);
	#else
		pthread_mutex_unlock(&m_DbapiLock);
	#endif
	return;
}
