/*
 * --------------------------------------------------
 *   文件名：wfa.h
 */

#if !defined(__WFA)
#define __WFA
#include "webconstant.h"

#ifndef WIN32
	#include <string.h>
#endif

typedef struct _rtuname{
	char name[CODESIZE];
}NAME;

struct FILELIST{
	struct FILELIST *pfilelist;
	char path[128];//相对环境变量的路径
	char filename[64];//文件名
	int yearmonday;//年月日,最近修改时间
	int hourminsec;//时分秒
	FILELIST()
	{
		pfilelist = NULL;
		memset(path,0,128);
		memset(filename,0,64);
		yearmonday = 0;
		hourminsec = 0;
	}
};

struct  NODEFILE{
	struct NODEFILE *pnodefile;
	char path[128];//相对环境变量的路径
	char filename[64];//文件名
	int yearmonday;//最近修改时间的年月日
	int hourminsec;//时分秒
	NODEFILE()
	{
		pnodefile = NULL;
		memset(path,0,128);
		memset(filename,0,64);
		yearmonday = 0;
		hourminsec = 0;
	}
};

struct WEBNODE{
	struct WEBNODE *pwebnode;
	char ipaddr[128];
	NODEFILE *pnodefile;
	WEBNODE()
	{
		pwebnode = NULL;
		pnodefile = NULL;
		memset(ipaddr,0,128);
	}
};

#ifndef WIN32
	struct DIRLIST{
		struct DIRLIST *dirlist;
		char path[256];	
		int bsendf ;
	};
#endif
#endif // !defined(WEBSTRUCT)
