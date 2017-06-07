#include <wchar.h>
#include  <stdarg.h>

#include  "scadatime.h"

#include  "net/netapi.h"
#include  "scd/cmddef.h"

#ifdef __unix
#include <unistd.h>
#endif

#ifdef __linux
#include <wctype.h>
#endif


#include "upctrl.h"

#include "comnstack.h"
#include "scd/comnlink.h"

#ifdef WIN32
#define  strcasecmp    stricmp
#endif

#define  LITERAL_INITADDR    (-0x6ffffff0)

#define  REPLACEADDR_INITADDR	(-0x7ffffff0)

extern void PrintInfoLine(const char *buf);

//关键字列表
USERPROCKEYSTRU  userprockeystru[]={
	{"IF",WT_IF},
	{"ELSE",WT_ELSE},
	{"DATAVAR",WT_DATAVAR},
	{"STRING",WT_STRVAR},
//	{"GOTO",WT_GOTO},
	{"PROC",WT_PROC},
	{"FOR",WT_FOR},
	{"RETURN",WT_RETURN},
	{"break",WT_BREAK},
	{"continue",WT_CONTINUE},

	{"RSQL",(int)obj_rsql},
	{"APPEVENT",(int)obj_appent},
	{"RDBTABLE",(int)obj_rdbtable},
	{"YKQUEUE",(int)obj_ykqueue},
	{"HISDATA",(int)obj_hisdata},
	{"HISTABLE",(int)obj_histable},
	{"FILEOP",(int)obj_fileop},
};


//函数列表
FUNCTIONTAB  interfunctab[]={
	{"SQRT",OBJECT_NULL,FUNC_SQRT,doubledata,complete,1,doubledata},
	{"CBRT",OBJECT_NULL,FUNC_CBRT,doubledata,complete,1,doubledata},
	{"COS",OBJECT_NULL,FUNC_COS,doubledata,complete,1,doubledata},
	{"SIN",OBJECT_NULL,FUNC_SIN,doubledata,complete,1,doubledata},
	{"TAN",OBJECT_NULL,FUNC_TAN,doubledata,complete,1,doubledata},
	{"ACOS",OBJECT_NULL,FUNC_ACOS,doubledata,complete,1,doubledata},
	{"ASIN",OBJECT_NULL,FUNC_ASIN,doubledata,complete,1,doubledata},
	{"ATAN",OBJECT_NULL,FUNC_ATAN,doubledata,complete,1,doubledata},
	{"EXP",OBJECT_NULL,FUNC_EXP,doubledata,complete,1,doubledata},
	{"LOG",OBJECT_NULL,FUNC_LOG,doubledata,complete,1,doubledata},
	{"LOG10",OBJECT_NULL,FUNC_LOG10,doubledata,complete,1,doubledata},
	{"ABS",OBJECT_NULL,FUNC_ABS,doubledata,complete,1,doubledata},
	{"POW",OBJECT_NULL,FUNC_POW,doubledata,complete,2,doubledata,doubledata},
	{"MAX",OBJECT_NULL,FUNC_MAX,doubledata,complete,2,doubledata,doubledata},
	{"MIN",OBJECT_NULL,FUNC_MIN,doubledata,complete,2,doubledata,doubledata},
	{"INT",OBJECT_NULL,FUNC_INT,doubledata,complete,1,doubledata},
	{"INT45",OBJECT_NULL,FUNC_INT45,doubledata,complete,1,doubledata},
	{"RAND",OBJECT_NULL,FUNC_RAND,doubledata,complete,1,doubledata},

	{"GETDATE",OBJECT_NULL,FUNC_GETDATE,nulltype,complete,3,doubledata,doubledata,doubledata},
	{"GETTIME",OBJECT_NULL,FUNC_GETTIME,nulltype,complete,3,doubledata,doubledata,doubledata},
	{"CALDAYS",OBJECT_NULL,FUNC_CALDAYS,doubledata,complete,6,doubledata,doubledata,doubledata,doubledata,doubledata,doubledata},
	{"CALSECS",OBJECT_NULL,FUNC_CALSECS,doubledata,complete,12,doubledata,doubledata,doubledata,doubledata,doubledata,doubledata,
				doubledata,doubledata,doubledata,doubledata,doubledata,doubledata},
	{"INCDATE",OBJECT_NULL,FUNC_INCDATE,nulltype,complete,4,doubledata,doubledata,doubledata,doubledata},
	{"getabssec",OBJECT_NULL,FUNC_GETABSSEC,nulltype,complete,1,doubledata},
	{"clocktosec",OBJECT_NULL,FUNC_CLKTOSEC,nulltype,complete,7,doubledata,doubledata,
								doubledata,doubledata,doubledata,doubledata,doubledata},
	{"sectoclock",OBJECT_NULL,FUNC_SECTOCLK,nulltype,complete,7,doubledata,doubledata,
								doubledata,doubledata,doubledata,doubledata,doubledata},

	{"PRINT",OBJECT_NULL,FUNC_PRINT,nulltype,nocheck},



	{"SLEEP",OBJECT_NULL,FUNC_SLEEP,nulltype,complete,1,doubledata},
	{"SLEEPMS",OBJECT_NULL,FUNC_SLEEPMS,nulltype,complete,2,doubledata,doubledata},
	{"CALLPROC",OBJECT_NULL,FUNC_CALLPROC,doubledata,morepara,1},
	{"EXECSHELL",OBJECT_NULL,FUNC_EXECSHELL,nulltype,complete,1,stringdata},


	//取正数
	{"pos",OBJECT_NULL,FUNC_POS,doubledata,complete,1,doubledata},
	//取负数
	{"neg",OBJECT_NULL,FUNC_NEG,doubledata,complete,1,doubledata},

	{"ATOI",OBJECT_NULL,FUNC_ATOI,doubledata,complete,1,stringdata},
	{"ITOA",OBJECT_NULL,FUNC_ITOA,stringdata,complete,3,doubledata,doubledata,stringdata},
	

	{"ismainserver",OBJECT_NULL,FUNC_ISMAINSERVER,doubledata,complete,1,stringdata},

	{"startpdr",OBJECT_NULL,FUNC_STARTPDR,doubledata,complete,13,
				doubledata,doubledata,doubledata,doubledata,doubledata,doubledata,
				doubledata,stringdata,doubledata,doubledata,stringdata,doubledata,doubledata},

	{"encodeobjname",OBJECT_NULL,FUNC_ENCODEOBJNAME,doubledata,complete,5,doubledata,stringdata,
				stringdata,doubledata,stringdata},
	{"encodeobjtype",OBJECT_NULL,FUNC_ENCODEOBJTYPE,doubledata,complete,2,stringdata,doubledata},
	{"decodeobjname",OBJECT_NULL,FUNC_DECODEOBJNAME,doubledata,complete,5,stringdata,doubledata,stringdata,
				stringdata,doubledata},

	{"getfield_name",OBJECT_NULL,FUNC_GETFIELD_NAME,doubledata,complete,3,stringdata,stringdata,stringdata},
	{"getfieldstr_name",OBJECT_NULL,FUNC_GETFIELDSTR_NAME,stringdata,complete,3,stringdata,stringdata,stringdata},
	{"putfield_name",OBJECT_NULL,FUNC_PUTFIELD_NAME,doubledata,complete,4,stringdata,stringdata,stringdata,doubledata},
	{"putfieldstr_name",OBJECT_NULL,FUNC_PUTFIELDSTR_NAME,doubledata,complete,4,stringdata,stringdata,stringdata,stringdata},
	
	{"openmemtable",(int)obj_rdbtable,FUNC_OPENMEMTAB,doubledata,complete,1,stringdata},
	{"closememtable",(int)obj_rdbtable,FUNC_CLOSEMEMTAB,nulltype,complete,0},
	{"countmemrcd",(int)obj_rdbtable,FUNC_COUNTMEMRCD,doubledata,complete,0},
	{"bindmemfield",(int)obj_rdbtable,FUNC_BINDMEMFIELD,doubledata,morepara,2,doubledata,stringdata},
	{"updmemfield",(int)obj_rdbtable,FUNC_UPDMEMFIELD,doubledata,morepara,2,doubledata,stringdata},
	
	{"getapent",(int)obj_appent,FUNC_GETAPEVENT,doubledata,complete,0},
	{"checksg",(int)obj_appent,FUNC_CHECKSG,doubledata,complete,1,stringdata},
	{"checkol",(int)obj_appent,FUNC_CHECKOL,doubledata,complete,4,stringdata,stringdata,doubledata,doubledata},
	{"checkchg",(int)obj_appent,FUNC_CHECKCHG,doubledata,complete,3,stringdata,stringdata,doubledata},
	{"checkodd",(int)obj_appent,FUNC_CHECKODD,doubledata,complete,2,stringdata,doubledata},
	{"adduserent",OBJECT_NULL,FUNC_ADDUSERENT,doubledata,morepara,14,doubledata,doubledata,
			doubledata,doubledata,doubledata,stringdata,stringdata,stringdata,stringdata,
			stringdata,stringdata,stringdata,stringdata,stringdata},
	
	{"checkykcond",OBJECT_NULL,FUNC_CHKYKCONDITION,doubledata,complete,2,stringdata,doubledata},
	{"ykpreset",(int)obj_ykqueue,FUNC_YKPRESET,doubledata,complete,2,stringdata,doubledata},
	{"waitykchk",(int)obj_ykqueue,FUNC_WAITYKCHK,doubledata,complete,1,stringdata},
	{"ykexe",(int)obj_ykqueue,FUNC_YKTAKE,doubledata,complete,2,stringdata,doubledata},
	{"ykcancel",(int)obj_ykqueue,FUNC_YKCANCEL,doubledata,complete,2,stringdata,doubledata},
	{"waitykresult",(int)obj_ykqueue,FUNC_WAITYKRES,doubledata,complete,1,stringdata},
	
	{"rdbselectfrom",(int)obj_rsql,FUNC_RSQL_SELFROM,nulltype,complete,1,stringdata},
	{"rdbselectfield",(int)obj_rsql,FUNC_RSQL_SELFIELD,nulltype,complete,1,stringdata},
	{"rdbwhere",(int)obj_rsql,FUNC_RSQL_WHERE,nulltype,morepara,1,stringdata},
	{"rdbselect",(int)obj_rsql,FUNC_RSQL_SELECT,doubledata,complete,0},
	{"rdbupdtable",(int)obj_rsql,FUNC_RSQL_UPDTAB,nulltype,complete,1,stringdata},
	{"rdbupdfield",(int)obj_rsql,FUNC_RSQL_UPDFIELD,nulltype,morepara,1,stringdata},
	{"rdbupdate",(int)obj_rsql,FUNC_RSQL_UPDATE,doubledata,complete,0},
	{"rdbcountrcd",(int)obj_rsql,FUNC_RSQL_COUNTRCD,doubledata,complete,0},
	{"rdbbindfield",(int)obj_rsql,FUNC_RSQL_BINDFIELD,doubledata,morepara,2,doubledata,stringdata},
	{"curtimetype",OBJECT_NULL,FUNC_CURTIMETYPE,doubledata,complete,1,doubledata},
	{"timetype",OBJECT_NULL,FUNC_TIMETYPE,doubledata,complete,6,doubledata,doubledata,doubledata,doubledata,doubledata,doubledata},


	// getdata 返回个数, 参数: 电度值标，表类型(分钟表，天表，月表)，年，月，日，点名，标志
	{"readdata",(int)obj_hisdata,FUNC_READHISDATA,doubledata,complete,7,doubledata,doubledata,
				doubledata,doubledata,doubledata,stringdata,doubledata},
	// 参数: 记录号，time，data,flag
	{"binddata",(int)obj_hisdata,FUNC_BINDHISDATA,doubledata,complete,4,doubledata,doubledata,
				doubledata,doubledata},
	// 参数: 记录号，odate,otime,oflag
	{"bindodata",(int)obj_hisdata,FUNC_BINDHISODATA,doubledata,complete,4,doubledata,doubledata,
				doubledata,doubledata},
	// upddata 参数: 电度值标，表类型(分钟表，天表，月表)，年，月，日，时，分，点名，值，标志,
	//				出现日期，出现时间，出现标志
	{"upddata",(int)obj_hisdata,FUNC_UPDHISDATA,doubledata,complete,13,doubledata,doubledata,
				doubledata,doubledata,doubledata,doubledata,doubledata,stringdata,doubledata,doubledata,
				doubledata,doubledata,doubledata},
	// deldata 参数: 电度值标，表类型(分钟表，天表，月表)，年，月，日，时，分，点名，标志
	{"deldata",(int)obj_hisdata,FUNC_DELHISDATA,doubledata,complete,9,doubledata,doubledata,
				doubledata,doubledata,doubledata,doubledata,doubledata,stringdata,doubledata},
	// insertdata 参数: 电度值标，表类型(分钟表，天表，月表)，年，月，日，时，分，点名，值，标志,
	//				出现日期，出现时间，出现标志
	{"insertdata",(int)obj_hisdata,FUNC_INSERTHISDATA,doubledata,complete,13,doubledata,doubledata,
				doubledata,doubledata,doubledata,doubledata,doubledata,stringdata,doubledata,doubledata,
				doubledata,doubledata,doubledata},

	// readmindata 返回个数, 参数: 电度值标，年，月，日，时，分，点名，标志，返回数据，返回数据标志
	{"readmindata",(int)obj_hisdata,FUNC_READHISMINDATA,doubledata,complete,10,doubledata,doubledata,
				doubledata,doubledata,doubledata,doubledata,stringdata,doubledata,doubledata,doubledata},

	////////////////////  历史表函数 /////////////////////////
	{"opentable",(int)obj_histable,FUNC_HISTABLE_OPEN,doubledata,morepara,2,stringdata,stringdata},
	{"closetable",(int)obj_histable,FUNC_HISTABLE_CLOSE,nulltype,complete,0},
	{"countrcd",(int)obj_histable,FUNC_HISTABLE_COUNT,doubledata,complete,0},
	{"bindfield",(int)obj_histable,FUNC_HISTABLE_BIND,doubledata,morepara,2,doubledata,stringdata},

	{"updrecord",(int)obj_histable,FUNC_HISTABLE_UPDATE,doubledata,morepara,4,stringdata,stringdata,stringdata,stringdata},
	{"delrecord",(int)obj_histable,FUNC_HISTABLE_DELETE,doubledata,complete,3,stringdata,stringdata,stringdata},
	{"insertrecord",(int)obj_histable,FUNC_HISTABLE_INSERT,doubledata,morepara,3,stringdata,stringdata,stringdata},

	///////////////////  历史库通用函数 //////////////////////////
	// 历史库SQL语句
	{"hissqlcmd",OBJECT_NULL,FUNC_HISSQLSTATEMENT,doubledata,complete,2,stringdata,doubledata},

	///////////////////  文件操作  ///////////////////////////
	{"FOPEN",(int)obj_fileop,FUNC_FILEOP_FOPEN,doubledata,complete,2,stringdata,stringdata},
	{"FCLOSE",(int)obj_fileop,FUNC_FILEOP_FCLOSE,doubledata,complete,0},
	{"FPRINT",(int)obj_fileop,FUNC_FILEOP_FPRINT,doubledata,morepara,0},
	{"FGETWORD",(int)obj_fileop,FUNC_FILEOP_FGETWORD,doubledata,morepara,0},
};  


