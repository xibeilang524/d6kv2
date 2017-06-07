#ifndef COMANDWAVE_H
#define COMANDWAVE_H

#include <QColor>

#include "comtrade.h"
#include "wavegraph.h"

// 连接格式化后的Comtrade数据和图形
class QLinker
{
public:
	static Draw_analog* AddAnalog(Comtrade* c, Wave_graph* w, int n, QColor clr = Qt::color0);
	static Draw_digtal* AddDigtal(Comtrade* c, Wave_graph* w, int n, QColor clr = Qt::color0);
	static Draw_channel* AddChannel(Comtrade* c, Wave_graph* w, int n, QColor clr = Qt::color0);
	static void AddAnalogs(Comtrade* c, Wave_graph* w);
	static void AddDigtals(Comtrade* c, Wave_graph* w);
	static void AddChannels(Comtrade* c, Wave_graph* w);
};

#endif