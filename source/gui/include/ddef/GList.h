#ifndef GLIST_H_HEADER_INCLUDED_BCB88CD8
#define GLIST_H_HEADER_INCLUDED_BCB88CD8
#include "ddes_comm.h"

//##ModelId=433A079C00EA
//##Documentation
//## 功能：
//##          双向列表节点结构
//##         如果 ITEM节点中保存的是对象的指针，
//##         该节点并不负责该对象的释放，
//##         删除节点时请先删除对象的内容
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
template <class ITEM>
struct GListNode
{
	//##ModelId=433A4FE0005D
	//##Documentation
	//## 功能：
	//##     空构造   
	//## 参数：
	//##     无
	//## 返回：
	//##     无
	//## 设计：
	//##     宋永生
	//## 日期：
	//##     2005-9-28
	GListNode();

	//##ModelId=433A4FF700AB
	//##Documentation
	//## 功能：
	//##       用指定元素构造  
	//## 参数：
	//##      const ITEM item
	//## 返回：
	//##     无
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GListNode(ITEM item);

	//##ModelId=433A07CD0261
	//##Documentation
	//## 上一个元素
	GListNode<ITEM>* m_pPrev;

	//##ModelId=433A08000186
	//##Documentation
	//## 下一个元素
	GListNode<ITEM>* m_pNext;

	//##ModelId=433A30C00000
	//##Documentation
	//## 数据指针
	ITEM m_Item;

};



//##ModelId=4339FFB603A9
//##Documentation
//## GList双向链表模板类
//## 
//## 功能：
//##     用于操作集合类。
//##     提供平台公用集合类
//##     缺点：如果ITEM为指针类型，删除节点时并不删
//##           除指针指向的内容，如果需要释放内存,
//##           请先释放内存。
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
template <class ITEM>
class GList
{
public:
	//##ModelId=433A313F0167
	//##Documentation
	//## 功能：
	//##       构造空列表      
	//## 参数：
	//##     无
	//## 返回：
	//##     无
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GList();

	//##ModelId=433A31710261
	//##Documentation
	//## 功能：
	//##       析构列表，如果指针需要删除，请先删除内容
	//## 参数：
	//##      无
	//## 返回：
	//##     无
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	virtual ~GList();

	//##ModelId=433A320B035B
	//##Documentation
	//## 功能：
	//##       在指定位置前插入元素。   
	//## 参数：
	//##         int32  nIndex //插入位置索引
	//##         ITEM item     //元素
	//## 返回：（GPOS）
	//##       if success return GPOS
	//##       else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS insertBefore(int32 nIndex, ITEM Item);

	//##ModelId=433A32F20271
	//##Documentation
	//## 功能：
	//##       在指定位置前插入元素。      
	//## 参数：
	//##         GPOS gPos  //插入位置
	//##         ITEM item     //元素
	//## 返回：（GPOS）
	//##       if success return GPOS
	//##       else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS insertBefore(GPOS gPos, ITEM Item);

	//##ModelId=433A332F0271
	//##Documentation
	//## 功能：
	//##       删除链表中指定位置元素。
	//##       如果ITEM为指针类型，并不删除指针指向的内容，
	//##       调用该函数前，如果需要释放内存请先释放内存。
	//## 参数：
	//##       int32 nIndex //删除位置索引
	//## 返回：（uint8）
	//##       if success return 1
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	uint8 removeAt(int32 nIndex);

	//##ModelId=433A335F00EA
	//##Documentation
	//## 功能：
	//##       删除链表中指定位置元素。
	//##       如果ITEM为指针类型，并不删除指针指向的内容，
	//##       调用该函数前，如果需要释放内存请先释放内存。
	//## 参数：
	//##         GPOS gPos  //删除位置
	//## 返回：（uint8）
	//##       if success return 1
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	uint8 removeAt(GPOS gPos);

	//##ModelId=433A33BA032C
	//##Documentation
	//## 功能：
	//##       删除链表所有元素。
	//##       如果ITEM为指针类型，并不删除指针指向的内容，
	//##       调用该函数前，如果需要释放内存请先释放内存。
	//## 参数：
	//##      无
	//## 返回：（uint8）
	//##       if success return 1
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	uint8 remove();

	//##ModelId=433A33EA031C
	//##Documentation
	//## 功能：
	//##       在链表头部添加一个元素
	//## 参数：
	//##       ITEM item //元素
	//## 返回：（GPOS）
	//##       if success return GPOS
	//##       else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	//##   
	GPOS addHead(ITEM Item);

