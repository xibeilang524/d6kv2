// f1jdll.cpp : Defines the initialization routines for the DLL.
//
#if defined (WIN32)
#include "stdafx.h"
#include "f1jdll.h"
#else
#include "p_process.h"
//#include "MmiLoginDialog.h"
#endif

#include "db/dbapi.h"
#include "rdb/rdbapi.h"
#include "usacreport_dllf1j.h"
/////////////nt unix不公用部分

//////////////
//////////////共享内存使用
//#include <sys/types.h>
//#include <sys/shm.h>
//////////////
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CF1jdllApp

#define DBNAMESIZE		64//数据库名长度
#define FIELDNAMESIZE	64//域名的长度
#define MODUSR			2//修改报表用户
#define VIEWUSR			3//查看用户

#if defined (WIN32)
#else
	CProcInfo procinfo;
//	CMmiLoginDialog ploginWnd;
#endif

DBApi db;
FUNC_STR func;
CS_DATAFMT *datafmt = NULL;
void* pt = NULL;
int colcount = 0;//列的数目
FileMan pFileMan;
NetDbg	netdbg;
/////////////////nt unix不公用部分

/////////////////


typedef struct _tagfdname
{
	char fdname[FIELDNAMESIZE];
}FDNAME;

#ifdef WIN32
	BEGIN_MESSAGE_MAP(CF1jdllApp, CWinApp)
		//{{AFX_MSG_MAP(CF1jdllApp)
			// NOTE - the ClassWizard will add and remove mapping macros here.
			//    DO NOT EDIT what you see in these blocks of generated code!
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CF1jdllApp construction

	CF1jdllApp::CF1jdllApp()
	{
		// TODO: add construction code here,
		// Place all significant initialization in InitInstance
	}
	/////////////////////////////////////////////////////////////////////////////
	// The one and only CF1jdllApp object

	CF1jdllApp theApp;
#endif

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

int checkusr()
{
	int right = 1;
#ifndef  WIN32
	/*
	if (ploginWnd.GetLoginRec() != NULL )
	{ 
		char * name = ploginWnd.GetLoginRec()->m_struOperator.username;
		if(NULL == name)
		{
			return 1;
		}
		USEROPERRIGHT_STRU *power = &(ploginWnd.GetLoginRec()->m_struPower);
		if(power == NULL) return 1;

		right = power->modifytable;
		if(right > 0) return MODUSR;
		right = power->viewtable;
		if(right > 0) {
			return VIEWUSR;
		}
	}
	*/
#endif
	return 1;
}

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_startdb
  (JNIEnv *, jobject)
{	
	int ret = 0;
#ifndef WIN32
	ret = procinfo.GetProcNum("dqreport");
	printf("目前的进程数为 %d\n",ret);
	netdbg.RegProc("dqreport");	
	if(ret > 2 || ret < 0) return -1;
#endif
	netdbg.RegProc("dqreport");
	ret = db.Open(DB_OP_WR);
	if(ret < 1) return ret;
	ret = checkusr();	
	return ret;
}

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_closedb
  (JNIEnv *, jobject)
{
	netdbg.UnRegProc();
	return db.Close();
}


JNIEXPORT jbyteArray JNICALL Java_usacreport_dllf1j_getenvvar
  (JNIEnv * jEnv, jobject)
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


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_getTableInfo
  (JNIEnv * jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename, jobjectArray tableinfo, jintArray maxlength, jbyteArray tableType)
{
	char prtbuf[128];
	memset(prtbuf,0,128);
	jint fieldnums = 0;
	char tn[128],tmpbuf[128],curdb[128];
	memset(tn,0,128);
	memset(curdb,0,128);

	sprintf(prtbuf,"Jhongtu Debug: GetTableInfo step1.\n");
//	printf(prtbuf);

	int arrlen = jEnv->GetArrayLength(tablename); //表名
	jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tn);

	sprintf(prtbuf,"Jhongtu Debug: GetTableInfo step2.\n");
//	printf(prtbuf);

	if(datafmt) {free(datafmt);datafmt=NULL;}
	memset(&func,0,sizeof(FUNC_STR));
	func.func = GET_TABLE_INFO;
	strcpy(func.table,tn);

	sprintf(prtbuf,"Jhongtu Debug: GetTableInfo step3.\n");
