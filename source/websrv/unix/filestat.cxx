//fstat.cxx

#ifdef WIN32
#include "stdafx.h"
#endif


#include "filestat.h"
#include <memory.h> 
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>




#ifdef WIN32
#include <afx.h>
#endif

FStat :: FStat()
{
	filestream = NULL;
	memset(fname,0,256);
}

FStat :: ~FStat()
{
}

int FStat :: isExist(char * file_name)//TRUE存在，FALSE不存在
{
	if(file_name == 0 || file_name[0] == '\0') return -1;
	strcpy(fname,file_name);
	#ifdef WIN32
		int fopenresult = _stat(file_name,&filestat);//-1失败，0成功
	#else
		int fopenresult = stat(file_name,&filestat);
	#endif
	return (fopenresult == 0);
}

long FStat :: getFileLen()
{
	return (long)filestat.st_size;
}

int FStat :: isDir()//TRUE是目录,否则不是
{
	//if( (filestat.st_mode & S_IFMT) == S_IFDIR)
	if( (filestat.st_mode & S_IFDIR) == 0)
		return 0;
	else 
		return 1;

}

int FStat :: openfile()
{
	#ifdef WIN32
		return cfile.Open(fname,CFile::modeRead|CFile::shareDenyNone);
	#else
		filestream = fopen(fname,"r");
		if(filestream == NULL)
			return 0;
		else
			return 1;
	#endif
}

int FStat::close()
{
	#ifdef WIN32
		 cfile.Close();
		 return 1;
	#else
		return fclose(filestream);
	#endif
}

int FStat::read(char *buffer, unsigned int count)
{
#ifdef WIN32
	return cfile.Read((void*)buffer,count);
#else
	int total = 0;
	FILE * stream = filestream;
	fseek( stream, 0L, SEEK_SET);
	while( !feof( stream ) )
	{
		int readcount = fread( buffer, sizeof( char ), count, stream );
		if( ferror( stream ) )      
		{
			perror( "Read error" );
			break;
		}
		int eof = feof( stream );
		/* Total up actual bytes read */
		buffer += readcount;
		total += readcount;
	}
	return total;
#endif
}

//得到文件最近修改的年月日，时分秒
int FStat::getmtime(int &ymd, int &hms)
{
	
	//struct tm *ptm = gmtime( &filestat.st_mtime );
	struct tm *ptm = localtime( &filestat.st_mtime );
	int year = ptm->tm_year + 1900;
	int month = ptm->tm_mon + 1 ;	//Month (0 – 11; January = 0)
	int day = ptm->tm_mday ;
	int hour = ptm->tm_hour ;
	int min = ptm->tm_min ;
	int sec = ptm->tm_sec ;
	ymd = year*10000 + month*100 + day;
	hms = hour*10000 + min*100 + sec;
	return 1;
}
