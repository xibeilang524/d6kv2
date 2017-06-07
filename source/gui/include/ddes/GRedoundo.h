#ifndef _GREDOUNDO_H
#define _GREDOUNDO_H

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"
#include "ddef/GPoint.h"

#include "GLineStyle.h"
#include "GFillStyle.h"
#include "GTextStyle.h"
#include "GObjectManage.h"

class GTree;
class GGraphLayer;

enum
{
	actAdd   ,
	actDel   ,
	actMove  ,
	actSize  ,
	actRotate,
	actMirror,

	actAlign ,
	actEqual ,
	actPen   ,
	actBrush ,
	actFont	 ,

	actUnion ,
	actSplit ,

	actLayerAdd,
	actLayerDel,
	actLayerSet,

	actMacro	
};

enum 
{
	LUF_COLOR,
	LUF_WIDTH,
	LUF_STYLE,
	LUF_LEFTA,
	LUF_RIGHTA
};

enum
{
	BUF_STYLE,
	BUF_RASTER,
	BUF_PATTERN,
	BUF_IMGMODE,
	BUF_IMGSRC,
	BUF_FORECOLOR,
	BUF_BACKCOLOR
};

enum
{
	FUF_FAMILY,
	FUF_STYLE,
	FUF_SIZE,
	FUF_BKCOLOR,
	FUF_TXCOLOR,
	FUF_ALIGN
};

class GDE_EXPORT_DES GCommand
{
public:

	GCommand( GObjectManage* obj ) ;
	virtual ~GCommand();
	void  clear();

	GObjectManage *object() const ;

	GTree*	tree() const;
	GTree*	topo() const;

	void	setTree(GTree *t);
	void	setTopo(GTree *t);
	uint8   compare(GCommand *cmd );

	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;

	virtual int32 type() const =0;
	virtual void  redo() = 0;
	virtual void  undo() =0;

protected:

	void  delObjs(GObjectManage& objs);
	void  addObjs(GObjectManage& objs);
private:

	GObjectManage* m_obj  ;
	GTree*		   m_tree ;
	GTree*		   m_topo ;
};

class GDE_EXPORT_DES GCommandAdd:public GCommand
{
public:
	GCommandAdd( GObjectManage *obj );
	virtual ~GCommandAdd();
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();
};

class GDE_EXPORT_DES GCommandDel:public GCommand
{
public:
	GCommandDel( GObjectManage *obj );
	virtual ~GCommandDel();
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();
};

class GDE_EXPORT_DES GCommandMove:public GCommand
{
public:
	GCommandMove( GObjectManage *obj , FLOAT8 x , FLOAT8 y , uint32 flags=0 );
	virtual ~GCommandMove();
	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();
private:
	FLOAT8 m_x , m_y ;
	uint32 m_flags;
};

class GDE_EXPORT_DES GCommandSize:public GCommand
{
public:
	GCommandSize( GObjectManage *obj , const GPoint& pt , \
		FLOAT8 x , FLOAT8 y, uint32 flags=0);
	virtual ~GCommandSize();
	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();
private:
	FLOAT8 m_x , m_y ;
	GPoint m_pt ;
	uint32 m_flags;
};

class GDE_EXPORT_DES GCommandAlign:public GCommand
{
public:
	GCommandAlign(GObjectManage *obj, uint32 op , FLOAT8 v);
	GCommandAlign(GObjectManage *obj, uint32 op);
	virtual ~GCommandAlign();

	virtual void merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd);
	virtual int32 type() const ;
	virtual void redo();
	virtual void undo();

private:
	uint32 m_op ;
	FLOAT8 m_v  ;
	uint8  m_bv ;
	GObjectManage *m_copy;
};

class GDE_EXPORT_DES GCommandEqual:public GCommand
{
public:
	GCommandEqual(GObjectManage *obj, uint32 op , FLOAT8 w , FLOAT8 h);
	GCommandEqual(GObjectManage *obj, FLOAT8 w , FLOAT8 h);
	GCommandEqual(GObjectManage *obj, uint32 op );
	virtual ~GCommandEqual();

	virtual void merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd);
	virtual int32 type() const ;
	virtual void redo();
	virtual void undo();

private:
	uint32 m_op ;
	FLOAT8 m_w , m_h  ;
	uint8  m_bwh ;
	GObjectManage *m_copy;
};