	//##ModelId=433A340101D4
	//##Documentation
	//## 功能：
	//##       在链表尾部添加一个元素
	//## 参数：
	//##       ITEM item //元素
	//## 返回：（GPOS）
	//##       if success return GPOS
	//##       else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS addTail(ITEM Item);

	//##ModelId=433A498701C5
	//##Documentation
	//## 功能：
	//##       在指定位置后插入元素。      
	//## 参数：
	//##         int32  nIndex //插入位置索引
	//##         ITEM item     //元素
	//## 返回：（GPOS）
	//##       if success return GPOS
	//##       else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS insertAfter(int32 nIndex, ITEM Item);

	//##ModelId=433A498E033C
	//##Documentation
	//## 功能：
	//##       在指定位置后插入元素。      
	//## 参数：
	//##         GPOS gPos  //插入位置
	//##         ITEM item     //元素
	//## 返回：（GPOS）
	//##       if success return GPOS
	//##       else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS insertAfter(GPOS gPos, ITEM Item);

	//##ModelId=433A4A0A000F
	//##Documentation
	//## 功能：
	//##       取指定位置的元素。
	//## 参数：
	//##         GPOS gPos  //获取位置      
	//## 返回：（ITEM&）
	//##       if success return item&
	//##       else return null
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM& getAt(GPOS gPos) const;

	//##ModelId=433A4A5C000F
	//##Documentation
	//## 功能：
	//##       取指定位置的元素。      
	//## 参数：
	//##         int32 nIndex  //获取位置索引     
	//## 返回：（ITEM&）
	//##       if success return item&
	//##       else return null
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM& getAt(int32 nIndex) const;

	//##ModelId=433A5172032C
	//##Documentation
	//## 功能：
	//##     取首节点位置
	//## 参数：
	//##    无
	//## 返回：（GPOS）
	//##     if success return pos
	//##     else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS firstPos() const;

	//##ModelId=433A520603A9
	//##Documentation
	//## 功能：
	//##     取尾节点位置
	//## 参数：
	//##    无
	//## 返回：（GPOS）
	//##     if success return pos
	//##     else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS lastPos() const;

	//##ModelId=433A523F01D4
	//##Documentation
	//## 功能：
	//##     取下一个节点位置
	//## 参数：
	//##    GPOS& pos //获取下一个位置
	//## 返回：（ITEM&）
	//##     if success return item&
	//##     else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM& nextPos(GPOS& gPos) const;

	//##ModelId=433A533A02BF
	//##Documentation
	//## 功能：
	//##     取上一个节点位置
	//## 参数：
	//##    GPOS& pos //获取上一个位置
	//## 返回：（ITEM&）
	//##     if success return item&
	//##     else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM& prevPos(GPOS& gPos) const;

	//##ModelId=433A536603D8
	//##Documentation
	//## 功能：
	//##     取集合中元素个数
	//## 参数：
	//##     无
	//## 返回：（uint32）
	//##     return m_nCount
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	uint32 count() const;

	//##ModelId=433A9608003E
	//##Documentation
	//## 功能：
	//##       合并指定链表到当前链表尾部
	//## 参数：
	//##       const GList<ITEM>& item //同类型链表
	//## 返回：（uint8 ）
	//##       if success return 1
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	uint8 merge( const GList<ITEM>& elems);

	//##ModelId=433AA496002E
	//##Documentation
	//## 功能：
	//##       查找项目所在位置
	//## 参数：
	//##       ITEM item //元素
	//## 返回：（GPOS）
	//##       项目所在位置
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS findPos(ITEM item) const;

	//##ModelId=433AA4F0034B
	//##Documentation
	//## 功能：
	//##       查找项目所在索引
	//## 参数：
	//##       ITEM item //元素
	//## 返回：（int32）
	//##       项目所在索引
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	int32 findIndex(ITEM item) const;

