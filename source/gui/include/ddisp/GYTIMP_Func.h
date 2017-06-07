#ifndef GYTIMP_FUNC_H_HEADER_INCLUDED
#define GYTIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############命令码参数说明######################
//################################################

enum
{
	YTTYPE_YKTYPE,//遥控方式
	YTTYPE_SETTYPE//设点方式
};

typedef struct _tagYTReqMail
{
	char	oper[ GDB_DESC_LEN ];//操作人
	char	devname[ GDB_DESC_LEN * 2 ];//设备名称
	char	yttypedesc[ GDB_DESC_LEN ];//遥调操作描述,比如：4档->5档
	char	graph[ GDB_PATH_LEN ];//遥调对象所在图名
	uint16	yttype;//遥调类型,设点方式还是遥控方式
	uint16	upykno;//升遥控号
	uint16	downykno;//降遥控号
	uint16	stopykno;//急停遥控号
	uint16	ytno;//遥调号
}YTREQMAIL, *PYTREQMAIL;

//开始遥控
//FUNCSTRUCT tmpstruct;
//int success=sendCMD( GYTF_STARTYT,(uint32)&tmpstruct,0 );
#define GYTF_STARTYT	0x200

//遥控许可
// REQSTRUCT ytreq;
//int success=sendCMD( GYTF_YTREQ,(uint32)&ytreq,0 );
#define GYTF_YTREQ	(GYTF_STARTYT+1)

//遥控申请回复
//int success=sendCMD( GYTF_YTREQRET,0,0 );
#define GYTF_YTREQRET	(GYTF_STARTYT+2)

//计时命令
//int success=sendCMD( GYTF_TIMEEVENT,0,0 );
#define GYTF_TIMEEVENT	(GYTF_STARTYT+3)

#endif