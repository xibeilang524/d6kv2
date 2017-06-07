#ifndef GTOOLSIMP_FUNC_H_HEADER_INCLUDED_BAD1AA20
#define GTOOLSIMP_FUNC_H_HEADER_INCLUDED_BAD1AA20

#include <QtCore/QString>

#include "ddef/ddes_comm.h"

typedef struct __tagProtDataIN
{
	void*	dboComm;
	QString dynDataPath;
}PROTDATAIN,*PPROTDATAIN;

#define GTSM_CREATE_PROTDATA 0x01
#define GTSM_XML_BIN		0x01
#define GTSM_BIN_XML		0x02
#define GTSM_VERSION_MNG	0x01

typedef struct __tagFileMNGIN
{
	void*	dboComm;
	void*	uri;
	void*	nti;
	void*	netbus;
}FILEMNGIN,*PFILEMNGIN;
#define GTSM_GRAPHFILE_MNG	0x01
#define GTSM_GRAPHFILE_SEARCH	0x01

typedef struct __tagLoginDataIN
{
	void*	dboComm				   ;
	char	hostname[GDB_CODE_LEN] ;

}LOGINDATAIN,*PLOGINDATAIN;

typedef struct __tagLoginDataOUT
{
	char	username[GDB_CODE_LEN];
	uint32	valtime;

}LOGINDATAOUT,*PLOGINDATAOUT;

#define GTSM_LOGIN_VDIALOG	0x01
#define GTSM_LOGIN_NDIALOG	0x02

typedef struct __tagTableDataIN
{
	void*	dbi;
	void*	dynData;
}TABLEDATAIN,*PTABLEDATAIN;

#define GTSM_TABLE_DIALOG	0x01

typedef struct __tagDevRealParaIN
{
	void* dboComm;
	void* dbi;
	void* dynData;
}DEVREALPARAIN,*PDEVREALPARAIN;

#define GTSM_DEVREAL_DIALOG	0x01

typedef struct __tagRealParaIN
{
	void* dboComm    ;
	void* dboRtdbComm;
	void* dynData    ;	
	uint8 bAdd       ;
	const char* stName;
	int16 devType    ;
}REALPARAIN,*PREALPARAIN;

#define GTSM_REAPARA_CREATE_DIALOG	 0x01
#define GTSM_REALPARA_DYNDATA_DIALOG 0x02

typedef struct __tagActionParaIN
{
	void* dbi;
	void* dboRtdbComm;
	uint8 fromDB;
	uint8 toDB;	
}ACTIONPARAIN,*PACTIONPARAIN;

#define GTSM_ACTIONPARA_DIALOG 0x01

typedef struct __tagSVGParaIN
{
	QString inFile;
	QString outFile;
}SVGPARAIN,*PSVGPARAIN;

#define GTSM_IMPORT_SVG	 0x01
#define GTSM_EXPORT_SVG	 0x02

#endif