static int dbfield_second;
static int replace_addr=REPLACEADDR_INITADDR;

typedef struct {
	int incstno;
	int forendno;
}  FOR_CTRL_PARA;

ComnStack<FOR_CTRL_PARA>  forloopstack(20);

typedef struct {
	int replaceaddr;
	int realaddr;
} REPLACEADDR_STRU;

ComnLinkTable<REPLACEADDR_STRU>  replaceaddrlink;

/////////////////////////////////////////
//过程控制语言编译类
UserProc::UserProc(void)
{
	errorflag=0;
    expoint=-1;
    srcfp=NULL;
    outfp=NULL;
    lineno=0;
    instruno=0;
    errornums=0;
    vartabroot=NULL;
    literaltabroot=NULL;
    inparanums=0;
    inparastack.stackp=0;
    tempdoublenums=TEMPDOUBLENUMS;
    tempstrnums=TEMPSTRINGNUMS;
    tempdoubleoff=0;
    tempstringoff=TEMPDOUBLENUMS*sizeof(double);
    dataunitp=tempstringoff+tempstrnums*MAXSTRINGLEN;
    literalp=LITERAL_INITADDR;
    predict.putpoint=predict.getpoint=0;
    synxstack.stackp=0;
    prochead.inparanums=0;
    codeoffset=(int)((sizeof(PROC_HEAD)+799)/800) * 800;
    funcparastack.stackp=0;
    init_tempdouble();
    init_tempstring();
}

UserProc::~UserProc(void)
{
    if(srcfp!=NULL)fclose(srcfp);
    if(outfp!=NULL)fclose(outfp);
    free_vartab();
    free_literaltab();
}


//////////////////////////////////////////
int  UserProc::opensrcfile(char *srcfname)
{
    strcpy(srcfilename,srcfname);
    srcfp=fopen(srcfname,"r");
    if(srcfp==NULL) 
    {
		PrintInfoLine("源文件打不开\r\n");
		return(USERPROCERR);
    }
    if(feof(srcfp)){
        PrintInfoLine("源文件无内容\r\n");
        return(USERPROCERR);    
    }
    else 
    {
        expoint=0;
        fgets(linestr,MAXLINELEN,srcfp);
        lineno=1;
    }
    return(USERPROCOK);
}

int  UserProc::openoutfile(char *outfname)
{
    strcpy(outfilename,outfname);
    outfp=fopen(outfname,"wb");
    if(outfp==NULL) 
    {
		PrintInfoLine("目标文件打不开\r\n");
		return(USERPROCERR);
    }
    else
    {
        return(USERPROCOK);
    }
}

int  UserProc::writeoutbuff(void *buff,int offset,int len)
{
    fseek(outfp,offset,SEEK_SET);
    fwrite(buff,1,len,outfp);
    return(USERPROCOK);
}


int  UserProc::readoutbuff(void *buff,int offset,int len)
{
    fseek(outfp,offset,SEEK_SET);
    fread(buff,1,len,outfp);
    return(USERPROCOK);
}

/////////////////////////////////////
//读目标文件新行
void   UserProc::read_newline()
{
    expoint=0;
    linestr[0]=SYM_EOF;
    if(feof(srcfp)) 
    {
		
    }
    else fgets(linestr,MAXLINELEN,srcfp);
    lineno++;
}

//读下一个字符
char  UserProc::get_nextsymbol()
{
    if(expoint>0) 
    {
		if(linestr[expoint-1]=='\n' || linestr[expoint-1]==NULL)read_newline();
    }
	
    if(linestr[expoint]==SYM_EOF)return(linestr[expoint]);
    else if(linestr[expoint]=='\n' || linestr[expoint]==NULL) 
    {
		expoint++;
		return('\n');
    }
    else return(linestr[expoint++]);
}

//还原前一读人字符
void  UserProc::restore_symbol()
{
    if(expoint>0)expoint--;
}

void  UserProc::skip_space()
{
    while(iswspace(linestr[expoint])||(linestr[expoint]=='\t'))expoint++;
}


void  UserProc::skip_spaceline()
{
	int   cursymbol;
	
	do {
		skip_space();
		cursymbol=get_nextsymbol();
	} while (cursymbol=='\n' || iswspace(cursymbol) || (cursymbol=='\t'));
	restore_symbol();
}

//SKIP注释：“//”起头
void  UserProc::skip_comment()
{
	int   cursymbol;
	
	skip_spaceline();
	
	do {
		skip_space();
		cursymbol=get_nextsymbol();
		if(cursymbol=='/') {
			cursymbol=get_nextsymbol();
			if(cursymbol=='/') {
				read_newline();
				skip_spaceline();
			}
			else {
				restore_symbol();
				break;
			}
		}
		else break;
	} while(1);
	restore_symbol();
}



//扫描数字
int   UserProc::scandigit(void)
{
	int i=0;
	do {
		takeword[wordpoint]=get_nextsymbol();
		if(takeword[wordpoint]=='.')	i++;
		if(i>=2)	{
			userprocerror(USERPROCERR_WORD,"数字格式错误");
			break;
		}
		if(iswdigit(takeword[wordpoint]) || takeword[wordpoint]=='.')wordpoint++;
		else 
		{
			if(iswalpha(takeword[wordpoint]) || ishanzhi(takeword[wordpoint]))
				userprocerror(USERPROCERR_WORD,"数字不能紧跟字母");
			restore_symbol();
			takeword[wordpoint]='\0';
			return(WT_DIGIT);
		}
	} while(1);
	return(WT_DIGIT);
}

//扫描字符串
int   UserProc::scanstring(void)
{
	char  tmpnextsymbol;

	do {
		takeword[wordpoint]=get_nextsymbol();
		if(takeword[wordpoint]!='"' && takeword[wordpoint]!='\n' && takeword[wordpoint]!=SYM_EOF) {
			if(takeword[wordpoint]=='\\') {
				tmpnextsymbol=get_nextsymbol();
				if(tmpnextsymbol=='n') takeword[wordpoint]='\n';
				else  if(tmpnextsymbol=='r') takeword[wordpoint]='\r';
				else  restore_symbol();
			}
			wordpoint++;
			if(wordpoint>=MAXWORDLEN) {
				userprocerror(USERPROCERR_WORD,"字符串太长，必须小于256字符");
				takeword[wordpoint-1]='\0';
				return(WT_STRING);
            }
		}
		else break;
	} while(1);
	if(takeword[wordpoint]=='"') {
		takeword[wordpoint]='\0';
		return(WT_STRING);
	}
	else {
		userprocerror(USERPROCERR_WORD,"缺少双引号");
		takeword[wordpoint]='\0';
		return(WT_STRING);
	}
}

//查找关键字
int   UserProc::searchkeyname(char *name)
{
	int  i;
	
	for(i=0;i<sizeof(userprockeystru)/sizeof(USERPROCKEYSTRU);i++) {
		if(strcasecmp(name,userprockeystru[i].keyname)==0)return(userprockeystru[i].wordtype);
	}
	return(-1);
}

//扫描各种名字：数据库域名，函数名，标号，变量名

int   UserProc::scanname_raw(char *wordbuf)
{
    int wordp=0;
	do {
		wordbuf[wordp]=get_nextsymbol();
		if(ishanzhi(wordbuf[wordp]) || iswalnum(wordbuf[wordp]) 
			|| wordbuf[wordp]=='_' || wordbuf[wordp]=='@' || wordbuf[wordp]=='#')wordp++;
		else if(dbfield_second==1  && wordbuf[wordp]=='-')wordp++;
		else break;
	} while(1);
	restore_symbol();
	wordbuf[wordp]='\0';
	return(WT_NAME);
}

int   UserProc::scanname(void)
{
    int ret;
	char  tmpsymbol;
	int  varaddr;
	VARSTYLE   varstyle;
	DATATYPE   datatype;


	dbfield_second=0;
	ret=scanname_raw(takeword+wordpoint);
	wordpoint=strlen(takeword);

	tmpsymbol=get_nextsymbol();

	if(tmpsymbol!='.') { 
		restore_symbol(); 
		if((ret=searchkeyname(takeword))>=0)return(ret);
		return(WT_NAME);
	}
	ret=search_vartab(takeword,&varaddr,&varstyle,&datatype,NULL,1);
	if(ret==USERPROCOK &&  DATATYPE_IS_OBJECT(datatype)) { restore_symbol(); return(WT_NAME);}

	takeword[wordpoint]=tmpsymbol;
	wordpoint++;
	dbfield_second=1;
	scanname_raw(takeword+wordpoint);
	tmpsymbol=get_nextsymbol();
	if(tmpsymbol!='.') {
		userprocerror(USERPROCERR_SYNX,"数据库域格式错误");
		return(WT_DBFIELD);
	}

	wordpoint=strlen(takeword);
	takeword[wordpoint]=tmpsymbol;
	wordpoint++;
	dbfield_second=2;
	scanname_raw(takeword+wordpoint);
	return(WT_DBFIELD);
}



