//#include "sybase.h"

#include "db/dbapi.h"
#if defined(__hpux)
#include <sys/termios.h>
#endif

#if defined(WIN32)
#include <io.h>
#elif defined(__unix) || defined(__linux)
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#endif

//#include <sys\stat.h>

#if defined(WIN32)
#include <conio.h>
#include <direct.h>
#elif defined(__unix) || defined(__linux)
#include <ctype.h>
//#include <curses.h>
#include <unistd.h>
long filelength(char *filename )
{
	struct stat rdbfstat;
	int ret=stat(filename,&rdbfstat);
	if(ret!=0)return(-1);
	return(rdbfstat.st_size);
}
#endif

#if defined(__linux)
#include <termios.h>
#endif



#define MAXPARA		10	// 最大参数个数
#define MAXPARALEN		100	// 每个参数的最大长度

#define FLAG_ERROR	-1		// 参数错误
#define FLAG_NO		0		// 无操作
#define FLAG_COPY	1		// 复制
#define FLAG_DEL	2		// 删除
#define FLAG_COMPARE	3	// 比较
#define FLAG_OUTFILE	4	// 表->文件
#define FLAG_INFILE	5		// 文件->表
#define FLAG_CLEAN	6		// 清空表
#define FLAG_DROP	7		// 删除表
#define FLAG_DISP	8		// 显示信息
#define FLAG_ALL	9		// 显示所有帮助
#define FLAG_SCADA	10		//SCADA数据追赶
#define FLAG_DUMP	11		//日志清除
#define FLAG_CHECK	12		//命名规则合理性检查
#define FLAG_DFHIS	13		//东方电子历史数据导入

char strstdin[255];

#define EX_MAXSTRINGLEN		255
#define MAXCPERRTABNUM  100
#define MAXINROWNUM		1000		//每次导入的最大行数
#define MAXOUTROWNUM		5000	//每次导出的最大行数
#define MAXSTATIONNUM		512		//系统允许的最大厂站数
#define DF_HIS				1		//东方电子的历史数据库

DBSingle dbsingle;
typedef  struct {
	char  name[24];				//代码
	char  desc[40];				//描述
	short type;					//类型
	char  devname[24];			//设备代码
	char  logicalflag;			//是否合理
}CHECK_VALUE;
typedef  struct {
	short typevalue;			//类型值
	char typedaima[24];			//类型代码
}SCD_TYPEVALUE;
FUNC_STR tmpfunc;
CS_DATAFMT *tmpdatafmt;
FILE *tmpstream=NULL;
long  recordnum;
int year,month;
int dbtype=DBTYPE_SYBASE;			//
char stationname[MAXSTATIONNUM][24];
int stationnum,scdtypenum;
SCD_TYPEVALUE *scdtypevalue=NULL;

/***************************************************************************/
char	getpass( char* lpszBuffer )
{
#ifndef WIN32
	char *ptr = NULL ;
	sigset_t  sig , sigsave ;
	struct termios term , termsave;
	FILE 	*fp;
	int	c  ;

	fp = fopen( ctermid( NULL ) , "r+" );

	if ( fp == NULL  ) return FALSE;

	int fno = fileno( fp ) ;

	setbuf( fp , NULL );
	sigemptyset(&sig);
	sigaddset(&sig , SIGINT);
	sigaddset(&sig , SIGTSTP);
	sigprocmask(SIG_BLOCK,&sig,&sigsave );

	tcgetattr( fno, &termsave );
	term = termsave ;
	term.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHONL);
	tcsetattr( fno , TCSAFLUSH, &term );

	ptr = lpszBuffer ;

	while ( (c=getc( fp ))!= EOF && c!= '\n' )
	{
		if ( ptr < lpszBuffer+256 ) *ptr++=c;
	}

	*ptr = 0 ;

	putc( '\n' , fp );

	tcsetattr( fno , TCSAFLUSH , &termsave );
	sigprocmask(SIG_SETMASK , &sigsave , NULL );

	fclose(fp);
#else
		int i = 0 ; 
		char letter ;
		do
		{
			letter=getch();
			lpszBuffer[i]=letter;
			i++;
		}
		while (letter!='\r');

		lpszBuffer[i-1]='\0';

#endif

		return 1 ;
}
/***************************************************************************/
/***************************************************************************/
  /* 将字符串后面的空格、回车、换行、TAB等截掉 */
void db_TrimRight(char *p)
{
	char *psz=p;
	char *pszLast=NULL;
	while(*psz != '\0')
	{
		if(isspace((unsigned char)(*psz)))
		{
			if(pszLast==NULL)
				pszLast=psz;
		}
		else pszLast=NULL;
		psz++;
	}
	if(pszLast!=NULL) *pszLast='\0';
}

/***************************************************************************/
  /* 将字符串前面的空格、回车、换行、TAB等截掉 */
void db_TrimLeft(char *p)
{
	int len=strlen(p);
	char *psz=p;
	char *pszLast=NULL;
	while(*psz != '\0')
	{
		if(isspace((unsigned char )(*psz))) pszLast=psz+1;
		else break;
		psz++;
	}
	if(pszLast!=NULL)
		memmove(p,pszLast,len-(pszLast-p)+1);
}



