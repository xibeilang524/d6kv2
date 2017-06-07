/**
@file feevent.cpp
@brief 前置平台包装实现之数据库接口实现

@author chenkai 
@version 1.0.0
@date 2016-07-13
*/
#include "fes/platformwrapper.h"
#include "net/netapi.h"
#include "pthread.h"

//在windows下实现unix的gettimeofday接口
#ifdef WIN32
static void gettimeofday(struct timeval *x, void *y)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	x->tv_sec = static_cast<long>(time(NULL));

	//实际上tv_usec应该是微秒，为了节省不必要的计算，此处故意直接等于毫秒值
	//因为get_time函数只需要精确到毫秒
	x->tv_usec = systime.wMilliseconds;
}
#endif

//获取毫秒级时间
void get_time(FETIME* time_ptr)
{
	struct timeval time_val;
	gettimeofday(&time_val, NULL);

	time_t being_time;
	time(&being_time);
	
	//防止转换精度出错
	time_t tmp = static_cast<time_t>(time_val.tv_sec);

	struct tm* p_tm = localtime(&tmp);

	time_ptr->year = static_cast<short>(p_tm->tm_year);
	time_ptr->month = static_cast<unsigned char>(p_tm->tm_mon + 1);
	time_ptr->day = static_cast<unsigned char>(p_tm->tm_mday);
	time_ptr->hour = static_cast<unsigned char>(p_tm->tm_hour);
	time_ptr->minute = static_cast<unsigned char>(p_tm->tm_min);
	time_ptr->second = static_cast<unsigned char>(p_tm->tm_sec);

	//windows下gettimeofday获取的是毫秒，UNIX下是微秒
#ifdef WIN32
	time_ptr->milisecond = static_cast<short>(time_val.tv_usec);
#else
	time_ptr->milisecond = static_cast<short>(time_val.tv_usec / 1000);
#endif
}



static EventProc s_event_proc;
static pthread_mutex_t s_event_mutex = PTHREAD_MUTEX_INITIALIZER;


void Fe_event::make_event(const int& sort, const int& type, float& val,
					 const char* object, const char* member0, 
					 const char* member1, const char* text)
{
	pthread_mutex_lock(&s_event_mutex);

	SYS_EVENT event;
	memset(&event, 0 ,sizeof(SYS_EVENT));

	event.grpno = 1;
	event.sort = sort;
	event.type = type;
	event.val = val;

	strcpy(event.Object, object);
	strcpy(event.member0, member0);
	strcpy(event.member1, member1);
	strcpy(event.text, text);

	event.DiskOut = event.PrintOut = event.DispOut = event.ToneOut = 1;

	struct timeval ttv;
	gettimeofday(&ttv, NULL);

	time_t tmp = static_cast<time_t>(ttv.tv_sec);

	struct tm *ttm = localtime(&tmp);

	event.ymd = (ttm->tm_year + 1900) * 100 * 100 + (ttm->tm_mon + 1) * 100 + ttm->tm_mday;
	event.hmsms= ttm->tm_hour * 100 * 100 * 1000 + ttm->tm_min * 100 * 1000 + ttm->tm_sec * 1000 + ttv.tv_usec / 1000;

	s_event_proc.Save(&event, 1);

	pthread_mutex_unlock(&s_event_mutex);
}


void Fe_event::make_channel_event(float& val,const char* station, const char* channel_order, const char* text)
{
	make_event(ENTCLASS_DEVICE, ENTTYPE_SYS_CH, val, station, channel_order, "", text);
}


void Fe_event::make_rtu_event(float& val,const char* station, const char* rtu_name, const char* text)
{
	make_event(ENTCLASS_DEVICE, ENTTYPE_SYS_RTU, val, station, rtu_name, "", text);
}