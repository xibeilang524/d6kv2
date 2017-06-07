//unix ÎÄ¼þ²Ù×÷
//filestat.h

#ifndef	____FSTAT
#define	____FSTAT

#ifdef WIN32
	#include "stdafx.h"
	#include <sys/types.h>
	#include <sys/stat.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif
#include "stdio.h"

class FStat  
{
public:
#ifdef WIN32
	CFile cfile;
#endif
	FILE *filestream;
	int getmtime(int &ymd,int &hms);
	int read(char *buffer, unsigned int count);
	int close();
	char fname[256];
	#ifdef WIN32
		struct _stat filestat;
	#else
		struct stat filestat;
	#endif
	FStat();
	~FStat();
	long getFileLen();
	int isExist(char *file_name);
	int isDir();
	int openfile();
	
};
#endif// ____FSTAT