//	printf(prtbuf);

	arrlen = jEnv->GetArrayLength(dbname); //参数库名
	jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)curdb);
	strcpy(func.dbname,curdb);
	
	sprintf(prtbuf,"Jhongtu Debug: 开始GetTableInfo.\n");
//	printf(prtbuf);


	char tabletype;
	if(db.GetTableInfo(&func,&datafmt,&tabletype)!=CS_FAIL)
	{
		sprintf(prtbuf,"Jhongtu Debug: 结束GetTableInfo.\n");
//		printf(prtbuf);

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


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_openTableInfo
  (JNIEnv * jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename)
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


JNIEXPORT void JNICALL Java_usacreport_dllf1j_closeTableInfo
  (JNIEnv *, jobject)
{
	memset(&func,0,sizeof(FUNC_STR));
	if(datafmt) {free(datafmt);datafmt=NULL;}
}


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_readdata
  (JNIEnv * jEnv, jobject jthis, jobjectArray buf)
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
//---mady -读取实时数据库表，目前支持kwh和analog表--20120331
JNIEXPORT jbyteArray JNICALL Java_usacreport_dllf1j_readRtdbRecorders
(JNIEnv *jEnv, jobject jthis, jbyteArray rtdbtablename, jbyteArray rtdbname,jbyteArray sfield)
{
	char tablename[128],name[128],field[128];
	char tmpbuf[256];
	memset(tablename,0,128);
	memset(name,0,128);
	memset(field,0,128);
	memset(tmpbuf,0,256);
	char prtbuf[128];
	int arrlen = 0;

	memset(prtbuf,0,128);
	sprintf(prtbuf,"mady Debug: readRtdbRecorders step1.\n");
	
	arrlen = jEnv->GetArrayLength(rtdbtablename);//选择表名

	jEnv->GetByteArrayRegion(rtdbtablename,0,arrlen,(jbyte*)tablename);

	arrlen = jEnv->GetArrayLength(rtdbname);//选择代码名称

	jEnv->GetByteArrayRegion(rtdbname,0,arrlen,(jbyte*)name);

	arrlen = jEnv->GetArrayLength(sfield);//选择代码名称
	
	jEnv->GetByteArrayRegion(sfield,0,arrlen,(jbyte*)field);

	Rdb_SqlCli rl;
	rl.RdbSelectFrom(tablename);   //查询的表名称
	rl.RdbSelectField(field);    //查询的字段
	rl.RdbWhereCond("name",RDB_DATATYPE_STRING,name);  //查询的条件

	int	ret = rl.RdbSelCmd(1,"scada","scada");

	int colcount=rl.rdbgetrcdcnt();

	sprintf(tmpbuf,"%.2f",rl.RdbGetVal_float(0,field));
	
	int num = strlen((char*)tmpbuf);

	jbyteArray rtnarray = jEnv->NewByteArray(num);
	
	jEnv->SetByteArrayRegion(rtnarray,0,num,(jbyte*)tmpbuf); 

	return rtnarray;

}

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_readRecorders
  (JNIEnv *jEnv, jobject jthis, jbyteArray dbname, jbyteArray tablename, jbyteArray fieldname, jbyteArray where, jbyteArray orderfn, jintArray fieldcount, jboolean bdistinct)
{
	char tn[128],ofn[128],curdb[128];//表名,排序域名
	memset(tn,0,128);
	memset(ofn,0,128);
	memset(curdb,0,128);
	char *fn = NULL,*we = NULL;//选择域名，条件
	int arrlen = 0;

	char prtbuf[128];
	memset(prtbuf,0,128);

	sprintf(prtbuf,"Jhongtu Debug: readRecorders step1.\n");
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

	sprintf(prtbuf,"Jhongtu Debug: readRecorders step2. %s %s\n",func.table,func.dbname);
//	printf(prtbuf);

	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{		
		FDNAME *parsedfdname = NULL;

		sprintf(prtbuf,"Jhongtu Debug: readRecorders success.\n");
//		printf(prtbuf);

		colcount = parsefieldstring(fn,(void**)&parsedfdname);
		
		//bool orderf = (ofn[0]==0) ? false : true;
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
		
		if((retcode = db.SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
		{
			retcode = func.ret_roxnum * colcount;

		}
		else
			retcode = -2;
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

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_insertARecord
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
					func.col[i].val.realvalue = float(atof(tmp));
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

	return rtn;
}

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_updateARecord
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
	
	int retUpdate = db.UpdateOneRcd(&func,datafmt);
	return func.ret_roxnum;
}


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_removeARecord
  (JNIEnv *jEnv, jobject jthis, jbyteArray where)
{
	CS_RETCODE      retcode = 0; 

	char tmp[MAX_CHAR_BUF];
	memset(tmp,0,MAX_CHAR_BUF);

	int arrlen = jEnv->GetArrayLength(where);
	jEnv->GetByteArrayRegion(where,0,arrlen,(jbyte*)tmp);

	func.func = DEL_ISQL_RECORDERS;
	func.roxnum = 1;
	strcpy(func.isql,tmp);

	char pt;
	return db.DeleteRcds(&func,datafmt,&pt);
}


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_reCall
  (JNIEnv *jEnv, jobject jthis, jobjectArray serverpara, jobjectArray namepara)
{
/*	int size = jEnv->GetArrayLength(serverpara);
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
	return rtn;*/
	return 1;
}


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_reloadRtdb
  (JNIEnv *jEnv, jobject jthis, jobjectArray serverpara)
{
	jint rtn = 1;
/*	int size = jEnv->GetArrayLength(serverpara);
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
	}*/
	return rtn;	
}


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_reloadMode
  (JNIEnv *jEnv, jobject jthis, jint load_type)
{
	jint rtn = 0;
/*	RtdbProc rtdbproc;
	char user[64];
	memset(user,0,64);
	strcpy(user,"scada");

	char password[64];
	memset(password,0,64);
	strcpy(password,"scada");

	rtn = rtdbproc.ReloadMode(user,password,load_type);
	*/
	return rtn;
}


JNIEXPORT jint JNICALL Java_usacreport_dllf1j_sendMsgToFert
  (JNIEnv *jEnv, jobject jthis, jint groupno, jbyte type, jbyteArray tablename)
{
/*	char tb[128];
	memset(tb,0,128);
	int arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)tb);
	else
		return -1;

	MailBox mb(groupno,"FEP");
	char buf[128];
	memset(buf,0,128);
	buf[0] = type;
	strcpy(buf+1,tb);
	int len = strlen(buf);
	int result = mb.SendSrvRequest("600268",buf,len);	//send mail to client,return TRUE if OK,called by server when result is to be sent
	return result; */
	return 1;
}


JNIEXPORT jbyteArray JNICALL Java_usacreport_dllf1j_makeBcp
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

	if(db.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{		
		func.distinctflag = false;
		int i = 0;
		for(i=0; i<func.colnum; i++)
		{
			func.col[i].condition = NO;
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].order_d = -1;
//			if(!strcmp(datafmt[i].name,"name"))
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
			//bcp superdb.dbo.pdb_field in  pdb_field -Usa -P  -Ssacdb  -c
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
				int j = 0;
				for(j= len;j<50;j++)
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

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_NetFileSave
  (JNIEnv * jEnv, jobject jthis, jbyteArray filename, jbyteArray buf1, jint len, jint tableflag)
{
	char file[256];	
	memset(file,0,256);
	char * buffer = NULL;

	int arrlen = jEnv->GetArrayLength(filename);

	if(arrlen>0)
		jEnv->GetByteArrayRegion(filename,0,arrlen,(jbyte*)file);
	else
		return 0;
	
	buffer = (char*)malloc(arrlen);
	memset(buffer,0,arrlen);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(buf1,0,arrlen,(jbyte*)buffer);
	}
	else
		return 0;

	FileLocal filesave((char *)file);
	int lenth;
	lenth = filesave.GetLen();
	void *buf = malloc(lenth);

	if( filesave.Read( buf,lenth) <lenth )
	{
		free(buf);
		return FALSE;
	}
	int ret = pFileMan.Save((char *)file,buf,lenth,0);
	free(buf);
	return ret;
/*	if(pFileMan.Save((char*)file,buffer,arrlen,0))
	{
		if(buffer) free(buffer);
		return 1;
	}
	else
	{
		if(buffer) free(buffer);
		return 0;
	}*/
}

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_ExecuteCmd
  (JNIEnv * jEnv, jobject, jbyteArray dbname, jbyteArray tablename)
{
	CS_RETCODE      retcode = 0; 
	char c_dbname[256];	
	memset(c_dbname,0,256);
	char c_tablename[4096];	
	memset(c_tablename,0,4096);

    func.func=ISQL_COMMAND;
	int arrlen = jEnv->GetArrayLength(dbname);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)c_dbname);
		strcpy(func.dbname,c_dbname);
	}
	else{return 0;}

	arrlen = jEnv->GetArrayLength(tablename);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(tablename,0,arrlen,(jbyte*)c_tablename);
		strcpy(func.isql,c_tablename);
	}
	else{return 0;}
	
	retcode=db.IsqlCommand(&func);
   	return (retcode);
}