//解释读入字符，并根据读人字符扫描下一数字/字符串/变量名
int   UserProc::get_wordinline(void)
{
    int   cursymbol;
	
    wordpoint=0;
    skip_space();
    skip_comment();
    skip_space();
    cursymbol=get_nextsymbol();
    if(cursymbol==SYM_EOF) return(WT_EOF);
    else if(cursymbol=='(') return(WT_LBRACKET);
    else if(cursymbol==')') return(WT_RBRACKET);
    else if(cursymbol=='{') return(WT_LLBRACKET);
    else if(cursymbol=='}') return(WT_RLBRACKET);
    else if(cursymbol=='[') return(WT_LMBRACKET);
    else if(cursymbol==']') return(WT_RMBRACKET);
    else if(cursymbol=='%') return(WT_MOD);
    else if(cursymbol=='+') return(WT_ADD);
    else if(cursymbol=='-') return(WT_SUB);
    else if(cursymbol=='/') return(WT_DIV);
    else if(cursymbol=='^') return(WT_XOR);
    else if(cursymbol=='~') return(WT_BITNOT);
    else if(cursymbol==',') return(WT_COMMA);
    else if(cursymbol==';') return(WT_SEMIC);
   else if(cursymbol=='.') return(WT_DOT);
    else if(cursymbol=='!') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='=') return(WT_NOTEQU);
		else {
			restore_symbol();
			return(WT_NOT);
		}
	}
    else if(cursymbol=='>') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='>') return(WT_RSHIFT);
		if(cursymbol=='=') return(WT_LARGEEQU);
		else {
			restore_symbol();
			return(WT_LARGE);
		}
	}
    else if(cursymbol=='<') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='<') return(WT_LSHIFT);
		if(cursymbol=='=') return(WT_SMALLEQU);
		else {
			restore_symbol();
			return(WT_SMALL);
		}
	}
    else if(cursymbol=='=') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='=') return(WT_EQUEQU);
		else {
			restore_symbol();
			return(WT_EQU);
		}
	}
    else if(cursymbol=='&') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='&') return(WT_AND);
		else {
			restore_symbol();
			return(WT_BITAND);
		}
	}
    else if(cursymbol=='|') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='|') return(WT_OR);
		else {
			restore_symbol();
			return(WT_BITOR);
		}
	}
    else if(cursymbol=='*') {
		cursymbol=get_nextsymbol();
		if(cursymbol=='*') return(WT_MULMUL);
		else {
			restore_symbol();
			return(WT_MUL);
		}
	}
    else if(iswdigit(cursymbol)) {
        takeword[0]=cursymbol;
        wordpoint=1;
        return(scandigit());
	}
    else if(cursymbol=='"') {
		wordpoint=0;
		return(scanstring());
	}
    else if(cursymbol=='@') {
        takeword[0]=cursymbol;
        wordpoint=1;
        return(scanname());
	}
    else if(ishanzhi(cursymbol) || iswalpha(cursymbol)) {
        takeword[0]=cursymbol;
        wordpoint=1;
        return(scanname());
	}
    userprocerror(USERPROCERR_WORD,"包含不能识别的字符");
    return(-1);
}  

//读人下一词
void  UserProc::get_nextword()
{
    int  tmptype;
	
    if(predict.putpoint==predict.getpoint) 
    {
		while((tmptype=get_wordinline())<0);
		tokentype=predict.predicttype[predict.putpoint]=tmptype;
		strcpy(predict.predictword[predict.putpoint],takeword);
		predict.putpoint++;
		if(predict.putpoint>=MAXPREDICTWORDNUMS)predict.putpoint=0;
		predict.getpoint=predict.putpoint;
		return;
    }
    tokentype=predict.predicttype[predict.getpoint];
    strcpy(takeword,predict.predictword[predict.getpoint]);
    predict.getpoint++;
    if(predict.getpoint>=MAXPREDICTWORDNUMS)predict.getpoint=0;
    return;
}

//还原前一读人词
void  UserProc::restore_word(void)
{
	predict.getpoint--;
	if(predict.getpoint<0)predict.getpoint=MAXPREDICTWORDNUMS-1;
}



//匹配指定字符类型
int   UserProc::match(int  matchtype)
{
	int i=0;
	
	do {
		get_nextword();
		if(tokentype==matchtype)return(USERPROCOK);
		else if(tokentype==WT_EOF)return(-1);
		else if(i==0)userprocerror(USERPROCERR_SYNX,"格式不匹配");
		i++;
	} while(1);
	return(-1);  
}


//匹配指定字符类型
int   UserProc::skip_match(int  matchtype,...)
{
	int i=0;
	int matchnums=0;
	int tmpmatch[20];
	va_list        ap;

	tmpmatch[0]=matchtype;
	matchnums=1;

	va_start(ap,matchtype);

	for(i=1;i<20;i++) {
		if((tmpmatch[i]=va_arg(ap,int))==-1)break;
		matchnums++;
	}
	va_end(ap);


	do {
		get_nextword();
		for(i=0;i<matchnums;i++) {
			if(tokentype==tmpmatch[i])return(USERPROCOK);
			else if(tokentype==WT_EOF)return(-1);
			else if(tokentype==WT_SEMIC)return(-1);
		}
	} while(1);
}


///////////////////////////////////////////////////////////////////////
//定位常量的地址
int UserProc::locate_literaladdr(int instrunums,int literal_startaddr)
{
	int  wrf=0;
	int  instrucode,addr1,addr2,addr3;
	for(int i=0;i<instrunums;i++) {
		wrf=0;
		readinstrument(i,&instrucode,&addr1,&addr2,&addr3);
		if(addr1>=LITERAL_INITADDR && addr1<-100) {addr1=literal_startaddr+(addr1-LITERAL_INITADDR);wrf=1;}
		if(addr2>=LITERAL_INITADDR && addr2<-100) {addr2=literal_startaddr+(addr2-LITERAL_INITADDR);wrf=1;}
		if(addr3>=LITERAL_INITADDR && addr3<-100) {addr3=literal_startaddr+(addr3-LITERAL_INITADDR);wrf=1;}
		if(wrf) {
			writeinstrument(i,instrucode,addr1,addr2,addr3);
		}
	}
	return(USERPROCOK);
}

//定位替换地址

int UserProc::get_replaceaddr(int addr,int *realaddr)
{
	COMN_POSITION  curposition;
	REPLACEADDR_STRU  *srepdata;

	curposition=replaceaddrlink.GetFirstNode();
	while(curposition) {
		srepdata=replaceaddrlink.NodeData(curposition);
		curposition=replaceaddrlink.GetNextNode(curposition);
		if(srepdata->replaceaddr==addr) {
			*realaddr=srepdata->realaddr;
			return(1);
		}
	}
	return(0);
}

int UserProc::locate_replaceaddr(int instrunums)
{
	int  wrf=0;
	int  instrucode,addr1,addr2,addr3,realaddr;
	for(int i=0;i<instrunums;i++) {
		wrf=0;
		readinstrument(i,&instrucode,&addr1,&addr2,&addr3);
		if(addr1>=REPLACEADDR_INITADDR && addr1<LITERAL_INITADDR) {
			if(get_replaceaddr(addr1,&realaddr)) {
				addr1=realaddr;
			}
			else userprocerror(USERPROCERR_SYNX,"内部错误：无法替换地址");
			wrf=1;
		}
		if(addr2>=REPLACEADDR_INITADDR && addr2<LITERAL_INITADDR) {
			if(get_replaceaddr(addr2,&realaddr)) {
				addr2=realaddr;
			}
			else userprocerror(USERPROCERR_SYNX,"内部错误：无法替换地址");
			wrf=1;
		}
		if(addr3>=REPLACEADDR_INITADDR && addr3<LITERAL_INITADDR) {
			if(get_replaceaddr(addr3,&realaddr)) {
				addr3=realaddr;
			}
			else userprocerror(USERPROCERR_SYNX,"内部错误：无法替换地址");
			wrf=1;
		}
		if(wrf) {
			writeinstrument(i,instrucode,addr1,addr2,addr3);
		}
	}
	return(USERPROCOK);
}

//过程控制语言编译：处理过程体和过程头
int UserProc::proc(char *tmpoutfname)
{
	static unsigned char pcout_magicnum[3]={66,10,88};
	int ret,exitprocinsno;

    ret=openoutfile(tmpoutfname);
    if(ret!=USERPROCOK)
    {
        return(USERPROCERR);
    }



	match(WT_PROC); 
	get_nextword();
	if(tokentype==WT_NAME)
		strcpy(prochead.procname,takeword);
	else	{
		userprocerror(USERPROCERR_SYNX,"格式不匹配");
		restore_word();
	}
	procbody();
	encode_procover();
	exitprocinsno=get_curinstruno();
	encode_exitproc();
	
    fclose(outfp);
    outfp=NULL;



    outfp=fopen(tmpoutfname,"r+b");

#if defined(__sun)    // sun unix platform
	prochead.platform = PC_PLATFORM_SUN;
#elif  defined(__unix) && defined(__alpha)  // digit unix platform
	prochead.platform = PC_PLATFORM_ALPHA;
#elif  defined(_AIX)  // IBM AIX platform
	prochead.platform = PC_PLATFORM_AIX;
#elif  defined(__hpux)
	prochead.platform = PC_PLATFORM_HPUX;
#elif defined(__linux)
	prochead.platform = PC_PLATFORM_LINUX;
#else          
	prochead.platform = PC_PLATFORM_WIN32;
#endif

printf("\nprochead.platform=%d\n",prochead.platform);

	memcpy(prochead.magicnum,pcout_magicnum,sizeof(pcout_magicnum));
	prochead.codeoffset=codeoffset;
	prochead.codelen=instruno*sizeof(PCINSTRUMENT);
	prochead.literaloffset=prochead.codeoffset+prochead.codelen;
	prochead.literallen=writeout_literaltab(prochead.literaloffset);
	prochead.exitprocinstruno=exitprocinsno;
	
	dataunitp=((int)(dataunitp+15)/16) * 16;

	heaplen=dataunitp+16*16;

	prochead.heaplen=heaplen;

	locate_literaladdr(instruno,heaplen);
	locate_replaceaddr(instruno);

	writeoutbuff((void *)&prochead,0,sizeof(PROC_HEAD));

	fclose(outfp);
    outfp=NULL;

	return(USERPROCOK);
}

//读过程输入参数
int UserProc::read_procinpara(void)
{
	DATATYPE  tmpdatatype;

	get_nextword();
	if(tokentype!=WT_LBRACKET) 	restore_word();
	else {	
		do {
			get_nextword();
			if(tokentype==WT_DATAVAR || tokentype==WT_STRVAR || IS_OBJECT_KEY(tokentype)){
				if(tokentype==WT_DATAVAR)tmpdatatype=doubledata;
				else if(tokentype==WT_STRVAR)tmpdatatype=stringdata;
				else tmpdatatype=(DATATYPE)tokentype;

				get_nextword();  
				if(tokentype==WT_NAME) {
						push_inparastack(takeword,tmpdatatype);
						get_nextword();
						if(tokentype!=WT_COMMA)	break;
				}
				else {
						userprocerror(USERPROCERR_SYNX,"格式不匹配");
						break;
				}
			}
			else break;
		} while(1);
		restore_word();
		match(WT_RBRACKET);
	}
	return(0);
}

//处理过程体：语法分析和中间代码生成
int UserProc::procbody(void)
{
	read_procinpara();
	encode_inpara();
	match(WT_LLBRACKET);
	do {
		statementlist();
		get_nextword();
		if(tokentype!= WT_RLBRACKET && tokentype!=WT_EOF) {
			restore_word();
		}
		else break;
	} while(1);
	if(tokentype==WT_EOF)userprocerror(USERPROCERR_SYNX,"缺 } ");
	return(USERPROCOK);
}

//语句列表
int UserProc::statementlist(void)
{
	get_nextword();
	switch(tokentype) {
	case  WT_LLBRACKET:
		do {
			statement();
			get_nextword();
			if(tokentype!= WT_RLBRACKET && tokentype!=WT_EOF) {
				restore_word();
			}
            else break;
		} while(1);
		if(tokentype==WT_EOF)userprocerror(USERPROCERR_SYNX,"缺 } ");
		break;
	default:
		restore_word();
		statement();
		break;
	}
	return(USERPROCOK);
}


//
int UserProc::free_temp_result(void)
{
	DATATYPE freedatatype;
	ADDRTYPE freeaddrtype;
	int freetmpaddr;

	if(pop_synxstack(&freedatatype,&freeaddrtype,&freetmpaddr)==USERPROCOK) {
		if(freeaddrtype==tempvar)free_tempdouble(freetmpaddr);
		else if(freeaddrtype==tempoffaddr)free_tempdouble(freetmpaddr);	
		else if(freeaddrtype==tempstr)free_tempstring(freetmpaddr);	
	}
	return(USERPROCOK);
}
//
int UserProc::free_temp_result(ADDRTYPE freeaddrtype,int freetmpaddr)
{
	if(freeaddrtype==tempvar)free_tempdouble(freetmpaddr);
	else if(freeaddrtype==tempoffaddr)free_tempdouble(freetmpaddr);	
	else if(freeaddrtype==tempstr)free_tempstring(freetmpaddr);	
	return(USERPROCOK);
}

