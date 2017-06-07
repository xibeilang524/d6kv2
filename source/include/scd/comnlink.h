#ifndef __comnlink_h
#define __comnlink_h

#ifdef __unix
#include  <sys/time.h>
#endif

inline int  comnlink_sleepms(int  msecs)
{
#ifdef __unix
struct timeval  stoptime;

stoptime.tv_sec=(int)(msecs/1000);
stoptime.tv_usec=(int)(msecs%1000)*1000;
return(select(0,0,0,0,&stoptime));
#endif

#ifdef WIN32
Sleep(msecs);
return(0);
#endif
}

struct __comn_position {};

typedef struct __comn_position *  COMN_POSITION;

template <class TYPE>
class  ComnLinkTable {
	struct  COMN_LINK_NODE {
		COMN_LINK_NODE *next;
		COMN_LINK_NODE *prev;
		TYPE  data;
	};

	//int    linklockf;
	COMN_LINK_NODE   *linkhead;
	COMN_LINK_NODE   *linktail;
public:
	void   FreeLink(void);
	uint nodesum;
	ComnLinkTable<TYPE>(void);
	~ComnLinkTable<TYPE>(void) {
		FreeLink();
	}
	int    linklockf;
	int    LockLink(void);
	void   UnlockLink(void);

	TYPE  *NodeData(COMN_POSITION node) {
		return(&((COMN_LINK_NODE *)node)->data);
	}
	int  AddNode(TYPE *databuf);
	int  AddHead(TYPE *databuf);
	int  DelNode(COMN_POSITION  delposition,int lockf=1);
	int  RemoveNodePos(COMN_POSITION  delposition);
	int  AddNodePos(COMN_POSITION pos);

	COMN_POSITION  GetFirstNode(void) {
		return((COMN_POSITION)linkhead);
	}
	COMN_POSITION  GetNextNode(COMN_POSITION nodebuf) {
		return((COMN_POSITION)(((COMN_LINK_NODE *)nodebuf)->next));
	}
	COMN_POSITION  GetPrevNode(COMN_POSITION nodebuf) {
		return((COMN_POSITION)(((COMN_LINK_NODE *)nodebuf)->prev));
	}
	COMN_POSITION  GetTailNode(void) {
		return((COMN_POSITION)linktail);
	}
};


template <class TYPE>
ComnLinkTable<TYPE>::ComnLinkTable(void)
{
	linklockf=0;
	nodesum=0;
	linkhead=linktail=NULL;
}

template <class TYPE>
void ComnLinkTable<TYPE>::FreeLink(void)
{
	COMN_POSITION  curnode,nextnode;
	LockLink();
	curnode=GetFirstNode();
	if(curnode==NULL) {
		UnlockLink();
		return;
	}

	do {
		nextnode=GetNextNode(curnode);
		delete  (COMN_LINK_NODE *)curnode;
		curnode=nextnode;
	} while(curnode);
	linkhead=linktail=NULL;
	nodesum=0;
	UnlockLink();
}


template <class TYPE>
int ComnLinkTable<TYPE>::LockLink(void)
{	
	if(linklockf==0) {linklockf=1;return 1;}
	int i;
	for(i=0;i<600;i++) {
		if(linklockf==0) {linklockf=1;break;}
		comnlink_sleepms(10);
	}
	return (i+1);
}

template <class TYPE>
void ComnLinkTable<TYPE>::UnlockLink(void)
{
	linklockf=0;
}
template <class TYPE>
int ComnLinkTable<TYPE>::AddHead(TYPE *databuf)
{
	COMN_LINK_NODE *addlinknode;
	addlinknode = new COMN_LINK_NODE;
	if(addlinknode==NULL)
	{
		return 0;
	}
	addlinknode->next=NULL;
	addlinknode->prev=NULL;
	addlinknode->data=*databuf;
	LockLink();
	if(linkhead==NULL && linktail==NULL) {
		linkhead=linktail=addlinknode;
	}
	else {
		addlinknode->next=linkhead;
		linkhead->prev=addlinknode;
		linkhead=addlinknode;
	}
	nodesum++;
	UnlockLink();
	return(1);
}

