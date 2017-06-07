#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data())) //  [1/15/2013 ljs]
#define _C2Q(str) (QString::fromLocal8Bit(str))

#endif
