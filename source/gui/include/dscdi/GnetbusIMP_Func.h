#ifndef GNETBUSIMP_FUNC_H_HEADER_INCLUDED_BB046DF9
#define GNETBUSIMP_FUNC_H_HEADER_INCLUDED_BB046DF9

#include "ddef/ddes_comm.h"
//################################################
//############命令码参数说明############
//################################################

//主机状态
//int groupnum = (int )sendCMD(GNBM_GROUPNUM,0,0);
#define GNBM_GROUPNUM		0x100

//int *groupenm = (int *)sendCMD(GNBM_GROUPNUM,0,0);
#define GNBM_GROUPENM		(GNBM_GROUPNUM+1)

//int groupno = (int )sendCMD(GNBM_GROUPNO,0,0);
#define GNBM_GROUPNO		(GNBM_GROUPENM+1)

//int hostnum = (int )sendCMD(GNBM_HOSTNUM,0,0);
#define GNBM_HOSTNUM		(GNBM_GROUPNO+1)

//int *hostenm = (int *)sendCMD(GNBM_HOSTENM,0,0);
#define GNBM_HOSTENM		(GNBM_HOSTNUM+1)

//int nethostnum=(int )sendCMD(GNBM_NETHOSTNUM,0,0);
#define GNBM_NETHOSTNUM		(GNBM_HOSTENM+1)

//hostname = 0 , get local hostno
//int hostno = (int)sendCMD(GNBN_HOSTNO,(uint32)hostname,0);
#define GNBM_HOSTNO			(GNBM_NETHOSTNUM+1)
//char hostname[];
//memset(hostname,0,sizeof(hostname));
//hostno = -1 , get local hostname
//success=sendCMD(GNBM_HOSTNAME,hostno,(uint32)hostname);
#define GNBM_HOSTNAME		(GNBM_HOSTNO+1)

typedef struct _tagGNetbusHostInfo
{
	int grpno;
	int hostord;
	const char* server;
}GNBHI, *PGNBHI;

//success=sendCMD(GNBM_GROUPSRV,(uint32)PGNBHI,0);
#define GNBM_GROUPSRV		(GNBM_HOSTNAME+1)

//success=sendCMD(GNBM_HOSTMAINSRV,PGNBHI,0);
#define GNBM_HOSTMAINSRV	(GNBM_GROUPSRV+1)

//success=sendCMD(GNBM_GROUPMAINSRV,groupno,(uint32)hostname);
#define GNBM_GROUPMAINSRV	(GNBM_HOSTMAINSRV+1)

//success=sendCMD(GNBM_GROUPCENTER,groupno,0);
#define GNBM_GROUPCENTER	(GNBM_GROUPMAINSRV+1)

//success=sendCMD(GNBM_GROUPMAINCENTER,groupno,0);
#define GNBM_GROUPMAINCENTER (GNBM_GROUPCENTER+1)

//注册管理
//success=sendCMD(GNBM_REGPROC,(uint32)proc_name,0);
#define GNBM_REGPROC		0x200

//success=sendCMD(GNBM_UNREGPROC,index,0);
#define GNBM_UNREGPROC		(GNBM_REGPROC+1)

//success=sendCMD(GNBM_PUTMSG,sort_type,(uint32)describe);
#define GNBM_PUTMSG			(GNBM_UNREGPROC+1)

//进程管理
//success=sendCMD(GNBM_STARTPROC,(uint32)asc_name,(uint32)ch_name);
#define GNBM_STARTPROC		0x300

//success=sendCMD(GNBM_ENDPROC,0,0);
#define GNBM_ENDPROC		(GNBM_STARTPROC+1)

//success=sendCMD(GNBM_RUNPROC,0,0);
#define GNBM_RUNPROC		(GNBM_ENDPROC+1)

//success=sendCMD(GNBM_EXISTPROC,(uint32)asc_name,0);
#define GNBM_EXISTPROC		(GNBM_RUNPROC+1)

//success=sendCMD(GNBM_KILLPROC,(uint32)asc_name,0);
#define GNBM_KILLPROC		(GNBM_EXISTPROC+1)

//success=sendCMD(GNBM_KILLALL,0,0);
#define GNBM_KILLALL		(GNBM_KILLPROC+1)

//success=sendCMD(GNBM_RESTARTBUS,groupno,(uint32)hostname);
#define GNBM_RESTARTBUS		(GNBM_KILLALL+1)

