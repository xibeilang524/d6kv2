

#if !defined(_MESSAGE_STRUCT)
#define _MESSAGE_STRUCT
#include "webconstant.h"
#include "ThreadStreamData.h"

//通信报文结构
typedef struct {
	int type;						//帧类型
	int length;						//报文长度
	char  registerf;				//注册标志，1-->注册;2-->撤消；0-->无效；
	char  reserved[3];				//为保持字节对齐
	char  messagebuf[MAXBUFSIZE];	//报文缓冲区，如改成动态分配的话，内存分配会非常频繁而影响效率
	char *hugebuf;
}MESSAGE;


//每线程的多流管理,构造1个链表
struct MULTISTREAM
{
	int winno;//窗口号
	CThreadStreamData *pthreadstreamdata;
	struct MULTISTREAM *plist;
	MULTISTREAM()
	{
		pthreadstreamdata = NULL;
		plist = NULL;
	}
	~MULTISTREAM()
	{
		if(pthreadstreamdata) {delete pthreadstreamdata; pthreadstreamdata = NULL;}
		plist = NULL;
	}
};


#endif // !defined(_MESSAGE_STRUCT)