// ThreadStreamData.cpp: implementation of the ThreadStreamData class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include "stdafx.h"
#endif


#include "ThreadStreamData.h"
#include "webconstant.h"

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

CThreadStreamData::CThreadStreamData(int datanums)
{
	datacount = datanums;
	bchangef = FALSE;
	pdatatype = (int*)calloc(1,sizeof(int)*datacount);
	pdatachangedf = (int*)calloc(1,sizeof(int)*datacount);
	pvalue = (RDB_VALUETYPE_STRU *)calloc(1,sizeof(RDB_VALUETYPE_STRU)*datacount);
	pold_order = (int*)calloc(1,sizeof(int)*datacount);
}

CThreadStreamData::~CThreadStreamData()
{
	FREE(pdatatype);
	FREE(pdatachangedf);
	FREE(pvalue);
	FREE(pold_order);
}
