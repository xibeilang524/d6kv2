#ifndef GSTREAMFIELD_H_HEADER_INCLUDED_BC30AA18
#define GSTREAMFIELD_H_HEADER_INCLUDED_BC30AA18
#include "ddef/ddes_comm.h"

struct GstreamField
{
    int32 groupno;

    char tabname[GDB_CODE_LEN];

    char objname[GDB_CODE_LEN];

    char fldname[GDB_CODE_LEN];

};

#endif
