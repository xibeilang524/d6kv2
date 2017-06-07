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

typedef struct _tagYKAQYSMail
{
	uint32	MailMark;			//4
	char	HostName0[ GDB_CODE_LEN ];	//+24=28
	char	HostName1[ GDB_CODE_LEN ];	//+24=52
	char	Operator[ GDB_CODE_LEN ];		//+24=76
	char	ObjCode[ GDB_CODE_LEN ];		//+24=100
	char	ObjName[ GDB_DESC_LEN ];		//+40=140
	uint32	OperateType;		//+4=144
	uint32	Permission;			//+
	char	Info[ 256 ];		//+300=444
}YKAQYSMAIL,*PYKAQYSMAIL;

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

//序列遥控
#define GYKF_SERIAL_YK	(0x01)

//序列编辑
#define GYKF_SERIAL_EDIT	(0x02)

//添加序列对象
#define GYKF_SERIAL_ADDOBJECT (0x03)

//关闭序列编辑窗口
#define GYKF_SERIAL_CLOSEEDITWND (0x04)

//遥控安全约束
#define GYKF_AQYS			(0x05)

//安全约束验证返回
#define GYKF_AQYSRET		(0x06)

//解/合环遥控
#define GYKF_Ring_YK			(0x07)

//解/合环遥控编辑
#define GYKF_Ring_EDIT		(0x08)

//添加解/合环对象
#define GYKF_Ring_ADDOBJECT (0x09)

//关闭解/合环编辑窗口
#define GYKF_Ring_CLOSEEDITWND (0x0a)

//群控
#define GYKF_GROUP_YK			(0x0b)

//群控编辑
#define GYKF_GROUP_EDIT		(0x0c)

//添加群控对象
#define GYKF_GROUP_ADDOBJECT (0x0d)

//关闭群控编辑窗口
#define GYKF_GROUP_CLOSEEDITWND (0x0e)

#endif