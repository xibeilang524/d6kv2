#ifndef _DM_SQLCA_H
#define _DM_SQLCA_H

#include "dpc_dll.h"

typedef struct sqlca_struct		sqlca_t;
struct sqlca_struct {
    udbyte  sqlcaid[8];
    sdint4  sqlcabc;
    sdint4  sqlcode;
    struct {
        sdint2  sqlerrml;
        udbyte  sqlerrmc[70];
    } sqlerrm;
    udbyte  sqlerrp[8];
    sdint4  sqlerrd[6];
    udbyte  sqlwarn[11];
    udbyte  sqlstate[6];
};

#define SQLCA_HAS_WARN		(sqlca.sqlcode > 0)
#define SQLCA_NOT_FOUND		(sqlca.sqlcode == 100)
#define SQLCA_HAS_ERROR		(sqlca.sqlcode < 0)


#endif //_DM_SQLCA_H
