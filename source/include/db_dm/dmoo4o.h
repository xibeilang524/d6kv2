#ifndef DM_OO4O_H
#define DM_OO4O_H

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <wtypes.h>
#include "DPI.h"
#include "DPIext.h"
#include "DPItypes.h"

#include <vector>

using namespace std;

#pragma warning(disable:4251)

extern HANDLE log_file;
#define LOG_FILE_PATH "C:\\dmoo4o.log"

typedef udint4  dm_bool;

extern void     app_debug_log(char* strBuf, ...);
extern void*    dm_malloc(udint4 size);
extern void     dm_free(void* p);
extern int      gbk_to_unicode(char* src, sdint4 len1, char* dst, sdint4 len2);

#define ORA_SUCCESS(code)   (code == OSUCCESS)
#define RET_FAILURE_IF_FALSE(b) {if (!DSQL_SUCCEEDED(b)) {return OFAILURE;}}
#define RET_FAILURE_IF_FALSE_WITH_CLEAN(b,exp) {if (!DSQL_SUCCEEDED(b)) {{exp;} return OFAILURE;}}
#define RET_IF_FALSE(b) {if (!b) {return b;}}
#define RET_IF_FALSE_WITH_CLEAN(b,stmt) {if (!b) {{stmt} return b;}}


dm_bool
dm_get_conf(
    char*       searchstr,
    char*       ip,
    udint2*     port
);

void 
app_print_param_for_debug(
    char*       param_name,
    sdint4      param_id,
    sdint4      c_type,
    sdint4      sql_type,
    sdint4      io_type,
    sdint4      prec,
    sdint4      scale,
    char*       value,
    sdint4      len,
    sdint4      ind,
    sdint4      ind_len
);

#define APP_DEBUG_LOG app_debug_log

#define DM_TSNAME_FILE "tsname.dm"

extern void err_none();
extern void err_sql(char* sql);
extern void err_assert();
extern void err_sig(char* sig);
extern void err_sql2(char* sql);

extern int flt_thread_id;

#ifdef _DEBUG
#define FUNC_ENTER(fname) {\
    char msg[4096];\
	struct tm	*newtime;\
	time_t		ltime;\
    int         thread_id;\
    err_none();\
    thread_id = GetCurrentThreadId();\
    if (thread_id != flt_thread_id)\
    {\
	    time( &ltime );\
	    if (ltime < 0)\
		    ltime = 0x7FFFFFFF;\
	    newtime = localtime(&ltime);\
	    sprintf(msg, "[%04d-%02d-%02d %02d:%02d:%02d-0X%08X | %s::(%d,%s)]",\
		    newtime->tm_year + 1900,\
		    newtime->tm_mon + 1,\
		    newtime->tm_mday,\
		    newtime->tm_hour,\
		    newtime->tm_min,\
		    newtime->tm_sec, GetCurrentThreadId(), fname, __LINE__, __FILE__);\
        strcat(msg, " entering...\n");\
	    DWORD nNumberOfBytesToWrite, NumberOfBytesWritten;\
	    nNumberOfBytesToWrite = (DWORD)strlen(msg);\
	    WriteFile(	log_file, \
		            msg, \
		            nNumberOfBytesToWrite,  \
		            &NumberOfBytesWritten, \
		            NULL );\
    }\
}

#define DM_DIAG_ERR_MSG(b,hstmt)\
{\
    if(!DSQL_SUCCEEDED(b))\
    {\
        char        errmsg[512];\
        dpi_get_diag_rec(DSQL_HANDLE_STMT, hstmt, 1, NULL, errmsg, sizeof(errmsg), NULL);\
        app_debug_log(errmsg);\
    }\
}

#define DM_DIAG_ERR_MSG_WITH_SQL(b,hstmt,sql) \
{\
    char    msg[8192];\
    char    errmsg[512];\
    if(!DSQL_SUCCEEDED(b) && b != DSQL_NO_DATA)\
    {\
        err_sql2((char*)sql);\
        dpi_get_diag_rec(DSQL_HANDLE_STMT, hstmt, 1, NULL, (sdbyte*)errmsg, sizeof(errmsg), NULL);\
        sprintf(msg, "  :[ERR] %s\n%s", errmsg, sql);\
        app_debug_log(msg);\
    }\
}

#define DM_TRACE_SQL(sql) {char msg[8192]; {err_sql((char*)sql); err_sig((char*)sql); sprintf(msg, "  :[SQL] %s\n", sql); app_debug_log(msg);}}
#define DM_ASSERT(exp) {if (!exp) {err_assert(); app_debug_log("[AST] DM_ASSERT FAILED......"); exit(-1);}}
#define DM_CRASH {err_assert(); app_debug_log(" [NOIMP] this function not imp.....::(%d,%s).", __LINE__, __FILE__);}
#else
#define FUNC_ENTER(fname)
#define DM_DIAG_ERR_MSG(hstmt)
#define DM_DIAG_ERR_MSG_WITH_SQL(b,hstmt,sql)
#define DM_TRACE_SQL(sql)
#define DM_ASSERT(exp)
#define DM_CRASH
#endif


class OOLEvar
{
    int a;
};

class OAQCallback
{
    int a;
};

dm_bool 
oo4o_type_to_c_type(
	int     oo4o_type,
	int*    c_type
);

dm_bool 
oo4o_type_to_dm_type(
	int     oo4o_type,
	int*    dm_type
);

dm_bool
oo4o_iotype_to_dm_iotype(
    int     iotype,
    int*    dm_iotype
);

dm_bool
oo4o_get_field_size(
    int     server_type,
    void*   value,
    int*   len
);

DPIRETURN
dm_get_paramid_by_name(
    char*           name,
    dhstmt          stmt,
    udint2*         paramid
);

#endif

