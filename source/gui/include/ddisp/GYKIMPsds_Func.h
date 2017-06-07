#ifndef GYKIMP_FUNC_H_HEADER_INCLUDED
#define GYKIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############命令码参数说明######################
//################################################


typedef struct _tagYKReqMail
{
	char	oper[ GDB_DESC_LEN ];//申请人
	char	devname[ GDB_DESC_LEN * 2 ];//设备名称
	char	yktype[ GDB_DESC_LEN ];//用字符串描述的操作类型，比如:由分到合
	char	graph[ GDB_PATH_LEN ];//遥控设备所在图名
	uint16	ykno;//遥控号
	uint16	yxno;//遥信号
}YKREQMAIL, *PYKREQMAIL;

//开始遥控
//FUNCSTRUCT tmpstruct;
//int success=sendCMD( GYKF_STARTYK,(uint32)※tmpstruct,0 );
#define GYKF_STARTYK	0x100

//遥控许可
// REQSTRUCT ykreqstruct;
//int success=sendCMD( GYKF_YKREQ,(uint32)&ykreqstruct,0 );
#define GYKF_YKREQ		(GYKF_STARTYK+1)

//遥控申请回复
//int success=sendCMD( GYKF_YKREQ,0,0 );
#define GYKF_YKREQRET	(GYKF_STARTYK+2)

//计时命令
//int success=sendCMD( GYKF_TIMEEVENT,0,0 );
#define GYKF_TIMEEVENT	(GYKF_STARTYK+3)

#endif