#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include "sysstr.h"
#include "netapi.h"
#include "sysdef.h"


#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

#define  DES_LEN	64
#define EVENT_VALID_GAP			15 ////收到事项的时间与事项的发生时间的有效间隔
#define DINAMIC_BUF_BLOCK_SIZE	100 ///以100个为一块内存单位进行动态分配内存


typedef struct
{
	SYS_TIME recvTime;//DA程序收到事项的时间！
	SYS_EVENT evt;
} EX_SYS_EVENT;


#endif