	//##ModelId=4345C6B601C5
	//##Documentation
	//## 功能：
	//##     取指定索引节点位置
	//## 参数：
	//##    int32 nIndex //索引节点位置
	//## 返回：（GPOS）
	//##     if success return pos
	//##     else return NULL
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	GPOS getPos(int32 nIndex) const;
	//##ModelId=434CB75E0177
	//##Documentation
	//## 功能：
	//##       设置指定位置的元素。
	//## 参数：
	//##       GPOS gPos  //获取位置
	//##       ITEM item  //设置元素 
	//## 返回：（uint8）
	//##       if success return 1
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-10-12
	uint8 setAt(GPOS gPos, ITEM item);
	//##ModelId=434CB7D6032C
	//##Documentation
	//## 功能：
	//##       设置指定位置的元素。
	//## 参数：
	//##       int32 index  //获取位置
	//##       ITEM item  //设置元素 
	//## 返回：（uint8）
	//##       if success return 1
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-10-12
	uint8 setAt(int32 index, ITEM item);
	//##ModelId=435C333B03B9
	//##Documentation
	//## 功能：
	//##     判断集合中元素是否为空
	//## 参数：
	//##     无
	//## 返回：（uint8）
	//##     if empty return 1
	//##     else return 0    
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-10-23
	uint8 isEmpty() const ;
	//##ModelId=435C4FAD000F
	//##Documentation
	//## 功能：
	//##       删除链表中头位置元素。
	//##       如果ITEM为指针类型，并不删除指针指向的内容，
	//##       调用该函数前，如果需要释放内存请先释放内存。
	//## 参数：
	//##   无
	//## 返回：（ITEM）
	//##   if success return ITEM
	//##   else assert
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM removeHead() ;
	//##ModelId=435C504300FA
	//##Documentation
	//## 功能：
	//##       删除链表中尾位置元素。
	//##       如果ITEM为指针类型，并不删除指针指向的内容，
	//##       调用该函数前，如果需要释放内存请先释放内存。
	//## 参数：
	//##   无
	//## 返回：（ITEM）
	//##   if success return ITEM
	//##   else assert
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM removeTail();
	//##ModelId=43D44C630119
	//##Documentation
	//## 功能：
	//##       取头位置的元素。      
	//## 参数：
	//##       无
	//## 返回：（ITEM&）
	//##       if success return item&
	//##       else return null
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM& firstItem() const;
	//##ModelId=43D44C750213
	//##Documentation
	//## 功能：
	//##       取尾位置的元素。      
	//## 参数：
	//##       无
	//## 返回：（ITEM&）
	//##       if success return item&
	//##       else return null
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	ITEM& lastItem() const;
	//##ModelId=43D44D7901F4
	//##Documentation
	//## 功能：
	//##       倒序列表      
	//## 参数：
	//##       无
	//## 返回：
	//##       无
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	void reverse();
	//##ModelId=4436376E000F
	//##Documentation
	//## 功能：
	//##       链表转换成数组元素,内存由用户释放      
	//## 参数：
	//##       ITEM** item
	//## 返回：（uint32）
	//##       if success return count
	//##       else return 0
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	uint32 convToArray(ITEM** items) const;

	//##ModelId=44363814029F
	//##Documentation
	//## 功能：
	//##       链表转换成数组元素,内存由用户释放      
	//## 参数：
	//##       ITEM* items
	//##       uint32 count
	//## 返回：
	//##       无
	//## 设计：
	//##       宋永生
	//## 日期：
	//##       2005-9-28
	void convFromArray(ITEM* items, uint32 count);

protected:
	//##ModelId=435C33DD02FD
	//##Documentation
	//## 功能：
	//##   申请节点内存
	//## 参数：
	//##   GListNode<TYPE>* pPrev
	//##   GListNode<TYPE>* pNext
	//## 返回：
	//##   if alloc success return node
	//##   else return NULL 
	//## 设计：
	//##   宋永生
	//## 日期：
	//##   2005-10-24
	//## 
	//##   
	GListNode<ITEM>* allocNode(GListNode<ITEM>* pPrev, GListNode<ITEM>* pNext);

	//##ModelId=435C392A008C
	//##Documentation
	//## 功能：
	//##   释放节点内存
	//## 参数：
	//##   GListNode<TYPE>* pNode
	//## 返回：
	//##   无
	//## 设计：
	//##   宋永生
	//## 日期：
	//##   2005-10-24
	void freeNode(GListNode<ITEM>* pNode);

protected:
	//##ModelId=433A1C0C0251
	//##Documentation
	//## 首节点
	GListNode<ITEM> *m_pFirst;

	//##ModelId=433A2071009C
	//##Documentation
	//## 记录元素个数
	uint32 m_nCount;

