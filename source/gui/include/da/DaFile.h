//*******************************
//zl200610 增加DA日志功能
//*******************************

#ifndef _DAFILE_H
#define _DAFILE_H

#define LEN_FILE_NAME        96
typedef struct _damode_
{
	short mode;
	short protct;
    unsigned char workstate;
	char machine[40];
    unsigned char performstat;
    unsigned char extendisolate;
    unsigned char removenoload;
    unsigned char getfaultdir;
    unsigned char beforesecs;
	unsigned char aftersecs;
	int extendflag;
}DAMODE;

typedef struct _faultpos_
{
	int no;
	char describe[48];
    unsigned char faulttype;
    unsigned char wholocate;
    unsigned char isolated;
    unsigned char restored;
	int extendflag;
}FAULTPOS;

typedef struct _isolateinfo_
{
	int no;
	int isono;
	char name[24];
	char describe[48];
    unsigned char result;
    unsigned char whoexe;
	int extendflag;
}ISOLATEINFO;

typedef struct {
	int no;
	int prjno;
	int prjsum;
	char name[24];
	char describe[48];
	unsigned char action;
	unsigned char result;
    unsigned char whoexe;
	int extendflag;
}RESTOREINFO;

typedef struct _fileTime
{
        unsigned short		 Ms;
        unsigned short   	 Sec;
        unsigned short 		 Min;
        unsigned short		 Hour;
        unsigned short		 Day;
        unsigned short   	 Mon;
        unsigned short   	 Year;          //start 1900
}FILE_TIME;

class CMyTime
{
public:
	void GetCurTime(FILE_TIME &mytime);
};


class CMyFile  
{
public:
	CMyFile();
	virtual ~CMyFile();
public:
	int  Open();
	int  WriteTabInfo();
	int  WriteStr(char *pStr,int timeFlag=1);
	BOOL ReadFaultpos(int no=-1);
	BOOL ReadIsolateinfo(int no=-1);
	BOOL ReadRestoreinfo(int prjno=-1);
	int  GetFileLen(char *filename);
protected:
    CMyTime m_time;
	FILE *m_pFile;

private:
//	DAMODE *damode;
//	int damodenum;
	FAULTPOS *faultpos;
	int faultposnum;
	ISOLATEINFO *isolateinfo;
	int isolateinfonum;
	RESTOREINFO *restoreinfo;
	int restoreinfonum;

};

#endif 
