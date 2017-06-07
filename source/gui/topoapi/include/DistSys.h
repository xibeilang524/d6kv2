// DistSys.h: interface for the CDS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTSYS_H__8E83BC55_2F28_49D4_B5FC_F76589BE3500__INCLUDED_)
#define AFX_DISTSYS_H__8E83BC55_2F28_49D4_B5FC_F76589BE3500__INCLUDED_

// #include "qptrlist.h"
// #include "qfile.h"
// #include "qtextstream.h"
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMap>

#include "sysdef.h"
#include "rdb/rdbapi.h"
#include "model/scdtp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define			LENGTH_ERRORMSG		512

/////////////////////////////////// 

class CDSMeas;		// 量测类，用于描述实时量测和伪量测

class CDSObject;	// 配电网抽象模型中所有设备的基类

class 		CDSRootNode;			// 根节点(电源点)
class 			CDSOutlet;			// 变电所出线

class		CDSBus;					// 母线(用于计算)
class		CDSLine;				// 线段(用于计算)

class		CDSDevice;				// 设备
class			CDSNode;			// 节点
class			CSglTermDSDev;		// 单端设备
class			CDSDevBus;			// 母线设备
class				CDSCapacitor;	// 电容器
class					CDSLoad;	// 负荷
class			CDblTermDSDev;		// 双端设备
class				CDSFeeder;		// 线段 ///////////////////////////////////////
// 配电网抽象模型中所有设备的基类
//
class CDSObject
{
private:

	int m_nObjId;	// CDS 中元素的标识(全局唯一)

public:
	CDSObject(int nObjId = -1) { m_nObjId = nObjId; }
	~CDSObject() {}

	// 获取元素标识
	int GetObjectId() { return m_nObjId; }
	// 设置元素标识
	void SetObjectId(int nObjId) { m_nObjId = nObjId; }
};

///////////////////////////////////////
// 配电网抽象模型中电气设备
// 

class CDSDevice : public CDSObject
{
private:

	// 配电网络的设备管理是通过设备名称和设备内码来唯一标识每个设备。不同设备可能
	// 会有相同的设备内码(m_nDevId)，但不可能有相同实体标识(m_nObjId)。
	char	m_strDevName[CODE_LEN];	// device name

public:	
	int		m_DevType;			//设备类型,由scd/devdef.h定义

	CDSDevice(char *strDevName, int nObjId) 
		: CDSObject(nObjId)
	{
		Q_ASSERT(strDevName != NULL); strcpy(m_strDevName, strDevName); 
	}
	~CDSDevice() { }

	// 获取设备名称
	char *GetDevName() { return m_strDevName; }
	void SetDevName(char* strDevName) { strcpy(m_strDevName, strDevName); }
};




///////////////////////////////////////
// 节点类 (用于记录拓扑结构)
//

class CDSNode : public CDSDevice
{
private:
	//modify by lipeng 20110524
	//CPtrArray				LinkedFeederArray; // 与该节点相连接的馈线指针数组
	QList<CDSFeeder*>		LinkedFeederArray;//与该节点相连接的馈线指针数组
	BOOL					m_bVisited; // 通过此标志可以判断该节点是否属于孤立(停电)区域

public:

	int								m_nIndex;
	//int							m_nodeType;			//节点所关联单端设备类型:-1:无设备;0:节点,1:母线;2:负荷;3:电容器;4:电源点;
	uint							m_nLoadNum,m_nCapNum,m_nRootNum,m_nDevbusNum;
	QList<CDSLoad*>				m_pLoad;
	QList<CDSCapacitor*>			m_pCapacitor;
	CDSRootNode*					m_pRoot;
	CDSDevBus*						m_pDevBus;

	CDSNode(char *strDevName, int nObjId);
	~CDSNode(); 

	BOOL m_bOutput;

