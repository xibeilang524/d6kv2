//恢复方案合格的前提：各开关的额定值不过负荷
//
//////////////////////////////////////////////////////////////////////
#ifndef _RATINGRETORE_H_
#define _RATINGRETORE_H_

#include "Restore.h"
//#include "ChainTab.h"
#define QUEUE_SIZE 1000

typedef struct _tree_node{
	int nodeaddr;
	float Psum;//节点负荷总加()
	float Qsum;//节点负荷总加
	struct _tree_node* pFather;
	struct _tree_node* pLChild;
	struct _tree_node* pRSibling;
} TREENODE;

typedef struct {
	float P;
	float Q;
} NODELOAD;

//该模板的作用用队列在于存储类指针
template<class T,int len>
class CQueue
{
public:
	CQueue(){read = write = 0; memset(pBase,0,len*sizeof(T*));}
	~CQueue(){}
	BOOL Write(T* pSave){
		if((write+1)%len == read)
			return FALSE;
		pBase[(write++)%len] = pSave;
		return TRUE;
	}
	BOOL Read(T** pRead){
		if(read == write)
			return FALSE;
		*pRead = pBase[(read++)%len];
		return TRUE;
	}
private:
	T* pBase[len];
	int read;
	int write;
};

class CRatingRestore : public CRestore
{
public:
	CRatingRestore();
	~CRatingRestore();

private:
	void GetBrkIslandinfo(int brkaddr,ISLANDINFO** pL,ISLANDINFO** pR);
	int IsRestored(RESTOREBRK* pBrk);
	void ExtendResRgn(int resrgnno);
//	RESTOREBRK* GetRestoreBrk(int resrgnno);
	float GetBrkVoltage( int brkaddr);
	float GetNodeVoltage( int nodeaddr);
	void MiddleExtend(TREENODE* pBase, float& PVal, float& QVal);
	//计算节点负荷,输入参数：节点在拓扑参数中的下标，系统开关状态，系统开关的有功、无功分布
	float CalNodeLoad( int iNodeaddr,  float* pVal,  unsigned char* pState = NULL);
	//计算待恢复区域的功率分布
	void ResRgnFlow(NODELOAD pLoad[]);
	BOOL IsAppropriateLoad(float Vol, TREENODE* pRoot);
	BOOL CanAddLoad(int nodeaddr,  int brkaddr,  float P,  float Q);
	void DeleteATree(TREENODE *pRoot);
	void GenNewTryPrj(RESTOREBRK* ptry,CQueue<RESTOREBRK,QUEUE_SIZE> &queue);

	
	int FindARoute(TREENODE *pNode, int **pnodeaddr, int &nodenum);
	int FindARoute(int startnodeaddr, int endnodeaddr, int** pnodeaddr, int& nodenum);
	TREENODE* FormATree(TREENODE* pFather, int startnodeaddr, int* findnodeaddr, TREENODE** pFind,int findnum);
private:
	CQueue<RESTOREBRK,QUEUE_SIZE> *m_pQueue;
public:
	virtual void GenRestorePro(int& prjnum);
	virtual void Clear();

	float* m_pP;
	float* m_pQ;
	float* m_pU;
	float* m_pI;
	float* m_pPForeFault;
	float* m_pQForeFault;
	float* m_pUForeFault;
	float* m_pIForeFault;
	int m_iYcValNum;
private:
	NODELOAD* m_pLoad;
};

#endif