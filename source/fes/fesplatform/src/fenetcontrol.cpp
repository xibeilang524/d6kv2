/**
@file fenetcontrol.cpp
@brief 前置平台包装实现之网络接口实现

@author hongxiang  
@version 1.0.0
@date 2013-11-13
*/

#include "fes/platformwrapper.h"
#include "net/netapi.h"

static NetControl sysnet;

int Fe_net_control::check_time(int year,int month,int day,int hour,int minute,int second,int milisecond)
{
	int ret = sysnet.CheckTime(DEFAULT_GRP_NO, year, month, day, hour, minute, second, milisecond);
	return ret == 1 ? 0 : -1;
}