	// 父节点（用于环网分析）
	CDSNode *m_pParent;
	// 设置父节点
	void SetParentNode(CDSNode *pParentNode) { m_pParent = pParentNode; }
	// 获取父节点
	CDSNode *GetParentNode() { return m_pParent; }


	// 判断该节点是否有负荷
	BOOL HasPowerLoad() { return (m_nLoadNum>0) ? TRUE : FALSE; }


	// 该节点映射的母线
	CDSBus *m_pMapedBus; 

	// 获取节点度数
	int GetDegree() { return LinkedFeederArray.count(); }
	// 获取指定的节点
	CDSNode* GetLinkedNode(uint i);
	// 获取指定的馈线
	CDSFeeder* GetLinkedFeeder(uint i);
	// 获取和指定节点相关的馈线
	CDSFeeder* GetLinkedFeeder(CDSNode *pNode);

	// 当该节点度数为二时，返回另一条馈线
	CDSFeeder* GetAnotherLinkedFeeder(CDSFeeder *pFeeder);
	// 返回和指定线路对应的馈线
	CDSFeeder* GetLinkedFeeder(CDSLine *pLine);
	// 增加一条馈线到动态馈线指针数组
	void AddLinkedFeeder(CDSFeeder *pFeeder);
	// 将一条馈线从动态馈线指针数组中移出
	void RemoveFeeder(CDSFeeder *pFeeder);
	
	// 返回访问标记
	BOOL GetVisitingFlag() { return m_bVisited; }
	void SetVisitingFlag(BOOL bFlag) { m_bVisited = bFlag; }

	//close by lipeng for topo
	// 判断是否为连接点
	BOOL IsJointPoint(); 

	// 分裂点（用于弱环网潮流/状态估计）
	CDSNode* m_pBreakNode;
	BOOL IsBreakpoint() { return (m_pBreakNode != NULL) ? TRUE : FALSE; }
	
	int m_nLayerId;
	void SetLayerId(int nLayerId) { m_nLayerId = nLayerId; }
	int GetLayerId() { return m_nLayerId; }

};

// CDSRootNode
//
// 电源点
class CDSRootNode : public CDSDevice
{
public:

	CDSNode *m_pNode;
	//BOOL m_bAllocateLoad;

	CDSRootNode(int nObjId, CDSNode *pNode, char* name) 
		: CDSDevice(name, nObjId)
	{
		m_pNode = pNode; 
		m_bActive = TRUE; 
		//m_bAllocateLoad = FALSE;
	}
	~CDSRootNode()
	{
		//OutletArray.RemoveAll();
	}
	
	// 电源点运行标志(活跃/休眠)
	BOOL m_bActive;
	void SetActiveFlag(BOOL bActive) { m_bActive = bActive; }
	BOOL IsActiveRootNode() { return m_bActive; }

	//删除无用的分支
	//void RemoveTwigs();
	
	// 该电源点的出线数组
	QList<CDSOutlet*>		OutletArray;
	void AddOutlet(CDSOutlet *pOutlet);
	CDSOutlet* GetOutlet(uint i);
	int GetNumOfOutlets();

	//friend ostream& operator << (ostream& os, CDSRootNode* pRootNode);
};


/////
// CSglTermDSDev 单端设备
//
class CSglTermDSDev : public CDSDevice
{
private:

	int m_nLinkId;
	CDSNode* m_pNode; // 相连接的节点指针

public:
	CSglTermDSDev(CDSNode *pNode, char *strDevName)
		: CDSDevice(strDevName, pNode->GetObjectId())
	{
		m_nLinkId = pNode->GetObjectId(); 
		m_pNode = pNode;
	}
	~CSglTermDSDev() { }
	
	int GetLinkId() { return m_nLinkId;	}
	// 获取连接的节点指针
	CDSNode* GetLinkedNode() { Q_ASSERT(m_pNode != NULL); return m_pNode; }
	// 设置相连接的节点指针
	void SetLinkedNode(CDSNode* pNode) { Q_ASSERT(pNode != NULL); m_pNode = pNode; }
};