//语句
int UserProc::statement(void)
{
	DATATYPE datatype;
	ADDRTYPE addrtype;
	int tmpaddr;
	
	get_nextword();
	switch(tokentype) {
	case WT_IF:		//分析IF语句
		int jmpzerono,elseno,jmpno,endifno;
		
		match(WT_LBRACKET);
		expression();
		pop_synxstack(&datatype,&addrtype,&tmpaddr);
		if(datatype!=doubledata)  userprocerror(USERPROCERR_SYNX,"表达式结果类型错误");
		jmpzerono=write_nullinstrument();
		match(WT_RBRACKET);
		statementlist();
		get_nextword();
		jmpno=write_nullinstrument();
		elseno=get_curinstruno();
		if(tokentype==WT_ELSE) {
			statementlist();
		}
		else restore_word();
		endifno=get_curinstruno();
		encode_ifelse(tmpaddr,jmpzerono,jmpno,elseno,endifno);
		
		free_temp_result(addrtype,tmpaddr);
		break;
    case WT_FOR:	//分析FOR语句
		int  expstno,expjmpzerono,expjmpno,incstno,incjmpno;
		int  loopstno,loopjmpno,forendno;
		int for_end_addr,for_inc_addr;
		REPLACEADDR_STRU  for_repstru;
		FOR_CTRL_PARA	  for_ctrlpara;

		match(WT_LBRACKET);
		expression();
		free_temp_result();
		expstno=get_curinstruno();
		match(WT_SEMIC);
		expression();
		pop_synxstack(&datatype,&addrtype,&tmpaddr);
		expjmpzerono=write_nullinstrument();
		expjmpno=write_nullinstrument();
		match(WT_SEMIC);
		incstno=get_curinstruno();
		expression();
		free_temp_result();
		incjmpno=write_nullinstrument();
		match(WT_RBRACKET);
		loopstno=get_curinstruno();
		for_inc_addr=replace_addr++;
		for_end_addr=replace_addr++;

		for_ctrlpara.incstno=for_inc_addr;
		for_ctrlpara.forendno=for_end_addr;
		forloopstack.Push(&for_ctrlpara);

		statementlist();

		loopjmpno=write_nullinstrument();
		forendno=get_curinstruno();
		encode_forstatement(tmpaddr,expstno,expjmpzerono,expjmpno,
			incstno,incjmpno,loopstno,loopjmpno,forendno);

		forloopstack.Pop(NULL);

		for_repstru.replaceaddr=for_inc_addr;
		for_repstru.realaddr=incstno;
		replaceaddrlink.AddNode(&for_repstru);
		for_repstru.replaceaddr=for_end_addr;
		for_repstru.realaddr=forendno;
		replaceaddrlink.AddNode(&for_repstru);

		free_temp_result(addrtype,tmpaddr);
		break;
    default:	
		restore_word();
		simplestatement();
		break;
	}
	return(USERPROCOK);
}

//分析简单语句
int UserProc::simplestatement(void)
{
	int  tmpaddr=0;

	get_nextword();
	if(IS_OBJECT_KEY(tokentype)) {
		varlist((DATATYPE)tokentype);
	}
	else {
		switch(tokentype) {
		case WT_DATAVAR:	//数据变量说明
			varlist(doubledata);
			break;
		case WT_STRVAR:		//字符串变量说明
			varlist(stringdata);
			break;
		case WT_GOTO:		//GOTO语句
			match(WT_NAME);
			match(WT_SEMIC);
			break;
		case WT_RETURN:		//RETURN语句
			expression();
			match(WT_SEMIC);
			encode_return();
			break;
		case WT_BREAK:		//break语句
			match(WT_SEMIC);
			encode_break();
			break;
		case WT_CONTINUE:		//continue语句
			match(WT_SEMIC);
			encode_continue();
			break;
		case WT_DBFIELD:
			dbfield_assignstatement(takeword);
			free_temp_result();
			match(WT_SEMIC);
			break;
		default:
			restore_word();
			expression();
			free_temp_result();
			match(WT_SEMIC);
			break;
		}
	}
	return(USERPROCOK);
}


//变量说明
int  UserProc::varlist(DATATYPE datatype)
{
	int  tmpaddr,nums=1;
	char  tmpvarname[MAXWORDLEN];
	
	do {
		get_nextword();  
		if(tokentype==WT_NAME) {
			nums=1;
			strcpy(tmpvarname,takeword);
			get_nextword();  
			if(tokentype==WT_LMBRACKET) {  // 数组
				get_nextword();
				if(tokentype==WT_DIGIT) {
					nums=atoi(takeword);
				}
				else {
					userprocerror(USERPROCERR_SYNX,"数组必须用数字定义大小");
					break;
				}
				match(WT_RMBRACKET);
			} 
			else restore_word();

			tmpaddr=malloc_vartab(tmpvarname,INTERVAR,datatype,nums);
			get_nextword();
			if(tokentype!=WT_COMMA && tokentype!=WT_SEMIC) {
				userprocerror(USERPROCERR_SYNX,"格式不匹配");
				break;
			}
			if(tokentype==WT_SEMIC) break;
		}
		else {
			userprocerror(USERPROCERR_SYNX,"格式不匹配");
			break;
		}
	} while(1);
	return(USERPROCOK);
}   



//数据库域赋值语句
int UserProc::dbfield_assignstatement(char  *wordbuf)
{
	DATATYPE datatype;
	char  dbfieldname[MAXWORDLEN];
		
	strcpy(dbfieldname,wordbuf);
	datatype=doubledata;
	get_nextword();
	if(tokentype==WT_LBRACKET) {
		get_nextword();
		if(tokentype==WT_STRVAR)datatype=stringdata;
		else if(tokentype==WT_DATAVAR)datatype=doubledata;
		else  userprocerror(USERPROCERR_SYNX,"type error !");
		match(WT_RBRACKET);
	}
	else  restore_word();
		
	get_nextword();
	if(tokentype==WT_EQU) { 
		expression();
		encode_putfieldvalue(dbfieldname,datatype);
	}
	else {
		userprocerror(USERPROCERR_SYNX,"必须是赋值语句");
	}
	return(USERPROCOK);
}

//表达式
int UserProc::expression()
{
	equtal();
	do {
		get_nextword();
		if(tokentype==WT_EQU) {
			equtal();
			encode_equtalop();
		}
		else {
			restore_word();
			break;
		}
	} while(1);
	return(USERPROCOK);
}

//赋值操作
int UserProc::equtal(void)
{
	int  tmpoptype;
	
	relation();
	do {
		get_nextword();
		if(tokentype==WT_AND || tokentype==WT_OR) {
			tmpoptype=tokentype;
			relation();
			encode_logicalop(tmpoptype);
		}
		else {
			restore_word();
			break;
		}
	} while(1);
	return(USERPROCOK);
}


//关系操作
int UserProc::relation()
{
	int  tmpoptype;
	
	bitlevel();
	get_nextword();
	if(tokentype==WT_LARGE || tokentype==WT_SMALL || tokentype==WT_LARGEEQU || 
		tokentype==WT_SMALLEQU || tokentype==WT_NOTEQU || tokentype==WT_EQUEQU) {
		tmpoptype=tokentype;
		bitlevel();
		encode_relationop(tmpoptype);
	}
	else {
		restore_word();
	}
	return(USERPROCOK);
}

//逻辑操作
int UserProc::bitlevel()
{
	int  tmpoptype;
	
	simplexp();
	do {
		get_nextword();
		if(tokentype==WT_BITAND || tokentype==WT_BITOR || tokentype==WT_XOR || 
			tokentype==WT_LSHIFT || tokentype==WT_RSHIFT)  {
			tmpoptype=tokentype;
			simplexp();
			encode_bitop(tmpoptype);
		}
		else {
			restore_word();
			break;
		}
	} while(1);
	return(USERPROCOK);
}

//简单表达式
int UserProc::simplexp()
{
	int tmpoptype;
	
	get_nextword();
	if(tokentype==WT_ADD || tokentype==WT_SUB || tokentype==WT_BITNOT)  {
		tmpoptype=tokentype;
		term();
		encode_signop(tmpoptype);
	}
	else {
		restore_word();
		term();
	}
	do {
		get_nextword();
		if(tokentype==WT_ADD || tokentype==WT_SUB) {
			tmpoptype=tokentype;
			term();
			encode_addingop(tmpoptype);
		}
		else {
			restore_word();
			break;
		}
	} while(1);
	return(USERPROCOK);
}

//术语
int UserProc::term()
{
	int tmpoptype;
	
	factor();
	do {
		get_nextword();
		if(tokentype==WT_MUL || tokentype==WT_DIV || tokentype==WT_MOD) {
			tmpoptype=tokentype;
			factor();
			encode_multiplyop(tmpoptype);
		}
		else {
			restore_word();
			break;
		}
	} while(1);
	return(USERPROCOK);
}

//因子
int UserProc::factor()
{
	int  tmpoptype;
	
	primary();
	do {
		get_nextword();
		if(tokentype==WT_MULMUL) {
			tmpoptype=tokentype;
			primary();
			encode_mulmulop(tmpoptype);
			printf("@@@@@@@@@@@@@@@@@@@@@@@@幂运算@@@@@@@@@@@@@@@@@@@@@@\n");
		}
		else {
			restore_word();
			break;
		}
	} while(1);
	return(USERPROCOK);
}



//原语
int UserProc::primary()
{
	int  ret,tmpaddr=0;
	DATATYPE datatype;
	ADDRTYPE addrtype;
	char   tmpname[MAXWORDLEN];
	char   funcname[MAXWORDLEN];
	
	get_nextword();
	switch(tokentype) {
	case WT_DIGIT:		//数字
        search_literaltab(doubledata,takeword,&tmpaddr);
        push_synxstack(doubledata,literal,tmpaddr);
        break;
	case WT_NAME:
		strcpy(tmpname,takeword);
		get_nextword();
		if(tokentype==WT_LBRACKET) {
			funccall(tmpname,nulltype,0);
		}
		else {
			restore_word();
			ret=proc_array(tmpname);
			if(ret==WTNAME_OBJECT || ret==WTNAME_OBJARRAY) {
				get_nextword();
				pop_synxstack(&datatype,&addrtype,&tmpaddr);
				if(tokentype==WT_DOT) {
					get_nextword();
					if(tokentype==WT_NAME) {
						strcpy(funcname,takeword);
						get_nextword();
						if(tokentype==WT_LBRACKET) {
							funccall(funcname,datatype,tmpaddr);
							free_temp_result(addrtype,tmpaddr);
						}
						else {
							userprocerror(USERPROCERR_SYNX,"函数格式不对");
						}
					}
					else {
						userprocerror(USERPROCERR_SYNX,"函数名非法");
					}
				}
				else restore_word();
			}
		}
        break;
	case WT_DBFIELD:	//数据库域名
		char  dbfieldname[MAXWORDLEN];
		
		strcpy(dbfieldname,takeword);
		datatype=doubledata;
		get_nextword();
		if(tokentype==WT_LBRACKET) {
			get_nextword();
			if(tokentype==WT_STRVAR) {
                datatype=stringdata;
			}
			else if(tokentype==WT_DATAVAR) {
                datatype=doubledata;
			}
			else {
                userprocerror(USERPROCERR_SYNX,"type error !");
			}
			match(WT_RBRACKET);
		}
		else  restore_word();

		encode_getfieldvalue(dbfieldname,datatype);
        break;
	case WT_STRING:		//字符串
        search_literaltab(stringdata,takeword,&tmpaddr);
        push_synxstack(stringdata,literal,tmpaddr);
        break;
	case WT_LBRACKET:	// (
        expression();
        match(WT_RBRACKET);
        break;
    default:
		userprocerror(USERPROCERR_OPERATION,"操作符类型不对");
		skip_match(WT_DIGIT,WT_NAME,WT_DBFIELD,WT_STRING,WT_LBRACKET,-1);
		//restore_word();
		break;
	}
	return(USERPROCOK);
}



