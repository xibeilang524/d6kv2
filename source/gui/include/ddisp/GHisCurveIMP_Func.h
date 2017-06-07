#ifndef GCURVEIMP_FUNC_H_HEADER_INCLUDED
#define GCURVEIMP_FUNC_H_HEADER_INCLUDED
//################################################
//############命令码参数说明######################
//################################################

//曲线查看
//FUNCSTRUCT funcstruct;
//uint32 curvetype;
//int success=sendCMD( GYKF_STARTYK,(uint32)&funcstruct,curvetype );

enum
{
	CURVETYPE_TODAY,// 今日曲线
	CURVETYPE_YES,//昨日曲线
	CURVETYPE_PLAN//计划曲线
};

#define GHCF_VIEWCURVE	0x600

#endif