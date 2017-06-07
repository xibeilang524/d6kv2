#ifndef GDISP_COMMON_DEFINE_H
#define GDISP_COMMON_DEFINE_H

#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddbo/ddbo.h"

class GGraphDynData;
class Ddbi;
class Gtools;

enum
{
	MT_YKREQ,
	MT_YKREQRET,
	MT_YTREQ,
	MT_YTREQRET
};

enum
{
	MT_YKAQYS,
	MT_YTAQYS
};

#define NULL_PTR(ptr) ptr=NULL

typedef struct _tagMailStruct
{
	uint16	mailtype;
	char	maildesc[ GDB_DESC_LEN ];
	uint32	maillenth;
}MAILSTRUCT, *PMAILSTRUCT;

typedef struct _tagFuncStruct
{
	GGraphDynData *dyndata;
	Ddbi		*rdbi;
	Ddbi		*hdbi;
	Gtools		*netbustool;
	Gtools		*scadatool;
	USERRIGHT	*curuser;
	DdboComm	*dbocomm;
	QString		curgraph;
	GPoint		pt;
	uint8		mode;

	_tagFuncStruct()
	{
		NULL_PTR(dyndata);
		NULL_PTR(rdbi);
		NULL_PTR(hdbi);
		NULL_PTR(netbustool);
		NULL_PTR(scadatool);
		NULL_PTR(curuser);
		NULL_PTR(dbocomm);
		mode = 0 ;
		curgraph = QString::null;
	}

}FUNCSTRUCT, *PFUNCSTRUCT;

typedef struct _tagReqStruct
{
	QString		trg;//请求源机器名
	int32		pid;//请求源进程ID
	Gtools		*netbustool;
	DdboComm	*dbocomm;
	void		*reqmail;
	PUSERRIGHT	curuser;

	_tagReqStruct()
	{
		trg = QString::null;
		pid = 0;
		NULL_PTR( curuser );
		NULL_PTR( netbustool );
		NULL_PTR( dbocomm );
		NULL_PTR( reqmail );
	}

}REQSTRUCT, *PREQSTRUCT;


#endif