// 进一步判断变量、数组
int  UserProc::proc_array(char *namestr)
{
	int ret,wordtype,nums;
	int  varaddr;
	DATATYPE   datatype;
	VARSTYLE   vartype;

	ret=search_vartab(namestr,&varaddr,&vartype,&datatype,&nums);
	if(ret==USERPROCOK) {
		if(DATATYPE_IS_OBJECT(datatype)) wordtype=2; // 对象
		else wordtype=1; // 变量
	}
	else wordtype=0; 

	if(wordtype==2) {// 对象
		get_nextword();
		if(tokentype==WT_LMBRACKET) { //数组
			if(nums<=1)userprocerror(USERPROCERR_SYNX,"非数组变量不能带下标");
			expression();
			encode_array_off(datatype,varaddr,nums);
			match(WT_RMBRACKET);
			return(WTNAME_OBJARRAY);
		}
		else {
			if(nums>1)userprocerror(USERPROCERR_SYNX,"数组变量必须带下标");
			restore_word();
			push_synxstack(datatype,definevar,varaddr);
			return(WTNAME_OBJECT);
		}
	}
	else if(wordtype==1) {// 变量
		get_nextword();
		if(tokentype==WT_LMBRACKET) { //数组
			if(nums<=1)userprocerror(USERPROCERR_SYNX,"非数组变量不能带下标");
			expression();
			encode_array_off(datatype,varaddr,nums);
			match(WT_RMBRACKET);
			return(WTNAME_VARARRAY);
		}
		else {
			if(nums>1)userprocerror(USERPROCERR_SYNX,"数组变量必须带下标");
			restore_word();
			push_synxstack(datatype,definevar,varaddr);
			return(WTNAME_VAR);
		}
 	}
	else {
		userprocerror(USERPROCERR_OPERATION,"操作符类型不对");
		return(WTNAME_OTHER);
	}
}



//函数调用
int UserProc::funccall(char *funcname,DATATYPE objtype,int objaddr)
{
	CALLPARA  callpara;
	DATATYPE resulttype,datatype;
	int  funcno;
        
    callpara.inparanums=0;
    get_nextword();
    if(tokentype!=WT_RBRACKET) {
			restore_word();
			do {
				expression();
				get_nextword();
				if(tokentype==WT_COMMA) {
					datatype=encode_pushpara();
					callpara.inparatype[callpara.inparanums]=datatype;
					callpara.inparanums++;
				}
				else if(tokentype==WT_RBRACKET) {
					datatype=encode_pushpara();
					callpara.inparatype[callpara.inparanums]=datatype;
					callpara.inparanums++;

					funcno=search_functab(funcname,objtype,&callpara,&resulttype);
					encode_callfunc(objtype,objaddr,funcno,&callpara,resulttype);
					break;
				}
				else { 
					restore_word();
					userprocerror(USERPROCERR_OPERATION,"函数输入参数格式不对");
					break;
				}
            } while(1);
	}
	else {
		funcno=search_functab(funcname,objtype,&callpara,&resulttype);
		encode_callfunc(objtype,objaddr,funcno,&callpara,resulttype);
	}
	return(USERPROCOK);
}


//////////////////////////////////
//查找变量
int  UserProc::search_vartab(char *varname,int *varaddr,VARSTYLE *vartype,DATATYPE *datatype,int *nums,int nodisperr)
{
	VARNAMETAB  *tmpvarp;
	
	tmpvarp=vartabroot;
	while(tmpvarp!=NULL) {
		if(strcasecmp(tmpvarp->varname,varname)==0) {
			*varaddr=tmpvarp->addr;
			*vartype=tmpvarp->vartype;
			*datatype=tmpvarp->datatype;
			if(nums)*nums=tmpvarp->nums;
			return(USERPROCOK);
		}
		tmpvarp=tmpvarp->next;
	}
	
	if(!nodisperr) {
		char  errinf[100];
		sprintf(errinf,"%s 无定义",varname);
		userprocerror(USERPROCERR_SYNX,errinf);
	}
	return(-1);
}

//查找变量有无重定义，如无则新增变量表
int  UserProc::malloc_vartab(char *varname,VARSTYLE vartype,DATATYPE  datatype,int nums)
{
	VARNAMETAB  *tmpvarp,*lastvarp;
	char  errinf[100];
	
	tmpvarp=vartabroot;
	while(tmpvarp!=NULL) {
		if(strcasecmp(tmpvarp->varname,varname)==0) {
			sprintf(errinf,"%s 重定义",varname);
			userprocerror(USERPROCERR_SYNX,errinf);
			return(-1);
		}
		lastvarp=tmpvarp;
		tmpvarp=tmpvarp->next;
	}
	
	tmpvarp=(VARNAMETAB  *)malloc(sizeof(VARNAMETAB));
	tmpvarp->next=NULL;
	strcpy(tmpvarp->varname,varname);
	tmpvarp->vartype=vartype;
	tmpvarp->datatype=datatype;
	tmpvarp->nums=nums;

	if(DATATYPE_IS_OBJECT(datatype)) {
		if((dataunitp%MALLOC_OBJECT_LEN)!=0)
			dataunitp=dataunitp+MALLOC_OBJECT_LEN-dataunitp%MALLOC_OBJECT_LEN;
		tmpvarp->addr=dataunitp;
		dataunitp+=(MALLOC_OBJECT_LEN * nums);
		encode_mallocobject(datatype,tmpvarp->addr,nums);
	}
	else if(datatype==doubledata) {
		if((dataunitp%MALLOC_DOUBLEDATA_LEN)!=0)
			dataunitp=dataunitp+MALLOC_DOUBLEDATA_LEN-dataunitp%MALLOC_DOUBLEDATA_LEN;
		tmpvarp->addr=dataunitp;
		dataunitp+=(MALLOC_DOUBLEDATA_LEN * nums);
	}
	else if(datatype==stringdata) {
		tmpvarp->addr=dataunitp;
		dataunitp+=(MALLOC_STRINGDATA_LEN * nums);
	}
	
	if(vartabroot==NULL) {
		vartabroot=tmpvarp;
	}
	else {
		lastvarp->next=tmpvarp;
	}
//printInfoLine("Var %s  add: %d \n",varname,tmpvarp->addr);
	return(tmpvarp->addr);
}


//释放对象变量
int  UserProc::free_object_in_vartab(void)
{
	VARNAMETAB  *tmpvarp,*lastvarp;
	tmpvarp=vartabroot;
	while(tmpvarp!=NULL) {
		encode_freeobject(tmpvarp->datatype,tmpvarp->addr,tmpvarp->nums);
		lastvarp=tmpvarp;
		tmpvarp=tmpvarp->next;
	}
	return(USERPROCOK);
}

//释放变量表
int  UserProc::free_vartab(void)
{
	VARNAMETAB  *tmpvarp,*lastvarp;
	tmpvarp=vartabroot;
	while(tmpvarp!=NULL) {
		lastvarp=tmpvarp->next;
		free(tmpvarp);
		tmpvarp=lastvarp;
	}
	vartabroot=NULL;
	return(USERPROCOK);
}

//////////////////////////////////////////////
//查找值，如无则新增值表
int  UserProc::search_literaltab(DATATYPE literaltype,char *literalstr,int *literaladdr)
{
	LITERALTAB  *tmpliteralp,*lastliteralp;
	double     tmpdouble;
	
	switch(literaltype) {
	case  doubledata:
		tmpdouble=atof(literalstr);
		break;
	}
	
	tmpliteralp=literaltabroot;
	while(tmpliteralp!=NULL) {
		if(literaltype==tmpliteralp->literaltype) {
			switch(tmpliteralp->literaltype) {
			case  doubledata:
				if(tmpliteralp->literal.literaldouble==tmpdouble) {
					*literaladdr=tmpliteralp->addr;
					return(USERPROCOK);
				}
				break;
			case  stringdata:
				if(strcmp(tmpliteralp->literal.literalstr,literalstr)==0) {
					*literaladdr=tmpliteralp->addr;
					return(USERPROCOK);
				}
				break;
			}
		}
		lastliteralp=tmpliteralp;
		tmpliteralp=tmpliteralp->next;
	}
	
	tmpliteralp=(LITERALTAB  *)malloc(sizeof(LITERALTAB));
	tmpliteralp->next=NULL;
	tmpliteralp->literaltype=literaltype;
	if(((literalp-LITERAL_INITADDR)%sizeof(double))!=0)
			literalp=literalp+sizeof(double)-(literalp-LITERAL_INITADDR)%sizeof(double);
	literalp+=sizeof(LITERAL_CTRL);
	switch(literaltype) {
		case  doubledata:
			tmpliteralp->addr=literalp;
			tmpliteralp->literal.literaldouble=tmpdouble;
			tmpliteralp->len=sizeof(double);
			literalp+=sizeof(double);
			break;
		case  stringdata:
			tmpliteralp->addr=literalp;
			strcpy(tmpliteralp->literal.literalstr,literalstr);
			tmpliteralp->len=strlen(literalstr)+1;
			literalp+=strlen(literalstr)+1;
			break;
	}
	
	if(literaltabroot==NULL) {
		literaltabroot=tmpliteralp;
	}
	else {
		lastliteralp->next=tmpliteralp;
	}
	*literaladdr=tmpliteralp->addr;
	return(USERPROCOK);
}


//根据地址获取常数
int  UserProc::getcontant_literaltab(int literaladdr,double *value)
{
	LITERALTAB  *tmpliteralp,*lastliteralp;
	
	tmpliteralp=literaltabroot;
	while(tmpliteralp!=NULL) {
		if(tmpliteralp->literaltype==doubledata && tmpliteralp->addr==literaladdr) {
				*value=tmpliteralp->literal.literaldouble;
				return(USERPROCOK);
		}
		lastliteralp=tmpliteralp;
		tmpliteralp=tmpliteralp->next;
	}
	return(-1);
}


//释放值表
int  UserProc::free_literaltab(void)
{
	LITERALTAB  *tmpvarp,*lastvarp;
	tmpvarp=literaltabroot;
	while(tmpvarp!=NULL) {
		lastvarp=tmpvarp->next;
		free(tmpvarp);
		tmpvarp=lastvarp;
	}
	literaltabroot=NULL;
	return(USERPROCOK);
}

//写入值表到目标文件中
int  UserProc::writeout_literaltab(int offset)
{
	int  len=0;
	LITERALTAB  *tmpvarp;
	LITERAL_CTRL   literalctrl;

	tmpvarp=literaltabroot;
	while(tmpvarp!=NULL) {
		literalctrl.literaltype=tmpvarp->literaltype;
		literalctrl.len=tmpvarp->len;
		writeoutbuff((void *)&literalctrl,offset+tmpvarp->addr-LITERAL_INITADDR-sizeof(LITERAL_CTRL),sizeof(LITERAL_CTRL));
		writeoutbuff((void *)tmpvarp->literal.literalstr,offset+tmpvarp->addr-LITERAL_INITADDR,tmpvarp->len);
		len=tmpvarp->addr-LITERAL_INITADDR+tmpvarp->len;
		tmpvarp=tmpvarp->next;
	}
	return(len);
}


//查找函数
int  UserProc::search_functab(char *funcname,DATATYPE datatype,CALLPARA *callpara,DATATYPE *resulttypep)
{
	int i,j;
	
	for(i=0;i<sizeof(interfunctab)/sizeof(FUNCTIONTAB);i++) {
		if(strcasecmp(funcname,interfunctab[i].funcname)==0 && datatype==interfunctab[i].objtype) {
			switch(interfunctab[i].parachktype) {
			case nocheck:
				break;
			case morepara:
				if(interfunctab[i].callpara.inparanums>callpara->inparanums) {
					userprocerror(USERPROCERR_SYNX,"调用参数不匹配");
					return(-1);
				}
				break;
			case complete:
			default:
				if(interfunctab[i].callpara.inparanums!=callpara->inparanums) {
					userprocerror(USERPROCERR_SYNX,"调用参数不匹配");
					return(-1);
				}
				for(j=0;j<callpara->inparanums;j++) {
					if(interfunctab[i].callpara.inparatype[j]!=callpara->inparatype[j]) {
						userprocerror(USERPROCERR_SYNX,"调用参数不匹配");
						return(-1);
					}
				}
				break;
			}
			break;
		}
	}
	
	if(i<(sizeof(interfunctab)/sizeof(FUNCTIONTAB))){
		*resulttypep=interfunctab[i].resulttype;
		return(interfunctab[i].funcno);
	}
	else {
		char  errinf[100];
		sprintf(errinf,"函数 %s 不存在",funcname);
		userprocerror(USERPROCERR_SYNX,errinf);
		return(-1);
	}
}

/////////////////////////////////
//初始化临时数值变量区标志
int  UserProc::init_tempdouble(void)
{
	memset((char *)tempdoubleflag,0,sizeof(tempdoubleflag));
	return(USERPROCOK);
}

//分配临时数值变量
int  UserProc::malloc_tempdouble(void)
{
	int i;
	
	for(i=0;i<tempdoublenums;i++) {
		if(tempdoubleflag[i]==0) {
			tempdoubleflag[i]=1;
			return(tempdoubleoff+i*sizeof(double));
		}
	}
	userprocerror(USERPROCERR_MALLOC,"malloc_tempdouble error !!");
	return(-1);
}

