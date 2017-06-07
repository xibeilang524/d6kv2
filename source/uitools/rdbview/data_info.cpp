#include "data_info.h"

DataInfo::DataInfo()
{
	RealDataNum = TerminalNum =0;
	FreshTime = 5;
	RealFlag = 4;
	RealData = NULL;
	Terminal = NULL;
	Rtdb=NULL;
	DataOK = FALSE;
	pRealDataInfo=NULL;
	pRealData=NULL;
	RtdbTableNum=NULL;
	Yx_Break=Yx_Switch=Yx_Prot=Yx_Status=Yx_Dmsdiagnose=NULL;
}

DataInfo::~DataInfo()
{
	if(RealData)
	{
		delete(RealData);
		RealData = NULL;
	}
	if(Terminal)
	{
		delete(Terminal);
		Terminal = NULL;
	}
	if(pRealDataInfo)
		free(pRealDataInfo);
	if(pRealData)
		free(pRealData);
	int i;
	for( i=0;i<RtdbTableNum;i++)
	{
		if(Rtdb[i].col)
		{
			delete(Rtdb[i].col);
			Rtdb[i].col =NULL;
		}
	}
	if (Rtdb)
		delete(Rtdb);
	if (Yx_Break)
		delete(Yx_Break);
	if (Yx_Switch)
		delete(Yx_Switch);
	if (Yx_Prot)
		delete(Yx_Prot);
	if (Yx_Status)
		delete(Yx_Status);
	if (Yx_Dmsdiagnose)
		delete(Yx_Dmsdiagnose);
}