//
// CDSCapacitor
//
class CDSCapacitor : public CSglTermDSDev
{
private:
	double Cap; // capacity under the normal voltage
	
public:
	CDSCapacitor(char *strDevName, CDSNode *pNode)
		: CSglTermDSDev(pNode, strDevName) {}
	~CDSCapacitor() { }
	
};


//
// CDSTrans
// 
class CDSLoad : public CSglTermDSDev
{
public:

	
	int m_nIndex;	

	CDSLoad(char *strDevName, CDSNode *pNode)
		: CSglTermDSDev(pNode, strDevName)
	{
		m_nIndex = 0;
	}
	~CDSLoad() {}
	
};

class CDSDevBus : public CSglTermDSDev
{
private:
	
public:
	CDSDevBus(char *strDevName, CDSNode *pNode)
		: CSglTermDSDev(pNode, strDevName) {}
	~CDSDevBus() { }
	
};


// CDblTermDSDev
//
class CDblTermDSDev : public CDSDevice
{
private:
	
	int m_nLinkId1, m_nLinkId2;
	CDSNode *m_pNode1, *m_pNode2;

public:

	CDblTermDSDev(int nObjId, char *strDevName, CDSNode *pNode1, CDSNode *pNode2)
	: CDSDevice(strDevName, nObjId)
	{
		m_pNode1 = pNode1; m_pNode2 = pNode2;
		m_nLinkId1 = pNode1->GetObjectId(); m_nLinkId2 = pNode2->GetObjectId();
	}

	~CDblTermDSDev() 
	{
		if (m_pNode1 != NULL) delete m_pNode1;
		if (m_pNode2 != NULL) delete m_pNode2;
	}

	void SetLinkedNode1(CDSNode *pNode1) 
	{ Q_ASSERT(pNode1 != NULL); m_pNode1 = pNode1; m_nLinkId1 = m_pNode1->GetObjectId(); }
	void SetLinkedNode2(CDSNode *pNode2)
	{ Q_ASSERT(pNode2 != NULL); m_pNode2 = pNode2; m_nLinkId2 = m_pNode2->GetObjectId(); }

	void _SetLinkedNode1() { m_pNode1 = NULL; }
	void _SetLinkedNode2() { m_pNode2 = NULL; }
	
	void GetLinkId(int& nLinkId1, int& nLinkId2) 
	{ nLinkId1 = m_nLinkId1; nLinkId2 = m_nLinkId2; }
	void SetLinkId(int nLinkId1, int nLinkId2) 
	{ m_nLinkId1 = nLinkId1; m_nLinkId2 = nLinkId2; }

	CDSNode *GetLinkedNode1() { Q_ASSERT(m_pNode1 != NULL); return m_pNode1; }
	CDSNode *GetLinkedNode2() { Q_ASSERT(m_pNode2 != NULL); return m_pNode2; }
};

///////////////////////////////////////
// CDSOutlet
//

class CDSOutlet : public CDSDevice
{
public:
	CDSRootNode *m_pRootNode;
	
	BOOL m_bActive;
	void SetActiveFlag(BOOL bActive) { m_bActive = bActive; }
	BOOL IsActiveOutlet() { return m_bActive; }

	// this flag is only valid in radial network
	BOOL m_bRadial;
	BOOL IsRadialNetwork() { return m_bRadial; }
	void SetRadialFlag(BOOL bRadial) { m_bRadial = bRadial; }

	CDSOutlet *m_pAnotherOutlet;

	CDSOutlet(int nObjId, char *strDevName, CDSRootNode *pRootNode);
	~CDSOutlet() {}

	CDSBus* m_pHeadBus;
	CDSBus* m_pTailBus;

	BOOL m_bAllocateLoad;

	//为手拉手判断用
	CDSFeeder*	m_pMapFeeder;

};


class CDSFeeder : public CDblTermDSDev
{
private:

public:
	