//释放临时数值变量
int  UserProc::free_tempdouble(int  tmpdoubleaddr)
{
	int  tmpno;
	
	tmpno=(tmpdoubleaddr-tempdoubleoff)/sizeof(double);
	if(tmpno<tempdoublenums)tempdoubleflag[tmpno]=0;
	return(USERPROCOK);
}

//初始化临时字符串变量区标志
int  UserProc::init_tempstring(void)
{
	memset((char *)tempstringflag,0,sizeof(tempstringflag));
	return(USERPROCOK);
}

//分配临时字符串变量
int  UserProc::malloc_tempstring(void)
{
	int i;
	
	for(i=0;i<tempstrnums;i++) {
		if(tempstringflag[i]==0) {
			tempstringflag[i]=1;
			return(tempstringoff+i*MAXSTRINGLEN);
		}
	}
	userprocerror(USERPROCERR_MALLOC,"inter error");
	return(USERPROCOK);
}

//释放临时字符串变量
int  UserProc::free_tempstring(int  tmpstraddr)
{
	int  tmpno;
	
	tmpno=(tmpstraddr-tempstringoff)/MAXSTRINGLEN;
	if(tmpno<tempstrnums)tempstringflag[tmpno]=0;
	return(USERPROCOK);
}


/////////////////////////////////
//PUSH语法栈
int  UserProc::push_synxstack(DATATYPE datatype,ADDRTYPE addrtype,int addr)
{
	synxstack.synxrecord[synxstack.stackp].datatype=datatype;
	synxstack.synxrecord[synxstack.stackp].addrtype=addrtype;
	synxstack.synxrecord[synxstack.stackp].addr=addr;
	synxstack.stackp++;
	return(USERPROCOK);
}

//POP语法栈
int  UserProc::pop_synxstack(DATATYPE *datatype,ADDRTYPE *addrtype,int *addr)
{
	if(synxstack.stackp<1) {
		userprocerror(USERPROCERR_OPERATION,"格式不匹配");
		return(-1);
	}
	synxstack.stackp--;
	*datatype=synxstack.synxrecord[synxstack.stackp].datatype;
	*addrtype=synxstack.synxrecord[synxstack.stackp].addrtype;
	*addr=synxstack.synxrecord[synxstack.stackp].addr;
	return(USERPROCOK);
}

//向当前位置写入指令(中间代码)
int  UserProc::writeinstrument(int  instrucode,int addr1,int addr2,int addr3)
{
	PCINSTRUMENT  ins;
	
	//PrintInfoLine("   %.3d : %d  %d  %d  %d\n",instruno,instrucode,addr1,addr2,addr3);
	ins.instrucode=instrucode;
	ins.addr1=addr1;
	ins.addr2=addr2;
	ins.addr3=addr3;
	writeoutbuff((void *)&ins,instruno*sizeof(PCINSTRUMENT)+codeoffset,sizeof(PCINSTRUMENT));
	instruno++;
	return(USERPROCOK);
}

//向指定位置写入指令
int  UserProc::writeinstrument(int inno,int  instrucode,int addr1,int addr2,int addr3)
{
	PCINSTRUMENT  ins;
	
	//PrintInfoLine("   %.3d : %d  %d  %d  %d\n",inno,instrucode,addr1,addr2,addr3);
	ins.instrucode=instrucode;
	ins.addr1=addr1;
	ins.addr2=addr2;
	ins.addr3=addr3;
	writeoutbuff((void *)&ins,inno*sizeof(PCINSTRUMENT)+codeoffset,sizeof(PCINSTRUMENT));
	return(USERPROCOK);
}

//写入空指令
int  UserProc::write_nullinstrument(void)
{
	int  tmpint;
	
	tmpint=instruno;
	instruno++;
	return(tmpint);
}



//获取当前指令号
int  UserProc::get_curinstruno(void)
{
	return(instruno);
}

//获取指定位置的指令
int  UserProc::readinstrument(int inno,int  *instrucode,int *addr1,int *addr2,int *addr3)
{
	PCINSTRUMENT  ins;
	
	readoutbuff((void *)&ins,inno*sizeof(PCINSTRUMENT)+codeoffset,sizeof(PCINSTRUMENT));
	*instrucode=ins.instrucode;
	*addr1=ins.addr1;
	*addr2=ins.addr2;
	*addr3=ins.addr3;
	return(USERPROCOK);
}


//分解数据库域名
int  UserProc::split_dbfieldname(char *dbfieldname,char *tabname,char *pointname,char *fieldname)
{
	char *tmpcharp,*tmpcharp1;
	
	tmpcharp=strchr(dbfieldname,'.');
	if(tmpcharp==NULL) {
		userprocerror(USERPROCERR_OPERATION,"数据库域格式不匹配");
		return(-1);
	}
	*tmpcharp='\0';
	strcpy(tabname,dbfieldname);
	tmpcharp++;
	tmpcharp1=tmpcharp;
	tmpcharp=strchr(tmpcharp1,'.');
	if(tmpcharp==NULL) {
		userprocerror(USERPROCERR_OPERATION,"数据库域格式不匹配");
		return(-1);
	}
	*tmpcharp='\0';
	strcpy(pointname,tmpcharp1);
	tmpcharp++;
	tmpcharp1=tmpcharp;
	strcpy(fieldname,tmpcharp1);
	return(USERPROCOK);
}

///////////////////////////////////

//PUSH输入参数栈
int  UserProc::push_inparastack(char *inparaname,DATATYPE  datatype)
{
	int   pointaddr,valueaddr;
	char  tmpname[MAXWORDLEN];
	
	strcpy(tmpname,inparaname);
	strcat(tmpname,"&");
	pointaddr=malloc_vartab(tmpname,INPUTVAR,datatype,1);
	valueaddr=malloc_vartab(inparaname,INTERVAR,datatype,1);
	
	inparastack.inpararcd[inparastack.stackp].pointaddr=pointaddr;
	inparastack.inpararcd[inparastack.stackp].valueaddr=valueaddr;
	inparastack.inpararcd[inparastack.stackp].datatype=datatype;
	inparastack.stackp++;
	inparanums++;
	return(USERPROCOK);
}

//POP输入参数栈
int  UserProc::pop_inparastack(int *pointaddr,int  *valueaddr,DATATYPE  *datatype)
{
	inparastack.stackp--;
	if(inparastack.stackp<0)return(-1);
	*pointaddr=inparastack.inpararcd[inparastack.stackp].pointaddr;
	*valueaddr=inparastack.inpararcd[inparastack.stackp].valueaddr;
	*datatype=inparastack.inpararcd[inparastack.stackp].datatype;
	return(USERPROCOK);
}

//PUSH nums个输入参数到栈中
int  UserProc::restore_inparastack(int nums)
{
	inparastack.stackp+=nums;
	return(USERPROCOK);
}


     
int  UserProc::push_funcparastack(ADDRTYPE  addrtype,int paraaddr)
{
   	funcparastack.addrtype[funcparastack.stackp]=addrtype;
   	funcparastack.paraaddr[funcparastack.stackp]=paraaddr;
   	funcparastack.stackp++;
   	if(funcparastack.stackp>=100) {
   		userprocerror(USERPROCERR_MALLOC,"push_funcparastack error !!");
   		return(-1);
   	   }
   	return(USERPROCOK);
}

int  UserProc::pop_funcparastack(ADDRTYPE  *addrtype,int *paraaddr)
{
   	if(funcparastack.stackp<1)return(-1);
   	funcparastack.stackp--;
   	*addrtype=funcparastack.addrtype[funcparastack.stackp];
   	*paraaddr=funcparastack.paraaddr[funcparastack.stackp];
   	return(USERPROCOK);
}




//编码输入参数
int  UserProc::encode_inpara(void)
{
	int   i,pointaddr,valueaddr;
	DATATYPE   datatype;
	
	prochead.inparanums=inparanums;
	for(i=0;i<inparanums;i++) {
		pop_inparastack(&pointaddr,&valueaddr,&datatype);
		prochead.inparatype[prochead.inparanums-i-1]=datatype;
		writeinstrument(INST_POP,-1,-1,pointaddr);
		if(datatype==stringdata)
			writeinstrument(INST_ADDRMOVSTR,pointaddr,-1,valueaddr);
		else 
			writeinstrument(INST_ADDRMOV,pointaddr,-1,valueaddr);
	}
	restore_inparastack(inparanums);
	return(USERPROCOK);
}


//编码输出参数
int  UserProc::encode_outpara(void)
{
	int  pointaddr,valueaddr;
	DATATYPE   datatype;
	int  i;
	
	for(i=0;i<prochead.inparanums;i++) {
		pop_inparastack(&pointaddr,&valueaddr,&datatype);
		if(datatype==doubledata)
			writeinstrument(INST_MOVADDR,valueaddr,-1,pointaddr);
		else if(datatype==stringdata)
			writeinstrument(INST_MOVSTRADDR,valueaddr,-1,pointaddr);
	}
	restore_inparastack(prochead.inparanums);
	return(USERPROCOK);
}

//编码RETURN句
int  UserProc::encode_return(void)
{
	int  tmpaddr;
	ADDRTYPE addrtype;
	DATATYPE datatype;
	
	encode_outpara();
	encode_get_arrayvalue(&datatype,&addrtype,&tmpaddr);
	writeinstrument(INST_RETURN,tmpaddr,-1,-1);
	free_temp_result(addrtype,tmpaddr);
	return(USERPROCOK);
}

//编码break句
int  UserProc::encode_break(void)
{
	FOR_CTRL_PARA  forctrlpara;

	if(forloopstack.Check(&forctrlpara)) {
		writeinstrument(INST_JMP,-1,-1,forctrlpara.forendno);
	}
	else {
		userprocerror(USERPROCERR_SYNX,"break 语句不在循环体内");
	}
	return(USERPROCOK);
}
//编码break句
int  UserProc::encode_continue(void)
{
	FOR_CTRL_PARA  forctrlpara;

	if(forloopstack.Check(&forctrlpara)) {
		writeinstrument(INST_JMP,-1,-1,forctrlpara.incstno);
	}
	else {
		userprocerror(USERPROCERR_SYNX,"continue 语句不在循环体内");
	}
	return(USERPROCOK);
}

//编码过程结束
int  UserProc::encode_procover(void)
{
	int  tmpaddr;
	
	encode_outpara();
	search_literaltab(doubledata,"0.0",&tmpaddr);
	writeinstrument(INST_RETURN,tmpaddr,-1,-1);
	return(USERPROCOK);
}


//编码过程结束或中间退出后的各种善后处理
int  UserProc::encode_exitproc(void)
{
	free_object_in_vartab();
	writeinstrument(INST_INPROCRET,-1,-1,-1);
	return(USERPROCOK);
}



// 编码对象分配
int  UserProc::encode_mallocobject(DATATYPE objtype,int objaddr,int nums)
{
	for(int i=0;i<nums;i++) {
		writeinstrument(INST_MALLOCOBJ,objtype,-1,objaddr+i*MALLOC_OBJECT_LEN);
	}
	return(USERPROCOK);
}

// 编码对象释放
int  UserProc::encode_freeobject(DATATYPE objtype,int objaddr,int nums)
{
	if(DATATYPE_IS_OBJECT(objtype)) {
		for(int i=0;i<nums;i++) {
			writeinstrument(INST_FREEOBJ,objtype,objaddr+i*MALLOC_OBJECT_LEN,-1);
		}
	}
	return(USERPROCOK);
}


//编码函数输入参数
DATATYPE  UserProc::encode_pushpara(void)
{
	int  tmpaddr,tmpaddr1;
	ADDRTYPE addrtype;
	DATATYPE datatype;
	
	encode_get_arrayvalue(&datatype,&addrtype,&tmpaddr);
	if(addrtype==literal) {
		if(datatype==doubledata) {
			tmpaddr1=malloc_tempdouble();
			writeinstrument(INST_MOV,tmpaddr,-1,tmpaddr1);
			tmpaddr=tmpaddr1;
			addrtype=tempvar;
		}
		else if(datatype==stringdata) {
			tmpaddr1=malloc_tempstring();
			writeinstrument(INST_MOVSTR,tmpaddr,-1,tmpaddr1);
			tmpaddr=tmpaddr1;
			addrtype=tempstr;
		}
    }

	writeinstrument(INST_PARATYPE,datatype,-1,-1);
	writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	push_funcparastack(addrtype,tmpaddr);
	return(datatype);
}

