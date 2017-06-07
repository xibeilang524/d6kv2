#ifndef GLOBALDEF_H
#define GLOBALDEF_H
#define _Q2C(str) ((const char *)((str).toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

/* 2007-9-19 added by lyg*/
#define TRY_LOCK QMutexLocker _tryLock(&m_mutex)
#define TRY_LOCKSTOP QMutexLocker _tryLockStop(&m_mutexstop)

#include "sysstr.h"

typedef struct
{
	int32u  grpno;					//0、分号组
	int16u sort;					//1、事件分类号
    int16u type;					//2、事件类型
	int32u ymd;						//3、年月日
	int32u hmsms;					//4、时分秒毫秒
}SENDEVENTDATA;

#define RALARMDEBUG		333
//标题栏的长度
const static int pos_min_x = 0;
const static int pos_min_y = 0;
const static int pos_max_x = 1920;
const static int pos_max_y = 64;

#define PADDING 5

enum Direction{
	UP = 0,
	DOWN=1,
	LEFT,
	RIGHT,
	LEFTTOP,
	LEFTBOTTOM,
	RIGHTBOTTOM,
	RIGHTTOP,
	NONE
};


#endif