	//##ModelId=433A50D2031C
	//##Documentation
	//## 尾节点
	GListNode<ITEM> *m_pLast;

};

//##ModelId=433A4FE0005D
template <class ITEM>
GListNode<ITEM>::GListNode()
{
	m_pNext = NULL ;
	m_pPrev = NULL ;
}

//##ModelId=433A4FF700AB
template <class ITEM>
GListNode<ITEM>::GListNode(ITEM item)
{
	m_pNext = NULL ;
	m_pPrev = NULL ;
	m_Item  = item ;
}

//##ModelId=433A313F0167
template <class ITEM>
GList<ITEM>::GList()
{
	m_nCount = 0 ;
	m_pFirst = m_pLast = NULL ;
}

//##ModelId=433A31710261
template <class ITEM>
GList<ITEM>::~GList()
{
	remove();
}

//##ModelId=433A320B035B
template <class ITEM>
GPOS GList<ITEM>::insertBefore(int32 nIndex, ITEM Item)
{
	if ( nIndex < 0 ) return addHead(Item) ;

	GPOS gPos = getPos(nIndex);

	return insertBefore( gPos , Item );
}

//##ModelId=433A32F20271
template <class ITEM>
GPOS GList<ITEM>::insertBefore(GPOS gPos, ITEM Item)
{
	if ( !gPos )  return addHead( Item );			

	GListNode<ITEM> *pNext = ( GListNode<ITEM> *) gPos ;
	GListNode<ITEM> *pNode = allocNode( pNext->m_pPrev , pNext );
	pNode->m_Item = Item ;

	if ( pNext->m_pPrev ) 
	{
		pNext->m_pPrev->m_pNext = pNode ;
	}
	else
	{
		m_pFirst = pNode ;
	}	

	pNext->m_pPrev = pNode ;

	return (GPOS )pNode ;
}

//##ModelId=433A332F0271
template <class ITEM>
uint8 GList<ITEM>::removeAt(int32 nIndex)
{
	GPOS gPos = getPos(nIndex);

	if ( !gPos ) return 0 ;

	return removeAt( gPos );
}

//##ModelId=433A335F00EA
template <class ITEM>
uint8 GList<ITEM>::removeAt(GPOS gPos)
{
	if ( !gPos ) return 0 ;

	GListNode<ITEM> *pNext = ( GListNode<ITEM> *) gPos ;

	if ( pNext == m_pFirst )
	{
		m_pFirst = pNext->m_pNext ;
	}
	else
	{
		if ( pNext->m_pPrev )
		{
			pNext->m_pPrev->m_pNext = pNext->m_pNext ;
		}
	}

	if ( pNext == m_pLast )
	{
		m_pLast = pNext->m_pPrev ;
	}
	else
	{
		if ( pNext->m_pNext )
		{
			pNext->m_pNext->m_pPrev = pNext->m_pPrev ;
		}
	}

	freeNode( pNext );

	return 1 ;
}

//##ModelId=433A33BA032C
template <class ITEM>
uint8 GList<ITEM>::remove()
{
	if( count() == 0 )
		return 1;

	GPOS gPos = firstPos() ;

	GPOS gRemove = gPos ;

	while ( gRemove )
	{
		nextPos( gPos );

		removeAt( gRemove );

		gRemove = gPos ;
	}

	return 1 ;
}

//##ModelId=433A33EA031C
template <class ITEM>
GPOS GList<ITEM>::addHead(ITEM Item)
{
	GListNode<ITEM> *pNode = allocNode( NULL , m_pFirst );

	pNode->m_Item = Item ;

	if (m_pFirst != NULL)	
	{
		m_pFirst->m_pPrev = pNode;
	}
	else				
	{
		m_pLast = pNode;
	}

	m_pFirst = pNode;

	return (GPOS )pNode ;
}

//##ModelId=433A340101D4
template <class ITEM>
GPOS GList<ITEM>::addTail(ITEM Item)
{
	GListNode<ITEM> *pNode = allocNode(m_pLast, NULL);

	pNode->m_Item = Item;

	if (m_pLast != NULL)	
	{
		m_pLast->m_pNext = pNode;
	}
	else				
	{
		m_pFirst = pNode;
	}

	m_pLast = pNode;

	return (GPOS )pNode ;
}

