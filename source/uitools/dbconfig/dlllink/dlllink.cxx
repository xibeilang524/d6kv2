#ifdef WIN32
	#include "stdafx.h"
	#include "dlllink.h"
#endif

#include "com_daqo_dbinfo_Dlllink.h"
#include "db/dbapi.h"
#include "rdb/rdbapi.h"//调试阜阳需要，以后可以去掉
#include "net/netapi.h"
#include "fes/femailstruct.h"



#ifdef WIN32
	#ifdef _DEBUG
		#define new DEBUG_NEW
	#undef THIS_FILE
		static char THIS_FILE[] = __FILE__;
	#endif

	BEGIN_MESSAGE_MAP(CDlllinkApp, CWinApp)
		//{{AFX_MSG_MAP(CDlllinkApp)
			// NOTE - the ClassWizard will add and remove mapping macros here.
			//    DO NOT EDIT what you see in these blocks of generated code!
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	CDlllinkApp theApp;

#endif
/////////////////////////////////////////////////////////////////////////////
// CDlllinkApp construction

#define DBNAMESIZE		64//数据库名长度
#define FIELDNAMESIZE	64//域名的长度

//是否包含ems应用操作部分，编译选项为保持和以前版本兼容而添加
//unix编译用makefile则需要有2个一个普通版本，一个是加ems操作的版本。
//不同的地方在lib中需要添加ems用的链接库，同时包含文件也需要添加ems用包含文件
//按我们系统习惯应该放在d6000的include中。考虑到兼容，直接放在本工程目录下的include_ems中了。
//#define INCEMS          1 
#ifdef  INCEMS
	char *emsrdbtbls[]={"tblname1","tblname2","tblname3"};
	//调用函数前需要gettableinfo函数
	bool IsUpdEmsRdb(){
		CS_CHAR tblName[128];
		strcpy(tblName,func.table);
		for(int i=0;i<sizeof(emsrdbtbls);i++)
	}
#endif


DBApi db;

FUNC_STR func;
CS_DATAFMT *datafmt = NULL;
void* pt = NULL;
int colcount = 0;//列的数目

char *errdbmsg = NULL;//数据库操作信息
int errdbmsglen;//信息长度
NetDbg	netdbg;
typedef struct _tagfdname
{
	char fdname[FIELDNAMESIZE];
}FDNAME;


#ifdef WIN32
	CDlllinkApp::CDlllinkApp()
	{
		// TODO: add construction code here,
		// Place all significant initialization in InitInstance
	}
#endif

//调试用程序 begin
void RdbDebugMesg(char *fmt,...)
{
	char *daqohomepath;
	Rdb_Time  rdbtime;
	RDB_DATETIME_MS_STRU  curclock;
	char  filename[128];
	char  debugmsg[1024];
	va_list  args;



	if((daqohomepath=getenv("NBENV"))==NULL)return;
#ifdef RDB_PLATFORM_UNIX
	strcpy(filename,daqohomepath);
	strcat(filename,"/log/dbconfig.txt");
#else
	strcpy(filename,daqohomepath);
	strcat(filename,"\\log\\dbconfig.txt");
#endif
	int len=RdbGetFileLen(filename);
#ifdef RDB_PLATFORM_SUNUNIX
	int debugfd;
	if(len>=(600 * 1000)) {
		unlink(filename);
		debugfd=open(filename,O_CREAT | O_APPEND | O_RDWR, 0666);
	}
	else if(len<0) {
		debugfd=open(filename,O_CREAT | O_APPEND | O_RDWR, 0666);
	}
	else {
		debugfd=open(filename,O_APPEND | O_RDWR);
	}
	if(debugfd<0)return;

 	rdbtime.RdbGetCurClock(&curclock);
 
 	sprintf(debugmsg,"%.4d-%.2d-%.2d  %.2d:%.2d:%.2d.%.3d  ",curclock.year,curclock.month,
      curclock.day,curclock.hour,curclock.minute,curclock.second,curclock.msecond);
	write(debugfd,debugmsg,strlen(debugmsg));

	va_start(args,fmt);
	vsprintf(debugmsg,fmt,args);
	va_end(args);
	write(debugfd,debugmsg,strlen(debugmsg));
	close(debugfd);
#else
	FILE *fp;
	if(len>=(200 * 1000)) {
		fp=fopen(filename,"w");
	}
	else {
		fp=fopen(filename,"a");
	}
	if(fp==NULL)return;

	rdbtime.RdbGetCurClock(&curclock);

 	fprintf(fp,"%.4d-%.2d-%.2d  %.2d:%.2d:%.2d.%.3d  ",curclock.year,curclock.month,
      curclock.day,curclock.hour,curclock.minute,curclock.second,curclock.msecond);

	va_start(args,fmt);
	vsprintf(debugmsg,fmt,args);
	va_end(args);
	fprintf(fp,"%s",debugmsg);
	fclose(fp);
#endif
	
}

void FromDataToStr (CS_DATAFMT *datafmt, char *cmdbuf1, void *datapt)
{
    CS_DATAFMT      destfmt; 
    CS_CHAR         dtstr[600]; 
    int i; 
    
    memset (&destfmt, 0, sizeof (destfmt)); 
    switch (datafmt->datatype) {
		case CS_TINYINT_TYPE:
		case CS_BIT_TYPE:
			sprintf (cmdbuf1, "%d", *((unsigned char *)datapt)); 
			break; 
		case CS_BINARY_TYPE:
			strcpy (cmdbuf1, "0x"); 
			for (i=0; i<datafmt->maxlength; i++) {
				sprintf (dtstr, "%02x", *((unsigned char *)datapt+i)); 
				strcat (cmdbuf1, dtstr); 
			}
			break; 
		case CS_SMALLINT_TYPE:
			sprintf (cmdbuf1, "%d", *((short int *)datapt)); 
			break; 
		case CS_INT_TYPE:
			sprintf (cmdbuf1, "%d", *((CS_INT *)datapt)); 
			break; 
		case CS_REAL_TYPE:
			sprintf (cmdbuf1, "%f", *((float *)datapt)); 
			break; 
		case CS_FLOAT_TYPE:
			sprintf (cmdbuf1, "%f", *((double *)datapt)); 
			break; 
		case CS_CHAR_TYPE:
			strcpy (cmdbuf1, "\""); 
			i=int (MIN ((int) (strlen ((char *)datapt)), datafmt->maxlength)); 
			memcpy (& (cmdbuf1[1]), datapt, i); 
			cmdbuf1[i+1]='\0'; 
			strcat (cmdbuf1, "\""); 
			break; 
		case CS_TEXT_TYPE:
		case CS_IMAGE_TYPE:
			sprintf (cmdbuf1, "0x44"); 
			break; 
		default:
			strcpy (cmdbuf1, ""); 
			break; 
    }
}
// 调试程序 end