class GDE_EXPORT_DES GCommandRotate:public GCommand
{
public:
	GCommandRotate( GObjectManage *obj , const GPoint& ptc , \
		FLOAT8 th , uint32 flags=0);
	virtual ~GCommandRotate();
	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();
private:
	GPoint m_ptc  ;
	FLOAT8 m_th   ;
	uint32 m_flags;
};

class GDE_EXPORT_DES GCommandMirror:public GCommand
{
public:
	GCommandMirror( GObjectManage *obj , const GPoint& pto ,\
		uint32 op ,uint32 flags=0);
	virtual ~GCommandMirror();

	virtual int32 type() const;
	virtual void redo();
	virtual void undo();
private:
	GPoint m_pto  ;
	uint32 m_op   ;
	uint32 m_flags;
};

class GDE_EXPORT_DES GCommandPen:public GCommand
{
public:
	GCommandPen( GObjectManage *obj , const GLineStyle& style , int32 f);
	virtual ~GCommandPen();

	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;

	virtual int32 type() const;
	virtual void  redo();
	virtual void  undo();
private:
	int32		m_flag;
	GLineStyle	m_style    ;
	GListLINES  m_oldStyle ;
};

class GDE_EXPORT_DES GCommandBrush:public GCommand
{
public:
	GCommandBrush( GObjectManage *obj , const GFillStyle& style, int32 f );
	virtual ~GCommandBrush();

	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:
	int32		m_flag;
	GFillStyle	m_style    ;
	GListFILLS  m_oldStyle ;
};

class GDE_EXPORT_DES GCommandFont:public GCommand
{
public:
	GCommandFont( GObjectManage *obj, const GTextStyle& style, int32 f );
	virtual ~GCommandFont();

	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;

	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:
	int32		m_flag	   ;
	GTextStyle	m_style    ;
	GListTEXTS  m_oldStyle ;

};

class GDE_EXPORT_DES GCommandUnion:public GCommand
{
public:

	GCommandUnion(GObjectManage *obj , GComObject* comObject);
	virtual ~GCommandUnion();
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:
	GComObject *m_comObject ;
};

class GDE_EXPORT_DES GCommandSplit:public GCommand
{
public:

	GCommandSplit( GObjectManage *obj , GComObject* comObject);
	virtual ~GCommandSplit();
	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:
	GComObject *m_comObject ;
};

class GDE_EXPORT_DES GCommandLayerSet:public GCommand
{
public:

	GCommandLayerSet( GObjectManage *obj , GGraphLayer* layer);
	virtual ~GCommandLayerSet();
	virtual void  merge(GCommand *cmd);
	virtual uint8 canMerge(GCommand *cmd) ;

	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:
	GGraphLayer* m_layer ;
	GListLAYERS  m_oldLayer ;
};

class GDE_EXPORT_DES GCommandLayerAdd:public GCommand
{
public:

	GCommandLayerAdd( GGraphLayer *layer );
	virtual ~GCommandLayerAdd();

	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:

	GGraphLayer* m_layer ;
};

class GDE_EXPORT_DES GCommandLayerDel:public GCommand
{
public:

	GCommandLayerDel( GGraphLayer *layer );
	virtual ~GCommandLayerDel();

	virtual int32 type() const;
	virtual void redo();
	virtual void undo();

private:

	GGraphLayer* m_layer ;
};

class GDE_EXPORT_DES GCommandMacroData:public GList<GCommand*>
{
public:

	GCommandMacroData();
	~GCommandMacroData();

	void  add( GCommand *cmd );
};

class  GDE_EXPORT_DES GCommandMacro:public GCommand
{
public:

	GCommandMacro( GCommandMacroData *cmdMacro );
	virtual ~GCommandMacro();
	virtual int32 type() const;

	virtual void redo();
	virtual void undo();

private:

	GCommandMacroData *m_cmdMacro ;

};

class  GDE_EXPORT_DES GCommandStack:public GList<GCommand*>
{
public:
	GCommandStack();
	~GCommandStack();
	void 		push( GCommand *cmd );
	GCommand* 	pop();
	GCommand*	look();
};

class  GDE_EXPORT_DES GCommandHistory
{
public:
	GCommandHistory();
	~GCommandHistory();
	void add( GCommand *cmd );
	uint8 canUndo();
	uint8 canRedo();
	uint8 undo();
	uint8 redo();
	void clear();
	void clearRedo();
	void clearUndo();

private:
	GCommandStack  m_stkRedo ;
	GCommandStack  m_stkUndo ;
};

#endif