//##ModelId=433A498701C5
template <class ITEM>
GPOS GList<ITEM>::insertAfter(int32 nIndex, ITEM Item)
{
	if ( nIndex == -1 )	return addTail( Item );

// 	GListNode<ITEM> *gPos = getPos( nIndex ) ;
	GPOS gPos = getPos( nIndex ) ;//modify by chenkai 2013年1月16日

	return insertAfter( gPos , Item );
}

//##ModelId=433A498E033C
template <class ITEM>
GPOS GList<ITEM>::insertAfter(GPOS gPos, ITEM Item)
{
	GListNode<ITEM> *pPrev = ( GListNode<ITEM> *) gPos ;

	GListNode<ITEM> *pNode = allocNode( pPrev , pPrev->m_pNext );

	pNode->m_Item = Item ;

	if ( pPrev->m_pNext ) 
	{
		pPrev->m_pNext->m_pPrev = pNode ;
	}
	else
	{
		m_pLast = pNode ;
	}

	pPrev->m_pNext = pNode ;

	return (GPOS )pNode ;
}

//##ModelId=433A4A0A000F
template <class ITEM>
ITEM& GList<ITEM>::getAt(GPOS gPos) const
{
	assert( gPos != NULL );

	GListNode<ITEM> *pNode = ( GListNode<ITEM> * )gPos ;

	return pNode->m_Item ;
}

//##ModelId=433A4A5C000F
template <class ITEM>
ITEM& GList<ITEM>::getAt(int32 nIndex) const
{
	GPOS gPos = getPos( nIndex );

	assert( gPos != NULL );

	GListNode<ITEM> *pNode = ( GListNode<ITEM> * )gPos ;

	return pNode->m_Item ;
}

//##ModelId=433A5172032C
template <class ITEM>
GPOS GList<ITEM>::firstPos() const
{
	return (GPOS )m_pFirst ;
}

//##ModelId=433A520603A9
template <class ITEM>
GPOS GList<ITEM>::lastPos() const
{
	return (GPOS )m_pLast ;
}

//##ModelId=433A523F01D4
template <class ITEM>
ITEM& GList<ITEM>::nextPos(GPOS& gPos) const
{
	GListNode<ITEM> *pNode = ( GListNode<ITEM> * )gPos ;

	gPos = (GPOS )pNode->m_pNext ;

	return pNode->m_Item ;
}

//##ModelId=433A533A02BF
template <class ITEM>
ITEM& GList<ITEM>::prevPos(GPOS& gPos) const
{
	GListNode<ITEM> *pNode = ( GListNode<ITEM> * )gPos ;

	gPos = (GPOS )pNode->m_pPrev ;

	return pNode->m_Item ;
}

//##ModelId=433A536603D8
template <class ITEM>
uint32 GList<ITEM>::count() const
{
	return m_nCount ;
}

//##ModelId=433A9608003E
template <class ITEM>
uint8 GList<ITEM>::merge( const GList<ITEM>& elems)
{
	if ( elems.count() == 0 )
		return 0;

	GPOS ePos = elems.firstPos();

	ITEM item ;

	while( ePos )
	{
		item = elems.nextPos( ePos );

		addTail( item );
	}

	return 1 ;
}

//##ModelId=433AA496002E
template <class ITEM>
GPOS GList<ITEM>::findPos(ITEM Item) const
{
	GPOS gPos = firstPos() ;

	while ( gPos )
	{
		GPOS rPos = gPos ;

		if ( Item == nextPos(gPos) )
			return rPos ;
	}

	return NULL ;
}

//##ModelId=433AA4F0034B
template <class ITEM>
int32 GList<ITEM>::findIndex(ITEM Item) const
{
	int32 nPos = 0 ;

	GPOS rPos = firstPos() ;

	while ( rPos )
	{
		if ( Item == nextPos(rPos) )
			return nPos ;

		nPos ++ ;
	}

	return -1 ;
}

//##ModelId=4345C6B601C5
template <class ITEM>
GPOS GList<ITEM>::getPos(int32 nIndex) const
{
	if ( nIndex < 0 || nIndex >= (int32)m_nCount )
		return NULL ;

	GListNode<ITEM> *pNode = m_pFirst ;

	while( nIndex -- )
	{
		pNode = pNode->m_pNext ;	
	}

	return (GPOS )pNode ;
}

//##ModelId=434CB75E0177
template <class ITEM>
uint8 GList<ITEM>::setAt(GPOS gPos, ITEM item) 
{
	if ( gPos == NULL ) return 0 ;

	GListNode<ITEM> *pNode = ( GListNode<ITEM> * )gPos ;

	pNode->m_Item = item ;

	return 1 ;
}

