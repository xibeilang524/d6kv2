#ifndef GADDCARDIMP_FUNC_H_HEADER_INCLUDED
#define GADDCARDIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############命令码参数说明######################
//################################################


#include "ddes/GBaseObject.h"

//弹出标志牌选筐
//FUNCSTRUCT funcstruct;
//int success=sendCMD( GVKF_LOCK,(uint32)&funcstruct,0 );
#define	GACF_START		0x400


typedef struct _tagAddCardStruct
{
	GBaseObject		*obj;
	GPoint			pt;
	QString			graphname;
}ADDCARDSTRUCT,*PADDCARDSTRUCT;
//挂牌
//ADDCARDSTRUCT card;
//int success=sendCMD( GACF_ADDCARD,(uint32)&card,0 );
#define	GACF_ADDCARD		(GACF_START+1)

//挂牌
//GFuncStruct funcstruct;
//ADDCARDSTRUCT card;
//int success=sendCMD( GACF_ADDCARD,(uint32)&card,0 );
#define	GACF_DELCARD		(GACF_START+2)

//查牌
//GFuncStruct funcstruct;
//ADDCARDSTRUCT card;
//int success=sendCMD( GACF_FINDCARD,(uint32)&card,0 );
#define	GACF_FINDCARD		(GACF_START+3)

//modify by lipeng for ems&scada
//调节牌的信息
#define	GACF_MODIFYCARD		(GACF_START+6)

//关闭窗口
//sendCMD( GACF_CLOSEWIN,0,0 );
#define	GACF_CLOSEWIN		(GACF_START+4)

//查询窗口是否关闭
//int closed = sendCMD( GACF_CLOSED,0,0 );
#define	GACF_CLOSED			(GACF_START+5)


#define	GACF_MODIFYCARD		(GACF_START+6)

#endif
