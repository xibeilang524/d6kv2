#ifndef GDAIMP_FUNC_H_HEADER_INCLUDED
#define GDAIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############命令码参数说明######################
//################################################

//DA信息设置
//FUNCSTRUCT funcstruct;
//int success=sendCMD( GDA_INFO,(uint32)&funcstruct );


#define GDA_INFO	0x800

#define GDA_EVENT	(GDA_INFO+1)

#endif