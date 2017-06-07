#include "comandwave.h"

// Clr-默认颜色，区分是否使用通道颜色 
Draw_analog* QLinker::AddAnalog(Comtrade* c, Wave_graph* w, int n, QColor clr)
{
	w->set_data_count(c->data_count());
	Draw_analog* pDraw = (Draw_analog*)w->add();
	Analog* pData = c->analog(n);
	if (clr == Qt::color0)
		pDraw->set_color(pData->GetColor());
	else
		pDraw->set_color(clr);
	pDraw->set_caption(pData->channel_name());
	pDraw->set_view_data(pData, pData->MaxValue());
	pDraw->set_comtrade_data(c);
	return pDraw;
}

Draw_digtal* QLinker::AddDigtal(Comtrade* c, Wave_graph* w, int n, QColor clr)
{
	w->set_data_count(c->data_count());
	Draw_digtal* pDraw = (Draw_digtal*)w->add(false);
	Digtal* pData = c->digtal(n);
	if (clr != Qt::color0)
		pDraw->set_color(clr);
	pDraw->set_caption(pData->channel_name());
	pData->Filtrate();
	pDraw->set_view_data(pData);
	pDraw->set_comtrade_data(c);
	return pDraw;
}

Draw_channel* QLinker::AddChannel(Comtrade* c, Wave_graph* w, int n, QColor clr)
{
	Draw_channel* pDraw;
	if ( n < c->AnaChannelNum())
		pDraw = AddAnalog(c, w, n, clr);
	else
		pDraw = AddDigtal(c, w, n - c->AnaChannelNum(), clr);
	return pDraw;
}

void QLinker::AddAnalogs(Comtrade* c, Wave_graph* w)
{
	w->set_data_count(c->data_count());
	w->lock();
	for (int i = 0; i < c->AnaChannelNum(); i++)
		AddAnalog(c, w, i);
	w->alloc_rect();
	w->unlock();
}

void QLinker::AddDigtals(Comtrade* c, Wave_graph* w)
{
	w->set_data_count(c->data_count());
	w->lock();
	for (int i = 0; i < c->DigChannelNum(); i++)
		AddDigtal(c, w, i);
	w->alloc_rect();
	w->unlock();
}

void QLinker::AddChannels(Comtrade* c, Wave_graph* w)
{
	w->set_data_count(c->data_count());
	w->lock();
	int i;
	for (i = 0; i < c->AnaChannelNum(); i++)
		AddAnalog(c, w, i);
	for (i = 0; i < c->DigChannelNum(); i++)
		AddDigtal(c, w, i);
	w->alloc_rect();
	w->unlock();
}