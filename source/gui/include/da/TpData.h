

#ifndef _DATPDATA_H_XWQ20030519_
#define _DATPDATA_H_XWQ20030519_
#include "sysdef.h"

#include "GlobalDef.h"

class CDaTp
{
public:
	CDaTp();
	~CDaTp();
	typedef struct
	{
		int No;
		char Name[CODE_LEN];
		char Desc[DESC_LEN];
		int LeftNodeNo;
		int RightNodeNo;
		int P_inNodeNo;
		short Type;
		char P_direct;
		char TurnOnYx[CODE_LEN];
		char TurnOffYx[CODE_LEN];
		int LastingSecs;
		float RatingCur;
		float RatingCurCoe;
		float RatingVol;
		float RatingVolCoe;
		char CurCode[CODE_LEN];
		float CurCoe;//current coefficient
		char VolCode[CODE_LEN];
		float VolCoe;
		char P_Code[CODE_LEN];
		float PCoe;
		char Q_Code[CODE_LEN];
		float QCoe;
		int ExtendFlag;
	} DABREAK_TP;
	typedef struct
	{
		int No;
		short Type;
		char Desc[DESC_LEN];
		unsigned char DealMode;
		char PowerVolCode[CODE_LEN];
		float VolCoe;
		int ExtendFlag;
	} DANODE_TP;
	typedef struct
	{
		DABREAK_TP* pDbTp;//数据库中的原始拓扑数据
		char BrkStatusCode[CODE_LEN];
		int Ykf;
		int LeftNodeAddr;
		int RightNodeAddr;//数组下标
		BOOL IsFault;
		int FaultTypeNum;//开关报的事项个数
		int FaultType[MAX_ONEBRK_EVTNUM];
	} STRONGBRKTP;
	typedef struct
	{
		DANODE_TP* pDbTp;
		int BrkNum;
		int BrkAddr[NODE_MAX_SHAREBRKNUM];//数组下标
		//搜索标记
		int IsChecked;
		int IslandNo;//岛号
	} STRONGNODETP;
private:
	BOOL ReadTpData();
	void InitBrkNodeTpData();
	BOOL PretreatNode();
public:
	int CallYxYkPara();
	BOOL CallandCalTpData();
	//varibles
public:
	BOOL PNIsTreeStru();//PN:power network
	int m_iBrkNum;
	int m_iNodeNum;
	STRONGBRKTP* m_pBrk;
	STRONGNODETP* m_pNode;
private:
	BOOL GetTreeFlag();
	BOOL m_bTree;
	DABREAK_TP* m_pDbBrk;
	DANODE_TP* m_pDbNode;
};
#endif