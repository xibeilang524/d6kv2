#ifndef _MFC_DBO_H
#define _MFC_DBO_H

#include "ddbo/ddbo.h"
#include <QtCore/QObject>

class Ddbi;

#define FMS_UNLOCKED	0x00
#define FMS_LOCKED		0x01
#define FMS_ISLOCKED	0x02

#define FMS_LOCAL		0x00
#define FMS_NETWORK		0x10
#define FMS_ISNETWORK	0x20

#define FIS_LOCK(s)	 ((s)&FMS_LOCKED )
#define FSS_LOCK(s)   s = s|FMS_LOCKED
#define FCS_LOCK(s)  s = (s&(~FMS_LOCKED))

#define FIS_NETWORK(s)	((s)&FMS_NETWORK )
#define FSS_NETWORK(s)	s = s|FMS_NETWORK
#define FCS_NETWORK(s)	s = (s&(~FMS_NETWORK))

typedef struct __tagFileMutex
{
	char	owner[24];
	char	type[64];
	char	path[255];
	int32	status;
	char	editer[64];
	char 	editernode[64];
	int32	edittime;
	char	network[64];
	char	networknode[64];
	int32	networktime;
	char	creater[64];
	char 	creaternode[64];
	int32	createtime;
}FILEMUTEX,*PFILEMUTEX;

class GDE_EXPORT_MFCDBO GfileMutex:public Ddbo
{
public:
	GfileMutex();
	~GfileMutex();

	uint8	networkFile(\
			const char* path);

	uint8	networkFile(\
			const char* path,\
			const char* network,\
			const char* networknode);

	uint8	localFile(\
			const char* path);

	uint8	localFile(\
			const char* path,\
			const char* network,\
			const char* networknode);

	uint8	isNetworkFile(\
			const char* path);

	uint8	isNetworkFile(\
			const char* path,\
			const char* network,\
			const char* networknode);
	uint8	lockFile(\
			const char* path);

	uint8	lockFile(\
			const char* path,\
			const char* editer,\
			const char* editernode);
	uint8	unlockFile(\
			const char* path);

	uint8	unlockFile(\
			const char* path,\
			const char* editer,\
			const char* editernode);
	uint8	isLockFile(\
			const char* path);

	uint8	isLockFile(\
			const char* path,\
			const char* editer,\
			const char* editernode);

	uint8	createFile(\
			const char* owner,\
			const char* type,\
			const char* path,\
			const char* creater,\
			const char* creaternode,\
			int32	createtime);
	uint8	updateFile(\
			const char* owner,\
			const char* type,\
			const char* path,\
			const char* editer,\
			const char* editernode,\
			int32	edittime);
	uint8	existFile(\
		const char* path);

	uint8	deleteFile(\
		const char* path);
	PFILEMUTEX getFileMutex() const ;
	void	   setEnv(const char* env) ;
protected:
	void	clear();

	uint8	setLockStatus(\
			const char* path,\
			uint8 status=FMS_ISLOCKED);

	uint8	setLockStatus(\
			const char* path,\
			const char* editer,\
			const char* editernode,\
			uint8	status=FMS_ISLOCKED);

	uint8	setNetworkStatus(\
			const char* path,\
			uint8 status=FMS_ISNETWORK);

	uint8	setNetworkStatus(\
			const char* path,\
			const char* network,\
			const char* networknode,\
			uint8	status=FMS_ISNETWORK);

	uint8	openFile(\
			const char* path);
	uint8	closeFile(\
			const char* path);
private:
	uint8		m_canUse   ;
	FILEMUTEX	m_fileMutex;
	QString		m_env;
};

class GDE_EXPORT_MFCDBO GfileMutexPara:public Ddbo
{
public:
	GfileMutexPara();
	~GfileMutexPara();

	uint8 readData();

	uint32 count() const ;
	PFILEMUTEX operator[](uint32 index) const;
	PFILEMUTEX at(uint32 index) const;
	PFILEMUTEX data() const;

	uint32		filterOwner(PFILEMUTEX* fm, const char* owner) const;
	uint32		filterType (PFILEMUTEX* fm , const char* type) const;
	uint32		types ( char*** type ) const;
	uint32		owners ( char*** owner ) const;
	PFILEMUTEX	find( const char* path ) const;

	static  uint32 sameOwnerType(PFILEMUTEX* fm,\
				const char* type,\
				const PFILEMUTEX sfm,\
				uint32 ncount);
	static uint32 sameTypeOwner(PFILEMUTEX* fm,\
				const char* owner,\
				const PFILEMUTEX sfm,\
				uint32 ncount);

	static uint32 types( char*** type,const PFILEMUTEX sfm, uint32 ncount );
	static uint32 owners( char*** owner,const PFILEMUTEX sfm, uint32 ncount );
protected:
	void    clear();
protected:
	PFILEMUTEX    m_fmPara ;
	uint32		  m_count  ;
};

#endif
