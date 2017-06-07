/**
@file procmanager.cpp
@brief 前置平台包装实现之进程管理

@author chenkai 
@version 1.0.0
@date 2016-07-15
*/
#include "fes/platformwrapper.h"
#include "net/netapi.h"

static ProcMan s_proc_manager;

//#ifdef WIN32
//void Proc_manager::start_proc(char *asc_name, char *ch_name, char *username, HWND hwnd /*= NULL*/)
//{
//	s_proc_manager.StartProc(asc_name, ch_name, username, hwnd);
//}
//#else
void Proc_manager::start_proc(char *asc_name, char *ch_name, char *username)
{
	s_proc_manager.StartProc(asc_name, ch_name, username);
	s_proc_manager.RegProc(ch_name);
	s_proc_manager.RegType(MAINDBG,"采集调试");
	s_proc_manager.RegType(G101DBG,"规约调试");

}
//#endif

int Proc_manager::proc_exist(char* proc)
{
	return s_proc_manager.IsProcExist(proc) == 1 ? 0 : -1;
}

int Proc_manager::check_parent(void)
{
	return s_proc_manager.CheckParentProc() == 1 ? 0 : -1;
}

void Proc_manager::end_proc(void)
{
	s_proc_manager.UnRegProc();
	s_proc_manager.EndProc();
}

void Proc_manager::putdbg(int type,char *format,...)
{
	char asc[DEBUG_MAX_LEN],temp[2048*10];
	memset(temp,0,2048*10);

	va_list args;
	va_start(args, format);
	sprintf(asc,"%s",format);
	vsprintf(temp,asc,args);
	s_proc_manager.PutDbg(type,temp);
	va_end(args);
}