//编码函数调用
int  UserProc::encode_callfunc(DATATYPE  objtype,int objaddr,int callfuncno,CALLPARA  *callparap,DATATYPE resulttype)
{  
	int  i,resultaddr,tmpaddr;
	ADDRTYPE  addrtype;
	
	if(DATATYPE_IS_OBJECT(objtype)) {
		writeinstrument(INST_OBJECT,objaddr,(int)objtype,-1);
	}

	if(resulttype==stringdata)	{
		resultaddr=malloc_tempstring();
		writeinstrument(INST_CALLFUNC,callfuncno,callparap->inparanums,resultaddr);
		push_synxstack(stringdata,tempstr,resultaddr);
	}
	else {
		resultaddr=malloc_tempdouble();
		writeinstrument(INST_CALLFUNC,callfuncno,callparap->inparanums,resultaddr);
		push_synxstack(doubledata,tempvar,resultaddr);
	}


	for(i=0;i<callparap->inparanums;i++) {
		if(pop_funcparastack(&addrtype,&tmpaddr)==USERPROCOK) {
			free_temp_result(addrtype,tmpaddr);
		}
	}
    
	return(USERPROCOK);
}

//编码 IF ELSE 指令跳变
int  UserProc::encode_ifelse(int resultaddr,int jmpzerono,int jmpno,int elseno,int endifno)
{
	writeinstrument(jmpzerono,INST_JMPZERO,resultaddr,-1,elseno);
	writeinstrument(jmpno,INST_JMP,-1,-1,endifno);
	return(USERPROCOK);
}

//编码 FOR 指令跳变
int  UserProc::encode_forstatement(int resultaddr,int expstno,int expjmpzerono,int expjmpno,
							  int incstno,int incjmpno,int loopstno,int loopjmpno,int forendno)
{
	int  tmpaddr=0;
//	ADDRTYPE addrtype;
//	DATATYPE datatype;
	
	writeinstrument(expjmpzerono,INST_JMPZERO,resultaddr,-1,forendno);
	writeinstrument(expjmpno,INST_JMP,-1,-1,loopstno);
	writeinstrument(incjmpno,INST_JMP,-1,-1,expstno);
	writeinstrument(loopjmpno,INST_JMP,-1,-1,incstno);
	return(USERPROCOK);
}

//编码数组偏移

int  UserProc::encode_array_off(DATATYPE  vardatatype,int varaddr,int nums)
{
	int  offaddr,tmpaddr,lastaddr;
	ADDRTYPE addrtype;
	DATATYPE datatype;
	
	encode_get_arrayvalue(&datatype,&addrtype,&offaddr);
	if(datatype!=doubledata)userprocerror(USERPROCERR_SYNX,"数组下标必须是数值类型");

	if(addrtype==literal) {
		int      intvalue;
		double   arrayvalue;

		getcontant_literaltab(offaddr,&arrayvalue);
		intvalue=(int)arrayvalue;
		if(arrayvalue>=nums) userprocerror(USERPROCERR_SYNX,"数组下标超出定义");
		if(vardatatype==stringdata) {
			lastaddr=varaddr + intvalue * MALLOC_STRINGDATA_LEN;
		}
		else if(vardatatype==doubledata) {
			lastaddr=varaddr + intvalue * MALLOC_DOUBLEDATA_LEN;
		}
        else {
			lastaddr=varaddr + intvalue * MALLOC_OBJECT_LEN;
		}
		push_synxstack(vardatatype,definevar,lastaddr);
	}
	else {
		char  tmpstring[50];
		int   arrayaddr,arraychkaddr;

		sprintf(tmpstring,"%d",nums);
		search_literaltab(doubledata,tmpstring,&arraychkaddr);
		writeinstrument(INST_ARRAYCHK,arraychkaddr,offaddr,-1); // 数组下标检查

		if(vardatatype==stringdata) {
			sprintf(tmpstring,"%d",MALLOC_STRINGDATA_LEN);
		}
		else if(vardatatype==doubledata) {
			sprintf(tmpstring,"%d",MALLOC_DOUBLEDATA_LEN);
		}
        else {
			sprintf(tmpstring,"%d",MALLOC_OBJECT_LEN);
		}
		search_literaltab(doubledata,tmpstring,&tmpaddr);
		arrayaddr=malloc_tempdouble();
		writeinstrument(INST_MUL,offaddr,tmpaddr,arrayaddr);

		sprintf(tmpstring,"%d",varaddr);
		search_literaltab(doubledata,tmpstring,&tmpaddr);

		writeinstrument(INST_ADD,arrayaddr,tmpaddr,arrayaddr);
		push_synxstack(vardatatype,tempoffaddr,arrayaddr);
	}
	free_temp_result(addrtype,offaddr);
	return(USERPROCOK);
}

//编码数组取值
int  UserProc::encode_get_arrayvalue(DATATYPE	*datatype,ADDRTYPE	*addrtype,int  *addr)
{
	DATATYPE	tmpdatatype;
	ADDRTYPE	tmpaddrtype;
	int			tmpaddr,tmpaddr1;

	pop_synxstack(&tmpdatatype,&tmpaddrtype,&tmpaddr);
	if(tmpaddrtype==tempoffaddr) {
		if(tmpdatatype==stringdata) {
			tmpaddr1=malloc_tempstring();
			writeinstrument(INST_OFFMOVSTR,tmpaddr,-1,tmpaddr1);
			free_temp_result(tmpaddrtype,tmpaddr);
			*datatype=tmpdatatype;
			*addrtype=tempstr;
			*addr=tmpaddr1;
		}
		else {
			tmpaddr1=malloc_tempdouble();
			writeinstrument(INST_OFFMOV,tmpaddr,-1,tmpaddr1);
			free_temp_result(tmpaddrtype,tmpaddr);
			*datatype=tmpdatatype;
			*addrtype=tempvar;
			*addr=tmpaddr1;
		}
	}
	else {
		*datatype=tmpdatatype;
		*addrtype=tmpaddrtype;
		*addr=tmpaddr;
	}
	return(USERPROCOK);
}



