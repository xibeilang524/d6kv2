////
///
///
//
//////////////////////////////////////////////////////////////////
#ifndef _DA_RESTORE_H_XWQ20030523_
#define _DA_RESTORE_H_XWQ20030523_

#include "GlobalDef.h"
#include "Isolate.h"
#include "ChainTab.h"

typedef struct _island_info
{
	int islandno;
	int state;//活岛或死岛
	int isneedrestore;//是否需要恢复
	BOOL isfault;//是否是故障岛
	_island_info* pNext;
} ISLANDINFO;

typedef struct _restore_brk_
{
	int brkaddr;
	unsigned char action;
	BOOL linkflag;//标记该开关是两个恢复岛之间的联络开关
	_restore_brk_* pNext;
} RESTOREBRK;

typedef struct
{
	int brkaddr;
	unsigned char action;
} RESTOREBRKINFO;

typedef struct 
{
	int BrkNum;
	RESTOREBRKINFO * m_pBrk;
	int *m_pBeginFlagPerRgn;
} RESTOREPRJ;

class CRestore
{
	//Functions
public:
	CRestore();
	~CRestore();
	int Initialize(CIsolate& Iso);
	virtual void Clear();
	virtual void GenRestorePro(int& prjnum);//组合方案
	int PerformResPrj(int PrjNo, BOOL bForceSerialYk=FALSE);

protected:
	void GetResRgnNum();
	int GetRestoreBrk();//产生征对恢复区域动作的开关序列
	int TpAnalysis();
	void SpreadAllIslandNode(int nodeaddr,const int islandno, int& flag);
	//Variables
public:
//	int GetResPrjNum();
	unsigned char *m_pState;
	unsigned char *m_pInvalidf;
	int m_iBrkNum;
protected:
	//返回值:FALSE，拓扑没变动，开关左、右节点岛号返回无效；TRUE，拓扑变动，开关左、右节点岛号返回有效
	BOOL TpSwitch(int brkaddr, int action);
	void WritePrjtoRdb();
	int m_iFaultNum;
	FAULTINFO *m_pFault;
	int m_iIsoBrkNum;
	ISOLATE_BRK *m_pIsoBrk;

	////拓扑分析结果
	ISLANDINFO * m_pIslandinfo;
	//////恢复过程中的基本统计信息
	int m_iResRgnNum;
	int *m_pActBrkNum;
	RESTOREBRK ** m_ppResBrkPerResRgn;
	/////方案信息
	int m_iPrjNum;
	RESTOREPRJ* m_pPrj;
};
#endif