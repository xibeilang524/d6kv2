#ifndef MSGVIEW_STRING_H_
#define MSGVIEW_STRING_H_
#include <QString>
// #define _Q2C(str) ((const char *)(str.local8Bit().data()))
#define  _Q2C(str) (str.toAscii().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))




#endif