//success=sendCMD(GNBM_SWITCHHOST,groupno,(uint32)srvname);
#define GNBM_SWITCHHOST		(GNBM_RESTARTBUS+1)

//邮件管理
typedef struct __tagGnetbusMailBuf
{
	uint8	type;					//调用类型
	char	user[GDB_DESC_LEN];		//用户名
	char	srvname[GDB_DESC_LEN];	//服务器类型
	char	source[GDB_DESC_LEN];	//源地址
	char	target[GDB_DESC_LEN];	//目的地址
	int32	pid;					//目标进程
	int32	level;					//邮件级别

	uint32  len;					//缓冲长度
	uint8*  buf;					//数据缓冲
}GNBMB,*PGNBMB;


//MailBox l_MailBox(host_group_no[0],(char*)mbuf->srvname);
//l_MailBox.SendSrvRequest( mbuf->user , mbuf->buf, mbuf->len , mbuf->target , mbuf->level );
#define MBT_SNDMAIL				0x01

//MailBox l_MailBox(host_group_no[0],(char*)mbuf->srvname);
//MAIL_HEAD	trg;
//strcpy( trg.user,mbuf->user   );
//strcpy( trg.host,mbuf->target );
//l_MailBox.SendSrvResultToClient( &trg , mbuf->buf, mbuf->len, mbuf->level );
#define MBT_SNDCLINETMAIL		0x02

//MailBox  l_MailBox( host_group_no[0] , (char*)mbuf->srvname ) ;
//int32 len=0;
//uint8* pBuf=NULL;
//l_MailBox.RecvSrvRequestFrmClient(&src,&trg,(void **)&pBuf,&len,mbuf->level);
//
#define MBT_RCVMAIL				0x01
#define MBT_RCVCLINETMAIL		0x02

//GNBMB mail_buf;
//success=sendCMD(GNBM_SNDMAIL,(uint32)&mail_buf,0);
#define GNBM_SNDMAIL	0x400

//GNBMB mail_buf;
//success=sendCMD(GNBM_RCVMAIL,0,(uint32)&mail_buf);
#define GNBM_RCVMAIL	(GNBM_SNDMAIL+1)

//事项管理
typedef struct __tagGnetbusEventBuf
{ 
	int32  grpno;
	int32  sort;
	int32  type;
	uint8  graphout;
	int16  state;
	int16  state1;
	int16  state2;
	FLOAT4 val;
	FLOAT4 val1;
	FLOAT4 val2;
	FLOAT4 val3;
	char object[GDB_CODE_LEN];
	char member0[GDB_CODE_LEN];	
	char member1[GDB_CODE_LEN];	
	char member2[GDB_CODE_LEN];	
	char member3[GDB_CODE_LEN];	
	char graph[GDB_PATH_LEN];
	char tone[GDB_PATH_LEN];
	char text[GDB_PATH_LEN];

}GNBEB,*PGNBEB;

//GNBEB event_buf;
//success=sendCMD(GNBM_SNDEVENT,(uint32)&event_buf,0);
#define GNBM_SNDEVENT 0x500

//GNBEB event_buf;
//success=sendCMD(GNBM_RCVEVENT,0,(uint32)&event_buf);
#define GNBM_RCVEVENT (GNBM_SNDEVENT+1)

//文件管理
//GNBMB file_buf;
//success=sendCMD(GNBM_NETSAVE,(uint32)filename,(uint32)&file_buf);
#define GNBM_NETSAVE  0x600

//success=sendCMD(GNBM_NETDEL,(uint32)filename,0);
#define GNBM_NETDEL	  (GNBM_NETSAVE+1)

//实时库加载
//success=sendCMD(GNBM_RELOADTABLE,(uint32)tablename,(uint32)srvname);
#define GNBM_RELOADTABLE	  0x700

//success=sendCMD(GNBM_RELOADTABLE,(uint32)srvname,0);
#define GNBM_RELOADSERVER	  (GNBM_RELOADTABLE+1)

//success=sendCMD(GNBM_RELOADTABLE,(uint32)mode,0);
#define GNBM_RELOADMODE		  (GNBM_RELOADSERVER+1)

//success=sendCMD(GNBM_RTDBBACKUP,(uint32)tablename,0);
#define GNBM_RTDBBACKUP		  (GNBM_RELOADMODE+1)

//success=sendCMD(GNBM_RTDBLOADUSR,0,0);
#define GNBM_RTDBLOADUSR	  (GNBM_RTDBBACKUP+1)
#endif