template <class TYPE>
int  ComnLinkTable<TYPE>::AddNode(TYPE *databuf)
{
	COMN_LINK_NODE  *addlinknode;

	addlinknode=new COMN_LINK_NODE;
	if(addlinknode==NULL)
	{
		return 0;
	}
	addlinknode->next=NULL;
	addlinknode->prev=NULL;
	addlinknode->data=*databuf;
	LockLink();
	if(linkhead==NULL && linktail==NULL) {
		linkhead=linktail=addlinknode;
	}
	else {
		addlinknode->prev=linktail;
		linktail->next=addlinknode;
		linktail=addlinknode;
	}
	nodesum++;
	UnlockLink();
	return(1);
}

template <class TYPE>
int ComnLinkTable<TYPE>::RemoveNodePos(COMN_POSITION  delposition)
{	
	COMN_LINK_NODE *delnode=(COMN_LINK_NODE  *)delposition;
	LockLink();

	COMN_LINK_NODE *nextlinknode=(COMN_LINK_NODE  *)GetNextNode((COMN_POSITION)delnode);
	COMN_LINK_NODE *prevlinknode=(COMN_LINK_NODE  *)GetPrevNode((COMN_POSITION)delnode);

	if(nextlinknode==NULL && prevlinknode==NULL) { //仅一个
		linkhead=linktail=NULL;
	}
	else if(nextlinknode==NULL)  {  //尾节点
		prevlinknode->next=NULL;
		linktail=prevlinknode;
	}
	else if(prevlinknode==NULL)  {  //首节点
		nextlinknode->prev=NULL;
		linkhead=nextlinknode;
	}
	else { // 中间节点
		prevlinknode->next=nextlinknode;
		nextlinknode->prev=prevlinknode;
	}
	nodesum--;
	UnlockLink();
	return (1);
}

template <class TYPE>
int  ComnLinkTable<TYPE>::AddNodePos(COMN_POSITION pos)
{
	COMN_LINK_NODE  *addlinknode =(COMN_LINK_NODE *)pos;

	if(addlinknode==NULL)
	{
		return 0;
	}
	addlinknode->next=NULL;
	addlinknode->prev=NULL;
	//addlinknode->data=*databuf;
	LockLink();
	if(linkhead==NULL && linktail==NULL) {
		linkhead=linktail=addlinknode;
	}
	else {
		addlinknode->prev=linktail;
		linktail->next=addlinknode;
		linktail=addlinknode;
	}
	nodesum++;
	UnlockLink();
	return(1);
}

template <class TYPE>
int  ComnLinkTable<TYPE>::DelNode(COMN_POSITION  delposition,int lockf)
{
	COMN_LINK_NODE  *delnode,*nextlinknode,*prevlinknode;

	delnode=(COMN_LINK_NODE  *)delposition;
	if(lockf)LockLink();

	nextlinknode=(COMN_LINK_NODE  *)GetNextNode((COMN_POSITION)delnode);
	prevlinknode=(COMN_LINK_NODE  *)GetPrevNode((COMN_POSITION)delnode);

	if(nextlinknode==NULL && prevlinknode==NULL) { //仅一个
		linkhead=linktail=NULL;
	}
	else if(nextlinknode==NULL)  {  //尾节点
		prevlinknode->next=NULL;
		linktail=prevlinknode;
	}
	else if(prevlinknode==NULL)  {  //首节点
		nextlinknode->prev=NULL;
		linkhead=nextlinknode;
	}
	else { // 中间节点
		prevlinknode->next=nextlinknode;
		nextlinknode->prev=prevlinknode;
	}

	nodesum--;
	delete delnode;

	if(lockf)UnlockLink();
	return(1); 
}

#endif

