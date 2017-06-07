// DB.h: interface for the CDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_H__D02BF2A3_4BDE_4987_B06A_9A3A55F6333A__INCLUDED_)
#define AFX_DB_H__D02BF2A3_4BDE_4987_B06A_9A3A55F6333A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "rdb/rdbapi.h"
#include "webconstant.h"

//Êý¾Ý¿â·ÃÎÊ
class CDB : public DBApi
{
private:
	#ifdef WIN32
		CRITICAL_SECTION m_DbapiLock; 
	#else
		pthread_mutex_t m_DbapiLock ;
	#endif
public:
	void releaseDBApi();
	void requestDBApi();
	CDB();
	virtual ~CDB();
};

#endif // !defined(AFX_DB_H__D02BF2A3_4BDE_4987_B06A_9A3A55F6333A__INCLUDED_)