	int m_nOperStatus;

	int m_nIndex;

	//modify by lipeng 20110524
	//BOOL m_bIsCB;
	int		m_feedType;		//-1:未确定类型;0:馈线;1:开关;2:刀闸;3:配电变压器;(4:连接线暂时和馈线一起);

	CDSFeeder(char *strDevName,  int nObjId, 
		CDSNode *pNode1, CDSNode *pNode2);
	~CDSFeeder() {}
	
	CDSNode *GetFarNode(CDSNode *pNode);

	CDSLine* m_pMapedLine;


	// 线路组
	int m_nGroupId, m_nIndivId; // group and individual identification
	void SetGroupId(int nGroupId) { m_nGroupId = nGroupId; }
	void SetIndivId(int nIndivId) { m_nIndivId = nIndivId; }
	int GetGroupId() { return m_nGroupId; }
	int GetIndivId() { return m_nIndivId; }

	// 线路运行状态
	enum { DSF_OPEN, DSF_CLOSE };
	int GetOperStatus() { return m_nOperStatus; }
	void SetOperStatus(int nOperStatus) { m_nOperStatus = nOperStatus; }
	
	// Since we don't know the direction of feeder before the WFS, we 
	// must revise this flag during topological analysis.
	enum { DSF_POSITIVE_DIR = 1, DSF_NEGATIVE_DIR = -1 };

	int m_nDirFlag; // 线路方向标记 1 or -1
	int GetDirectionalFlag() { return m_nDirFlag; }
	void SetDirectionalFlag(int nDirFlag) { m_nDirFlag = nDirFlag; }

	uint	MeshFlag;		//环网标志
	uint	GetMeshFlag(){return MeshFlag;}
	void	SetMeshFlag(uint nMeshFlag){MeshFlag = nMeshFlag;}

};


// CDSBus is a abstract concept for analysis of distrubition system.

class CDSBus 
{
private:
	int m_nLayerId;
	
public:
	CDSBus* m_pNextSibling;
	CDSBus* m_pPrevSibling;
	CDSBus* m_pParent;
	CDSBus* m_pFirstChild;

	CDSBus* m_pPrevBus; // in WFS
	CDSBus* m_pNextBus; // 

	CDSLine* m_pLine; // this line connects its parent and itself

	CDSNode* m_pOrigNode;

	CDSBus* m_pHeadBus;
	CDSBus* GetHeadBus() { Q_ASSERT(m_pHeadBus != NULL); return m_pHeadBus; }
	CDSBus* m_pTailBus;
	CDSBus* GetTailBus() { Q_ASSERT(m_pHeadBus != NULL); return m_pHeadBus->m_pTailBus; }

	
	CDSBus(CDSBus *pHeadBus = NULL, int nLayerId = 1);
	~CDSBus() {}

	int GetLayerId() { return m_nLayerId; }

};
	
// CDSLine 类(用于计算)
// 

class CDSLine : public CDSObject
{
private:
	CDSBus* m_pStartBus; // 首末母线
	CDSBus* m_pEndBus;
	
	int m_nNumOfNodes; // 该线段所包含的节点数目

public:
	CDSLine(CDSBus* pStartBus, CDSBus* pEndBus);
	~CDSLine() {}
	
	// 获取首末母线
	CDSBus* GetStartBus() {	Q_ASSERT(m_pStartBus != NULL); return m_pStartBus; }
	CDSBus* GetEndBus() { Q_ASSERT(m_pEndBus != NULL); return m_pEndBus; }


	// 获取该线段所包含的节点数目
	int GetNumOfNodes() { return m_nNumOfNodes; }
	// 设置该线段所包含的节点数目
	void SetNumOfNodes(int nNumOfNodes) { m_nNumOfNodes = nNumOfNodes; }

