#ifndef _GTREE_H
#define _GTREE_H

#include "ddef/ddes_comm.h"
#include "ddef/GRect.h"
#include "ddef/GPoint.h"
#include "ddef/GList.h"

class GBaseObject;
class GObjectManage;

#define MAX_CHILD_NODES 8

typedef struct _gtree_node
{
	GRect 				rect;
	GList<GBaseObject*>	objs;
	int32				subcnt;
	_gtree_node*		subnode[MAX_CHILD_NODES];
}GTN,*LPGTN;

class GDE_EXPORT_DES GTree
{
public:
	GTree();
	~GTree();
public:
	void createTree(const GRect& rect);
	void destroyTree();

	uint8 addObj(GBaseObject* obj);
	uint8 delObj(GBaseObject* obj);

	uint8 searchTree(const GRect& rect,GObjectManage& objs,\
				GTS_FILTER_FUNC filter=NULL,void* clientData=NULL);
	uint8 searchTree(const GPoint& pt,GObjectManage& objs,\
				GTS_FILTER_FUNC filter=NULL,void* clientData=NULL);

	void  setDepth(int32 depth) { m_depth=depth;}
	int32 getDepth() const { return m_depth; }

	LPGTN getRoot() const { return m_root; }
	void  trimTree();
protected:

	LPGTN createNode(const GRect& rect);
	void  destroyNode(LPGTN *node);

	uint8 addNodeObj(LPGTN* node , GBaseObject* obj,int32 depth);
	uint8 delNodeObj(LPGTN* node , GBaseObject* obj);

	uint8 trimTreeNode(LPGTN* node);

	uint8 searchNode(LPGTN node,const GRect& rect,GObjectManage& objs,\
				GTS_FILTER_FUNC filter=NULL,void* clientData=NULL);
	uint8 searchNode(LPGTN node,const GPoint& pt,GObjectManage& objs,\
				GTS_FILTER_FUNC filter=NULL,void* clientData=NULL);
	void  splitBox(const GRect& src, GRect& dst1,GRect& dst2);
	void  splitBox(const GRect& src, GRect* dst ,int32 cnt );
protected:
	int32  m_shapes;
	int32  m_depth;
	LPGTN  m_root ;
};

#endif
