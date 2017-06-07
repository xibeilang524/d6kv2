#ifndef GLOBALDEF_H
#define GLOBALDEF_H
#include <QString>
#include <QColor>

#define  _Q2C(str) (str.toAscii().data())
#define _C2Q( str ) ( QString::fromLocal8Bit( str ) )


const QString PHASES[] = {"A", "B", "C", "N", "None"};
const QColor COLORTABS[] = 
{Qt::yellow, Qt::green, Qt::red, Qt::cyan, Qt::white, Qt::gray};
const QString NEWFILENAME = _C2Q("新建波形文件");

// 游标相关
const QColor TIMELINECOLORS[] =
{Qt::yellow, Qt::gray, Qt::red};
const QString TIMELINECAPTION[] =
{_C2Q("游标"), _C2Q("相对时标零点"),  _C2Q("△t起点")};
const QString TIMELINESTR[] =
{ _C2Q("锁定游标和△t起点"), _C2Q( "解锁游标和△t起点")};
const QString AUTOSTRING[] =
{ _C2Q("自动演示"),  _C2Q("停止演示")};
const QString MATHSTR =  _C2Q("算法: ");
const QString ARITHMETICSTRING[] =
{ _C2Q("全周傅氏"),  _C2Q("半周傅氏"),  _C2Q("差分＋全周傅氏"),  _C2Q("差分＋半周傅氏")};

const QString AVERAGESTR =  _C2Q("设置计算平均值的点数: ");

const QString ANAINFOSTR[] =
{ _C2Q("瞬时值"),
_C2Q("基波相量"),
_C2Q("基波实虚部"),
_C2Q("平均值"),
_C2Q("频率"),
_C2Q("直流"),
_C2Q("%1次谐波"),
_C2Q("∠"),
_C2Q("j"),
_C2Q("选择列..."),
_C2Q("增加列..."),
_C2Q("删除列"),
_C2Q("关联矢量图")};

const QString DIGINFOSTR[] =
{_C2Q("状态%1"),
_C2Q("↓"),
_C2Q("↑"),
_C2Q("游标基准"),
_C2Q("隐藏无变位通道")};

const QString ADDVECTORSTR = _C2Q("添加矢量通道");
const QString NOVECTORSTR = _C2Q("无基准矢量");
const QString ADDHARMSTR = _C2Q("添加谐波通道");
const QString HARMONESTR = _C2Q("基波");

#endif