JNIEXPORT jdouble JNICALL Java_usacreport_dllf1j_SqlReadRecorder
  (JNIEnv * jEnv, jobject, jbyteArray dbname, jbyteArray sql, jint type)
{
	CS_RETCODE      retcode = 0; 
	char c_dbname[256];	
	memset(c_dbname,0,256);
	char c_sql[4096];	
	memset(c_sql,0,4096);

    func.func=ISQL_COMMAND;
	int arrlen = jEnv->GetArrayLength(dbname);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)c_dbname);
		strcpy(func.dbname,c_dbname);
	}
	else{return 0;}

	arrlen = jEnv->GetArrayLength(sql);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(sql,0,arrlen,(jbyte*)c_sql);
		strcpy(func.isql,c_sql);
	}
	else{return 0;}

	func.func = SEL_ISQL_RESULTS;

	retcode = db.SelectIsqlResults(&func,&datafmt,&pt);
    if ((retcode != CS_SUCCEED) || (func.ret_roxnum<1))
	{
	    if(datafmt) {free(datafmt);datafmt=NULL;}
		if(pt) {free(pt);pt = NULL;}
		return 0;
	}
	double d_temp;
	switch(type)
	{
	case 0://double
		d_temp = *(double*)pt;
		break;
	case 1://float
		break;
	case 2://int
		d_temp = *(int*)pt;
		break;
	case 3:
		break;
	}
	
	if(datafmt) {free(datafmt);datafmt=NULL;}
	if(pt) {free(pt);pt = NULL;}
	return d_temp;
}

