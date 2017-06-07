#ifndef _GLOBLALDEF_H
#define _GLOBLALDEF_H

#define REALDATA_BUF_SIZE	100 //实时数据断面个数
#define POWERCUTEVENTNUM	50  //事故事项断面个数

void ReadStreamData(int);
void pwrmngQuit(int);

void* EvtProc(void* pVoid);

#endif