	CDSOutlet *m_pOutlet; // 该线路所在的出线
	void SetOutlet(CDSOutlet *pOutlet) // 设置该线路所在的出线
	{ Q_ASSERT(pOutlet != NULL); m_pOutlet = pOutlet; }
	CDSOutlet *GetOutlet() // 获取该线路所在的出线
	{ Q_ASSERT(m_pOutlet != NULL); return m_pOutlet; } 

	/*CDSNode **GetNodeArray(); // 获取节点数组*/

	//feeder列表
	QList<CDSFeeder*>		m_pFeederArray;
	void		AddFeeder(CDSFeeder*  pFeeder);
	CDSFeeder **GetFeederArray(); // 获取馈线数组

};

////////////////////////////////////////////////////////////

//母线和节点的对照表
typedef struct  
{
	char	BusName[CODE_LEN];
	char	NodeName[CODE_LEN];

	//char*	GetBusName(char* nodeName);
}BUS2NODE;

class CDS  
{

public:
	char					m_beginDevName[CODE_LEN];	//起始设备代码
	int						m_bInit;			//初始化标志

	//读取实时库用
	//Rdb_Tab				lsd;				//本地表
	Rdb_SqlCli				m_rdbTbl;			//服务器表
	PTPRCD					m_tpRcd;			//实时库拓扑表
	uint					m_scdtpnum;			//scdtp表记录数
	//////////////////////////////////////////////////////////

	// 以下是关于节点，馈线，用户负荷，电源点、开关刀闸、配变和电容器的动态指针数组
	uint nNumOfNds, nNumOfSgs, nNumOfLds, nNumOfGens, nNumOfCbs, nNumOfLns, nNumOfDisTrans,nNumOfCps;
	QList<CDSNode*>			NodeArray;
	QList<CDSFeeder*>			FeederArray;
	QList<CDSLoad*>			LoadArray;
	QList<CDSRootNode*>		RootArray;
	QList<CDSCapacitor*>		CPArray;
	
	QList<CDSFeeder*>			CBArray;
	QList<CDSFeeder*>			LnArray;
	QList<CDSFeeder*>			DisTransArray;

	uint						nNumOfBus;
	QList<BUS2NODE*>			Bus2NodeArray;
	char*						GetBusnameFromNode(char* nodename);
	char*						GetNodeFromBusname(char* busname);


	QList<CDSNode*>			BreakpointArray; // 关于分裂点的动态指针数组(用于弱环网潮流/状态估计)
	QList<CDSFeeder*>			pmeshArray;		//无源环网设备列表
	QList<CDSFeeder*>			h2hArray;		//手拉手环网设备列表


	// 调试信息文件(用于调试/分析)
	char  errormsg[LENGTH_ERRORMSG];
	QFile	m_ofsCore;
	QTextStream m_topostream;
	void PrintMsgToFile(char* strMsg) {m_topostream<<strMsg;}



public:
	CDS();
	virtual ~CDS();
	void			ClearMem();							//清除内存


	// 模块1. 初始化CDS 
	void			setBeginDevName(const char* devName);		//设置起始设备代码
	int				InitDS();							//初始化CDS
	int				InitBefDS( QMap<QString, int> &devActiveMap );							//初始化CDS
	uint 			ReadTopo();							//读取拓扑结构
	uint			ReadBefTopo( QMap<QString, int> &devActiveMap );
	uint			PutRcdToCDS(PTPRCD  onescdrcd);		//将拓扑记录转入CDS拓扑结构
	CDSNode*		IsNodeExist(char*	nodeName);		//是否在nodearray中存在已有节点
	char*			GetBusNameOfNode(char* nodename);	//根据节点名取对应母线名称
	// 系统运行状态 辐射网/弱环网
	enum { DS_RADIAL, DS_WEAKLY_MESH };

	// 模块2.  读刀闸开关状态
	void			ReadCBStatus();						//读取遥信状态
	void			ReadCBBefStatus( QMap<QString, int> &devStatusMap );						//读取遥信状态