JNIEXPORT jint JNICALL Java_usacreport_dllf1j_SqlReadRecorders
(JNIEnv * jEnv, jobject,jbyteArray dbname, jbyteArray sql,jintArray fieldcount){
	CS_RETCODE      retcode = 0; 
	char c_dbname[256];	
	memset(c_dbname,0,256);
	char c_sql[4096];	
	memset(c_sql,0,4096);
	
    func.func=ISQL_COMMAND;
	int arrlen = jEnv->GetArrayLength(dbname);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(dbname,0,arrlen,(jbyte*)c_dbname);
		strcpy(func.dbname,c_dbname);
	}
	else{return 0;}
	
	arrlen = jEnv->GetArrayLength(sql);
	if(arrlen>0)
	{
		jEnv->GetByteArrayRegion(sql,0,arrlen,(jbyte*)c_sql);
		strcpy(func.isql,c_sql);
	}
	else{return 0;}
	
	func.func = SEL_ISQL_RESULTS;
	
	retcode = db.SelectIsqlResults(&func,&datafmt,&pt);
    if ((retcode != CS_SUCCEED) || (func.ret_roxnum<1))
	{
		if(datafmt) {free(datafmt);datafmt=NULL;}
		if(pt) {free(pt);pt = NULL;}
		return -2;
	}
	colcount=func.colnum;
	retcode = func.ret_roxnum * colcount;	//lcg修改返回列字段个数
	jEnv->SetIntArrayRegion(fieldcount,0,1,(jint*)&colcount);//mdy UNIX下返回列数错误，修改为colcount
	return retcode;
}


JNIEXPORT jbyteArray JNICALL Java_usacreport_dllf1j_RString
  (JNIEnv * jEnv, jobject)
{
	jbyteArray rtnarray=0;
	return rtnarray;
}

JNIEXPORT void JNICALL Java_usacreport_dllf1j_WString
  (JNIEnv * jEnv, jobject, jbyteArray name)
{

}
