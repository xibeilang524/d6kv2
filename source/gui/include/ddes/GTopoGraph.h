#ifndef _GTOPOGRAPH_H
#define _GTOPOGRAPH_H

#include "ddef/GList.h"

#define GS_UNKNOWN	0x00
#define GS_LONE		0x01
#define GS_JOINT	0x02
#define GS_SPLIT	0x04

#define GS_ONLINE	0x08
#define GS_OFFLINE	0x10

#define GM_UNVISIT	0x00
#define GM_VISITED	0x01

#define GI_UNKNOWN	 -1

class GEdge   ;
class GVertex ;
class GTopoGraph  ;
class GBaseObject;

typedef void (*TOPOGRAPH_VISIT)(\
			GTopoGraph* pGraph,\
			void*		clientData,\
			GVertex* vtx,\
			GEdge*   edg);

typedef TOPOGRAPH_VISIT TOPOGRAPH_STATUS;

class GDE_EXPORT_DES GEdgeData
{
public:

	GEdgeData();
	~GEdgeData();

	void clear();

public:

	uint8 		mark() const ;
	uint32 		status() const ;
	uint32 		id1() const ;
	uint32 		id2() const ;
	GVertex* 	vtx1() const;
	GVertex* 	vtx2() const;
	GBaseObject* vtx1Obj() const;
	GBaseObject* vtx2Obj() const;
	GBaseObject* obj() const ;
	GBaseObject* obj1() const;
	GBaseObject* obj2() const;
	uint8		 sno1() const;
	uint8		 sno2() const;

	GVertex*	vtx(uint32 v) const;
	GVertex*    vtxO(GVertex* v) const;
	GVertex*    vtxO(uint32 v) const;
	uint32      vtxId(uint32 v) const;
	uint8 container( uint32 vtx) const ;
	uint8 container( GVertex* vtx) const;
	uint8 container( uint32 v1 , uint32 v2 ) const;
	uint8 container( GVertex* v1 , GVertex* v2 ) const;
	void  setMark( uint8 m );
	void  setStatus(uint32 s);
	void  addStatus( uint32 s);
	void  clrStatus( uint32 s);
	void  setObj( GBaseObject *pObj);
	void  setObj1(GBaseObject *pObj);
	void  setObj2(GBaseObject *pObj);
	void  setVtx1(GVertex* vtx);
	void  setVtx2(GVertex* vtx);
	void  setSno1(uint8 sno);
	void  setSno2(uint8 sno);
protected:
	uint8		 m_mark ;
	uint32		 m_vtx1 ;
	GVertex*	 m_objVtx1;
	uint32		 m_vtx2 ;
	GVertex*	 m_objVtx2;
	uint32		 m_status;
	GBaseObject* m_obj ;
	GBaseObject* m_obj1;
	GBaseObject* m_obj2;
	uint8		 m_sno1;
	uint8		 m_sno2;
};

class GDE_EXPORT_DES GEdge:public GEdgeData
{
	friend class GVertex ;
	friend class GTopoGraph  ;

public:
	GEdge();
	~GEdge();
	void clear();
	GEdge* vtxNext1() const;
	GEdge* vtxNext2() const;
	GEdge* vtxPrev1() const;
	GEdge* vtxPrev2() const;
	void   setVtxNext1( GEdge* vtx);
	void   setVtxNext2( GEdge* vtx);
	void   setVtxPrev1( GEdge* vtx);
	void   setVtxPrev2( GEdge* vtx);
	GEdge* vtxNext(uint32 v) const;
	GEdge* vtxPrev(uint32 v) const;
	GEdge* vtxNext(GVertex* v) const;
	GEdge* vtxPrev(GVertex* v) const;
	void   setVtxNext( uint32 v , GEdge* vtx);
	void   setVtxPrev( uint32 v , GEdge* vtx);
	void   setVtxNext( GVertex* v , GEdge* vtx);
	void   setVtxPrev( GVertex* v , GEdge* vtx);
	GEdge* vtxNextO(uint32 v) const;
	GEdge* vtxPrevO(uint32 v) const;
	GEdge* vtxNextO(GVertex* v) const;
	GEdge* vtxPrevO(GVertex* v) const;
protected:
	GEdge*	 m_vtx1_next;
	GEdge*   m_vtx1_prev;
	GEdge*	 m_vtx2_next;
	GEdge*	 m_vtx2_prev;
};

