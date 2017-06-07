//树结构的通用模板
//树中的节点属性"PROPERTY",在树中应能有主键的属性（唯一）
//									written by xiaowq 2004-09-29
////////////////////////////////////////////////////////////////////////////
#ifndef _TREETREE_H_
#define _TREETREE_H_

template <class PROPERTY,int MAX_CHILD>
class CTree
{
	struct _NODE
	{
		PROPERTY propterty;
		struct _NODE* pchild[MAX_CHILD];
	}
public:
	CTree(){
		pRoot = NULL;
	}
	~CTree(){
	}
	int Add(PROPERTY father,PROPERTY self);
public:
	struct _NODE* pRoot;
};

template <class PROPERTY,int MAX_CHILD>
int CTree<PROPERTY,MAX_CHILD>::Add(PROPERTY father,PROPERTY self)
{
	struct _NODE* pNew = new struct _NODE;
	pNew->propterty = self;
	memset(pNew->pchild,0,MAX_CHILD*sizeof(struct _NODE*));
	if(pRoot == NULL)
		pRoot = pNew;
	else
	{

	}
	return 0;
}

#endif