#ifndef DPC_ORA_DLL_H
#define DPC_ORA_DLL_H

#include "sqlda_ora.h"
#include <stdio.h>

/* ----------------- */
/* defines for sqlda */
/* ----------------- */

#define SQLSQLDAAlloc(arg1, arg2, arg3, arg4) sqlald(arg2, arg3, arg4) 

#define SQLSQLDAFree(arg1, arg2) sqlclu(arg2) 

#define SQLNumberPrecV6(arg1, arg2, arg3, arg4) sqlprc(arg2, arg3, arg4)

#define SQLColumnNullCheck(arg1, arg2, arg3, arg4) sqlnul(arg2, arg3, arg4)

#define SQLErrorGetText(arg1, arg2, arg3, arg4) sqlglm(arg2, arg3, arg4)


// oracle Data types and C Data type relative
#define TYPE_ORA_VARCHAR2           1           //->char[n]
#define TYPE_ORA_NUMBER             2           //->char[n](n <= 22)
#define TYPE_ORA_INTEGER            3           //->int
#define TYPE_ORA_FLOAT              4           //->float
#define TYPE_ORA_STRING             5           //->char[n+1]
#define TYPE_ORA_VARNUM             6           //->char[n](n <= 22)
#define TYPE_ORA_DECIMAL            7           //->float
#define TYPE_ORA_LONG               8           //->char[n]
#define TYPE_ORA_VARCHAR            9           //->char[n + 2]
#define TYPE_ORA_ROWID              11          //->char[n]
#define TYPE_ORA_DATE               12          //->char[n]
#define TYPE_ORA_VARRAW             15          //->char[n]
#define TYPE_ORA_RAW                23          //->unsigned char[n]
#define TYPE_ORA_LONG_RAW           24          //->unsigned char[n]
#define TYPE_ORA_UNSIGNED           68          //->unsigned int
#define TYPE_ORA_DISPLAY            91          //->char[n]
#define TYPE_ORA_LONG_VARCHAR       94          //->char[n + 4]
#define TYPE_ORA_LONG_VARRAW        95          //->unsigned char[n + 4]
#define TYPE_ORA_CHAR_OR_CHARF      96          //->char[n]
#define TYPE_ORA_CHARZ              97          //->char[n + 1]


#ifdef __cplusplus
extern "C" {
#endif

DllExport
void
dpc_set_sqlca_ora(
    void*        sqlca
);

DllExport
int
sqlda_ora_describe(
    sdbyte*		    stmt,
    SQLDA*			da,
    udint4         is_param
);

DllExport
int
sqlda_ora_fetch(
    sdbyte*	        curname,
    SQLDA*			da,
	udint4			rows
);

DllExport
int
sqlda_ora_exec(
    sdbyte*	    	stmt,
    SQLDA*			da,
	udint4			rows
);

DllExport
SQLDA*
sqlald(
	int			max_n,
	size_t		max_name,
	size_t		max_i
);

DllExport
void 
sqlclu(
	SQLDA*		da
);

DllExport
void
sqlprc(
	udint4* 	length,
	sdint4*	    precision,
	sdint4*	    scale
);

DllExport
void
sqlnul(
	udint2*	    value_type,
	udint2*	    type_code,
	sdint4*	    null_status
);

DllExport
void 
sqlglm(
	sdbyte*		message_buffer, 
	size_t*		buffer_size,
	size_t*		message_length
);

DllExport
int
sqlda_ora_fetch_ex(
    sdbyte*     curname,
    SQLDA*      da,
    udint4      rows,
	sdint4      orient,
    sdint2      c_type,
    void*       data_ptr,
    sdint4      data_sz,
    sdint4      is_const
);

DllExport
int
sqlda_ora_open_cursor(
    sdbyte*      cur_name,
    SQLDA*       da,
	udint4       rows
);

#ifdef __cplusplus
}
#endif

#endif