class  GDE_EXPORT_DES GEdgeSet:public GList<GEdge*>
{
public:

	GEdgeSet();
	~GEdgeSet();
	void	clear();

	void	mark_clear();
};

class GDE_EXPORT_DES GVertexData
{
public:
	GVertexData();
	~GVertexData();
	void clear();
	uint8	mark() const;
	uint32	status() const;
	uint32	id() const;
	int32   island() const;
	GBaseObject* obj() const;
	void	setMark( uint8 m );
	void	setStatus( uint32 s);
	void	addStatus( uint32 s);
	void    clrStatus( uint32 s);
	void	setID( uint32 id );
	void	setIs( uint32 is );
	void	setObj( GBaseObject* obj);
protected:
	uint8		 m_mark   ;
	int32		 m_island ;
	uint32		 m_status ;
	uint32 		 m_id  	  ;
	GBaseObject* m_obj    ;
};

class GDE_EXPORT_DES GVertex:public GVertexData
{
public:
	GVertex();
	~GVertex();

	void clear();
	GEdge*		firstItem() const ;
	GEdge*		secondItem() const;
	GEdge*		lastItem() const ;
	void		skipItem(GEdge* edge);
	uint8		isEmpty() const ;
	uint32		count() const;
	void		addTail( GEdge* edge );
	GPOS		firstPos() const;
	GEdge*		nextPos( GPOS& gPos ) const;
	GPOS	 	lastPos() const ;
	GEdge*		prevPos( GPOS& gPos ) const;

protected:
	GEdge*		m_head  ;
	GEdge*		m_tail  ;
	uint32		m_count ;
};

class GDE_EXPORT_DES GVertexSet:public GList<GVertex*>
{
public:
	GVertexSet();
	~GVertexSet();
	void clear();
	GVertex* vertex ( uint32 id ) const ;
	uint8	 exist( uint32 vtx1 , uint32 vtx2 ) const;
	GEdge*	 edge( uint32 vtx1 , uint32 vtx2 ) const ;

	void	 mark_clear();
};

class GDE_EXPORT_DES GTopoGraph
{

private:

	GVertexSet		m_vtxs ;
	GEdgeSet		m_edges;
	uint32			m_isCnt;
	TOPOGRAPH_VISIT	m_visitFunc;
	void*			m_visitData;
	TOPOGRAPH_STATUS m_statusFunc;
	void*			 m_statusData;

public:

	GTopoGraph();
	~GTopoGraph();

	GEdgeSet&	edges()  ;
	GVertexSet&	vertexs() ;

	void		status_travel();

	uint32		islands() const;
	void		clear();
	uint8		canReach(uint32 vtx1,uint32 vtx2);

	void		setVisit(TOPOGRAPH_VISIT visitFunc,void* clientData=NULL);
	void		setStatus(TOPOGRAPH_STATUS statusFunc,void* clientData=NULL);
	void		addVtx( GVertex* vtx );
	void		rmvVtx( GVertex* vtx);
	void		rmvVtx( uint32   id );

	GEdge*		addEdge( uint32 vtx1 , uint32 vtx2 );
	void		rmvEdge( uint32 vtx1 , uint32 vtx2 );
	void		unionVtx( uint32 vtx1 , uint32 vtx2 );
	void		spiltVtx( uint32 vtx1 , uint32 vtx2 );

	void		visit_clear();
	void		visit_dfs();
	void		visit_dfs(GVertex* vtx);
	void		visit_bfs();
	void		visit_bfs(GVertex* vtx);

};

#endif