/////////////////////////////////////////////////////////////////////////////
// The one and only CDlllinkApp object

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_startdb()
 * 功能描述：连接数据库
 * 参数    ：
 *		   无
 *
 * 返回值  ：成功=1，失败=0
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_startdb
  (JNIEnv *jEnv, jobject jthis)
{
	netdbg.RegProc("数据库管理工具");
	//打开ems实时表
	
	return db.Open(DB_OP_WR,FALSE);
	
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_closedb()
 * 功能描述：关闭数据库
 * 参数    ：
 *		   无
 *
 * 返回值  ：
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_closedb
  (JNIEnv *jEnv, jobject jthis)
{
	netdbg.UnRegProc();
	if(datafmt) 
	{
		free(datafmt);
		datafmt = 0;
	}
	if(pt) 
	{
		free(pt);
		pt = 0;
	}
	return db.Close();
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：endsWith()
 * 功能描述：检查字符串str是否以字符串postfix结尾
 * 参数    ：
 *           str=目标字符串
 *           postfix=搜索字符串
 * 返回值  ： 
 *           1=是 
 *           0=否 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char endsWith(char *str,char *postfix)
{
	int i;
	int lenpost=strlen(postfix),lenstr=strlen(str);
    for(i=0;i<lenpost;i++){
		if(str[lenstr-1-i]!=postfix[lenpost-1-i])
			break;
	}
	return i==lenpost ? 1:0;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：parsefieldstring()
 * 功能描述：分析选择域字符串
 * 参数    ：
 *           srcstr=目标字符串
 *           desp分析结束后的指针
 * 返回值  ： 
 *           域的个数
 *            
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
int parsefieldstring(char *srcstr,void **desp)
{

	int num = 10,count=0;//num,递进基数;  count,域计数
	FDNAME *p = (FDNAME *)malloc(num*sizeof(FDNAME));
	memset(p,0,sizeof(FDNAME)*num);
	int len = strlen(srcstr);
	int j = 0;
	int i;
	for(i =0;i<len;i++)
	{
		if(srcstr[i] == ',')
		{
			if(i>j)
			{
				if(count>0 && num/count >=1)
				{
					FDNAME *temp = p;
					p = (FDNAME*)malloc((num+count)*sizeof(FDNAME));
					memset(p,0,sizeof(FDNAME)*(num+count));
					memcpy(p,temp,num*sizeof(FDNAME));
					free(temp);
					num += count;
				}
				memcpy((char*)(p[count].fdname),(char*)&srcstr[j],i-j);
				count++;
			}
			j = i+1;
		}
	}
	if(i>j)//最后一个不是",",还有可能最后一个是","
	{
		memcpy((char*)(p[count].fdname),(char*)&srcstr[j],i-j);
		j = i+1;
		count++;
	}
	*desp = (void*)p;
	return count;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_getenvvar
 * 功能描述：获得系统运行环境变量
 * 参数    ：
 *		   无
 *
 * 返回值  ：环境变量的值
 *
 * 
 * --------------------------------------------------
 */
JNIEXPORT jbyteArray JNICALL Java_com_daqo_dbinfo_Dlllink_getenvvar
  (JNIEnv * jEnv, jobject jthis)
{

	char envvar[512];
	memset(envvar,0,512);
	char *name1 = getenv("NBENV");
	if(name1==NULL)
	{
		#if defined(WIN32)
			strcpy(envvar,"c:\\");
		#else
			name1 = getenv("HOME");
			strcpy(envvar,name1);
		#endif
	}
	else
		strcpy(envvar,name1);

	char post[4];
	memset((char*)post,0,4);
	strcpy(post,"\\");
	char rtn = endsWith((char*)envvar,(char*)post);
	if(rtn)
		envvar[strlen(envvar)-1]=0;


	int num = strlen((char*)envvar);
	jbyteArray rtnarray = jEnv->NewByteArray(num);
	
	jEnv->SetByteArrayRegion(rtnarray,0,num,(jbyte*)envvar); 

	return rtnarray;

}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_getTableInfo
 * 功能描述：得到表信息
 * 参数    ：tablename,表名;tableinfo,返回的表信息,包括域名，类型;rownum,记录数；maxlength,内容的最大长度(byte)
 *		     typeType,表的类型"U" or "V"
 *
 * 返回值  ：域的数目，<=0时，错误
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_getTableInfo
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename, jobjectArray tableinfo, jintArray maxlength, jbyteArray tableType)
{
    char prtbuf[64];
	memset(prtbuf,0,64);
	jint fieldnums = 0;
	char tn[128],tmpbuf[128],curdb[128];
	memset(tn,0,128);
	memset(curdb,0,128);

	int arrlen = jEnv->GetArrayLength(tablename); //表名
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);


	if(datafmt) 
	{
		free(datafmt);
		datafmt=NULL;
	}
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);

	arrlen = jEnv->GetArrayLength(dbname); //参数库名
	jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)curdb);
	strcpy(func.dbname,curdb);
	


	char tabletype;
	if(db.GetTableInfo(&func,&datafmt,&tabletype)!=CS_FAIL)
	{

		fieldnums = func.colnum;
		int collen = -9999;
		jbyteArray field;
		for(int i=0; i<fieldnums; i++)
		{

			if((datafmt+i)->maxlength > collen)
				collen = (datafmt+i)->maxlength;
			memset(tmpbuf,0,128);
			sprintf(tmpbuf,"%s",(datafmt+i)->name);//域名
			field = (jbyteArray)jEnv->GetObjectArrayElement(tableinfo,i*3);
			jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
			memset(tmpbuf,0,128);
			sprintf(tmpbuf,"%d",(datafmt+i)->datatype);//数据类型
			field = (jbyteArray)jEnv->GetObjectArrayElement(tableinfo,i*3+1);
			jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);

			memset(tmpbuf,0,128);
			if(datafmt[i].status & CS_KEY)//是否为主键
			{
				sprintf(tmpbuf,"%d",1);
			}
			else
			{
				sprintf(tmpbuf,"%d",0);
			}
			field = (jbyteArray)jEnv->GetObjectArrayElement(tableinfo,i*3+2);
			jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);				

		}
		jEnv->SetIntArrayRegion(maxlength,0,1,(jint*)&collen);
		jEnv->SetByteArrayRegion(tableType,0,1,(jbyte*)&tabletype);
	}
	else
	{
		fieldnums = -1;
	}

	if(datafmt) {free(datafmt);datafmt=NULL;}

	return fieldnums;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_openTableInfo
 * 功能描述：得到表信息
 * 参数    ：tablename,表名
 *		     
 *
 * 返回值  ：域的数目，<=0时，错误
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_openTableInfo
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename)
{
	jint fieldnums = 0;
	char tn[128],curdb[128];
	memset(tn,0,128);
	memset(curdb,0,128);

	int arrlen = jEnv->GetArrayLength(tablename); //表名
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);

	if(datafmt) {free(datafmt);datafmt=NULL;}
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);
	jEnv->GetByteArrayRegion(dbname,0,jEnv->GetArrayLength(dbname),(jbyte*)curdb);
	strcpy(func.dbname,curdb);

	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{
		fieldnums = func.colnum;
	}
	else
	{
		fieldnums = -1;
	}
	return fieldnums;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_closeTableInfo
 * 功能描述：清除表信息
 * 参数    ：
 *		     
 *
 * 返回值  ：
 *
 * 
 * --------------------------------------------------
 */

