//链表通用模板
//实现链表的初始化，增加，删除，查找
//并控制多线程同步
//									written by xiaowq  2004-9-29
/////////////////////////////////////////////////////////////////////////
#ifndef _CHAINTAB_H_
#define _CHAINTAB_H_

template <class COMSTRU>
class CChainTab
{
	struct _node {
		COMSTRU Val;
		struct _node* pNext;
	};
public:
	CChainTab();
	~CChainTab(){
		Clean();
	}
	int Add(COMSTRU node);
	int Del(COMSTRU* node);
	int Del(int i);
	int Ins(int i, COMSTRU node);
	int Get(int i, COMSTRU* node);
	BOOL IsEmpty();
	void Clean();
	int GetNodeNum();
	void ConvertArray(COMSTRU** pnode,int& num)
	{
		while(m_iSync == 1)
			;
		m_iSync = 1;
		if(pHeader == NULL)
		{
			*pnode = NULL;
			num = 0;
		}
		else
		{
			num = 0; 
			*pnode = new COMSTRU[nodenum];
			struct _node *pt = pHeader;
			while(pt != NULL)
			{
				(*pnode)[num++] = pt->Val;
				pt = pt -> pNext;
			}
#ifdef WIN32
			ASSERT(num == nodenum);
#endif
		}
		m_iSync = 0;
	}
private:
	int m_iSync;
	struct _node* pHeader;
	int nodenum;
};


template <class COMSTRU>
CChainTab<COMSTRU>::CChainTab()
{
	m_iSync = 0;
	pHeader = NULL;
	nodenum = 0;
}

template <class COMSTRU>
int CChainTab<COMSTRU>::Add(COMSTRU node)
{
	struct _node* pNew = new struct _node;
	pNew->Val = node;
	pNew->pNext = NULL;

	while(m_iSync != 0)
		;
	m_iSync = 1;
	if(pHeader == NULL)
	{
		pHeader = pNew;
	}
	else
	{
		struct _node* pt = pHeader;
		while(pt->pNext != NULL)
			pt = pt->pNext;
		pt->pNext = pNew;
	}

	nodenum ++;
	m_iSync = 0;
	return 0;
}

template <class COMSTRU>
int CChainTab<COMSTRU>::Del(int no)
{
	while(m_iSync != 0)
		;
	m_iSync = 1;

	if( no >= nodenum || no < 0 ) return 1;
	struct _node* pDelLast = pHeader;
	if(no == 0)
	{
		pHeader = pHeader->pNext;
		delete pDelLast;
	}
	else
	{
		for(int i = 0; i < no-1; i++)
			pDelLast = pDelLast->pNext;
		struct _node* pDel = pDelLast->pNext;
		pDelLast->pNext = pDel->pNext;
		delete pDel;
	}
	nodenum --;
	m_iSync = 0;
	return 0;
}

template <class COMSTRU>
int CChainTab<COMSTRU>::Del(COMSTRU* node)
{
	struct _node* pFindLast = NULL;

	while(m_iSync != 0)
		;
	m_iSync = 1;

	struct _node* pFind = pHeader;
	while(pFind != NULL && pFind->Val != *node)
	{
		pFindLast = pFind;
		pFind = pFind->pNext;
	}
	if(pFind == NULL) return 1;
	if(pFindLast == NULL)
		pHeader = pFind->pNext;
	else
		pFindLast->pNext = pFind->pNext;
	delete pFind;
	nodenum --;

	m_iSync = 0;
	return 0;
}

template <class COMSTRU>
int CChainTab<COMSTRU>::Ins(int i, COMSTRU node)
{
	while(m_iSync != 0)
		;
	m_iSync = 1;

	if(i < 0 || i > nodenum) return 1;
	if(i == nodenum) return (Add(node));//Add() always return 0;
	struct _node* pIns = new struct _node;
	pIns->Val = node;
	if(i == 0)
	{
		pIns->pNext = pHeader;
		pHeader = pIns;
	}
	else
	{
		struct _node* pFind = pHeader;
		for(int j=0; j < i-1; j++)
			pFind = pFind->pNext;
		pIns->pNext = pFind->pNext;
		pFind->pNext = pIns;
	}
	nodenum++ ;
	m_iSync = 0;
	return 0;
}

template <class COMSTRU>
int CChainTab<COMSTRU>::Get(int no, COMSTRU* node)
{
	while(m_iSync != 0)
		;
	m_iSync = 1;

	if(no < 0 || no >= nodenum) return 1;
	int i;
	struct _node* pFind = pHeader;
	for(i = 0; i < no; i++)
		pFind = pFind->pNext;
	*node = pFind->Val;

	m_iSync = 0;
	return 0;
}

template <class COMSTRU>
BOOL CChainTab<COMSTRU>::IsEmpty()
{
	if(pHeader == NULL)
		return TRUE;
	return FALSE;
}

template <class COMSTRU>
void CChainTab<COMSTRU>::Clean()
{
	struct _node* pt;
	while(m_iSync != 0)
		;
	m_iSync = 1;
	while(pHeader != NULL)
	{
		pt = pHeader->pNext;
		delete pHeader;
		pHeader = pt;		
	}
	nodenum = 0;
	m_iSync = 0;
}

template <class COMSTRU>
int CChainTab<COMSTRU>::GetNodeNum()
{
	return nodenum;
}
#endif