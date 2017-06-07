/**
@file femail.cpp
@brief 前置平台包装实现之邮件接口实现

@author chenkai 
@version 1.0.0
@date 2016-07-13
*/
#include "fes/platformwrapper.h"
#include "fes/femailstruct.h"
#include "net/netapi.h"
#include "scd/scdapi.h"
#include "pthread.h"

static MailBox s_mail_box(1, SCD_SERVER);  //该MailBox用于发送邮件到SCDDA
static MailBox fep_mail_box(1, FEP_SERVER);		//该mailbox用于发送邮件到前置，用于前置之间的交互
static MailBox recv_mail_box(1, FEP_SERVER);		//该mailbox用于接收前置邮件，用于前置之间的交互
static MailBox pub_mail_box(1,PUB_FEP_SERVER);//发送3区前置之间的邮件
static MailBox recv_pub_mail_box(1,PUB_FEP_SERVER);//接收3区前置之间的邮件

static pthread_mutex_t s_mail_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t fep_mail_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t recv_mail_mutex = PTHREAD_MUTEX_INITIALIZER;

static MAIL_HEAD prot_trg[MAX_RTU_NUM];

int Fe_mail::receive(void** mail, int* lenth)
{
	MAIL_HEAD src;
	MAIL_HEAD trg;
	
	int groupno = 1;
	int ret = 0;
	int pubflag = Net_stat::get_PUBFEP_flag();

	pthread_mutex_lock(&recv_mail_mutex);
	if (pubflag)
	{
		ret = recv_pub_mail_box.RecvSrvResult(&src, &trg, mail, lenth ,PUB_FEP_SERVER, 1, &groupno);
	}
	else
	{
		ret = recv_mail_box.RecvSrvResult(&src, &trg, mail, lenth ,FEP_SERVER, 1, &groupno);
	}
	pthread_mutex_unlock(&recv_mail_mutex);

	//为保护做准备
	if (*mail && ret)
	{
		FE_MAIL_STRUCT* tempmail = (FE_MAIL_STRUCT*)(*mail);
		if (tempmail->mail_type == PROT || tempmail->mail_type == WAVE)
		{
			FE_PROT_MAIL* protmail = (FE_PROT_MAIL*)tempmail->mail_body;
			prot_trg[protmail->rtu_no] = src;
		}
	}
	return ret == 0 ? -1 : 0;
}

int Fe_mail::send_to_scada(void* mail, int lenth, int level)
{
	static int mail_num_success = 0;
	int ret = send(FEP_SERVER, mail, lenth, level);
// 	if (ret == 0)
// 	{
// 		mail_num_success++;
// 		printf( "成功发送邮件数=%d 邮件长度 =%d !\n", mail_num_success, lenth);
// 	}
	return ret;
}

int Fe_mail::send_to_fe(void* mail, int lenth)		//todo
{
	int ret = 1;
	int pubflag = Net_stat::get_PUBFEP_flag();
	pthread_mutex_lock(&fep_mail_mutex);
/*	if (Net_stat::get_PUBFEP_flag())
	{
// 		ret = pub_mail_box.SendPubSrvRequest(PUB_FEP_SERVER, mail, lenth, false, 0);
		ret = 0 ;
	}
	else
	{
		ret = fep_mail_box.SendSrvRequest(FEP_SERVER, mail, lenth, false, 0);		//邮件等级暂定为0级
	}
	*/
	//之前的做法是I区发往I区前置，III区发往III区前置
	//现在改为不分I区III区都发邮件
	if(!fep_mail_box.SendSrvRequest(FEP_SERVER, mail, lenth, false, 0) || !pub_mail_box.SendPubSrvRequest(PUB_FEP_SERVER, mail, lenth, false, 0))	//邮件等级暂定为0级
	{
		ret = 0;
	}
	pthread_mutex_unlock(&fep_mail_mutex);
	return ret == 0 ? -1 : 0;
}

int Fe_mail::send(char* user_name, void* mail, int lenth, int level)
{
	pthread_mutex_lock(&s_mail_mutex);
	int ret = s_mail_box.SendSrvRequest(user_name, mail, lenth, true, level);		//实时数据邮件等级为2,通道事项邮件等级为0
	pthread_mutex_unlock(&s_mail_mutex);

	return ret == 0 ? -1 : 0;
}

int Fe_mail::send_prot_ack( void* mail, int lenth, int rtu_no)
{
	pthread_mutex_lock(&s_mail_mutex);
	int ret = s_mail_box.SendSrvResultToClient(&prot_trg[rtu_no],mail,lenth);		
	pthread_mutex_unlock(&s_mail_mutex);
/*	memset(&prot_trg[rtu_no],0,sizeof(MAIL_HEAD));*/
	return ret == 0 ? -1 : 0;

}