JNIEXPORT void JNICALL Java_com_daqo_dbinfo_Dlllink_closeTableInfo
  (JNIEnv *jEnv, jobject jthis)
{
	memset(&func,0,sizeof(FUNC_STR));
	if(datafmt) {free(datafmt);datafmt=NULL;}
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_readdata
 * 功能描述：得到记录
 * 参数    ：buf,记录数据存放的缓冲区,1行1行鱼贯而放数据
 *		     
 *
 * 返回值  ：值的个数
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_readdata
  (JNIEnv *jEnv, jobject jthis, jobjectArray buf)
{

	if((datafmt==NULL) || (pt==NULL))
		return -1;
	if(func.ret_roxnum == 0) return 0;
	char tmpbuf[256];
	int ptcnt = 0;
	jint count = 0;
	for(int i =0;i<func.ret_roxnum;i++)//row
	{

		for(int j =0;j<colcount;j++)
		{
			for(int k =0;k<func.colnum;k++)
			{
				if(func.col[k].select!=YES)		continue;

				int od = func.col[k].order_d;
				if(od!=j) continue;
				count++;
				memset(tmpbuf,0,256);
				jbyteArray field = (jbyteArray)jEnv->GetObjectArrayElement(buf,i*colcount + j);
				switch ((datafmt+k)->datatype)
				{
					case CS_TINYINT_TYPE:
					{
						unsigned char bb = (unsigned char)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",bb);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_SMALLINT_TYPE:
					{
						short sint = (short)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",sint);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_INT_TYPE:
					{
						int  intrtn = (int)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",intrtn);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_LONG_TYPE:
					{
						long longrtn = (long)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%d",longrtn);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_REAL_TYPE:
					/*{
						break;
					}*/
					case CS_FLOAT_TYPE:
					{
						double  frtn = (double)db.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
						sprintf(tmpbuf,"%f",frtn);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_BINARY_TYPE:
					case CS_CHAR_TYPE:
					case CS_VARCHAR_TYPE:
					{
						db.GetData(i,j,&func,datafmt,pt,ptcnt,tmpbuf);
						jEnv->SetByteArrayRegion(field,0,strlen(tmpbuf),(jbyte*)tmpbuf);
						break;
					}
					case CS_DECIMAL_TYPE:
					{
						break;
					}
				}//switch
				break;//new add
			}//for k
		}//for j
	}//for i
	if(datafmt) {	free (datafmt);datafmt=NULL;}
	if(pt)	{	free(pt);pt=NULL;	}
	return count;

}



/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_ReadRecorders
 * 功能描述：读数据库
 * 参数    ：tablename,表名;fieldname,域名;  where,条件;orderfn,排序域名
 *		   fieldcount,选择域的数量;bdistinct,是否选择唯一值
 *
 * 返回值  ：值的个数,<0时出错。
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_readRecorders
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename, jbyteArray fieldname, jbyteArray where, jbyteArray orderfn, jintArray fieldcount, jboolean bdistinct)
{

	char tn[128],ofn[128],curdb[128];//表名,排序域名
	memset(tn,0,128);
	memset(ofn,0,128);
	memset(curdb,0,128);
	char *fn = NULL,*we = NULL;//选择域名，条件
	int arrlen = 0;
	arrlen = jEnv->GetArrayLength(fieldname);//选择域名
	if(arrlen>0)
	{
		fn = (char*)malloc(arrlen+1);
		memset(fn,0,arrlen+1);
		jEnv->GetByteArrayRegion(fieldname,0,arrlen,(jbyte*)fn);
	}
	else
	{
		fn = (char*)malloc(1+1);
		memset(fn,0,2);
		fn[0] = '*';
	}

	arrlen = jEnv->GetArrayLength(where);//条件
	if(arrlen>0)
	{
		we = (char*)malloc(arrlen+1);
		memset(we,0,arrlen+1);
		jEnv->GetByteArrayRegion(where,0,arrlen,(jbyte*)we); 
	}

	arrlen = jEnv->GetArrayLength(tablename); //表名
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);
	
	arrlen = jEnv->GetArrayLength(orderfn);//排序域名
	if(arrlen>0)	
		jEnv->GetByteArrayRegion(orderfn,0,arrlen,(jbyte*)ofn);

	CS_RETCODE      retcode = 0; 

	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);

	jEnv->GetByteArrayRegion(dbname,0,jEnv->GetArrayLength(dbname),(jbyte*)curdb);
	strcpy(func.dbname,curdb);
	
	if(datafmt) {free(datafmt);datafmt=NULL;}
	if(pt) {free(pt);pt = NULL;}
     
	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{		
		FDNAME *parsedfdname = NULL;

		colcount = parsefieldstring(fn,(void**)&parsedfdname);
		int orderf = (ofn[0]==0) ? FALSE : TRUE;
		func.distinctflag = bdistinct;
		if(colcount > 0 && parsedfdname[0].fdname[0] == '*')//全选
		{
			colcount = func.colnum;
			for(int i=0; i<func.colnum; i++)
			{
				func.col[i].condition = NO;
				func.col[i].select = YES;
				func.col[i].order_d = i; //order data,used to column order ,must be 赋值
				func.col[i].order = -1;
			#ifdef WIN32
				if(orderf && !stricmp(datafmt[i].name,ofn))
			#else
				if(orderf && !strcasecmp(datafmt[i].name,ofn))
			#endif
				func.col[i].order = 0;	//used to  order by,需要就赋值				
			}
		}
		else
		{
			for(int i=0; i<func.colnum; i++)
			{
				func.col[i].condition = NO;
				func.col[i].select = NO;
				func.col[i].order = -1;
				func.col[i].order_d = -1;
				for(int j=0; j<colcount; j++)
				{
				#ifdef WIN32
					if(!stricmp(datafmt[i].name,parsedfdname[j].fdname))
				#else
					if(!strcasecmp(datafmt[i].name,parsedfdname[j].fdname))
				#endif
					{
						func.col[i].select = YES;
						func.col[i].order_d = j;
					}
				}
			#ifdef WIN32
				if(orderf && !stricmp(datafmt[i].name,ofn))
			#else
				if(orderf && !strcasecmp(datafmt[i].name,ofn))
			#endif
					func.col[i].order = 0;
			}
		} 

		jEnv->SetIntArrayRegion(fieldcount,0,1,(jint*)&colcount);

		if(parsedfdname) free(parsedfdname);

		func.func = SEL_RECORDERS;
		if(we != NULL)
		{
			strcpy(func.isql,we);
			func.func = SEL_ISQL_RECORDERS;
		}
		//printf("%s step 1\n",tn);
		if((retcode = db.SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
		{
			retcode = func.ret_roxnum * colcount;
		}
		else
			retcode = -2;
		//printf("%s step 2\n",tn);
	}
	else
		retcode = -1;

	if(fn) free(fn);
	if(we) free(we);
	if(retcode<0)
	{
		if(datafmt) {free(datafmt);datafmt=NULL;}
		if(pt) {free(pt);pt = NULL;}
	}
	return retcode;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_insertARecord
 * 功能描述：插入一条记录数据库
 * 参数    ：jobjectArray,存放记录值的数组byte[][]
 *		   
 *
 * 返回值  ：值的个数,<0时出错。
 *
 *在使用此函数前，要先使用Java_com_daqo_dbinfo_Dlllink_openTableInfo。
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_insertARecord
  (JNIEnv *jEnv, jobject jthis, jobjectArray colvalue)
{
	jint rtn = -1;
	int fieldnums = func.colnum;
	int arrlen = jEnv->GetArrayLength(colvalue);
	if((fieldnums != arrlen) || (datafmt==NULL))
	{
		rtn = -2;//输入的记录长度不对
	}
	else
	{
		func.func = INS_ONE_RECORDER;

		int ptcnt = 0;
		char tmp[256];
		for(int i =0;i<fieldnums;i++)
		{
			jbyteArray colarr = (jbyteArray)jEnv->GetObjectArrayElement(colvalue,i);
			arrlen = jEnv->GetArrayLength(colarr);
			memset(tmp,0,256);
			jEnv->GetByteArrayRegion(colarr,0,arrlen,(jbyte*)tmp);
			switch ((datafmt+i)->datatype)
			{
				case CS_BINARY_TYPE:
				{
					break;
				}
				case CS_TINYINT_TYPE:
				{
					func.col[i].val.tinyvalue = atoi(tmp);
					break;
				}
				case CS_SMALLINT_TYPE:
				{
					func.col[i].val.smallvalue = atoi(tmp);
					break;
				}
				case CS_INT_TYPE:
				{
					func.col[i].val.intvalue = atoi(tmp);
					break;
				}
				case CS_LONG_TYPE:
				{
					func.col[i].val.intvalue = atoi(tmp);
					break;
				}
				case CS_REAL_TYPE:
				{
					func.col[i].val.realvalue = (float)atof(tmp);
					break;
				}
				case CS_FLOAT_TYPE:
				{
					func.col[i].val.floatvalue = atof(tmp);
					break;
				}
				case CS_CHAR_TYPE:
				case CS_VARCHAR_TYPE:
				{
					strcpy(func.col[i].val.charvalue,tmp);
					break;
				}
				case CS_DECIMAL_TYPE:
				{
					break;
				}
			}//switch
		}//for
	}//if

	if(db.InsertOneRcd(&func,datafmt)==CS_SUCCEED)
		rtn = CS_SUCCEED;
	
#ifdef INCEMS
	//插入到高级应用实时库
	
#endif 


	return rtn;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_updateARecord
 * 功能描述：更新一条记录
 * 参数    ：colvalue,更新的记录值，前几列为主键原值;field,为记录的列号，前几个值为主键的列号
 *		     keycolnums,主键的数目
 *
 * 返回值  ：>0,成功；否则失败
 *
 *在使用此函数前，要先使用Java_com_daqo_dbinfo_Dlllink_openTableInfo。
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_updateARecord
  (JNIEnv *jEnv, jobject jthis, jobjectArray colvalue, jintArray field, jint keycolnums)
{
	CS_RETCODE      retcode = 0; 

	func.roxnum = 1;
	int fieldArrLen = jEnv->GetArrayLength(field);
	
	int *pfield = new int[fieldArrLen];

	int i,j;
	char tmp[256];
	for(i=0;i<fieldArrLen;i++)
	{
		jEnv->GetIntArrayRegion(field,i,1,(jint *)(pfield+i));//列序号取出。
	}



	func.func = UPD_ONE_RECORDER;
	
	for(i=0; i<func.colnum; i++)
	{
		func.col[i].condition = NO;
		func.col[i].select = NO;

		for( j =0;j<keycolnums;j++)//关键字段
		{
			if(i==pfield[j])
			{
				func.col[i].condition = YES;
				jbyteArray colarr = (jbyteArray)jEnv->GetObjectArrayElement(colvalue,j);
				int arrlen = jEnv->GetArrayLength(colarr);
				memset(tmp,0,256);
				jEnv->GetByteArrayRegion(colarr,0,arrlen,(jbyte*)tmp);

				switch ((datafmt+i)->datatype)
				{
					case CS_BINARY_TYPE:
					{
						break;
					}
					case CS_TINYINT_TYPE:
					{
						func.col[i].val_o.tinyvalue = atoi(tmp);
						break;
					}
					case CS_SMALLINT_TYPE:
					{
						func.col[i].val_o.smallvalue = atoi(tmp);
						break;
					}
					case CS_INT_TYPE:
					{
						func.col[i].val_o.intvalue = atoi(tmp);
						break;
					}
					case CS_LONG_TYPE:
					{
						func.col[i].val_o.intvalue = atoi(tmp);
						break;
					}
					case CS_REAL_TYPE:
					case CS_FLOAT_TYPE:
					{
						func.col[i].val_o.floatvalue = atof(tmp);
						break;
					}
					case CS_CHAR_TYPE:
					case CS_VARCHAR_TYPE:
					{
						strcpy(func.col[i].val_o.charvalue,tmp);
						break;
					}
					case CS_DECIMAL_TYPE:
					{
						break;
					}
				}//switch

			}//if
		}//for j //关键字段

		for(j=keycolnums;j<fieldArrLen;j++)
		{
			if(i==pfield[j])
			{
				func.col[i].select = YES;
				jbyteArray colarr = (jbyteArray)jEnv->GetObjectArrayElement(colvalue,j);
				int arrlen = jEnv->GetArrayLength(colarr);
				memset(tmp,0,256);
				jEnv->GetByteArrayRegion(colarr,0,arrlen,(jbyte*)tmp);
				switch ((datafmt+i)->datatype)
				{
					case CS_BINARY_TYPE:
					{
						break;
					}
					case CS_TINYINT_TYPE:
					{
						func.col[i].val.tinyvalue = atoi(tmp);
						break;
					}
					case CS_SMALLINT_TYPE:
					{
						func.col[i].val.smallvalue = atoi(tmp);
						break;
					}
					case CS_INT_TYPE:
					{
						func.col[i].val.intvalue = atoi(tmp);
						break;
					}
					case CS_LONG_TYPE:
					{
						func.col[i].val.intvalue = atoi(tmp);
						break;
					}
					case CS_REAL_TYPE:
					{
						func.col[i].val.realvalue = (float)atof(tmp);
						break;
					}
					case CS_FLOAT_TYPE:
					{
						func.col[i].val.floatvalue = (float)atof(tmp);
						break;
					}
					case CS_CHAR_TYPE:
					case CS_VARCHAR_TYPE:
					{
						strcpy(func.col[i].val.charvalue,tmp);
						break;
					}
					case CS_DECIMAL_TYPE:
					{
						break;
					}
				}//switch
			}
		}//for j

	}
	if(pfield) free(pfield);
	int ret = db.UpdateOneRcd(&func,datafmt);

#ifdef INCEMS
	//更新高级应用实时库

#endif 


	//在金华现场发现修改遥测参数表中的一个系数值时，除两个站外，所有的记录都变成了同样的值
	//为此增加记录调试信息的代码段
	//debug begin
	retcode = 0; 
    CS_CHAR *cmdbuf, *cmdbuf1,*cmdbufc,*cmdbufc1; 
    CS_INT          beginflag,beginflagc; 
    CS_INT          affectrow = 0; 
    CS_CHAR        tempstr[600]; 
    

    if ((func.func != UPD_ONE_RECORDER)|| (func.colnum<1)) return (ret); 
    
	cmdbuf = (CS_CHAR *) malloc (EX_BUFSIZE); 
    cmdbuf1 = (CS_CHAR *) malloc (EX_BUFSIZE); 
    if ((cmdbuf == (CS_CHAR *)NULL)|| (cmdbuf1 == (CS_CHAR *)NULL))        return ret; 
    memset (cmdbuf, 0, EX_BUFSIZE); 
    memset (cmdbuf1, 0, EX_BUFSIZE); 

	cmdbufc = (CS_CHAR *) malloc (EX_BUFSIZE); 
    cmdbufc1 = (CS_CHAR *) malloc (EX_BUFSIZE); 
    
    beginflag = YES; 
	beginflagc = YES; 
    sprintf (cmdbuf, UPDATE_ANY_ONE_RECORDER, func.table); 

  
    for (i=0; i<func.colnum; i++) {
        if (func.col[i].select != YES && func.col[i].condition != YES) continue;

		if (func.col[i].select == YES){
            if (beginflag!= YES)
                strcat (cmdbuf, ","); 
            beginflag = NO; 
			FromDataToStr (datafmt+i, tempstr, & (func.col[i].val)); 
            sprintf (cmdbuf1, "%s=%s", datafmt[i].name, tempstr); 
            strcat (cmdbuf, cmdbuf1); 
        }

		if (func.col[i].condition == YES)	{
            if (beginflagc == YES) {
                sprintf (cmdbufc, " where "); 
                beginflagc = NO; 

            }
            else strcat (cmdbufc, " and "); 

			FromDataToStr (datafmt+i, tempstr, & (func.col[i].val_o)); 
			sprintf (cmdbufc1, "%s=%s", datafmt[i].name, tempstr); 
			strcat (cmdbufc, cmdbufc1); 
		}
    }
//    sprintf (cmdbuf1, UPDATE_ANY_ONE_RECORDER_CONDITION, func->his.sname);
    strcat (cmdbuf, cmdbufc); 
    free (cmdbuf1); 
	free (cmdbufc); 
	free (cmdbufc1); 
    
    affectrow = 0; 

    if (affectrow<1) retcode = CS_FAIL; 
	
	RdbDebugMesg("%s\n",cmdbuf);

    free (cmdbuf); 
    return ret; 
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_removeARecord
 * 功能描述：删除一条记录
 * 参数    ：where,条件
 *		   
 *
 * 返回值  ：
 *
 *在使用此函数前，要先使用Java_com_daqo_dbinfo_Dlllink_openTableInfo。
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_removeARecord
  (JNIEnv *jEnv, jobject jthis, jbyteArray where)
{
	CS_RETCODE      retcode = 0; 

	char tmp[MAX_CHAR_BUF];
	memset(tmp,0,MAX_CHAR_BUF);

	int arrlen = jEnv->GetArrayLength(where);
	jEnv->GetByteArrayRegion(where,0,arrlen,(jbyte*)tmp);

	func.func = DEL_ISQL_RECORDERS;
	func.roxnum = 1;
	func.distinctflag = 0;
	strcpy(func.isql,tmp);

	char pt;
#ifdef INCEMS
	//删除高级应用实时库
	//需要从where中提取关键字的值，并在表中检索。
	//如果这个方案不可行，则需要新的接口来实现。
#endif 

	return db.DeleteRcds(&func,datafmt,&pt);

}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_reCall
 * 功能描述：加载实时库
 * 参数    ：server,服务器名数组；name，表名数组(可能是视)
 *		   单个表加载
 *
 * 返回值  ：=0,加载失败
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_reCall
  (JNIEnv *jEnv, jobject jthis, jobjectArray serverpara, jobjectArray namepara)
{
	int size = jEnv->GetArrayLength(serverpara);
	jint rtn = 1;
	if(size == 0) return 0;
	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	char server[64];
	char name[64];
	for(int i=0;i<size;i++)
	{
		memset(server,0,64);
		memset(name,0,64);

		jbyteArray serverarr = (jbyteArray)jEnv->GetObjectArrayElement(serverpara,i);
		jbyteArray namearr = (jbyteArray)jEnv->GetObjectArrayElement(namepara,i);
		jEnv->GetByteArrayRegion(serverarr,0,jEnv->GetArrayLength(serverarr),(jbyte*)server);//服务器
		jEnv->GetByteArrayRegion(namearr,0,jEnv->GetArrayLength(namearr),(jbyte*)name);//表名
		rtn = rtn*rtdbproc.Recall(user,password,server,name);
	}
	return rtn;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_reloadRtdb
 * 功能描述：加载实时库
 * 参数    ：server,服务器名数组；
 *		   整个服务器加载
 *
 * 返回值  ：=0,加载失败
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_reloadRtdb
  (JNIEnv *jEnv, jobject jthis, jobjectArray serverpara)
{
	int size = jEnv->GetArrayLength(serverpara);
	jint rtn = 1;
	if(size == 0) return 0;
	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	char server[64];
	for(int i=0;i<size;i++)
	{
		memset(server,0,64);
		jbyteArray serverarr = (jbyteArray)jEnv->GetObjectArrayElement(serverpara,i);
		jEnv->GetByteArrayRegion(serverarr,0,jEnv->GetArrayLength(serverarr),(jbyte*)server);//服务器
		rtn = rtn*rtdbproc.ReloadRtdb(user,password,server);
	}
	return rtn;	
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_reloadMode
 * 功能描述：加载实时库
 * 参数    ：load_type,未用；
 *		   全部加载
 *
 * 返回值  ：=0,加载失败
 *
 *
 * --------------------------------------------------
 */



JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_reloadMode
  (JNIEnv *jEnv, jobject jthis, jint load_type)
{
	jint rtn = 0;
	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	rtn = rtdbproc.ReloadMode(user,password,load_type);
	return rtn;	

}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_makeBcp
 * 功能描述：生成bcp文件
 * 参数    ： groupno,组号；type,表的类型代码(前置机定义),tablename,表名
 *
 * 返回值  ：生成文件的位置和名称
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_sendMsgToFert
  (JNIEnv *jEnv, jobject jthis, jint groupno, jbyte type, jbyteArray tablename)
{
	char tb[128];
	memset(tb,0,128);
	int arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return -1;

	MailBox mb(groupno,"FEP");
	MailBox pub_mb(groupno,"PUBFEP"); //增加加载实时库往III区发送邮件功能。
	char buf[128];
	FE_MAIL_STRUCT* mail_struct = (FE_MAIL_STRUCT*)buf;
	memset(mail_struct, 0, sizeof(FE_MAIL_STRUCT));	//初始化

	mail_struct->mail_type = LOAD_PARA;
	mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT);
// 	Fe_mail::send_to_fe(m_mail_struct, m_mail_struct->mail_lenth);

// 	memset(buf,0,128);
// 	buf[0] = type;
// 	strcpy(buf+1,tb);
	int len = mail_struct->mail_lenth;
	int result = mb.SendSrvRequest("123456",buf,len,FALSE);	//send mail to client,return TRUE if OK,called by server when result is to be sent
	pub_mb.SendPubSrvRequest("123456",buf,len,FALSE);	//发往III区前置
	return result;											//修改为可以发送主备前置机
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_makeBcp
 * 功能描述：生成bcp文件
 * 参数    ： groupno,组号；type,表的类型代码(前置机定义),tablename,表名
 *
 * 返回值  ：生成文件的位置和名称
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_sendMsgToWeb
  (JNIEnv *jEnv, jobject jthis)
{
	EventProc event;
	SYS_EVENT sysevent;
	memset(&sysevent,0,sizeof(SYS_EVENT));
	sysevent.PrintOut = 0;
	sysevent.DiskOut = 0;
	sysevent.ToneOut = 0;
	sysevent.GraphOut = 0;
	sysevent.DispOut = 1;

	sysevent.sort = ENTCLASS_WEB;
	sysevent.type = ENTTYPE_NULL;//
	sysevent.grpno = 1;
	return event.Save(&sysevent, 1);	
}
/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_makeBcp
 * 功能描述：生成bcp文件
 * 参数    ： groupno,组号；type,表的类型代码(前置机定义),tablename,表名
 *
 * 返回值  ：生成文件的位置和名称
 *
 *
 * --------------------------------------------------
 */
JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_sendEventMsg
  (JNIEnv *jEnv, jobject jthis, jbyteArray username,jbyteArray tablename,jbyteArray hostname)
{
	
	char tb[128];
	memset(tb,0,128);
	int arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return -1;

	char usr[128];
	memset(usr,0,128);
	arrlen = jEnv->GetArrayLength(username);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(username,0,arrlen,(jbyte*)usr);
	else
		jEnv->GetByteArrayRegion(username,0,arrlen,(jbyte*)"");


	char htn[24];
	memset(htn,0,24);
	arrlen = jEnv->GetArrayLength(hostname);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(hostname,0,arrlen,(jbyte*)htn);
	else
		jEnv->GetByteArrayRegion(hostname,0,arrlen,(jbyte*)"");
	NetStatus cfg;
	
	if(strcmp(usr,"")==0)
		sprintf(usr,"%s","调试用户");

	char * hostdesc=cfg.GetNodeName(1,htn);

	sprintf(usr,"%s 在 %s 上修改了%s",usr,hostdesc,tb);

	EventProc event;
	SYS_EVENT sysevent;
	memset(&sysevent,0,sizeof(SYS_EVENT));
	sysevent.PrintOut = 1;
	sysevent.DiskOut = 1;
	sysevent.ToneOut = 1;
	sysevent.GraphOut = 1;
	sysevent.DispOut = 1;

//	time_t  cur_time;
//	time(&cur_time);//得到1970年以来今天的秒数
    //cur_time -= (24*60*60);//减去一天的秒数，得到昨天和今天相同的时分秒
//	CTime etime(cur_time);


	Rdb_Time  rdbtime;
	RDB_DATETIME_MS_STRU  curclock;

	rdbtime.RdbGetCurClock(&curclock);

//	sprintf(debugmsg,"%.4d-%.2d-%.2d  %.2d:%.2d:%.2d.%.3d  ",curclock.year,curclock.month,
  //   curclock.day,curclock.hour,curclock.minute,curclock.second,curclock.msecond);
 	sysevent.ymd=curclock.year*10000+curclock.month*100+curclock.day;
 	sysevent.hmsms=curclock.hour*10000000+curclock.minute*100000+curclock.second*1000;

//	sysevent.ymd=etime.GetYear()*10000+etime.GetMonth()*100+etime.GetDay();
//	sysevent.hmsms=etime.GetHour()*10000000+etime.GetMinute()*100000+etime.GetSecond()*1000;

	sysevent.sort = ENTCLASS_OPERATION;
	sysevent.type = ENTTYPE_USERDEF;//
	sysevent.grpno = 1;
	strcpy(sysevent.text,usr);

	return event.Save(&sysevent, 1);
	return 1;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  Java_com_daqo_dbinfo_Dlllink_getAMaxMinVal
 * 功能描述：执行select 内嵌函数
 * 参数    ： tablename,表名；fieldname，字段名，val返回值 
 *
 * 返回值 ==0, 失败。否则成功。
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_getAMaxMinVal
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname,jbyteArray tablename, 
	jbyteArray fieldname, jbyte flag)
{
	char dbn[64],tb[128],fd[128];

	memset(dbn,0,64);
	int arrlen = jEnv->GetArrayLength(dbname);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)dbn);
	else
		return false;

	memset(tb,0,128);
	arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return false;

	memset(fd,0,128);
	arrlen = jEnv->GetArrayLength(fieldname);
	if(arrlen > 0)
		 jEnv->GetByteArrayRegion(fieldname,0,arrlen,(jbyte*)fd);
	else return false;

	CS_RETCODE		retcode;

	pt = NULL;
	if(dbn[0] == '\0') return false;
	strcpy(func.dbname,dbn);
	func.func = SEL_ISQL_RESULTS;
	char SQLWord[256];
	memset(SQLWord,0,256);
	if(flag == 0)
	{
	sprintf(SQLWord,"select distinct %s from %s where %s = \
		(select max(%s) from %s)",fd,tb,fd,fd,tb);
	}
	else if(flag == 1)
	{
	sprintf(SQLWord,"select distinct %s from %s where %s = \
		(select min(%s) from %s)",fd,tb,fd,fd,tb);
	}
	sprintf(func.isql,"%s",SQLWord);
	
	retcode = db.SelectIsqlResults(&func,&datafmt,&pt);
    if ((retcode != CS_SUCCEED) || (func.ret_roxnum<1))
	{
	    if(datafmt) 
		{
			free(datafmt);
			datafmt = NULL;
		}
	    if(pt)
		{
			free(pt);
			pt = NULL;
		}
		return false;
	}

	func.col[0].order_d = 0;
	func.col[0].select=YES;

	int ptcnt = 0;
	int  frtn = (int)db.GetData(0,0,&func,datafmt,pt,ptcnt,NULL);
	
	if (datafmt) {free(datafmt);;datafmt=NULL;}
	if(pt) {free(pt);pt=NULL;}
	return frtn;		
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_makeBcp
 * 功能描述：生成bcp文件
 * 参数    ： binoutf=true,生成in文件；反之，为out文件
 *		   dbname,数据库名;srvname,服务器名
 *         filename,文件名;username,用户名
 *		   pass,口令
 *
 * 返回值  ：生成文件的位置和名称
 *
 *
 * --------------------------------------------------
 */
  JNIEXPORT jint JNICALL Java_com_daqo_dbinfo_Dlllink_execproc
  (JNIEnv *jEnv, jobject jthis, jint flag, jint devid, jint devtype,jint olddevtype)
  {
		FUNC_STR    func1;
		func1.func=ISQL_COMMAND;
		if(flag == 0)//INSERT
		{
			sprintf(func1.isql,"exec insertdevice %d ,%d",devid,devtype);
		}
		else if(flag == 1)//DELETE
		{
			sprintf(func1.isql,"exec deletedevice %d, %d",devid,devtype);
		}
		else if(flag == 2)//UPDATE
		{
			sprintf(func1.isql,"exec updatedevice %d, %d,%d",devid,olddevtype,devtype);
		}
		strcpy(func1.dbname,"modeldb");
		return db.IsqlCommand(&func1);
  }


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：Java_com_daqo_dbinfo_Dlllink_makeBcp
 * 功能描述：生成bcp文件
 * 参数    ： binoutf=true,生成in文件；反之，为out文件
 *		   dbname,数据库名;srvname,服务器名
 *         filename,文件名;username,用户名
 *		   pass,口令
 *
 * 返回值  ：生成文件的位置和名称
 *
 *
 * --------------------------------------------------
 */

JNIEXPORT jbyteArray JNICALL Java_com_daqo_dbinfo_Dlllink_makeBcp
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbnamearr, jbyteArray srvname, jbyteArray filename, jbyteArray username, jbyteArray pass, jboolean binoutf)
{

#ifdef WIN32
    char dbname[64],srv[64],file[256],user[64],pwd[64];
	memset(dbname,0,64);//数据库名
	memset(srv,0,64);
	memset(file,0,256);
    memset(user,0,64);
	memset(pwd,0,64);

	BOOL bcreatefile = false;

	int arrlen = jEnv->GetArrayLength(dbnamearr);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(dbnamearr,0,arrlen,(jbyte*)dbname);
	else
		strcpy(dbname,"modeldb");

	arrlen = jEnv->GetArrayLength(srvname);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(srvname,0,arrlen,(jbyte*)srv);
	else
		strcpy(srv,"nbdb");

	arrlen = jEnv->GetArrayLength(filename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(filename,0,arrlen,(jbyte*)file);
	else
	{
		if(binoutf)
		  strcpy(file,"c:\\bcpin.bat");
		else
			strcpy(file,"c:\\bcpout.bat");
	}

	arrlen = jEnv->GetArrayLength(username);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(username,0,arrlen,(jbyte*)user);
	else
		strcpy(user,"sa");

	arrlen = jEnv->GetArrayLength(pass);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(pass,0,arrlen,(jbyte*)pwd);
	else
		strcpy(pwd,"");

	
	int retcode = 0;

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"sysobjects");

	strcpy(func.dbname,dbname);
	
	if(datafmt) {free(datafmt);datafmt=NULL;}
	if(pt) {free(pt);pt = NULL;}

	int i;
	int j;

	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{		
		func.distinctflag = false;
		for( i=0; i<func.colnum; i++ )
		{
			func.col[i].condition = NO;
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].order_d = -1;
	#ifdef WIN32
			if(!stricmp(datafmt[i].name,"name"))
	#else 
			if(!strcasecmp(datafmt[i].name,"name"))
	#endif
			{
				func.col[i].select = YES;
				func.col[i].order_d = 0;
			}
		}

		func.func = SEL_ISQL_RECORDERS;
		strcpy(func.isql," type = 'U' ");

		int ptcnt = 0;

		if(db.SelectRcds(&func,datafmt,&pt) == CS_SUCCEED)
		{
			//bcp modeldb.dbo.pdb_field in  pdb_field -Usa -P  -Snbdb  -c
			char *buf = (char*)malloc(func.ret_roxnum*128);
			memset(buf,0,func.ret_roxnum*128);
			int count = 0;
			char tablebuf[256];
			char tmpbuf[256];
			for( i =0;i<func.ret_roxnum;i++)
			{
				memset(tablebuf,0,256);
				memset(tmpbuf,0,256);
				db.GetData(i,0,&func,datafmt,pt,ptcnt,tablebuf);//表名
				sprintf(tmpbuf,"bcp %s.dbo.%s",func.dbname,tablebuf);
				int len = strlen(tmpbuf);
				for( j= len;j<50;j++)
				{
					strcat(tmpbuf," ");
				}
				strcat(buf,tmpbuf);
				memset(tmpbuf,0,256);
				if(binoutf)
					sprintf(tmpbuf,"in  %s",tablebuf);
				else
					sprintf(tmpbuf,"out %s",tablebuf);
				len = strlen(tmpbuf);
				for(j = len;j<35;j++)//表名的最大长度为30来设计
					strcat(tmpbuf," ");
				strcat(buf,tmpbuf);
				memset(tmpbuf,0,256);
				sprintf(tmpbuf," -U%s -P%s -S%s -c\r\n",user,pwd,srv);
				strcat(buf,tmpbuf);
				if(++count%5 ==0)
					strcat(buf,"\r\n");
			}
			CFile cfile;
			bcreatefile = cfile.Open(file,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
			if(bcreatefile)			
				cfile.Write(buf,strlen(buf));

			cfile.Close();
			if(buf) 
				free(buf);
		}
		else
			retcode = -1;
		if(datafmt) {free(datafmt);datafmt=NULL;}
		if(pt) {free(pt);pt = NULL;}
	}
	else
		retcode = -1;
	jbyteArray rtnarray;
	if(retcode == -1 || !bcreatefile)
	{
		memset(file,0,256);
		rtnarray = jEnv->NewByteArray(256);		
		jEnv->SetByteArrayRegion(rtnarray,0,0,(jbyte*)file); 
	}
	else
	{
		rtnarray = jEnv->NewByteArray(strlen(file));		
		jEnv->SetByteArrayRegion(rtnarray,0,strlen(file),(jbyte*)file); 
	}
	return rtnarray;
#else
	return 0;
#endif
}