	// 模块3. 拓扑分析
	void			TopologicalAnalysis();
	// 弱环网分析
	void AnalysisOfWeaklyMeshedNetwork();
	// 广度优先搜索
	CDSNode *WidthFirstSearch(CDSNode *pNode, CDSOutlet* pOutlet);
	// 环路汇报
	CDSNode* LoopReport(CDSNode *pNode1, CDSNode *pNode2);
	// 连接相关联的两个节点
	void			LinkTwoNodes(CDSNode *pNode1, CDSNode *pNode2, CDSFeeder *pFeeder);
	// 连接双向链表中相邻的两个母线
	void LinkTwoBuses(CDSBus* pPrevBus, CDSBus* pNextBus);
	// 连接母线和对应的节点
	void LinkBusAndNode(CDSBus *pBus, CDSNode *pNode);
	// 节点分裂
	void SplitNode(CDSNode *pNode, CDSNode *pSplittedNode);
	// 删除支路
	void			RemoveFeeder(CDSFeeder *pObsoleteFeeder);
	// 判断指定的节点是否为电源点
	BOOL IsRootNode(CDSNode *pNode);


	// 模块6. 计算结果统计
	// 输出计算结果汇总信息
	void WriteFnlRpt();
	//将拓扑着色信息输出到实时库scdtp
	void UpdateScdtpRcd();
	// 输出核心文件(用于调试)
	void WriteCoreFile();


	//路径查询函数
	int GetPowerPathFromDev(const char *devName, char **devList);			//-1:失电;-2:手拉手环网
	uint GetLoadPathFromDev(const char *devName, uint	devType,char **devList );
	void GetNodeDev(CDSNode* pNode,char**  devlist,uint*  count);			//取节点上单端设备
	void GetDSLineDev(CDSLine* pLine,char**  devlist,uint*  count);			//取Line上的设备列表
	void GetChildFeedersOfLineFromFeeder(CDSFeeder*  begFeeder,char** pDevList,uint* devCnt);	//取feder所在line里feeder的子设备
	void GetParentFeedersOfLineFromFeeder(CDSFeeder*  begFeeder,char** pDevList,uint* devCnt);	//取feder所在line里feeder的父设备

};

// the return feeder may be null if some nodes are merged between the pNode1 & pNode2
inline CDSFeeder* GetDSFeeder(CDSNode *pNode1, CDSNode *pNode2)
{  Q_ASSERT(pNode1 != NULL && pNode2 != NULL);  return pNode1->GetLinkedFeeder(pNode2); }
// 判断指定的两个节点是否为同一个节点
inline BOOL IsSameNode(CDSNode *pNode1, CDSNode *pNode2)
{  Q_ASSERT(pNode1 != NULL && pNode2 != NULL); return (pNode1->GetObjectId() == pNode2->GetObjectId()); }
// 判断指定的两条母线是否为同一个母线
inline BOOL IsSameBus(CDSBus *pBus1, CDSBus *pBus2)
{  Q_ASSERT(pBus1 != NULL && pBus2 != NULL); return IsSameNode(pBus1->m_pOrigNode, pBus2->m_pOrigNode); }
inline BOOL IsSameFeeder(CDSFeeder *pFeeder1, CDSFeeder *pFeeder2)
{  Q_ASSERT(pFeeder1 != NULL && pFeeder2 != NULL); return (pFeeder1->GetObjectId() == pFeeder2->GetObjectId()); }
// 判断指定的两条线路是否为同一条线路
inline BOOL IsSameLine(CDSLine *pLine1, CDSLine *pLine2)
{  
	Q_ASSERT(pLine1 != NULL && pLine2 != NULL);

	return IsSameBus(pLine1->GetStartBus(), pLine2->GetStartBus()) 
		&& IsSameBus(pLine1->GetEndBus(), pLine2->GetEndBus());
}

#endif // !defined(AFX_DISTSYS_H__8E83BC55_2F28_49D4_B5FC_F76589BE3500__INCLUDED_)