int STDCALL proc(int* row,void* ppt,void* client)//给dbi的回调函数
{
	int nRow = * row;
	char* pt = (char*)ppt;
	char strvalue[255];

	int nCol = tmpfunc.colnum;
	VAL_UNION* pdata = (VAL_UNION*)malloc(sizeof(VAL_UNION)*nRow*nCol);
	fprintf(stderr,"-");
	int ptcnt = 0;
	for(int i=0; i<nRow; i++)
	{
		ptcnt = tmpfunc.recorderlen*i;
		for(int j=0; j<nCol; j++)
		{
			if(tmpfunc.col[j].select!=YES)	continue;

			CS_CHAR* charpt = 0;
			switch (tmpdatafmt[j].datatype) 
			{
				case CS_DATETIME_TYPE:
					charpt=(CS_CHAR*)((CS_CHAR*)pt+ptcnt);
					memcpy(&(pdata[i*nCol+j].charvalue),charpt,tmpdatafmt[j].maxlength-1);
					ptcnt+=tmpdatafmt[j].maxlength;
					strncpy(strvalue,pdata[i*nCol+j].charvalue,tmpdatafmt[j].maxlength-1);
					db_TrimLeft(strvalue);
					db_TrimRight(strvalue);
					break;
				case CS_CHAR_TYPE:
					charpt=(CS_CHAR*)((CS_CHAR*)pt+ptcnt);
					memcpy(&(pdata[i*nCol+j].charvalue),charpt,tmpdatafmt[j].maxlength-1);
					ptcnt+=tmpdatafmt[j].maxlength;
					strncpy(strvalue,pdata[i*nCol+j].charvalue,tmpdatafmt[j].maxlength-1);
					if (tmpdatafmt[j].maxlength>1)//add 2004 11
					strvalue[tmpdatafmt[j].maxlength-1]=0;//add 2004 11
					db_TrimLeft(strvalue);
					db_TrimRight(strvalue);
					break;
				case CS_TINYINT_TYPE:
				case CS_BIT_TYPE:
					pdata[i*nCol+j].tinyvalue=*(CS_TINYINT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%d",pdata[i*nCol+j].tinyvalue);		
					break;
				case CS_SMALLINT_TYPE:
					if ((ptcnt % sizeof(CS_SMALLINT) ) != 0) ptcnt++;
					pdata[i*nCol+j].smallvalue=*(CS_SMALLINT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%d",pdata[i*nCol+j].smallvalue);		
					break;
				case CS_INT_TYPE:
					if ((ptcnt % sizeof(CS_INT)) != 0)
						ptcnt=(ptcnt/sizeof(CS_INT) +1)*sizeof(CS_INT);
					pdata[i*nCol+j].intvalue=*(CS_INT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%d",pdata[i*nCol+j].intvalue);		
					break;
				case CS_REAL_TYPE:
					if ((ptcnt % sizeof(CS_REAL)) != 0)
						ptcnt=((ptcnt/sizeof(CS_REAL))+1)*sizeof(CS_REAL);
					pdata[i*nCol+j].realvalue = *(CS_REAL*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%g",pdata[i*nCol+j].realvalue);
					break;
				case CS_FLOAT_TYPE:
					if ((ptcnt % sizeof(CS_FLOAT)) != 0)
						ptcnt=((ptcnt/sizeof(CS_FLOAT))+1)*sizeof(CS_FLOAT);
					pdata[i*nCol+j].floatvalue = *(CS_FLOAT*)((CS_CHAR*)pt+ptcnt);
					ptcnt+=tmpdatafmt[j].maxlength;
					sprintf(strvalue,"%lg",pdata[i*nCol+j].floatvalue);
					break;
				default:
					ptcnt+=tmpdatafmt[j].maxlength;
			}//switch
			fputs(strvalue,tmpstream);
			memset(strvalue,0,strlen(strvalue));//清空strvalue
			fputs("\t",tmpstream);
		}//for j
		fputs("\n",tmpstream);
		recordnum++;
	}//for i

	if(pdata)	free(pdata);
	pdata = 0;

	return 1;
}

#define HISDATANUM 5
#define CHECKTABLENUM 5

char *datatype[HISDATANUM]=//历史数据的结构
{
	"所有类型的历史数据",
	"存盘数据",
	"历史事项和操作记录",
	"追忆数据",
	"秒级曲线数据"
};
char *checktable[CHECKTABLENUM]=//检测代码校验的结构
{
	"所有数据",
	"SCD设备参数表",
	"遥测参数表",
	"遥信参数表",
	"电度参数表"
};

class CDbOperate
{
public:
	char argc;				// 入口参数个数
	char argv[MAXPARA][MAXPARALEN];	// 入口参数字符串

	int OpeType;		// 操作类型
	int bHelpFlag;		// 帮助标识
	int bPrompt;		// 操作每个表是否提示
	int bAutoOpe;		// 不输入口令和确认，直接执行
	int bAllTables;		// =1导入文件件内所有表，=0部分表不需要导入
	int oneallflag;		// =1对表操作，=2对库操作
	int bInputUser;		// 是否指定用户名
	char cUser[20];		// 指定的用户名
	char cPasswd[20];	// 指定的用户口令
	char mode[2];            //模式是0 追加 1 清空追加
	int HistableNameCount;	// histable中历史表的个数，用于oracle等单库来判别历史表
	int HistableNameLen;
	char *pHistableName;
	int UnchangedTableNameCount;	// 不需要导入的表的个数
	int UnchangedTableNameLen;
	char *pUnchangedTableName;
private:
	int get_input_char(char *strdisp,int flag=0);	// 输入 Y,N,Q (yes,no,quit)
	int db_start(int serverno,int anotherflag=0);	// 启动数据库
	int get_serverno();								// 取server号
	int get_dbname(char *dbname);					// 取数据库名
	int get_tablename(char *tablename);				// 取表名
	int get_filename(char *filename);				// 取文件名
	int realize_correct_onetable(CS_SMALLINT serverno,CS_SMALLINT sserverno,CS_CHAR *tablename,CS_CHAR *dbname);
	int realize_correct_table(CS_SMALLINT fserverno,CS_SMALLINT sserverno,CS_CHAR *dbname);
	int check_root_password(int srvno);				// 检验口令
	int do_in_hisdata();	/* 历史数据 */
	int do_in_para();		/* 参数 */
	int realize_in_file(FUNC_STR *func,char *filename,int null_num=0,int id=-1);
	int do_out_hisdata();	/* 历史数据 */
	int do_out_para();		/* 参数 */
	int realize_out_file(FUNC_STR *func,char *filename,int null_num=0,int id=-1);
	int load_histable_name();	//从histable中取历史表名
	int load_unchangedtable_name();	//获取不需要导入的表名
	int check_histable_name(char *tbname);	//检查是否历史表
	int check_unchangedtable_name(char *tbname);	//检查是否为不需要导入表
	int check_tmptable_name(char *tbname);  //检查是否临时表，临时表为 tmp_????
	int do_cmp_table(CS_CHAR *dbname,CS_CHAR *tablename,int id=-1);	//比较双服务器的一个表
	int do_cmp_db(CS_CHAR *dbname);		//比较双服务器的一个库
	int do_disp_table(CS_CHAR *dbname,CS_CHAR *tablename);	//显示表的信息
	int	check_table(CS_CHAR *tablename,int dataflag);	//比较表中记录的命名规则
	void CheckNameLogic(CHECK_VALUE *value,int checknum,int dataflag);//检查命名规则
public:
	CDbOperate();
	~CDbOperate();
	void DoOperate();		// 操作
	int GetOperateType();	// 取操作类型
	void DispHelp();		// 显示帮助信息
		
		/*
		  公用命令参数： [-P  -NQ -USR用户 -PWD口令]
		     -P      ：当操作库中所有表时，对每个表进行提示");
		     -NQ	 ：直接执行，不需输入口令和确认删除
		     -USR用户：以此用户登录数据库，无此选项则以modeldb用户登录 
		     -PWD口令：用户口令
		*/

	int do_cmp();
		/* 比较双服务器一致性");
		   dbmt cmp -选项");
		     -T表名  ：比较的表
		     -D库名  ：比较的库
		     
		*/
	int do_copy();
		/* 在服务器之间复制某个表或某个库中的所有表");
		   dbmt copy -Sn <-T表名 | -D库名> [-P -debug -NQ -USR用户 -PWD口令]");
		     -Sn     ：-S1 或 -S2 , 源服务器为服务器1（或服务器2）");
		     -T表名  ：复制一个表");
		     -D库名  ：复制库中的所有表。当-T -D同时存在时，-T有效,");
			
			 -debug  ：显示调试信息
		*/
	int do_delete();
		/* 删除历史数据 
		   dbmt del -Sn -B起始时间 -E终止时间 [-DTn -NQ -USR用户 -PWD口令]
		   -Sn			: -S1或-S2 
		   -B起始时间	: 格式为年月，如2002年1月 -B200201
		   -E终止时间	:
					 以下为可选项
		   -DTn			: 可选，删除的数据种类，不带此选项，则给出选择提示
		*/
	int do_out_file();
		/* 表或库->文件
		    参数表的文件名为其表名
		    历史数据以月为单位，名字为 表名yyyymm ,
			   如事项：电力历史事项表200201、monyc200201, 
			   但表名包含年月的除外，如 yc200201,dayyc200201
		   历史数据：
			 dbmt out -HD -Sn -YM年月 [-PATH目录 -DTn -debug -NQ -USR用户 -PWD口令]
				-HD     : 历史数据
				-Sn		: -S1 或-S2
				-YM		: 年月，yyyymm 如2002年1月为 200201
				-PATH目录: 文件的存放目录，必须已经存在
			    -DTn	: 可选，倒出的数据种类，不带此选项，则给出选择提示
		   参数表：
		 	 dbmt out -PA <-D库名 | -T表名> [-PATH目录 -P -debug -NQ -USR用户 -PWD口令]
				-PA     : 参数表
				-Sn		: -S1(nbdb) 或-S2(nbdbb)
				-D库名  : 库中所有表 modeldb
				-T		: 某个表 库名.dbo.表名
				-PATH目录: 文件的存放目录，必须已经存在
		*/
	int do_in_file();
		/* 文件->表或库
		    参数表的文件名为其表名
		    历史数据以月为单位，名字为 表名yyyymm ,
			    如事项：电力历史事项表200201、monthyc200201, 
			    但表名包含年月的除外，如 yc200201,dayyc200201
		   历史数据：
		 	 dbmt in -HD -Sn -YM年月 [-PATH目录 -DTn -debug -NQ -USR用户 -PWD口令]
				-HD     : 历史数据
				-Sn		: -S1 或-S2
				-YM		: 年月，yyyymm 如2002年1月为 200201
				-PATH目录: 文件的存放目录，必须已经存在
				-DTn	: 可选，倒入的数据种类，不带此选项，则给出选择提示
		   参数表：
			 dbmt in -PA <-D库名 | -T表名> [-PATH目录 -P -debug -NQ -USR用户 -PWD口令]
				-PA     : 参数表
				-Sn		: -S1(nbdb) 或-S2(nbdbb)
				-D库名  : 库中所有表 modeldb
				-T		: 某个表 库名.dbo.表名
				-PATH目录: 文件的存放目录，必须已经存在
		*/
	int do_clean();
		/* 清空某个表或某个库中的所有表");
		    dbmt clean -Sn <-T表名 | -D库名> [-P -debug -NQ -USR用户 -PWD口令]");
		     -Sn     ：-S1 或 -S2 , 源服务器为服务器1（或服务器2）");
		     -T表名  ：清空一个表");
		     -D库名  ：清空库中的所有表。当-T -D同时存在时，只有-T有效");
		*/
	int do_drop();
		/* 删除某个表或某个库中的所有表");
		    dbmt drop -Sn <-T表名 | -D库名> [-P -debug -NQ -USR用户 -PWD口令]");
		     -Sn     ：-S1 或 -S2 , 源服务器为服务器1（或服务器2）");
		     -T表名  ：删除一个表");
		     -D库名  ：删除库中的所有表。当-T -D同时存在时，只有-T有效");
		*/
	int do_disp();
		/* 显示数据库信息");
		     dbmt disp -Sn");
		     -Sn     ：-S1 或 -S2 , 源服务器为服务器1（或服务器2）");
		*/
	int do_scada();
		/*SCADA数据追赶文件的导入
		*/
	int do_dump();
		/*数据库日志清除
		*/
	int do_check();
		/*命名规则合理性检查
		*/
	int do_dfhis();
		/*
		*/
	int realize_in_file_ex(FUNC_STR *func,char *filename,CHECK_VALUE *change,int null_num=0,int id=-1);
		/*
		*/
};
CDbOperate::CDbOperate()
{
	OpeType=FLAG_ERROR;
	bHelpFlag=0;
	bPrompt=0;
	bAutoOpe=0;
	bAllTables=0;
	oneallflag=-1;
	bInputUser=0;
	cUser[0]=0;
	cPasswd[0]=0;
	HistableNameCount=0;
	pHistableName=NULL;
}

CDbOperate::~CDbOperate()
{
	if(pHistableName) free(pHistableName);
}

void CDbOperate::DoOperate()
{
	if( GetOperateType()==0 )
	{
		DispHelp();
		return;
	}

	switch(OpeType)
	{
	case FLAG_COMPARE:	/*比较双数据库*/
		do_cmp();
		break;
	case FLAG_COPY:	/* 在双服务器之间复制数据 */
		do_copy();
		break;
	case FLAG_DEL:	/* 删除历史数据 */
		do_delete();
		break;
	case FLAG_OUTFILE:
		do_out_file();
		break;
	case FLAG_INFILE:
		do_in_file();
		break;
	case FLAG_CLEAN:
		do_clean();
		break;
	case FLAG_DROP:
		do_drop();
		break;
	case FLAG_DISP:
		do_disp();
		break;
	case FLAG_SCADA:
		do_scada();
		break;
	case FLAG_DUMP:
		do_dump();
		break;
	case FLAG_CHECK:
		do_check();
		break;
	case FLAG_DFHIS://200508
		do_dfhis();
		break;
	default:
		DispHelp();
		break;
	}
}

int CDbOperate::GetOperateType()
{
	int i;
	int len;
	char cPathName[255];	// 文件存放的目录

	OpeType=FLAG_ERROR;
	bHelpFlag=0;
	bPrompt=0;
	bAutoOpe=0;

	if(argc<2)		//无参数
	{
		OpeType=0;
		bHelpFlag=1;
		return 0;
	}
	
    for (i=1;i<argc;i++)
		if (strcmp(argv[i],"/?")==0 || strcmp(argv[i],"help")==0 )
		{
			bHelpFlag=1;
			if(i==1) 
			{
				OpeType=FLAG_NO;
				return 0;
			}
		}
    for (i=1;i<argc;i++)
		if (strcmp(argv[i],"helpall")==0 )
		{
			bHelpFlag=2;
			if(i==1) 
			{
				OpeType=FLAG_ALL;
				return 0;
			}
		}
    for (i=1;i<argc;i++)	/* 对每个表显示提示信息 */
		if(strcmp(argv[i],"-P")==0)
			{	bPrompt=1; break;}
    for (i=1;i<argc;i++)	/* 是否输入口令和确认 */
		if(strcmp(argv[i],"-NQ")==0)
			{	bAutoOpe=1; break;}
    for (i=1;i<argc;i++)	/* 是否将每个表都导入或清空 */
		if(strcmp(argv[i],"-A")==0)
			{	bAllTables=1; break;}
	for (i=1;i<argc;i++)
		if(strncmp(argv[i],"-PATH",5)==0)
		{
			strcpy(cPathName,argv[i]+5);
			len=strlen(cPathName);
			if(len>1)
			{
				if(chdir(cPathName)!=0)
				{
					fprintf(stderr,"\n-PATH给出的目录 %s 不存在。\n",cPathName);
					return 0;
				}
			}
			break;
		}

	int bflag=0;
	for (i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-USR",4)==0)
		{
			if(argv[i][4]!=0)
				strcpy(cUser,argv[i]+4);
			bflag++;
		}
		else if(strncmp(argv[i],"-PWD",4)==0)
		{
			if(argv[i][4]!=0)
				strcpy(cPasswd,argv[i]+4);
			bflag++;
		}
       else if(strncmp(argv[i],"-M",2)==0)
		{
			if(argv[i][2]!=0)
				strcpy(mode,argv[i]+2);
		}
	}
	if(bflag==2) bInputUser=1;

	if(strcmp(argv[1],"cmp")==0) OpeType=FLAG_COMPARE;
	else if(strcmp(argv[1],"copy")==0) OpeType=FLAG_COPY;
	else if(strcmp(argv[1],"del")==0) OpeType=FLAG_DEL;
	else if(strcmp(argv[1],"out")==0) OpeType=FLAG_OUTFILE;
	else if(strcmp(argv[1],"in")==0) OpeType=FLAG_INFILE;
	else if(strcmp(argv[1],"clean")==0) OpeType=FLAG_CLEAN;
	else if(strcmp(argv[1],"drop")==0) OpeType=FLAG_DROP;
	else if(strcmp(argv[1],"disp")==0) OpeType=FLAG_DISP;
	else if(strcmp(argv[1],"scada")==0) OpeType=FLAG_SCADA;
	else if(strcmp(argv[1],"dump")==0) OpeType=FLAG_DUMP;
	else if(strcmp(argv[1],"check")==0) OpeType=FLAG_CHECK;
	else if(strcmp(argv[1],"dfhis")==0) OpeType=FLAG_DFHIS;//200508
	else 
	{
		OpeType=FLAG_ERROR;
		bHelpFlag=1;
		return 0;
	}
	if(bHelpFlag>0) return 0;
	else return 1;
}

void CDbOperate::DispHelp()
{
	if(bHelpFlag==0) return;

	switch(OpeType)
	{
	case FLAG_ERROR:
		fprintf(stderr,"\n 无此命令，请用dbmt help 查询命令!");
		break;
	case FLAG_NO:
        fprintf(stderr,"系统管理员命令: ");
		fprintf(stderr,"\n dbmt < copy | del | out | in | cmp | scada | dump | disp | drop | clean | check> [help] ");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n 命令：");
		fprintf(stderr,"\n dbmt out  : 复制参数表或历史数据 -> 文件");
		fprintf(stderr,"\n dbmt in   : 从文件装入参数表或历史数据到数据库");
		fprintf(stderr,"\n dbmt copy : 在双服务器之间复制数据");
		fprintf(stderr,"\n dbmt del  : 删除某时期的历史数据");
		fprintf(stderr,"\n dbmt cmp  : 比较数据库或数据库中的表");
		fprintf(stderr,"\n dbmt scada: 将数据追赶的文件插入到数据库中");
		fprintf(stderr,"\n dbmt clean: 清空某个表或某个库中的所有表");
		fprintf(stderr,"\n dbmt drop : 删除某个表或某个库中的所有表");
		fprintf(stderr,"\n dbmt dump : 清除数据库日志");
		fprintf(stderr,"\n dbmt disp : 显示数据库信息");
		fprintf(stderr,"\n dbmt check : 检查SCD命名规则");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n help    ：显示命令的详细使用方法，例 dbmt copy help");
		fprintf(stderr,"\n");
		break;
	case FLAG_ALL:
        fprintf(stderr,"系统管理员命令: ");
		fprintf(stderr,"\n dbmt <命令> [help] [ -P -NQ -USR用户 -PWD口令]");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n 命令：");
		fprintf(stderr,"\n dbmt out  : 复制参数表或历史数据 -> 文件");
		fprintf(stderr,"\n dbmt in   : 从文件装入参数表或历史数据到数据库");
		fprintf(stderr,"\n dbmt copy : 在双服务器之间复制数据");
		fprintf(stderr,"\n dbmt del  : 删除某时期的历史数据");
		fprintf(stderr,"\n dbmt cmp  : 比较双服务器的表或库");
		fprintf(stderr,"\n dbmt clean: 清空某个表或某个库中的所有表");
		fprintf(stderr,"\n dbmt drop : 删除某个表或某个库中的所有表");
		fprintf(stderr,"\n dbmt disp : 显示数据库信息");
		fprintf(stderr,"\n dbmt dump : 清除数据库日志");
		fprintf(stderr,"\n dbmt scada: 将数据追赶的文件插入到数据库中");
		fprintf(stderr,"\n dbmt check : 检查SCD命名规则");
		fprintf(stderr,"\n");
		fprintf(stderr,"\n help    ：显示命令的详细使用方法，例 dbmt copy help");
		fprintf(stderr,"\n -P      ：当进行操作时，不对每个操作进行提示");
		fprintf(stderr,"\n -NQ     ：直接执行，不再提示输入口令和对操作进行确认");
		fprintf(stderr,"\n -USR用户：以此用户登录数据库，无此选项则以系统缺省用户登录");
		fprintf(stderr,"\n -PWD口令：用户口令");
		fprintf(stderr,"\n");
		break;
	case FLAG_COPY:
		fprintf(stderr,"\n dbmt copy : 在服务器之间复制某个表或某个库中的所有表");
		
		if(bHelpFlag==2) 
			fprintf(stderr,"\n      dbmt copy -Snn <-T表名 | -D库名> [-P -NQ -USR用户 -PWD口令]");
		else 
			fprintf(stderr,"\n      dbmt copy -Snn <-T表名 | -D库名> [-P]");
		fprintf(stderr,"\n      -Snn     ：例: -S13  将服务器1的数据拷贝到服务器3上");
		fprintf(stderr,"\n              ：     -S42  将服务器4的数据拷贝到服务器2上");
		fprintf(stderr,"\n      -T表名  ：复制一个表");
		fprintf(stderr,"\n        当不是modeldb中表时,－T后表名带上库名 例如：-Thisdb..历史事项表");
		fprintf(stderr,"\n      -D库名  ：复制库中的所有表。当-T -D同时存在时，只有-T有效");
		fprintf(stderr,"\n      -P      ：复制库中所有表时，对每个表进行提示");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -NQ     ：直接执行，不再提示输入口令和对操作进行确认");
		fprintf(stderr,"\n      -USR用户：以此用户登录数据库，无此选项则以系统缺省用户登录");
		fprintf(stderr,"\n      -PWD口令：用户口令");
		break;
	case FLAG_DEL:
		fprintf(stderr,"\n dbmt del : 删除某时期的历史数据");
		if(bHelpFlag==2)
			fprintf(stderr,"\n      dbmt del -Sn -B起始时间 -E终止时间  [-DTn -NQ -USR用户 -PWD口令]");
		else 
			fprintf(stderr,"\n      dbmt del -Sn -B起始时间 -E终止时间");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -B起始时间 ：");
		fprintf(stderr,"\n      -E终止时间 ：删除历史数据的时间段（包含起始和终止时间）");
		fprintf(stderr,"\n                   格式为：年月，如2002年1月：200201");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -DTn    ：删除的数据种类n，不带此选项，则给出选择提示");
		fprintf(stderr,"\n      -NQ     ：直接执行，不再提示输入口令和对操作进行确认");
		fprintf(stderr,"\n      -USR用户：以此用户登录数据库，无此选项则以系统缺省用户登录");
		fprintf(stderr,"\n      -PWD口令：用户口令");
		break;
	case FLAG_COMPARE:	
		fprintf(stderr,"\n dbmt cmp : 比较双服务器的表或库");
		fprintf(stderr,"\n      dbmt cmp -Snn <-T表名 | -D库名>");
		fprintf(stderr,"\n      -Snn     ：例: -S12  比较服务器1和服务器2");
		fprintf(stderr,"\n              ：    -S24  比较服务器2和服务器4");
		fprintf(stderr,"\n      -T表名  :  比较的表");
		fprintf(stderr,"\n      -D库名  :  比较的表所在的库（若不写表名）就是直接比较库");
		break;
	case FLAG_SCADA:	
		fprintf(stderr,"\n dbmt scada : 将数据追赶的文件插入到数据库中");
		fprintf(stderr,"\n      dbmt scada -选项");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -F文件名	：追赶的文件名");
		fprintf(stderr,"\n      -PATH目录: 文件的存放目录，无此选项则为当前目录");
		fprintf(stderr,"\n      -P      ：进行追赶之前先对数据库进行删除操作（会造成很慢）");
		fprintf(stderr,"\n      例如：-PATHc:\\\\ds3210\\\\procscript");
		break;
	case FLAG_OUTFILE:
		fprintf(stderr,"\n dbmt out : 复制参数表或某月的历史数据 --> 文件");
		fprintf(stderr,"\n                 对数据库中数据无影响。");
		fprintf(stderr,"\n    参数：");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt out  -Sn <-T表名 | -D库名> [-PATH目录 -P -NQ -USR用户 -PWD口令]");
		else	
			fprintf(stderr,"\n    dbmt out  -Sn <-T表名 | -D库名> [-PATH目录 -P]");
		fprintf(stderr,"\n       主要复制参数表-->文件，文件名为表名");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -T表名  ：复制一个表");
		fprintf(stderr,"\n      -D库名  ：复制库中的所有表(若有表名，则为表所在的库名)");
		fprintf(stderr,"\n      -PATH目录: 文件的存放目录，无此选项则为当前目录");
		fprintf(stderr,"\n      -P      ：复制库中所有表时，对每个表进行提示");
		fprintf(stderr,"\n    历史数据：");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt out -Dhisdb -Sn -YM年月 [-PATH目录 -DTn -NQ -USR用户 -PWD口令]");
		else
			fprintf(stderr,"\n    dbmt out -Dhisdb -Sn -YM年月 [-PATH目录]");
		fprintf(stderr,"\n      主要复制历史数据表 --> 文件，不删除数据库中的数据。");
		fprintf(stderr,"\n      文件名为(表名+年月),文件名不要改动，因为往数据库中恢复使用");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -YM年月 ：时间，格式为 YYYYMM,如2002年1月为 -YM200201");
		fprintf(stderr,"\n      -PATH目录: 文件的存放目录，无此选项则为当前目录");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -DTn    ：删除的数据种类n，不带此选项，则给出选择提示");
		fprintf(stderr,"\n      -NQ     ：直接执行，不再提示输入口令和对操作进行确认");
		fprintf(stderr,"\n      -USR用户：以此用户登录数据库，无此选项则以系统缺省用户登录");
		fprintf(stderr,"\n      -PWD口令：用户口令");
		break;
	case FLAG_INFILE:
		fprintf(stderr,"\n dbmt in : 从文件装入参数表或某月的历史数据");
		fprintf(stderr,"\n    参数：");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt in -Sn <-T表名 | -D库名> [-PATH目录 -P -NQ -A -USR用户 -PWD口令]");
		else
			fprintf(stderr,"\n    dbmt in  -Sn <-T表名 | -D库名> [-PATH目录 -P]");
		fprintf(stderr,"\n      从文件装入记录到参数表");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -T表名  ：装入一个表");
		fprintf(stderr,"\n      -D库名  ：装入库中的所有表（若不写表名）就是直接装入库");
        fprintf(stderr,"\n      -M模式  ：-Ma追加,-Mb 先清空再追加 ");
		fprintf(stderr,"\n      -PATH目录: 文件的存放目录，无此选项则为当前目录");
		fprintf(stderr,"\n      -P      ：装入库中所有表时，对每个表进行提示");
		fprintf(stderr,"\n      -A      ：装入库中所有表，无此选项则下列表不装入库:");
		fprintf(stderr,"\n               (pdb_field、pdb_table、rtdb_field、rtdb_table、topocolor、程序注册表、标志牌操作菜单配置表、SCD对象类型表、事项类型表、事项句表、电压等级表)");
		fprintf(stderr,"\n  ");
		fprintf(stderr,"\n    历史数据：");
		if(bHelpFlag==2)
			fprintf(stderr,"\n    dbmt in -Dhisdb -Sn -YM年月 [-PATH目录 -DTn -NQ -USR用户 -PWD口令]");
		else
			fprintf(stderr,"\n    dbmt in -Dhisdb -Sn -YM年月 [-PATH目录]");
		fprintf(stderr,"\n      装入历史数据到数据库，不删除数据库中的原有数据。");
		//fprintf(stderr,"\n      当数据库中该月的数据存在时，不需要装入数据；确实要装入，");
		//fprintf(stderr,"\n      请用 dbmt del 命令先删除该月的历史数据。");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
        fprintf(stderr,"\n      -M模式  ：-Ma追加,-Mb 先清空再追加 ");
		fprintf(stderr,"\n      -YM年月 ：时间，格式为 YYYYMM,如2002年1月为 -YM200201");
		fprintf(stderr,"\n      -PATH目录: 文件的存放目录，无此选项则为当前目录");
		if(bHelpFlag!=2) break;
		fprintf(stderr,"\n      -DTn    ：删除的数据种类n，不带此选项，则给出选择提示");
		fprintf(stderr,"\n      -NQ     ：直接执行，不再提示输入口令和对操作进行确认");
		fprintf(stderr,"\n      -USR用户：以此用户登录数据库，无此选项则以系统缺省用户登录");
		fprintf(stderr,"\n      -PWD口令：用户口令");
		break;
	case FLAG_CLEAN:
		fprintf(stderr,"\n dbmt clean : 清空某个表或某个库中的所有表");
		fprintf(stderr,"\n      dbmt clean -Sn <-T表名 | -D库名> [-P] -A");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -T表名  ：清空一个表");
		fprintf(stderr,"\n      -D库名  ：清空的表所在的库（若不写表名）就是直接比较库");
		fprintf(stderr,"\n      -P      ：清空库中所有表时，对每个表进行提示");
		fprintf(stderr,"\n      -A      ：清空库中所有表，无此选项则下列表不进行清空：");
		fprintf(stderr,"\n               (pdb_field、pdb_table、rtdb_field、rtdb_table、topocolor、程序注册表、标志牌操作菜单配置表、SCD对象类型表、事项类型表、事项句表、电压等级表)");
		break;
	case FLAG_DROP:
		fprintf(stderr,"\n dbmt drop : 删除某个表或某个库中的所有表");
		fprintf(stderr,"\n      dbmt drop -Sn <-T表名 | -D库名> [-P]");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -T表名  ：删除一个表");
		fprintf(stderr,"\n      -D库名  ：删除的表所在的库（若不写表名）就是直接比较库");
		fprintf(stderr,"\n      -P      ：删除库中所有表时，对每个表进行提示");
		break;
	case FLAG_DISP:
		fprintf(stderr,"\n dbmt disp : 显示数据库信息");
		fprintf(stderr,"\n      dbmt disp -Sn");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -T表名  ：显示表信息的表名");
		break;
	case FLAG_DUMP:
		fprintf(stderr,"\n dbmt dump : 清除数据库日志");
		fprintf(stderr,"\n      dbmt dump -Sn");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -D库名  ：清除日志的库名");
		break;
	case FLAG_CHECK:
		fprintf(stderr,"\n dbmt check : 检查SCD命名规则");
		fprintf(stderr,"\n      dbmt check -Sn");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		break;
	case FLAG_DFHIS:
		fprintf(stderr,"\n dbmt dfhis : 东方电子历史数据导入");
		fprintf(stderr,"\n      dbmt dfhis -Sn -YM年月");
		fprintf(stderr,"\n      -Sn     ：-S1 或 -S2 , -S3 , -S4, 服务器1（或服务器2,服务器3,服务器4）");
		fprintf(stderr,"\n      -YM年月 ：时间，格式为 YYYYMM,如2002年1月为 -YM200201");
		fprintf(stderr,"\n      注意：需要在本系统内存在：遥测关系对应表 、遥信关系对应表");
		break;
	default:
		fprintf(stderr,"\n 无此命令，请用dbmt help 查询命令!");
		break;
	}
}

/*
	从控制台输入 Y,N,Q (yes,no,quit)
	strdisp : 显示的语句
	flag    : 输入字符标志，=0 Y/N  =1 Y/N/Q
	default_input : 缺省的输入字符。
*/
int CDbOperate::get_input_char(char *strdisp,int flag)
{
	int inchar;
	char instr[255];

	if(bAutoOpe==1) return 'Y';
	do
	{
		fprintf(stderr,"%s",strdisp);
		gets(instr);
		inchar=toupper(instr[0]);
	}while( !(inchar=='Y' || inchar=='N' || (flag==1 && inchar=='Q')) );
	return inchar;
}

int CDbOperate::db_start(int serverno,int anotherflag)
{
	LOG_INFO *loginfo;
	int retcode;
	
	if(serverno<0)
	{
		fprintf(stderr,"\n错误：无指定服务器(%d)\n",serverno+1);
		return CS_FAIL;
	}

	loginfo=(LOG_INFO *)malloc (sizeof(LOG_INFO));
	memset(loginfo,0,sizeof(LOG_INFO));

	switch(serverno)
	{
	case 1:
		strcpy(loginfo->servername,"nbdb");
		break;
	case 2:
		strcpy(loginfo->servername,"nbdbb");
		break;
	case 3:
		strcpy(loginfo->servername,"nbdb1");
		break;
	case 4:
		strcpy(loginfo->servername,"nbdbb1");
		break;
	default:
		break;
	}
	if (anotherflag == DF_HIS) 
		strcpy(loginfo->servername,"dfdb");

	strcpy(loginfo->username,"modeldb");
	strcpy(loginfo->password,"supernb");

	if(bInputUser==1 && loginfo!=NULL)
	{
		strcpy(loginfo->username,cUser);
		strcpy(loginfo->password,cPasswd);
	}

	loginfo->useflag=1;

	retcode=dbsingle.Open(0,loginfo,0,1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n错误：服务器%s打开失败!\n",loginfo->servername);
		fprintf(stderr,"请检查 1、服务器%s是否运行\n",loginfo->servername);
		fprintf(stderr,"       2、客户端配置是否正确\n");

		if (loginfo) free (loginfo);
		return retcode;
	}

	
	if (loginfo) free(loginfo);
	return retcode;
}

/* 从命令行中取server号*/
int CDbOperate::get_serverno()
{
	int serverno=-1;
	for (int i=1;i<argc;i++)
	{
		if (!(strcmp(argv[i],"-S1")))
		{	serverno=1; break;}
		if (!(strcmp(argv[i],"-S2")))
		{	serverno=2; break;}
		if (!(strcmp(argv[i],"-S3")))
		{	serverno=3; break;}
		if (!(strcmp(argv[i],"-S4")))
		{	serverno=4; break;}
		if (!(strcmp(argv[i],"-S12")))
		{	serverno=12; break;}
		if (!(strcmp(argv[i],"-S13")))
		{	serverno=13; break;}
		if (!(strcmp(argv[i],"-S14")))
		{	serverno=14; break;}
		if (!(strcmp(argv[i],"-S23")))
		{	serverno=23; break;}
		if (!(strcmp(argv[i],"-S24")))
		{	serverno=24; break;}
		if (!(strcmp(argv[i],"-S34")))
		{	serverno=34; break;}
		if (!(strcmp(argv[i],"-S21")))
		{	serverno=21; break;}
		if (!(strcmp(argv[i],"-S31")))
		{	serverno=31; break;}
		if (!(strcmp(argv[i],"-S41")))
		{	serverno=41; break;}
		if (!(strcmp(argv[i],"-S32")))
		{	serverno=32; break;}
		if (!(strcmp(argv[i],"-S42")))
		{	serverno=42; break;}
		if (!(strcmp(argv[i],"-S43")))
		{	serverno=43; break;}
	}
	return serverno;
}
/* 从命令行中取库名 -D库名
   =1 成功， =0 失败
*/
int CDbOperate::get_dbname(char *dbname)
{
    for (int i=1;i<argc;i++)
	{
		if (strncmp(argv[i],"-D",2)==0)
		{
			if (argv[i][2]!=0)
			{
				strcpy(dbname,argv[i]+2);
				return 1;
			}
			fprintf(stderr,"\n-D 未提供库名!");
			return 0;
		}
	}
	return 0;
}
/* 从命令行中取表名 -T表名
   =1 成功， =0 失败
*/
int CDbOperate::get_tablename(char *tablename)
{   
	int i,k=0,len=0;
    for (i=1;i<argc;i++)
	{
		if (strncmp(argv[i],"-T",2)==0)
		{
			if (argv[i][2]!=0)
			{
				strcpy(tablename,argv[i]+2);
			if (dbtype==DBTYPE_ORACLE)//大小写转换
			{
				len=strlen(tablename);
				for(k=0;k<len;k++)
				if( tablename[k]>='a' && tablename[k]<='z' )
				tablename[k]=toupper(tablename[k]);
			}
				return 1;
			}
			fprintf(stderr,"\n-T 未提供表名!");
			return 0;
		}
	}
	return 0;
}

int CDbOperate::load_histable_name()
{
    HistableNameLen=50;
	int MaxTableName=50;
	pHistableName=(char *)malloc(MaxTableName*HistableNameLen);
	if(pHistableName==NULL) 
	{
		HistableNameLen=0;
		HistableNameCount=0;
		return CS_FAIL;
	}
	HistableNameCount=0;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yx");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"kwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"bk");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"dayyc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"daykwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"monthyc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"monthkwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yearyc");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"yearkwh");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"sdata");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"电力历史事项表");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"操作历史事项表");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"设备运行统计表");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"pdrdescribe");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"pdrdata");
	HistableNameCount++;
	strcpy(pHistableName+HistableNameLen*HistableNameCount,"开关变位统计表");
	HistableNameCount++;
	return CS_SUCCEED;
}

int CDbOperate::load_unchangedtable_name()
{
	UnchangedTableNameLen=50;
	int MaxTableName=50;
	pUnchangedTableName=(char *)malloc(MaxTableName*UnchangedTableNameLen);
	if(pUnchangedTableName==NULL) 
	{
		UnchangedTableNameLen=0;
		UnchangedTableNameCount=0;
		return CS_FAIL;
	}
	UnchangedTableNameCount=0;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"pdb_field");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"pdb_table");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"rtdb_field");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"rtdb_table");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"程序注册表");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"标志牌操作菜单配置表");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"SCD对象类型表");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"事项类型表");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"事项句表");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"电压等级表");
	UnchangedTableNameCount++;
	strcpy(pUnchangedTableName+UnchangedTableNameLen*UnchangedTableNameCount,"topocolor");
	UnchangedTableNameCount++;

	return CS_SUCCEED;
}