//##ModelId=434CB7D6032C
template <class ITEM>
uint8 GList<ITEM>::setAt(int32 index, ITEM item)
{
	GPOS gPos = getPos( index );

	if ( gPos == NULL ) return 0 ;

	GListNode<ITEM> *pNode = ( GListNode<ITEM> * )gPos ;

	pNode->m_Item = item ;

	return 1 ;
}

//##ModelId=435C33DD02FD
template <class ITEM>
GListNode<ITEM>* GList<ITEM>::allocNode(GListNode<ITEM>* pPrev, GListNode<ITEM>* pNext)
{
	GListNode<ITEM>* pNode = new GListNode<ITEM>;
	g_CHECK_PTR(pNode);

	if ( !pNode ) return NULL ;
	pNode->m_pPrev = pPrev;

	pNode->m_pNext = pNext;

	m_nCount++;

	if (m_nCount <= 0) return NULL;  // make sure we don't overflow

	return pNode ;
}

//##ModelId=435C392A008C
template <class ITEM>
void GList<ITEM>::freeNode(GListNode<ITEM>* pNode)
{
	if ( !pNode ) return ;

	delete pNode ;

	pNode = NULL ;

	m_nCount -- ;
}

template <class ITEM>
ITEM GList<ITEM>::removeHead()
{
	if (m_pFirst == NULL) 
	{
 		return 0; 
	}
	GListNode<ITEM>* pNode = m_pFirst ;

	ITEM returnValue = pNode->m_Item;

	m_pFirst = pNode->m_pNext;

// 	if (pNode != NULL)	pNode->m_pPrev = NULL;		//modify by chenkai 2013年8月8日
	if (m_pFirst != NULL)	m_pFirst->m_pPrev = NULL;
	else				m_pLast = NULL;

	freeNode( pNode );

	return returnValue;
}

//##ModelId=435C504300FA
template <class ITEM>
ITEM GList<ITEM>::removeTail()
{
	if (m_pLast == NULL ) return 0;  // don't call on empty list !!!


	GListNode<ITEM>* pNode = m_pLast;
	ITEM returnValue = pNode->m_Item;

	m_pLast = pNode->m_pPrev;

	if (m_pLast != NULL)	m_pLast->m_pNext = NULL;
	else					m_pFirst = NULL;

	freeNode(pNode);

	return returnValue;
}

//##ModelId=435C333B03B9
template <class ITEM>
uint8 GList<ITEM>::isEmpty() const
{
	return ( m_pFirst == NULL );
}

//##ModelId=43D44C630119
template <class ITEM>
ITEM& GList<ITEM>::firstItem() const
{
	assert(m_pFirst != NULL );

	return m_pFirst->m_Item;
}

//##ModelId=43D44C750213
template <class ITEM>
ITEM& GList<ITEM>::lastItem() const
{
	assert(m_pLast != NULL );

	return m_pLast->m_Item ;
}

//##ModelId=43D44D7901F4
template <class ITEM>
void GList<ITEM>::reverse()
{
	if ( isEmpty() ) return ;

	GPOS gPos = firstPos();
	GPOS gLast= lastPos();

	uint32 ncount = 0 ;

	while(gPos && gLast)
	{
		if( gPos == gLast )
			break;

		if ( ncount == count()/2)
			break;

		ITEM& item1 = nextPos(gPos);
		ITEM& item2 = prevPos(gLast);

		ITEM item = item1 ;
		item1 = item2;
		item2 = item ;

		ncount ++ ;
	}
}

//##ModelId=4436376E000F
template <class ITEM>
uint32 GList<ITEM>::convToArray(ITEM** items) const
{
	if(count()==0) 
		return 0;

	(*items) = new ITEM[count()];
	g_CHECK_PTR((*items));

	int icnt = 0;

	GPOS gPos = firstPos();
	while(gPos)
	{
		(*items)[icnt++]=nextPos(gPos);
	}

	return icnt ;
}

//##ModelId=44363814029F
template <class ITEM>
void GList<ITEM>::convFromArray(ITEM* items, uint32 count)
{
	if(count==0) return ;

	for(uint32 i=0;i<count;i++)
	{
		addTail(items[i]);
	}

	return ;
}

#endif /* GLIST_H_HEADER_INCLUDED_BCB88CD8 */