//编码获取数据库域值, 有'@'是变量名，无'@'则是值名
int  UserProc::encode_getfieldvalue(char *dbfieldname,DATATYPE  getdatatype)
{
	int  ret,tmpaddr=0;
	VARSTYLE vartype;
	DATATYPE datatype;
	char tabname[MAXWORDLEN],pointname[MAXWORDLEN],fieldname[MAXWORDLEN];

	split_dbfieldname(dbfieldname,tabname,pointname,fieldname);
	
	if(tabname[0]=='@') {	//变量名
		ret=search_vartab(tabname+1,&tmpaddr,&vartype,&datatype);
		if(ret==0) {
			if(datatype!=stringdata)userprocerror(USERPROCERR_SYNX,"数据库域名格式中变量类型不对");
		}
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	else {			//值名
		search_literaltab(stringdata,tabname,&tmpaddr);
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	
	if(pointname[0]=='@') {
		ret=search_vartab(pointname+1,&tmpaddr,&vartype,&datatype);
		if(ret==0) {
			if(datatype!=stringdata)userprocerror(USERPROCERR_SYNX,"数据库域名格式中变量类型不对");
		}
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	else {
		search_literaltab(stringdata,pointname,&tmpaddr);
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	
	if(fieldname[0]=='@') {   
		ret=search_vartab(fieldname+1,&tmpaddr,&vartype,&datatype);
		if(ret==0) {
			if(datatype!=stringdata)userprocerror(USERPROCERR_SYNX,"数据库域名格式中变量类型不对");
		}
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	else {
		search_literaltab(stringdata,fieldname,&tmpaddr);
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	
	if(getdatatype==stringdata)  {
		tmpaddr=malloc_tempstring();
		writeinstrument(INST_CALLFUNC,FUNC_GETFIELDSTR_NAME,-1,tmpaddr);
		push_synxstack(getdatatype,tempstr,tmpaddr);
	}
	else {
		tmpaddr=malloc_tempdouble();
		writeinstrument(INST_CALLFUNC,FUNC_GETFIELD_NAME,-1,tmpaddr);
		push_synxstack(getdatatype,tempvar,tmpaddr);
	}
	return(USERPROCOK);
}


//编码置入数据库域值，有'@'是变量名，无'@'则是值名
int  UserProc::encode_putfieldvalue(char *dbfieldname,DATATYPE  putdatatype)
{
	int  ret,tmpaddr=0,freetmpaddr;
	VARSTYLE vartype;
	ADDRTYPE addrtype,freeaddrtype;
	DATATYPE datatype;
	char tabname[MAXWORDLEN],pointname[MAXWORDLEN],fieldname[MAXWORDLEN];
	
	split_dbfieldname(dbfieldname,tabname,pointname,fieldname);
	if(tabname[0]=='@') {
		ret=search_vartab(tabname+1,&tmpaddr,&vartype,&datatype);
		if(ret==0) {
			if(datatype!=stringdata)userprocerror(USERPROCERR_SYNX,"数据库域名格式中变量类型不对");
		}
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	else {
		search_literaltab(stringdata,tabname,&tmpaddr);
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	
	if(pointname[0]=='@') {
		ret=search_vartab(pointname+1,&tmpaddr,&vartype,&datatype);
		if(ret==0) {
			if(datatype!=stringdata)userprocerror(USERPROCERR_SYNX,"数据库域名格式中变量类型不对");
		}
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	else {
		search_literaltab(stringdata,pointname,&tmpaddr);
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	
	if(fieldname[0]=='@') {   
		ret=search_vartab(fieldname+1,&tmpaddr,&vartype,&datatype);
		if(ret==0) {
			if(datatype!=stringdata)userprocerror(USERPROCERR_SYNX,"数据库域名格式中变量类型不对");
		}
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	else {
		search_literaltab(stringdata,fieldname,&tmpaddr);
		writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	}
	
	encode_get_arrayvalue(&datatype,&addrtype,&tmpaddr);
	if(datatype!=putdatatype)userprocerror(USERPROCERR_OPERATION,"类型不匹配");
	writeinstrument(INST_PUSH,tmpaddr,-1,-1);
	
	freeaddrtype=addrtype;
	freetmpaddr=tmpaddr;
	
	tmpaddr=malloc_tempdouble();
	writeinstrument(INST_CALLFUNC,FUNC_PUTFIELD_NAME,-1,tmpaddr);
	push_synxstack(doubledata,tempvar,tmpaddr);	

	free_temp_result(freeaddrtype,freetmpaddr);
	
	return(USERPROCOK);
}

//编码赋值操作
int  UserProc::encode_equtalop(void)
{
	int  opaddr[2];
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	pop_synxstack(&datatype[0],&addrtype[0],&opaddr[0]);
	if(datatype[0]!=doubledata && datatype[0]!=stringdata)
		userprocerror(USERPROCERR_OPERATION,"对象不能进行赋值");
	if(datatype[0]!=datatype[1])userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");
	
	if(addrtype[0]==tempoffaddr) {
		if(datatype[0]==doubledata)
			writeinstrument(INST_MOVOFF,opaddr[1],-1,opaddr[0]);
		else 
			writeinstrument(INST_MOVSTROFF,opaddr[1],-1,opaddr[0]);
	}
	else {
		if(datatype[0]==doubledata)
			writeinstrument(INST_MOV,opaddr[1],-1,opaddr[0]);
		else 
			writeinstrument(INST_MOVSTR,opaddr[1],-1,opaddr[0]);
	}

 	push_synxstack(datatype[0],addrtype[0],opaddr[0]);
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}


//编码逻辑操作
int  UserProc::encode_logicalop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);
	if(datatype[0]!=datatype[1] || datatype[0]!=doubledata)
		userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");
	if(addrtype[0]==tempvar) 
		resultaddr=opaddr[0];
	else 
		resultaddr=malloc_tempdouble();
	
	switch(optype) {
	case  WT_AND:
        writeinstrument(INST_AND,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_OR:
        writeinstrument(INST_OR,opaddr[0],opaddr[1],resultaddr);
        break;
	}
	push_synxstack(doubledata,tempvar,resultaddr);
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}

//编码关系操作
int  UserProc::encode_relationop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);

	if(datatype[0]!=datatype[1] || (datatype[0]!=doubledata && datatype[0]!=stringdata))
		userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");
	
	if(addrtype[0]==tempvar) 
		resultaddr=opaddr[0];
	else 
		resultaddr=malloc_tempdouble();
	
	switch(optype) {
	case  WT_EQUEQU:
        if(datatype[0]==stringdata)
			writeinstrument(INST_STREQUEQU,opaddr[0],opaddr[1],resultaddr);
        else 
			writeinstrument(INST_EQUEQU,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_NOTEQU:
        if(datatype[0]==stringdata)
			writeinstrument(INST_STRNOTEQU,opaddr[0],opaddr[1],resultaddr);
        else 
			writeinstrument(INST_NOTEQU,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_LARGE:
        if(datatype[0]==stringdata)
			writeinstrument(INST_STRLARGE,opaddr[0],opaddr[1],resultaddr);
        else 
			writeinstrument(INST_LARGE,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_LARGEEQU:
        if(datatype[0]==stringdata)
			writeinstrument(INST_STRLARGEEQU,opaddr[0],opaddr[1],resultaddr);
        else 
			writeinstrument(INST_LARGEEQU,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_SMALL:
        if(datatype[0]==stringdata)
			writeinstrument(INST_STRSMALL,opaddr[0],opaddr[1],resultaddr);
        else 
			writeinstrument(INST_SMALL,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_SMALLEQU:
        if(datatype[0]==stringdata)
			writeinstrument(INST_STRSMALLEQU,opaddr[0],opaddr[1],resultaddr);
        else 
			writeinstrument(INST_SMALLEQU,opaddr[0],opaddr[1],resultaddr);
        break;
	}
	push_synxstack(doubledata,tempvar,resultaddr);
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}

//编码比特操作
int  UserProc::encode_bitop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);

	if(datatype[0]!=datatype[1] || datatype[0]!=doubledata)
		userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");

	if(addrtype[0]==tempvar) 
		resultaddr=opaddr[0];
	else 
		resultaddr=malloc_tempdouble();
	
	switch(optype) {
	case  WT_BITAND:
        writeinstrument(INST_BITAND,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_BITOR:
        writeinstrument(INST_BITOR,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_XOR:
        writeinstrument(INST_XOR,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_LSHIFT:
        writeinstrument(INST_LSHIFT,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_RSHIFT:
        writeinstrument(INST_RSHIFT,opaddr[0],opaddr[1],resultaddr);
        break;
	}
	push_synxstack(doubledata,tempvar,resultaddr);
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}

//编码符号操作
int  UserProc::encode_signop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	if(optype==WT_ADD)return(USERPROCOK);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);

	if(datatype[0]!=doubledata)	userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");

	if(addrtype[0]==tempvar) 
		resultaddr=opaddr[0];
	else 
		resultaddr=malloc_tempdouble();
	
	switch(optype) {
	case  WT_BITNOT:
        writeinstrument(INST_BITNOT,opaddr[0],-1,resultaddr);
        break;
	case  WT_SUB:
        writeinstrument(INST_FU,opaddr[0],-1,resultaddr);
        break;
	}
	push_synxstack(doubledata,tempvar,resultaddr);
	return(USERPROCOK);
}

//编码加减操作
int  UserProc::encode_addingop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);
	if(datatype[0]!=datatype[1])userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");

	
	switch(optype) {
	case  WT_ADD:
		if(datatype[0]==doubledata) {
			if(addrtype[0]==tempvar) 
				resultaddr=opaddr[0];
			else 
				resultaddr=malloc_tempdouble();
			writeinstrument(INST_ADD,opaddr[0],opaddr[1],resultaddr);
			push_synxstack(doubledata,tempvar,resultaddr);
		}
		else if(datatype[0]==stringdata) {
			if(addrtype[0]==tempvar) 
				resultaddr=opaddr[0];
			else 
				resultaddr=malloc_tempstring();
			writeinstrument(INST_ADDSTR,opaddr[0],opaddr[1],resultaddr);
			push_synxstack(stringdata,tempstr,resultaddr);
		}
		else userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");
        break;
	case  WT_SUB:
		if(datatype[0]!=doubledata)userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");
		if(addrtype[0]==tempvar) 
			resultaddr=opaddr[0];
		else 
			resultaddr=malloc_tempdouble();
        writeinstrument(INST_SUB,opaddr[0],opaddr[1],resultaddr);
		push_synxstack(doubledata,tempvar,resultaddr);
        break;
	}
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}

//编码 乘,除,取余 操作
int  UserProc::encode_multiplyop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);

	if(datatype[0]!=datatype[1] || datatype[0]!=doubledata)
		userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");

	if(addrtype[0]==tempvar) 
		resultaddr=opaddr[0];
	else 
		resultaddr=malloc_tempdouble();
	
	switch(optype) {
	case  WT_MUL:
        writeinstrument(INST_MUL,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_DIV:
        writeinstrument(INST_DIV,opaddr[0],opaddr[1],resultaddr);
        break;
	case  WT_MOD:
        writeinstrument(INST_MOD,opaddr[0],opaddr[1],resultaddr);
        break;
	}
	push_synxstack(doubledata,tempvar,resultaddr);
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}

//编码幂操作
int  UserProc::encode_mulmulop(int optype)
{
	int  opaddr[2],resultaddr;
	DATATYPE   datatype[2];
	ADDRTYPE   addrtype[2];
	
	encode_get_arrayvalue(&datatype[1],&addrtype[1],&opaddr[1]);
	encode_get_arrayvalue(&datatype[0],&addrtype[0],&opaddr[0]);

	if(datatype[0]!=datatype[1] || datatype[0]!=doubledata)
		userprocerror(USERPROCERR_OPERATION,"操作类型不匹配");

	if(addrtype[0]==tempvar) 
		resultaddr=opaddr[0];
	else 
		resultaddr=malloc_tempdouble();
	
	switch(optype) {
	case  WT_MULMUL:
        writeinstrument(INST_MULMUL,opaddr[0],opaddr[1],resultaddr);
        break;
	}
	push_synxstack(doubledata,tempvar,resultaddr);
	free_temp_result(addrtype[1],opaddr[1]);
	return(USERPROCOK);
}



////////////////////////////////////////////////
//编译错误指示

#ifndef CC_FORMULA

int  UserProc::userprocerror(int  errtype,char *context)
{
	
    char buf[512];
    if(errornums<30) {
		switch(errtype) {
		case  USERPROCERR_WORD:
			sprintf(buf,"%s: line %d   词法错误: %s \r\n",srcfilename,lineno,context);
			break;
		case  USERPROCERR_SYNX:
			sprintf(buf,"%s: line %d   语法错误: %s \r\n",srcfilename,lineno,context);
			break;
		case  USERPROCERR_OPERATION:
			sprintf(buf,"%s: line %d   表达式错误: %s \r\n",srcfilename,lineno,context);
			break;
		default:
			sprintf(buf,"%s: line %d   内部错误 : %s \r\n",srcfilename,lineno,context);
			break;
		}
		errorflag=1;
		errornums++;
	    PrintInfoLine(buf);
		if(errornums==30){ 
			sprintf(buf, "%s: 错误太多(>=30) \r\n",srcfilename);
			PrintInfoLine(buf);
		}
		
	}
	return(USERPROCERR);
}

#else

extern  int  cc_formula_perror(int errtype,char *context,int lineno);

int  UserProc::userprocerror(int  errtype,char *context)
{
	
	errorflag=1;
    errornums++;
    return(cc_formula_perror(errtype,context,lineno));
}

#endif

//----------------------------------------------------------------------------------
//过程控制语言编译，参数：输入文件名和输出文件名
int  UserProc::userprocfile(char *srcfname,int localflag)
{
    int    ret;
    char  tmpoutfname[256],tmpoutfname1[256],procname[200];
    char  *sachomepath;
    char  *lastdirchar,*firstdot;
    		    		
	strcpy(tmpoutfname1,srcfname);

	lastdirchar=strrchr(tmpoutfname1,'/');
#ifdef _WIN32
	if( !lastdirchar )
		lastdirchar=strrchr(tmpoutfname1,'\\');
#endif
	if(lastdirchar==NULL) {
		lastdirchar=tmpoutfname1;
	}
	else lastdirchar++;
	
	firstdot=strchr(lastdirchar,'.');
	if(firstdot!=NULL)*firstdot='\0';
	strcpy(procname,lastdirchar);

printf("\n proc name ==== %s \n",procname);
    sachomepath=(char *)getenv("NBENV");
    if(sachomepath==NULL) {
        PrintInfoLine("NBENV not defined\r\n");
        return(USERPROCERR);
        }
    	    
    ret=opensrcfile(srcfname);
    if(ret!=USERPROCOK)
    {
        return(USERPROCERR);
    }

#ifdef __unix
    sprintf(tmpoutfname,"%s/%s/procscript.out.%d",sachomepath,UPOUT_PATHNAME,time(NULL));
#else
    sprintf(tmpoutfname,"%s\\%s\\procscript.out.%d",sachomepath,UPOUT_PATHNAME,time(NULL));
#endif

    if((ret=proc(tmpoutfname))!=USERPROCOK)return(ret);

    if(errornums==0) {		
#ifdef __unix
    		sprintf(tmpoutfname1,"%s/%s/%s%s",sachomepath,UPOUT_PATHNAME,procname,UPOUT_FEXNAME);
#else
    		sprintf(tmpoutfname1,"%s\\%s\\%s%s",sachomepath,UPOUT_PATHNAME,procname,UPOUT_FEXNAME);
#endif
		
		remove(tmpoutfname1);
		rename(tmpoutfname,tmpoutfname1);
		PrintInfoLine(procname);
		PrintInfoLine(" 编译成功\r\n");
		//dispobjfile(tmpoutfname1);0

		if(!localflag) {
			//全网存储编译后的用户过程
			char *fileBuffer;
			char fileName[128];
			FileMan fileman;
			int fileSize=prochead.codeoffset+prochead.codelen+prochead.literallen;
			outfp=fopen(tmpoutfname1,"rb");
			if(outfp==NULL) 
			{
				PrintInfoLine("目标文件打不开\r\n");
				return(USERPROCERR);
			}
			fileBuffer=(char *)malloc(fileSize);
			fseek(outfp,0,SEEK_SET);
			fread(fileBuffer,1,fileSize,outfp);
	#ifdef __unix
    			sprintf(fileName,"%s/%s%s",UPOUT_PATHNAME,procname,UPOUT_FEXNAME);
	#else
    			sprintf(fileName,"%s\\%s%s",UPOUT_PATHNAME,procname,UPOUT_FEXNAME);
	#endif
				if((ret = fileman.Save (fileName, (void*)fileBuffer, fileSize))==1) {
					PrintInfoLine(procname);
					PrintInfoLine(" 全网同步成功\r\n");
				}
			free(fileBuffer);
			fclose(outfp);
			outfp=NULL;
		}
	}
    else { 
		remove(tmpoutfname);
	}
	if(errorflag)	return(USERPROCERR);
    return(USERPROCOK);
}

//编译结果显示
int UserProc::dispobjfile(char *objfname)
{
    int i;
    PROC_HEAD tmphead;
    PCINSTRUMENT *ins;
    FILE  *fp;
    
    char buf[1000];
	
    fp=fopen(objfname,"rb");
    if(fp==NULL)return(-1);
    fread(&tmphead,1,sizeof(tmphead),fp);
    sprintf(buf,"Procname %s \r\n",tmphead.procname);
    
    PrintInfoLine(buf);
	
    PrintInfoLine("Inparatype  \r\n");
    
    for(i=0;i<tmphead.inparanums;i++) {
		sprintf(buf+i*6, "%5d ",tmphead.inparatype[i]);
	}
    strcat(buf, "\r\n");
	
    PrintInfoLine(buf);
	
    PrintInfoLine("Instruction:\r\n");
	
    ins=(PCINSTRUMENT *)malloc(tmphead.codelen);
    fseek(fp,tmphead.codeoffset,SEEK_SET);
    fread((void *)ins,1,tmphead.codelen,fp);
    for(i=0;(unsigned int)i<tmphead.codelen/sizeof(PCINSTRUMENT);i++) {
		sprintf(buf, " %d : %d %d %d %d \r\n",i,ins[i].instrucode,ins[i].addr1,ins[i].addr2,ins[i].addr3);
		PrintInfoLine(buf);
	}
    free(ins);

    /**************************    
    char *tmpliteralp=(char  *)malloc(tmphead.literallen);
    fseek(fp,tmphead.literaloffset,SEEK_SET);
    fread(tmpliteralp,1,tmphead.literallen,fp);
    
    char *tmpcharp=tmpliteralp;
    
    while(tmpcharp<(tmpliteralp+tmphead.literallen)) {
    	if(iswprint(tmpcharp[0]) && iswprint(tmpcharp[1]) && iswprint(tmpcharp[2])) {
    	printf("literal :  %s \n",tmpcharp);
    	tmpcharp+=(strlen(tmpcharp)+1);
    	}
    	else {
    	printf("literal :  %f \n",tmpcharp);
    	tmpcharp+=sizeof(double);
    	}
    }

    free(tmpliteralp);
    *************************/
    
    fclose(fp);
    
    return(USERPROCOK);
}

