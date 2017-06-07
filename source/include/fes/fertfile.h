#ifndef _FERTFILE_
#define _FERTFILE_

#include "stdio.h"
#include <string.h>

#define MYCHAR_BIT        0 //二进制
#define MYCHAR_ASC        1 //ASCII

#define FILE_READ		0 //只读
#define FILE_WRITE_NEW	1 //新建
#define FILE_WRITE_CON	2 //追加

#define FILE_NAME_LEN   512		//文件名长度

#ifdef WIN32
#ifdef PLATFORM_E_LANG_FILE_DLL
#define	PLATFORM_FERT_FILE_EXPORT __declspec(dllexport) 
#else
#define	PLATFORM_FERT_FILE_EXPORT __declspec(dllimport) 
#endif
#else
#define	PLATFORM_FERT_FILE_EXPORT
#endif

class PLATFORM_FERT_FILE_EXPORT FertFile  
{
public:
	FertFile(char* fileName);
	virtual ~FertFile();
	bool open();
	bool close();

	int Save(const char *pBuf,unsigned int size, unsigned int offset,char type, int cType=0);
	int SaveToSys();//全网存盘
	int GetLenth();
	bool GetFileName(char *pName);
	unsigned long GetCurSec();//获取当前秒
	int Read_file(char* pReadBuf);
private:
	FILE * m_pFile;
	char * m_pCon; 
	char m_fileName[FILE_NAME_LEN];
};

#endif 
