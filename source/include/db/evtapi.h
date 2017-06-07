#ifndef	EVT_API_H
#define	EVT_API_H

#include "sysstr.h"
#include "dbapi.h"

#if defined(WIN32)
#if defined(EVTPORTDLL)
#define	EVTEXPORT				__declspec(dllexport) 
#else
#define	EVTEXPORT				__declspec(dllimport) 
#endif
#else
#define	EVTEXPORT
#endif

class EVTEXPORT CEvtProc : public DBApi
{
	virtual int GetDataFmt(DBApi& db,char *libname,char *tblname,FUNC_STR *func,CS_DATAFMT **data_fmt);
	virtual int BulkCopyInEvent(DBApi& db,HIS_EVENT *buf,int sum);
	virtual int ExeCmd(DBApi& db,int del_flag,FUNC_STR *func,CS_DATAFMT* datafmt);

public:
	CEvtProc();
	virtual ~CEvtProc();

	virtual void FromDataToStr (CS_DATAFMT *datafmt, char *cmdbuf1, void *datapt);
	virtual int IsOptEvt(HIS_EVENT *buf);
	virtual int InitDb(DBApi& db,int year,int month,int OEventf,FUNC_STR *func,CS_DATAFMT **data_fmt);

	virtual int InsertEvent(DBApi& db,int nEvent,HIS_EVENT *pEvent);
	virtual int DeleteEvent(DBApi& db,FUNC_STR *func,CS_DATAFMT* data_fmt,int OEventf);
	virtual int UpdateEvent(DBApi& db,FUNC_STR *func,CS_DATAFMT* data_fmt,int OEventf);
};
#endif