int CDbOperate::check_histable_name(char *tbname)
{
	char tmpname[50];
	char *p;
	int len;
	if(pHistableName==NULL) return CS_FAIL;
	
	p=strrchr(tbname,'.');
	if(p!=NULL) strcpy(tmpname,p+1);
	else strcpy(tmpname,tbname);

	for(int i=0;i<HistableNameCount;i++)
	{
		/* 比较是否历史表： yc????,kwh????...
		   因为 pHistableName中可能只记录历史表名的前一部分（后一部分为年月） */
		len=strlen(pHistableName+i*HistableNameLen);
#if defined(__unix)
		if(strncasecmp(tmpname,pHistableName+i*HistableNameLen,len)==0)
#else 
		if(strnicmp(tmpname,pHistableName+i*HistableNameLen,len)==0)
#endif
			return CS_SUCCEED;	
	}
	return CS_FAIL;
}

int CDbOperate::check_unchangedtable_name(char *tbname)
{
	char tmpname[50];
	char *p;
	int len;
	if(pUnchangedTableName==NULL) return CS_FAIL;
	
	p=strrchr(tbname,'.');
	if(p!=NULL) strcpy(tmpname,p+1);
	else strcpy(tmpname,tbname);

	for(int i=0;i<UnchangedTableNameCount;i++)
	{
		len=strlen(pUnchangedTableName+i*UnchangedTableNameLen);
#if defined(__unix)
		if(strncasecmp(tmpname,pUnchangedTableName+i*UnchangedTableNameLen,len)==0)
#else 
		if(strnicmp(tmpname,pUnchangedTableName+i*UnchangedTableNameLen,len)==0)
#endif
			return CS_SUCCEED;	
	}
	return CS_FAIL;
}


int CDbOperate::check_tmptable_name(char *tbname)
{
#if defined(__unix)
	if(strncasecmp(tbname,"tmp_",4)==0)
#else 
	if(strnicmp(tbname,"tmp_",4)==0)
#endif
	return CS_SUCCEED;
	return CS_FAIL;
}

/* 复制单个表 */
int CDbOperate::realize_correct_onetable(CS_SMALLINT fserverno,CS_SMALLINT sserverno,CS_CHAR *tablename,CS_CHAR *dbname)
{
    FUNC_STR        func;
	memset(&func,0,sizeof(FUNC_STR));
    CS_RETCODE      retcode;
	char filename[255];

    strcpy(func.table,tablename);
	strcpy(func.dbname,dbname);
	strcpy(filename,tablename);

	retcode=db_start(fserverno);
	if (retcode!=CS_SUCCEED) 
		return (retcode);

	retcode=realize_out_file(&func,filename);
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n   获取源服务器数据错误!");
		remove(filename);
	    return (retcode);
	}
	dbsingle.Close();
	//打开另一个服务器
	retcode=db_start(sserverno);
	if (retcode!=CS_SUCCEED) 
	{
		remove(filename);
		return retcode;
	}

	strcpy(func.table,tablename);
	strcpy(func.dbname,dbname);
	retcode=realize_in_file(&func,filename);
	if (retcode!=CS_SUCCEED) 
	{
		fprintf(stderr,"\n   向目标服务器插入数据错误!");
		remove(filename);
		return retcode;
	}
	remove(filename);
	dbsingle.Close();
	//if (p6) free (p6);
    return (retcode);
}

/* 复制整个库中的所有表 */
int CDbOperate::realize_correct_table(CS_SMALLINT fserverno,CS_SMALLINT sserverno,CS_CHAR *dbname)
{   
    FUNC_STR        func;
	memset(&func,0,sizeof(FUNC_STR));
    CS_RETCODE      retcode;
	int inchar;
	int tbcount=0;
	CS_DATAFMT *datafmt;
	char *pt=NULL;
    if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) ) //modify by chenkai 增加mysql
	if(pHistableName==NULL) load_histable_name();


if (dbtype==DBTYPE_DB2)
	sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T'");
else if(dbtype==DBTYPE_ORACLE)
	sprintf(func.isql,"select table_name from user_tables");
else if(dbtype==DBTYPE_DMSQL)
	sprintf(func.isql,"select table_name from user_tables");
else if (dbtype==DBTYPE_MYSQL)					//modify by chenkai 增加mysql
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
	sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U'",dbname);

	retcode=db_start(fserverno);
	if (retcode!=CS_SUCCEED) 
		return (retcode);

	func.func=SEL_ISQL_RESULTS;
	retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr," 从库中取表名失败!\n");
		if (datafmt) free(datafmt);
		if (pt) free(pt);
		return CS_FAIL;
	}

	int tbnum=func.ret_roxnum;
	int i;
	int hisflag=0;
	int reclen=func.recorderlen;
	if(strcmp(dbname,"hisdb")==0)
		hisflag=1;
	tbcount=0;
	for(i=0;i<tbnum;i++)
	{
		if(check_tmptable_name(pt+i*reclen)==1)
		{	//临时表，将表名置为空
			*(pt+i*reclen)=0;
			continue;
		}
		if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE ) || ( dbtype == DBTYPE_MYSQL) || (dbtype==DBTYPE_DMSQL ) )  //modify by chenkai 增加mysql
		{
			retcode=check_histable_name(pt+i*reclen);
				// 参数库：只处理参数表; 历史库：只处理历史表
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
				*(pt+i*reclen)=0;
				continue;
			}
		}
		tbcount++;
	}
	fprintf(stderr," 已从库中取出表名: %d 个表。\n",tbcount);
	dbsingle.Close();

	tbcount=0;
	for(i=0;i<tbnum;i++)
	{
		if(*(pt+i*reclen)==0) continue;
		tbcount++;
		sprintf(func.table,"%s",pt+i*reclen);
		fprintf(stderr,"\n\n---------------------------------------------------");
		

		if(bPrompt==1)
		{
			sprintf(strstdin,"\n%d: 是否修补表 [ %s ] 是/否/退出 (Y/N/Q)? ",tbcount,func.table);
			inchar=get_input_char(strstdin,1);
			if(inchar==(int)'Q') break;
			if (inchar==(int)'N') 
				continue;
		}

		fprintf(stderr,"\n%d: 正在修补表 [ %s ] ...",tbcount,func.table);
		retcode=realize_correct_onetable(fserverno,sserverno,func.table,dbname);
	}
	if (datafmt) free(datafmt);
	if(pt) free(pt);
    return 1;
}

/* 检查系统管理员口令 	*/
int CDbOperate::check_root_password(int srvno)
{
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_DATAFMT *datafmt;
	char *pt=NULL;
	char inpasswd[255];
	int i=0;

	if (bAutoOpe) return CS_SUCCEED;
	//inpasswd[0]=0;
	fprintf(stderr,"\n请输入系统管理员口令: ");
	if(bAutoOpe==1) strcpy(inpasswd,"dbtools");
	else 
	{
		getpass( inpasswd );
		/*do
		{
			letter=getch();
			inpasswd[i]=letter;
			i++;
		}
		while (letter!='\n');
		*/
	}
	inpasswd[i-1]='\0';
	if(strcmp(inpasswd,"dbtools")==0)	
		return CS_SUCCEED;
	
	func.func=SEL_ISQL_RESULTS;
	strcpy(func.isql,"select 密码 from 用户口令参数表 where 操作员='sa' ");
	int ret=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
	if(ret!=CS_SUCCEED)
	{
		fprintf(stderr,"\n取系统管理员口令失败!");

		if (datafmt) free (datafmt);
		if (pt) free (pt);

		return CS_FAIL;
	}
	if(func.ret_roxnum==0) 
	{
		fprintf(stderr,"\n未配置系统管理员!");

		if (datafmt) free (datafmt);
		if (pt) free (pt);

		return CS_FAIL;
	}
	char rootpasswd[24];
	strncpy(rootpasswd,pt,24);

	if (datafmt) free(datafmt);
	if(pt) free(pt);

	if(strcmp(inpasswd,rootpasswd)==0)	
		return CS_SUCCEED;
	fprintf(stderr,"\n口令错误!");
	return CS_FAIL;
}

int CDbOperate::do_cmp()
{
    int objflag=0;
	char tablename[100],dbname[100];

	if(get_tablename(tablename)==1) 
	{
		objflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   未提供库名，请用dbmt cmp help 帮助!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) objflag=2;
	

	if(objflag==0) 
	{
	    fprintf(stderr,"\n命令格式不对! 请用 dbmt cmp help 帮助。\n");
		return 0;
	}
	
	if (objflag==1)	do_cmp_table(dbname,tablename);
	else if (objflag==2) do_cmp_db(dbname);

	return 1;
}
int CDbOperate::do_cmp_table(CS_CHAR *dbname,CS_CHAR *tablename,int id)
{
	CS_RETCODE retcode;
	CS_SMALLINT serverno,firstserverno,secondserverno;
	int i=0;

	serverno=get_serverno();
	if(serverno<12||serverno>43)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名或服务器不存在。请用 dbmt in help 帮助。\n");
	    return 0;
	}else
	{
		firstserverno = (int)(serverno/10);
		secondserverno = (int)(serverno%10);
	}

	if (id==-1)
	fprintf(stderr,"\n		表名：%s",tablename);
	else
	fprintf(stderr,"\n%d		表名：%s",id,tablename);

	retcode=db_start(firstserverno);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			数据库打开失败!");
		return retcode;
	}
	
	CS_DATAFMT *datafmt1,*datafmt2;
	FUNC_STR func1,func2;
	void *pt1,*pt2;

	memset(&func1,0,sizeof(FUNC_STR));
	memset(&func2,0,sizeof(FUNC_STR));

	func1.func=GET_TABLE_INFO;
	strcpy(func1.table,tablename);
	strcpy(func1.dbname,dbname);
	retcode=dbsingle.GetTableInfo(&func1,&datafmt1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			取服务器(%d)的%s表信息失败!",firstserverno,tablename);
		if (datafmt1) free(datafmt1);
		return retcode;
	}
	for (i=0;i<func1.colnum;i++)
	{
		func1.col[i].condition = NO;
		func1.col[i].select = YES;
		func1.col[i].order  = -1;
		func1.col[i].order_d = i;		
	}
	func1.func=SEL_RECORDERS;
	retcode=dbsingle.SelectRcds(&func1,datafmt1,&pt1);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			取服务器(%d)的%s表记录个数失败!",firstserverno,tablename);
		if (datafmt1) free(datafmt1);
		return retcode;
	}

	dbsingle.Close();

	retcode=db_start(secondserverno);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			数据库打开失败!");
		return retcode;
	}
	
	func2.func=GET_TABLE_INFO;
	strcpy(func2.table,tablename);
	strcpy(func2.dbname,dbname);
	retcode=dbsingle.GetTableInfo(&func2,&datafmt2);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			取服务器(&d)的 %s 表信息失败!",secondserverno,tablename);
		if (datafmt1) free(datafmt1);
		if (datafmt2) free(datafmt2);
		return retcode;
	}
	for ( i=0;i<func2.colnum;i++)
	{
		func2.col[i].condition = NO;
		func2.col[i].select = YES;
		func2.col[i].order  = -1;
		func2.col[i].order_d = i;		
	}
	func2.func=SEL_RECORDERS;
	retcode=dbsingle.SelectRcds(&func2,datafmt2,&pt2);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n			取服务器(%d)的%s表记录个数失败!",secondserverno,tablename);
		if (datafmt2) free(datafmt2);
		if (datafmt1) free(datafmt1);
		return retcode;
	}

	for(i=0;i<func1.colnum;i++)//为了比较datafmt 而作
	{
		(datafmt1+i)->locale=0;
		(datafmt2+i)->locale=0;
	}
	if (func1.colnum!=func2.colnum) fprintf(stderr,"\n		结果：列数不一致!");
	else if (func1.ret_roxnum!=func2.ret_roxnum) fprintf(stderr,"\n		结果：记录个数不一致!");
	else if (memcmp(datafmt1,datafmt2,(sizeof(CS_DATAFMT)*func1.colnum))!=0) fprintf(stderr,"\n		结果：列信息不一致!");
	else if (memcmp(pt1,pt2,func1.recorderlen*func1.ret_roxnum)!=0) fprintf(stderr,"\n		结果：记录内容不一致!");
	else
	{
		fprintf(stderr,"\n		结果：一致!");
	}

	dbsingle.Close();
	
	if (datafmt1) free(datafmt1);
	if (datafmt2) free(datafmt2);

	if (pt1) free(pt1);
	if (pt2) free(pt2);
	return retcode;
}

int CDbOperate::do_cmp_db(CS_CHAR *dbname)
{
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode;
	CS_DATAFMT *datafmt;
	int inchar;

	CS_SMALLINT serverno,firstserverno,secondserverno;

	serverno=get_serverno();
	if(serverno<12||serverno>43)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名或服务器不存在。请用 dbmt in help 帮助。\n");
	    return 0;
	}else
	{
		firstserverno = (int)(serverno/10);
		secondserverno = (int)(serverno%10);
	}
	retcode=db_start(firstserverno);//以第一个服务器为标准取得全部的表名
	if (retcode!=CS_SUCCEED) return retcode;

	if (dbtype==DBTYPE_ORACLE)
		sprintf(func.isql,"select table_name from user_tables");
	else if (dbtype==DBTYPE_DMSQL)
		sprintf(func.isql,"select table_name from user_tables");	
	else if (dbtype==DBTYPE_MYSQL)			// add by chenkai 增加mysql
	{
		sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
	}
	else
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);
	
	char *pt=NULL;
	int tbnum,tbcount;
	int reclen,i,hisflag;
	func.func=SEL_ISQL_RESULTS;
	retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n 从库中取表名失败!\n");
		if (datafmt) free (datafmt);
		if (pt) free (pt);
		return retcode;
	}
	tbnum=func.ret_roxnum;
	reclen=func.recorderlen;
	tbcount=0;
	if (strcmp(dbname,"modeldb")==0)
		hisflag=0;
	else hisflag =1;
	load_histable_name();
	for(i=0;i<tbnum;i++)
	{
		if(check_tmptable_name(pt+i*reclen)==1)
		{	//临时表，将表名置为空仅oracle有
			*(pt+i*reclen)=0;
			continue;
		}
		if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype == DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )
		{		
			retcode=check_histable_name(pt+i*reclen);
					// 参数库：只处理参数表; 历史库：只处理历史表
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
					*(pt+i*reclen)=0;
					continue;
			}
		}
		tbcount++;
	}
	fprintf(stderr,"\n 已从库中取出表名: %d 个表。\n",tbcount);
	tbcount=0;
	
	dbsingle.Close();//关闭服务器1

	for(i=0;i<tbnum;i++)
	{
		strcpy(func.table,pt+i*reclen);
		if(func.table[0]==0) continue;
		tbcount++;
		if(bPrompt==1)
		{
			sprintf(strstdin,"\n%d: 是否比较表 [ %s ]  : 是/否/退出 (Y/N/Q)? ",tbcount,func.table);
			inchar=get_input_char(strstdin,1);
			if(inchar==(int)'Q') break;
			if (inchar==(int)'N') continue;
		}
		do_cmp_table(dbname,func.table,tbcount);
	}
	if(pt) free(pt);

	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}
