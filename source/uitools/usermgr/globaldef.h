#ifndef GLOBALDEF_H
#define GLOBALDEF_H


#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

#define ROLETYPE_MANGER		0

#define ATUHTYPE		660	

#endif
