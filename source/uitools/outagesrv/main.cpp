#include "initialize.h"
#include "streamdata.h"
#include "tpdata.h"
#include "manageevent.h"
#include "verapi.h"

CInitialize g_init;
CStreamData g_stream;
CTpData g_topoData;
CManageEvent g_manageEvent;


int gbDaNeedStart = FALSE;
int bPwrCutQuit = FALSE;

pthread_t pEvtThread ;


///////////////////事项线程

void* EvtProc(void* pVoid)
{
	while(bPwrCutQuit == FALSE)
	{
		if( g_manageEvent.readEvent() != 0 )
			bPwrCutQuit = TRUE;
		_delay_time(1000*1000);
	}
#ifdef _WINDOWS
	return 0;
#else
	return NULL;
#endif
}


//////////////////////


void pwrmngQuit(int i)
{
	printf("==============收到网络底层的退出信号===============");
	bPwrCutQuit = TRUE;
}


int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("outagesrv,1.1\n");
				printf("outagesrv, 1.1\n");
				return 0;
			}
		}
	}

	int nInit;
	char cStartError = 0;
	nInit = g_init.beginInit();
	if(nInit != 0)
	{
		if(nInit == 1)
			printf("--------------停电管理程序启动错误--------------\n程序已经在运行!\n");
		if(nInit == 2)
			printf("--------------停电管理程序启动错误--------------\n获取实时库出错\n");
		if(nInit == 3)
			printf("--------------停电管理程序启动错误--------------\n设置实时流参数出错\n");
		if(nInit == 4)
			printf("--------------停电管理程序启动错误--------------\n停电管理处理线程启动失败\n");
		g_init.endCleanup();
		return 0;
	}

	printf("--------------停电管理程序启动成功!--------------\n");

	while(bPwrCutQuit == FALSE)
	{
		_delay_time(1000*1000);
		if(g_init.checkParentProc() == FALSE)
			bPwrCutQuit = TRUE;
	}

	g_init.endCleanup();
	return 0;
}