/* 复制表或库 */
int CDbOperate::do_copy()
{
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,firstserverno,secondserverno;
	char tablename[100];
	char dbname[100];
    int         inchar;
	time_t		nowtime;
	struct tm	*ptm;

	if(get_tablename(tablename)==1) oneallflag=1;
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
	    fprintf(stderr,"\n命令格式不对! 未提供表名或库名。请用 dbmt copy help 帮助。\n");
	    return 0;
	}

	serverno=get_serverno();
	if(serverno<12||serverno>43)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供源服务器名或服务器不存在。请用 dbmt copy help 帮助。\n");
	    return 0;
	}else
	{
		firstserverno = (int)(serverno/10);
		secondserverno = (int)(serverno%10);
	}

	if(db_start(firstserverno)!=CS_SUCCEED) return 0;
		/* 检查系统管理员口令 */	
	int dbflag=0;
	retcode=check_root_password(firstserverno);
	if(retcode!=CS_SUCCEED)
	{
			dbsingle.Close();
			return 0;
	}
	dbsingle.Close();

	if(oneallflag==1) fprintf(stderr,"\n复制表 %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n复制库 %s :\n",dbname);

	
	if(bPrompt==1) 
	{
		if (serverno==1)
			sprintf(strstdin,"\n是否用服务器1(nbdb) 复制到 服务器2(nbdbb) (Y/N)?");
		else if (serverno==2)
			sprintf(strstdin,"\n是否用服务器2(nbdbb) 复制到 服务器1(nbdb) (Y/N)?");
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if (serverno==1)
			sprintf(strstdin,"\n确认用服务器1(nbdb) 复制到 服务器2(nbdbb) (Y/N)?");
		else if (serverno==2)
			sprintf(strstdin,"\n确认用服务器2(nbdbb) 复制到 服务器1(nbdb) (Y/N)?");
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}
	time(&nowtime);
	ptm=localtime(&nowtime);
    fprintf(stderr,"\n\nBegin time:  %04d-%02d-%02d  %02d:%02d:%02d\n",
		ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
	
	if (oneallflag==1)
    {
        strcpy(dbname,"modeldb");
		fprintf(stderr,"\n正在修补表 [ %s ] ...",tablename);
		realize_correct_onetable(firstserverno,secondserverno,tablename,dbname);
    }
	else if(oneallflag==2)
    {
        fprintf(stderr,"\n正在修补库 [ %s ] ...",dbname);
		realize_correct_table(firstserverno,secondserverno,dbname);
    }
	
	time(&nowtime);
	ptm=localtime(&nowtime);
    fprintf(stderr,"\nEnd time:  %04d-%02d-%02d  %02d:%02d:%02d\n",
		ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
	return 1;
}

/* 删除历史数据 
   dbmt del -Sn -B起始时间 -E终止时间 [-Dn -NQ -USR用户 -PWD口令]
   -Sn			: -S1或-S2 
   -B起始时间	: 格式为年月，如2002年1月 -B200201
   -E终止时间	:
             以下为可选项
   -Dn			: 可选，删除的数据种类，不带此选项，则给出选择提示
   -NQ			: 直接执行，不输入口令和确认删除
   -USR用户		: 无此选项则以modeldb用户登录 
   -PWD口令		:
*/
int CDbOperate::do_delete()
{  
	int i,k,inchar;
	int deltype=-1;
	int err;
	int startdate=0,enddate=0;//起始时间，终止时间
	int syear,smon,eyear,emon;//起始年，起始月，终止年，终止月
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode;
	int serverno=-1;
	char *pt;
	int recnum,ret;
	time_t time1,time2;
	struct tm tmtime1,tmtime2;
	CS_DATAFMT *datafmt;

	serverno=get_serverno();
	if(serverno<1 ||serverno>4 )
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名或服务器不存在。请用 dbmt del help 帮助。\n");
	    return 0;
	}

	for(i=1;i<argc;i++)		// -DTn n为要删除的数据种类
		if(strncmp(argv[i],"-DT",3)==0)
		{
			if(argv[i][3]!=0)
			{
				if(sscanf(argv[i]+3,"%d",&deltype)<=0) deltype=-1;
				break;
			}
		}
	if(deltype<1 || deltype>HISDATANUM) deltype=-1;

	err=0;
	for (i=1;i<argc;i++)
		if (strncmp(argv[i],"-B",2)==0)
		{
			if(argv[i][2]!=0)
			{
				if(sscanf(argv[i]+2,"%d",&startdate)<=0) err=2;
				break;
			}
		}
	if(i>=argc) err=1;
	if(err==0)
	{
		syear=startdate/100;
		smon=startdate%100;
		if(syear<1998 || syear>2999 || smon<1 || smon>12 ) err=2;
	}
	if(err>0)
	{
		if(err==1) fprintf(stderr,"未提供起始时间! ");
		else fprintf(stderr,"-B提供的起始时间错误! ");
		fprintf(stderr,"请用 dbmt del help 帮助\n");
		return 0;
	}

	err=0;
	for (i=1;i<argc;i++)
		if (strncmp(argv[i],"-E",2)==0)
		{
			if(argv[i][2]!=0)
			{
				if(sscanf(argv[i]+2,"%d",&enddate)<=0) err=2;
				break;
			}
		}
	if(i>=argc) err=1;
	if(err==0)
	{
		eyear=enddate/100;
		emon=enddate%100;
		if(eyear<1998 || eyear>2999 || emon<1 || emon>12 ) err=2;
		else if(eyear<syear || (eyear==syear && emon<smon) ) err=3;
	}
	if(err>0)
	{
		if(err==1) fprintf(stderr,"未提供终止时间! ");
		else if(err==2) fprintf(stderr,"-E提供的终止时间错误! ");
		else if(err==3) fprintf(stderr,"-E提供的终止时间小于起始时间! ");
		fprintf(stderr,"请用 dbmt del help 帮助\n");
		return 0;
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* 检查系统管理员口令 */	
	int dbflag=0;

	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	LOG_INFO *loginfo=NULL;
	int srvnum=0;

	fprintf(stderr,"\n");
	for(i=0;i<HISDATANUM;i++)
		fprintf(stderr,"    %d、%s\n",i+1,datatype[i]);
	fprintf(stderr,"    0、退出\n");

	int dataflag;
	if(deltype==-1)
	{
		do
		{
			fprintf(stderr,"请选择要删除的历史数据类型：[");
			for(i=1;i<=HISDATANUM;i++) fprintf(stderr,"%d,",i);
			fprintf(stderr,"0] : ");
			gets(strstdin);
			inchar=strstdin[0];
		}while(inchar<'0' || inchar>('0'+HISDATANUM) );
		
		if(inchar=='0') return 1;

		dataflag=inchar-'0';
	}
	else dataflag=deltype;
	if (bPrompt==1)
	{
		sprintf(strstdin,"\n是否删除 %04d年%02d月 -- %04d年%02d月 之间的%s [Y/N]: ",
				syear,smon,eyear,emon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin,0);
		if(inchar=='N') return 1;

		sprintf(strstdin,"\n确认删除 %04d年%02d月 -- %04d年%02d月 之间的%s [Y/N]: ",
				syear,smon,eyear,emon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin,0);
		if(inchar=='N') return 1;
	}


	int datayear,datamon,dataday;
	int monnum=(eyear-syear)*12+(emon-smon)+1;
	char *pcmd;
	char tmpstr[255],tmpstr1[50];

	pcmd=(char *)malloc(EX_BUFSIZE*2);
	if(pcmd==NULL) 
	{
		fprintf(stderr,"malloc() failed!");
		dbsingle.Close();
		return 0;
	}
	memset(pcmd,0,EX_BUFSIZE*2);
	
	memset(&tmtime1,0,sizeof(tmtime1));
	dataday=0;
	tmpstr1[0]=0;
	if(dataflag==1 || dataflag==2)	/* 删除存盘数据 */
	{
if(dbtype==DBTYPE_DB2)
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			pcmd[0]=0;

			tmtime1.tm_isdst=-1;
			tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=8;
			tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
			time1=mktime(&tmtime1);
			dataday=time1/(24*3600);

			for(k=0;k<7;k++)
			{
				sprintf(tmpstr1,"%s where date>=%d and date<=%d",tmpstr,dataday+k*5,dataday+(k+1)*5);
				sprintf(func.isql,"delete from yc%s;delete from kwh%s;\
					delete from dayyc%s;delete from daykwh%s;",
					tmpstr1,tmpstr1,tmpstr1,tmpstr1);
				strcat(pcmd,func.isql);

			}
			sprintf(func.isql,"delete from yc%s;delete from kwh%s;\
				delete from dayyc%s;delete from daykwh%s;",
				tmpstr,tmpstr,tmpstr,tmpstr);
			strcat(pcmd,func.isql);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"日数据正确 ");
			else fprintf(stderr,"日数据错误 ");

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from monyc%04d where date=%d;\
				delete from monkwh%04d where date=%d",
				datayear,(datayear-1970)*12+datamon,
				datayear,(datayear-1970)*12+datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"月数据正确 ");
			else fprintf(stderr,"月数据无表或删除错误 ");
			
			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from 设备运行统计表 where 存盘日期>=%04d%02d01 and 存盘日期<=%04d%02d31;",datayear,datamon,datayear,datamon);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"设备运行统计表 正确 ");
			else fprintf(stderr," 设备运行统计表 错误 ");
		}
}
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"truncate table yc%s;truncate table kwh%s;\
				truncate table dayyc%s;truncate table daykwh%s;",
				tmpstr,tmpstr,tmpstr,tmpstr);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"日数据正确 ");
			else fprintf(stderr,"日数据错误 ");

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from monyc%04d where date=%d;\
				delete from monkwh%04d where date=%d",
				datayear,(datayear-1970)*12+datamon,
				datayear,(datayear-1970)*12+datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"月数据正确 ");
			else fprintf(stderr,"月数据无表或删除错误 ");
			
			func.func=ISQL_COMMAND;
			sprintf(pcmd,"delete from 设备运行统计表 where 存盘日期>=%04d%02d01 and 存盘日期<=%04d%02d31",
				datayear,datamon,datayear,datamon);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"设备运行统计表 正确 ");
			else fprintf(stderr," 设备运行统计表 错误 ");
		}			
}
else // SYBASE_DB MSSQL_DB
{		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n if exists(select name from sysobjects where name='yc%s') truncate table yc%s\
				\r\ngo\r\n if exists(select name from sysobjects where name='kwh%s') truncate table kwh%s\
				\r\ngo\r\n if exists(select name from sysobjects where name='dayyc%s') truncate table dayyc%s\
				\r\ngo\r\n if exists(select name from sysobjects where name='daykwh%s') truncate table daykwh%s",
				tmpstr,tmpstr,tmpstr,tmpstr,tmpstr,tmpstr,tmpstr,tmpstr);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"日数据正确 ");
			else fprintf(stderr,"日数据错误 ");

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n if exists(select name from sysobjects where name='monthyc%04d')\
				delete from monthyc%04d where sdate=%d \
				\r\ngo\r\n dump tran hisdb with truncate_only\
				\r\ngo\r\n if exists(select name from sysobjects where name='monthkwh%04d')\
				delete from monthkwh%04d where sdate=%d \
				\r\ngo\r\n dump tran hisdb with truncate_only",
				datayear,datayear,(datayear-1970)*12+datamon,
				datayear,datayear,(datayear-1970)*12+datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"月数据正确 ");
			else fprintf(stderr,"月数据无表或删除错误 ");
			

			func.func=ISQL_COMMAND;
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n delete from 设备运行统计表 where 存盘日期>=%04d%02d01 and 存盘日期<=%04d%02d31\
				\r\ngo\r\n dump transaction hisdb with truncate_only\r\ngo\r\n ",
				datayear,datamon,datayear,datamon);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"设备运行统计表 正确 ");
			else fprintf(stderr," 设备运行统计表 错误 ");
		}
}
	}
	
	if(dataflag==1 || dataflag==3)	/* 删除事项和操作记录 */
	{
		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			//func.serverno=serverno;
			pcmd[0]=0;
if ( (dbtype==DBTYPE_SYBASE) || (dbtype==DBTYPE_SQLSERVER) )
			strcpy(pcmd,"use hisdb\r\ngo\r\n");

			
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
{			sprintf(func.isql,"truncate table 电力历史事项表%04d%02d;",
					datayear,datamon);
				strcat(pcmd,func.isql);
}
else // SYBASE_DB MSSQL_DB
{				sprintf(func.isql,"truncate table 电力历史事项表%04d%02d\r\ngo\r\n",
					datayear,datamon);
				strcat(pcmd,func.isql);
				
}			

if ( (dbtype==DBTYPE_SYBASE) || (dbtype==DBTYPE_SQLSERVER) )
			strcat(pcmd,"dump tran hisdb with truncate_only\r\ngo\r\n");

			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 电力历史事项... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"正确 ");
			else fprintf(stderr,"错误 ");

			func.func=ISQL_COMMAND;
			pcmd[0]=0;
if ( (dbtype==DBTYPE_SYBASE) || (dbtype==DBTYPE_SQLSERVER) )
			strcpy(pcmd,"use hisdb\r\ngo\r\n");
			
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
{sprintf(func.isql,"delete from 操作历史事项表%04d%02d;",
					datayear,datamon);
strcat(pcmd,func.isql);}
else // SYBASE_DB MSSQL_DB
{				sprintf(func.isql,"truncate table 操作历史事项表%04d%02d\r\ngo\r\n",
					datayear,datamon);
				strcat(pcmd,func.isql);
				}
			
if ( (dbtype==DBTYPE_SYBASE) ||(dbtype==DBTYPE_SQLSERVER) )
			strcat(pcmd,"dump tran hisdb with truncate_only\r\ngo\r\n");

			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 操作记录... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"正确 ");
			else fprintf(stderr,"错误 ");
		}
	}

	memset(&tmtime1,0,sizeof(tmtime1));
	memset(&tmtime2,0,sizeof(tmtime2));
	if(dataflag==1 || dataflag==4)	/* 删除追忆数据 */
	{
		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 追忆数据... ",
				serverno,datayear,datamon);

			func.func=SEL_ISQL_RESULTS;
			func.recorderlen=0;
			sprintf(func.isql,"select 时标 from hisdb.dbo.pdrdescribe where 年月日>=%04d%02d%02d and 年月日<=%04d%02d%02d",
				datayear,datamon,1,datayear,datamon,31);
			
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
			if(retcode!=CS_SUCCEED) 
			{
				fprintf(stderr,"错误");
				continue;
			}
		
			recnum=func.ret_roxnum;
			int *pint;
			ret=CS_SUCCEED;
			for(k=0;k<recnum;k++)
			{
				func.func=ISQL_COMMAND;
				pcmd[0]=0;
				pint=(int*)(pt+func.recorderlen*k);
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
				sprintf(func.isql,"delete from pdrdata where 时标=%d;\
					delete from pdrdescribe where 时标=%d;",
					*pint,*pint);
else //SYBASE_DB MSSQL_DB
{				strcpy(pcmd,"use hisdb\r\ngo\r\n");
				sprintf(func.isql,"delete from pdrdata where 时标=%d\r\ngo\r\n\
					delete from pdrdescribe where 时标=%d\r\ngo\r\n",
					*pint,*pint);
				strcat(func.isql,"dump tran hisdb with truncate_only\r\n");
}
				strcat(pcmd,func.isql);
				retcode=dbsingle.IsqlCommand(&func,pcmd);
				if(retcode!=CS_SUCCEED) ret=CS_FAIL;
			}
			if (datafmt) free(datafmt);
			if(pt) free(pt);
	
			tmtime1.tm_isdst=-1; tmtime2.tm_isdst=-1;
			tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=0;
			tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
			tmtime2.tm_sec=0; tmtime2.tm_min=0; tmtime2.tm_hour=0;
			tmtime2.tm_mday=1;
			if(datamon<12) { tmtime2.tm_mon=datamon; tmtime2.tm_year=datayear-1900; }
			else { tmtime2.tm_mon=0; tmtime2.tm_year=datayear+1-1900; }
			time1=mktime(&tmtime1);
			time2=mktime(&tmtime2);
			func.func=ISQL_COMMAND;
			pcmd[0]=0;
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE)  || (dbtype==DBTYPE_DMSQL ) )
			sprintf(func.isql,"delete from pdrdata where 时标>=%d and 时标<=%d;",
				time1,time2);
else // SYBASE_DB MSSQL_DB
{			strcpy(pcmd,"use hisdb\r\ngo\r\n");
			sprintf(func.isql,"delete from pdrdata where 时标>=%d and 时标<=%d\r\ngo\r\n",
				time1,time2);
			strcat(func.isql,"dump tran hisdb with truncate_only\r\n");
}
			strcat(pcmd,func.isql);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode!=CS_SUCCEED) ret=CS_FAIL;
			if(ret==CS_SUCCEED) fprintf(stderr,"正确 ");
			else fprintf(stderr,"错误 ");
		}
	}

	memset(&tmtime1,0,sizeof(tmtime1));
	dataday=0;
	tmpstr1[0]=0;
	if(dataflag==1 || dataflag==5)	/* 删除秒级曲线数据 */
	{
if (dbtype==DBTYPE_DB2)
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			pcmd[0]=0;

			tmtime1.tm_isdst=-1;
			tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=8;
			tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
			time1=mktime(&tmtime1);
			dataday=time1/(24*3600);

			for(k=0;k<7;k++)
			{
				sprintf(tmpstr1,"%s where sdate>=%d and sdate<=%d",tmpstr,dataday+k*5,dataday+(k+1)*5);
				sprintf(func.isql,"delete from sdata%s;",tmpstr1);
				strcat(pcmd,func.isql);

			}
			sprintf(func.isql,"delete from sdata%s;",tmpstr);
			strcat(pcmd,func.isql);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"秒级曲线数据正确 ");
			else fprintf(stderr,"秒级曲线数据错误 ");
		}
}
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
{	for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"truncate table sdata%s;",tmpstr);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"秒级曲线数据正确 ");
			else fprintf(stderr,"秒级曲线数据错误 ");
		}			
}
else // SYBASE_DB MSSQL_DB
{		for(i=0;i<monnum;i++)
		{
			datamon=(smon-1+i)%12+1;
			datayear=syear+(smon-1+i)/12;
			func.func=ISQL_COMMAND;
			sprintf(tmpstr,"%04d%02d",datayear,datamon);
			sprintf(pcmd,"use hisdb\
				\r\ngo\r\n if exists(select name from sysobjects where name='sdata%s') truncate table sdata%s",
				tmpstr,tmpstr);
			fprintf(stderr,"\n\n删除服务器(%d)  %04d年%02d月 存盘数据... ",
				serverno,datayear,datamon);
			retcode=dbsingle.IsqlCommand(&func,pcmd);
			if(retcode==CS_SUCCEED) fprintf(stderr,"秒级曲线数据正确 ");
			else fprintf(stderr,"秒级曲线数据错误 ");
		}
}
	}

	free(pcmd);
	dbsingle.Close();
	return 1;
}

/*  文件->表或库
参数表的文件名为其表名
历史数据以月为单位，名字为 表名yyyymm ,
	如事项：电力历史事项表200201、monthyc200201、
	但表名包含年月的除外，如 yc200201,dayyc200201
历史数据：
    dbmt in -Dhisdb -Sn -YM年月 [-debug]
		-HD     : 历史数据
		-Sn		: -S1 或-S2
		-YM		: 年月，yyyymm 如2002年1月为 200201 
参数表：
	dbmt in  <-D库名 | -T表名> [-P] [-debug]
		-Sn		: -S1(nbdb) 或-S2(scadbb)
		-D库名  : 库中所有表 modeldb,hisdb
		-T		: 某个表 库名.dbo.表名
		-P		: 同-D配合使用，对每个表提示是否输出。
*/
int CDbOperate::do_in_file()
{   
	char dbname[100];
	if (get_dbname(dbname)==1)
	{
		if (strcmp(dbname,"hisdb")==0)
			return do_in_hisdata();	/* 历史数据 */
		else if(strcmp(dbname,"modeldb")==0)
		{
			return do_in_para();		/* 参数 */
		}
	}
	
	fprintf(stderr,"\n命令格式不对!  未提供库名 。请用 dbmt in help 帮助。\n");
	return 0;
}

int CDbOperate::realize_in_file(FUNC_STR *func,char *filename,int null_num,int id)
{
	CS_RETCODE retcode;
	int i,numrows;
	void *p6;
	FILE *stream=NULL;
	if(id>=0) fprintf(stderr,"\n%d 正在装入记录到表[%s] ...",id,func->table);
	else fprintf(stderr,"\n 正在装入记录到表[%s] ...",func->table);

	if (func->his.flag==2)
	{
		func->func=ISQL_COMMAND;
		sprintf(func->isql,"truncate table %s",func->table);
		retcode=dbsingle.IsqlCommand(func,func->isql);
		if (retcode==CS_SUCCEED) fprintf(stderr,"\n		原记录清除成功");
		else {
			fprintf(stderr,"\n		原记录清除失败");
			//return retcode;
		}
	}	
	CS_DATAFMT *datafmt;
	func->func=GET_TABLE_INFO;
	retcode = dbsingle.GetTableInfo(func,&datafmt);
	if (retcode!=CS_SUCCEED)
	{

		if (datafmt) free (datafmt);

		fprintf (stderr,"\n %s 打开失败!",func->table);
		return CS_FAIL;
	}
	
	CS_DATAFMT** fmt = &datafmt;
	void**  ptt = (void**)fmt;
	stream=fopen(filename,"rt");
	if(stream==NULL)
	{

		if (datafmt) free(datafmt);

		fprintf(stderr,"\n无文件[%s] 或打开文件错误!",filename);
		return CS_FAIL;
	}

	fseek(stream,0,SEEK_SET);//定位在文件头
	
	numrows=1;
	char * pstr=(char*)malloc(EX_BUFSIZE*2+10);

	fgets(pstr,EX_BUFSIZE*2,stream);//若第一行为表结构，过滤第一行
	if(strncmp(pstr,"--DATAFMT: ",11)!=0)
	fseek(stream,0,SEEK_SET);//定位在文件头

	char *pcur;
	VAL_UNION *value;
	int tmpint,j;
	int shortvalue =0;
	int recnum=0;
	value=(VAL_UNION *) malloc (sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); //为了防止内存不足最大只能为5000*func->colnum

	while(1)
	{
		memset(value,0,sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); 
		for(i=0;i<MAXINROWNUM;i++)
		{
			if (fgets(pstr,EX_BUFSIZE*2,stream)==NULL)
				break;
			else 
				pcur=pstr;

			for(j=0;j<func->colnum;j++)
			{
				if(j>0)
				{
					if( (pcur=strchr(pcur,'\t'))==NULL )	/* 每个域用TAB键隔开 */
					{
						retcode=CS_FAIL;
						break;
					}
					pcur+=1;
				}
				tmpint=i*func->colnum+j;
				switch(datafmt[j].datatype)
				{
					case CS_SMALLINT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->smallvalue=0;
						else
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->smallvalue=shortvalue;
						break;
					case CS_TINYINT_TYPE:
					case CS_BIT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->tinyvalue=0;
						else	
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else 
								(value+tmpint)->tinyvalue=(unsigned char)shortvalue;
						break;
					case CS_USHORT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->usmallvalue=0;
						else
							if(sscanf(pcur,"%d",&value)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->usmallvalue=shortvalue;
						break;
					case CS_INT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->intvalue=0;
						else
							if(sscanf(pcur,"%d",&((value+tmpint)->intvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_CHAR_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') strcpy((value+tmpint)->charvalue," ");
						else 
							if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->charvalue[datafmt[j].maxlength-1]=0;//2006.2.10 解决老现场数据导入首条记录描述乱码问题
						break;
					case CS_REAL_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->realvalue=0;
						else
							if(sscanf(pcur,"%g",&((value+tmpint)->realvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_FLOAT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->floatvalue=0;
						else
						if(sscanf(pcur,"%lg",&((value+tmpint)->floatvalue))<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					case CS_DATETIME_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') 
						{
							retcode=CS_FAIL;
							break;
						}
						if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					default:
						break;
				}//switch
			}//for (j)
		}//for (i)
		p6=value;
		fprintf(stderr,"-");
		if(i==0) break;
		retcode=dbsingle.DbTools(DB_IN_FILE,(void*)func->dbname,(void*)func->table,(void*)(&func->colnum),ptt,&i,&p6);
		if(retcode==CS_SUCCEED)			recnum+=i;

		if(i<MAXINROWNUM) break;
	}
//	}//while

	//for(j=0;j<null_num;j++) putchar(' ');
	putchar('\t');
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"	失败!  ");
		fprintf(stderr,"插入数据错误!",func->table);
		if (pstr) free(pstr);

		if (datafmt) free(datafmt);

		if (p6) free(p6);
		fclose(stream);
		return 0;
	}
	else 
	{
		fprintf(stderr,"	成功!  ");
		fprintf(stderr,"插入%d条数据!",recnum);
		if (pstr) free(pstr);

		if (datafmt) free(datafmt);

		if (p6) free(p6);
		fclose(stream);
		return 1;
	}
}

int CDbOperate::do_in_para()		/* 参数 */
{   
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,i;
	CS_DATAFMT *datafmt;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));

	serverno=get_serverno();
	if(serverno<1||serverno>4)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名或服务器不存在。请用 dbmt in help 帮助。\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (get_dbname(dbname)!=1)
		{
			fprintf(stderr,"\n   未提供库名，请用dbmt in help 帮助!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	

	if(strcmp(mode,"a")==0)
		func.his.flag=0;	/* 追加记录，不考虑表中是否有记录 默认*/
		else
        if(strcmp(mode,"b")==0)
        func.his.flag=2;	/* 先清空 */
        else
        if(strcmp(mode,"c")==0)
        func.his.flag=3;	/*先执行isql */
	/*if(oneallflag==1)
	{
		dbname[0]=0;
		char *pstr=strchr(tablename,'.');
		if(pstr!=NULL) strncpy(dbname,tablename,pstr-tablename);
	}*/

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* 检查系统管理员口令 */	
	int dbflag=0;
	retcode=check_root_password(serverno);

	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		fprintf(stderr,"\n 数据库打开失败!");
		return 0;
	}

	if(oneallflag==1) fprintf(stderr,"\n装入记录到表 %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n装入记录到库 %s :\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n是否装入记录到表 %s  (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n是否装入记录到库 %s  (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }

		if(oneallflag==1)
			sprintf(strstdin,"\n确认装入记录到表 %s  (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n确认装入记录到库 %s  (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}

	char filename[255];
	char *pstr;

	strcpy(func.dbname,dbname);
	if(oneallflag==1)//一个表
	{
		pstr=strrchr(tablename,'.');
		if(pstr==NULL) strcpy(filename,tablename);
		else strcpy(filename,pstr+1);
		strcpy(func.table,tablename);
		if(strcmp(mode,"a")==0)
		func.his.flag=0;	/* 追加记录，不考虑表中是否有记录 */
		else
        if(strcmp(mode,"b")==0)
        func.his.flag=2;	/* 先清空 */
        else
        if(strcmp(mode,"c")==0)
        func.his.flag=3;	/*先执行isql */

		retcode=realize_in_file(&func,filename);
		fprintf(stderr,"\n");
		return retcode;
	}
	else
	{
if ( (dbtype==DBTYPE_DB2) ||(dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) ) //modify by chenkai 增加mysql
		if(pHistableName==NULL) load_histable_name();
// 如果参数没有 [-A]
if (!bAllTables)
		load_unchangedtable_name();

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
else if (dbtype==DBTYPE_ORACLE || dbtype == DBTYPE_DMSQL )
		sprintf(func.isql,"select table_name from user_tables");
else if (dbtype == DBTYPE_MYSQL)
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);

		char *pt=NULL;
		int tbnum;
		int reclen;
		func.func=SEL_ISQL_RESULTS;
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n 从库中取表名失败!\n");

			if (datafmt) free (datafmt);
			if (pt) free (pt);

			return retcode;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;

		tbcount=0;

		char *tabName;//记录表名
		tabName = (char*) malloc(tbnum * reclen);
		memcpy(tabName,pt,tbnum*reclen);

		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(tabName+i*reclen)==1)
			{	//临时表，将表名置为空仅oracle有
				*(tabName+i*reclen)=0;
				continue;
			}
			if (!bAllTables)
			{
				if (check_unchangedtable_name(tabName+i*reclen)==1)
				{
					*(tabName+i*reclen)=0;
					continue;
				}
			}
			if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL )  ) //modify by chenkai 增加mysql
			{			
				retcode=check_histable_name(tabName+i*reclen);
				// 参数库：只处理参数表; 历史库：只处理历史表
				if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
				{
					*(tabName+i*reclen)=0;
					continue;
				}
			}
			tbcount++;
		}
		fprintf(stderr,"\n 已从库中取出表名: %d 个表。\n",tbcount);
		tbcount=0;
		int maxnamelen=0;
		int tmplen;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(tabName+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,tabName+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;
			strcpy(filename,func.table);
			sprintf(func.table,"%s",filename);
			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: 是否装入记录到表 [ %s ]  : 是/否/退出 (Y/N/Q)? ",tbcount,func.table);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') continue;
			}

			tmplen=maxnamelen-strlen(func.table);
			realize_in_file(&func,filename,tmplen,tbcount);
		}

		if (!bAllTables)
		{
			fprintf(stderr,"\n \n  下列%d个表未导入数据库，如需导入请加参数[-A]，详见dbmt in help: \n",UnchangedTableNameCount);

			char tmpname[50];

			for(int i=0;i<UnchangedTableNameCount;i++)
			{
				strcpy(tmpname,pUnchangedTableName+i*UnchangedTableNameLen);
				fprintf(stderr,"  %d  %s \n",i+1,tmpname);
			}

		}

		if(pt) free(pt);

	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_in_hisdata()	/* 历史数据 */
{
	int serverno=-1;
	int err=0,deltype=-1;
	int dataYM=-1,datayear,datamon;
	int i,retcode,inchar;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));

	char filename[255];
    if(strcmp(mode,"a")==0)
		func.his.flag=0;	/* 追加记录，不考虑表中是否有记录 */
		else
        if(strcmp(mode,"b")==0)
        func.his.flag=2;	/* 先清空 */
        else
        if(strcmp(mode,"c")==0)
        func.his.flag=3;	/*先执行isql */
	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名。请用 dbmt in help 帮助。\n");
	    return 0;
	}

	for(i=1;i<argc;i++)		// -DTn n为要装入的数据类型
		if(strncmp(argv[i],"-DT",3)==0)
		{
			if(argv[i][3]!=0)
			{
				if(sscanf(argv[i]+3,"%d",&deltype)<=0) deltype=-1;
				break;
			}
		}
	if(deltype<1 || deltype>HISDATANUM) deltype=-1;

	for(i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-YM",3)==0)
				dataYM=atoi(argv[i]+3);
	}

	if(dataYM<=0)
	{
		fprintf(stderr,"\n命令格式不对，未提供日期。请用 dbmt in help 帮助。\n");
		return 0;
	}
	datayear=dataYM/100;
	datamon=dataYM%100;
	if(datayear<1998 || datayear>2999 || datamon<1 || datamon>12)
	{
		fprintf(stderr,"\n提供的日期不对。请用 dbmt in help 帮助。\n");
		return 0;
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;

		/* 检查系统管理员口令 */	
	int dbflag=0;

	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	fprintf(stderr,"\n在装入历史数据之前，确保过去畚份的文件在当前(或指定)目录，");
	fprintf(stderr,"\n并且数据库中没有%d年%d月的数据。",datayear,datamon);
	fprintf(stderr,"\n\n---------------------------\n");
	for(i=0;i<HISDATANUM;i++)
		fprintf(stderr,"    %d、%s\n",i+1,datatype[i]);
	fprintf(stderr,"    0、退出\n");

	int dataflag;
	if(deltype==-1)
	{
		do
		{
			fprintf(stderr,"\n请选择要装入的历史数据类型：[");
			for(i=1;i<=HISDATANUM;i++) fprintf(stderr,"%d,",i);
			fprintf(stderr,"0] : ");
			gets(strstdin);
			inchar=strstdin[0];
		}
		while(inchar<'0' || inchar>('0'+HISDATANUM) );
		
		if(inchar=='0') return 1;

		dataflag=inchar-'0';
	}
	else dataflag=deltype;
	if (bPrompt==1)
	{
		sprintf(strstdin,"\n是否装入 %04d年%02d月 的%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
		sprintf(strstdin,"\n确认装入 %04d年%02d月 的%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
	}
	if (dbtype==DBTYPE_ORACLE || dbtype==DBTYPE_DMSQL )
		strcpy(func.dbname,"modeldb");
	else
		strcpy(func.dbname,"hisdb");
	if(dataflag==1 || dataflag==2)	//存盘数据、统计结果
	{
		// yc199901 -> file yc199901
		sprintf(func.table,"yc%04d%02d",datayear,datamon);
		sprintf(filename,"yc%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// kwh199901 -> file kwh199901
		sprintf(func.table,"kwh%04d%02d",datayear,datamon);
		sprintf(filename,"kwh%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// dayyc199901 -> dayyc199901
		sprintf(func.table,"dayyc%04d%02d",datayear,datamon);
		sprintf(filename,"dayyc%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// daykwh199901 ->daykwh199901
		sprintf(func.table,"daykwh%04d%02d",datayear,datamon);
		sprintf(filename,"daykwh%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// monthyc1999 -> file monthyc199901,monthyc199902...
		sprintf(func.table,"monthyc%04d",datayear);
		sprintf(filename,"monthyc%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// monthkwh1999 -> file monthkwh199901,monthkwh199902...
		sprintf(func.table,"monthkwh%04d",datayear);
		sprintf(filename,"monthkwh%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
			
		// 设备运行统计表 -> file 设备运行统计表199901,...
		sprintf(func.table,"设备运行统计表");
		sprintf(filename,"设备运行统计表%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);

		
	}

	if(dataflag==1 || dataflag==3)	//事项和操作记录
	{
		// 电力历史事项表199901 -> file 电力历史事项表199901,...
		sprintf(func.table,"电力历史事项表%04d%02d",datayear,datamon);
		sprintf(filename,"电力历史事项表%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);

		// 操作历史事项表199901 -> file 操作历史事项表199901,...
		sprintf(func.table,"操作历史事项表%04d%02d",datayear,datamon);
		sprintf(filename,"操作历史事项表%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
	}
	if(dataflag==1 || dataflag==4)	//追忆数据
	{

		// 追忆描述表 -> file 追忆描述表199901,...
		sprintf(func.table,"pdrdescribe");
		sprintf(filename,"pdrdescribe%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		
		// 追忆数据表 -> file 追忆数据表199901,...
		sprintf(func.table,"pdrdata");
		sprintf(filename,"pdrdata%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
	}

	if(dataflag==1 || dataflag==5)	//秒级曲线数据
	{
		// sdata199901 -> file sdata199901
		sprintf(func.table,"sdata%04d%02d",datayear,datamon);
		sprintf(filename,"sdata%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);

		//专为甘肃水调设计 begin
		sprintf(func.table,"开关变位统计表%04d%02d",datayear,datamon);
		sprintf(filename,"开关变位统计表%04d%02d",datayear,datamon);
		realize_in_file(&func,filename);
		//专为甘肃水调设计 end
	}

	fprintf(stderr,"\n");
	return 1;
}

/*  表或库->文件
参数表的文件名为其表名
历史数据以月为单位，名字为 表名yyyymm ,
	如事项：电力历史事项表200201、monyc200201, 
	但表名包含年月的除外，如 yc200201,dayyc200201
历史数据：
    dbmt out  -Sn -YM年月 [-debug]
		-Sn		: -S1 或-S2
		-YM		: 年月，yyyymm 如2002年1月为 200201 
参数表：
	dbmt out  <-D库名 | -T表名> [-P] [-debug]
		-Sn		: -S1(sybase11) 或-S2(sybase22)
		-D库名  : 库中所有表 modeldb,hisdb
		-T		: 某个表 库名.dbo.表名
		-P		: 同-D配合使用，对每个表提示是否输出。
*/
int CDbOperate::do_out_file()
{
	char dbname[100];
	if (get_dbname(dbname)==1)
	{
		if (strcmp(dbname,"hisdb")==0)
		{
			int ret = do_out_hisdata();	/* 历史数据 */
			return ret;
		}
		else if(strcmp(dbname,"modeldb")==0)
		{
			int ret = do_out_para();		/* 参数 */
			return ret;
		}
	}
	fprintf(stderr,"\n命令格式不对! 未提供库名 。请用 dbmt out help 帮助。\n");
	return 0;
}

int CDbOperate::realize_out_file(FUNC_STR *func,char *filename,int null_num,int id)
{
	CS_RETCODE retcode;
	CS_DATAFMT *datafmt=0;
	int i;
	FILE *stream=NULL;

	if(id>=0) fprintf(stderr,"\n%d 正在输出表[%s] ...",id,func->table);
	else fprintf(stderr,"\n 正在输出表[%s] ...",func->table);
	
	func->func=GET_TABLE_INFO;
	retcode = dbsingle.GetTableInfo(func,&datafmt);

	if (retcode!=CS_SUCCEED)
	{

		if (datafmt) free (datafmt);

		fprintf (stderr,"\n %s 打开失败!",func->table);
		return CS_FAIL;
	}

	stream=fopen(filename,"wt");
	if(stream==NULL)
	{
		

		free(datafmt);

		return CS_FAIL;
	}

	/* 畚份表结构 */
	fputs("--DATAFMT: ",stream);
	for(i=0;i<func->colnum;i++)
	{
		func->col[i].condition = NO;
		func->col[i].select = YES;
		func->col[i].order_d = i;
		func->col[i].order = -1;
		fputs(datafmt[i].name,stream);
		fputs("\t",stream);
	}
	fputs("\n",stream);

	/////////////add begin 20150310

	tmpfunc=*func;
	tmpdatafmt=datafmt;
	tmpstream=stream;
	recordnum=0;
	void *client=0;
	retcode=dbsingle.SetBlkOutProcFunc(proc,client,MAXOUTROWNUM);//设置回调函数及步长

	func->func = BLK_OUT;
	void* pt;
	retcode = dbsingle.BlkCopyOut(func,datafmt,&pt);

	retcode=dbsingle.SetBlkOutProcFunc(0,client,MAXOUTROWNUM);//取消回调函数

	/////////////add  end 20150310


	//for(i=0;i<null_num;i++) putchar(' ');
	putchar('\t');
	if(recordnum<0)
	{
		fprintf(stderr,"失败!  ");
	}
	else 
	{
		fprintf(stderr,"成功 导出 %d 条记录. ",recordnum);

	}

	fclose(stream);

	if (datafmt) free(datafmt);

	return 1;
}

int CDbOperate::do_out_para()
{   
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,i;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;
	CS_DATAFMT *datafmt;

	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名。请用 dbmt out help 帮助。\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   未提供库名，请用dbmt out help 帮助!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
		fprintf(stderr,"\n命令格式不对! 未提供表名或库名。请用 dbmt out help 帮助。\n");
		return 0;
	}

	
	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* 检查系统管理员口令 */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}
	
	if(oneallflag==1) fprintf(stderr,"\n输出表 %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n输出库 %s :\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n是否输出表 %s 到文件 (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n是否输出库 %s 中的所有表到文件 (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if(oneallflag==1)
			sprintf(strstdin,"\n确认输出表 %s 到文件 (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n确认输出库 %s 中的所有表到文件 (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}

	char filename[255];
	char *pstr;

	if(oneallflag==1)
	{
		pstr=strrchr(tablename,'.');
		if(pstr==NULL) strcpy(filename,tablename);
		else strcpy(filename,pstr+1);

		strcpy(func.table,tablename);
		strcpy(func.dbname,dbname);
		sprintf(func.isql,"select * from %s ",tablename);
		retcode=realize_out_file(&func,filename);
		return retcode;
	}
	else
	{
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL )  )
		if(pHistableName==NULL) load_histable_name();

if (dbtype==DBTYPE_DB2)
{
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
}
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
{
		sprintf(func.isql,"select table_name from user_tables");
}
else if (dbtype==DBTYPE_MYSQL)
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
{
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);
}

		char *pt=NULL;
		int tbnum;
		int reclen;
		func.func=SEL_ISQL_RESULTS;
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n 从库中取表名失败!\n");

			if (datafmt) free (datafmt);
			if (pt) free (pt);

			return CS_FAIL;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;
		tbcount=0;
		
		char *tabName;//记录表名
		tabName = (char*) malloc(tbnum * reclen);
		memcpy(tabName,pt,tbnum*reclen);
		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(tabName+i*reclen)==1)
			{	//临时表，将表名置为空
				*(tabName+i*reclen)=0;
				continue;
			}
			if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) || (dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL )  )
			{		
				retcode=check_histable_name(tabName+i*reclen);
						// 参数库：只处理参数表; 历史库：只处理历史表
				if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
				{
						*(tabName+i*reclen)=0;
						continue;
				}
			}
		tbcount++;
		}
		fprintf(stderr,"\n 已从库中取出表名: %d 个表。\n",tbcount);
		tbcount=0;		
		int maxnamelen=0;
		int tmplen;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(tabName+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,tabName+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;
			strcpy(filename,tabName+i*reclen);
			strcpy(func.dbname,dbname);
			sprintf(func.table,"%s",filename);

			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: 是否输出表 [ %s ] 到文件 [%s] : 是/否/退出 (Y/N/Q)? ",tbcount,func.table,filename);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') 
					continue;
			}
			sprintf(func.isql,"select * from %s ",func.table);
			tmplen=maxnamelen-strlen(func.table);
			realize_out_file(&func,filename,tmplen,tbcount);
		}

		if (datafmt) free ( datafmt);
		if(pt) free(pt);

	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_out_hisdata()
{
	int serverno=-1;
	int err=0,deltype=-1;
	int dataYM=-1,datayear,datamon;
	int i,retcode,inchar,mon_num;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	char filename[255];

	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名。请用 dbmt out help 帮助。\n");
	    return 0;
	}

	for(i=1;i<argc;i++)		// -DTn n为要畚份的数据种类
		if(strncmp(argv[i],"-DT",3)==0)
		{
			if(argv[i][3]!=0)
			{
				if(sscanf(argv[i]+3,"%d",&deltype)<=0) deltype=-1;
				break;
			}
		}
	if(deltype<1 || deltype>HISDATANUM) deltype=-1;

	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-P")==0)
			bPrompt=1;
		else if(strncmp(argv[i],"-YM",3)==0)
				dataYM=atoi(argv[i]+3);
	}

	if(dataYM==-1)
	{
		fprintf(stderr,"\n命令格式不对，未提供日期。请用 dbmt out help 帮助。\n");
		return 0;
	}
	datayear=dataYM/100;
	datamon=dataYM%100;
	year=datayear;
	month=datamon;
	if(datayear<1998 || datayear>2999 || datamon<1 || datamon>12)
	{
		fprintf(stderr,"\n提供的日期不对。请用 dbmt out help 帮助。\n");
		return 0;
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;

		/* 检查系统管理员口令 */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	fprintf(stderr,"\n");
	for(i=0;i<HISDATANUM;i++)
		fprintf(stderr,"    %d、%s\n",i+1,datatype[i]);
	fprintf(stderr,"    0、退出\n");

	int dataflag;
	if(deltype==-1)
	{
		do
		{
			fprintf(stderr,"\n请选择要畚份的历史数据类型：[");
			for(i=1;i<=HISDATANUM;i++) fprintf(stderr,"%d,",i);
			fprintf(stderr,"0] : ");
			gets(strstdin);
			inchar=strstdin[0];
		}while(inchar<'0' || inchar>('0'+HISDATANUM) );
		
		if(inchar=='0') return 1;

		dataflag=inchar-'0';
	}
	else dataflag=deltype;

	if (bPrompt==1)
	{
		sprintf(strstdin,"\n是否畚份 %04d年%02d月 的%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
		sprintf(strstdin,"\n确认畚份 %04d年%02d月 的%s [Y/N]: ",
			datayear,datamon,datatype[dataflag-1]);
		inchar=get_input_char(strstdin);
		if(inchar=='N') return 1;
	}

	if (dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		strcpy(func.dbname,"modeldb");
	else
		strcpy(func.dbname,"hisdb");

	if(dataflag==1 || dataflag==2)	//存盘数据、统计结果
	{
		// yc199901 -> file yc199901
		sprintf(func.table,"yc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"yc%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// kwh199901 -> file kwh199901
		sprintf(func.table,"kwh%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"kwh%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// dayyc199901 -> dayyc199901
		sprintf(func.table,"dayyc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"dayyc%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// daykwh99901 ->daykwh199901
		sprintf(func.table,"daykwh%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daykwh%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// monthyc1999 -> file monthyc199901,monyc199902...
		sprintf(func.table,"monthyc%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monthyc%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);
		
		// monthkwh1999 -> file monkwh199901,monthkwh199902...
		sprintf(func.table,"monthkwh%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monthkwh%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);
		
		// 设备运行统计表 -> file 设备运行统计表199901,...
		sprintf(func.table,"设备运行统计表");
		sprintf(func.isql,"select * from %s where 存盘日期>=%04d%02d01 and 存盘日期<=%04d%02d31",
			func.table,dataYM/100,dataYM%100,dataYM/100,dataYM%100);
		sprintf(filename,"设备运行统计表%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

	}

	if(dataflag==1 || dataflag==3)	//事项和操作记录
	{
		// 电力历史事项表199901 -> file 电力历史事项表199901,...
		sprintf(func.table,"电力历史事项表%04d%02d",year,month);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"电力历史事项表%04d%02d",year,month);
		realize_out_file(&func,filename);
		
				
		// 操作历史事项表199901 -> file 操作历史事项表199901,...
		sprintf(func.table,"操作历史事项表%04d%02d",year,month);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"操作历史事项表%04d%02d",year,month);
		realize_out_file(&func,filename);

	}
	if(dataflag==1 || dataflag==4)	//追忆数据
	{
		// pdrdescribe -> file pdrdescribe199901,...
		sprintf(func.table,"pdrdescribe");
		sprintf(func.isql,"select * from %s where 年月日>=%04d%02d01 and 年月日<=%04d%02d31",
			func.table,dataYM/100,dataYM%100,dataYM/100,dataYM%100);
		sprintf(filename,"pdrdescribe%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// pdrdata -> file pdrdata199901,...

		time_t time1,time2;
		struct tm tmtime1,tmtime2;
		memset(&tmtime1,0,sizeof(tmtime1));
		memset(&tmtime2,0,sizeof(tmtime2));
		tmtime1.tm_isdst=-1;
		tmtime2.tm_isdst=-1;
		tmtime1.tm_sec=0; tmtime1.tm_min=0; tmtime1.tm_hour=0;
		tmtime1.tm_mday=1; tmtime1.tm_mon=datamon-1; tmtime1.tm_year=datayear-1900;
		tmtime2.tm_sec=0; tmtime2.tm_min=0; tmtime2.tm_hour=0;
		tmtime2.tm_mday=1;
		if(datamon<12) { tmtime2.tm_mon=datamon; tmtime2.tm_year=datayear-1900; }
		else { tmtime2.tm_mon=0; tmtime2.tm_year=datayear+1-1900; }
		time1=mktime(&tmtime1);
		time2=mktime(&tmtime2);

		sprintf(func.table,"pdrdata");
		sprintf(func.isql,"select * from %s where 时标>=%d and 时标<=%d",
			func.table,time1,time2);
		sprintf(filename,"pdrdata%04d%02d",datayear,datamon);
		realize_out_file(&func,filename);
	}

	if(dataflag==1 || dataflag==5)	//秒级曲线数据
	{
		// sdata199901 -> file sdata199901
		sprintf(func.table,"sdata%04d%02d",datayear,datamon);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"sdata%04d%02d",datayear,datamon);
		realize_out_file(&func,filename);

		//专为甘肃水调设计 begin
		sprintf(func.table,"开关变位统计表%04d%02d",datayear,datamon);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"开关变位统计表%04d%02d",datayear,datamon);
		realize_out_file(&func,filename);
		//专为甘肃水调设计 end
	}

	fprintf(stderr,"\n");
	return 1;
}

int CDbOperate::do_clean()
{   
    CS_RETCODE      retcode;
	CS_DATAFMT *datafmt;
	CS_SMALLINT	serverno,i,j;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;

	serverno=get_serverno();
	if(serverno<1 || serverno>4)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名或服务器不存在。请用 dbmt clean help 帮助。\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   未提供库名，请用dbmt clean help 帮助!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
		fprintf(stderr,"\n命令格式不对! 未提供表名或库名。请用 dbmt clean help 帮助。\n");
		return 0;
	}
	if(oneallflag==2)
		for(i=1;i<argc;i++)
		{
			if(strcmp(argv[i],"-P")==0)
				bPrompt=1;
		}

	if(oneallflag==2)
	{
		if(strcmp(dbname,"hisdb")==0)
		{
			fprintf(stderr,"\n %s为历史库，其数据量较大，建议采用 dbmt del 来删除数据。",dbname);
			fprintf(stderr,"\n 详细命令请用 dbmt del help 帮助");
			sprintf(strstdin,"\n 是否继续? (Y/N)");
			inchar=get_input_char(strstdin);
			if(inchar=='N') return 0;
			hisflag=1;
		}
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* 检查系统管理员口令 */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	if(oneallflag==1) fprintf(stderr,"\n清空表 %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n清空库 %s 中所有表:\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n是否清空表 %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n是否清空库 %s 中的所有表 (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if(oneallflag==1)
			sprintf(strstdin,"\n确认清空表 %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n确认清空库 %s 中的所有表 (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}

	if(oneallflag==1)
	{
		strcpy(func.table,tablename);

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"delete from %s where 1=1",tablename);
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"truncate table %s ",tablename);
else if (dbtype==DBTYPE_MYSQL)
		sprintf(func.isql,"delete from %s where 1=1",tablename);
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"truncate table %s.dbo.%s ",dbname,tablename);

		fprintf(stderr,"\n 正在清空表[%s] ...",tablename);

		func.func=ISQL_COMMAND;
		strcpy(func.dbname,dbname);
		retcode=dbsingle.IsqlCommand(&func,func.isql);
		if(retcode==CS_SUCCEED) fprintf(stderr,"正确");
		else fprintf(stderr,"错误");
		return retcode;
	}
	else
	{
if ((dbtype==DBTYPE_DB2) ||(dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai 增加mysql
		if(pHistableName==NULL) load_histable_name();
// 如果参数没有 [-A]
if (!bAllTables)
		load_unchangedtable_name();

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"select table_name from user_tables");
else if (dbtype==DBTYPE_MYSQL)
//		sprintf(func.isql,"SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
		sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);


		char *pt=NULL;
		int tbnum;
		int reclen;
		func.func=SEL_ISQL_RESULTS;

		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n 从库中取表名失败!\n");
			if (datafmt) free (datafmt);
			if (pt) free (pt);
			return CS_FAIL;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;

		tbcount=0;
		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(pt+i*reclen)==1)
			{	//临时表，将表名置为空
				*(pt+i*reclen)=0;
				continue;
			}
			if (!bAllTables)
			{
				if (check_unchangedtable_name(pt+i*reclen)==1)
				{
					*(pt+i*reclen)=0;
					continue;
				}
			}
if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai 增加mysql
{	
	retcode=check_histable_name(pt+i*reclen);
				// 参数库：只处理参数表; 历史库：只处理历史表
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
				*(pt+i*reclen)=0;
				continue;
			}
}
			tbcount++;
		}
		fprintf(stderr,"\n 已从库中取出表名: %d 个表。\n",tbcount);
		tbcount=0;		
		int maxnamelen=0;
		int tmplen;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(pt+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,pt+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;

			sprintf(func.table,"%s",pt+i*reclen);

			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: 是否清空表 [ %s ] : 是/否/退出 (Y/N/Q)? ",tbcount,func.table);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') 
					continue;
			}

if (dbtype==DBTYPE_DB2)
			sprintf(func.isql,"delete from %s where 1=1",func.table);
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL  )
			sprintf(func.isql,"truncate table %s ",func.table);
else if (dbtype == DBTYPE_MYSQL)
{
	sprintf(func.isql,"delete from %s where 1=1",func.table);
}
else // SYBASE_DB MSSQL_DB
			sprintf(func.isql,"truncate table %s ",func.table);

			func.func=ISQL_COMMAND;
			tmplen=maxnamelen-strlen(func.table);

			fprintf(stderr,"\n%d: 正在清空表 [ %s ] ...",tbcount,func.table);
			for(j=0;j<tmplen;j++) fprintf(stderr," ");
			retcode=dbsingle.IsqlCommand(&func,func.isql);
			if(retcode==CS_SUCCEED) fprintf(stderr,"成功");
			else fprintf(stderr,"失败");

		}
		if (!bAllTables)
		{
			fprintf(stderr,"\n \n  下列%d个表未清理，如需清理请加参数 [-A]详见dbmt clear help: \n",UnchangedTableNameCount);

			char tmpname[50];

			for(int i=0;i<UnchangedTableNameCount;i++)
			{
				strcpy(tmpname,pUnchangedTableName+i*UnchangedTableNameLen);
				fprintf(stderr,"  %d  %s \n",i+1,tmpname);
			}

		}

		if (datafmt) free (datafmt);
		if(pt) free(pt);
	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_drop()
{   
    CS_RETCODE      retcode;
	CS_SMALLINT	serverno,i,j;
	CS_DATAFMT *datafmt;
	char tablename[100];
	char dbname[100];
	int tbcount=0;
    int  inchar,hisflag=0;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));

	serverno=get_serverno();
	if(serverno<1||serverno>4)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名或服务器不存在。请用 dbmt drop help 帮助。\n");
	    return 0;
	}

	if(get_tablename(tablename)==1) 
	{
		oneallflag=1;
		if (!get_dbname(dbname))
		{
			fprintf(stderr,"\n   未提供库名，请用dbmt drop help 帮助!");
			return 0;
		}
	}
	else if(get_dbname(dbname)==1) oneallflag=2;
	else
	{
		fprintf(stderr,"\n命令格式不对! 未提供表名或库名。请用 dbmt drop help 帮助。\n");
		return 0;
	}
	if(oneallflag==2)
		for(i=1;i<argc;i++)
		{
			if(strcmp(argv[i],"-P")==0)
				bPrompt=1;
		}

	if(oneallflag==2)
	{
		if(strcmp(dbname,"hisdb")==0)
		{
			fprintf(stderr,"\n %s为历史库，建议采用 dbmt del 来删除数据。",dbname);
			fprintf(stderr,"\n 详细命令请用 dbmt del help 帮助");
			sprintf(strstdin,"\n 是否继续? (Y/N)");
			inchar=get_input_char(strstdin);
			if(inchar=='N') return 0;
			hisflag=1;
		}
	}

	if(db_start(serverno)!=CS_SUCCEED) return 0;
		/* 检查系统管理员口令 */	
	int dbflag=0;
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}

	if(oneallflag==1) fprintf(stderr,"\n删除表 %s :\n",tablename);
	else if(oneallflag==2) fprintf(stderr,"\n删除库 %s 中所有表:\n",dbname);

	if (bPrompt==1)
	{
		if(oneallflag==1)
			sprintf(strstdin,"\n是否删除表 %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n是否删除库 %s 中的所有表 (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
		if(oneallflag==1)
			sprintf(strstdin,"\n确认删除表 %s (Y/N)?",tablename);
		else if (oneallflag==2)
			sprintf(strstdin,"\n确认删除库 %s 中的所有表 (Y/N)?",dbname);
		inchar=get_input_char(strstdin);
		if (inchar=='N') { dbsingle.Close(); return 1; }
	}
	int maxnamelen=0;
	int tmplen;
	char *pt=NULL;
	int tbnum;
	int reclen;

	if(oneallflag==1)
	{
		strcpy(func.table,tablename);

		sprintf(func.isql,"drop table %s ",tablename);
		fprintf(stderr,"\n 正在删除表[%s] ...",tablename);

		func.func=ISQL_COMMAND;
		retcode=dbsingle.IsqlCommand(&func,func.isql);
		if(retcode==CS_SUCCEED) fprintf(stderr,"正确");
		else fprintf(stderr,"错误");
		return retcode;
	}
	else
	{
if ( (dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai 增加mysql
		if(pHistableName==NULL) load_histable_name();

		if(hisflag==0)
		{
if (dbtype==DBTYPE_DB2)
			sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='V' order by TABNAME");
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL  )
		sprintf(func.isql,"select table_name from user_tables");
else // SYBASE_DB MSSQL_DB
			sprintf(func.isql,"select name from %s.dbo.sysobjects where type='V' order by name",dbname);

			func.func=SEL_ISQL_RESULTS;
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
			if(retcode!=CS_SUCCEED)
			{
				fprintf(stderr,"\n 从库中取视图名失败!\n");
				if (datafmt) free (datafmt);
				if (pt) free (pt);
				return CS_FAIL;
			}
			tbnum=func.ret_roxnum;
			reclen=func.recorderlen;
			fprintf(stderr,"\n 已从库中取出视图名: %d 个视图。\n",tbnum);
			
			for(i=0;i<tbnum;i++) 
			{
				tmplen=strlen(pt+i*reclen);
				if(maxnamelen<tmplen) maxnamelen=tmplen;
			}
			maxnamelen+=10+strlen(dbname);

			for(i=0;i<tbnum;i++)
			{
				strcpy(func.table,pt+i*reclen);

				if(bPrompt==1)
				{
					sprintf(strstdin,"\n%d: 是否删除视图 [ %s ] : 是/否/退出 (Y/N/Q)? ",tbcount,func.table);
					inchar=get_input_char(strstdin,1);
					if(inchar==(int)'Q') break;
					if (inchar==(int)'N') 
						continue;
				}

				sprintf(func.isql,"use %s\r\ngo\r\ndrop view %s ",dbname,func.table);
				func.func=ISQL_COMMAND;
				tmplen=maxnamelen-strlen(func.table);

				fprintf(stderr,"\n%d: 正在删除视图 [ %s ] ...",i,func.table);
				for(j=0;j<tmplen;j++) fprintf(stderr," ");
				retcode=dbsingle.IsqlCommand(&func,func.isql);
				if(retcode==CS_SUCCEED) fprintf(stderr,"成功");
				else fprintf(stderr,"失败");

			}
			if (datafmt) free (datafmt);
			if(pt) free(pt);
		}

if (dbtype==DBTYPE_DB2)
		sprintf(func.isql,"select TABNAME from SYSCAT.TABLES where DEFINER='modeldb' and TYPE='T' order by TABNAME");
else if(dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"select table_name from user_tables");
else if (dbtype==DBTYPE_MYSQL)
{
	sprintf(func.isql,"SELECT table_name FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'modeldb' and table_type = 'base table'");
}
else // SYBASE_DB MSSQL_DB
		sprintf(func.isql,"select name from %s.dbo.sysobjects where type='U' order by name",dbname);

		func.func=SEL_ISQL_RESULTS;
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void**)&pt);
		if(retcode!=CS_SUCCEED)
		{
			fprintf(stderr,"\n 从库中取表名失败!\n");
			if (datafmt) free (datafmt);
			return CS_FAIL;
		}
		tbnum=func.ret_roxnum;
		reclen=func.recorderlen;

		tbcount=0;
		for(i=0;i<tbnum;i++)
		{
			if(check_tmptable_name(pt+i*reclen)==1)
			{	//临时表，将表名置为空
				*(pt+i*reclen)=0;
				continue;
			}
if ((dbtype==DBTYPE_DB2) || (dbtype==DBTYPE_ORACLE) ||(dbtype==DBTYPE_MYSQL)  || (dbtype==DBTYPE_DMSQL ) )		//modify by chenkai 增加mysql
{	retcode=check_histable_name(pt+i*reclen);
				// 参数库：只处理参数表; 历史库：只处理历史表
			if( (hisflag==0 && retcode==1) || (hisflag==1 && retcode==0) )
			{
				*(pt+i*reclen)=0;
				continue;
			}
}
			tbcount++;
		}
		fprintf(stderr,"\n 已从库中取出表名: %d 个表。\n",tbcount);
		tbcount=0;
		for(i=0;i<tbnum;i++) 
		{
			tmplen=strlen(pt+i*reclen);
			if(maxnamelen<tmplen) maxnamelen=tmplen;
		}
		maxnamelen+=10+strlen(dbname);

		for(i=0;i<tbnum;i++)
		{
			strcpy(func.table,pt+i*reclen);
			if(func.table[0]==0) continue;
			tbcount++;
			sprintf(func.table,"%s",pt+i*reclen);
			
			if(bPrompt==1)
			{
				sprintf(strstdin,"\n%d: 是否删除表 [ %s ] : 是/否/退出 (Y/N/Q)? ",tbcount,func.table);
				inchar=get_input_char(strstdin,1);
				if(inchar==(int)'Q') break;
				if (inchar==(int)'N') 
					continue;
			}

			sprintf(func.isql,"drop table %s ",func.table);
			func.func=ISQL_COMMAND;

			tmplen=maxnamelen-strlen(func.table);

			fprintf(stderr,"\n%d: 正在删除表 [ %s ] ...",tbcount,func.table);
			for(j=0;j<tmplen;j++) fprintf(stderr," ");
			retcode=dbsingle.IsqlCommand(&func,func.isql);
			if(retcode==CS_SUCCEED) fprintf(stderr,"成功");
			else fprintf(stderr,"失败");

		}
		if (datafmt) free (datafmt);
		if(pt) free(pt);
	}
	
	fprintf(stderr,"\n");
	dbsingle.Close();
	return 1;
}

int CDbOperate::do_disp()
{   

    CS_RETCODE      retcode;
	CS_SMALLINT	serverno;
	char dbname[20];
	char tablename[100];

	serverno=get_serverno();
	if(serverno<0)
	{
	    fprintf(stderr,"\n命令格式不对! 未提供服务器名。请用 dbmt disp help 帮助。\n");
	    return 0;
	}
	if(db_start(serverno)!=CS_SUCCEED) return 0;

	if(get_dbname(dbname))
	{
		if(get_tablename(tablename))
		{
		return do_disp_table(dbname,tablename);		
		}		
	}

	if ( dbtype==DBTYPE_SYBASE) 
	{
		FUNC_STR tmpfunc;
		CS_DATAFMT *datafmt;
		tmpfunc.func=SEL_ISQL_RESULTS;
		sprintf(tmpfunc.isql,"declare @size1 int,@fsize1 int,@size2 int,@fsize2 int,@segm int,@segbit int,@segsize float,@freesize float\r\n\
			select @segm=segment from hisdb.dbo.syssegments where name='sacsegment'\r\n\
			select @segbit=power(2,@segm)\r\n\
			select @segsize=sum(size),@freesize=sum(curunreservedpgs(db_id('hisdb'),lstart,unreservedpgs)) from master.dbo.sysusages\
			 where segmap&@segbit=@segbit and dbid=db_id('hisdb')\r\n\
			select @size1=@segsize*2048.0/1048576.0\r\n\
			select @fsize1=@freesize*2048.0/1048576.0\r\n\
			select @segm=segment from hisdb.dbo.syssegments where name='pdrsegment'\r\n\
			select @segbit=power(2,@segm)\r\n\
			select @segsize=sum(size),@freesize=sum(curunreservedpgs(db_id('hisdb'),lstart,unreservedpgs)) from master.dbo.sysusages\
			 where segmap&@segbit=@segbit and dbid=db_id('hisdb')\r\n\
			select @size2=@segsize*2048.0/1048576.0\r\n\
			select @fsize2=@freesize*2048.0/1048576.0\r\n\
			select @size1,@fsize1,@size2,@fsize2\r\ngo\r\n");
		typedef struct _dbsize_
		{ 
			int size;
			int freesize;
			int pdrsize;
			int pdrfreesize;
		} dbsize;
		dbsize *pt;
		retcode=dbsingle.SelectIsqlResults(&tmpfunc,&datafmt,(void **)&pt);
		if(retcode!=CS_SUCCEED || tmpfunc.ret_roxnum<=0 ||pt->size==0)
		{
			retcode=CS_FAIL;
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (pt) {
				free (pt);pt=NULL;}
			fprintf(stderr,"\n不能显示!\n");
			dbsingle.Close();
			return 0;
		}
		
		int dbspace,dbfreespace,pdrspace,pdrfreespace;
		dbspace=pt->size;
		dbfreespace= pt->freesize;
		pdrspace=pt->pdrsize;
		pdrfreespace=pt->pdrfreesize;
		fprintf(stderr,"\n----------------------------------------------------");
		if(dbspace>0)
			fprintf(stderr,"\n历史数据:  容量=%6d M , 剩余=%6d M , 剩余%3d%%",
				dbspace,dbfreespace,dbfreespace*100/dbspace);
		else 
			fprintf(stderr,"\n历史数据:  容量=%6d M , 剩余=%6d M , 剩余%3d%%",
				dbspace,dbfreespace,0);
		if(pdrspace>0)
			fprintf(stderr,"\n追忆数据:  容量=%6d M , 剩余=%6d M , 剩余%3d%%",
				pdrspace,pdrfreespace,pdrfreespace*100/pdrspace);
		else
			fprintf(stderr,"\n追忆数据:  容量=%6d M , 剩余=%6d M , 剩余%3d%%",
				pdrspace,pdrfreespace,0);
		fprintf(stderr,"\n----------------------------------------------------\n");

		sprintf(tmpfunc.isql,"declare @value1 int,@value2 int,@value3 int,@value4 int,@value5 int,@value6 int\r\n\
			select @value1=value from master.dbo.sysconfigures where name='number of user connections'\r\n\
			select @value2=value from master.dbo.sysconfigures where name='number of devices'\r\n\
			select @value3=value from master.dbo.sysconfigures\
			where name='procedure cache percent'\r\n\
			select @value4=value from master.dbo.sysconfigures where name='total memory'\r\n\
			select @value5=value from master.dbo.sysconfigures where name='default data cache' and comment='4K I/O Buffer Pool'\r\n\
			select @value6=value from master.dbo.sysconfigures where name='default data cache' and comment='16K I/O Buffer Pool'\r\n\
			select @value1,@value2,@value3,@value4,@value5,@value6\r\ngo\r\n");
		typedef struct _dboptimize
		{ 
			int value1;//最大用户连接数：
			int value2;//最大设备总数：
			int value3;//过程缓冲比例：
			int value4;//总内存数：
			int value5;//数据缓冲 4K
			int value6;//数据缓冲 16K
		} dboptimize;
		dboptimize *ptt;
		retcode=dbsingle.SelectIsqlResults(&tmpfunc,&datafmt,(void **)&ptt);
		if(retcode!=CS_SUCCEED || tmpfunc.ret_roxnum<=0 )
		{
			retcode=CS_FAIL;
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (ptt) {
				free (ptt);ptt=NULL;}
			fprintf(stderr,"\n不能显示!\n");
			dbsingle.Close();
			return 0;
		}
		float v4k,v16k;
		v4k=(float)ptt->value5/1024;
		v16k=(float)ptt->value6/1024;
		fprintf(stderr,"\n----------------------------------------------------");
		fprintf(stderr,"\n数据库性能:  最大用户连接数： %d 个。",ptt->value1);
		fprintf(stderr,"\n数据库性能:  最大设备总数： %d 个。",ptt->value2);
		fprintf(stderr,"\n数据库性能:  过程缓冲比例： %d %%。",ptt->value3);
		fprintf(stderr,"\n数据库性能:  总内存数： %d M。",ptt->value4/500);
		if(ptt->value5)
			fprintf(stderr,"\n数据库性能:  数据缓冲 4K: %0.2f M。",v4k);
		else
			fprintf(stderr,"\n数据库性能:  数据缓冲 4K: 未分配。");
		if (ptt->value6)
			fprintf(stderr,"\n数据库性能:  数据缓冲 16K: %0.2f M。",v16k);
		else
			fprintf(stderr,"\n数据库性能:  数据缓冲 16K: 未分配。");
		fprintf(stderr,"\n----------------------------------------------------\n");

		if (datafmt) {
			free(datafmt);datafmt=NULL;}
		if (ptt) {
			free (ptt);ptt=NULL;}
		if (pt) {
			free(pt);pt=NULL;}

	}//end if (dbtype==DBTYPE_SYBASE)
	else if (dbtype ==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
	{
		FUNC_STR func;
		CS_DATAFMT *datafmt=NULL;
		void *pt;
		int ptcnt;

		int totalnum[3],freenum[3];
		int maxconnectnum,i;
		float speedstorerate;
		char *tablespace[3]={"USER_DATA","TEMP_DATA","RB_DATA"};
		func.func=SEL_ISQL_RESULTS;
		strcpy(func.dbname,"modeldb");

		for (i=0;i<3;i++)
		{
			//读取表空间的总大小
			sprintf(func.isql,"select sum(bytes)/1024 from dba_data_files where tablespace_name='%s'",tablespace[i]);
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
			if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
			{
				if (datafmt) {
					free(datafmt);datafmt=NULL;}
				if (pt) {
					free (pt);pt=NULL;}
				fprintf(stderr,"\n不能显示!\n");
				dbsingle.Close();
				return retcode;
			}
			totalnum[i] = (int)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (pt) {
				free (pt);pt=NULL;}

			//读取表空间的剩余大小
			sprintf(func.isql,"select sum(bytes)/1024 from dba_free_space where tablespace_name='%s'",tablespace[i]);
			retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
			if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
			{
				if (datafmt) free(datafmt);
				if (pt) free (pt);
				fprintf(stderr,"\n不能显示!\n");
				dbsingle.Close();
				return retcode;
			}
			freenum[i] = (int)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);
			if (datafmt) {
				free(datafmt);datafmt=NULL;}
			if (pt) {
				free (pt);pt=NULL;}
		}
		//读取数据库最大连接数
		strcpy(func.isql,"select to_number(value) from v$parameter where name='processes'");
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
		if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
		{
			if (datafmt) free(datafmt);
			if (pt) free (pt);
			fprintf(stderr,"\n不能显示!\n");
			dbsingle.Close();
			return retcode;
		}
		maxconnectnum=(int)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);

		if (datafmt) {
			free(datafmt);datafmt=NULL;}
		if (pt) {
			free (pt);pt=NULL;}

		//读取数据库缓冲区高速缓存命中率
		strcpy(func.isql,"select (a.value-b.value+c.value)*100/a.value from v$sysstat a,v$sysstat b,v$sysstat c where a.name='session logical reads' and b.name='physical reads' and c.name='physical reads direct'");
		retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
		if(retcode!=CS_SUCCEED || func.ret_roxnum<=0 )
		{
			if (datafmt) free(datafmt);
			if (pt) free (pt);
			fprintf(stderr,"\n不能显示!\n");
			dbsingle.Close();
			return retcode;
		}
		speedstorerate=(float)dbsingle.GetData(0,&func,datafmt,pt,ptcnt,NULL);

		if (datafmt) {
			free(datafmt);datafmt=NULL;}
		if (pt) {
			free (pt);pt=NULL;}
		fprintf(stderr,"\n----------------------------------------------------");
	
		if(freenum[0]>0)
			fprintf(stderr,"\n\n用户表空间:  容量=%6d M , 剩余=%6d M , 剩余%3.2f%%",
				(int)(totalnum[0]/1024),(int)(freenum[0]/1024),((float)freenum[0]*100/(float)totalnum[0]));
		else 
			fprintf(stderr,"\n用户表空间:  容量=%6d M , 剩余=%6d M , 剩余%3.2f%%",
				(int)(totalnum[0]/1024),(int)(freenum[0]/1024),0);
		
		if(freenum[1]>0)
			fprintf(stderr,"\n临时表空间:  容量=%6d M , 剩余=%6d M , 剩余%3.2f%%",
				(int)(totalnum[1]/1024),(int)(freenum[1]/1024),((float)freenum[1]*100/(float)totalnum[1]));
		else 
			fprintf(stderr,"\n临时表空间:  容量=%6d M , 剩余=%6d M , 剩余%3.2f%%",
				(int)(totalnum[1]/1024),(int)(freenum[1]/1024),0);
		if(freenum[2]>0)
			fprintf(stderr,"\n回滚段空间:  容量=%6d M , 剩余=%6d M , 剩余%3.2f%%",
				(int)(totalnum[2]/1024),(int)(freenum[2]/1024),((float)freenum[2]*100/(float)totalnum[2]));
		else 
			fprintf(stderr,"\n回滚段空间:  容量=%6d M , 剩余=%6d M , 剩余%3.2f%%",
				(int)(totalnum[2]/1024),(int)(freenum[2]/1024),0);
		fprintf(stderr,"\n\n----------------------------------------------------\n");
		
		fprintf(stderr,"\n----------------------------------------------------\n");
		fprintf(stderr,"\n数据库性能:  最大用户连接数： %d 个。",maxconnectnum);
		fprintf(stderr,"\n数据库性能:  高速缓存命中率： %2.2f%%。",speedstorerate);
		fprintf(stderr,"\n\n----------------------------------------------------\n");
	
	}//end if (dbtype ==DBTYPE_ORACLE)
	else 
	{
		fprintf(stderr,"\n不能显示!\n");
	}
	dbsingle.Close();
	return 1;
}
int CDbOperate::do_disp_table(CS_CHAR *dbname,CS_CHAR *tablename)
{
	CS_RETCODE retcode;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_DATAFMT *datafmt;
	int flag=0,i=0;
	
	func.func=GET_TABLE_INFO;
	strcpy(func.table,tablename);
	strcpy(func.dbname,dbname);
	retcode=dbsingle.GetTableInfo(&func,&datafmt);
	if (retcode!=CS_SUCCEED) 
	{
		fprintf(stderr,"\n  获取%s表信息失败",tablename);
		return retcode;
	}
	fprintf(stderr,"\n%s	域名			类型",tablename);
	for (i=0;i<func.colnum;i++)
	{
		switch((datafmt+i)->datatype)
		{
		case CS_SMALLINT_TYPE:
			if (dbtype==DBTYPE_SYBASE)
				fprintf(stderr,"\n		%s:		smallint",(datafmt+i)->name);
			else 
				fprintf(stderr,"\n		%s:		number(5)",(datafmt+i)->name);
			break;
		case CS_TINYINT_TYPE:
			if (dbtype==DBTYPE_SYBASE)
				fprintf(stderr,"\n		%s:		tinyint",(datafmt+i)->name);
			else 
				fprintf(stderr,"\n		%s:		number(3)",(datafmt+i)->name);
			break;
		case CS_USHORT_TYPE:
			fprintf(stderr,"\n		%s:		uchar",(datafmt+i)->name);
			break;
		case CS_INT_TYPE:
			if (dbtype==DBTYPE_SYBASE)
				fprintf(stderr,"\n		%s:		int",(datafmt+i)->name);
			else 
				fprintf(stderr,"\n		%s:		number(10)",(datafmt+i)->name);
			break;
		case CS_CHAR_TYPE:
			fprintf(stderr,"\n		%s:		char[%02d]",(datafmt+i)->name,(datafmt+i)->maxlength);
			break;
		case CS_REAL_TYPE:
			fprintf(stderr,"\n		%s:		real",(datafmt+i)->name);
			break;
		case CS_FLOAT_TYPE:
			fprintf(stderr,"\n		%s:		float",(datafmt+i)->name);
			break;
		default:
			break;
		}
	}
	fprintf(stderr,"\n----------------------------------------------------");

	fprintf(stderr,"\n\n主键：	");
	for (i=0;i<func.colnum;i++)
		if (((datafmt+i)->status)&CS_KEY)
		{
			fprintf(stderr,"%s	",(datafmt+i)->name);
			flag++;
		}
	if (flag==0) 	fprintf(stderr,"无主键");
	fprintf(stderr,"\n----------------------------------------------------");

	fprintf(stderr,"\n\n索引：	\n");
	func.func=SEL_ISQL_RESULTS;
	strcpy(func.dbname,"modeldb");
	char *pt;
	char indexname[30];
	if (dbtype==DBTYPE_ORACLE  || dbtype==DBTYPE_DMSQL )
		sprintf(func.isql,"select index_name from user_indexes where table_name='%s'",func.table);
	else//DBTYPE_SYBASE
		sprintf(func.isql,"select name from sysindexes where id=Object_id('%s')",func.table);


	retcode=dbsingle.SelectIsqlResults(&func,&datafmt,(void **)&pt);
	
	strncpy(indexname,pt,30);
	if (strcmp(indexname,tablename)==0)
		fprintf(stderr,"无索引!");
	else
	{
		for (int tt=0;tt<func.ret_roxnum;tt++)
		{
			memset(indexname,0,sizeof(char)*30);
			strncpy(indexname,pt+tt*30,30);
			fprintf(stderr,"\t%s\n",indexname);
		}
	}

	dbsingle.Close();
	return retcode;
}
int CDbOperate::do_scada()
{
	FILE *stream;
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	int serverno;
	int pluseflag,daystatflag,devflag;
	int year,month,Sec;
	CS_RETCODE retcode;
	struct tm *Time;
	char filename[24];

	retcode=get_filename(filename);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n		未提供文件名或文件的路径不正确!");
		fprintf(stderr,"\n		请用 dbmt scada help 帮助。");
		return retcode;
	}
	if (strstr(filename,"YC")==NULL)
		pluseflag=1;
	else	pluseflag=0;
	if (strstr(filename,"DAY")==NULL)
		daystatflag=0;
	else daystatflag=1;
	if (strstr(filename,"DEV")==NULL)
		devflag=0;
	else devflag=1;
	//取得文件的大小
	long filesize;
	#if defined(WIN32)
	int file_handle;
	if ((file_handle=open(filename,1)) ==-1)
	{
		printf("failed");
	}
	else
	{
		filesize = filelength(file_handle);
		close(file_handle);
	}
	#elif defined(__unix)
	filesize = filelength(filename);
	#endif

	stream = fopen(filename,"rb");
	if (stream==NULL)
	{
		fprintf(stderr,"\n		文件%s打开失败!",filename);
		return 0;
	}
	serverno=get_serverno();
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"\n		数据库打开失败!");
		return retcode;
	}
	int i=0;
	//////////////////////////////////
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return 0;
	}
	HIS_DATA *hisdata;
	hisdata = (HIS_DATA *) malloc(filesize);
	memset(hisdata,0,filesize);
	int retvalue=fread(hisdata,sizeof(HIS_DATA),filesize/sizeof(HIS_DATA),stream);
	
	//add 2014 11 begin
	func.his.date= hisdata->date;
	func.his.time= hisdata->time;
	//add 2015 11 end
    Sec=(hisdata->date)*24*60*60;
	time_t stime;
	stime=Sec;
	Time=localtime(&stime);
	year=Time->tm_year+1900;//年
	month=Time->tm_mon+1;//月
	if(devflag) sprintf(func.table,"设备运行统计表");
	else if (daystatflag) 
		{	
			if (pluseflag) 	sprintf(func.table,"daykwh%04d%02d",year,month);
			else 	sprintf(func.table,"dayyc%04d%02d",year,month);
		}
		else 
		{
			if (pluseflag) 	sprintf(func.table,"kwh%04d%02d",year,month);
			else 	sprintf(func.table,"yc%04d%02d",year,month);
		}

	func.func=ISQL_COMMAND;
	char pSqlCmd[100];
	sprintf(pSqlCmd,"use hisdb\r\ngo\r\ndelete from %s where sdate=%d and time=%d \r\ngo\r\n",func.table,func.his.date,func.his.time);
	if(bPrompt==1)
	{
		retcode = dbsingle.IsqlCommand(&func,pSqlCmd);
		if (retcode==CS_SUCCEED)
		{
			fprintf(stderr,"\n			删除成功!");
		}
		else
			fprintf(stderr,"\n			删除失败!");
	}
	//////////////////////设置时间和日期
	for(i=1;i<(int)(filesize/sizeof(HIS_DATA));i++)
	{
		(hisdata+i)->date=hisdata->date;
		(hisdata+i)->time=hisdata->time;
	}

	func.func = INS_MIN_HISDATA;
	func.roxnum = filesize/sizeof(HIS_DATA);
	func.his.date = hisdata->date;
	func.his.time = hisdata->time;
	func.his.pulseflag=pluseflag;
	strcpy(func.dbname,"hisdb");
	retcode = dbsingle.InsertHisdata(&func,hisdata);
	if (retcode==CS_SUCCEED)
	{
		fprintf(stderr,"\n			文件导入成功!");
	}
	else
		fprintf(stderr,"\n			文件导入失败!");

	if (hisdata) free(hisdata);
	fclose(stream);
	dbsingle.Close();
	return retcode;
}
int CDbOperate::get_filename(char *filename)
{
	for (int i=1;i<argc;i++)
	{
		if (strncmp(argv[i],"-F",2)==0)
		{
			if (argv[i][2]!=0)
			{
				strcpy(filename,argv[i]+2);
				return 1;
			}
			fprintf(stderr,"\n-F 未提供文件名!");
			return 0;
		}
	}
	return 0;
}
int CDbOperate::do_dump()
{
	CS_RETCODE retcode;
	int serverno=0;
	char dbname[100];
	char pSqlCmd[100];
	serverno=get_serverno();
	if (serverno<1||serverno>4)
	{
		fprintf(stderr,"\n		未指定服务器!或服务器不存在!");
		fprintf(stderr,"\n		请用 dbmt dump help 帮助。");
		return 0;
	}
	if (get_dbname(dbname)==0)
	{
		fprintf(stderr,"\n		未指定数据库!或数据库错误!");
		fprintf(stderr,"\n		请用 dbmt dump help 帮助。");
		return 0;
	}

	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;

	/* 检查系统管理员口令 */	
	retcode=check_root_password(serverno);
	if(retcode!=CS_SUCCEED)
	{
		dbsingle.Close();
		return retcode;
	}
	FUNC_STR func;
	func.func=ISQL_COMMAND;
	strcpy(func.dbname,"modeldb");
	if (dbtype==DBTYPE_SYBASE)
	{
		sprintf(pSqlCmd,"use master \r\ngo\r\n dump transaction %s with truncate_only\r\ngo\r\n",dbname);
	}
	else//DBTYPE_ORACLE
	{
		fprintf(stderr,"\n		库日志不必清除");
		return retcode;
	}
	
	retcode=dbsingle.IsqlCommand(&func,pSqlCmd);
	if (retcode==CS_SUCCEED) fprintf(stderr,"\n		%s库日志清除成功",dbname);
	dbsingle.Close();
	return retcode;
}
int CDbOperate::do_check()
{
	CS_RETCODE retcode;
	int serverno=0;
	int inchar,i;
	serverno=get_serverno();
	if (serverno<1||serverno>4)
	{
		fprintf(stderr,"\n		未指定服务器!或服务器不存在!");
		fprintf(stderr,"\n		请用 dbmt check help 帮助。");
		return 0;
	}
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;
	for(i=0;i<CHECKTABLENUM;i++)
		fprintf(stderr,"    %d、%s\n",i+1,checktable[i]);
	fprintf(stderr,"    0、退出\n");
	do
	{
		fprintf(stderr,"\n请选择要检查的参数表：[");
		for(i=1;i<=CHECKTABLENUM;i++) fprintf(stderr,"%d,",i);
		fprintf(stderr,"0] : ");
		gets(strstdin);
		inchar=strstdin[0];
	}while(inchar<'0' || inchar>('0'+CHECKTABLENUM) );
		
	if(inchar=='0') return 1;
	int dataflag=inchar-'0';
	
	switch(dataflag)
	{
	case 1:
		check_table("SCD设备参数表",1);
		check_table("遥测参数表",2);
		check_table("遥信参数表",3);
		check_table("电度参数表",4);
		break;
	case 2:
		check_table("SCD设备参数表",dataflag-1);
		break;
	case 3:
		check_table("遥测参数表",dataflag-1);
		break;
	case 4:
		check_table("遥信参数表",dataflag-1);
		break;
	case 5:
		check_table("电度参数表",dataflag-1);
		break;
	default:
		break;
	}
	dbsingle.Close();
	return retcode;
}
int CDbOperate::check_table(CS_CHAR *tablename,int dataflag)
{
	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_DATAFMT *datafmt;
	int j1,j2,j3,j4,ptcnt;
	int checknum,i;

	/////////////////////////////////////////////////////////////
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tablename);
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"代码")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
		if(strcmp(datafmt[i].name,"描述")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 1;
			j2=i;
		}
		if(strcmp(datafmt[i].name,"类型")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 2;
			j3=i;
		}
		if (dataflag!=1){
		if(strcmp(datafmt[i].name,"设备代码")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 3;
			j4=i;
		}
		}
	}
	void *pt;
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<=0) return FALSE;
	checknum=func.ret_roxnum;
	CHECK_VALUE *check_value;
	check_value = (CHECK_VALUE *) malloc (sizeof(CHECK_VALUE)*func.ret_roxnum);
	memset(check_value,0,sizeof(CHECK_VALUE)*func.ret_roxnum);
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,check_value[i].name);
		dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,check_value[i].desc);
		check_value[i].type=(short)dbsingle.GetData(i,&func,datafmt+j3,pt,ptcnt,NULL);
		if (dataflag!=1)
			dbsingle.GetData(i,&func,datafmt+j4,pt,ptcnt,check_value[i].devname);
	}
	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	/////////////////////////////////////////////////////////////////////////
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"站所线系参数表");
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"代码")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
	}
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<=0) return FALSE;
	stationnum=func.ret_roxnum;
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,stationname[i]);
	}
	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	////////////////////////////////////////////////////////////////////////
	if (dataflag!=1){
		func.func = GET_TABLE_INFO;
		strcpy(func.table,"SCD对象类型表");
		strcpy(func.dbname,"modeldb");
		if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
			return FALSE;

		for (i=0;i<func.colnum;i++)
		{
			func.col[i].condition = NO;
			func.col[i].select = NO;
			func.col[i].order  = -1;
			if(strcmp(datafmt[i].name,"类型值")==0)
			{
				func.col[i].select = YES;
				func.col[i].order_d = 0;
				j1=i;
			}
			if(strcmp(datafmt[i].name,"类型代码")==0)
			{
				func.col[i].select = YES;
				func.col[i].order_d = 1;
				j2=i;
			}
		}
		func.func=SEL_RECORDERS;
		if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
			return FALSE;
		if (func.ret_roxnum<=0) return FALSE;
		scdtypenum=func.ret_roxnum;
		scdtypevalue = (SCD_TYPEVALUE *) malloc (sizeof(SCD_TYPEVALUE)*scdtypenum);
		memset(scdtypevalue,0,sizeof(SCD_TYPEVALUE)*scdtypenum);
		for(i=0;i<func.ret_roxnum;i++)
		{
			scdtypevalue[i].typevalue=(short)dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,NULL);
			dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,scdtypevalue[i].typedaima);
			if (strlen(scdtypevalue[i].typedaima)==1)
				scdtypevalue[i].typedaima[1]=scdtypevalue[i].typedaima[0];
			if (!strncmp(scdtypevalue[i].typedaima,"Cb",2))
				strcpy(scdtypevalue[i].typedaima,"Kg");
		}
	}
	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	int badnum=0;

	CheckNameLogic(check_value,checknum,dataflag);
	for(i=0;i<checknum;i++)	{
	if (check_value[i].logicalflag) 	badnum++;
	}
	
	fprintf(stderr,"\n----------------------------------------------------");
	fprintf(stderr,"\n%s		错误代码共有%d个",tablename,badnum);
	if(badnum>0)
	{
		int ii=1;
		fprintf(stderr,"\n----------------------------------------------------");
		if (dataflag==1)
				fprintf(stderr,"\n%3s %24s %40s	%3s","序号","代码","描述","类型");
		else	fprintf(stderr,"\n%3s %24s %40s	%3s %24s","序号","代码","描述","类型","设备代码");
		for(i=0;i<checknum;i++)
		{
			if (dataflag!=1){
				if (check_value[i].logicalflag) 
				fprintf(stderr,"\n%3d %24s %40s	%3d %24s",ii++,check_value[i].name,check_value[i].desc,check_value[i].type,check_value[i].devname);
			}
			else
			{
				if (check_value[i].logicalflag) 
				fprintf(stderr,"\n%3d %24s %40s	%3d",ii++,check_value[i].name,check_value[i].desc,check_value[i].type);
			}
		}
	}
	if (check_value)
	{
		free (check_value);
		check_value=NULL;
	}
	if (scdtypevalue)
	{
		free (scdtypevalue);
		scdtypevalue=NULL;
	}
	return 1;
}
void CDbOperate::CheckNameLogic(CHECK_VALUE *value,int checknum,int dataflag)
{
	char first[1],last[2];
	char *tempflag=NULL;
	int i,j,strlength;
	int cmpflag=-1;

	if (dataflag==1)
	{
		for(i=0;i<checknum;i++)
		{
			strncpy(first,value[i].name,1);
			switch(first[0])
			{
			case 'K'://开关
				if (value[i].type!=1) value[i].logicalflag=1;
				break;
			case 'D'://刀闸
				if (value[i].type!=2&&value[i].type!=4) value[i].logicalflag=1;
				break;
			case 'B'://变压器
				if (value[i].type!=5&&value[i].type!=6) value[i].logicalflag=1;
				break;
			case 'M'://母线
				if (value[i].type!=16) value[i].logicalflag=1;
				break;
			case 'X'://线路
				if (value[i].type!=14&&value[i].type!=15&&value[i].type!=19) value[i].logicalflag=1;
				break;
			case 'F'://发电机
				if (value[i].type!=17) value[i].logicalflag=1;
				break;
			case 'I'://电流互感器
				if (value[i].type!=11) value[i].logicalflag=1;
				break;
			case 'U'://电压互感器
				if (value[i].type!=10) value[i].logicalflag=1;
				break;
			case 'C'://电容器
				if (value[i].type!=7) value[i].logicalflag=1;
				break;
			case 'R'://电抗器
				if (value[i].type!=8) value[i].logicalflag=1;
				break;
			case 'L'://避雷器
				if (value[i].type!=12) value[i].logicalflag=1;
				break;
			case 'P'://保护装置
				if (value[i].type!=22) value[i].logicalflag=1;
				break;
			case 'Q'://消弧线圈
				if (value[i].type!=9) value[i].logicalflag=1;
				break;
			case 'H'://负荷
				if (value[i].type!=21) value[i].logicalflag=1;
				break;
			case 'O'://其它设备
				if (value[i].type!=51) value[i].logicalflag=1;
				break;
			case 'S'://厂站
				if (value[i].type!=52) value[i].logicalflag=1;
				break;
			default:
				value[i].logicalflag=1;
				break;
			}//switch
			if (!strcmp(value[i].name,"sys")) value[i].logicalflag=0;
			for (j=0;j<stationnum;j++)
			{
				tempflag=strstr(value[i].name,stationname[j]);
				if (tempflag) break;
			}
			if (!tempflag) value[i].logicalflag=1;
		}
	}
	else if (dataflag==2||dataflag==3||dataflag==4)
	{
		//比较量测类型码
		for(i=0;i<checknum;i++)
		{
			int existflag=0;
			for(j=0;j<scdtypenum;j++)
			{
				if (value[i].type == (int)scdtypevalue[j].typevalue)
				{
					existflag=1;
					strlength=strlen(value[i].name);
					if (strlength<=4) {
						value[i].logicalflag=1;
						break;
					}
					last[0]=value[i].name[strlength-2];
					last[1]=value[i].name[strlength-1];
					if (last[1]<='Z'&&last[1]>='A')//最后字母为大写字母
						last[0]=last[1];
					cmpflag=strncmp(last,scdtypevalue[j].typedaima,2);
					if (cmpflag!=0)
						value[i].logicalflag=1;
					break;
				}
			}

			if (!existflag) value[i].logicalflag=1;
			//比较量测表中是否包含厂站代码
			tempflag=NULL;
			for (j=0;j<stationnum;j++)
			{
				tempflag=strstr(value[i].name,stationname[j]);
				if (tempflag) break;
			}
			if (!tempflag) value[i].logicalflag=1;
			//比较量测表的头一个字符是否与其设备的头一个字符相同
			last[0]=value[i].name[0];
			last[1]=value[i].devname[0];
			if (last[0]!=last[1]&&last[0]!=last[1]-32)
				value[i].logicalflag=1;
		}
	}
}
//201508 add begin
int CDbOperate::realize_in_file_ex(FUNC_STR *func,char *filename,CHECK_VALUE *change,int changenum,int id)
{
	CS_RETCODE retcode;
	int i,numrows;
	void *p6;
	FILE *stream=NULL;
	if(id>=0) fprintf(stderr,"\n%d 正在装入记录到表[%s] ...",id,func->table);
	else fprintf(stderr,"\n 正在装入记录到表[%s] ...",func->table);

	if (change==NULL) return 0;

	if (func->his.flag==2)
	{
		func->func=ISQL_COMMAND;
		sprintf(func->isql,"truncate table %s",func->table);
		retcode=dbsingle.IsqlCommand(func,func->isql);
		if (retcode==CS_SUCCEED) fprintf(stderr,"\n		原记录清除成功");
		else {
			fprintf(stderr,"\n		原记录清除失败");
			//return retcode;
		}
	}	
	CS_DATAFMT *datafmt;
	func->func=GET_TABLE_INFO;
	retcode = dbsingle.GetTableInfo(func,&datafmt);
	if (retcode!=CS_SUCCEED)
	{
		if (datafmt) free (datafmt);
		fprintf (stderr,"\n %s 打开失败!",func->table);
		return CS_FAIL;
	}
	
	CS_DATAFMT** fmt = &datafmt;
	void**  ptt = (void**)fmt;
	stream=fopen(filename,"rt");
	if(stream==NULL)
	{
		if (datafmt) free(datafmt);
		fprintf(stderr,"\n无文件[%s] 或打开文件错误!",filename);
		return CS_FAIL;
	}

	fseek(stream,0,SEEK_SET);//定位在文件头
	
	numrows=1;
	char * pstr=(char*)malloc(EX_BUFSIZE*2+10);

	fgets(pstr,EX_BUFSIZE*2,stream);//若第一行为表结构，过滤第一行
	if(strncmp(pstr,"--DATAFMT: ",11)!=0)
	fseek(stream,0,SEEK_SET);//定位在文件头

	char *pcur;
	VAL_UNION *value;
	int tmpint,j,tt;
	int shortvalue =0;
	int recnum=0;
	value=(VAL_UNION *) malloc (sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); //为了防止内存不足最大只能为5000*func->colnum

	while(1)
	{
		memset(value,0,sizeof(VAL_UNION)*MAXINROWNUM*func->colnum); 
		for(i=0;i<MAXINROWNUM;i++)
		{
			if (fgets(pstr,EX_BUFSIZE*2,stream)==NULL)
				break;
			else 
				pcur=pstr;

			for(j=0;j<func->colnum;j++)
			{
				if (j==5) continue;//第六列为rawdata 
				if(j>0)
				{
					if( (pcur=strchr(pcur,'\t'))==NULL )	/* 每个域用TAB键隔开 */
					{
						retcode=CS_FAIL;
						break;
					}
					pcur+=1;
				}
				tmpint=i*func->colnum+j;
				switch(datafmt[j].datatype)
				{
					case CS_SMALLINT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->smallvalue=0;
						else
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->smallvalue=shortvalue;
						break;
					case CS_TINYINT_TYPE:
					case CS_BIT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->tinyvalue=0;
						else	
							if(sscanf(pcur,"%d",&shortvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else 
								(value+tmpint)->tinyvalue=(unsigned char)shortvalue;
						break;
					case CS_USHORT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->usmallvalue=0;
						else
							if(sscanf(pcur,"%d",&value)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
							else (value+tmpint)->usmallvalue=shortvalue;
						break;
					case CS_INT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->intvalue=0;
						else
							if(sscanf(pcur,"%d",&((value+tmpint)->intvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_CHAR_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') strcpy((value+tmpint)->charvalue," ");
						else 
							if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						for (tt=0;tt<changenum;tt++)
						{
							if (strcmp((value+tmpint)->charvalue,(change+tt)->name)==0)
							{
								strcpy((value+tmpint)->charvalue,(change+tt)->devname);
								break;
							}
						}
						break;
					case CS_REAL_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->realvalue=0;
						else
							if(sscanf(pcur,"%g",&((value+tmpint)->realvalue))<=0)
							{
								retcode=CS_FAIL;
								break;
							}
						break;
					case CS_FLOAT_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') (value+tmpint)->floatvalue=0;
						else
						if(sscanf(pcur,"%lg",&((value+tmpint)->floatvalue))<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					case CS_DATETIME_TYPE:
						if(*pcur=='\t' || *pcur=='\r' || *pcur=='\n') 
						{
							retcode=CS_FAIL;
							break;
						}
						if(sscanf(pcur,"%[^\t\r\n]s",(value+tmpint)->charvalue)<=0)
						{
							retcode=CS_FAIL;
							break;
						}
						break;
					default:
						break;
				}//switch
			}//for (j)
		}//for (i)
		p6=value;
		fprintf(stderr,"-");
		if(i==0) break;
		retcode=dbsingle.DbTools(DB_IN_FILE,(void*)func->dbname,(void*)func->table,(void*)(&func->colnum),ptt,&i,&p6);
		if(retcode==CS_SUCCEED)			recnum+=i;

		if(i<MAXINROWNUM) break;
	}
//	}//while

	//for(j=0;j<null_num;j++) putchar(' ');
	putchar('\t');
	if(retcode!=CS_SUCCEED)
	{
		fprintf(stderr,"	失败!  ");
		fprintf(stderr,"插入数据错误!",func->table);
		if (pstr) free(pstr);
		if (datafmt) free(datafmt);
		if (p6) free(p6);
		fclose(stream);
		return 0;
	}
	else 
	{
		fprintf(stderr,"	成功!  ");
		fprintf(stderr,"插入%d条数据!",recnum);
		if (pstr) free(pstr);
		if (datafmt) free(datafmt);
		if (p6) free(p6);
		fclose(stream);
		return 1;
	}

}

int CDbOperate::do_dfhis()
{
	int retcode=0;
	int serverno;
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	int i,j1,j2,ptcnt;
	int changeycnum,changekwhnum;	
	int dataYM,datayear,datamon;
	CHECK_VALUE *change_yc,*change_kwh;
	void *pt;

	serverno=get_serverno();
	if (serverno<1||serverno>4)
	{
		fprintf(stderr,"\n		未指定服务器!或服务器不存在!");
		fprintf(stderr,"\n		请用 dbmt check help 帮助。");
		return 0;
	}
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;
	
	for(i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-YM",3)==0)
				dataYM=atoi(argv[i]+3);
	}

	if(dataYM<=0)
	{
		fprintf(stderr,"\n命令格式不对，未提供日期。请用 dbmt in help 帮助。\n");
		return 0;
	}
	datayear=dataYM/100;
	datamon=dataYM%100;
	if(datayear<1995 || datayear>2999 || datamon<1 || datamon>12)
	{
		fprintf(stderr,"\n提供的日期不对。请用 dbmt in help 帮助。\n");
		return 0;
	}


	/////////////////////////////////////////////////////////////
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"遥测关系对应表");
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"df")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
		if(strcmp(datafmt[i].name,"sac")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 1;
			j2=i;
		}
	}
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<=0) return FALSE;

	changeycnum=func.ret_roxnum;
	change_yc = (CHECK_VALUE *) malloc (sizeof(CHECK_VALUE)*func.ret_roxnum);
	memset(change_yc,0,sizeof(CHECK_VALUE)*func.ret_roxnum);
	
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,change_yc[i].name);
		dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,change_yc[i].devname);
	}


	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"电度关系对应表");
	strcpy(func.dbname,"modeldb");
	if(dbsingle.GetTableInfo(&func,&datafmt)==CS_FAIL)	
		return FALSE;

	for (i=0;i<func.colnum;i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;
		func.col[i].order  = -1;
		if(strcmp(datafmt[i].name,"df")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 0;
			j1=i;
		}
		if(strcmp(datafmt[i].name,"sac")==0)
		{
			func.col[i].select = YES;
			func.col[i].order_d = 1;
			j2=i;
		}
	}
	func.func=SEL_RECORDERS;
	if ((dbsingle.SelectRcds(&func,datafmt,&pt))!=CS_SUCCEED)
		return FALSE;
	if (func.ret_roxnum<0) return FALSE;

	changekwhnum=func.ret_roxnum;
	change_kwh = (CHECK_VALUE *) malloc (sizeof(CHECK_VALUE)*func.ret_roxnum);
	memset(change_yc,0,sizeof(CHECK_VALUE)*func.ret_roxnum);
	
	for(i=0;i<func.ret_roxnum;i++)
	{
		dbsingle.GetData(i,&func,datafmt+j1,pt,ptcnt,change_kwh[i].name);
		dbsingle.GetData(i,&func,datafmt+j2,pt,ptcnt,change_kwh[i].devname);
	}


	if (pt) 
	{
		free (pt);
		pt=NULL;
	}
	if (datafmt) 
	{
		free (datafmt);
		datafmt=NULL;
	}
	dbsingle.Close();

	//打开东方电子的历史数据库
	retcode=db_start(serverno,DF_HIS);
	if (retcode!=CS_SUCCEED) 
	{
		fprintf(stderr,"\n东方电子的历史数据库打开失败。\n");
		return retcode;
	}
	char filename[255];
	int mon_num;
	{
		memset(&func,0,sizeof(FUNC_STR));
		sprintf(func.dbname,"xopenshdb");

		// data199901 -> file data199901
		sprintf(func.table,"data%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"data%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// pdata199901 -> file pdata199901
		sprintf(func.table,"pdata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"pdata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// daydata199901 -> daydata199901
		sprintf(func.table,"daydata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daydata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// daypdata199901 ->daypdata199901
		sprintf(func.table,"daypdata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daypdata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// mondata1999 -> file mondata199901,mondata199902...
		sprintf(func.table,"mondata%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"mondata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		// monpdata1999 -> file monpdata199901,monpdata199902...
		sprintf(func.table,"monpdata%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monpdata%04d%02d",dataYM/100,dataYM%100);
		realize_out_file(&func,filename);

		dbsingle.Close();
	}	

	//开始数据导入
	retcode=db_start(serverno);
	if (retcode!=CS_SUCCEED) return retcode;

	{
		memset(&func,0,sizeof(FUNC_STR));
		sprintf(func.dbname,"hisdb");

		//  file data199901 -> yc199901
		sprintf(func.table,"yc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"data%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_yc,changeycnum,-1);
		remove(filename);

		//  file pdata199901 -> kwh199901
		sprintf(func.table,"kwh%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"pdata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_kwh,changekwhnum,-1);
		remove(filename);

		//  file daydata199901 -> dayyc199901
		sprintf(func.table,"dayyc%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daydata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_yc,changeycnum,-1);
		remove(filename);

		//  file daypdata199901 -> daykwh199901
		sprintf(func.table,"daypdata%04d%02d",dataYM/100,dataYM%100);
		sprintf(func.isql,"select * from %s ",func.table);
		sprintf(filename,"daypdata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_kwh,changekwhnum,-1);
		remove(filename);

		// file mondata199901 -> monthyc1999
		sprintf(func.table,"monthyc%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"mondata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_yc,changeycnum,-1);
		remove(filename);

		// file monpdata199901 -> monthkwh1999
		sprintf(func.table,"monthkwh%04d",dataYM/100);
		mon_num=(datayear-1970)*12+datamon;
		sprintf(func.isql,"select * from %s where sdate=%d ",func.table,mon_num);
		sprintf(filename,"monpdata%04d%02d",dataYM/100,dataYM%100);
		realize_in_file_ex(&func,filename,change_kwh,changekwhnum,-1);
		remove(filename);

		dbsingle.Close();
	}

	if (change_yc)
	{
		free(change_yc);
		change_yc = NULL;
	}
	
	if (change_kwh)
	{
		free(change_kwh);
		change_kwh = NULL;
	}
	return retcode;

}
//201508 add ends

int
main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				printf("dbmt, 1.1");
				return 0;
			}
		}
	}
	NetDbg dbg;
	dbg.RegProc("数据库工具");

//#if defined(__Edition2001)
	if(dbsingle.isDBType(DBTYPE_SYBASE))  //Sybase数据库
	{
		dbtype = DBTYPE_SYBASE;
	}
	else if(dbsingle.isDBType(DBTYPE_ORACLE))  //oracle数据库
	{
		dbtype = DBTYPE_ORACLE;
	}
	else if(dbsingle.isDBType(DBTYPE_DMSQL))  //达梦数据库
	{
		dbtype = DBTYPE_DMSQL;
	}
	else if(dbsingle.isDBType(DBTYPE_SQLSERVER)) //   SQLSERVER
	{
		dbtype = DBTYPE_SQLSERVER;
	}
	else if(dbsingle.isDBType(DBTYPE_MYSQL)) //   SQLSERVER
	{
		dbtype = DBTYPE_MYSQL;
	}
//#endif

	int i;
	CDbOperate dbOperate;

	dbOperate.argc=argc;
	if(dbOperate.argc>MAXPARA) dbOperate.argc=MAXPARA;
	for (i=0;i<dbOperate.argc;i++) strncpy(dbOperate.argv[i],argv[i],MAXPARALEN);


	dbOperate.DoOperate();

	dbg.UnRegProc();

	printf("\n");
	return 1;

}
